/*==========================================================================================================================

ゲーム画面処理[game.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "game.h"
#include "input.h"
#include "player.h"
#include "hako.h"
#include "bg.h"
#include "title.h"
#include "tutorial.h"
#include "door.h"
#include "move.h"
#include "block.h"
#include "fade.h"
#include "pause.h"
#include "particle.h"

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;	//頂点バッファへのポインタ
GAMESTATE g_gameState = GAMESTATE_NONE;			//ゲームの状態
int g_nCounterGameState = ZERO;					//状態管理カウンター
bool g_bPause;									//ポーズ状態のON/OFF

void InitGame(void)
{
	//ゲーム画面に使われるオブジェクトの初期化処理
	InitMove();		//移動処理の初期化処理
	InitPlayer();	//プレイヤーの初期化処理
	InitBg();		//背景の初期化処理
	InitTitle();	//タイトルの初期化
	InitTutorial();	//チュートリアルの初期化
	InitDoor();		//扉の初期化処理
	InitBlock();	//ブロックの初期化処理
	InitHako();		//ハコ初期化
	InitParticle();	//パーティクルの初期化
	InitPause();	//ポーズ画面の初期化

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_gameState = GAMESTATE_START;			//ゲーム状態の初期化
	g_nCounterGameState = ZERO;				//状態管理カウンターの初期化
	g_bPause = false;						//ポーズの初期化
}

void UninitGame(void)
{
	/*======================================================================
							各種オブジェクトの終了処理
	========================================================================*/
	//ゲーム画面に使ったオブジェクトの終了処理
	UninitBlock();		//ブロックの終了処理
	UninitPlayer();		//プレイヤーの終了処理
	UninitTitle();		//タイトル終了
	UninitTutorial();	//チュートリアルの終了処理
	UninitBg();			//背景の終了処理
	UninitDoor();		//扉の終了処理
	UninitHako();		//ハコ終了処理
	UninitParticle();	//パーティクルの終了処理
	UninitPause();		//ボーズの終了処理
}

void UpdateGame(void)
{
	FADE fadeState = GetFade();
	Player *pPlayer = GetPlayer();

	if (g_bPause == false)
	{
		UpdatePlayer();		//プレイヤーの更新処理
		UpdateHako();		//プレイヤーのハコ更新処理
		UpdateDoor();		//扉の更新処理
		UpdateBlock();		//ブロックの更新処理
		UpdateTitle();		//タイトルの更新処理
		UpdateTutorial();	//チュートリアルの更新処理
		UpdateBg();			//背景の更新処理
		UpdateParticle();	//パーティクルの更新処理
	}
	else
	{
		UpdatePause();		//ポーズ画面の更新処理

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

	//ポーズ状態切り替え
	if (GetKeyboardTrigger(DIK_P) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true)
	{
		g_bPause = g_bPause ? false : true;
	}
}

void DrawGame(void)
{
	/*======================================================================
							各種オブジェクトの描画処理
	========================================================================*/
	//ゲーム内オブジェクトの描画処理		
	DrawBg();		//背景の描画処理
	DrawTitle();	//タイトル画面描画
	DrawTutorial();	//チュートリアルの描画処理
	DrawDoor();		//扉の描画処理
	DrawBlock();	//ブロックの描画処理
	DrawHako();		//プレイヤーのハコ描画処理
	DrawPlayer();	//プレイヤーの描画処理
	DrawParticle();	//パーティクルの描画処理
	DrawFPS();		//FPS表示
	
	//ポーズ中
	if (g_bPause == true)
	{
		DrawPause();		//ポーズ画面の描画処理
	}
}

void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterGameState = nCounter;
}

GAMESTATE GetGameState(void)
{
	return g_gameState;
}