/*==========================================================================================================================================================

カメラの処理[camera.cpp]
Author:大宮愛羅  平澤詩苑  石原颯馬

============================================================================================================================================================*/
#include "SelPlayer_camera.h"
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

//３人称視点時のマクロ
#define TPS_LENGTH		(150.0f)	//注視点～視点間の距離
#define TPS_posV_HEIGHT	(100.0f)	//視点の高さ
#define TPS_posR_HEIGHT	(30.0f)		//注視点の高さ

//グローバル変数
Camera		g_SelPlayer_Camera;	//カメラの情報

//=========================================
//カメラの位置設定処理
//Author:石原颯馬
//=========================================
void InitSelPlayer_SetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	//設定
	g_SelPlayer_Camera.posV = posV;	//視点
	g_SelPlayer_Camera.posR = posR;	//注視点

										//上方向ベクトルだけ固定
	g_SelPlayer_Camera.vecU = VECU_OVER;	//上方向ベクトル

											//それぞれの位置の差分を格納する変数
	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_SelPlayer_Camera.posR.x - g_SelPlayer_Camera.posV.x, DIFF_TIMES);	//２乗
	PosDiffZ = powf(g_SelPlayer_Camera.posR.z - g_SelPlayer_Camera.posV.z, DIFF_TIMES);	//２乗

																							//長さの算出
	g_SelPlayer_Camera.fLength = sqrtf(PosDiffX + PosDiffZ);

	//視点の位置更新
	UpdateSelPlayer_PosVCamera();
}

//=========================================
//カメラの初期化処理
//=========================================
void InitSelPlayer_Camera(void)
{
	//基本情報の初期化処理
	g_SelPlayer_Camera.rot = ZERO_SET;
}

//=========================================
//カメラの終了処理
//=========================================
void UninitSelPlayer_Camera(void)
{

}

//=========================================
//カメラの更新処理
//=========================================
void UpdateSelPlayer_Camera(void)
{
	//カメラが使われている
	if (g_SelPlayer_Camera.bUse == true)
	{
		//カメラの移動処理
		MoveSelPlayer_Camera();
	}
}

//=========================================
//カメラの設定処理
//=========================================
void SetSelPlayer_Camera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビューポートの設定
	g_SelPlayer_Camera.fMaxLength = MAX_DRAW;				//最大描画距離設定

	g_SelPlayer_Camera.viewport.X = (DWORD)0.0f;			//原点Ⅹ位置代入
	g_SelPlayer_Camera.viewport.Y = (DWORD)0.0f;			//原点Ｙ位置代入
	g_SelPlayer_Camera.viewport.Width = SCREEN_WIDTH;		//画面幅初期化
	g_SelPlayer_Camera.viewport.Height = SCREEN_HEIGHT;		//画面高さ初期化
	g_SelPlayer_Camera.viewport.MinZ = 0.0f;
	g_SelPlayer_Camera.viewport.MaxZ = 1.0f;
	g_SelPlayer_Camera.bUse = true;

	pDevice->SetViewport(&g_SelPlayer_Camera.viewport);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_SelPlayer_Camera.mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_SelPlayer_Camera.mtxProjection, D3DXToRadian(ANGLE_OF_VIEW), (float)g_SelPlayer_Camera.viewport.Width / (float)g_SelPlayer_Camera.viewport.Height, MIN_DRAW, g_SelPlayer_Camera.fMaxLength);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_SelPlayer_Camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_SelPlayer_Camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_SelPlayer_Camera.mtxView, &g_SelPlayer_Camera.posV, &g_SelPlayer_Camera.posR, &g_SelPlayer_Camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_SelPlayer_Camera.mtxView);
}

//カメラの移動処理
void MoveSelPlayer_Camera(void)
{
#ifdef _DEBUG
	//----------------------
	//		視点の移動	
	//----------------------
	//視点の上下
	if (GetKeyboardPress(DIK_T) == true)
	{
		g_SelPlayer_Camera.posV.y += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{
		g_SelPlayer_Camera.posV.y -= POSV_SPEED;
	}

	//視点の前後
	if (GetKeyboardPress(DIK_N) == true)
	{
		g_SelPlayer_Camera.fLength += POSV_SPEED;
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_SelPlayer_Camera.fLength -= POSV_SPEED;
	}

	//視点の左右
	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_SelPlayer_Camera.rot.y -= POSV_ROTSPEED;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{
		g_SelPlayer_Camera.rot.y += POSV_ROTSPEED;
	}

	//------------------------
	//		注視点の移動	
	//------------------------
	//注視点の上下
	if (GetKeyboardPress(DIK_I) == true)
	{
		g_SelPlayer_Camera.posR.y += POSR_ADD;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		g_SelPlayer_Camera.posR.y -= POSR_ADD;
	}
#endif // _DEBUG

	//視点カメラ更新
	UpdateSelPlayer_PosVCamera();
}

//３人称視点
void TPS_SelPlayer_ChaseCamera(int nCntCamera, D3DXVECTOR3 rot)
{
	//プレイヤーの角度の逆に設定
	g_SelPlayer_Camera.rot.y = (D3DX_PI - rot.y);

	//角度修正
	FIX_ROT(g_SelPlayer_Camera.rot.y);

	//視点の高さ設定
	g_SelPlayer_Camera.posV.y = g_SelPlayer_Camera.posR.y + TPS_posV_HEIGHT;
	g_SelPlayer_Camera.posR.y += TPS_posR_HEIGHT;
	
	//注視点～視点　間の距離設定
	g_SelPlayer_Camera.fLength = TPS_LENGTH;

	//視点位置更新
	UpdateSelPlayer_PosVCamera();
}

//視点の位置更新
void UpdateSelPlayer_PosVCamera(void)
{
	//視点の位置更新
	g_SelPlayer_Camera.posV.x = g_SelPlayer_Camera.posR.x + sinf(D3DX_PI - g_SelPlayer_Camera.rot.y) * g_SelPlayer_Camera.fLength;
	g_SelPlayer_Camera.posV.z = g_SelPlayer_Camera.posR.z + cosf(D3DX_PI - g_SelPlayer_Camera.rot.y) * g_SelPlayer_Camera.fLength;
}

//カメラの取得
Camera *GetSelPlayer_Camera(void)
{
	return &g_SelPlayer_Camera;
}

//*********************************************
//タイトル用カメラ処理
//MEMO : カメラは１番目のカメラのみを使用する
//*********************************************
void SelPlayer_CameraForTitle(void)
{
	//カメラの向きY　のポインタを取得
	float *pRot_Y = &g_SelPlayer_Camera.rot.y;

	//回転
	*pRot_Y += 0.04f;

	//向きが3.14を超えた
	if (D3DX_PI <= *pRot_Y)
	{
		FIX_ROT(*pRot_Y);
	}

	//視点の位置更新
	UpdateSelPlayer_PosVCamera();
}