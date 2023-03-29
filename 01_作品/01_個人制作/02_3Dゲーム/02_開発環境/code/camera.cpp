/*==========================================================================================================================================================

														カメラ処理[camera.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "camera.h"
#include "input.h"
#include "player.h"

#define POS_POSV			D3DXVECTOR3(-500.0f, 100.0f, -500.0f)	//視点の初期位置
#define POS_POSR			D3DXVECTOR3(0.0f, 0.0f, 0.0f)		//注視点の初期位置
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//上方向ベクトルの初期値
#define VIEW_ANGLE			(45.0f)								//視野角
#define MOVE_CAMERA_SPEED	(2.0f)								//カメラ・注視点座標の移動量
#define MIN_DRAW_Z			(10.0f)								//描画処理の最低奥行き
#define MAX_DRAW_Z			(2000.0f)							//描画処理の最高奥行き
#define MIN_LENGTH			(100.0f)							//視点と注視点の最低距離
#define MAX_LENGTH			(300.0f)							//視点と注視点の最大距離
#define CHANGE_LENGHT		(0.5f)								//長さの移動量
#define SPIN_ROT			(0.05f)								//回転量
#define COR_ROT_POSV		(3.14f)								//角度計算による視点操作の調整値
#define COR_ROT_POSR		(0.0f)								//角度計算による注視点操作の調整値
#define DRAW_LENGTH			(500.0f)							//オブジェクトを描画させてあげる範囲

//グローバル変数
Camera						g_Camera;							//カメラの情報

//------------------------------------------------
//				カメラの初期化処理
//------------------------------------------------
void InitCamera(void)
{
	ResetCameraPos();

	D3DXVECTOR3 fDiffPos = g_Camera.posR - g_Camera.posV;	//視点と注視点の位置の差
	fDiffPos.x = powf(fDiffPos.x, 2.0f);					//視点同士の位置の差を２乗
	fDiffPos.z = powf(fDiffPos.z, 2.0f);					//視点同士の位置の差を２乗

	g_Camera.rot.y = -atan2f(fDiffPos.x, fDiffPos.z);		// Y方向の角度を計算

	float PosDiffX, PosDiffY, PosDiffZ;

	PosDiffX = powf(g_Camera.posR.x - g_Camera.posV.x, 2.0f);	//2乗
	PosDiffY = powf(g_Camera.posR.y - g_Camera.posV.y, 2.0f);	//2乗
	PosDiffZ = powf(g_Camera.posR.z - g_Camera.posV.z, 2.0f);	//2乗

	g_Camera.fLength = sqrtf(PosDiffX + PosDiffY + PosDiffZ);	//長さを算出

	MoveCamera(&g_Camera.posR, &g_Camera.posV, COR_ROT_POSV);
}

//------------------------------------------------
//				カメラの終了処理
//------------------------------------------------
void UninitCamera(void)
{

}

//------------------------------------------------
//				カメラの更新処理
//------------------------------------------------
void UpdateCamera(void)
{	
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得

	g_Camera.posR = pPlayer->pos;						//注視点の位置をプレイヤーの位置に設定
	g_Camera.posV += pPlayer->pos - pPlayer->posOld;	//プレイヤーが移動した分を視点にも同じように加算
	
	MovePosV();		//視点移動
	SpinPosV();		//視点回転

	LengthCamera();

	//カメラの角度修正
	FixCameraRot();

#if _DEBUG
	//カメラの位置初期化
	if (GetKeyboardTrigger(DIK_N) == true)
	{
		ResetCameraPos();
	}
#endif
}

//視点移動
void MovePosV(void)
{
#if 0
	//視点、注視点ともに左に移動
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Camera.posV.x += sinf(LEFT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(LEFT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
									 
		g_Camera.posV.z += sinf(DOWN - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(DOWN - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//視点、注視点ともに右に移動
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Camera.posV.x += sinf(RIGHT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(RIGHT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(UP - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(UP - g_Camera.rot.y) * MOVE_CAMERA_SPEED;		
	}

	//視点、注視点ともに奥に移動
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_Camera.posV.x += sinf(UP + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(UP + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(RIGHT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(RIGHT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//視点、注視点ともに手前に移動
	if (GetKeyboardPress(DIK_S) == true)
	{
		g_Camera.posV.x += sinf(DOWN + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(DOWN + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(LEFT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(LEFT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

#endif

	//視点上昇
	if ((GetKeyboardPress(DIK_UP) || GetGamePadPress(XINPUT_GAMEPAD_Y)) == true)
	{
		g_Camera.posV.y -= cosf(UP) * MOVE_CAMERA_SPEED;
	}

	//視点下降
	if ((GetKeyboardPress(DIK_DOWN) || GetGamePadPress(XINPUT_GAMEPAD_X)) == true)
	{
		g_Camera.posV.y -= cosf(DOWN) * MOVE_CAMERA_SPEED;
	}
}

//視点回転
void SpinPosV(void)
{
	//左旋回
	if (GetKeyboardPress(DIK_LEFT) == true || GetGamePadPress(XINPUT_GAMEPAD_LEFT_SHOULDER) == true)
	{
		g_Camera.rot.y -= SPIN_ROT;
	}

	//右旋回
	if (GetKeyboardPress(DIK_RIGHT) == true || GetGamePadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER) == true)
	{
		g_Camera.rot.y += SPIN_ROT;
	}

	MoveCamera(&g_Camera.posR, &g_Camera.posV, COR_ROT_POSV);
}

//角度修正
void FixCameraRot(void)
{
	//X方向の右回り修正(3.14以上)
	if (g_Camera.rot.x >= D3DX_PI)
	{
		g_Camera.rot.x -= FIX_ROT;
	}

	//X方向の左回り修正(-3.14以下)
	if (g_Camera.rot.x <= -D3DX_PI)
	{
		g_Camera.rot.x += FIX_ROT;
	}

	//Y方向の右回り修正(3.14以上)
	if (g_Camera.rot.y >= D3DX_PI)
	{
		g_Camera.rot.y -= FIX_ROT;
	}

	//Y方向の左回り修正(-3.14以下)
	if (g_Camera.rot.y <= -D3DX_PI)
	{
		g_Camera.rot.y += FIX_ROT;
	}

	//Z方向の右回り修正(3.14以上)
	if (g_Camera.rot.z >= D3DX_PI)
	{
		g_Camera.rot.z -= FIX_ROT;
	}

	//Z方向の左回り修正(-3.14以下)
	if (g_Camera.rot.z <= -D3DX_PI)
	{
		g_Camera.rot.z += FIX_ROT;
	}
}

//カメラの移動処理
void MoveCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = pParentPos->x + sinf((CorRot - g_Camera.rot.y)) * g_Camera.fLength;
	pChildPos->z = pParentPos->z + cosf((CorRot - g_Camera.rot.y)) * g_Camera.fLength;
}

//カメラの位置リセット
void ResetCameraPos(void)
{
	g_Camera.posR = GetPlayer()->pos;			//注視点の位置をプレイヤーの位置に設定
	g_Camera.posV = g_Camera.posR + POS_POSV;	//視点初期化
	g_Camera.rot = ZERO_SET;					//向き初期化
	g_Camera.vecU = POS_VECU;					//上方向ベクトル初期化
	LengthCamera();								//カメラの距離初期化
}

//視点間の長さ処理
void LengthCamera(void)
{
	//距離が離れすぎた
	if (g_Camera.fLength > MAX_LENGTH)
	{
		g_Camera.fLength -= 1.5f;
	}

	//距離が近すぎた
	if (g_Camera.fLength < MIN_LENGTH)
	{
		g_Camera.fLength = MIN_LENGTH;
	}
}

//------------------------------------------------
//				カメラの設定処理
//------------------------------------------------
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, D3DXToRadian(VIEW_ANGLE), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_Z, MAX_DRAW_Z);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	//ビューマトリックス
	D3DXMatrixIdentity(&g_Camera.mtxview);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_Camera.mtxview,
						&g_Camera.posV,		//視点
						&g_Camera.posR,		//注視点
						&g_Camera.vecU);	//上方向ベクトル

	//ビューマトリックス
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxview);
}

//カメラの情報取得
Camera *GetCamera(void)
{
	return &g_Camera;
}