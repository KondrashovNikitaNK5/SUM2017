#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "units.h"
#include "render.h"

#define NK5_MAX_UNITS 50

typedef struct tagnk5ANIM nk5ANIM;
typedef struct tagnk5UNIT nk5UNIT;

extern nk5ANIM NK5_Anim;
extern INT NK5_MouseWheel;

struct tagnk5UNIT
{
  NK5_UNIT_BASE_FIELDS
};

struct tagnk5ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;
  HBITMAP hFrame;
  nk5UNIT *Units[NK5_MAX_UNITS];
  INT NumOfUnits;
  DBL GlobalTime, GlobalDeltaTime, Time, DeltaTime, FPS;
  BOOL IsPause;
  BYTE Keys[256];
  BYTE KeysOld[256];
  BYTE KeysClick[256];
  INT
    Mx, My, Mz,     /* Absolute coordinates */
    Mdx, Mdy, Mdz;  /* Relative coordinates shift */
  BYTE
    JBut[32],                    /* Joystick button state */
    JButOld[32],                 /* Joystick button old state */
    JButClick[32];               /* Joystick button click info */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL
    Jx, Jy, Jz, Jr;              /* Joystick axes */

};

VOID NK5_AnimInit( HWND hWnd );
VOID NK5_AnimClose( VOID );
VOID NK5_AnimResize( INT W, INT H );
VOID NK5_AnimCopyFrame( HDC hDC );
VOID NK5_AnimRender( VOID );
VOID NK5_AnimAddUnit( nk5UNIT *Uni );
VOID NK5_AnimDoExit( VOID );
VOID NK5_AnimFlipFullScreen( VOID );

nk5UNIT * NK5_AnimUnitCreate( INT Size );
nk5UNIT * NK5_UnitCreateControl( VOID );
nk5UNIT * NK5_UnitCreateCow( VOID );
VOID NK5_AnimAddUnit( nk5UNIT *Uni );

#endif