/*==========================================================================================================================================================

														カメラ処理[camera.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXVECTOR3 rot;			//向き
	float		fLength;		//視点と注視点の距離
	D3DXMATRIX	mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX	mtxview;		//ビューマトリックス
}Camera;

void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

//カメラの移動処理
void MoveCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot);
void MovePosV(void);
void SpinPosV(void);
void ResetCameraPos(void);
void FixCameraRot(void);
void SetCamera(void);
Camera *GetCamera(void);
void LengthCamera(void);

#endif 