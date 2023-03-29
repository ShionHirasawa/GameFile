/*==========================================================================================================================================================

カメラの処理[camera.cpp]
Author:大宮愛羅  平澤詩苑  石原颯馬

============================================================================================================================================================*/
#include "camera.h"
#include "input.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "camera_frame.h"
#include "input.h"

//注視点情報
#define POSR_SPEED		(10.0f)		//注視点の移動速度
#define POSR_WIDTH		(0.0f)		//注視点の幅
#define POSR_HEIGHT		(0.0f)		//注視点の高さ
#define POSR_DEPTH		(0.0f)		//注視点の奥行き

//視点情報
#define POSV_ROTSPEED	(0.05f)		//視点の回転速度
#define POSV_SPEED		(10.0f)		//視点の移動速度
#define POSR_ADD		(8.0f)		//注視点の上下移動
#define POSV_DISTANCE	(100.0f)	//視点の距離
#define POSV_WIDTH		(0.0f)		//視点の幅
#define POSV_HEIGHT		(1050.0f)	//視点の高さ
#define POSV_DEPTH		(1000.0f)	//視点の奥行き

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

//３人称視点時のマクロ
#define TPS_LENGTH		(150.0f)	//注視点～視点間の距離
#define TPS_posV_HEIGHT	(100.0f)	//視点の高さ
#define TPS_posR_HEIGHT	(30.0f)		//注視点の高さ

//グローバル変数
Camera		g_Camera[NUM_CAMERA];	//カメラの情報
NumCamera	g_NumCameraType;		//カメラ分割の情報

//=========================================
//カメラの位置設定処理
//Author:石原颯馬
//=========================================
void InitSetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNumCamera)
{
	//設定
	g_Camera[nNumCamera].posV = posV;	//視点
	g_Camera[nNumCamera].posR = posR;	//注視点

										//上方向ベクトルだけ固定
	g_Camera[nNumCamera].vecU = VECU_OVER;	//上方向ベクトル

											//それぞれの位置の差分を格納する変数
	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_Camera[nNumCamera].posR.x - g_Camera[nNumCamera].posV.x, DIFF_TIMES);	//２乗
	PosDiffZ = powf(g_Camera[nNumCamera].posR.z - g_Camera[nNumCamera].posV.z, DIFF_TIMES);	//２乗

																							//長さの算出
	g_Camera[nNumCamera].fLength = sqrtf(PosDiffX + PosDiffZ);

	//視点の位置更新
	UpdatePosVCamera(nNumCamera);
}

//=========================================
//カメラの初期化処理
//=========================================
void InitCamera(NumCamera type)
{
	//基本情報の初期化処理
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		g_Camera[nCntCamera].rot = ZERO_SET;
	}

	Set_NumCamera(type);	//カメラの数によるカメラ情報の初期化
}

//=========================================
//カメラの終了処理
//=========================================
void UninitCamera(void)
{

}

//=========================================
//カメラの更新処理
//=========================================
void UpdateCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		//カメラが使われている
		if (g_Camera[nCntCamera].bUse == true)
		{
			//カメラの移動処理
			MoveCamera(nCntCamera);
		}
	}
}

//=========================================
//カメラの設定処理
//=========================================
void SetCamera(int nIdx)
{
	//カメラが使われている
	if (g_Camera[nIdx].bUse == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//ビューポートの設定
		pDevice->SetViewport(&g_Camera[nIdx].viewport);

		//プロジェクションマトリックスの初期化
		D3DXMatrixIdentity(&g_Camera[nIdx].mtxProjection);

		//プロジェクションマトリックスの作成
		D3DXMatrixPerspectiveFovLH(&g_Camera[nIdx].mtxProjection, D3DXToRadian(ANGLE_OF_VIEW), (float)g_Camera[nIdx].viewport.Width / (float)g_Camera[nIdx].viewport.Height, MIN_DRAW, g_Camera[nIdx].fMaxLength);

		//プロジェクションマトリックスの設定
		pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera[nIdx].mtxProjection);

		//ビューマトリックスの初期化
		D3DXMatrixIdentity(&g_Camera[nIdx].mtxView);

		//ビューマトリックスの作成
		D3DXMatrixLookAtLH(&g_Camera[nIdx].mtxView, &g_Camera[nIdx].posV, &g_Camera[nIdx].posR, &g_Camera[nIdx].vecU);

		//ビューマトリックスの設定
		pDevice->SetTransform(D3DTS_VIEW, &g_Camera[nIdx].mtxView);
	}
}

//=========================================
//カメラの台数別　設定処理
//=========================================
void Set_NumCamera(NumCamera type)
{
	int nCntCamera = 0;				//カウンター初期化
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得
	g_NumCameraType = type;			//カメラの分割情報格納

	switch (type)
	{
		/*----------------------------------------------------------------
		俯瞰で全体が見える視点の処理
		----------------------------------------------------------------*/
	case NumCamera_ONLY:
	{
		g_Camera[nCntCamera].posV = ALONE_CAMERA_POS;			//視点座標初期化
		g_Camera[nCntCamera].posR = ZERO_SET;					//注視点座標初期化

		g_Camera[nCntCamera].fMaxLength = MAX_DRAW;				//最大描画距離設定

		g_Camera[nCntCamera].viewport.X = (DWORD)0.0f;					//原点Ⅹ位置代入
		g_Camera[nCntCamera].viewport.Y = (DWORD)0.0f;					//原点Ｙ位置代入
		g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH;		//画面幅初期化
		g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//画面高さ初期化
		g_Camera[nCntCamera].viewport.MinZ = 0.0f;
		g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
		g_Camera[nCntCamera].bUse = true;
	}
	break;

	/*----------------------------------------------------------------
	横２分割の視点の処理
	----------------------------------------------------------------*/
	case NumCamera_HALF_SIDE:
	{
		for (nCntCamera; nCntCamera < NUM_CAMERA_HALF; nCntCamera++)
		{
			g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//最大描画距離設定

			g_Camera[nCntCamera].viewport.X = (DWORD)(SCREEN_WIDTH / 2) * (nCntCamera % 2);	//原点Ⅹ位置代入
			g_Camera[nCntCamera].viewport.Y = (DWORD)NIL_F;									//原点Ｙ位置代入
			g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH / 2;						//画面幅初期化
			g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT;						//画面高さ初期化
			g_Camera[nCntCamera].viewport.MinZ = 0.0f;
			g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
			g_Camera[nCntCamera].bUse = true;

			//注視点の位置更新
			UpdatePosVCamera(nCntCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	縦２分割の視点の処理
	----------------------------------------------------------------*/
	case NumCamera_HALF_HIGH_row:
	{
		for (nCntCamera; nCntCamera < NUM_CAMERA_HALF; nCntCamera++)
		{
			g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//最大描画距離設定

			g_Camera[nCntCamera].viewport.X = (DWORD)NIL_F;									//原点Ⅹ位置代入
			g_Camera[nCntCamera].viewport.Y = (DWORD)(SCREEN_HEIGHT / 2) * (nCntCamera % 2);	//原点Ｙ位置代入
			g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH;							//画面幅初期化
			g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT / 2;					//画面高さ初期化
			g_Camera[nCntCamera].viewport.MinZ = 0.0f;
			g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
			g_Camera[nCntCamera].bUse = true;

			//注視点の位置更新
			UpdatePosVCamera(nCntCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	各プレイヤー専用のカメラの処理
	----------------------------------------------------------------*/
	case NumCamera_FOUR_Separate:
	{
		for (nCntCamera; nCntCamera < NUM_CAMERA; nCntCamera++)
		{
			g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//最大描画距離設定

			g_Camera[nCntCamera].viewport.X = (SCREEN_WIDTH / 2) * (nCntCamera % 2);	//原点Ⅹ位置代入
			g_Camera[nCntCamera].viewport.Y = (SCREEN_HEIGHT / 2) * (nCntCamera / 2);	//原点Ｙ位置代入
			g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH / 2;						//画面幅初期化
			g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT / 2;					//画面高さ初期化
			g_Camera[nCntCamera].viewport.MinZ = 0.0f;
			g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
			g_Camera[nCntCamera].bUse = true;

			//注視点の位置更新
			UpdatePosVCamera(nCntCamera);
		}
	}
	break;

	/*----------------------------------------------------------------
	各プレイヤー専用のカメラの処理
	----------------------------------------------------------------*/
	case NumCamera_FOUR_SIDE:
	{
		for (nCntCamera; nCntCamera < NUM_CAMERA; nCntCamera++)
		{
			g_Camera[nCntCamera].fMaxLength = MAX_DRAW;	//最大描画距離設定

			g_Camera[nCntCamera].viewport.X = (DWORD)(SCREEN_WIDTH / 4) * nCntCamera;	//原点Ⅹ位置代入
			g_Camera[nCntCamera].viewport.Y = (DWORD)NIL_F;								//原点Ｙ位置代入
			g_Camera[nCntCamera].viewport.Width = SCREEN_WIDTH / 4;						//画面幅初期化
			g_Camera[nCntCamera].viewport.Height = SCREEN_HEIGHT;						//画面高さ初期化
			g_Camera[nCntCamera].viewport.MinZ = 0.0f;
			g_Camera[nCntCamera].viewport.MaxZ = 1.0f;
			g_Camera[nCntCamera].bUse = true;

			//注視点の位置更新
			UpdatePosVCamera(nCntCamera);
		}
	}
	}

	//画面分割の枠を設定
	SetUseFrame(type);

	//各カメラの設定
	for (int nCntUse = 0; nCntUse < NUM_CAMERA; nCntUse++)
	{
		//上のスイッチ文で使われることになったカメラ
		if (nCntUse <= nCntCamera)
		{
			//注視点の位置設定
			SetPosRCamera(nCntCamera);

			//視点の位置設定
			UpdatePosVCamera(nCntUse);
		}

		//設定されたカメラ以外のカメラを使用していないようにする
		else
		{
			g_Camera[nCntUse].bUse = false;
		}
	}
}

//カメラの移動処理
void MoveCamera(int nCntCamera)
{
#ifdef _DEBUG
	//----------------------
	//		視点の移動	
	//----------------------
	//視点の上下
	if (GetKeyboardPress(DIK_T) == true)
	{
		g_Camera[nCntCamera].posV.y += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{
		g_Camera[nCntCamera].posV.y -= POSV_SPEED;
	}

	//視点の前後
	if (GetKeyboardPress(DIK_N) == true)
	{
		g_Camera[nCntCamera].fLength += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_Camera[nCntCamera].fLength -= POSV_SPEED;
	}

	//視点の左右
	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_Camera[nCntCamera].rot.y -= POSV_ROTSPEED;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{
		g_Camera[nCntCamera].rot.y += POSV_ROTSPEED;
	}

	//------------------------
	//		注視点の移動	
	//------------------------
	//注視点の上下
	if (GetKeyboardPress(DIK_I) == true)
	{
		g_Camera[nCntCamera].posR.y += POSR_ADD;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		g_Camera[nCntCamera].posR.y -= POSR_ADD;
	}
#endif // _DEBUG

	//注視点の位置設定
	SetPosRCamera(nCntCamera);

	//視点カメラ更新
	UpdatePosVCamera(nCntCamera);
}

//３人称視点
void TPS_ChaseCamera(int nCntCamera, D3DXVECTOR3 rot)
{
	//プレイヤーの角度の逆に設定
	g_Camera[nCntCamera].rot.y = (D3DX_PI - rot.y);

	//角度修正
	FIX_ROT(g_Camera[nCntCamera].rot.y);

	//視点の高さ設定
	g_Camera[nCntCamera].posV.y = g_Camera[nCntCamera].posR.y + TPS_posV_HEIGHT;
	g_Camera[nCntCamera].posR.y += TPS_posR_HEIGHT;
	
	//注視点～視点　間の距離設定
	g_Camera[nCntCamera].fLength = TPS_LENGTH;

	//視点位置更新
	UpdatePosVCamera(nCntCamera);
}

//視点の位置更新
void UpdatePosVCamera(int nCntCamera)
{
	//視点の位置更新
	g_Camera[nCntCamera].posV.x = g_Camera[nCntCamera].posR.x + sinf(D3DX_PI - g_Camera[nCntCamera].rot.y) * g_Camera[nCntCamera].fLength;
	g_Camera[nCntCamera].posV.z = g_Camera[nCntCamera].posR.z + cosf(D3DX_PI - g_Camera[nCntCamera].rot.y) * g_Camera[nCntCamera].fLength;
}

//注視点の位置設定
void SetPosRCamera(int nCntCamera)
{
	//プレイヤー情報取得
	Player *pPlayer = GetPlayer();

	//対象のプレイヤーに注視点を合わせる
	g_Camera[nCntCamera].posR = pPlayer[nCntCamera].pos;

	//3人称視点設定
	TPS_ChaseCamera(nCntCamera, pPlayer[nCntCamera].rot);
}

//カメラの取得
Camera *GetCamera(void)
{
	return &g_Camera[0];
}

//*********************************************
//タイトル用カメラ処理
//MEMO : カメラは１番目のカメラのみを使用する
//*********************************************
void CameraForTitle(void)
{
	//カメラの向きY　のポインタを取得
	float *pRot_Y = &g_Camera[0].rot.y;

	//回転
	*pRot_Y += 0.04f;

	//向きが3.14を超えた
	if (D3DX_PI <= *pRot_Y)
	{
		FIX_ROT(*pRot_Y);
	}

	//視点の位置更新
	UpdatePosVCamera(0);
}