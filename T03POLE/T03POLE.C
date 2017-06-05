/* FILE NAME: T03POLE.C
 * PROGRAMMER: NK5
 * DATE: 05.06.2017
 * PURPOSE: main.
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "pole.h"

#pragma warning (disable: 4244)

#define WND_CLASS_NAME "My window class"
#define defx 100
#define defy 100

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
  HDC hDC;
  POINT pt;
  PAINTSTRUCT ps;
  INT x, y;
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
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    /* draw background */
    SelectObject(hMemDC, GetStockObject(GRAY_BRUSH));
    Rectangle(hMemDC, -1, -1, w + 1, h + 1);
    InvalidateRect(hWnd, NULL, FALSE);
    SelectObject(hMemDC, GetStockObject(NULL_BRUSH));
    for (x = 0; x < w; x += 42)
      for (y = 0; y < h; y += 42)
        DrawArrow(hMemDC, x, y, pt.x, pt.y);

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

/* END OF 'T03POLE.C' FILE */