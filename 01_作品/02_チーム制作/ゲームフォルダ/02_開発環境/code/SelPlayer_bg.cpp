//==========================================
//
//背景表示プログラム[SelPlayer_bg.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureBg = {};	//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBg = NULL;	//頂点バッファポインタ

//========================
//背景初期化処理
//========================
void InitSelPlayer_Bg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//背景読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PlayerSelect_BG.png",
		&g_apTextureBg);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBg,
		NULL);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBg->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 200.0f);
	pVtx[1].pos = D3DXVECTOR3((float)SCREEN_WIDTH, 0.0f, 200.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, (float)SCREEN_HEIGHT, 200.0f);
	pVtx[3].pos = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 200.0f);

	//座標変換用係数設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//頂点バッファをアンロック
	g_pVtxbuffBg->Unlock();
}

//========================
//背景終了処理
//========================
void UninitSelPlayer_Bg(void)
{
	//テクスチャの破棄
	if (g_apTextureBg != NULL)
	{
		g_apTextureBg->Release();
		g_apTextureBg = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxbuffBg != NULL)
	{
		g_pVtxbuffBg->Release();
		g_pVtxbuffBg = NULL;
	}
}

//========================
//背景更新処理
//========================
void UpdateSelPlayer_Bg(void)
{
	
}

//========================
//背景描画処理
//========================
void DrawSelPlayer_Bg(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_apTextureBg);
	
	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}