/*==========================================================================================================================================================

制限時間処理[timelimit.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "timelimit.h"
#include "color.h"
#include "input.h"
#include "sound.h"
#include "score.h"
#include "fade.h"
#include "game.h"

//マクロ定義
#define TIMELIMIT_POS				(D3DXVECTOR3(620.0f, 3.0f, 0.0f))	//スコアの位置
#define TIMELIMIT_WIDTH				(35.0f)								//幅
#define TIMELIMIT_HEIGHT			(60.0f)								//高さ
#define TIMELIMIT_TEX_DIVISION		(0.1f)								//テクスチャの分割数（10分割）

#define TIMELIMIT_NUM_DEJIT			(3)									//制限時間の桁数
#define TIMELIMIT_MAX_DEJIT			(1000)								//桁最大
#define TIMELIMIT_MAX				(200 * 60)							//初期制限時間 × フレーム
#define TIMELIMIT_SPEED				(100)								//残り制限時間１００秒でBGMの速度を変える

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureTimeLimit = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeLimit = NULL;		//頂点バッファへのポインタ
TimeLimit				g_TimeLimit;					//制限時間の情報

//--------------------------------------------------------
//					制限時間の初期化処理
//--------------------------------------------------------
void InitTimeLimit(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureTimeLimit);

	//スコアの情報の初期化
	g_TimeLimit.pos = TIMELIMIT_POS;
	g_TimeLimit.nTime = TIMELIMIT_MAX;
	g_TimeLimit.nDejit = TIMELIMIT_MAX_DEJIT;
	g_TimeLimit.bSoundAlarm = false;
	g_TimeLimit.nFromAlarm = NULL;
	g_TimeLimit.bSoundSpeed = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * TIMELIMIT_NUM_DEJIT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimeLimit, NULL);

	//頂点座標を設定
	SetTimeLimit_Vertex();
}

//--------------------------------------------------------
//					制限時間の終了処理
//--------------------------------------------------------
void UninitTimeLimit(void)
{
	//テクスチャの破棄
	if (g_pTextureTimeLimit != NULL)
	{
		g_pTextureTimeLimit->Release();
		g_pTextureTimeLimit = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTimeLimit != NULL)
	{
		g_pVtxBuffTimeLimit->Release();
		g_pVtxBuffTimeLimit = NULL;
	}
}

//--------------------------------------------------------
//					制限時間の更新処理
//--------------------------------------------------------
void UpdateTimeLimit(void)
{
	g_TimeLimit.nTime--;	//制限時間減少

	SetTimeLimit_Vertex();	//頂点座標を設定

	TimeLimit_Sound_Change();	//残りの制限時間によるBGMの停止・再生

	//制限時間が切れたらゲーム終了
	Timelimit_TheEnd();
}

//残りの制限時間による音楽の再生
void TimeLimit_Sound_Change(void)
{
	//残り時間が１００秒になった
	if (g_TimeLimit.nTime / FPS_60 <= TIMELIMIT_SPEED && g_TimeLimit.bSoundAlarm == false)
	{
		StopSound(SOUND_LABEL_GAMEBGM);			//通常ゲームBGMを停止

		PlaySound(SOUND_LABEL_SE_TIME_ALARM);	//制限時間アラームを再生
		g_TimeLimit.bSoundAlarm = true;			//再生した
	}

	//アラーム音が再生された				倍速BGMは流れていない
	if (g_TimeLimit.bSoundAlarm == true && g_TimeLimit.bSoundSpeed == false)
	{
		g_TimeLimit.nFromAlarm++;	//経過時間測定

		//100フレーム経過したら倍速ＢＧＭを流す
		if (TIMELIMIT_SPEED < g_TimeLimit.nFromAlarm)
		{
			PlaySound(SOUND_LABEL_GAMEBGM_SPEED);
			g_TimeLimit.bSoundSpeed = true;			//倍速ＢＧＭを流した
		}
	}
}

//制限時間の終了判定
void Timelimit_TheEnd(void)
{
	//制限時間が切れた
	if (g_TimeLimit.nTime <= 0)
	{
		SetFade(MODE_RESULT);		//リザルト画面へ遷移
		SetEndGame(EndGAME_OVER);	//ゲームオーバーに設定
	}
}

//--------------------------------------------------------
//					制限時間の描画処理
//--------------------------------------------------------
void DrawTimeLimit(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeLimit, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimeLimit);

	for (int nCntTimeLimit = START_CNT; nCntTimeLimit < TIMELIMIT_NUM_DEJIT; nCntTimeLimit++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimeLimit * VTX_MAX, SEC);
	}
}

//制限時間の情報を取得
TimeLimit *GetTimeLimit(void)
{
	return &g_TimeLimit;	//制限時間の情報を情報を返す
}

//頂点座標を設定
void SetTimeLimit_Vertex(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimeLimit->Lock(0, 0, (void* *)&pVtx, 0);

	//頂点情報の設定
	for (int nCntTimeLimit = 0; nCntTimeLimit < TIMELIMIT_NUM_DEJIT; nCntTimeLimit++)
	{
		//タイムの各桁を抽出
		int aTexU = (g_TimeLimit.nTime / FPS_60) % g_TimeLimit.nDejit / (g_TimeLimit.nDejit / DEJIT_DIVISION);

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos.x = (g_TimeLimit.pos.x) + nCntTimeLimit * TIMELIMIT_WIDTH;
			pVtx[VTX_LE_UP].pos.y = (g_TimeLimit.pos.y);
			pVtx[VTX_RI_UP].pos.x = (g_TimeLimit.pos.x + TIMELIMIT_WIDTH) + nCntTimeLimit * TIMELIMIT_WIDTH;
			pVtx[VTX_RI_UP].pos.y = (g_TimeLimit.pos.y);
			pVtx[VTX_LE_DO].pos.x = (g_TimeLimit.pos.x) + nCntTimeLimit * TIMELIMIT_WIDTH;
			pVtx[VTX_LE_DO].pos.y = (g_TimeLimit.pos.y + TIMELIMIT_HEIGHT);
			pVtx[VTX_RI_DO].pos.x = (g_TimeLimit.pos.x + TIMELIMIT_WIDTH) + nCntTimeLimit * TIMELIMIT_WIDTH;
			pVtx[VTX_RI_DO].pos.y = (g_TimeLimit.pos.y + TIMELIMIT_HEIGHT);
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
			//制限時間が100秒以上の場合の色
			if (TIMELIMIT_SPEED < g_TimeLimit.nTime / FPS_60)
			{
				pVtx[VTX_LE_UP].col = TIMELIMIT_NORMAL_COLOR;
				pVtx[VTX_RI_UP].col = TIMELIMIT_NORMAL_COLOR;
				pVtx[VTX_LE_DO].col = TIMELIMIT_NORMAL_COLOR;
				pVtx[VTX_RI_DO].col = TIMELIMIT_NORMAL_COLOR;
			}

			//制限時間が100秒以下の色
			else
			{
				pVtx[VTX_LE_UP].col = TIMELIMIT_SPEED_COLOR;
				pVtx[VTX_RI_UP].col = TIMELIMIT_SPEED_COLOR;
				pVtx[VTX_LE_DO].col = TIMELIMIT_SPEED_COLOR;
				pVtx[VTX_RI_DO].col = TIMELIMIT_SPEED_COLOR;
			}
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(TIMELIMIT_TEX_DIVISION * aTexU, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(TIMELIMIT_TEX_DIVISION + TIMELIMIT_TEX_DIVISION * aTexU, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(TIMELIMIT_TEX_DIVISION * aTexU, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(TIMELIMIT_TEX_DIVISION + TIMELIMIT_TEX_DIVISION * aTexU, 1.0f);
		}

		pVtx += VTX_MAX;						//頂点データのポインタを4つ分進める
		g_TimeLimit.nDejit /= DEJIT_DIVISION;	//抽出したい桁に下げる
	}

	g_TimeLimit.nDejit = TIMELIMIT_MAX_DEJIT;	//桁を最大に直す

	//頂点バッファをロックする
	g_pVtxBuffTimeLimit->Unlock();
}