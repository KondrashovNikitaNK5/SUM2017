/* FILE NAME: OBJ.C
 * PROGRAMMER: NK5
 * DATE: 14.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Create object function.
 * ARGUMENTS:
 *   - created object:
 *       nk5OBJ *Obj;
 *   - number of primitives:
 *       INT NumOfP;
 * RETURNS: None.
 */
VOID NK5_RndObjCreate( nk5OBJ *Obj, INT NumOfP )
{
  memset(Obj, 0, sizeof(nk5OBJ));
  if ((Obj->P = malloc(sizeof(nk5PRIM) * NumOfP)) == NULL)
    return;
  memset(Obj->P, 0, sizeof(nk5PRIM) * NumOfP);
  Obj->NumOfP = NumOfP;
} /* Ens of 'NK5_RndObjCreate' function */

/* Free object function.
 * ARGUMENTS:
 *   - deleted object:
 *       nk5OBJ *Obj;
 * RETURNS: None.
 */
VOID NK5_RndObjFree( nk5OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    NK5_RndPrimFree(&Obj->P[i]);
  free(Obj->P);
  memset(Obj, 0, sizeof(nk5OBJ));
} /* Ens of 'NK5_RndObjFree' function */

/* Draw object function.
 * ARGUMENTS:
 *   - drawing object:
 *       nk5OBJ *Obj;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID NK5_RndObjDraw( nk5OBJ *Obj, MATR M )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    NK5_RndPrimDraw(&Obj->P[i], M);
} /* Ens of 'NK5_RndObjDraw' function */

/* Load object from "G3DM" file function.
 * ARGUMENTS:
 *   - drawing object:
 *       nk5OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL NK5_RndObjLoad( nk5OBJ *Obj, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NoofP = 0, NoofM = 0, NoofT = 0, i;

  memset(Obj, 0, sizeof(nk5OBJ));

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Check signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3DM")
  {
    fclose(F);
    return FALSE;
  }

  /* Read quantities */
  fread(&NoofP, 4, 1, F);
  fread(&NoofM, 4, 1, F);
  fread(&NoofT, 4, 1, F);

  if (NK5_RndNumOfMaterials + NoofM >= NK5_RES_TABLE_SIZE ||
      NK5_RndNumOfTextures + NoofT >= NK5_RES_TABLE_SIZE)
  {
    fclose(F);
    return FALSE;
  }

  /* Create object */
  NK5_RndObjCreate(Obj, NoofP);

  /* Load primitives */
  for (i = 0; i < NoofP; i++)
  {
    INT nv = 0, ni = 0, mtl_no = -1, size;
    nk5VERTEX *V;
    INT *I;

    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    fread(&mtl_no, 4, 1, F);
    size = sizeof(nk5VERTEX) * nv + sizeof(INT) * ni;
    if ((V = malloc(size)) == NULL)
    {
      fclose(F);
      NK5_RndObjFree(Obj);
      return FALSE;
    }
    fread(V, 1, size, F);
    I = (INT *)(V + nv);
    NK5_RndPrimCreate(&Obj->P[i], TRUE, V, nv, I, ni);
    free(V);
    if (mtl_no != -1)
      mtl_no += NK5_RndNumOfMaterials;
    Obj->P[i].MtlNo = mtl_no;
  }

  /* Load materials */
  fread(&NK5_RndMaterials[NK5_RndNumOfMaterials], sizeof(nk5MATERIAL), NoofM, F);
  for (i = 0; i < NoofM; i++)
  {
    INT j;
    nk5MATERIAL *mtl = &NK5_RndMaterials[NK5_RndNumOfMaterials + i];

    if (mtl->ShaderStr[0] != 0)
      mtl->Shader = NK5_RndShaderAdd(mtl->ShaderStr);

    for (j = 0; j < 8; j++)
      if (mtl->Tex[j] != -1)
        mtl->Tex[j] += NK5_RndNumOfTextures;
  }
  NK5_RndNumOfMaterials += NoofM;

  /* Load textures */
  for (i = 0; i < NoofT; i++)
  {
    CHAR Name[300];
    INT W, H, C;
    VOID *mem;

    fread(Name, 1, 300, F);
    fread(&W, 4, 1, F);
    fread(&H, 4, 1, F);
    fread(&C, 4, 1, F);
    if ((mem = malloc(W * H * C)) == NULL)
    {
      fclose(F);
      NK5_RndObjFree(Obj);
      return FALSE;
    }
    fread(mem, C, W * H, F);
    NK5_RndTextureAdd(Name, W, H, C, mem);
    free(mem);
  }
  NK5_RndNumOfTextures += NoofT;

  fclose(F);
  return TRUE;
} /* Ens of 'NK5_RndObjLoad' function */

/* END OF 'OBJ.C' FILE */
