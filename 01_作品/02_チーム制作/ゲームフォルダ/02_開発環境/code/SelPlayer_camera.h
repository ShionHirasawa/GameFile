/*==========================================================================================================================================================

カメラの処理[SelPlayer_camera.h]
Author:大宮愛羅  平澤詩苑  石原颯馬

============================================================================================================================================================*/
#ifndef _SELPLAYER_CAMERA_H_
#define _SELPLAYER_CAMERA_H_

#include "main.h"

//カメラの構造体
typedef struct
{
	D3DXVECTOR3		posV;			// 視点
	D3DXVECTOR3		posR;			// 注視点
	D3DXVECTOR3		vecU;			// 上方向ベクトル
	D3DXVECTOR3		rot;			// 向き
	float			fLength;		// 距離
	float			fMaxLength;		// 最大描画距離
	D3DXMATRIX		mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX		mtxView;		// ビューマトリックス
	D3DVIEWPORT9	viewport;		// ビューポート
	bool			bUse;			// 使用されているかいないか
} Camera;

//プロトタイプ宣言
void InitSelPlayer_SetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR);	//カメラの位置設定
void InitSelPlayer_Camera(void);
void UninitSelPlayer_Camera(void);				//終了処理
void UpdateSelPlayer_Camera(void);				//更新処理
void SetSelPlayer_Camera(void);				//設定処理
void MoveSelPlayer_Camera(void);		//カメラの移動処理

void UpdateSelPlayer_PosVCamera(void);	//視点の位置更新
Camera *GetSelPlayer_Camera(void);				//カメラの取得

//=========================================
//カメラの台数別　設定処理
//=========================================
void TPS_SelPlayer_ChaseCamera(int nCntCamera, D3DXVECTOR3 rot);

void SelPlayer_CameraForTitle(void);//タイトル用カメラ処理

#endif