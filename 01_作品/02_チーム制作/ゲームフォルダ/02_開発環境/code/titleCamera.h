/*==========================================================================================================================================================

リザルト用カメラ処理[resultCamera.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _TITLECAMERA_H_
#define _TITLECAMERA_H_

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
}TitleCamera;

void InitTitleCamera(void);
void UninitTitleCamera(void);
void UpdateTitleCamera(void);

//カメラの移動処理
void MoveTitleCamera(D3DXVECTOR3 ParentPos, D3DXVECTOR3 *pChildPos, float CorRot);
void SpinTitlePosV(void);
void SetTitleCamera(void);
TitleCamera *GetTitleCamera(void);

#endif 