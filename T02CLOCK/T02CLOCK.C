/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: NK5
 * DATE: 02.06.2017
 * PURPOSE: drawing clock.
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#pragma warning (disable: 4244)

#define WND_CLASS_NAME "My window class"
#define pi 3.1415926535
#define Rm 150
#define dm 0
#define wm 3
#define Rs 150
#define ds 10
#define ws 1
#define Rh 100
#define dh 0
#define wh 5

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, unsigned Message, WPARAM wParam, LPARAM lParam);
/* Set/reset full screen mode function */
static VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(hWnd, HWND_TOP,
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
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */

/* Main program function */   
INT WINAPI WinMain( HINSTANCE hInstancce, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstancce;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* register class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "...", "...", MB_OK);
    return 0;
  }

  /* create window */
  hWnd = CreateWindow(WND_CLASS_NAME, "Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstancce, NULL);

  /* show and redraw window */
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  /* waiting message about 'WM_QUIT' */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
} /* End of 'WinMain' function */

/* draw hand function */
void DrawHand(INT w, INT h, INT wlength, FLOAT time, INT radius, INT dlength, HDC hDC )
{
  POINT p[4];

  SelectObject(hDC, GetStockObject(BLACK_BRUSH));
  p[0].x = w / 2 + wlength * cos(time) - radius * sin(time);
  p[0].y = h / 2 - wlength * sin(time) - radius * cos(time);
  p[1].x = w / 2 - wlength * cos(time) - radius * sin(time);
  p[1].y = h / 2 + wlength * sin(time) - radius * cos(time);
  p[2].x = w / 2 - wlength * cos(time) + dlength * sin(time);
  p[2].y = h / 2 + wlength * sin(time) + dlength * cos(time);
  p[3].x = w / 2 + wlength * cos(time) + dlength * sin(time);
  p[3].y = h / 2 - wlength * sin(time) + dlength * cos(time);
  Polygon(hDC, p, 4);
  SelectObject(hDC, GetStockObject(NULL_BRUSH));
} /* end 'DrawHand' function */


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  BITMAP bm;
  HDC hDC;
  SYSTEMTIME st;
  FLOAT sec, min, hour;
  PAINTSTRUCT ps;
  CHAR Buf[30];
  HFONT hFnt;
  MINMAXINFO *MinMax;
  static INT w, h;
  static HDC hMemDC, hMemDCLogo;
  static HBITMAP hBm, hBmAND, hBmXOR;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)lParam;
    MinMax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    SetTimer(hWnd, 47, 30, NULL);
    /* Load images */
    hBmXOR = LoadImage(NULL, "XOR.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAND = LoadImage(NULL, "AND.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    FlipFullScreen(hWnd);

    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE) /* press esc = exit*/
      DestroyWindow(hWnd);
    if (wParam == 'F') /* press F = fullscreen */
      FlipFullScreen(hWnd);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    GetLocalTime(&st);
    hFnt  = CreateFont(50, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH | FF_ROMAN, "Times");

    sec = (-st.wSecond - st.wMilliseconds / 1000.0) / 30.0 * pi;
    min = (-st.wMinute - st.wSecond / 60.0) / 30.0 * pi;
    hour = (-st.wHour - st.wMinute / 60.0) / 6.0 * pi;
    /* draw background */
    SelectObject(hMemDC, GetStockObject(GRAY_BRUSH));
    Rectangle(hMemDC, -1, -1, w + 1, h + 1);
    InvalidateRect(hWnd, NULL, FALSE);
    SelectObject(hMemDC, GetStockObject(NULL_BRUSH));
    /* draw sprite */
    GetObject(hBmXOR, sizeof(BITMAP), &bm);
    SetStretchBltMode(hMemDCLogo, COLORONCOLOR);
    SelectObject(hMemDCLogo, hBmAND);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCAND);
    SelectObject(hMemDCLogo, hBmXOR);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCINVERT);
    /* draw second hand */
    DrawHand(w, h, ws, sec, Rs, ds, hMemDC);
    /* draw minute hand */
    DrawHand(w, h, wm, min, Rm, dm, hMemDC);
    /* draw hour hand */
    DrawHand(w, h, wh, hour, Rh, dh, hMemDC);
    /* create font */
    SelectObject(hMemDC, hFnt);
    SetTextColor(hMemDC, RGB(0, 0, 0));
    SetBkColor(hMemDC, RGB(127, 127, 127));
    SetBkMode(hMemDC, TRANSPARENT);
    /* write day.month.year */
    TextOut(hMemDC, w / 2 - 100, h / 2 + 320, Buf, sprintf(Buf, "%i.%i.%i", st.wDay, st.wMonth, st.wYear));
    DeleteObject(hFnt);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' fuction */

/* END OF 'T02CLOCK.C' FILE */