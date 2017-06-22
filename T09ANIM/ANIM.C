/* FILE NAME: ANIM.C
 * PROGRAMMER: NK5
 * DATE: 09.06.2017
 * PURPOSE: Creating animation library.
 */

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include "anim.h"

#pragma comment(lib, "winmm")

#define NK5_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min - 1) - 1)

nk5ANIM NK5_Anim;
INT NK5_MouseWheel;

static UINT64
  NK5_StartTime,    /* Start program time */
  NK5_OldTime,      /* Time from program start to previous frame */
  NK5_OldTimeFPS,   /* Old time FPS measurement */
  NK5_PauseTime,    /* Time during pause period */
  NK5_TimePerSec,   /* Timer resolution */
  NK5_FrameCounter; /* Frames counter */

/* animation's initialisation */
BOOL NK5_AnimInit( HWND hWnd )
{
  LARGE_INTEGER t;
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&NK5_Anim, 0, sizeof(nk5ANIM));

  NK5_Anim.hWnd = hWnd;
  NK5_Anim.hDC = GetDC(hWnd);

  /* Timer initialization */
  QueryPerformanceFrequency(&t);
  NK5_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  NK5_StartTime = NK5_OldTime = NK5_OldTimeFPS = t.QuadPart;
  NK5_PauseTime = 0;
  NK5_FrameCounter = 0;

  /* Store window and create memory device context */
  NK5_Anim.IsPause = FALSE;
  NK5_Anim.FPS = 50;
  NK5_AnimAddUnit(NK5_UnitCreateControl());

  /* OpenGL initialization: setup pixel format */
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(NK5_Anim.hDC, &pfd);
  DescribePixelFormat(NK5_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(NK5_Anim.hDC, i, &pfd);

  /* OpenGL initialization: setup rendering context */
  NK5_Anim.hGLRC = wglCreateContext(NK5_Anim.hDC);
  wglMakeCurrent(NK5_Anim.hDC, NK5_Anim.hGLRC);
  if (glewInit() != GLEW_OK || !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(NK5_Anim.hGLRC);
    ReleaseDC(NK5_Anim.hWnd, NK5_Anim.hDC);
    return FALSE;
  }

  NK5_RndInit();

  return TRUE;
}

/* animation's deinitialisation */
VOID NK5_AnimClose( VOID )
{
  INT i;
  
  for (i = 0; i < NK5_Anim.NumOfUnits; i++)
  {  
    NK5_Anim.Units[i]->Close(NK5_Anim.Units[i], &NK5_Anim);
    free(NK5_Anim.Units[i]);
  }

  NK5_Anim.NumOfUnits = 0;

  NK5_RndClose();

  /* Delete OpenGL data */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(NK5_Anim.hGLRC);
  /* Delete GDI data */
  ReleaseDC(NK5_Anim.hWnd, NK5_Anim.hDC);

  memset(&NK5_Anim, 0, sizeof(nk5ANIM));
}

/* change frame's size */
VOID NK5_AnimResize( INT W, INT H )
{
  NK5_Anim.W = W;
  NK5_Anim.H = H;

  glViewport(0, 0, W, H);

  NK5_RndSetProj();
}

/* copy frame */
VOID NK5_AnimCopyFrame( VOID )
{
  SwapBuffers(NK5_Anim.hDC);
}

/* build frame*/
VOID NK5_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER t;
  POINT pt;
  static DBL ShdTime;
  
  /*** Handle timer ***/
  NK5_FrameCounter++;                    /* increment frame counter (for FPS) */
  QueryPerformanceCounter(&t);           /* obtain current timer value */
  /* Global time */
  NK5_Anim.GlobalTime = (DBL)(t.QuadPart - NK5_StartTime) / NK5_TimePerSec;
  NK5_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - NK5_OldTime) / NK5_TimePerSec;
  /* Time with pause */
  if (NK5_Anim.IsPause)
  {
    NK5_PauseTime += t.QuadPart - NK5_OldTime;
    NK5_Anim.DeltaTime = 0;
  }
  else
  {
    NK5_Anim.Time = (DBL)(t.QuadPart - NK5_PauseTime - NK5_StartTime) / NK5_TimePerSec;
    NK5_Anim.DeltaTime = NK5_Anim.GlobalDeltaTime;
  }
  /* FPS */
  if (t.QuadPart - NK5_OldTimeFPS > NK5_TimePerSec)
  {
    NK5_Anim.FPS = (DBL)NK5_FrameCounter * NK5_TimePerSec / (t.QuadPart - NK5_OldTimeFPS);
    NK5_OldTimeFPS = t.QuadPart;
    NK5_FrameCounter = 0;
  }
  NK5_OldTime = t.QuadPart;

  /* Keyboard */
  GetKeyboardState(NK5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    NK5_Anim.Keys[i] >>= 7;
    NK5_Anim.KeysClick[i] = NK5_Anim.Keys[i] && !NK5_Anim.KeysOld[i];
  }
  memcpy(NK5_Anim.KeysOld, NK5_Anim.Keys, 256);

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(NK5_Anim.hWnd, &pt);
  NK5_Anim.Mdx = pt.x - NK5_Anim.Mx;
  NK5_Anim.Mdy = pt.y - NK5_Anim.My;
  NK5_Anim.Mx = pt.x;
  NK5_Anim.My = pt.y;

  NK5_Anim.Mdz = NK5_MouseWheel;
  NK5_Anim.Mz += NK5_MouseWheel;
  NK5_MouseWheel = 0;

  /*** Joystick handle ***/
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(JOYCAPS)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Axes */
        NK5_Anim.Jx = NK5_GET_JOYSTICK_AXIS(X);
        NK5_Anim.Jy = NK5_GET_JOYSTICK_AXIS(Y);
        NK5_Anim.Jz = NK5_GET_JOYSTICK_AXIS(Z);
        NK5_Anim.Jr = NK5_GET_JOYSTICK_AXIS(R);

        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          NK5_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          NK5_Anim.JButClick[i] = NK5_Anim.JBut[i] && !NK5_Anim.JButOld[i];
          memcpy(NK5_Anim.JButOld, NK5_Anim.JBut, 32);
        }

        /* Point of view */
        NK5_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

  for (i = 0; i < NK5_Anim.NumOfUnits; i++)
    NK5_Anim.Units[i]->Response(NK5_Anim.Units[i], &NK5_Anim);

  /*** Update shader ***/
  if (NK5_Anim.GlobalTime - ShdTime > 2)
  {
    INT i;

    for (i = 0; i < NK5_RndNumOfShaders; i++)
    {
      NK5_RndShaderFree(NK5_RndShaders[i].ProgId);
      NK5_RndShaders[i].ProgId = NK5_RndShaderLoad(NK5_RndShaders[i].Name);
    }
    ShdTime = NK5_Anim.GlobalTime;
  }

  /*** Clear frame ***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (i = 0; i < NK5_Anim.NumOfUnits; i++)
  {
    NK5_Anim.Units[i]->Render(NK5_Anim.Units[i], &NK5_Anim);
  }

  glFinish();
}

/* close animation */
VOID NK5_AnimDoExit( VOID )
{
  static BOOL IsExist = FALSE;

  if(IsExist)
    return;
  IsExist = TRUE;
  PostMessage(NK5_Anim.hWnd, WM_CLOSE, 0, 0);
}

/* add new animation object*/
VOID NK5_AnimAddUnit( nk5UNIT *Uni )
{
  if(NK5_Anim.NumOfUnits < NK5_MAX_UNITS)
  {
    NK5_Anim.Units[NK5_Anim.NumOfUnits] = Uni;
    NK5_Anim.Units[NK5_Anim.NumOfUnits]->Init(NK5_Anim.Units[NK5_Anim.NumOfUnits], &NK5_Anim);
    NK5_Anim.NumOfUnits++;
  }
}

/* Set/reset full screen mode function */
VOID NK5_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(NK5_Anim.hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    /* Store window old size */
    GetWindowRect(NK5_Anim.hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(NK5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(NK5_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(NK5_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */

/* end of 'Anim.C' file '*/