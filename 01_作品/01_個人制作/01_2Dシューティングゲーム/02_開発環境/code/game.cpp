/*==========================================================================================================================

ゲーム画面処理[game.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "game.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "hp.h"
#include "pause.h"
#include "particle.h"
#include "boss.h"
#include "beam.h"

//マクロ定義
//カウントダウン「３・２・１」の座標
#define COUNT_THREE_POS_ZERO	D3DXVECTOR3(590.0f, 300.0f, 0.0f)
#define COUNT_THREE_POS_ONE		D3DXVECTOR3(690.0f, 300.0f, 0.0f)
#define COUNT_THREE_POS_SEC		D3DXVECTOR3(590.0f, 400.0f, 0.0f)
#define COUNT_THREE_POS_SAN		D3DXVECTOR3(690.0f, 400.0f, 0.0f)

//カウントダウン「START!」の座標
#define COUNT_START_POS_ZERO	D3DXVECTOR3(490.0f, 300.0f, 0.0f)
#define COUNT_START_POS_ONE		D3DXVECTOR3(790.0f, 300.0f, 0.0f)
#define COUNT_START_POS_SEC		D3DXVECTOR3(490.0f, 400.0f, 0.0f)
#define COUNT_START_POS_SAN		D3DXVECTOR3(790.0f, 400.0f, 0.0f)

//カウントダウン「３」のテクスチャ座標
#define COUNT_THREE_TEX_ZERO	D3DXVECTOR2(0.0f, 0.0f)
#define COUNT_THREE_TEX_ONE		D3DXVECTOR2(0.2f, 0.0f)
#define COUNT_THREE_TEX_SEC		D3DXVECTOR2(0.0f, 0.5f)
#define COUNT_THREE_TEX_SAN		D3DXVECTOR2(0.2f, 0.5f)

//カウントダウン「2」のテクスチャ座標
#define COUNT_SEC_TEX_ZERO		D3DXVECTOR2(0.4f, 0.0f)
#define COUNT_SEC_TEX_ONE		D3DXVECTOR2(0.6f, 0.0f)
#define COUNT_SEC_TEX_SEC		D3DXVECTOR2(0.4f, 0.5f)
#define COUNT_SEC_TEX_SAN		D3DXVECTOR2(0.6f, 0.5f)

//カウントダウン「1」のテクスチャ座標
#define COUNT_ONE_TEX_ZERO		D3DXVECTOR2(0.8f, 0.0f)
#define COUNT_ONE_TEX_ONE		D3DXVECTOR2(1.0f, 0.0f)
#define COUNT_ONE_TEX_SEC		D3DXVECTOR2(0.8f, 0.5f)
#define COUNT_ONE_TEX_SAN		D3DXVECTOR2(1.0f, 0.5f)

//カウントダウン「START!」のテクスチャ座標
#define COUNT_START_TEX_ZERO	D3DXVECTOR2(0.0f, 0.5f)
#define COUNT_START_TEX_ONE		D3DXVECTOR2(1.0f, 0.5f)
#define COUNT_START_TEX_SEC		D3DXVECTOR2(0.0f, 1.0f)
#define COUNT_START_TEX_SAN		D3DXVECTOR2(1.0f, 1.0f)

#define COUNTDOWN_TEX			"data/TEXTURE/ゲーム開始カウントダウン.png"	//カウントダウンのテクスチャ
#define COUNTDOWN_NUM			(25)		//カウントダウンのタイミング
#define NUM_SET_ENEMY			(40)		//セットする敵の数
#define DECIDE_TYPE				(4)			//敵の種類を決める

//ゲーム終了のクリア判定の構造体
typedef enum
{
	RESULT_OVER = ZERO,
	RESULT_CLEAR,
	RESULT_MAX
}RESULT;

typedef enum
{
	COUNT_SAN = ZERO,
	COUNT_SEC,
	COUNT_ONE,
	COUNT_MAX,
}COUNTDOWN;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureGame = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;	//頂点バッファへのポインタ
GAMESTATE g_gameState = GAMESTATE_NONE;			//ゲームの状態
int g_nCounterGameState = ZERO;					//状態管理カウンター
RESULT g_numResult;								//リザルト分岐
bool g_bPause;									//ポーズ状態のON/OFF


void InitGame(void)
{
	//ゲーム画面に使われるオブジェクトの初期化処理
	InitBg();			//背景初期化
	InitPlayer();		//プレイヤーの初期化
	InitBullet();		//弾の初期化
	InitBeam();			//ビームの初期化
	InitExplosion();	//爆発の初期化
	InitEnemy();		//敵の初期化処理
	InitScore();		//スコアの初期化処理
	SetScore(ZERO);		//もちろんスコアの初期値は0
	InitEffect();		//エフェクトの初期化処理
	InitParticle();		//パーティクルの初期化
	InitHp();			//HPアイコンの初期化処理	
	InitPause();		//ポーズ画面の初期化処理
	InitBoss();			//ボスの初期化処理

	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

														//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, COUNTDOWN_TEX, &g_pTextureGame);

	g_gameState = GAMESTATE_START;			//ゲーム状態の初期化
	g_nCounterGameState = ZERO;				//状態管理カウンターの初期化
	g_bPause = false;						//ポーズの初期化

											//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGame, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGame->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[ZERO].pos = COUNT_THREE_POS_ZERO;
		pVtx[ONE].pos = COUNT_THREE_POS_ONE;
		pVtx[SEC].pos = COUNT_THREE_POS_SEC;
		pVtx[SAN].pos = COUNT_THREE_POS_SAN;
	}

	//rhwの設定
	{
		pVtx[ZERO].rhw = RHW_NUM;
		pVtx[ONE].rhw = RHW_NUM;
		pVtx[SEC].rhw = RHW_NUM;
		pVtx[SAN].rhw = RHW_NUM;
	}

	//頂点カラーの設定
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
	}

	//テクスチャの座標
	{
		pVtx[ZERO].tex = COUNT_THREE_TEX_ZERO;
		pVtx[ONE].tex = COUNT_THREE_TEX_ONE;
		pVtx[SEC].tex = COUNT_THREE_TEX_SEC;
		pVtx[SAN].tex = COUNT_THREE_TEX_SAN;
	}

	//頂点バッファをロックする
	g_pVtxBuffGame->Unlock();

	//ランダム関数のシードを時刻より設定
	srand((unsigned int)time(NULL));

	//敵の配置
	for (int nCnt = START_CNT; nCnt < NUM_SET_ENEMY; nCnt++)
	{
		SetEnemy();
	}

	StopSound(SOUND_LABEL_BGM000);	//タイトルBGMを終了
	PlaySound(SOUND_LABEL_BGM001);	//ゲーム用BGM開始
}

void UninitGame(void)
{
	/*======================================================================
	各種オブジェクトの終了処理
	========================================================================*/
	//ゲーム画面に使ったオブジェクトの終了処理
	UninitBg();			//背景の終了処理
	UninitPlayer();		//プレイヤーの終了処理
	UninitBullet();		//弾の終了処理	
	UninitBeam();		//ビームの初期化
	UninitExplosion();	//爆発の終了処理	
	UninitEnemy();		//敵の終了処理
	UninitBoss();		//ボスの終了処理
	UninitScore();		//スコアの終了処理	
	UninitEffect();		//エフェクトの終了処理
	UninitParticle();	//パーティクルの終了処理
	UninitHp();			//HPアイコンの終了処理
	UninitPause();		//ポーズ画面の終了処理
}

void UpdateGame(void)
{
	FADE fadeState = GetFade();
	//敵 or 自分が全滅
	Player *pPlayer = GetPlayer();
	int NumEnemy = GetNumEnemy();

	//ポーズ中ではない
	if (g_bPause == false)
	{
		/*======================================================================
		各種オブジェクトの更新処理
		========================================================================*/
		UpdateBg();			//背景の更新処理
		UpdatePlayer();		//プレイヤーの更新処理
		UpdateBullet();		//弾の更新処理
		UpdateBeam();		//ビームの更新処理
		UpdateExplosion();	//爆発の更新処理
		UpdateEnemy();		//敵の更新処理
		UpdateScore();		//スコアの更新処理
		UpdateEffect();		//エフェクトの更新処理
		UpdateParticle();	//パーティクルの更新処理
		UpdateHp();			//Hpアイコンの更新処理

							//ボス出現
		if (NumEnemy <= ZERO)
		{
			UpdateBoss();

			BOSS *pBoss = GetBoss();

			if (pBoss->nLife <= ZERO)
			{
				g_numResult = RESULT_CLEAR;
			}
		}
	}

	//ポーズ中であり、敵を倒した余韻中ではない
	else
	{
		int numEnemy = GetNumEnemy();

		//ポーズを解除するときにメニューをコンティニューに設定
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			SetPause(PAUSE_MENU_CONTINUE);
		}

		if (g_nCounterGameState == ZERO)
		{
			UpdatePause();

			PAUSE_MENU *pPause = GetPause();

			if (*pPause == PAUSE_MENU_CONTINUE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					g_bPause = false;
				}
			}
		}

		else
		{
			g_bPause = false;
		}
	}

	if (g_gameState == GAMESTATE_START)
	{
		//ポインタを設定
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffGame->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

		switch (pPlayer->AppearCnt / COUNTDOWN_NUM)
		{
		case COUNT_SAN:				//カウントダウン「3」
			break;

		case COUNT_SEC:				//カウントダウン「2」
									//テクスチャの座標
		{
			pVtx[ZERO].tex = COUNT_SEC_TEX_ZERO;
			pVtx[ONE].tex = COUNT_SEC_TEX_ONE;
			pVtx[SEC].tex = COUNT_SEC_TEX_SEC;
			pVtx[SAN].tex = COUNT_SEC_TEX_SAN;
		}
		break;

		case COUNT_ONE:				//カウントダウン「1」
									//テクスチャの座標
		{
			pVtx[ZERO].tex = COUNT_ONE_TEX_ZERO;
			pVtx[ONE].tex = COUNT_ONE_TEX_ONE;
			pVtx[SEC].tex = COUNT_ONE_TEX_SEC;
			pVtx[SAN].tex = COUNT_ONE_TEX_SAN;
		}
		break;

		default:			//「START!」
							//頂点座標の設定
		{
			pVtx[ZERO].pos = COUNT_START_POS_ZERO;
			pVtx[ONE].pos = COUNT_START_POS_ONE;
			pVtx[SEC].pos = COUNT_START_POS_SEC;
			pVtx[SAN].pos = COUNT_START_POS_SAN;
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = COUNT_START_TEX_ZERO;
			pVtx[ONE].tex = COUNT_START_TEX_ONE;
			pVtx[SEC].tex = COUNT_START_TEX_SEC;
			pVtx[SAN].tex = COUNT_START_TEX_SAN;
		}

		if (pPlayer->state == PLAYERSTATE_NORMAL)
		{
			g_gameState = GAMESTATE_NORMAL;
		}
		break;
		}

		//頂点バッファをロックする
		g_pVtxBuffGame->Unlock();
	}

	//プレイヤーの体力が０になった
	if (pPlayer->nLife <= ZERO)
	{
		g_numResult = RESULT_OVER;
	}

	//ゲームの状態
	switch (g_gameState)
	{
	case GAMESTATE_START:
		break;

	case GAMESTATE_NORMAL:	//通常状態
		break;

	case GAMESTATE_END:		//終了状態
		g_nCounterGameState--;

		if (g_nCounterGameState == ZERO)
		{
			g_gameState = GAMESTATE_NONE;		//何もしていない状態

												//フェード設定（リザルトへ移行)
			SetFade(MODE_RESULT);
		}
		break;
	}

	//ポーズキーのON/OFF
	if (GetKeyboardTrigger(DIK_P) == true && g_nCounterGameState == ZERO)
	{//Pキーが押された & 敵を倒した余韻中ではない
		SetPause(PAUSE_MENU_CONTINUE);	//ポーズ中でないときはポーズメニューをコンティニューに更新

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
	DrawPlayer();	//プレイヤーの描画処理
	DrawEffect();	//エフェクトの描画処理
	DrawParticle();	//パーティクルの描画処理
	DrawBullet();	//弾の描画処理7
	DrawBeam();		//ビームの描画処理
	DrawExplosion();//爆発の描画処理
	DrawEnemy();	//敵の描画処理
	DrawBoss();		//ボスの描画処理
	DrawScore();	//スコアの描画処理
	DrawHp();		//Hpの描画処理

					//ゲーム開始のカウントダウン
	if (g_gameState == GAMESTATE_START)
	{
		LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

															//デバイスの取得
		pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(ZERO, g_pVtxBuffGame, ZERO, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureGame);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
	}

	if (g_bPause == true)
	{
		DrawPause();
	}
}

void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterGameState = nCounter;
}

int ResultChoice(void)
{
	return g_numResult;
}

GAMESTATE GetGameState(void)
{
	return g_gameState;
}