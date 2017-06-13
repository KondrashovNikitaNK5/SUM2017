/* FILE NAME: RENDER.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#include "anim.h"


/* Project parameters */
DBL
  NK5_RndProjDist, /* Distance from viewer to project plane */
  NK5_RndProjFarClip,
  NK5_RndProjSize; /* Prohect plane inner size */

MATR
  NK5_RndMatrView, /* Viewer matrix */
  NK5_RndMatrProj;

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndInit( VOID )
{
  NK5_RndProjDist = 1;
  NK5_RndProjSize = 1;
  NK5_RndProjFarClip = 1000;

  NK5_RndMatrView = MatrView(VecSet1(23), VecSet1(0), VecSet(0, 1, 0));
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

/* END OF 'RENDER.C' FILE */