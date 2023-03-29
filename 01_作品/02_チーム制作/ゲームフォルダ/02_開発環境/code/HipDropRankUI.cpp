/*==========================================================================================================================================================

ヒップドロップのランクUI処理[HipDropRankUI.h]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "hdr_player.h"
#include "HipDropRankUI.h"
#include "color.h"
#include "input.h"

//マクロ定義
#define RANK_UI_NUM			(4)				// ４人分
#define RANK_UI_LIFE		(80)			// 寿命
#define RANK_UI_WIDTH		(130.0f)		// 幅
#define RANK_UI_HEIGHT		(50.0f)			// 高さ
#define RANK_UI_TEX_HEIGHT	(0.2f)			// テクスチャの高さ
#define RANK_UI_TEX			"data/TEXTURE/HipDropRank.png"	//ヒップドロップ量のUIのテクスチャ名

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureHipDropRankUI = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHipDropRankUI = NULL;	//頂点バッファへのポインタ
HDRankUI				g_HDRankUI[RANK_UI_NUM];		//UI情報

//ランクUIの位置情報
const D3DXVECTOR3 c_RankUIPos[RANK_UI_NUM] = {
	D3DXVECTOR3(160.0f, 80.0f, 0.0f),
	D3DXVECTOR3(480.0f, 80.0f, 0.0f),
	D3DXVECTOR3(800.0f, 80.0f, 0.0f),
	D3DXVECTOR3(1120.0f,80.0f, 0.0f)
};

//------------------------------------------
//		ヒップドロップ量のUI初期化処理
//------------------------------------------
void InitHipDropRankUI(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, RANK_UI_TEX, &g_pTextureHipDropRankUI);

	//情報初期化
	for (int nCntRankUI = 0; nCntRankUI < RANK_UI_NUM; nCntRankUI++)
	{
		g_HDRankUI[nCntRankUI].pos = c_RankUIPos[nCntRankUI];	// 位置初期化
		g_HDRankUI[nCntRankUI].nLife = RANK_UI_LIFE;			// 寿命初期化
		g_HDRankUI[nCntRankUI].fWidth = RANK_UI_WIDTH;			// 幅初期化
		g_HDRankUI[nCntRankUI].fHeight = RANK_UI_HEIGHT;		// 高さ初期化
		g_HDRankUI[nCntRankUI].bDisp = false;					// 表示するか否か
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * RANK_UI_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHipDropRankUI, NULL);

	//頂点情報の初期化処理
	InitHipDropRankUIVertex();
}

//------------------------------------------
//	ヒップドロップ量のUI頂点情報初期化処理
//------------------------------------------
void InitHipDropRankUIVertex(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHipDropRankUI->Lock(0, 0, (void* *)&pVtx, 0);

	//UIのポインタを取得
	HDRankUI *pUI = &g_HDRankUI[0];

	for (int nCntVtx = 0; nCntVtx < RANK_UI_NUM; nCntVtx++, pVtx += VTX_MAX, pUI++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pUI->pos.x - pUI->fWidth, pUI->pos.y - pUI->fHeight, NIL_F);
		pVtx[1].pos = D3DXVECTOR3(pUI->pos.x + pUI->fWidth, pUI->pos.y - pUI->fHeight, NIL_F);
		pVtx[2].pos = D3DXVECTOR3(pUI->pos.x - pUI->fWidth, pUI->pos.y + pUI->fHeight, NIL_F);
		pVtx[3].pos = D3DXVECTOR3(pUI->pos.x + pUI->fWidth, pUI->pos.y + pUI->fHeight, NIL_F);

		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = RHW;			//rhwの設定
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = RGBA_WHITE;		//頂点カラーの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, RANK_UI_TEX_HEIGHT * (float)nCntVtx);	//テクスチャの座標
		pVtx[1].tex = D3DXVECTOR2(1.0f, RANK_UI_TEX_HEIGHT * (float)nCntVtx);	//テクスチャの座標
		pVtx[2].tex = D3DXVECTOR2(0.0f, RANK_UI_TEX_HEIGHT * (float)nCntVtx + RANK_UI_TEX_HEIGHT);	//テクスチャの座標
		pVtx[3].tex = D3DXVECTOR2(1.0f, RANK_UI_TEX_HEIGHT * (float)nCntVtx + RANK_UI_TEX_HEIGHT);	//テクスチャの座標
	}

	//頂点バッファをロックする
	g_pVtxBuffHipDropRankUI->Unlock();
}

//------------------------------------------
//		ヒップドロップ量のUI終了処理
//------------------------------------------
void UninitHipDropRankUI(void)
{
	//テクスチャの破棄
	if (g_pTextureHipDropRankUI != NULL)
	{
		g_pTextureHipDropRankUI->Release();
		g_pTextureHipDropRankUI = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHipDropRankUI != NULL)
	{
		g_pVtxBuffHipDropRankUI->Release();
		g_pVtxBuffHipDropRankUI = NULL;
	}
}

//------------------------------------------
//		ヒップドロップ量のUI更新処理
//------------------------------------------
void UpdateHipDropRankUI(void)
{
	for (int nCntRankUI = 0; nCntRankUI < RANK_UI_NUM; nCntRankUI++)
	{
		//表示されている
		if (g_HDRankUI[nCntRankUI].bDisp)
		{
			//寿命を減らす
			g_HDRankUI[nCntRankUI].nLife--;

			//寿命が尽きた
			if (g_HDRankUI[nCntRankUI].nLife <= 0)
			{
				//非表示にする
				g_HDRankUI[nCntRankUI].bDisp = false;
			}
		}
	}
}

//------------------------------------------
//		ヒップドロップ量のUI描画処理
//------------------------------------------
void DrawHipDropRankUI(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHipDropRankUI, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureHipDropRankUI);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntRankUI = 0; nCntRankUI < RANK_UI_NUM; nCntRankUI++)
	{
		//UIが使われている
		if (g_HDRankUI[nCntRankUI].bDisp)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRankUI * VTX_MAX, 2);
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//------------------------------------------
//		ヒップドロップ量のUI設定処理
//------------------------------------------
void SetRankUI(int nNumUI, int nTexHeight)
{
	g_HDRankUI[nNumUI].nLife = RANK_UI_LIFE;	/*寿命設定*/
	g_HDRankUI[nNumUI].bDisp = true;			/*表示する*/

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHipDropRankUI->Lock(0, 0, (void* *)&pVtx, 0);

	//ポインタをずらす
	pVtx += nNumUI * VTX_MAX;

	//テクスチャのＹ座標の設定
	pVtx[0].tex.y = pVtx[1].tex.y = RANK_UI_TEX_HEIGHT * (float)nTexHeight;							/*テクスチャの上辺*/
	pVtx[2].tex.y = pVtx[3].tex.y = RANK_UI_TEX_HEIGHT * (float)nTexHeight + RANK_UI_TEX_HEIGHT;	/*テクスチャの底辺*/

	//頂点バッファをロックする
	g_pVtxBuffHipDropRankUI->Unlock();
}