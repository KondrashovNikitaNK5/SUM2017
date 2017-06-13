/* FILE NAME: RENDER.H
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

/* Project parameters */
extern DBL
  NK5_RndWp,       /* Project plane width */
  NK5_RndHp,       /* Project plane height */
  NK5_RndProjDist, /* Distance from viewer to project plane */
  NK5_RndProjSize; /* Prohect plane inner size */

extern MATR
  NK5_RndMatrView, /* Viewer matrix */
  NK5_RndMatrProj;
/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndInit( VOID );

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndSetProj( VOID );

/***
 * Object handle
 ***/

/* Object description type */
typedef struct tagnk5OBJ3D
{
  VEC *V;      /* Vertex array */
  INT NumOfV;  /* Vertex array size */
  INT (*F)[3]; /* Facets array (point indices) */
  INT NumOfF;  /* Facets array size */
} nk5OBJ3D;

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL NK5_RndObjLoad( nk5OBJ3D *Obj, CHAR *FileName );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ3D *Obj;
 * RETURNS: None.
 */
VOID NK5_RndObjFree( nk5OBJ3D *Obj );

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID NK5_RndObjDraw( nk5OBJ3D *Obj, MATR M );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */