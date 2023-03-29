/*==========================================================================================================================================================

														リザルト画面処理[result.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "input.h"
#include "fade.h"
#include "game.h"

#define NUM_RESULT			(2)		//リザルト画像の種類

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;				//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureResult[EndGAME_MAX] = {};		//テクスチャのポインタ

const char *c_apResultTexName[EndGAME_MAX] = {
	"data/TEXTURE/GAME_CLEAR.png",
	"data/TEXTURE/GAME_OVER.png",
};

//------------------------------------------------
//				リザルトの初期化処理
//------------------------------------------------
void InitResult(void)
{
	//デバイスへのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_RESULT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//テクスチャ(２枚)の読み込み
	for (int nCntRuselt = START_CNT; nCntRuselt < EndGAME_MAX; nCntRuselt++, pVtx += VTX_MAX)
	{
		D3DXCreateTextureFromFile(pDevice, c_apResultTexName[nCntRuselt], &g_pTextureResult[nCntRuselt]);

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);
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
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = ORIGIN_TEX;
			pVtx[VTX_RI_UP].tex = MA_ZE_TEX;
			pVtx[VTX_LE_DO].tex = ZE_MA_TEX;
			pVtx[VTX_RI_DO].tex = LIMIT_TEX;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffResult->Unlock();
}

//------------------------------------------------
//				リザルトの終了処理
//------------------------------------------------
void UninitResult(void)
{
	//テクスチャ(２枚)の破棄
	for (int nCntResult = START_CNT; nCntResult < EndGAME_MAX; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//------------------------------------------------
//				リザルトの更新処理
//------------------------------------------------
void UpdateResult(void)
{
	//Enterが押された　もしくは　ゲームパッドのSTART or A ボタンが押された
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
	{
		//タイトル画面に移行
		SetFade(MODE_TITLE);
	}
}

//------------------------------------------------
//				リザルトの描画処理
//------------------------------------------------
void DrawResult(void)
{
	//リザルトの種類取得
	int nTypeResult = EndGameResult();

	//デバイスのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffResult, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureResult[nTypeResult]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}