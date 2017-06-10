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
} /* End of 'VG4_UnitInit' function */
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
} /* End of 'VG4_UnitClose' function */

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
} /* End of 'VG4_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitRender( nk5UNIT *Uni, nk5ANIM *Ani )
{
} /* End of 'VG4_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
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
} /* End of 'VG4_AnimUnitCreate' function */