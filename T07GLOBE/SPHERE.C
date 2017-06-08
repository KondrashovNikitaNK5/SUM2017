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

MATR MatrRotate( DBL AngleInDegree, VEC R )
{
  DBL a = AngleInDegree * PI / 180, sine = sin(a), cosine = cos(a);
  MATR m;

  a = sqrt(R.x * R.x + R.y * R.y + R.z * R.z);
  R.x /= a;
  R.y /= a;
  R.z /= a;

  m.A[0][0] = cosine + R.x * R.x * (1 - cosine);
  m.A[0][1] = R.x * R.y * (1 - cosine) - R.z * sine;
  m.A[0][2] = R.x * R.z * (1 - cosine) + R.y * sine;

  m.A[1][0] = R.y * R.x * (1 - cosine) + R.z * sine;
  m.A[1][1] = cosine + R.y * R.y * (1 - cosine);
  m.A[1][2] = R.y * R.z * (1 - cosine) - R.x * sine;

  m.A[2][0] = R.z * R.x * (1 - cosine) - R.y * sine;
  m.A[2][1] = R.z * R.y * (1 - cosine) + R.x * sine;
  m.A[2][2] = cosine + R.z * R.z * (1 - cosine);
  return m;
} /* End of 'MatrRotate' function */

/* Vector transform function */
VEC VecTransform( VEC V, MATR Matrix )
{
  VEC P =
  {
    V.x * Matrix.A[0][0] + V.y * Matrix.A[0][1] + V.z * Matrix.A[0][2],
    V.x * Matrix.A[1][0] + V.y * Matrix.A[1][1] + V.z * Matrix.A[1][2],
    V.x * Matrix.A[2][0] + V.y * Matrix.A[2][1] + V.z * Matrix.A[2][2]
  };

  return P;
} /* End of 'VecTransform' function */

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
  DBL size = 1, wp = size, hp = size, t = clock() / (DBL)CLOCKS_PER_SEC, xp, yp;
  INT i, j;
  VEC vector = {1, 2, 3};
  POINT Pts[4];

  if (w > h)
    wp *= (DBL)w / h;
  else
    hp *= (DBL)h / w;

  SelectObject(hDC, GetStockObject(NULL_BRUSH));
  SelectObject(hDC, GetStockObject(WHITE_PEN));
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      G[i][j] = VecTransform(G[i][j], MatrRotate(t * 20, vector));

      xp = G[i][j].x;
      yp = G[i][j].y;

      if (i > 0 && j > 0)
      {
        Pts[0].x = G[i - 1][j - 1].x * w / wp + w / 2;
        Pts[0].y = h / 2 - G[i - 1][j - 1].y * h / hp;
        Pts[1].x = G[i - 1][j].x * w / wp + w / 2;
        Pts[1].y = h / 2 - G[i - 1][j].y * h / hp;
        Pts[2].x = G[i][j].x * w / wp + w / 2;
        Pts[2].y = h / 2 - G[i][j].y * h / hp;
        Pts[3].x = G[i][j - 1].x * w / wp + w / 2;
        Pts[3].y = h / 2 - G[i][j - 1].y * h / hp;
        if ((Pts[0].x - Pts[1].x) * (Pts[0].y + Pts[1].y) + (Pts[1].x - Pts[2].x) * (Pts[1].y + Pts[2].y) + (Pts[2].x - Pts[3].x) * (Pts[2].y + Pts[3].y) + (Pts[3].x - Pts[0].x) * (Pts[3].y + Pts[0].y) > 0)
          Polygon(hDC, Pts, 4);
      }
    }
  SelectObject(hDC, GetStockObject(NULL_PEN));
}

/* END OF 'SPHERE.C' FILE */