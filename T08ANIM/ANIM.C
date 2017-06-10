#include <string.h>

#include "anim.h"

nk5ANIM NK5_Anim;

VOID NK5_AnimInit( HWND hWnd )
{
  HDC hDC;

  memset(&NK5_Anim, 0, sizeof(nk5ANIM));

  /* Store window and create memory device context */
  NK5_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  NK5_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
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
} /* End of 'NK5_AnimResize' function */

VOID NK5_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, NK5_Anim.W, NK5_Anim.H, NK5_Anim.hDC, 0, 0, SRCCOPY);
} /* End of 'NK5_AnimCopyFrame' function */

VOID NK5_AnimRender( VOID )
{
  INT i;

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