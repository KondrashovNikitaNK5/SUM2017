/* FILE NAME: OBJ3D.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL NK5_RndObjLoad( nk5OBJ3D *Obj, CHAR *FileName )
{
  INT vn = 0, fn = 0, size;
  FILE *F;
  static CHAR Buf[1000];

  memset(Obj, 0, sizeof(nk5OBJ3D));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;
  }

  /* Memory allocation */
  size = sizeof(VEC) * vn + sizeof(INT [3]) * fn;
  if ((Obj->V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Obj->F = (INT (*)[3])(Obj->V + vn);
  Obj->NumOfV = vn;
  Obj->NumOfF = fn;

  /* Read model data */
  rewind(F);
  vn = 0;
  fn = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Obj->V[vn++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n0, n1, n2;

      if (sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &n0, &n1, &n2) == 3 ||
          sscanf(Buf + 2, "%i %i %i", &n0, &n1, &n2) == 3)
      {
        Obj->F[fn][0] = n0 - 1;
        Obj->F[fn][1] = n1 - 1;
        Obj->F[fn][2] = n2 - 1;
        fn++;
      }
    }
  }

  fclose(F);
  return TRUE;
} /* End of 'NK5_RndObjLoad' function */

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ3D *Obj;
 * RETURNS: None.
 */
VOID NK5_RndObjFree( nk5OBJ3D *Obj )
{
  if (Obj->V != NULL)
    free(Obj->V);
  memset(Obj, 0, sizeof(nk5OBJ3D));
} /* End of 'NK5_RndObjFree function */

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID NK5_RndObjDraw( nk5OBJ3D *Obj, MATR M )
{
  INT i;
  POINT *pts;
  MATR WVP;

  if ((pts = malloc(sizeof(POINT) * Obj->NumOfV)) == NULL)
    return;

  WVP = MatrMulMatr(M, MatrMulMatr(NK5_RndMatrView, NK5_RndMatrProj));

  /* Project all points */
  for (i = 0; i < Obj->NumOfV; i++)
  {
    VEC P = PointTransform(Obj->V[i], WVP);
    
    pts[i].x = (P.X + 1) * NK5_Anim.W / 2;
    pts[i].y = (-P.Y + 1) * NK5_Anim.H / 2;
  }

  /* Draw all facets */
    SelectObject(NK5_Anim.hDC, GetStockObject(NULL_BRUSH));
    SelectObject(NK5_Anim.hDC, GetStockObject(DC_PEN));
    SetDCPenColor(NK5_Anim.hDC, RGB(0, 200, 0));

  for (i = 0; i < Obj->NumOfF; i++)
  {    
    POINT p[3];

    p[0] = pts[Obj->F[i][0]];
    p[1] = pts[Obj->F[i][1]];
    p[2] = pts[Obj->F[i][2]];
    Polygon(NK5_Anim.hDC, p, 3);
  }
  free(pts);
} /* End of 'NK5_RndObjDraw' function */

/* END OF 'OBJ3D.C' FILE */