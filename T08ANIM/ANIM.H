#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "units.h"

#define NK5_MAX_UNITS 50

typedef struct tagnk5ANIM nk5ANIM;
typedef struct tagnk5UNIT nk5UNIT;

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
};

VOID NK5_AnimInit( HWND hWnd );
VOID NK5_AnimClose( VOID );
VOID NK5_AnimResize( INT W, INT H );
VOID NK5_AnimCopyFrame( HDC hDC );
VOID NK5_AnimRender( VOID );
VOID NK5_AnimAddUnit( nk5UNIT *Uni );
VOID NK5_AnimDoExit( VOID );
VOID NK5_AnimFlipFullScreen( VOID );

#endif