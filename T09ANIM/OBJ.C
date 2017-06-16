/* FILE NAME: OBJ.C
 * PROGRAMMER: NK5
 * DATE: 14.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Create object function.
 * ARGUMENTS:
 *   - created object:
 *       nk5OBJ *Obj;
 *   - number of primitives:
 *       INT NumOfP;
 * RETURNS: None.
 */
VOID NK5_RndObjCreate( nk5OBJ *Obj, INT NumOfP )
{
  memset(Obj, 0, sizeof(nk5PRIM));
  if ((Obj->P = malloc(sizeof(nk5PRIM) * NumOfP)) == NULL)
    return;
  memset(Obj->P, 0, sizeof(nk5PRIM) * NumOfP);
  Obj->NumOfP = NumOfP;
} /* Ens of 'NK5_RndObjCreate' function */

/* Free object function.
 * ARGUMENTS:
 *   - deleted object:
 *       nk5OBJ *Obj;
 * RETURNS: None.
 */
VOID NK5_RndObjFree( nk5OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    NK5_RndPrimFree(&Obj->P[i]);
  free(Obj->P);
  memset(Obj, 0, sizeof(nk5PRIM));
} /* Ens of 'NK5_RndObjFree' function */

/* Draw primitive function.
 * ARGUMENTS:
 *   - drawing object:
 *       nk5OBJ *Obj;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID NK5_RndObjDraw( nk5OBJ *Obj, MATR M )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    NK5_RndPrimDraw(&Obj->P[i], M);
} /* Ens of 'NK5_RndObjDraw' function */

/* END OF 'OBJ.C' FILE */
