/*==========================================================================================================================================================

タイトル用カメラ処理[titleCamera.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "titleCamera.h"
#include "conversioninput.h"

#define POS_POSV			D3DXVECTOR3(0.0f, 200.0f, -800.0f)	//視点の初期位置
#define POS_POSR			D3DXVECTOR3(0.0f, 100.0f, 0.0f)		//注視点の初期位置
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//上方向ベクトルの初期値
#define VIEW_ANGLE			(45.0f)								//視野角
#define MIN_DRAW_Z			(10.0f)								//描画処理の最低奥行き
#define MAX_DRAW_Z			(20000.0f)							//描画処理の最高奥行き
#define SPIN_ROT			(0.01f)								//回転量

//グローバル変数
TitleCamera				g_TitleCamera;							//カメラの情報

//------------------------------------------------
//			タイトル用カメラの初期化処理
//------------------------------------------------
void InitTitleCamera(void)
{
	g_TitleCamera.posR = POS_POSR;
	g_TitleCamera.posV = POS_POSV;
	g_TitleCamera.vecU = POS_VECU;

	D3DXVECTOR3 fDiffPos = g_TitleCamera.posR - g_TitleCamera.posV;	//視点と注視点の位置の差
	fDiffPos.x = powf(fDiffPos.x, 2.0f);					//視点同士の位置の差を２乗
	fDiffPos.z = powf(fDiffPos.z, 2.0f);					//視点同士の位置の差を２乗

	g_TitleCamera.rot.y = -atan2f(fDiffPos.x, fDiffPos.z);		// Y方向の角度を計算

	float PosDiffX, PosDiffZ;

	PosDiffX = powf(g_TitleCamera.posR.x - g_TitleCamera.posV.x, 2.0f);	//2乗
	PosDiffZ = powf(g_TitleCamera.posR.z - g_TitleCamera.posV.z, 2.0f);	//2乗

	g_TitleCamera.fLength = sqrtf(PosDiffX + PosDiffZ);	//長さを算出

	//位置設定
	MoveTitleCamera(g_TitleCamera.posR, &g_TitleCamera.posV, D3DX_PI);
}

//------------------------------------------------
//			タイトル用カメラの終了処理
//------------------------------------------------
void UninitTitleCamera(void)
{

}

//------------------------------------------------
//			タイトル用カメラの更新処理
//------------------------------------------------
void UpdateTitleCamera(void)
{
	//視点移動
	SpinTitlePosV();

	//位置設定
	MoveTitleCamera(g_TitleCamera.posR, &g_TitleCamera.posV, D3DX_PI);
}

//視点回転
void SpinTitlePosV(void)
{
	//回転する
	g_TitleCamera.rot.y += SPIN_ROT;

	//角度が円周率を超えたら
	if (D3DX_PI < g_TitleCamera.rot.y)
	{
		//角度を直す
		FIX_ROT(g_TitleCamera.rot.y);
	}
}

//カメラの移動処理
void MoveTitleCamera(D3DXVECTOR3 ParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = ParentPos.x + sinf((CorRot - g_TitleCamera.rot.y)) * g_TitleCamera.fLength;
	pChildPos->z = ParentPos.z + cosf((CorRot - g_TitleCamera.rot.y)) * g_TitleCamera.fLength;
}

//------------------------------------------------
//			タイトル用カメラの設定処理
//------------------------------------------------
void SetTitleCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

												//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&g_TitleCamera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_TitleCamera.mtxProjection, D3DXToRadian(VIEW_ANGLE), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_Z, MAX_DRAW_Z);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_TitleCamera.mtxProjection);

	//ビューマトリックス
	D3DXMatrixIdentity(&g_TitleCamera.mtxview);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_TitleCamera.mtxview,
		&g_TitleCamera.posV,		//視点
		&g_TitleCamera.posR,		//注視点
		&g_TitleCamera.vecU);	//上方向ベクトル

								//ビューマトリックス
	pDevice->SetTransform(D3DTS_VIEW, &g_TitleCamera.mtxview);
}

//タイトル用カメラの情報取得
TitleCamera *GetTitleCamera(void)
{
	return &g_TitleCamera;
}