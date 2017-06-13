#include <string.h>

#include "anim.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

/* Joystic axis value determination */
#define NK5_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min - 1) - 1)

nk5ANIM NK5_Anim;
INT NK5_MouseWheel;

static UINT64 NK5_StartTime, NK5_OldTime, NK5_OldTimeFPS, NK5_PauseTime, NK5_TimePerSec, NK5_FrameCounter;

VOID NK5_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;

  memset(&NK5_Anim, 0, sizeof(nk5ANIM));

  /* Store window and create memory device context */
  NK5_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  NK5_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  QueryPerformanceFrequency(&t);
  NK5_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  NK5_StartTime = NK5_OldTime = NK5_OldTimeFPS = t.QuadPart;
  NK5_PauseTime = 0;
  NK5_FrameCounter = 0;

  NK5_Anim.IsPause = FALSE;
  NK5_Anim.FPS = 50;
  NK5_AnimAddUnit(NK5_UnitCreateControl());
  NK5_AnimAddUnit(NK5_UnitCreateCow());
  NK5_RndInit();
} /* End of 'NK5_AnimInit' function */

VOID NK5_AnimClose( VOID )
{
  INT i;

  /* Destroy all units */
  for (i = 0; i < NK5_Anim.NumOfUnits; i++)
  {
    NK5_Anim.Units[i]->Close(NK5_Anim.Units[i], &NK5_Anim);
    free(NK5_Anim.Units[i]);
  }
  NK5_Anim.NumOfUnits = 0;

  /* Delete GDI data */
  DeleteDC(NK5_Anim.hDC);
  DeleteObject(NK5_Anim.hFrame);

  memset(&NK5_Anim, 0, sizeof(nk5ANIM));
} /* End of 'NK5_AnimClose' function */

VOID NK5_AnimResize( INT W, INT H )
{
  HDC hDC;

  /* Store new window frame size */
  NK5_Anim.W = W;
  NK5_Anim.H = H;

  /* Create double buffer image */
  if (NK5_Anim.hFrame != NULL)
    DeleteObject(NK5_Anim.hFrame);
  hDC = GetDC(NK5_Anim.hWnd);
  NK5_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(NK5_Anim.hWnd, hDC);

  SelectObject(NK5_Anim.hDC, NK5_Anim.hFrame);
  NK5_RndSetProj();
} /* End of 'NK5_AnimResize' function */

VOID NK5_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, NK5_Anim.W, NK5_Anim.H, NK5_Anim.hDC, 0, 0, SRCCOPY);
} /* End of 'NK5_AnimCopyFrame' function */

VOID NK5_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER t;
  POINT pt;

  /*** Send response to all units ***/
  for (i = 0; i < NK5_Anim.NumOfUnits; i++)
    NK5_Anim.Units[i]->Response(NK5_Anim.Units[i], &NK5_Anim);

  /*** Clear frame ***/
  /* Clear background */
  SelectObject(NK5_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(NK5_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(NK5_Anim.hDC, RGB(100, 155, 220));
  Rectangle(NK5_Anim.hDC, 0, 0, NK5_Anim.W + 1, NK5_Anim.H + 1);

  /*** Render all units ***/
  for (i = 0; i < NK5_Anim.NumOfUnits; i++)
  {
    SelectObject(NK5_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(NK5_Anim.hDC, GetStockObject(NULL_BRUSH));
    SetDCPenColor(NK5_Anim.hDC, RGB(0, 255, 0));
    SetDCBrushColor(NK5_Anim.hDC, RGB(255, 255, 255));

    NK5_Anim.Units[i]->Render(NK5_Anim.Units[i], &NK5_Anim);
  }

  /*** Handle timer ***/
  NK5_FrameCounter++;
  QueryPerformanceCounter(&t);
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

  GetKeyboardState(NK5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    NK5_Anim.Keys[i] >>= 7;
    NK5_Anim.KeysClick[i] = NK5_Anim.Keys[i] && !NK5_Anim.KeysOld[i];
  }
  memcpy(NK5_Anim.KeysOld, NK5_Anim.Keys, 256);

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
} /* End of 'NK5_AnimRender' function */

VOID NK5_AnimAddUnit( nk5UNIT *Uni )
{
  if (NK5_Anim.NumOfUnits < NK5_MAX_UNITS)
  {
    NK5_Anim.Units[NK5_Anim.NumOfUnits] = Uni;
    NK5_Anim.Units[NK5_Anim.NumOfUnits]->Init(NK5_Anim.Units[NK5_Anim.NumOfUnits], &NK5_Anim);
    NK5_Anim.NumOfUnits++;
  }
}

VOID NK5_AnimDoExit( VOID )
{
  static BOOL IsExit = FALSE;

  if(IsExit)
    return;
  IsExit = TRUE;
  PostMessage(NK5_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'NK5_AnimAddUnit' function */

VOID NK5_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(NK5_Anim.hWnd, HWND_NOTOPMOST, SaveRect.left, SaveRect.top, SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top, SWP_NOOWNERZORDER);
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

    SetWindowPos(NK5_Anim.hWnd, HWND_TOPMOST, rc.left, rc.top, rc.right - rc. left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'NK5_AnimFlipFullScreen' function */

/* END OF 'ANIM.C' FILE */