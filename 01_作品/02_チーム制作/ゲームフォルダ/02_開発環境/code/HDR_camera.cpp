/*==========================================================================================================================================================

カメラの処理[HDR_camera.cpp]
Author:飯田洲暉 (大宮愛羅  平澤詩苑  石原颯馬)

============================================================================================================================================================*/
#include "HDR_camera.h"
#include "camera.h"
#include "input.h"
#include "HDR_player.h"
#include "camera_frame.h"
#include "input.h"

//視点情報
#define POSV_ROTSPEED	(0.05f)		//視点の回転速度
#define POSV_SPEED		(10.0f)		//視点の移動速度
#define POSR_ADD		(8.0f)		//注視点の上下移動
#define CAMERA_LENGTH	(375.0f)	//カメラ間の距離

//上方向ベクトル情報
#define VECU_OVER		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	//上方向ベクトル

//カメラ情報
#define ANGLE_OF_VIEW	(45.0f)		//画角

//描画範囲
#define MIN_DRAW		(10.0f)		//最小描画範囲
#define MAX_DRAW		(40000.0f)	//最大描画範囲

//位置の差分を2乗
#define DIFF_TIMES		(2.0f)		//2乗

//カメラ単体の場合の視点座標
#define ALONE_CAMERA_POS	(D3DXVECTOR3(0.0f, 400.0f, 500.0f))

//２人称視点時のマクロ
#define SPS_posR_HEIGHT	(100.0f)		//注視点の高さ
#define SPS_posV_HEIGHT	(140.0f)		//視点の高さ

//カメラ準備におけるマクロ定義
#define SETUP_FLOAT_SPEED		(30.0f)		//カメラの上昇量	(float は「浮く」という意味

//グローバル変数
HDRCamera	g_HDRCamera[NUM_HDRCAMERA];	//カメラの情報
NumCamera	g_NumHDRCameraType;			//カメラ分割の情報
HDR_Ready	g_HDR_Ready;				//カメラ準備情報

//=========================================
//カメラの位置設定処理
//Author:石原颯馬
//=========================================
void InitSetHDRCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumHDRCamera)
{
	//カメラのポインタ取得
	HDRCamera *pCamera = &g_HDRCamera[nNumHDRCamera];

	pCamera->posV = posV;	//視点
	pCamera->posR = posR;	//注視点

	//上方向ベクトルだけ固定
	pCamera->vecU = VECU_OVER;

	//それぞれの位置の差分を算出
	float PosDiffX = powf(pCamera->posR.x - pCamera->posV.x, DIFF_TIMES);
	float PosDiffZ = powf(pCamera->posR.z - pCamera->posV.z, DIFF_TIMES);

	//長さの算出
	pCamera->fLength = sqrtf(PosDiffX + PosDiffZ);

	//角度を算出
	pCamera->rot.y = 0.0f;
}

//=========================================
//カメラの初期化処理
//=========================================
void InitHDRCamera(NumCamera type)
{
	Set_NumHDRCamera(type);	//カメラ分割の初期化
	g_HDR_Ready = HDR_Ready_SETUP;//カメラ準備中
	HDRCamera *pCamera = &g_HDRCamera[0];

	//カメラの情報初期化処理
	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pCamera++)
	{	
		pCamera->ready = HDR_Ready_SETUP;	//カメラを準備中に
		pCamera->posV.x = 0.0f;				//視点は原点にとどまる
		pCamera->posR.x = GetPlayer_HDR()[nCntHDRCamera].pos.x;	//注視点を各プレイヤーに

																//それぞれの位置の差分を算出
		float PosDiffX = powf(pCamera->posR.x - pCamera->posV.x, DIFF_TIMES);
		float PosDiffZ = powf(pCamera->posR.z - pCamera->posV.z, DIFF_TIMES);

		//長さの算出
		pCamera->fLength = sqrtf(PosDiffX + PosDiffZ);

		//角度を算出
		pCamera->rot.y = -atan2f(PosDiffX, PosDiffZ);
	}
}

//=========================================
//カメラの終了処理
//=========================================
void UninitHDRCamera(void)
{

}

//=========================================
//カメラの更新処理
//=========================================
void UpdateHDRCamera(void)
{
	Player_HDR *pPlayer = GetPlayer_HDR();	//プレイヤーのポインターを取得
	HDRCamera *pCamera = &g_HDRCamera[0];	//カメラのポインターも取得

	for (int nCntHDRCamera = 0; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pCamera++, pPlayer++)
	{
		pCamera->posR.x = pPlayer->pos.x;	//注視点
		pCamera->posR.z = pPlayer->pos.z;	//注視点
		pCamera->posV.x = 0.0f;
		
		//カメラが準備中
		if (g_HDR_Ready == HDR_Ready_SETUP)
		{
			//カメラ準備
			SetUpHDRCamera(pCamera, pPlayer->pos.y);
		}

		//準備完了
		else if (g_HDR_Ready == HDR_Ready_OK)
		{
			//プレイヤーがゴールしたら
			if (pPlayer->bGoal)
			{
				//プレイヤーの正面に向くように徐々に角度を補正
				pCamera->rot.y += (0.0f - pCamera->rot.y) * 0.3f;
				pCamera->fLength = CAMERA_LENGTH;

				//視点の位置設定
				UpdatePosVHDRCamera(pCamera);
			}

			//注視点の位置設定
			SetPosRHDRCamera(pCamera, pPlayer->pos.y);
		}
	}

	//セットアップ完了かどうか検査
	SetUpCheckHDRCamera();
}

//カメラ準備処理
void SetUpHDRCamera(HDRCamera *pCamera, float PlayerPosY) 
{
	//視点・注視点を上昇
	pCamera->posR.y += SETUP_FLOAT_SPEED;
	pCamera->posV.y += SETUP_FLOAT_SPEED;

	//プレイヤーの位置より、注視点が上に行った
	if (PlayerPosY <= pCamera->posR.y)
	{
		pCamera->posR.y = PlayerPosY + SPS_posR_HEIGHT;	//注視点の位置修正
		pCamera->posV.y = PlayerPosY + SPS_posV_HEIGHT;	//視点の位置修正
		pCamera->ready = HDR_Ready_OK;					//準備OK
	}
}

//セットアップ完了状態検査処理
void SetUpCheckHDRCamera(void)
{
	//-------------------------------------
	//すでに準備完了しているなら処理しない
	//-------------------------------------
	if (g_HDR_Ready == HDR_Ready_OK)
	{
		return;
	}

	int nSetUpCounter = 0;	//準備完了している数を格納

	for (int nCntSetUp = 0; nCntSetUp < NUM_HDRCAMERA; nCntSetUp++)
	{
		//カメラ準備がOKかどうか
		if (g_HDRCamera[nCntSetUp].ready == HDR_Ready_OK)
		{
			//準備完了している数を加算
			nSetUpCounter++;
		}
	}

	//準備完了している数が、カメラの台数と同じ
	if (nSetUpCounter == NUM_HDRCAMERA)
	{
		//全カメラ準備完了
		g_HDR_Ready = HDR_Ready_OK;
	}
}

//カメラの移動処理
void MoveHDRCamera(HDRCamera *pCamera)
{
#ifdef _DEBUG
	//----------------------
	//		視点の移動	
	//----------------------
	//視点の上下
	if (GetKeyboardPress(DIK_T) == true)
	{
		pCamera->posV.y += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{
		pCamera->posV.y -= POSV_SPEED;
	}

	//視点の前後
	if (GetKeyboardPress(DIK_N) == true)
	{
		pCamera->fLength += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{
		pCamera->fLength -= POSV_SPEED;
	}

	//視点の左右
	if (GetKeyboardPress(DIK_Z) == true)
	{
		pCamera->rot.y -= POSV_ROTSPEED;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{
		pCamera->rot.y += POSV_ROTSPEED;
	}

	//------------------------
	//		注視点の移動	
	//------------------------
	//注視点の上下
	if (GetKeyboardPress(DIK_I) == true)
	{
		pCamera->posR.y += POSR_ADD;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		pCamera->posR.y -= POSR_ADD;
	}
#endif // _DEBUG
}

//注視点の位置設定
void SetPosRHDRCamera(HDRCamera *pCamera, float PlayerPosY)
{
	//プレイヤーの位置が、注視点よりも低かった場合
	if (PlayerPosY < pCamera->posR.y - SPS_posR_HEIGHT && g_HDR_Ready == HDR_Ready_OK)
	{
		pCamera->posR.y = PlayerPosY + SPS_posR_HEIGHT;	//対象のプレイヤーに注視点を合わせる
		pCamera->posV.y = PlayerPosY + SPS_posV_HEIGHT;	//対象のプレイヤーに視点を合わせる
	}
}

//２人称視点
void SPS_ChaseHDRCamera(HDRCamera *pCamera)
{
	//プレイヤーの正面に向くように徐々に角度を補正
	pCamera->rot.y -= (0.0f - pCamera->rot.y) * 0.3f;

	//視点の位置更新
	pCamera->posV.x = pCamera->posR.x + sinf(D3DX_PI - pCamera->rot.y) * pCamera->fLength;
	pCamera->posV.z = pCamera->posR.z + cosf(D3DX_PI - pCamera->rot.y) * pCamera->fLength;
}

//視点の位置更新
void UpdatePosVHDRCamera(HDRCamera *pCamera)
{
	//視点の位置更新
	pCamera->posV.x = pCamera->posR.x + sinf(pCamera->rot.y - D3DX_PI) * pCamera->fLength;
	pCamera->posV.z = pCamera->posR.z + cosf(pCamera->rot.y - D3DX_PI) * pCamera->fLength;
}

//=========================================
//カメラの設定処理
//=========================================
void SetHDRCamera(int nIdx)
{
	//カメラが使われている
	if (g_HDRCamera[nIdx].bUse == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//ビューポートの設定
		pDevice->SetViewport(&g_HDRCamera[nIdx].viewport);

		//プロジェクションマトリックスの初期化
		D3DXMatrixIdentity(&g_HDRCamera[nIdx].mtxProjection);

		//プロジェクションマトリックスの作成
		D3DXMatrixPerspectiveFovLH(&g_HDRCamera[nIdx].mtxProjection, D3DXToRadian(ANGLE_OF_VIEW), (float)g_HDRCamera[nIdx].viewport.Width / (float)g_HDRCamera[nIdx].viewport.Height, MIN_DRAW, g_HDRCamera[nIdx].fMaxLength);

		//プロジェクションマトリックスの設定
		pDevice->SetTransform(D3DTS_PROJECTION, &g_HDRCamera[nIdx].mtxProjection);

		//ビューマトリックスの初期化
		D3DXMatrixIdentity(&g_HDRCamera[nIdx].mtxView);

		//ビューマトリックスの作成
		D3DXMatrixLookAtLH(&g_HDRCamera[nIdx].mtxView, &g_HDRCamera[nIdx].posV, &g_HDRCamera[nIdx].posR, &g_HDRCamera[nIdx].vecU);

		//ビューマトリックスの設定
		pDevice->SetTransform(D3DTS_VIEW, &g_HDRCamera[nIdx].mtxView);
	}
}

//=========================================
//カメラの台数別　設定処理
//=========================================
void Set_NumHDRCamera(NumCamera type)
{
	int nCntHDRCamera = 0;				//カウンター初期化
	g_NumHDRCameraType = type;			//カメラの分割情報格納
	Player_HDR *pPlayer_HDR = GetPlayer_HDR();	//プレイヤーの情報取得
	HDRCamera *pCamera = &g_HDRCamera[0];		//カメラのポインタ取得

	switch (type)
	{
		/*----------------------------------------------------------------
		俯瞰で全体が見える視点の処理
		----------------------------------------------------------------*/
	case NumHDRCamera_ONLY:
	{
		pCamera->posR.x = pPlayer_HDR->pos.x;		//注視点のⅩ座標設定
		pCamera->posR.z = pPlayer_HDR->pos.z;		//注視点のＺ座標設定

		pCamera->fMaxLength = MAX_DRAW;			//最大描画距離設定

		pCamera->viewport.X = (DWORD)0.0f;		//原点Ⅹ位置代入
		pCamera->viewport.Y = (DWORD)0.0f;		//原点Ｙ位置代入
		pCamera->viewport.Width = SCREEN_WIDTH;	//画面幅初期化
		pCamera->viewport.Height = SCREEN_HEIGHT;	//画面高さ初期化
		pCamera->viewport.MinZ = 0.0f;
		pCamera->viewport.MaxZ = 1.0f;
		pCamera->bUse = true;
		nCntHDRCamera++;	//次のカメラへ
	}
	break;

	/*----------------------------------------------------------------
	横２分割の視点の処理
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_SIDE:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pPlayer_HDR++, pCamera++)
		{
			pCamera->posR.x = pPlayer_HDR->pos.x;		//注視点のⅩ座標設定
			pCamera->posR.z = pPlayer_HDR->pos.z;		//注視点のＺ座標設定

			pCamera->fMaxLength = MAX_DRAW;	//最大描画距離設定

			pCamera->viewport.X = (DWORD)(SCREEN_WIDTH / 4) * nCntHDRCamera;	//原点Ⅹ位置代入
			pCamera->viewport.Y = (DWORD)NIL_F;									//原点Ｙ位置代入
			pCamera->viewport.Width = SCREEN_WIDTH / 4;							//画面幅初期化
			pCamera->viewport.Height = SCREEN_HEIGHT;							//画面高さ初期化
			pCamera->viewport.MinZ = 0.0f;
			pCamera->viewport.MaxZ = 1.0f;
			pCamera->bUse = true;

			//注視点の位置更新
			UpdatePosVHDRCamera(pCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	縦２分割の視点の処理
	----------------------------------------------------------------*/
	case NumHDRCamera_HALF_HIGH_row:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA_HALF; nCntHDRCamera++, pPlayer_HDR++, pCamera++)
		{
			pCamera->posR.x = pPlayer_HDR->pos.x;		//注視点のⅩ座標設定
			pCamera->posR.z = pPlayer_HDR->pos.z;		//注視点のＺ座標設定

			pCamera->fMaxLength = MAX_DRAW;	//最大描画距離設定

			pCamera->viewport.X = (DWORD)NIL_F;										//原点Ⅹ位置代入
			pCamera->viewport.Y = (DWORD)(SCREEN_HEIGHT / 2) * (nCntHDRCamera % 2);	//原点Ｙ位置代入
			pCamera->viewport.Width = SCREEN_WIDTH;									//画面幅初期化
			pCamera->viewport.Height = SCREEN_HEIGHT / 2;								//画面高さ初期化
			pCamera->viewport.MinZ = 0.0f;
			pCamera->viewport.MaxZ = 1.0f;
			pCamera->bUse = true;

			//注視点の位置更新
			UpdatePosVHDRCamera(pCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	各プレイヤー専用のカメラの処理
	----------------------------------------------------------------*/
	case NumHDRCamera_FOUR_Separate:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_HDRCAMERA; nCntHDRCamera++, pPlayer_HDR++, pCamera++)
		{
			pCamera->posR.x = pPlayer_HDR->pos.x;		//注視点のⅩ座標設定
			pCamera->posR.z = pPlayer_HDR->pos.z;		//注視点のＺ座標設定

			pCamera->fMaxLength = MAX_DRAW;	//最大描画距離設定

			pCamera->viewport.X = (SCREEN_WIDTH / 2) * (nCntHDRCamera % 2);	//原点Ⅹ位置代入
			pCamera->viewport.Y = (SCREEN_HEIGHT / 2) * (nCntHDRCamera / 2);	//原点Ｙ位置代入
			pCamera->viewport.Width = SCREEN_WIDTH / 2;						//画面幅初期化
			pCamera->viewport.Height = SCREEN_HEIGHT / 2;						//画面高さ初期化
			pCamera->viewport.MinZ = 0.0f;
			pCamera->viewport.MaxZ = 1.0f;
			pCamera->bUse = true;

			//注視点の位置更新
			UpdatePosVHDRCamera(pCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	各プレイヤー専用のカメラの処理
	----------------------------------------------------------------*/
	case NumCamera_FOUR_SIDE:
	{
		for (nCntHDRCamera; nCntHDRCamera < NUM_CAMERA; nCntHDRCamera++, pCamera++)
		{
			pCamera->fMaxLength = MAX_DRAW;	//最大描画距離設定

			pCamera->viewport.X = (DWORD)(SCREEN_WIDTH / 4) * nCntHDRCamera;	//原点Ⅹ位置代入
			pCamera->viewport.Y = (DWORD)NIL_F;								//原点Ｙ位置代入
			pCamera->viewport.Width = SCREEN_WIDTH / 4;						//画面幅初期化
			pCamera->viewport.Height = SCREEN_HEIGHT;						//画面高さ初期化
			pCamera->viewport.MinZ = 0.0f;
			pCamera->viewport.MaxZ = 1.0f;
			pCamera->bUse = true;

			//注視点の位置更新
			UpdatePosVHDRCamera(pCamera);
		}
	}
	}

	//画面分割の枠を設定
	SetUseFrame(type);

	//設定されなかったカメラを使用しないようにする
	for (int nCntUse = nCntHDRCamera; nCntUse < NUM_HDRCAMERA; nCntUse++, pCamera++)
	{
		pCamera->bUse = false;
	}
}

//カメラの取得
HDRCamera *GetHDRCamera(void)
{
	return &g_HDRCamera[0];
}

//カメラ準備状態の取得
HDR_Ready *GetHDR_Ready(void)
{
	//準備状態を返す
	return &g_HDR_Ready;
}