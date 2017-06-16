/* FILE NAME: U_STAT.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "anim.h"

/* Animation unit representation type */
typedef struct tagnk5UNIT_GROUND
{
  NK5_UNIT_BASE_FIELDS
  nk5PRIM Ground;/* Ground model */
  VEC Pos;
  DBL RotateY;
} nk5UNIT_GROUND;

/* Cow drawing unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_GROUND *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitInit( nk5UNIT_GROUND *Uni, nk5ANIM *Ani )
{
  NK5_RndPrimCreatePlane(&Uni->Ground, VecSet(-50, 0, -50), VecSet(0, 0, 100), VecSet(100, 0, 0), 400, 400);
} /* End of 'NK5_UnitInit' function */

/* Cow drawing unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_COW *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitClose( nk5UNIT_GROUND *Uni, nk5ANIM *Ani )
{
  NK5_RndPrimFree(&Uni->Ground);
} /* End of 'NK5_UnitClose' function */

/* Cow drawing unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_COW *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitResponse( nk5UNIT_GROUND *Uni, nk5ANIM *Ani )
{
  Uni->RotateY += Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON] * Ani->Mdx * 200;
} /* End of 'NK5_UnitResponse' function */

/* Cow drawing unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_COW *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitRender( nk5UNIT_GROUND *Uni, nk5ANIM *Ani )
{
  NK5_RndPrimDraw(&Uni->Ground, MatrMulMatr(MatrRotateY(Uni->RotateY), MatrTranslate(VecSet1((DBL)Ani->Mz / 100))));
} /* End of 'NK5_UnitRender' function */

/* Cow drawing unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (nk5UNIT *) pointer to created unit.
 */
nk5UNIT * NK5_UnitCreateGround( VOID )
{
  nk5UNIT_GROUND *Uni;

  /* Memory allocation */
  if ((Uni = (nk5UNIT_GROUND *)NK5_AnimUnitCreate(sizeof(nk5UNIT_GROUND))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)NK5_UnitInit;
  Uni->Close = (VOID *)NK5_UnitClose;
  Uni->Response = (VOID *)NK5_UnitResponse;
  Uni->Render = (VOID *)NK5_UnitRender;
  return (nk5UNIT *)Uni;
} /* End of 'NK5_UnitCreateGround' function */

/* END OF 'U_STAT.C' FILE */