
#ifndef UNITS_H
#define UNITS_H

#include "def.h"

#define NK5_UNIT_BASE_FIELDS \
  VOID (*Init) (nk5UNIT *Uni, nk5ANIM *Ani); \
  VOID (*Close) (nk5UNIT *Uni, nk5ANIM *Ani); \
  VOID (*Response) (nk5UNIT *Uni, nk5ANIM *Ani); \
  VOID (*Render) (nk5UNIT *Uni, nk5ANIM *Ani);

#endif 