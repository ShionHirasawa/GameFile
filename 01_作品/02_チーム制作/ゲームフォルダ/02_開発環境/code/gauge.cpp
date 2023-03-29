/*==========================================================================================================================================================

ゲージの処理[gauge.cpp]
Author:大宮愛羅

============================================================================================================================================================*/
#include "main.h"
#include "gauge.h"
#include "PvP_player.h"
#include "input.h"
#include "color.h"
#include "sound.h"

//マクロ定義 
#define NUM_GAUGE		(GAUGETYPE_MAX * MAX_USE_GAMEPAD)	//ゲージ４人分の全体数

//初期化関連
#define MIN_COL			(0.0f)		//色の初期値

//幅のマクロ定義
#define GAUGE_WIDTH		(200.0f)	//ゲージ幅の初期値
#define GAUGE_HEIGHT	(50.0f)		//高さの最小値
#define MIN_DEPTH		(0.0f)		//奥行きの最小値

//rhw関連
#define MAX_RHW		(1.0f)	//rhwの最大値

//色関連
#define USE_RED		(1.0f)	//赤色を使用する
#define	NUSE_RED	(0.0f)	//赤色を使用しない
#define USE_GREEN	(1.0f)	//緑色を使用する
#define NUSE_GREEN	(0.0f)	//緑色を使用しない
#define USE_BLUE	(1.0f)	//青色を使用する
#define	NUSE_BLUE	(0.0f)	//青色を使用しない
#define	USE_ALPHA	(1.0f)	//アルファー値

//テクスチャ関連
#define	MAX_TEX		(1.0f)	//テクスチャの最大値
#define MIN_TEX		(0.0f)	//テクスチャの最小値

//更新関連
#define MAX_NUM		(1.0f)	//割られる数(最大値)
#define	UNQUAR_NUM	(0.9f)	//割る数(最大値-10)
#define UNHALF_NUM  (0.75f)	//割る数(最大値-25)
#define HALF_NUM	(0.5f)	//割る数(最大値-50)
#define QUAR_NUM	(0.25f)	//割る数(最大値-75)

//グローバル変数	
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGauge = NULL;					//バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureGauge[GAUGETYPE_MAX] = {};	//テクスチャへのポインタ(ゲージ分)
Gauge					g_Gauge[MAX_USE_GAMEPAD][GAUGETYPE_MAX];					//ゲージ情報

const D3DXVECTOR3		g_Pos[MAX_USE_GAMEPAD] =				//ゲージの位置
{
	D3DXVECTOR3(26.0f,286.0f,0.0f),
	D3DXVECTOR3(1054.0f,286.0f,0.0f),
	D3DXVECTOR3(26.0f,646.0f,0.0f),
	D3DXVECTOR3(1054.0f,646.0f,0.0f),
};

//初期化処理
void InitGauge(void)
{
	//ポインター取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,			//==============
		"data\\TEXTURE\\GaugeBg.png",			//ゲージ背景
		&g_pTextureGauge[GAUGETYPE_BG]);		//===========

	D3DXCreateTextureFromFile(pDevice,			//==============
		"data\\TEXTURE\\Gauge.png",				//ゲージ本体
		&g_pTextureGauge[GAUGETYPE_NORMAL]);	//===========

	D3DXCreateTextureFromFile(pDevice,			//==============
		"data\\TEXTURE\\GaugeFrame.png",		//ゲージの枠
		&g_pTextureGauge[GAUGETYPE_FRAME]);		//===========

												//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_GAUGE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGauge, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void **)&pVtx, 0);

	//４人全員のゲージの頂点情報初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		for (int nCntGauge = 0; nCntGauge < GAUGETYPE_MAX; nCntGauge++, pVtx += VTX_MAX)
		{//ゲージ分
			//位置を初期化
			g_Gauge[nCntPlayer][nCntGauge].pos = g_Pos[nCntPlayer];

			//頂点座標の設定
			switch (nCntGauge)
			{
			case GAUGETYPE_NORMAL:
				pVtx[VTX_LE_UP].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[VTX_RI_UP].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pVtx[VTX_LE_DO].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, GAUGE_HEIGHT, 0.0f);
				pVtx[VTX_RI_DO].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, GAUGE_HEIGHT, 0.0f);
				break;
			default:
				pVtx[VTX_LE_UP].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//左上
				pVtx[VTX_RI_UP].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(GAUGE_WIDTH, 0.0f, 0.0f);	//右上
				pVtx[VTX_LE_DO].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(0.0f, GAUGE_HEIGHT, 0.0f);	//左下
				pVtx[VTX_RI_DO].pos = g_Gauge[nCntPlayer][nCntGauge].pos + D3DXVECTOR3(GAUGE_WIDTH, GAUGE_HEIGHT, 0.0f);	//右下
				break;
			}

			//rhwの設定
			pVtx[VTX_LE_UP].rhw = MAX_RHW;
			pVtx[VTX_RI_UP].rhw = MAX_RHW;
			pVtx[VTX_LE_DO].rhw = MAX_RHW;
			pVtx[VTX_RI_DO].rhw = MAX_RHW;

			//頂点カラー設定
			pVtx[VTX_LE_UP].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(USE_RED, USE_GREEN, USE_BLUE, USE_ALPHA);

			//テクスチャ情報の設定
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(MIN_TEX, MIN_TEX);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_TEX, MIN_TEX);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(MIN_TEX, MAX_TEX);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_TEX, MAX_TEX);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffGauge->Unlock();
}

//終了処理
void UninitGauge(void)
{
	for (int nCntGauge = 0; nCntGauge < GAUGETYPE_MAX; nCntGauge++)
	{//ゲージ分
	 //テクスチャの破棄
		if (g_pTextureGauge[nCntGauge] != NULL)
		{
			g_pTextureGauge[nCntGauge]->Release();
			g_pTextureGauge[nCntGauge] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//更新処理
void UpdateGauge(void)
{
	Player *pPlayer = GetPlayer();		//プレイヤー情報の取得	

	VERTEX_2D *pVtx;					//ポインタを設定

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void **)&pVtx, 0);

	//プレイヤー１のゲージにポインタを合わせる
	pVtx += VTX_MAX;

	for (int nCntGauge = 0; nCntGauge < MAX_USE_GAMEPAD; nCntGauge++, pPlayer++, pVtx += NUM_GAUGE)
	{//ゲージ分
	 //計算(今のゲージ量　/　最大ゲージ量)
		float Parcent = pPlayer->moveGauge / PLAYER_POWER_MAX;

		if (Parcent > UNQUAR_NUM)
		{//赤色
		 //頂点カラー設定
			pVtx[VTX_LE_UP].col = RGBA_RED;
			pVtx[VTX_RI_UP].col = RGBA_RED;
			pVtx[VTX_LE_DO].col = RGBA_RED;
			pVtx[VTX_RI_DO].col = RGBA_RED;
		}

		else if (Parcent > UNHALF_NUM)
		{//オレンジ色
		 //頂点カラー設定
			pVtx[VTX_LE_UP].col = RGBA_ORANGE;
			pVtx[VTX_RI_UP].col = RGBA_ORANGE;
			pVtx[VTX_LE_DO].col = RGBA_ORANGE;
			pVtx[VTX_RI_DO].col = RGBA_ORANGE;
		}

		else if (Parcent > HALF_NUM)
		{//青
		 //頂点カラー設定
			pVtx[VTX_LE_UP].col = RGBA_BLUE;
			pVtx[VTX_RI_UP].col = RGBA_BLUE;
			pVtx[VTX_LE_DO].col = RGBA_BLUE;
			pVtx[VTX_RI_DO].col = RGBA_BLUE;
		}

		else if (Parcent > QUAR_NUM)
		{//水色
		 //頂点カラー設定
			pVtx[VTX_LE_UP].col = RGBA_RIGHTBLUE;
			pVtx[VTX_RI_UP].col = RGBA_RIGHTBLUE;
			pVtx[VTX_LE_DO].col = RGBA_RIGHTBLUE;
			pVtx[VTX_RI_DO].col = RGBA_RIGHTBLUE;
		}

		else
		{//白色
		 //頂点カラー設定
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos.x = g_Gauge[nCntGauge][1].pos.x + 0.0f;						//左上
		pVtx[VTX_RI_UP].pos.x = g_Gauge[nCntGauge][1].pos.x + GAUGE_WIDTH * Parcent;	//右上
		pVtx[VTX_LE_DO].pos.x = g_Gauge[nCntGauge][1].pos.x + 0.0f;						//左下
		pVtx[VTX_RI_DO].pos.x = g_Gauge[nCntGauge][1].pos.x + GAUGE_WIDTH * Parcent;	//右下
	}

	//頂点バッファをロックする
	g_pVtxBuffGauge->Unlock();
}

//描画処理
void DrawGauge(void)
{
	//ポインター取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < NUM_GAUGE; nCntGauge++)
	{//ゲージ分
	 //テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGauge[nCntGauge % GAUGETYPE_MAX]);

		//ポリゴンの描画	
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * VTX_MAX, 2);
	}
}