//#include "anim.h"
//
//typedef struct tagnk5UNIT_CLOCK
//{
//  NK5_UNIT_BASE_FIELDS;
//  VEC Pos;
//} nk5UNIT_CLOCK;
//
//* Unit ball initialization function.
// * ARGUMENTS:
// *   - self-pointer to unit object:
// *       nk5UNIT_BALL *Uni;
// *   - animation context:
// *       nk5ANIM *Ani;
// * RETURNS: None.
// */
//static VOID NK5_UnitInit( nk5UNIT_BALL *Uni, nk5ANIM *Ani )
//{
//  Uni->Pos = VecSet(0, 1, 0);
//} /* End of 'NK5_UnitInit' function */
//
//* Unit render function.
// * ARGUMENTS:
// *   - self-pointer to unit object:
// *       nk5UNIT_BALL *Uni;
// *   - animation context:
// *       nk5ANIM *Ani;
// * RETURNS: None.
// */
//static VOID NK5_UnitRender( nk5UNIT_BALL *Uni, nk5ANIM *Ani )
//{
//  DrawSphere(Uni->Pos, 111);
//} /* End of 'NK5_UnitRender' function */
//
//* Unit ball creation function.
// * ARGUMENTS: None.
// * RETURNS:
// *   (nk5UNIT *) pointer to created unit.
// */
//nk5UNIT * NK5_UnitCreateBall( VOID )
//{
//  nk5UNIT_BALL *Uni;
//
//  if ((Uni = (nk5UNIT_BALL *)NK5_AnimUnitCreate(sizeof(nk5UNIT_BALL))) == NULL)
//    return NULL;
//  /* Setup unit methods */
//  Uni->Init = NK5_UnitInit;
//  Uni->Render = NK5_UnitRender;
//  return (nk5UNIT *)Uni;
//} /* End of 'NK5_UnitCreateBall' function */
//