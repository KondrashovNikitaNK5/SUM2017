/* FILE NAME: MATERIAL.C
 * PROGRAMMER: NK5
 * DATE: 17.06.2017
 * PURPOSE: Material (texture, shader store) handle module.
 */

#include <string.h>

#include "anim.h"

/***
 * Resource storages
 ***/

/* Materials table */
nk5MATERIAL NK5_RndMaterials[NK5_RES_TABLE_SIZE];
INT NK5_RndNumOfMaterials;

/* Textures table */
nk5TEXTURE NK5_RndTextures[NK5_RES_TABLE_SIZE];
INT NK5_RndNumOfTextures;

/* Shaders table */
nk5SHADER NK5_RndShaders[NK5_RES_TABLE_SIZE];
INT NK5_RndNumOfShaders;

/* Resourcse initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndResInit( VOID )
{
  nk5MATERIAL def_mtl =
  {
    "Default material",
    {0.1, 0.1, 0.1}, {0.7, 0.7, 0.7}, {0.3, 0.3, 0.3}, 170,
    1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    "", 0
  };

  memset(NK5_RndMaterials, 0, sizeof(NK5_RndMaterials));
  memset(NK5_RndTextures, 0, sizeof(NK5_RndTextures));
  memset(NK5_RndShaders, 0, sizeof(NK5_RndShaders));
  NK5_RndNumOfMaterials = 0;
  NK5_RndNumOfTextures = 0;
  NK5_RndNumOfShaders = 0;

  NK5_RndShaderAdd("SHADERS\\DEFAULT");
  strcpy(def_mtl.ShaderStr, "SHADERS\\DEFAULT");
  NK5_RndMaterialAdd(&def_mtl);
} /* End of 'NK5_RndResInit' function */

/* Resourcse deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndResClose( VOID )
{
  INT i;

  /* Delete shaders */
  for (i = 0; i < NK5_RndNumOfShaders; i++)
    NK5_RndShaderFree(NK5_RndShaders[i].ProgId);
  memset(NK5_RndShaders, 0, sizeof(NK5_RndShaders));
  NK5_RndNumOfShaders = 0;

  /* Delete textures */
  for (i = 0; i < NK5_RndNumOfTextures; i++)
    glDeleteTextures(1, &NK5_RndTextures[i].TexId);
  memset(NK5_RndTextures, 0, sizeof(NK5_RndTextures));
  NK5_RndNumOfTextures = 0;

  /* Remove materials */
  memset(NK5_RndMaterials, 0, sizeof(NK5_RndMaterials));
  NK5_RndNumOfMaterials = 0;
} /* End of 'NK5_RndResClose' function */

/* Add shader function.
 * ARGUMENTS:
 *   - shader file name prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader table number.
 */
INT NK5_RndShaderAdd( CHAR *FileNamePrefix )
{
  if (NK5_RndNumOfShaders >= NK5_RES_TABLE_SIZE)
    return 0;
  strncpy(NK5_RndShaders[NK5_RndNumOfShaders].Name, FileNamePrefix,
    NK5_MATERIAL_NAME_SIZE - 1);
  NK5_RndShaders[NK5_RndNumOfShaders].ProgId =
    NK5_RndShaderLoad(FileNamePrefix);
  return NK5_RndNumOfShaders++;
} /* End of 'NK5_RndShaderAdd' function */

/* Add material function.
 * ARGUMENTS:
 *   - material data:
 *       nk5MATERIAL *Mtl;
 * RETURNS:
 *   (INT) material table number.
 */
INT NK5_RndMaterialAdd( nk5MATERIAL *Mtl )
{
  if (NK5_RndNumOfMaterials >= NK5_RES_TABLE_SIZE)
    return 0;
  NK5_RndMaterials[NK5_RndNumOfMaterials] = *Mtl;
  return NK5_RndNumOfMaterials++;
} /* End of 'NK5_RndMaterialAdd' function */

/* Add texture function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture image size:
 *       INT W, H;
 *   - texture image pixel number of component:
 *       INT Components;
 *   - texture image pixel color data:
 *       VOID *Data;
 * RETURNS:
 *   (INT) texture table number.
 */
INT NK5_RndTextureAdd( CHAR *Name, INT W, INT H, INT Components, VOID *Data )
{
  if (NK5_RndNumOfTextures >= NK5_RES_TABLE_SIZE)
    return -1;
  strncpy(NK5_RndTextures[NK5_RndNumOfTextures].Name, Name,
    NK5_MATERIAL_NAME_SIZE - 1);
  NK5_RndTextures[NK5_RndNumOfTextures].W = W;
  NK5_RndTextures[NK5_RndNumOfTextures].H = H;

  glGenTextures(1, &NK5_RndTextures[NK5_RndNumOfTextures].TexId);
  glBindTexture(GL_TEXTURE_2D, NK5_RndTextures[NK5_RndNumOfTextures].TexId);
  gluBuild2DMipmaps(GL_TEXTURE_2D, Components, W, H,
    Components == 4 ? GL_BGRA_EXT : Components == 3 ? GL_BGR_EXT : GL_LUMINANCE,
    GL_UNSIGNED_BYTE, Data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return NK5_RndNumOfTextures++;
} /* End of 'NK5_RndTextureAdd' function */

/* Material apply function.
 * ARGUMENTS:
 *   - material table number:
 *       INT MtlNo;
 * RETURNS:
 *   (UINT) shader program Id.
 */
UINT NK5_RndMaterialApply( INT MtlNo )
{
  nk5MATERIAL *mtl;
  INT prg, loc, i;

  if (MtlNo < 0 || MtlNo >= NK5_RndNumOfMaterials)
    MtlNo = 0;
  mtl = &NK5_RndMaterials[MtlNo];

  /* Enable shader */
  prg = mtl->Shader;
  if (prg < 0 || prg >= NK5_RndNumOfShaders)
    prg = 0;
  prg = NK5_RndShaders[prg].ProgId;

  glUseProgram(prg);

  /* Illumination coefficients */
  loc = glGetUniformLocation(prg, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  loc = glGetUniformLocation(prg, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  loc = glGetUniformLocation(prg, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  loc = glGetUniformLocation(prg, "Ph");
  if (loc != -1)
    glUniform1f(loc, mtl->Ph);
  loc = glGetUniformLocation(prg, "Trans");
  if (loc != -1)
    glUniform1f(loc, mtl->Trans);

  /* Textures */
  for (i = 0; i < 8; i++)
  {
    CHAR ist[] = "IsTexture0";
    INT t = mtl->Tex[i];

    /* Set texture usage flag */
    ist[9] = '0' + i;
    loc = glGetUniformLocation(prg, ist);
    if (loc != -1)
      glUniform1i(loc, t != -1);

    /* Select texture to sampler */
    if (t != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, NK5_RndTextures[t].TexId);
    }
  }
  return prg;
} /* End of 'NK5_RndMaterialApply' function */

/* END OF 'MATERIAL.C' FILE */