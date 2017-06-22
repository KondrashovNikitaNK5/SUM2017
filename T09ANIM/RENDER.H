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

/* OpenAL specific includes */
#include <al.h>
#include <alc.h>
#include <alut.h>

#include "def.h"

/***
 * Material support
 ***/

#define NK5_RES_TABLE_SIZE 1000

#define NK5_MATERIAL_NAME_SIZE 300

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

extern VEC
  NK5_RndLightPos,
  NK5_RndLightColor;

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
  INT MtlNo;
} nk5PRIM;

typedef struct tagnk5OBJ
{
  nk5PRIM *P;
  INT NumOfP;
  MATR M;
} nk5OBJ  ;

/* Shader store representation type */
typedef struct tagnk5SHADER
{
  CHAR Name[NK5_MATERIAL_NAME_SIZE]; /* Shader file name prefix */
  UINT ProgId;                       /* Shader program ID */
} nk5SHADER;

/* Texture store representation type */
typedef struct tagnk5TEXTURE
{
  CHAR Name[NK5_MATERIAL_NAME_SIZE]; /* Texture file name */
  INT W, H;                          /* Texture image size */
  UINT TexId;                        /* Texture ID */
} nk5TEXTURE;

/* Material store representation type */
typedef struct tagnk5MATERIAL
{
  CHAR Name[NK5_MATERIAL_NAME_SIZE]; /* Material name */

  /* Illumination coefficients */ 
  VEC Ka, Kd, Ks;                    /* Ambient, diffuse, specular coefficients */
  FLT Ph;                            /* Phong power coefficient */
  FLT Trans;                         /* Transparency factor */
  INT Tex[8];                        /* Texture references */

  /* Shader string */
  CHAR ShaderStr[NK5_MATERIAL_NAME_SIZE];
  INT Shader;                        /* Texture references */
} nk5MATERIAL;

/* Materials table */
extern nk5MATERIAL NK5_RndMaterials[NK5_RES_TABLE_SIZE];
extern INT NK5_RndNumOfMaterials;

/* Textures table */
extern nk5TEXTURE NK5_RndTextures[NK5_RES_TABLE_SIZE];
extern INT NK5_RndNumOfTextures;

/* Shaders table */
extern nk5SHADER NK5_RndShaders[NK5_RES_TABLE_SIZE];
extern INT NK5_RndNumOfShaders;

VEC LightPos;

VOID NK5_RndPrimCreate( nk5PRIM *Pr, BOOL IsTrimesh, nk5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
VOID NK5_RndPrimFree( nk5PRIM *Pr );
VOID NK5_RndPrimDraw( nk5PRIM *Pr, MATR M );
BOOL NK5_RndPrimLoad( nk5PRIM *Obj, CHAR *FileName );
VOID NK5_RndTriMeshEvalNormals( nk5VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
VOID NK5_RndGridEvalNormals( nk5VERTEX *V, INT N, INT M );
VOID NK5_RndPrimCreateSphere( nk5PRIM *Pr, VEC C, FLT R, INT N, INT M );
VOID NK5_RndPrimCreatePlane( nk5PRIM *Pr, VEC C, VEC Du, VEC Dv, INT N, INT M );
VOID NK5_RndClose( VOID );

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

/* Resourcse initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndResInit( VOID );

/* Resourcse deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NK5_RndResClose( VOID );

/* Add shader function.
 * ARGUMENTS:
 *   - shader file name prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader table number.
 */
INT NK5_RndShaderAdd( CHAR *FileNamePrefix );

/* Add material function.
 * ARGUMENTS:
 *   - material data:
 *       nk5MATERIAL *Mtl;
 * RETURNS:
 *   (INT) material table number.
 */
INT NK5_RndMaterialAdd( nk5MATERIAL *Mtl );

/* Add texture function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture image size:
 *       INT W, H;
 *   - texture image pixel number of component:
 *       INT Components;
 *   - texture image pixel color data:
 *       VOID *Data;
 * RETURNS:
 *   (INT) texture table number.
 */
INT NK5_RndTextureAdd( CHAR *Name, INT W, INT H, INT Components, VOID *Data );

/* Material apply function.
 * ARGUMENTS:
 *   - material table number:
 *       INT MtlNo;
 * RETURNS:
 *   (UINT) shader program Id.
 */
UINT NK5_RndMaterialApply( INT MtlNo );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */