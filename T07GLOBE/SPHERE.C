/* FILE NAME: SPHERE.C
 * PROGRAMMER: NK5
 * DATE: 07.06.2017
 * PURPOSE: drawing sphere.
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sphere.h"

#pragma warning (disable: 4244)

#define N 50
#define M 100

static VEC G[N][M];

VOID BuildSphere( DBL R )
{
  DBL theta, phi, x, y, z;
  INT i, j;
  for (theta = 0, i = 0; i < N; i++, theta += PI / (N - 1))
    for (phi = 0, j = 0; j < M; j++, phi += 2 * PI / (M - 1))
    {
      x = R * sin(theta) * sin(phi);
      y = R * cos(theta);
      z = R * sin(theta) * cos(phi);

      G[i][j].x = R * sin(theta) * sin(phi);
      G[i][j].y = R * cos(theta);
      G[i][j].z = R * sin(theta) * cos(phi);
    }
}

VOID DrawSphere( HDC hDC, INT w, INT h )
{
  DBL size = 1, wp = size, hp = size, xs, ys, t = clock() / (DBL)CLOCKS_PER_SEC, xp, yp;
  INT i, j;

  if (w > h)
    wp *= (DBL)w / h;
  else
    hp *= (DBL)h / w;

  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
  SelectObject(hDC, GetStockObject(WHITE_PEN));
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      xp = G[i][j].x * cos(t) - G[i][j].y * sin(t);
      yp = G[i][j].x * sin(t) + G[i][j].y * cos(t);

      xs = xp * w / wp + w / 2;
      ys = h / 2 - yp * h / hp;
      Ellipse(hDC, xs - 3, ys - 3, xs + 3, ys + 3);
    }
  SelectObject(hDC, GetStockObject(NULL_BRUSH));
  SelectObject(hDC, GetStockObject(NULL_PEN));
}

/* END OF 'SPHERE.C' FILE */