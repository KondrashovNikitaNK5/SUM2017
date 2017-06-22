/* FILE NAME: RENDER.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#include "anim.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")
#pragma comment(lib, "openal32")
#pragma comment(lib, "alut")

/* Project parameters */
DBL
  NK5_RndProjDist, /* Distance from viewer to project plane */
  NK5_RndProjFarClip,
  NK5_RndProjSize; /* Prohect plane inner size */

MATR
  NK5_RndMatrView, /* Viewer matrix */
  NK5_RndMatrProj;

VEC
  NK5_RndLightPos,
  NK5_RndLightColor;

/* Materials table */
nk5MATERIAL NK5_RndMaterials[NK5_RES_TABLE_SIZE];
INT NK5_RndNumOfMaterials;

/* Textures table */
nk5TEXTURE NK5_RndTextures[NK5_RES_TABLE_SIZE];
INT NK5_RndNumOfTextures;

/* Shaders table */
nk5SHADER NK5_RndShaders[NK5_RES_TABLE_SIZE];
INT NK5_RndNumOfShaders;

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */

VOID NK5_RndInit( VOID )
{
  NK5_RndProjDist = 1;
  NK5_RndProjSize = 1;
  NK5_RndProjFarClip = 2500;

  NK5_RndMatrView = MatrView(VecSet1(23), VecSet1(0), VecSet(0, 1, 0));

  NK5_RndLightPos = VecSet(0, 0, 0);
  NK5_RndLightColor = VecSet(0.5, 0.5, 0.5);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.5, 0.7, 1);

  alutInit(NULL, NULL);
  alGetError();

  NK5_RndResInit();
} /* End of 'NK5_RndInit' function */

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndSetProj( VOID )
{
  DBL rx = NK5_RndProjSize / 2, ry = NK5_RndProjSize / 2;

  if (NK5_Anim.W > NK5_Anim.H)
    rx *= (DBL)NK5_Anim.W / NK5_Anim.H;
  else
    ry *= (DBL)NK5_Anim.H / NK5_Anim.W;

  NK5_RndMatrProj = MatrFrustum(-rx, rx, -ry, ry, NK5_RndProjDist, NK5_RndProjFarClip);
} /* End of 'NK5_RndSetProj' function */

VOID NK5_RndClose( VOID )
{
  NK5_RndResClose();
}

/* END OF 'RENDER.C' FILE */