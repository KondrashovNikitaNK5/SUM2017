/* FILE NAME: SPHERE.H
 * PROGRAMMER: NK5
 * DATE: 07.06.2017
 * PURPOSE: prototype.
 */

#ifndef __GLOBE_H_
#define __GLOBE_H_

#include <windows.h>

#define PI 3.14159265359
#define INT BOOL

typedef double DBL;
typedef struct tagVEC
{
  DBL x, y, z;
} VEC;

/* Matrix representation type */
typedef struct tagMATR
{
  DBL A[3][3];
} MATR;


VOID BuildSphere( DBL R );
VOID DrawSphere( HDC hDC, INT w, INT h);

#endif __GLOBE_H_

/* END OF 'SPHERE.H' FILE */