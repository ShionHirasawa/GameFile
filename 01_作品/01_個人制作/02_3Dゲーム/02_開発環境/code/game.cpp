/*==========================================================================================================================================================

														ゲーム画面処理[game.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "color.h"
#include "particle.h"
#include "model.h"
#include "shadow.h"
#include "player.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "meshfield.h"
#include "slope.h"
#include "slopewall.h"
#include "fade.h"
#include "game.h"
#include "pause.h"
#include "cannon.h"
#include "sound.h"
#include "life.h"
#include "grassland.h"
#include "score.h"
#include "timelimit.h"
#include "logthorns.h"
#include "launchpad.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;			// 頂点バッファへのポインタ
bool					g_bFPS = true;					// FPSを表示するかどうか
bool					g_bPause = false;				// ポーズ
EndGAME					g_EndGame;						// ゲーム終了の理由

#if _DEBUG
PLAYMODE		g_PlayMode = PLAYMODE_MODEL;//プレイモード設定
#else
PLAYMODE		g_PlayMode = PLAYMODE_MAX;	//プレイモード設定
#endif

//------------------------------------------------
//				ゲームの初期化処理
//------------------------------------------------
void InitGame(void)
{
	InitBillboard();	// ビルボードの初期化処理
	InitGrassLand();	// 草原の初期化処理
	InitShadow();		// 影の初期化処理
	InitPlayer();		// プレイヤーの初期化処理
	InitCamera();		// カメラの初期化処理
	InitLight();		// ライトの初期化処理
	InitWall();			// 壁の初期化処理
	InitParticle();		// パーティクルの初期化処理
	InitBullet();		// 弾の初期化処理
	InitExplosion();	// 爆発の初期化処理
	InitEffect();		// エフェクトの初期化処理
	InitMeshField();	// メッシュフィールドの初期化処理
	InitModel();		// モデルの初期化処理
	InitCannon();		// 大砲の初期化処理
	InitLogThorns();	// 丸太トゲの初期化処理
	InitLaunchPad();	// 発射台の初期化処理
	InitSlope();		// 坂道の初期化処理
	InitSlopeWall();	// 坂道の壁の初期化処理

	InitPause();		// ポーズ画面初期化
	InitLife();			// ライフアイコンの初期化
	InitScore();		// スコアの初期化処理
	InitTimeLimit();	// 制限時間の初期化処理

	g_bPause			= false;			// ポーズの初期化
	g_bFPS				= false;			// FPS表示の初期化
	g_EndGame			= EndGAME_CLEAR;	// ゲーム終了理由の初期化

	//ゲームBGM開始
	PlaySound(SOUND_LABEL_GAMEBGM);
}

//------------------------------------------------
//				ゲームの終了処理
//------------------------------------------------
void UninitGame(void)
{
	/*======================================================================
							各種オブジェクトの終了処理
	========================================================================*/
	
	/*エフェクトなどの板ポリゴンの終了処理*/
	UninitParticle();	// パーティクルの終了処理
	UninitExplosion();	// 爆発の終了処理
	UninitEffect();		// エフェクトの終了処理
	UninitShadow();		// 影の終了処理

	/*UI関係の終了処理*/
	UninitPause();		// ポーズ画面の終了処理
	UninitLife();		// ライフアイコンの終了処理
	UninitScore();		// スコアの終了処理
	UninitTimeLimit();	// 制限時間の終了処理

	/*ステージ関係の終了処理*/
	UninitGrassLand();	// 草原の終了処理
	UninitWall();		// 壁の終了処理
	UninitBillboard();	// ビルボードの終了処理
	UninitMeshField();	// メッシュフィールドの終了処理
	UninitSlopeWall();	// 坂道の壁の終了処理
	UninitSlope();		// 坂道の終了処理
	UninitLight();		// ライトの終了処理

	UninitCamera();		// カメラの終了処理
	UninitPlayer();		// プレイヤーの終了処理
	UninitBullet();		// 弾の終了処理
	UninitModel();		// モデルの終了処理
	UninitCannon();		// 大砲の終了処理
	UninitLogThorns();	// 丸太トゲの終了処理
	UninitLaunchPad();	// 発射台の終了処理

	//ゲームBGM停止
	StopSound(SOUND_LABEL_GAMEBGM);
	StopSound(SOUND_LABEL_GAMEBGM_SPEED);
}

//------------------------------------------------
//				ゲームの更新処理
//------------------------------------------------
void UpdateGame(void)
{
	FADE fadeState = GetFade();
	Player *pPlayer = GetPlayer();

	//ポーズがOFF
	if (g_bPause == false)
	{
#ifdef _DEBUG
		//プレイモードによる更新処理分岐
		switch (g_PlayMode)
		{
		case PLAYMODE_MODEL:
			UpdatePlayer();		//プレイヤーの更新処理
			UpdateCamera();		//カメラの更新処理
			UpdateModel();		//モデルの更新処理
			break;

		case PLAYMODE_FIELD:
			break;

		case PLAYMODE_WALL:
			break;

		case PLAYMODE_CAMERA:
			UpdateCamera();			//カメラの更新処理
			break;

		case PLAYMODE_STAGE:
			UpdateMeshField();		//メッシュフィールドの更新処理
			UpdateWall();			//壁の更新処理
			break;

			//全部更新
		case PLAYMODE_MAX:
			UpdateModel();			//モデルの更新処理
			UpdatePlayer();			//プレイヤーの更新処理
			UpdateCamera();			//カメラの更新処理
			break;
		}
#else
		UpdatePlayer();		//プレイヤーの更新処理
		UpdateCamera();		//カメラの更新処理
		UpdateModel();		//モデルの更新処理
		UpdateMeshField();	//メッシュフィールドの更新処理
		UpdateWall();		//壁の更新処理
#endif

		UpdateCannon();			// 大砲の更新処理
		UpdateLogThorns();		// 丸太トゲの更新処理
		UpdateLaunchPad();		// 発射台の更新処理
		UpdateSlope();			// 坂道の更新処理
		UpdateSlopeWall();		// 坂道の壁の更新処理
		UpdateBullet();			// 弾の更新処理
		UpdateExplosion();		// 爆発の更新処理
		UpdateEffect();			// エフェクトの更新処理
		UpdateParticle();		// パーティクルの更新処理
		UpdateShadow();			// 影の更新処理
		UpdateLife();			// ライフアイコンの更新処理
		UpdateScore();			// スコアの更新処理
		UpdateTimeLimit();		// 制限時間の更新処理
	}

	else
	{
		//ポーズ画面の更新処理
		UpdatePause();

		//ポーズを解除するときにメニューをコンティニューに設定
		if (GetKeyboardTrigger(DIK_P) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true)
		{
			SetPause(PAUSE_MENU_CONTINUE);
		}

		if (*GetPause() == PAUSE_MENU_CONTINUE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
			{
				g_bPause = false;
			}
		}
	}

	UpdateLight();			// ライトの更新処理

#ifdef _DEBUG
	// プレイモード変更
	PlayMode();
#endif

	//ポーズ状態切り替え
	if (GetKeyboardTrigger(DIK_P) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true)
	{
		g_bPause = g_bPause ? false : true;
	}
}

//------------------------------------------------
//				ゲームの描画処理
//------------------------------------------------
void DrawGame(void)
{
	/*======================================================================
							各種オブジェクトの描画処理
	========================================================================*/
	//ゲーム内オブジェクトの描画処理
	SetCamera();		// カメラの設定処理
	
	DrawMeshField();	// メッシュフィールドの描画処理
	DrawGrassLand();	// 草原の描画処理
	DrawSlope();		// 坂道の描画処理
	DrawSlopeWall();	// 坂道の壁の描画処理
	DrawWall();			// 壁の描画処理

	DrawModel();		// モデルの描画処理
	DrawCannon();		// 大砲の描画処理
	DrawLogThorns();	// 丸太トゲの描画処理
	DrawLaunchPad();	// 発射台の描画処理

	DrawShadow();		// 影の描画処理
	DrawLife();			// ライフアイコンの描画処理
	DrawScore();		// スコアの描画処理
	DrawTimeLimit();	// 制限時間の描画処理

	DrawEffect();		// エフェクトの描画処理
	DrawExplosion();	// 爆発の描画処理
	DrawBullet();		// 弾の描画処理
	DrawParticle();		// パーティクルの描画処理

	DrawPlayer();		// プレイヤーの描画処理

#if _DEBUG
	//FPS表示をする
	if (g_bFPS == true)
	{
		DrawFPS(g_PlayMode);		//FPS描画処理
		//DrawFPSSecond(g_PlayMode);	//右側にデバッグ表示
	}

	//FPS表示 ON / OFF
	if (GetKeyboardTrigger(DIK_F1) == true)
	{
		g_bFPS = g_bFPS ? false : true;
	}
#endif

	//ポーズ画面の表示がON
	if (GetDispPause() == true && g_bPause == true)
	{
		DrawPause();		//ポーズ画面の描画処理
	}

	//ポーズ画面が表示されていなければカメラを移動させる
	else if (GetDispPause() == false)
	{
		MovePosV();		//視点移動
		SpinPosV();		//視点回転
	}
}

//プレイモード変更
void PlayMode(void)
{
	//プレイヤーモード変更
	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		switch (g_PlayMode)
		{
		case PLAYMODE_MODEL:
			g_PlayMode = PLAYMODE_FIELD;	//モデル　→　フィールド
			break;

		case PLAYMODE_FIELD:
			g_PlayMode = PLAYMODE_WALL;		//フィールド　→　壁
			break;

		case PLAYMODE_WALL:
			g_PlayMode = PLAYMODE_STAGE;	//壁　→　壁とフィールド
			break;

		case PLAYMODE_STAGE:
			g_PlayMode = PLAYMODE_CAMERA;	//壁とフィールド　→　カメラ
			break;

		case PLAYMODE_CAMERA:
			g_PlayMode = PLAYMODE_MAX;		//カメラ　→　全体
			break;

		case PLAYMODE_MAX:
			g_PlayMode = PLAYMODE_MODEL;	//全体　→　モデル
			break;
		}
	}

	//プレイモードを全体に
	if (GetKeyboardTrigger(DIK_F6) == true)
	{
		g_PlayMode = PLAYMODE_MAX;		//プレイモード 全体
	}
}

//ゲームが終了した理由を返す
void SetEndGame(EndGAME end)
{
	g_EndGame = end;
}

//ゲーム終了した理由を返す
int EndGameResult(void)
{
	return (int)g_EndGame;
}