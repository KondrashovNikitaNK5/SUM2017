/* FILE NAME: U_BATTLESHIP.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "anim.h"

/* Animation unit representation type */
typedef struct tagnk5UNIT_BATTLESHIP
{
  NK5_UNIT_BASE_FIELDS
  nk5OBJ Battleship;/* Battleship model */
  DBL RotateY;
  VEC Pos;
} nk5UNIT_BATTLESHIP;

/* Battleship drawing unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_BATTLESHIP *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitInit( nk5UNIT_BATTLESHIP *Uni, nk5ANIM *Ani )
{
  NK5_RndObjLoad(&Uni->Battleship, "imperial.g3dm");
} /* End of 'NK5_UnitInit' function */

/* Battleship drawing unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_BATTLESHIP *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitClose( nk5UNIT_BATTLESHIP *Uni, nk5ANIM *Ani )
{
  NK5_RndObjFree(&Uni->Battleship);
} /* End of 'NK5_UnitClose' function */

/* Battleship drawing unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_BATTLESHIP *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitResponse( nk5UNIT_BATTLESHIP *Uni, nk5ANIM *Ani )
{
  
} /* End of 'NK5_UnitResponse' function */

/* Battleship drawing unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_BATTLESHIP *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitRender( nk5UNIT_BATTLESHIP *Uni, nk5ANIM *Ani )
{
  NK5_RndObjDraw(&Uni->Battleship, MatrScale(VecSet1(0.01)));
} /* End of 'NK5_UnitRender' function */

/* Battleship drawing unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (nk5UNIT *) pointer to created unit.
 */
nk5UNIT * NK5_UnitCreateBattleship( VOID )
{
  nk5UNIT_BATTLESHIP *Uni;

  /* Memory allocation */
  if ((Uni = (nk5UNIT_BATTLESHIP *)NK5_AnimUnitCreate(sizeof(nk5UNIT_BATTLESHIP))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)NK5_UnitInit;
  Uni->Close = (VOID *)NK5_UnitClose;
  Uni->Response = (VOID *)NK5_UnitResponse;
  Uni->Render = (VOID *)NK5_UnitRender;
  return (nk5UNIT *)Uni;
} /* End of 'NK5_UnitCreateBattleship' function */

/* END OF 'U_BATTLESHIP.C' FILE */