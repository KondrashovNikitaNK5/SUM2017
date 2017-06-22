/* FILE NAME: U_FIGHTER2.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "anim.h"

/* Animation unit representation type */
typedef struct tagnk5UNIT_FIGHTER
{
  NK5_UNIT_BASE_FIELDS
  nk5OBJ Fighter;/* Fighter model */
  DBL RotateY;
  VEC Pos;
} nk5UNIT_FIGHTER;

/* Fighter drawing unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_FIGHTER *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitInit( nk5UNIT_FIGHTER *Uni, nk5ANIM *Ani )
{
  NK5_RndObjLoad(&Uni->Fighter, "TIE-fighter.g3dm");
  Uni->Fighter.M= MatrMulMatr(MatrTranslate(VecSet(-2000, 1000, 0)), MatrScale(VecSet1(0.1)));
} /* End of 'NK5_UnitInit' function */

/* Fighter drawing unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_FIGHTER *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitClose( nk5UNIT_FIGHTER *Uni, nk5ANIM *Ani )
{
  NK5_RndObjFree(&Uni->Fighter);
} /* End of 'NK5_UnitClose' function */

/* Fighter drawing unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_FIGHTER *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitResponse( nk5UNIT_FIGHTER *Uni, nk5ANIM *Ani )
{
  
} /* End of 'NK5_UnitResponse' function */

/* Fighter drawing unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_FIGHTER *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitRender( nk5UNIT_FIGHTER *Uni, nk5ANIM *Ani )
{
  NK5_RndObjDraw(&Uni->Fighter, MatrMulMatr(Uni->Fighter.M ,MatrRotateY(Ani->Time * 102)));
} /* End of 'NK5_UnitRender' function */

/* Fighter drawing unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (nk5UNIT *) pointer to created unit.
 */
nk5UNIT * NK5_UnitCreateFighter2( VOID )
{
  nk5UNIT_FIGHTER *Uni;

  /* Memory allocation */
  if ((Uni = (nk5UNIT_FIGHTER *)NK5_AnimUnitCreate(sizeof(nk5UNIT_FIGHTER))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)NK5_UnitInit;
  Uni->Close = (VOID *)NK5_UnitClose;
  Uni->Response = (VOID *)NK5_UnitResponse;
  Uni->Render = (VOID *)NK5_UnitRender;
  return (nk5UNIT *)Uni;
} /* End of 'NK5_UnitCreateFighter' function */

/* END OF 'U_FIGHTER2.C' FILE */