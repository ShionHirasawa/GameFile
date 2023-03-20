/*==========================================================================================================================================================

														プレイヤーの処理[player.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "player.h"
#include "model.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "model.h"
#include "wall.h"
#include "slope.h"
#include "color.h"
#include "fade.h"
#include "meshfield.h"
#include "billboard.h"
#include "sound.h"
#include "game.h"
#include "cannon.h"

#define PLAYER_START_POS		(D3DXVECTOR3(-550.0f, 0.0f, -550.0f))
//#define PLAYER_START_POS		(D3DXVECTOR3(-0.0f, 180.0f, -400.0f))		//デバッグ用初期位置
#define PLAYER_TEX_MAX			(10)		//モデルのテクスチャの最大数
#define PLAYER_SPEED			(0.5f)		//プレイヤーの移動スピード
#define PLAYER_JUMP				(14.0f)		//プレイヤーのジャンプ量
#define MAX_SPEED				(4.0f)		//プレイヤーの限界移動量
#define CROSS_SPEED_COR			(0.9f)		//斜め方向への移動量補正値
#define PLAYER_LIFE				(5)			//プレイヤーの体力
#define PLAYER_STATE_COUNTER	(90)		//特殊状態の時間
#define FLASH_PLAYER_COUNT		(3)			//プレイヤーが点滅する間隔

#define PLAYER_SHADOW_SIZE		(10.0f)		//プレイヤーの影の大きさ

#define PLAYER_ROT_CORRECT_MOVE	(0.3f)		//プレイヤーの進行方向への向き修正の補正値
#define ERROR_PI				(3.14)		//移動向きにおける不適切な挙動をする角度
#define PLAYER_ERROR_ROT_COR	(0.01f)		//移動向きでエラーになった時の加算補正値

#define LIMIT_PLAYER_LEFT		(-600.0f)	//プレイヤーの移動限界地点　左
#define LIMIT_PLAYER_RIGHT		(600.0f)	//プレイヤーの移動限界地点　右
#define LIMIT_PLAYER_CEELING	(100.0f)	//プレイヤーの移動限界地点　上
#define LIMIT_PLAYER_FLOAR		(0.0f)		//プレイヤーの移動限界地点　下
#define LIMIT_PLAYER_FRONT		(600.0f)	//プレイヤーの移動限界地点　手前
#define LIMIT_PLAYER_BACK		(-600.0f)	//プレイヤーの移動限界地点　奥

//プレイヤーの重力マクロ
//上昇
#define PLAYER_RIZE_GRAVITY			(-3.0f)		//プレイヤーの上昇中にかかる重力
#define PLAYER_RIZE_GRA_MAG			(0.2f)		//プレイヤーの上昇中にかかる重力の倍率
//下降
#define PLAYER_FALL_GRAVITY			(-5.0f)		//プレイヤーの下降中にかかる重力
#define PLAYER_FALL_GRA_MAG			(0.4f)		//プレイヤーの下降中にかかる重力の倍率

//プレイヤーの弾の発射におけるマクロ
#define BULLET_COLOR					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0)	//弾の色
#define PLAYER_BULLET_SIZE				(4.0f)								//弾のサイズ
#define PLAYER_BULLET_SPEED				(-5.0f)								//弾の発射速度
#define PLAYER_BULLET_CORRECT_HEIGHT	(9.0f)								//弾の発射位置の補正値

//グローバル変数
LPDIRECT3DTEXTURE9			g_pTexturePlayer[PLAYER_TEX_MAX] = {};	//テクスチャへのポインタ
LPD3DXMESH					g_pMeshPlayer = NULL;					//メッシュ(頂点情報)へのポインタ
D3DXMATRIX					g_mtxWorldPlayer;						//ワールドマトリックス
LPD3DXBUFFER				g_pBuffMatPlayer = NULL;				//マテリアルへのポインタ
DWORD						g_dwNumMatPlayer = ZERO;				//マテリアルの数
Player						g_player;								//プレイヤーの情報

//--------------------------------------------------------
//					プレイヤーの初期化処理
//--------------------------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	g_pMeshPlayer = NULL;							//メッシュ初期化
	g_pBuffMatPlayer = NULL;						//マテリアルへのポインタ初期化
	g_dwNumMatPlayer = ZERO;						//マテリアルの数初期化
	ResetPlayer();									//プレイヤーの情報初期化
	g_player.state = PLAYERSTATE_APPEAR;			//出現状態に設定
	g_player.nStateCounter = PLAYER_STATE_COUNTER;	//状態カウンターを設定

	//Xファイル読み込み
	D3DXLoadMeshFromX("data/MODEL/floating_car.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatPlayer, NULL, &g_dwNumMatPlayer, &g_pMeshPlayer);

	//マテリアルデータへのポインタを取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntPlayer = START_CNT; nCntPlayer < (int)g_dwNumMatPlayer; nCntPlayer++)
	{
		if (pMat[nCntPlayer].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntPlayer].pTextureFilename, &g_pTexturePlayer[nCntPlayer]);
		}
	}
}

//--------------------------------------------------------
//					プレイヤーの終了処理
//--------------------------------------------------------
void UninitPlayer(void)
{
	//テクスチャの破棄
	for (int nCntPlayer = START_CNT; nCntPlayer < PLAYER_TEX_MAX; nCntPlayer++)
	{
		if (g_pTexturePlayer[nCntPlayer] != NULL)
		{
			g_pTexturePlayer[nCntPlayer]->Release();
			g_pTexturePlayer[nCntPlayer] = NULL;
		}
	}

	//メッシュの破棄
	if (g_pMeshPlayer != NULL)
	{
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatPlayer != NULL)
	{
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//--------------------------------------------------------
//					プレイヤーの更新処理
//--------------------------------------------------------
void UpdatePlayer(void)
{
	//前回の位置更新
	g_player.posOld = g_player.pos;

	//プレイヤーの状態による分岐処理
	SwitchPlayerState();

	//プレイヤーが死亡していない
	if (g_player.state != PLAYERSTATE_DEAD)
	{
		//プレイヤーの移動処理
		MovePlayer();

		//プレイヤーの回転処理
		RotPlayer();

		//位置更新
		g_player.pos += g_player.move;

		//プレイヤーの重力処理
		PlayerGravity();

		//プレイヤーの位置修正
		FixPlayerPos();

		//プレイヤーと他に対する当たり判定まとめ
		WholePlayerCollision();

		//弾発射
		if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_B)) == true)
		{
			//弾の移動向き・量
			D3DXVECTOR3 BulletMove;

			BulletMove.x = sinf(g_player.rot.y) * PLAYER_BULLET_SPEED;
			BulletMove.z = cosf(g_player.rot.y) * PLAYER_BULLET_SPEED;
			BulletMove.y = sinf(-g_player.rot.x) * PLAYER_BULLET_SPEED;

			SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + PLAYER_BULLET_CORRECT_HEIGHT, g_player.pos.z), BulletMove, BULLET_COLOR, PLAYER_BULLET_SIZE, BULLET_OWNER_PLAYER);
		}
	}
	
#if _DEBUG
	//プレイヤーのリセット処理
	if (GetKeyboardTrigger(DIK_N) == true)
	{
		ResetPlayer();
	}
#endif
}

//プレイヤーの状態による分岐処理
void SwitchPlayerState(void)
{
	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:	//通常状態
		break;

	case PLAYERSTATE_DAMEGE:	//ダメージ状態
	case PLAYERSTATE_APPEAR:	//出現状態
		//状態カウンター減少
		g_player.nStateCounter--;

		//カウンターがゼロになった
		if (g_player.nStateCounter <= ZERO)
		{
			g_player.bDisp = true;							//表示する
			g_player.state = PLAYERSTATE_NORMAL;			//通常状態に
			g_player.nStateCounter = PLAYER_STATE_COUNTER;	//カウンターを初期値に戻す
		}

		//状態継続中
		else if(g_player.nStateCounter % FLASH_PLAYER_COUNT == ZERO)
		{
			//プレイヤーを点滅させる
			g_player.bDisp = g_player.bDisp ? false : true;
		}

		break;

	case PLAYERSTATE_DEAD:		//死亡状態
		//表示しない
		g_player.bDisp = false;	

		//状態カウンター減少
		g_player.nStateCounter--;

		//カウンターがゼロになった
		if (g_player.nStateCounter <= ZERO)
		{
			//リザルト画面へ移行
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//プレイヤーの移動処理
void MovePlayer(void)
{
	float fCameraRot = GetCamera()->rot.y;	//カメラの角度代入
	float fMoveRotcor = F_ZERO;				//
	float fPi = F_ZERO;						//進行方向の角度を代入
	bool bMoveRot = false;					//進行方向に向かせるかどうか

	//左に移動・方向転換
	if ((GetKeyboardPress(DIK_A) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_LEFT)) == true)
	{
		//左奥に移動
		if ((GetKeyboardPress(DIK_W) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_UP)) == true)
		{
			g_player.move.x += sinf(LEFT_DOWN - fCameraRot) * PLAYER_SPEED;	//X方向に移動
			g_player.move.z += cosf(LEFT_DOWN - fCameraRot) * PLAYER_SPEED;	//Z方向に移動

			fPi = RIGHT_UP;		//右方向の角度を代入
		}
		
		//左手前に移動
		else if ((GetKeyboardPress(DIK_S) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN)) == true)
		{
			g_player.move.x += sinf(LEFT_UP  - fCameraRot) * PLAYER_SPEED;	//X方向に移動
			g_player.move.z += cosf(LEFT_UP - fCameraRot) * PLAYER_SPEED;	//Z方向に移動

			fPi = RIGHT_DOWN;		//右方向の角度を代入
		}

		//左に移動
		else
		{
			g_player.move.x += sinf(LEFT - fCameraRot) * PLAYER_SPEED;	//X方向に移動
			g_player.move.z += cosf(LEFT - fCameraRot) * PLAYER_SPEED;	//Z方向に移動

			fPi = RIGHT;		//右方向の角度を代入
		}
		
		bMoveRot = true;	//進行方向に向かせる
	}

	//右に移動・方向転換
	else if ((GetKeyboardPress(DIK_D) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_RIGHT)) == true)
	{
		//左奥に移動
		if ((GetKeyboardPress(DIK_W) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_UP)) == true)
		{
			g_player.move.x += sinf(RIGHT_DOWN - fCameraRot) * PLAYER_SPEED;	//X方向に移動
			g_player.move.z += cosf(RIGHT_DOWN - fCameraRot) * PLAYER_SPEED;	//Z方向に移動

			fPi = LEFT_UP;		//左方向の角度を代入
		}

		//左手前に移動
		else if ((GetKeyboardPress(DIK_S) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN)) == true)
		{
			g_player.move.x += sinf(RIGHT_UP - fCameraRot) * PLAYER_SPEED;	//X方向に移動
			g_player.move.z += cosf(RIGHT_UP - fCameraRot) * PLAYER_SPEED;	//Z方向に移動

			fPi = LEFT_DOWN;		//左方向の角度を代入
		}

		//左に移動
		else
		{
			g_player.move.x += sinf(RIGHT + fCameraRot) * PLAYER_SPEED;
			g_player.move.z += cosf(RIGHT - fCameraRot) * PLAYER_SPEED;

			fPi = LEFT;		//左方向の角度を代入
		}

		bMoveRot = true;	//進行方向に向かせる
	}

	//奥に移動・方向転換
	else if ((GetKeyboardPress(DIK_W) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_UP)) == true)
	{
		g_player.move.x += sinf(DOWN - fCameraRot) * PLAYER_SPEED;
		g_player.move.z += cosf(DOWN - fCameraRot) * PLAYER_SPEED;

		float f = sinf((DOWN - fCameraRot) - g_player.rot.y);

		fPi += UP;			//上方向の角度を代入
		bMoveRot = true;	//進行方向に向かせる
	}

	//手前に移動・方向転換
	else if ((GetKeyboardPress(DIK_S) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN)) == true)
	{
		//移動
		g_player.move.x += sinf(UP - fCameraRot) * PLAYER_SPEED;
		g_player.move.z += cosf(UP - fCameraRot) * PLAYER_SPEED;

		fPi += DOWN;			//下方向の角度を代入
		bMoveRot = true;	//進行方向に向かせる
	}

	//上昇
	if ((GetKeyboardTrigger(DIK_SPACE) || GetGamePadPress(XINPUT_GAMEPAD_A)) == true && g_player.bLand == true && g_player.bJump == false)
	{
		//g_player.move.y = PLAYER_JUMP;	//プレイヤーのジャンプ量を設定
		g_player.bLand = false;
		g_player.bJump = true;
	}

	//Rキーが押されていたら向きを固定する かつ 進行方向に向かせて良い
	if (GetKeyboardPress(DIK_R) == false && bMoveRot == true)
	{
		float fCorrectPlus = F_ZERO;	//向きの加算補正値

		//エラーの角度が算出された
		if (ERROR_PI <= fabsf((fPi - fCameraRot) - g_player.rot.y))
		{
			fCorrectPlus += PLAYER_ERROR_ROT_COR;
		}

		g_player.rot.y += sinf((fPi - fCameraRot) - g_player.rot.y) * PLAYER_ROT_CORRECT_MOVE + fCorrectPlus;	//移動方向に向かせる
	}

	//プレイヤーの移動量修正
	FixPlayerMove();

	//プレイヤーの移動慣性処理
	InertiaPlayerMove();
}

//プレイヤーの重力処理
void PlayerGravity(void)
{
	//プレイヤーが上昇中
	if (g_player.move.y > F_ZERO)
	{
		//徐々に上昇スピードを落とす
		g_player.move.y += (PLAYER_RIZE_GRAVITY - g_player.move.y) * PLAYER_RIZE_GRA_MAG;
	}

	//プレイヤーが下降中
	else
	{
		//徐々に上昇スピードを落とす
		g_player.move.y += (PLAYER_FALL_GRAVITY - g_player.move.y) * PLAYER_FALL_GRA_MAG;
	}
}

//プレイヤーの移動量修正
void FixPlayerMove(void)
{
	//プレイヤーの移動量が2.0f以上になった
	if (g_player.move.x >= MAX_SPEED)
	{
		g_player.move.x = MAX_SPEED;
	}

	//プレイヤーの移動量が -2.0f以下になった
	if (g_player.move.x <= -MAX_SPEED)
	{
		g_player.move.x = -MAX_SPEED;
	}

	//プレイヤーの移動量が2.0f以上になった
	if (g_player.move.z >= MAX_SPEED)
	{
		g_player.move.z = MAX_SPEED;
	}

	//プレイヤーの移動量が -2.0f以下になった
	if (g_player.move.z <= -MAX_SPEED)
	{
		g_player.move.z = -MAX_SPEED;
	}
}

//プレイヤーの移動慣性処理
void InertiaPlayerMove(void)
{
	g_player.move.x += (F_ZERO - g_player.move.x) * 0.1f;
	g_player.move.z += (F_ZERO - g_player.move.z) * 0.1f;
}

//プレイヤーの位置修正
void FixPlayerPos(void)
{
	//「移動限界地点　左」を超えた
	if (g_player.pos.x < LIMIT_PLAYER_LEFT)
	{
		g_player.pos.x = LIMIT_PLAYER_LEFT;
	}

	//「移動限界地点　右」を超えた
	if (g_player.pos.x > LIMIT_PLAYER_RIGHT)
	{
		g_player.pos.x = LIMIT_PLAYER_RIGHT;
	}

	//「移動限界地点　手前」を超えた
	if (g_player.pos.z > LIMIT_PLAYER_FRONT)
	{
		g_player.pos.z = LIMIT_PLAYER_FRONT;
	}

	//「移動限界地点　奥」を超えた
	if (g_player.pos.z < LIMIT_PLAYER_BACK)
	{
		g_player.pos.z = LIMIT_PLAYER_BACK;
	}
}

//プレイヤーの回転処理
void RotPlayer(void)
{
	//左旋回
	if ((GetKeyboardPress(DIK_LSHIFT) || GetGamePadPress(XINPUT_GAMEPAD_LEFT_THUMB)) == true)
	{
		g_player.rot.y += sinf(LEFT) * 0.1f;
	}

	//右旋回
	if ((GetKeyboardPress(DIK_RSHIFT) || GetGamePadPress(XINPUT_GAMEPAD_RIGHT_THUMB)) == true)
	{
		g_player.rot.y += sinf(RIGHT) * 0.1f;
	}

	/*==============================================
						角度修正
	================================================*/
	//--------------------------------
	//		角度が3.14を上回った
	//--------------------------------
	// X の角度判定
	if (g_player.rot.x >= D3DX_PI)
	{
		g_player.rot.x -= D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	// Y の角度判定
	if (g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	// Z の角度判定
	if (g_player.rot.z >= D3DX_PI)
	{
		g_player.rot.z -= D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	//---------------------------------
	//		角度が-3.14を下回った
	//---------------------------------
	// X の角度判定
	if (g_player.rot.x <= -D3DX_PI)
	{
		g_player.rot.x += D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	// Y の角度判定
	if (g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	// Z の角度判定
	if (g_player.rot.z <= -D3DX_PI)
	{
		g_player.rot.z += D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}
}

//プレイヤーのリセット処理
void ResetPlayer(void)
{
	g_player.pos			= PLAYER_START_POS;		// 位置初期化
	g_player.posOld			= ZERO_SET;				// 前回の位置初期化
	g_player.move			= ZERO_SET;				// 移動量初期化
	g_player.rot			= ZERO_SET;				// 向き初期化
	g_player.state			= PLAYERSTATE_NORMAL;	// プレイヤーの状態初期化
	g_player.pField			= NULL;					// フィールドの情報取得
	g_player.nStateCounter	= ZERO;					// 状態カウンター初期化
	g_player.nLife			= PLAYER_LIFE;			// プレイヤーの体力初期化
	g_player.bLand			= true;					// 着地している
	g_player.bJump			= false;				// ジャンプしていない
	g_player.bDisp			= true;					// 表示する
}

//全体の当たり判定処理
void WholePlayerCollision(void)
{
	//当たり判定によってジャンプできるか
	if ((CollisionSlope(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.rot, COLLITYPE_PLAYER) ||	//坂道の当たり判定処理
		ColliMeshField(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.pField)) == true)	//メッシュフィールドとの当たり判定
	{
		g_player.bLand = true;
		g_player.bJump = false;
	}

	else
	{
		g_player.bLand = false;
		g_player.bJump = true;
	}

	//坂道の壁との当たり判定

	//壁との当たり判定
	CollisionWall(&g_player.pos, &g_player.posOld, &g_player.move);
	
	//モデルとの当たり判定
	CollisionModel(&g_player.pos, &g_player.posOld, &g_player.move, COLLITYPE_PLAYER);

	//大砲の当たり判定
	CollisionCannon(&g_player.pos, &g_player.posOld, &g_player.move, COLLITYPE_PLAYER, &g_player.bLand, &g_player.bJump);
}

//--------------------------------------------------------
//					プレイヤーの描画処理
//--------------------------------------------------------
void DrawPlayer(void)
{
	//プレイヤーの表示が	ON
	if (g_player.bDisp == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
		
		/*------------------------------------------------------------------
								プレイヤーの描画設定
		--------------------------------------------------------------------*/
		D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
		D3DMATERIAL9 matDef;						//現在のマテリアル保存用
		D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
		D3DXCOLOR pMatStrage;						//マテリアルデータ保存用
		BillTest *pBill = GetAlphaTest();			//ビルボードのα・Zテスト情報を取得

													//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldPlayer);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);

		D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);

		D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
		{
			//保存用ストレージに代入
			pMatStrage = pMat[nCntMat].MatD3D.Diffuse;

			//ダメージ状態である
			if (g_player.state == PLAYERSTATE_DAMEGE)
			{
				//赤色に設定
				pMat[nCntMat].MatD3D.Diffuse = XCOL_RED;
			}

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTexturePlayer[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshPlayer->DrawSubset(nCntMat);

			//色を戻す
			pMat[nCntMat].MatD3D.Diffuse = pMatStrage;
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);

		/*------------------------------------------------------------------
									影の描画設定
		--------------------------------------------------------------------*/
		D3DXVECTOR3 pos = ZERO_SET;

		//フィールドのアドレスを持っている
		if (g_player.pField != NULL)
		{
			//持っているフィールドの情報の位置より、上にいる
			if (g_player.pField->pos.y <= g_player.pos.y)
			{
				pos = g_player.pField->pos;
			}

			//フィールドより下に居たら、アドレスを消して、影を描画しない
			else
			{
				g_player.pField = NULL;		//アドレスを消す
				return;						//影を描画せずに終了する
			}
		}

		else
		{
			return;
		}

		D3DXMATRIX	mtxShadow;		//シャドウマトリックス
		D3DLIGHT9	light;			//ライト情報
		D3DXVECTOR4	posLight;		//ライトの位置
		D3DXVECTOR3 normal;			//法線ベクトル
		D3DXPLANE	plane;			//平面情報

		//ライトの位置を設定
		pDevice->GetLight(0, &light);
		posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

		normal = SET_NOR;
		D3DXPlaneFromPointNormal(&plane, &pos, &normal);

		//シャドウマトリックスの初期化
		D3DXMatrixIdentity(&mtxShadow);

		//シャドウマトリックスの作成
		D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		D3DXMatrixMultiply(&mtxShadow, &g_mtxWorldPlayer, &mtxShadow);

		//シャドウマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
		{
			//保存用ストレージに代入
			pMatStrage = pMat[nCntMat].MatD3D.Diffuse;

			//赤色に設定
			pMat[nCntMat].MatD3D.Diffuse = XCOL_BLACK;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTexturePlayer[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshPlayer->DrawSubset(nCntMat);

			//色を戻す
			pMat[nCntMat].MatD3D.Diffuse = pMatStrage;
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);

	}
}

//プレイヤーの情報
Player *GetPlayer(void)
{
	return &g_player;
}

//プレイヤーと弾の当たり判定
bool ColliPlayerToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner)
{
	D3DXVECTOR3 posDiff = g_player.pos - pos;	//プレイヤーと弾の距離の差
	bool bCollision = false;					//あったかどうかの判定

	//プレイヤーと弾との距離が弾のサイズ内に収まる
	if (-size <= posDiff.x && posDiff.x <= size &&
		-size <= posDiff.y && posDiff.y <= size &&
		-size <= posDiff.z && posDiff.z <= size &&
		ZERO < g_player.nLife && owner == BULLET_OWNER_CANNON &&	//プレイヤーの体力が残っている	かつ	弾が大砲から放たれたもの
		g_player.state != PLAYERSTATE_DAMEGE)						//プレイヤーがダメージ状態ではない
	{
		g_player.nLife--;			//体力を減らす

		//体力が尽きてしまった
		if (g_player.nLife <= ZERO)
		{
			//死亡状態に
			g_player.state = PLAYERSTATE_DEAD;

			//ゲームオーバー
			SetEndGame(EndGAME_OVER);
		}

		//まだ体力がある
		else
		{
			//ダメージ状態に
			g_player.state = PLAYERSTATE_DAMEGE;
		}

		//ヒット音再生
		PlaySound(SOUND_LABEL_SE_HIT);

		bCollision = true;			//当たった
	}

	//当たったかどうかの判定を返す
	return bCollision;
}