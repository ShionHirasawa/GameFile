/*==========================================================================================================================================================

スコア表示処理[score.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "score.h"
#include "input.h"

//マクロ定義
#define NUM_DEJIT_PLACE			(8)									//最大桁数
#define MAX_DEJIT				(100000000)							//最大の桁

#define SCORE_POS				(D3DXVECTOR3(1000.0f, 3.0f, 0.0f))	//スコアの位置
#define SCORE_WIDTH				(35.0f)								//幅
#define SCORE_HEIGHT			(60.0f)								//高さ
#define SCORE_TEX_DIVISION		(0.1f)								//テクスチャの分割数（10分割）
#define SCORE_COLOR				(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f))	//スコアの色

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファへのポインタ
Score					g_Score;					//スコアの情報

//--------------------------------------------------------
//					スコアの初期化処理
//--------------------------------------------------------
void InitScore(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	//スコアの情報の初期化
	g_Score.pos = SCORE_POS;
	g_Score.nScore = NULL;
	FixDejit();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_DEJIT_PLACE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void* *)&pVtx, 0);

	//頂点情報の設定
	for (int nCntScore = 0; nCntScore < NUM_DEJIT_PLACE; nCntScore++)
	{
		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos.x = (g_Score.pos.x) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_LE_UP].pos.y = (g_Score.pos.y);
			pVtx[VTX_RI_UP].pos.x = (g_Score.pos.x + SCORE_WIDTH) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_RI_UP].pos.y = (g_Score.pos.y);
			pVtx[VTX_LE_DO].pos.x = (g_Score.pos.x) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_LE_DO].pos.y = (g_Score.pos.y + SCORE_HEIGHT);
			pVtx[VTX_RI_DO].pos.x = (g_Score.pos.x + SCORE_WIDTH) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_RI_DO].pos.y = (g_Score.pos.y + SCORE_HEIGHT);
		}

		//rhwの設定
		{
			pVtx[VTX_LE_UP].rhw = RHW_NUM;
			pVtx[VTX_RI_UP].rhw = RHW_NUM;
			pVtx[VTX_LE_DO].rhw = RHW_NUM;
			pVtx[VTX_RI_DO].rhw = RHW_NUM;
		}

		//頂点カラーの設定
		{
			pVtx[VTX_LE_UP].col = SCORE_COLOR;
			pVtx[VTX_RI_UP].col = SCORE_COLOR;
			pVtx[VTX_LE_DO].col = SCORE_COLOR;
			pVtx[VTX_RI_DO].col = SCORE_COLOR;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(SCORE_TEX_DIVISION * g_Score.nScore, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(SCORE_TEX_DIVISION + SCORE_TEX_DIVISION * g_Score.nScore, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(SCORE_TEX_DIVISION * g_Score.nScore, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(SCORE_TEX_DIVISION + SCORE_TEX_DIVISION * g_Score.nScore, 1.0f);
		}

		pVtx += VTX_MAX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffScore->Unlock();
}

//--------------------------------------------------------
//					スコアの終了処理
//--------------------------------------------------------
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

//--------------------------------------------------------
//					スコアの更新処理
//--------------------------------------------------------
void UpdateScore(void)
{

}

//--------------------------------------------------------
//					スコアの描画処理
//--------------------------------------------------------
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

	for (int nCntScore = START_CNT; nCntScore < NUM_DEJIT_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * VTX_MAX, SEC);
	}
}

//スコアの加算処理
void AddScore(int nValue)
{
	g_Score.nScore += nValue;		//スコアを加算

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点情報の設定
	for (int nCntScore = 0; nCntScore < NUM_DEJIT_PLACE; nCntScore++)
	{
		//スコアの桁数を抽出
		int aTexU = g_Score.nScore % g_Score.Dejit / (g_Score.Dejit / DEJIT_DIVISION);

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos.x = (g_Score.pos.x) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_LE_UP].pos.y = (g_Score.pos.y);
			pVtx[VTX_RI_UP].pos.x = (g_Score.pos.x + SCORE_WIDTH) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_RI_UP].pos.y = (g_Score.pos.y);
			pVtx[VTX_LE_DO].pos.x = (g_Score.pos.x) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_LE_DO].pos.y = (g_Score.pos.y + SCORE_HEIGHT);
			pVtx[VTX_RI_DO].pos.x = (g_Score.pos.x + SCORE_WIDTH) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_RI_DO].pos.y = (g_Score.pos.y + SCORE_HEIGHT);
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(SCORE_TEX_DIVISION * aTexU, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(SCORE_TEX_DIVISION + SCORE_TEX_DIVISION * aTexU, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(SCORE_TEX_DIVISION * aTexU, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(SCORE_TEX_DIVISION + SCORE_TEX_DIVISION * aTexU, 1.0f);
		}

		pVtx += VTX_MAX;					//頂点データのポインタを4つ分進める
		g_Score.Dejit /= DEJIT_DIVISION;	//桁数を一つ下げる
	}

	//頂点バッファをロックする
	g_pVtxBuffScore->Unlock();

	//最大桁数に初期化
	FixDejit();
}

//桁数を直すだけ
void FixDejit(void)
{
	//最大桁数に初期化
	g_Score.Dejit = MAX_DEJIT;
}

//スコアの値を獲得
Score *GetScore(void)
{
	return &g_Score;	//スコアを情報を返す
}