/* FILE NAME: U_CONTR.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "anim.h"

/* Animation unit representation type */
typedef struct tagnk5UNIT_CONTROL
{
  NK5_UNIT_BASE_FIELDS
  HFONT hFont;         /* Using font */
} nk5UNIT_CONTROL;

/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_CONTROL *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitInit( nk5UNIT_CONTROL *Uni, nk5ANIM *Ani )
{
} /* End of 'NK5_UnitInit' function */

/* Control unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_CONTROL *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitClose( nk5UNIT_CONTROL *Uni, nk5ANIM *Ani )
{
  DeleteObject(Uni->hFont);
} /* End of 'NK5_UnitClose' function */

/* Control unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_CONTROL *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitResponse( nk5UNIT_CONTROL *Uni, nk5ANIM *Ani )
{
  if (Ani->KeysClick[VK_ESCAPE])
    NK5_AnimDoExit();
  else if (Ani->KeysClick['F'])
    NK5_AnimFlipFullScreen();
  else if (Ani->KeysClick['P'])
    NK5_Anim.IsPause = !NK5_Anim.IsPause;
} /* End of 'NK5_UnitResponse' function */

/* Control unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       nk5UNIT_CONTROL *Uni;
 *   - animation context:
 *       nk5ANIM *Ani;
 * RETURNS: None.
 */
static VOID NK5_UnitRender( nk5UNIT_CONTROL *Uni, nk5ANIM *Ani )
{
  INT len;
  static CHAR Buf[100];

  len = sprintf(Buf, "FPS: %.5f, Units: %d, Wheel: %d"
    "J: %.3f %.3f %.3f %.3f",
    Ani->FPS, Ani->NumOfUnits, Ani->Mz,
    Ani->Jx, Ani->Jy, Ani->Jz, Ani->Jr);

  SetWindowText(Ani->hWnd, Buf);
} /* End of 'NK5_UnitRender' function */

/* Control unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (nk5UNIT *) pointer to created unit.
 */
nk5UNIT * NK5_UnitCreateControl( VOID )
{
  nk5UNIT_CONTROL *Uni;

  /* Memory allocation */
  if ((Uni = (nk5UNIT_CONTROL *)NK5_AnimUnitCreate(sizeof(nk5UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)NK5_UnitInit;
  Uni->Close = (VOID *)NK5_UnitClose;
  Uni->Response = (VOID *)NK5_UnitResponse;
  Uni->Render = (VOID *)NK5_UnitRender;
  return (nk5UNIT *)Uni;
} /* End of 'NK5_UnitCreateControl' function */

/* END OF 'U_CONTR.C' FILE */