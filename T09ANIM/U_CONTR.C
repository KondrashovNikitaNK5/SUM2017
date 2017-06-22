/* FILE NAME: U_CONTR.C
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "anim.h"


DBL px = 0, py = 0, pz = 0, rt = PI / 2;
/* Animation unit representation type */
typedef struct tagnk5UNIT_CONTROL
{
  NK5_UNIT_BASE_FIELDS
  DBL HRot, VRot, X, Y, Z;
  DBL Dist;
  INT ABuf[2], ASrc[2];
} nk5UNIT_CONTROL;

/* Sound */

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
  INT format;
  UINT size, freq;
  CHAR loop;
  VOID *mem;

  /* Create sound buffers */
  alGenBuffers(2, Uni->ABuf);

  alutLoadWAVFile("mus.wav", &format, &mem, &size, &freq, &loop);
  alBufferData(Uni->ABuf[0], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  alutLoadWAVFile("mus.wav", &format, &mem, &size, &freq, &loop);
  alBufferData(Uni->ABuf[1], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  /* Create sound sources */
  alGenSources(2, Uni->ASrc);

  /* Bind buffer to source */
  alSourcei(Uni->ASrc[0], AL_BUFFER, Uni->ABuf[0]);
  /* alSourcef(Uni->ASrc[0], AL_PITCH, 0.50); */
  alSourcef(Uni->ASrc[0], AL_GAIN, 0.30);
  alSourcei(Uni->ASrc[0], AL_LOOPING, 1);

  alSourcei(Uni->ASrc[1], AL_BUFFER, Uni->ABuf[1]);
  alSourcef(Uni->ASrc[1], AL_PITCH, 1.00);
  alSourcef(Uni->ASrc[1], AL_GAIN, 1);
  alSourcei(Uni->ASrc[1], AL_LOOPING, 0);

  alSourcePlay(Uni->ASrc[1]);
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
  alSourceStop(Uni->ASrc[0]);
  alDeleteBuffers(2, Uni->ASrc);
  alDeleteSources(2, Uni->ASrc);
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
  VEC V;

  if (Ani->KeysClick[VK_ESCAPE])
    NK5_AnimDoExit();
  else if (Ani->KeysClick['F'])
    NK5_AnimFlipFullScreen();
  else if (Ani->KeysClick['P'])
    NK5_Anim.IsPause = !NK5_Anim.IsPause;
  else if (Ani->KeysClick['E'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->JButClick[0])
    alSourcePlay(Uni->ASrc[1]);
  if (Ani->KeysClick['M'])
    alSourcePause(Uni->ASrc[0]);
  if (Ani->KeysClick['N'])
    alSourcePlay(Uni->ASrc[0]);

  if (Ani->Keys['W'])
  {
    px += 90 * Ani->GlobalDeltaTime * 1.8 * cos(rt);
    pz += 90 * Ani->GlobalDeltaTime * 1.8 * sin(rt);
  }
  if (Ani->Keys['A'])
  {
    px += 90 * Ani->GlobalDeltaTime * 1.8 * sin(rt);
    pz -= 90 * Ani->GlobalDeltaTime * 1.8 * cos(rt);
  }
  if (Ani->Keys['S'])
  {
    px -= 90 * Ani->GlobalDeltaTime * 1.8 * cos(rt);
    pz -= 90 * Ani->GlobalDeltaTime * 1.8 * sin(rt);
  }
  if (Ani->Keys['D'])
  {
    px -= 90 * Ani->GlobalDeltaTime * 1.8 * sin(rt);
    pz += 90 * Ani->GlobalDeltaTime * 1.8 * cos(rt);
  }
  if (Ani->Keys[VK_CONTROL])
  {
    py -= 90 * Ani->GlobalDeltaTime * 1.2;
  }
  if (Ani->Keys[VK_SPACE])
  {
    py += 90 * Ani->GlobalDeltaTime * 1.2;
  }
  if (Ani->Mdx != 0)
    rt += Ani->GlobalDeltaTime * Ani->Mdx * Ani->Keys[VK_LBUTTON];

  NK5_RndMatrView = MatrView(VecSet(px, py, pz), VecSet(px + cos(rt), py, pz + sin(rt)), VecSet(0, 1, 0));

  /*Uni->Dist += Ani->Mdz / 120;
  Uni->HRot -= 8 * 30 * Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON] * Ani->Mdx;
  Uni->VRot -= 8 * 30 * Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON] * Ani->Mdy;
  Uni->X += 240 * Ani->GlobalDeltaTime * (Ani->Keys['S'] - Ani->Keys['W']);
  Uni->Z += 240 * Ani->GlobalDeltaTime * (Ani->Keys['A'] - Ani->Keys['D']);
  if (Uni->VRot > 89)
    Uni->VRot = 89;
  if (Uni->VRot < -89)
    Uni->VRot = -89;
  
  V = VecSet(0, 4, Uni->Dist);
  V = VecMulMatr43(V, MatrMulMatr(MatrTranslate(VecSet(Uni->X, Uni->Y, Uni->Z)), MatrMulMatr(MatrRotateX(Uni->VRot), MatrRotateY(Uni->HRot))));
  NK5_RndMatrView = MatrView(V, VecSet(0, 0, 0), VecSet(0, 1, 0));*/
  
  NK5_RndLightPos = VecAddVec(NK5_RndLightPos, VecMulNum(VecSet(Ani->Jx, Ani->Jy, Ani->Jz), 8 * Ani->GlobalDeltaTime));
  if (Ani->JBut[2])
    NK5_RndLightPos = VecSet(30000, 0, 0);
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