/* FILE NAME: RENDER.H
 * PROGRAMMER: NK5
 * DATE: 10.06.2017
 * PURPOSE: Base rendering system module.
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
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

extern UINT
  NK5_RndProgId;
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

typedef struct tagnk5VERTEX
{
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
} nk5VERTEX;

typedef struct tagnk5PRIM
{
  BOOL IsTrimesh;
  INT NumOfI;
  MATR M;

  INT VA, VBuf;
  INT IBuf;
} nk5PRIM;

typedef struct tagnk5OBJ
{
  nk5PRIM *P;
  INT NumOfP;
} nk5OBJ;

VEC LightPos;

VOID NK5_RndPrimCreate( nk5PRIM *Pr, BOOL IsTrimesh, nk5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
VOID NK5_RndPrimFree( nk5PRIM *Pr );
VOID NK5_RndPrimDraw( nk5PRIM *Pr, MATR M );
BOOL NK5_RndPrimLoad( nk5PRIM *Obj, CHAR *FileName );
VOID NK5_RndTriMeshEvalNormals( nk5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
VOID NK5_RndGridEvalNormals( nk5VERTEX *V, INT N, INT M );
VOID NK5_RndPrimCreateSphere( nk5PRIM *Pr, VEC C, FLT R, INT N, INT M );
VOID NK5_RndPrimCreatePlane( nk5PRIM *Pr, VEC C, VEC Du, VEC Dv, INT N, INT M );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL NK5_RndObjLoad( nk5OBJ *Obj, CHAR *FileName );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ *Obj;
 * RETURNS: None.
 */
VOID NK5_RndObjFree( nk5OBJ *Obj );

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       nk5OBJ *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID NK5_RndObjDraw( nk5OBJ *Obj, MATR M );

UINT NK5_RndShaderLoad( CHAR *FileNamePrefix );

VOID NK5_RndShaderFree( UINT Prg );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */