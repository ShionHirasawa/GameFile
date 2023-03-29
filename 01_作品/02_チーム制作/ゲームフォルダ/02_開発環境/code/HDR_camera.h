/*==========================================================================================================================================================

カメラの処理[camera.h]
Author:大宮愛羅  平澤詩苑  石原颯馬

============================================================================================================================================================*/
#ifndef _HDRCAMERA_H_
#define _HDRCAMERA_H_

#include "camera.h"
#include "main.h"

#define NUM_HDRCAMERA_HALF	(2)			//画面分割が、縦・横　どちらか２分割の場合のカメラの数
#define NUM_HDRCAMERA		(4)			//カメラの数

//描画するカメラの数 列挙
typedef enum
{
	NumHDRCamera_ONLY = 0,		//全体の俯瞰視点
	NumHDRCamera_HALF_SIDE,		//横に分けた２つの視点
	NumHDRCamera_HALF_HIGH_row,	//縦に分けた２つの視点
	NumHDRCamera_FOUR_Separate,	//各プレイヤーを見られる４つの視点
	NumHDRCamera_MAX,
}NumHDRCamera;

//カメラ準備OK
typedef enum
{
	HDR_Ready_SETUP = 0,	//カメラ準備中
	HDR_Ready_OK,			//準備完了
	HDR_Ready_MAX
}HDR_Ready;

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
	HDR_Ready		ready;			// 準備OK？
	bool			bUse;			// 使用されているかいないか
} HDRCamera;

//プロトタイプ宣言
void InitSetHDRCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumHDRCamera);	//カメラの位置設定
void InitHDRCamera(NumCamera type);			//初期化処理
void UninitHDRCamera(void);					//終了処理
void UpdateHDRCamera(void);					//更新処理
void SetHDRCamera(int nIdx);				//設定処理
void MoveHDRCamera(HDRCamera *pCamera);		//カメラの移動処理

void SetUpHDRCamera(HDRCamera *pCamera, float PlayerPosY);	//カメラのセットアップ処理（ゲーム開始画面の演出用
void SetUpCheckHDRCamera(void);								//カメラのセットアップ完了状態検査
void SetPosRHDRCamera(HDRCamera *pCamera, float PlayerPosY);//注視点設定
void UpdatePosVHDRCamera(HDRCamera *pCamera);				//視点の位置更新
HDRCamera *GetHDRCamera(void);								//カメラの取得
HDR_Ready *GetHDR_Ready(void);								//カメラが準備OKか

//=========================================
//カメラの台数別　設定処理
//=========================================
void Set_NumHDRCamera(NumCamera type);
void SPS_ChaseHDRCamera(HDRCamera *pCamera);

#endif
