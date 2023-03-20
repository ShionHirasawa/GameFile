/*==========================================================================================================================

														リザルト画面処理[result.cpp]
															Author:平澤詩苑

==========================================================================================================================*/
#include "result.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "fade.h"

//マクロ定義
#define RESULTTIME_LIMIT	(6000)

//ゲーム終了の種類構造体
typedef enum
{
	RESULT_TYPE_OVER = ZERO,	//ゲームオーバー
	RESULT_TYPE_CLEAR,			//ゲームクリア
	RESULT_TYPE_MAX
}RESULT_TYPE;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureResult[RESULT_TYPE_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//頂点バッファへのポインタ
DWORD g_ResultTime;											//リザルト表示時間

void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GAME_OVER.png",  &g_pTextureResult[RESULT_TYPE_OVER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GAME_CLEAR.png", &g_pTextureResult[RESULT_TYPE_CLEAR]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * RESULT_TYPE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCnt = START_CNT; nCnt < RESULT_TYPE_MAX; nCnt++, pVtx += NUM_VERTEX)
	{
		//頂点座標の設定
		{
			pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS , ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS  , SCREEN_HEIGHT, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);
		}

		//rhwの設定
		{
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw =  RHW_NUM;
			pVtx[SEC].rhw =  RHW_NUM;
			pVtx[SAN].rhw =  RHW_NUM;
		}

		//頂点カラーの設定
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA( COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA( COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA( COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , F_TEN);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffResult->Unlock();

	//現在時刻を取得
	g_ResultTime = timeGetTime();

	if (ResultChoice() == RESULT_TYPE_OVER)
	{
		StopSound(SOUND_LABEL_BGM001);	//ゲーム画面のBGMを停止

		PlaySound(SOUND_LABEL_BGM002);	//死亡音を鳴らす
	}
}

void UninitResult(void)
{
	for (int nCntResult = START_CNT; nCntResult < RESULT_TYPE_MAX; nCntResult++)
	{
		//テクスチャの破棄
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

void UpdateResult(void)
{
	DWORD EndResultTime = timeGetTime();				//リザルト表示の制限時間
	DWORD ResultTimeDiff = EndResultTime - g_ResultTime;//リザルト発生から終了までの差分

	if (GetKeyboardTrigger(DIK_RETURN) == true || ResultTimeDiff > RESULTTIME_LIMIT)
	{//ENTERキーが押された
	 //モード設定（ランキング画面に遷移)
		SetFade(MODE_RANKING);
	}
}

void DrawResult(void)
{
	int result = ResultChoice();

	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffResult, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureResult[result]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}