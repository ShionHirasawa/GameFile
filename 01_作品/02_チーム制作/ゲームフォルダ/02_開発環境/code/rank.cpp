//==========================================
//
//ランクプログラム[rank.cpp]
//Author:藤原龍輝  平澤詩苑
//
//==========================================
#include "rank.h"
#include "color.h"
#include "fade.h"
#include "HDRgame.h"
#include "hdr_player.h"
#include "debugproc.h"

//マクロ定義
#define POS_X			(200)
#define POS_Y			(100)
#define RANK_TEX_WIDTH	(0.25f)	//テクスチャの横分割
#define FADE_COUNTER	(120)	//終了までの時間

//****************************//
//	   グローバル変数宣言     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTextureRank = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;	//頂点バッファへのポインタ
int			g_nFadeCounter;		//フェードするまでのカウンター
int			g_nUpperRank;		//順位を上げる回数
RANKING		g_nRanking;			//現在の順位
Rank		g_aRank[MAX_RANK];	//順位ＵＩ情報

//=================================
//ランクの初期化処理
//=================================
void InitRank(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &g_pTextureRank);
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * MAX_RANK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRank, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void* *)&pVtx, 0);

	//各情報読み込み
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++, pVtx += VTX_MAX)
	{
		g_aRank[nCntRank].pos = D3DXVECTOR3(150.0f + nCntRank * 320, 600.0f, 0.0f);
		g_aRank[nCntRank].bDisp = false;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aRank[nCntRank].pos.x - POS_X, g_aRank[nCntRank].pos.y - POS_Y, NIL_F);
		pVtx[1].pos = D3DXVECTOR3(g_aRank[nCntRank].pos.x + POS_X, g_aRank[nCntRank].pos.y - POS_Y, NIL_F);
		pVtx[2].pos = D3DXVECTOR3(g_aRank[nCntRank].pos.x - POS_X, g_aRank[nCntRank].pos.y + POS_Y, NIL_F);
		pVtx[3].pos = D3DXVECTOR3(g_aRank[nCntRank].pos.x + POS_X, g_aRank[nCntRank].pos.y + POS_Y, NIL_F);

		//頂点カラー・RHW・テクスチャ座標設定
		for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
		{
			pVtx[nCntVtx].rhw = RHW;					//rhwの設定
			pVtx[nCntVtx].col = RGBA_WHITE;				//頂点カラーの設定
			pVtx[nCntVtx].tex.x = 0.0f;					//テクスチャのⅩ座標（順位設定処理で、テクスチャ座標を設定するので、初期値は０
			pVtx[nCntVtx].tex.y = (float)(nCntVtx / 2);	//テクスチャのＹ座標（カウンターが 0・1 は0.0ｆ　　2・3は1.0f
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffRank->Unlock();

	g_nRanking = RANKING_1ST;
	g_nUpperRank = 0;
	g_nFadeCounter = FADE_COUNTER;
}

//=================================
//ランクの終了処理
//=================================
void UninitRank(void)
{
	//テクスチャの破棄
	if (g_pTextureRank != NULL)
	{
		g_pTextureRank->Release();
		g_pTextureRank = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}

//=================================
//ランクの更新処理
//=================================
void UpdateRank(void)
{
	//順位を上げる
	UpperRank();

	//順位がプレイ人数に達した
	if (g_nRanking >= JoinPlayer())
	{
		//終了時間減少
		g_nFadeCounter--;

		//終了した
		if (g_nFadeCounter <= 0)
		{
			//リザルトへ移行
			SetFade(MODE_RESULT);
		}
	}
}

//参加人数を測定
int JoinPlayer(void)
{
	//参加しているプレイヤー人数を格納
	int nJoinNumPlayer = GetUseControllerNum_HDR();

	//参加者がいない
	if (nJoinNumPlayer == 0)
	{
		//キーボードを使用していることを想定し、参加者が一人はいることにする
		nJoinNumPlayer++;
	}

	for (int nCntJoin = nJoinNumPlayer; nCntJoin < MAX_USE_GAMEPAD; nCntJoin++)
	{
		//AIが使用されている
		if (GetPlayer_HDR()[nCntJoin].bUseAI)
		{
			//参加人数を増やす
			nJoinNumPlayer++;
		}
	}

	PrintDebugProc("参加人数：%d\n", nJoinNumPlayer);

	//参加人数を返す
	return nJoinNumPlayer;
}

//順位を上げる
void UpperRank(void)
{
	for (g_nUpperRank; 0 < g_nUpperRank; g_nUpperRank--)
	{
		//順位を上げる
		g_nRanking = (RANKING)(g_nRanking + 1);

		//順位が４位を超えてしまった
		if (g_nRanking > RANKING_MAX)
		{
			g_nRanking = RANKING_MAX;	//ランキングを最下位にとどめる
			break;						//処理を止める
		}
	}
}

//=================================
//順位の描画処理
//=================================
void DrawRank(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (g_aRank[nCntRank].bDisp == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRank);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * VTX_MAX, 2);
		}
	}
}

//ランキング設定
int SetRank(int nCntPlayer)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void* *)&pVtx, 0);

	//対象の頂点までポインタをずらす
	pVtx += VTX_MAX * nCntPlayer;

	//テクスチャの座標
	pVtx[0].tex.x = pVtx[2].tex.x = g_nRanking * RANK_TEX_WIDTH;					//テクスチャの左辺
	pVtx[1].tex.x = pVtx[3].tex.x = g_nRanking * RANK_TEX_WIDTH + RANK_TEX_WIDTH;	//テクスチャの右辺

	//頂点バッファをロックする
	g_pVtxBuffRank->Unlock();

	g_aRank[nCntPlayer].bDisp = true;	//表示する
	g_nUpperRank++;						//順位を次の更新処理で上げる

	return g_nRanking;	//現在の順位を返す
}