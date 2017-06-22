#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitInit( nk5UNIT *Uni, nk5ANIM *Ani )
{
} /* End of 'NK5_UnitInit' function */
/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitClose( nk5UNIT *Uni, nk5ANIM *Ani )
{
} /* End of 'NK5_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitResponse( nk5UNIT *Uni, nk5ANIM *Ani )
{
} /* End of 'NK5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitRender( nk5UNIT *Uni, nk5ANIM *Ani )
{
} /* End of 'NK5_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (nk5UNIT *) pointer to created unit.
 */
nk5UNIT * NK5_AnimUnitCreate( INT Size )
{
  nk5UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(nk5UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = NK5_UnitInit;
  Uni->Close = NK5_UnitClose;
  Uni->Response = NK5_UnitResponse;
  Uni->Render = NK5_UnitRender;
  return Uni;
} /* End of 'NK5_AnimUnitCreate' function */