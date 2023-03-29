//==========================================
//
//タイマープログラム[timer.cpp]
//Author:飯田洲暉 石原颯馬
//
//==========================================

#include "main.h"
#include "pvp_game.h"
#include "timer.h"
#include "PvP_player.h"
#include "camera.h"
#include "fade.h"
#include "color.h"
#include "bonus.h"
#include "time.h"
#include "item.h"

//マクロ定義
#define NUM_PLACE  (2)								 //スコアの桁数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//頂点バッファへのポインタ
TIME g_aTime;

D3DXMATRIX mtxWorldTime;

int BonusLimit;
bool bBonusUse;
MODE g_mode_time;

//===============================
//タイムの初期化の処理 
//===============================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Number.png",
		&g_pTextureTime);

	
		g_aTime.pos = ZERO_SET;//位置初期化
		g_aTime.nCounter = 0;
		SetTimerDigit();		//桁数設定

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * VTX_MAX * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += VTX_MAX)
	{

		//テクスチャ座標の設定
		pVtx[0].pos = D3DXVECTOR3(TIMER_MINUS_X + (nCntTime * TIMER_WIDTH_X), TIMER_MINUS_Y, NIL_F);
		pVtx[1].pos = D3DXVECTOR3(TIMER_PLUS_X + (nCntTime * TIMER_WIDTH_X), TIMER_MINUS_Y, NIL_F);
		pVtx[2].pos = D3DXVECTOR3(TIMER_MINUS_X + (nCntTime * TIMER_WIDTH_X), TIMER_PLUS_Y, NIL_F);
		pVtx[3].pos = D3DXVECTOR3(TIMER_PLUS_X + (nCntTime * TIMER_WIDTH_X), TIMER_PLUS_Y, NIL_F);
			
		//rhwの設定
		pVtx[VTX_LE_UP].rhw = RHW;
		pVtx[VTX_RI_UP].rhw = RHW;
		pVtx[VTX_LE_DO].rhw = RHW;
		pVtx[VTX_RI_DO].rhw = RHW;

		//頂点カラーの設定
		pVtx[0].col = RGBA_ORANGE;
		pVtx[1].col = RGBA_ORANGE;
		pVtx[2].col = RGBA_ORANGE;
		pVtx[3].col = RGBA_ORANGE;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//現在時間の取得
	srand((unsigned int)time(0));

	//ボーナスのリミットをランダムに設定
	BonusLimit = rand() % 900;

	//ボーナスの使用できるようにする
	bBonusUse = false;

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//===============================
//タイムの終了処理
//===============================
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//===============================
//タイムの更新処理
//===============================
void UpdateTime(MODE mode)
{
	g_mode_time = mode;

	if (g_mode_time == MODE_PvPGAME)
	{
		//ボーナスの出現処理
		if (g_aTime.nTime <= 90)
		{
			BonusLimit--;

			if (bBonusUse == false)
			{
				if (BonusLimit <= 0)
				{
					//ボーナスのセット処理
					SetBonus();

					//ボーナスを使用できないようにする
					bBonusUse = true;
				}
			}
		}
	}

	g_aTime.nCounter++;

	if ((g_aTime.nCounter % 60) == 0)
	{//一定時間経過
		g_aTime.nCounter = 0;		//カウンターを初期値に戻す

		if (g_mode_time == MODE_PvPGAME)
		{
			SetItem();
		}

		AddTime(1);
	}

}

//===============================
//タイムの描画処理
//===============================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * VTX_MAX, 2);
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===============================
//タイムの設定処理
//===============================
void SetTime(int nTime)
{
	//タイムの保存
	g_aTime.nTemp = g_aTime.nTime = nTime;

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += VTX_MAX)
	{
		int aTexU = g_aTime.nTime % g_aTime.nDigit / (g_aTime.nDigit / 10);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU * 0.1f + 0.1f, 1.0f);

		g_aTime.nDigit /= 10;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();

	//桁数再設定
	SetTimerDigit();
}

//===============================
//タイムの減算処理
//===============================
void AddTime(int nValue)
{
	g_aTime.nTime -= nValue;

	//制限時間が0になったらゲーム終了
	if (g_aTime.nTime <= 0)
	{
		SetFade(MODE_RESULT);
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++, pVtx += VTX_MAX)
	{
		int aTexU = g_aTime.nTime % g_aTime.nDigit / (g_aTime.nDigit / 10);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU * 0.1f + 0.1f, 1.0f);

		g_aTime.nDigit /= 10;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();

	//桁数再設定
	SetTimerDigit();
}

//===============================
//タイムの桁数設定
//===============================
void SetTimerDigit(void)
{
	g_aTime.nDigit = (int)pow(10, NUM_PLACE);
}

//===============================
//タイム取得処理
//===============================
int GetTime(void)
{
	return g_aTime.nTime;
}
