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
#define ds 0
#define ws 1
#define Rh 100
#define dh 0
#define wh 5

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, unsigned Message, WPARAM wParam, LPARAM lParam);
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


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  BITMAP bm;
  HDC hDC;
  SYSTEMTIME st;
  FLOAT t, sec, min, hour;
  PAINTSTRUCT ps;
  POINT pts[4], p[4];
  static INT w, h;
  static HDC hMemDC, hMemDCLogo;
  static HBITMAP hBm, hBmAND, hBmXOR;

  switch (Msg)
  {
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
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    GetLocalTime(&st);

    sec = (-st.wSecond - st.wMilliseconds / 1000.0) / 30.0 * pi;
    min = (-st.wMinute + sec / 60) / 30.0 * pi;
    hour = (-st.wHour + min / 60) / 6.0 * pi;
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

    SelectObject(hMemDC, GetStockObject(BLACK_BRUSH));
    /* draw second hand */
    p[0].x = w / 2 + ws * cos(sec) - Rs * sin(sec);
    p[0].y = h / 2 - ws * sin(sec) - Rs * cos(sec);
    p[1].x = w / 2 - ws * cos(sec) - Rs * sin(sec);
    p[1].y = h / 2 + ws * sin(sec) - Rs * cos(sec);
    p[2].x = w / 2 - ws * cos(sec) + ds * sin(sec);
    p[2].y = h / 2 + ws * sin(sec) + ds * cos(sec);
    p[3].x = w / 2 + ws * cos(sec) + ds * sin(sec);
    p[3].y = h / 2 - ws * sin(sec) + ds * cos(sec);
    Polygon(hMemDC, p, 4);

    /* draw minute hand */
    p[0].x = w / 2 + wm * cos(min) - Rm * sin(min);
    p[0].y = h / 2 - wm * sin(min) - Rm * cos(min);
    p[1].x = w / 2 - wm * cos(min) - Rm * sin(min);
    p[1].y = h / 2 + wm * sin(min) - Rm * cos(min);
    p[2].x = w / 2 - wm * cos(min) + dm * sin(min);
    p[2].y = h / 2 + wm * sin(min) + dm * cos(min);
    p[3].x = w / 2 + wm * cos(min) + dm * sin(min);
    p[3].y = h / 2 - wm * sin(min) + dm * cos(min);
    Polygon(hMemDC, p, 4);

    /* draw hour hand */
    p[0].x = w / 2 + wh * cos(hour) - Rh * sin(hour);
    p[0].y = h / 2 - wh * sin(hour) - Rh * cos(hour);
    p[1].x = w / 2 - wh * cos(hour) - Rh * sin(hour);
    p[1].y = h / 2 + wh * sin(hour) - Rh * cos(hour);
    p[2].x = w / 2 - wh * cos(hour) + dh * sin(hour);
    p[2].y = h / 2 + wh * sin(hour) + dh * cos(hour);
    p[3].x = w / 2 + wh * cos(hour) + dh * sin(hour);
    p[3].y = h / 2 - wh * sin(hour) + dh * cos(hour);
    Polygon(hMemDC, p, 4);

    SelectObject(hMemDC, GetStockObject(NULL_BRUSH));

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