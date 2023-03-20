/*==========================================================================================================================================================

													メッシュシリンダー処理[meshcylinder.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _MESHCYLINDER_H
#define _MESHCYLINDER_H

#include "main.h"

//メッシュシリンダーの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
}MeshCylinder;

//プロトタイプ宣言
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif