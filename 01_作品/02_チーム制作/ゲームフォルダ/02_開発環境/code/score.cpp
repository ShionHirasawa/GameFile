//============================================================================================
//
//スコアの処理[score.cpp]
//Author:藤原龍輝
//
//============================================================================================

#include"main.h"
#include"color.h"
#include"input.h"
#include"score.h"

//****************************//
//		　 マクロ定義		  //
//****************************//
#define NUM_SCORE		(4)		//スコアの数
#define NUM_PLASE		(2)		//スコア桁数

#define SCORE_MINUS_X		(230.0f)	//スコアのマイナスX座標
#define SCORE_PLUS_X		(270.0f)	//スコアのプラスX座標
#define SCORE_WIDTH			(30.0f)		//スコアの幅
#define SCORE_HEIGHT		(50.0f)		//スコアの高さ
#define SCORE_INTERVAL_X	(300.0f)	//スコア同士の間隔

#define SCORE_MINUS_Y		(600.0f)	//スコアのマイナスY座標
#define SCORE_PLUS_Y		(650.0f)	//スコアのプラスX座標

//******************************//
//		グローバル変数宣言		//
//******************************//
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
int g_aScore[NUM_SCORE];

const D3DXVECTOR3		g_PosScore[MAX_USE_GAMEPAD] =				//ゲージの位置
{
	D3DXVECTOR3(166.0f,236.0f,0.0f),
	D3DXVECTOR3(1194.0f,236.0f,0.0f),
	D3DXVECTOR3(166.0f,596.0f,0.0f),
	D3DXVECTOR3(1194.0f,596.0f,0.0f),
};

//===================================================
//スコアの初期化処理
//===================================================
void InitScore(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Number.png",
		&g_pTextureScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_PLASE * NUM_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_PLASE * NUM_SCORE; nCntScore++, pVtx += VTX_MAX)
	{
		pVtx[0].pos = pVtx[1].pos = pVtx[2].pos = pVtx[3].pos = ZERO_SET;

		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.0f;

		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = RGBA_WHITE;

		pVtx[0].tex = D3DXVECTOR2(NIL_F, NIL_F);
		pVtx[1].tex = D3DXVECTOR2(0.1f , NIL_F);
		pVtx[2].tex = D3DXVECTOR2(NIL_F, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f , 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}
//================================================
//スコア終了処理
//=================================================
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}
//=========================================================
//スコア更新処理
//=========================================================
void UpdateScore(void)
{
	
}
//======================================================
//スコア描画処理
//======================================================
void DrawScore(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntScore = 0; nCntScore < NUM_PLASE * NUM_SCORE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * VTX_MAX, 2);
	}
}
//==================================================
//スコアのセット処理
//==================================================
void SetScore(int nScore, int nPlayer)
{
	//テクスチャの幅を保存
	int aTexU[NUM_SCORE][NUM_PLASE];

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//スコアの設定
	for (int nCntPlayer = 0; nCntPlayer < nPlayer; nCntPlayer++)
	{
		g_aScore[nCntPlayer] = nScore;

		aTexU[nCntPlayer][0] = g_aScore[nCntPlayer] % 100 / 10;	//十の位を算出
		aTexU[nCntPlayer][1] = g_aScore[nCntPlayer] % 10;		//一の位を算出

		for (int nCntScore = 0; nCntScore < NUM_PLASE; nCntScore++, pVtx += VTX_MAX)
		{
			pVtx[0].pos = D3DXVECTOR3(g_PosScore[nCntPlayer].x + (nCntScore * SCORE_WIDTH), g_PosScore[nCntPlayer].y, NIL_F);
			pVtx[1].pos = D3DXVECTOR3(g_PosScore[nCntPlayer].x + ((nCntScore + 1) * SCORE_WIDTH), g_PosScore[nCntPlayer].y, NIL_F);
			pVtx[2].pos = D3DXVECTOR3(g_PosScore[nCntPlayer].x + (nCntScore * SCORE_WIDTH), g_PosScore[nCntPlayer].y + SCORE_HEIGHT, NIL_F);
			pVtx[3].pos = D3DXVECTOR3(g_PosScore[nCntPlayer].x + ((nCntScore + 1) * SCORE_WIDTH), g_PosScore[nCntPlayer].y + SCORE_HEIGHT, NIL_F);

			pVtx[0].tex = D3DXVECTOR2(NIL_F + (aTexU[nCntPlayer][nCntScore] * 0.1f), NIL_F);
			pVtx[1].tex = D3DXVECTOR2(0.1f  + (aTexU[nCntPlayer][nCntScore] * 0.1f), NIL_F);
			pVtx[2].tex = D3DXVECTOR2(NIL_F + (aTexU[nCntPlayer][nCntScore] * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f  + (aTexU[nCntPlayer][nCntScore] * 0.1f), 1.0f);
		}
	}

	//頂点バッファをロック
	g_pVtxBuffScore->Unlock();
}
//==============================================================
//スコアの加算処理
//===============================================================
void AddScore(int nValue, int nPlayer)
{
	int aTexU[NUM_SCORE][NUM_PLASE];
	int nCntScore;

	VERTEX_2D *pVtx;

	g_aScore[nPlayer] += nValue;

	aTexU[nPlayer][0] = g_aScore[nPlayer] % 100 / 10;
	aTexU[nPlayer][1] = g_aScore[nPlayer] % 10;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//対象の桁までポインタをスキップ
	pVtx += nPlayer * (VTX_MAX * NUM_PLASE);

	for (nCntScore = 0; nCntScore < NUM_PLASE; nCntScore++, pVtx += VTX_MAX)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nPlayer][nCntScore] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nPlayer][nCntScore] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nPlayer][nCntScore] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nPlayer][nCntScore] * 0.1f), 1.0f);
	}

	//頂点バッファをロック
	g_pVtxBuffScore->Unlock();
}