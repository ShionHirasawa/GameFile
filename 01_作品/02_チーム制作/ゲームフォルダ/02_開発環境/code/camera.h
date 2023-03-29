/*==========================================================================================================================================================

カメラの処理[camera.h]
Author:大宮愛羅  平澤詩苑  石原颯馬

============================================================================================================================================================*/
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define NUM_CAMERA_HALF	(2)			//画面分割が、縦・横　どちらか２分割の場合のカメラの数
#define NUM_CAMERA		(4)			//カメラの数

//描画するカメラの数 列挙
typedef enum
{
	NumCamera_ONLY = 0,			//全体の俯瞰視点
	NumCamera_HALF_SIDE,		//横に分けた２つの視点
	NumCamera_HALF_HIGH_row,	//縦に分けた２つの視点
	NumCamera_FOUR_Separate,	//各プレイヤーを見られる４つの視点
	NumCamera_FOUR_SIDE,
	NumCamera_MAX,
}NumCamera;

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
void InitSetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumCamera);	//カメラの位置設定
void InitCamera(NumCamera type);		//初期化処理
void UninitCamera(void);				//終了処理
void UpdateCamera(void);				//更新処理
void SetCamera(int nIdx);				//設定処理
void MoveCamera(int nCntCamera);		//カメラの移動処理

void SetPosRCamera(int nCntCamera);		//注視点設定
void UpdatePosVCamera(int nCntCamera);	//視点の位置更新
Camera *GetCamera(void);				//カメラの取得

//=========================================
//カメラの台数別　設定処理
//=========================================
void Set_NumCamera(NumCamera type);
void TPS_ChaseCamera(int nCntCamera, D3DXVECTOR3 rot);

void CameraForTitle(void);//タイトル用カメラ処理

#endif