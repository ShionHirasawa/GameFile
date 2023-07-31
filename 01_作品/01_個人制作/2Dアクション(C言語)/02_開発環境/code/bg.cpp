/*==========================================================================================================================

ゲーム背景の処理[bg.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "bg.h"
#include "input.h"

//マクロ定義
#define NUM_BG		(3)			//背景の数

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBg = NULL;							//頂点バッファへのポインタ		

//背景の初期化処理
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ + 取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_BG, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
		pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);
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
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MIN, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
	}

	//頂点バッファをロックする
	g_pVtxBuffBg->Unlock();
}

//背景の終了処理
void UninitBg(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//背景の更新処理
void UpdateBg(void)
{
	
}

//背景の描画処理
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ + 取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBg, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	//テクスチャの設定
	pDevice->SetTexture(ZERO, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);	
}