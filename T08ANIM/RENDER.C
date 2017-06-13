/* FILE NAME: RENDER.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#include "anim.h"

/* Project parameters */
DBL
  NK5_RndWp,       /* Project plane width */
  NK5_RndHp,       /* Project plane height */
  NK5_RndProjDist, /* Distance from viewer to project plane */
  NK5_RndProjSize; /* Prohect plane inner size */

MATR
  NK5_RndMatrView; /* Viewer matrix */

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndInit( VOID )
{
  NK5_RndWp = 1;
  NK5_RndHp = 1;
  NK5_RndProjDist = 1;
  NK5_RndProjSize = 1;

  NK5_RndMatrView = MatrView(VecSet1(23), VecSet1(1), VecSet(0, 1, 0));
} /* End of 'NK5_RndInit' function */

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndSetProj( VOID )
{
  NK5_RndWp = NK5_RndProjSize;
  NK5_RndHp = NK5_RndProjSize;
  if (NK5_Anim.W > NK5_Anim.H)
    NK5_RndWp *= (DBL)NK5_Anim.W / NK5_Anim.H;
  else
    NK5_RndHp *= (DBL)NK5_Anim.H / NK5_Anim.W;
} /* End of 'NK5_RndSetProj' function */

/* END OF 'RENDER.C' FILE */