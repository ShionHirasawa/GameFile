/*==========================================================================================================================================================

フェード画面処理[fade.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "fade.h"
#include "color.h"

//マクロ定義
#define FADE_SPEED			(0.015f)

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffFade = NULL;		//頂点バッファへのポインタ		
FADE g_fade;											//フェードの状態
MODE g_modeNext;										//次の画面(モード)
D3DXCOLOR g_colorfade;									//ポリゴン(フェード)の色

//------------------------------------------------
//			フェードの初期化処理
//------------------------------------------------
void InitFade(MODE modeNext)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_fade = FADE_IN;			//フェードイン状態に
	g_modeNext = modeNext;		//次の画面(モード)を設定
	g_colorfade = XCOL_BLACK;	//黒い画面(不透明)にしておく

								//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffFade, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void* *)&pVtx, 0);

	//頂点座標の設定
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(NIL_F, NIL_F, NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, NIL_F, NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(NIL_F, SCREEN_HEIGHT, NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);
	}

	//rhwの設定
	{
		pVtx[VTX_LE_UP].rhw = RHW;
		pVtx[VTX_RI_UP].rhw = RHW;
		pVtx[VTX_LE_DO].rhw = RHW;
		pVtx[VTX_RI_DO].rhw = RHW;
	}

	//頂点カラーの設定
	{
		pVtx[VTX_LE_UP].col = g_colorfade;
		pVtx[VTX_RI_UP].col = g_colorfade;
		pVtx[VTX_LE_DO].col = g_colorfade;
		pVtx[VTX_RI_DO].col = g_colorfade;
	}

	//頂点バッファをロックする
	g_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);
}

//------------------------------------------------
//			フェードの終了処理
//------------------------------------------------
void UninitFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//------------------------------------------------
//			フェードの更新処理
//------------------------------------------------
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{
		//フェードイン処理
		if (g_fade == FADE_IN)
		{
			FadeIn();
		}

		//フェードアウト処理
		else if (g_fade == FADE_OUT)
		{
			FadeOut();
		}

		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx[VTX_LE_UP].col = g_colorfade;
		pVtx[VTX_RI_UP].col = g_colorfade;
		pVtx[VTX_LE_DO].col = g_colorfade;
		pVtx[VTX_RI_DO].col = g_colorfade;

		//頂点バッファをロックする
		g_pVtxBuffFade->Unlock();
	}
}

//フェードイン処理
void FadeIn(void)
{
	g_colorfade.a -= FADE_SPEED;					//ポリゴンを透明にしていく

	if (g_colorfade.a <= 0.0f)
	{
		g_colorfade.a = 0.0f;
		g_fade = FADE_NONE;						//何もしていない状態に
	}
}

//フェードアウト処理
void FadeOut(void)
{
	g_colorfade.a += FADE_SPEED;	//ポリゴンを不透明にしていく

	if (g_colorfade.a >= 1.0f)
	{
		g_colorfade.a = 1.0f;
		g_fade = FADE_IN;		//フェードイン状態に

		//モード設定(次の画面に移行)
		SetMode(g_modeNext);
	}
}

//------------------------------------------------
//			フェードの描画処理
//------------------------------------------------
void DrawFade(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//フェードの設定処理
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;			//フェードアウト状態に
	g_modeNext = modeNext;		//次の画面(モード)を設定
	g_colorfade = XCOL_NONE;	//黒いポリゴン(透明)にしておく
}

//フェードの取得処理
FADE GetFade(void)
{
	return g_fade;
}