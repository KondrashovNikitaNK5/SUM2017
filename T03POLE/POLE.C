/* FILE NAME: POLE.C
 * PROGRAMMER: NK5
 * DATE: 05.06.2017
 * PURPOSE: drawing pole.
 */

#include <stdlib.h>
#include <math.h>
#include <windows.h>

#pragma warning (disable: 4244)

#define wx 50
#define wy 0
#define rx 5
#define ry 0

void DrawArrow( HDC hDC, INT x, INT y, INT posx, INT posy)
{
  POINT p[3];
  INT dx = x - posx, dy = y - posy;
  FLOAT len = sqrt(dx * dx + dy * dy), si = - dy / len, co = dx / len;
  HBRUSH hBr, hOldBr;

  hBr = CreateSolidBrush(RGB(255, 0, 0));
  hOldBr = SelectObject(hDC, hBr);

  SelectObject(hDC, GetStockObject(hBr));
  p[0].x = x - rx * si;
  p[0].y = y - rx * co;
  p[1].x = x - wx * co + rx * si;
  p[1].y = y + wx * si + rx * co;
  p[2].x = x + rx * si;
  p[2].y = y + rx * co;
  Polygon(hDC, p, 3);

  SelectObject(hDC, hOldBr);
  DeleteObject(hBr);

  hBr = CreateSolidBrush(RGB(0, 0, 255));
  hOldBr = SelectObject(hDC, hBr);

  SelectObject(hDC, GetStockObject(hBr));
  p[0].x = x + wx * co - rx * si;
  p[0].y = y - wx * si - rx * co;
  p[1].x = x - rx * si;
  p[1].y = y - rx * co;
  p[2].x = x + rx * si;
  p[2].y = y + rx * co;
  Polygon(hDC, p, 3);

  SelectObject(hDC, hOldBr);
  DeleteObject(hBr);
}

/* END OF 'POLE.C' FILE */