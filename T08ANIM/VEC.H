/* FILE NAME: VEC.H
 * PROGRAMMER: NK5
 * DATE: 09.06.2017
 * PURPOSE: library.
 */

#ifndef __VEC_H_
#define __VEC_H_                    

#include <windows.h>
#include <math.h>

#define PI 3.14159265358979323846
#define INT BOOL
#define D2R(A) ((A) * (PI / 180))
#define Degree2Radian(a) D2R(a)

/* basic double type */
typedef double DBL;

typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* Matrix representation type */
typedef struct tagMATR
{
  DBL M[4][4];
} MATR;

static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

_inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;
  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */

_inline VEC VecSet1( DBL R )
{
  VEC v;
  v.X = R;
  v.Y = R;
  v.Z = R;
  return v;
} /* End of 'VecSet' function */

_inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}

_inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

_inline VEC VecMulNum( VEC V1, DBL N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

_inline VEC VecDivNum( VEC V1, DBL N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

_inline VEC VecNeg( VEC V            )
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

_inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

_inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
}

_inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

_inline DBL VecLen( VEC V )
{
  return sqrt(VecLen2(V));
}

_inline VEC VecNormalize( VEC V )
{
  DBL l = VecLen(V);
  return VecSet(V.X / l, V.Y / l, V.Z / l);
}

_inline VEC VecAddVecEq( VEC *VRes, VEC V2 )
{
  VRes->X += V2.X;
  VRes->Y += V2.Y;
  VRes->Z += V2.Z;
  return *VRes;
}

/* Vector by matrix multiplication (without projection) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr43( VEC V, MATR M )
{
  return VecSet(V.X * M.M[0][0] + V.Y * M.M[1][0] + V.Z * M.M[2][0] + M.M[3][0],
                V.X * M.M[0][1] + V.Y * M.M[1][1] + V.Z * M.M[2][1] + M.M[3][1],
                V.X * M.M[0][2] + V.Y * M.M[1][2] + V.Z * M.M[2][2] + M.M[3][2]);
} /* End of 'VecMulMatr43' function */

/* Vector by matrix multiplication (only orientation) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr3( VEC V, MATR M )
{
  return VecSet(V.X * M.M[0][0] + V.Y * M.M[1][0] + V.Z * M.M[2][0],
                V.X * M.M[0][1] + V.Y * M.M[1][1] + V.Z * M.M[2][1],
                V.X * M.M[0][2] + V.Y * M.M[1][2] + V.Z * M.M[2][2]);
} /* End of 'VecMulMatr3' function */

/* Vector by matrix multiplication (with homogenious devide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M )
{
  DBL w = V.X * M.M[0][3] + V.Y * M.M[1][3] + V.Z * M.M[2][3] + M.M[3][3];

  return VecSet((V.X * M.M[0][0] + V.Y * M.M[1][0] + V.Z * M.M[2][0] + M.M[3][0]) / w,
                (V.X * M.M[0][1] + V.Y * M.M[1][1] + V.Z * M.M[2][1] + M.M[3][1]) / w,
                (V.X * M.M[0][2] + V.Y * M.M[1][2] + V.Z * M.M[2][2] + M.M[3][2]) / w);
} /* End of 'PointTransform' function */
_inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}

_inline MATR MatrTranslate( VEC T )
{
  MATR m = UnitMatrix;

  m.M[3][0] = T.X;
  m.M[3][1] = T.Y;
  m.M[3][2] = T.Z;
  return m;
}

_inline MATR MatrScale( VEC S )
{
  MATR m = UnitMatrix;

  m.M[0][0] = S.X;
  m.M[1][1] = S.Y;
  m.M[2][2] = S.Z;
  return m;
}
_inline MATR MatrRotateX( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a = D2R(AngleInDegree);

  m.M[1][1] = cos(a);
  m.M[1][2] = sin(a);
  m.M[2][1] = -sin(a);
  m.M[2][2] = cos(a);
  return m;
}
_inline MATR MatrRotateY( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a = D2R(AngleInDegree);

  m.M[0][0] = cos(a);
  m.M[0][2] = -sin(a);
  m.M[2][0] = sin(a);
  m.M[2][2] = cos(a);
  return m;
}
_inline MATR MatrRotateZ( DBL AngleInDegree )
{
  MATR m = UnitMatrix;
  DBL a = D2R(AngleInDegree);

  m.M[0][0] = cos(a);
  m.M[0][1] = sin(a);
  m.M[1][0] = -sin(a);
  m.M[1][1] = cos(a);
  return m;
}

_inline MATR MatrRotate( DBL AngleInDegree, VEC R )
{
  DBL A = D2R(AngleInDegree), si = sin(A), co = cos(A);
  VEC V = VecNormalize(R);
  MATR M =
  {
    {
      {co + V.X * V.X * (1 - co),
         V.X * V.Y * (1 - co) - V.Z * si,
           V.X * V.Z * (1 - co) + V.Y * si, 0},
      {V.Y * V.X * (1 - co) + V.Z * si,
         co + V.Y * V.Y * (1 - co),
           V.Y * V.Z + (1 - co) - V.X * si, 0},
      {V.Z * V.X * (1 - co) - V.Y * si,
         V.Z * V.Y * (1 - co) + V.Y * si,
           co + V.Z * V.Z * (1 - co), 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}

_inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;
  int i, j, k; 

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.M[i][j] = 0, k = 0; k < 4; k++)
        r.M[i][j] += M1.M[i][k] * M2.M[k][j];
  return r;
}

_inline MATR MatrTranspose( MATR M )
{
  MATR r;
  int i, j;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.M[i][j] = M.M[j][i];
  return r;
}

_inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                           DBL A21, DBL A22, DBL A23,
                           DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 + A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
}

_inline DBL MatrDeterm( MATR M )
{
  return
    M.M[0][0] * MatrDeterm3x3(M.M[1][1], M.M[1][2], M.M[1][3],
                              M.M[2][1], M.M[2][2], M.M[2][3],
                              M.M[3][1], M.M[3][2], M.M[3][3]) -
    M.M[0][1] * MatrDeterm3x3(M.M[1][0], M.M[1][2], M.M[1][3],
                              M.M[2][0], M.M[2][2], M.M[2][3],
                              M.M[3][0], M.M[3][2], M.M[3][3]) +
    M.M[0][2] * MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][3],
                              M.M[2][0], M.M[2][1], M.M[2][3],
                              M.M[3][0], M.M[3][1], M.M[3][3]) -
    M.M[0][3] * MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][2],
                              M.M[2][0], M.M[2][1], M.M[2][2],
                              M.M[3][0], M.M[3][1], M.M[3][2]);
}

_inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT i, j;
  INT p[4][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};

  if (det == 0)
    return UnitMatrix;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.M[j][i] =
        MatrDeterm3x3(
          M.M[p[i][0]][p[j][0]], M.M[p[i][0]][p[j][1]], M.M[p[i][0]][p[j][2]],
          M.M[p[i][1]][p[j][0]], M.M[p[i][1]][p[j][1]], M.M[p[i][1]][p[j][2]],
          M.M[p[i][2]][p[j][0]], M.M[p[i][2]][p[j][1]], M.M[p[i][2]][p[j][2]]) / 
        det;
  return r;
}

__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {               Right.X,                Up.X,              -Dir.X, 0},
      {               Right.Y,                Up.Y,              -Dir.Y, 0},
      {               Right.Z,                Up.Z,              -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

__inline MATR MatrFrustum( DBL l, DBL r, DBL b, DBL t, DBL n ,DBL f )
{
  MATR m = MatrIdentity();
  m.M[0][0] = 2 * n / (r - l);
  m.M[1][1] = 2 * n / (t - b);
  m.M[2][0] = (r + l) / (r - l);
  m.M[2][1] = (t + b) / (t - b);
  m.M[2][2] = -(f + n) / (f - n);
  m.M[2][3] = -1;
  m.M[3][2] = -2 * n * f / (f - n);
  m.M[3][3] = 0;
  return m;
} /* End of 'MatrFrustum' function */

#endif __VEC_H_

/* END OF 'VEC.H' FILE */