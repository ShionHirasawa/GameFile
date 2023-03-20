/*==========================================================================================================================

タイトル画面の処理[title.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "title.h"
#include "input.h"
#include "player.h"
#include "move.h"

//マクロ定義
#define TITLE_POS		(D3DXVECTOR3(100.0f, 100.0f, 0.0f))	//タイトルの位置
#define TITLE_WIDTH		(600.0f)							//タイトルの横幅
#define TITLE_HEIGHT	(250.0f)							//タイトルの高さ
#define FADE_TITLE_A	(3)									//タイトルのα値の変化量

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureTitle = NULL;	//テクスチャのポインタ
D3DXVECTOR3 g_TitlePos;								//タイトルの位置
int g_aTexV;										//テクスチャのα値

//タイトルの初期化処理
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ + 取得

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/タイトル画面.png", &g_pTextureTitle);
		
	g_TitlePos = TITLE_POS;	//タイトルの位置初期化
	g_aTexV = ZERO;			//テクスチャのα値初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[ZERO].pos = g_TitlePos;
		pVtx[ONE].pos = D3DXVECTOR3(g_TitlePos.x + TITLE_WIDTH,   g_TitlePos.y, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_TitlePos.x,  TITLE_HEIGHT + g_TitlePos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_TitlePos.x + TITLE_WIDTH,   g_TitlePos.y + TITLE_HEIGHT, ORIGIN_POS);
	}

	//rhwの設定
	{
		pVtx[ZERO].rhw = RHW_NUM;
		pVtx[ONE].rhw = RHW_NUM;
		pVtx[SEC].rhw = RHW_NUM;
		pVtx[SAN].rhw = RHW_NUM;
	}

	//頂点カラーの設定   COLOR_MIN
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
	}

	//テクスチャ座標
	{
		pVtx[ZERO].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[ONE].tex  = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[SEC].tex  = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[SAN].tex  = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();
}

//タイトルの終了処理
void UninitTitle(void)
{
	//テクスチャの破棄
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//タイトルの更新処理
void UpdateTitle(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報

	//座標更新
	moveObject(&g_TitlePos);

	if (fabsf(pPlayer->pos.x - g_TitlePos.x) < 400.0f)
	{
		g_aTexV += FADE_TITLE_A;

		if (g_aTexV > COLOR_MAX)
		{
			g_aTexV = COLOR_MAX;
		}
	}

	else
	{
		g_aTexV -= FADE_TITLE_A;

		if (g_aTexV < COLOR_MIN)
		{
			g_aTexV = COLOR_MIN;
		}
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[ZERO].pos = g_TitlePos;
		pVtx[ONE].pos = D3DXVECTOR3(g_TitlePos.x + TITLE_WIDTH, g_TitlePos.y, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_TitlePos.x, TITLE_HEIGHT + g_TitlePos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_TitlePos.x + TITLE_WIDTH, g_TitlePos.y + TITLE_HEIGHT, ORIGIN_POS);
	}

	//頂点カラーの設定   COLOR_MIN
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
	}

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();
}

//タイトルの描画処理
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ + 取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTitle, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureTitle);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}