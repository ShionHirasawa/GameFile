/*==========================================================================================================================================================

カメラが使われていなかったときのカーテン[curtain.h]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "curtain.h"
#include "color.h"

#define NUM_CURTAIN		(4)	//カーテンの数
#define PVP_CURTAIN_TEX	(6)	//PVPのカーテンテクスチャ
#define HDR_CURTAIN_TEX	(5)	//HDRのカーテンテクスチャ

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureCurtain[NUM_CURTAIN] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCurtain = NULL;			//頂点バッファへのポインタ

// PVPのカーテンテクスチャのファイルパス
const char *c_apPVPCurtainTex[PVP_CURTAIN_TEX] = {
	"data\\TEXTURE\\PVP_Curtain000.png",
	"data\\TEXTURE\\PVP_Curtain001.png",
	"data\\TEXTURE\\PVP_Curtain002.png",
	"data\\TEXTURE\\PVP_Curtain003.png",
	"data\\TEXTURE\\PVP_Curtain004.png",
	"data\\TEXTURE\\PVP_Curtain005.png",
};

// HDRのカーテンテクスチャのファイルパス
const char *c_apHDRCurtainTex[HDR_CURTAIN_TEX] = {
	"data\\TEXTURE\\HDR_Curtain000.png",
	"data\\TEXTURE\\HDR_Curtain001.png",
	"data\\TEXTURE\\HDR_Curtain002.png",
	"data\\TEXTURE\\HDR_Curtain003.png",
	"data\\TEXTURE\\HDR_Curtain004.png",
};

//-----------------------------------
//		PVPのカーテン初期化処理
//-----------------------------------
void InitSelModePvPCurtain(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_CURTAIN, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCurtain, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCurtain->Lock(0, 0, (void* *)&pVtx, 0);

	//========================
	//		頂点情報設定
	//========================
	for (int nCntCurtain = 0; nCntCurtain < NUM_CURTAIN; nCntCurtain++, pVtx += VTX_MAX)
	{
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, c_apPVPCurtainTex[rand() % PVP_CURTAIN_TEX], &g_pTextureCurtain[nCntCurtain]);

		//各画面の左上の原点座標を算出
		float Pos_X = (SCREEN_WIDTH / 2)  * (nCntCurtain % EVENPARITY);
		float Pos_Y = (SCREEN_HEIGHT / 2) * (nCntCurtain / EVENPARITY);

		for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
		{
			pVtx[nCntVtx].pos.x = Pos_X + (SCREEN_WIDTH / 2)  * (nCntVtx % EVENPARITY);
			pVtx[nCntVtx].pos.y = Pos_Y + (SCREEN_HEIGHT / 2) * (nCntVtx / EVENPARITY);
			pVtx[nCntVtx].pos.z = 0.0f;

			pVtx[nCntVtx].tex.x = 1.0f * (nCntVtx % EVENPARITY);
			pVtx[nCntVtx].tex.y = 1.0f * (nCntVtx / EVENPARITY);

			D3DXVECTOR2 pos = D3DXVECTOR2(pVtx[nCntVtx].pos.x, pVtx[nCntVtx].pos.y);
			D3DXVECTOR2 tex = pVtx[nCntVtx].tex;

			pVtx[nCntVtx].rhw = RHW;
			pVtx[nCntVtx].col = RGBA_WHITE;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffCurtain->Unlock();
}

//-----------------------------------
//		HDRのカーテン初期化処理
//-----------------------------------
void InitSelModeHDRCurtain(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_CURTAIN, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCurtain, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCurtain->Lock(0, 0, (void* *)&pVtx, 0);

	//========================
	//		頂点情報設定
	//========================
	for (int nCntCurtain = 0; nCntCurtain < NUM_CURTAIN; nCntCurtain++, pVtx += VTX_MAX)
	{
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, c_apHDRCurtainTex[rand() % HDR_CURTAIN_TEX], &g_pTextureCurtain[nCntCurtain]);

		//各画面の左上の原点座標を算出
		float Pos_X = (SCREEN_WIDTH / 4) * nCntCurtain;

		for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
		{
			pVtx[nCntVtx].pos.x = Pos_X + (SCREEN_WIDTH / 4) * (nCntVtx % EVENPARITY);
			pVtx[nCntVtx].pos.y = SCREEN_HEIGHT * (nCntVtx / EVENPARITY);
			pVtx[nCntVtx].pos.z = 0.0f;

			pVtx[nCntVtx].tex.x = 1.0f * (nCntVtx % EVENPARITY);
			pVtx[nCntVtx].tex.y = 1.0f * (nCntVtx / EVENPARITY);

			D3DXVECTOR2 pos = D3DXVECTOR2(pVtx[nCntVtx].pos.x, pVtx[nCntVtx].pos.y);
			D3DXVECTOR2 tex = pVtx[nCntVtx].tex;

			pVtx[nCntVtx].rhw = RHW;
			pVtx[nCntVtx].col = RGBA_WHITE;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffCurtain->Unlock();
}

//----------------------------
//		カーテン終了処理
//----------------------------
void UninitCurtain(void)
{
	//テクスチャ全破棄
	for (int nCntTex = 0; nCntTex < NUM_CURTAIN; nCntTex++)
	{
		if (g_pTextureCurtain[nCntTex] != NULL)
		{
			g_pTextureCurtain[nCntTex]->Release();
			g_pTextureCurtain[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCurtain != NULL)
	{
		g_pVtxBuffCurtain->Release();
		g_pVtxBuffCurtain = NULL;
	}
}

//----------------------------
//		カーテン更新処理
//----------------------------
void UpdateCurtain(void)
{

}

//----------------------------
//		カーテン描画処理
//----------------------------
void DrawCurtain(int nCntCurtain)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCurtain, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCurtain[nCntCurtain]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCurtain * VTX_MAX, 2);
}