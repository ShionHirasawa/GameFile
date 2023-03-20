/*==========================================================================================================================

													ランキング表示処理[ranking.cpp]
														Author:平澤詩苑

==========================================================================================================================*/
#include "score.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"

//マクロ定義
#define GRAY			(0.7f)						//灰色
#define BLUCK			(0.4f)						//黒色
#define RANK_HEIGHT		(50.0f)						//順位の数字の高さ
#define RANK_WIDTH		(40.0f)						//順位の数字の幅
#define HEIGHT_DIS		(80.0f)						//数値と数値の距離
#define WIDTH__DIS		(80.0f)						//数値と数値の距離
#define RANK_SIZE		(5.0f)						//順位ごとのサイズ(順位が下がるごとに縮小されていく)
#define DIS_SIZE		(7.9f)						//各順位の横との幅(順位が下がるごとに狭まる)
#define RANK_DEJIT		(8)							//ランキングスコアの桁数
#define RANK_NUM		(5)							//順位の数
#define ALL_NUM			(RANK_DEJIT * RANK_NUM)		//ランキングに使用する全桁数
#define RANK_DATA		"data/RANKING.txt"			//ランキングファイル
#define RANK_TIME_LIMIT	(6000)						//ランキング表示時間(10秒)	
#define RANK_BG_NUM		(0)							//背景画像の番号
#define RANK_NUMBER		(1)							//ランキングに使用する画像の番号
#define RANK_TEXTURE	(2)							//ランキングに使用する画像の数
#define TEX_MAX			(41)						//描画するポリゴンの数

//順位ごとの座標マクロ
//	X座標
#define NO_1POSX	(350.0f)	//No.1のポジション x
#define NO_2POSX	(380.0f)	//No.2のポジション x
#define NO_3POSX	(410.0f)	//No.3のポジション x
#define NO_4POSX	(430.0f)	//No.4のポジション x
#define NO_5POSX	(460.0f)	//No.5のポジション x

//	Y座標
#define NO_1POSY	(150.0f)	//No.1のポジション y
#define NO_2POSY	(200.0f)	//No.2のポジション y
#define NO_3POSY	(240.0f)	//No.3のポジション y
#define NO_4POSY	(270.0f)	//No.4のポジション y
#define NO_5POSY	(300.0f)	//No.5のポジション y

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureRanking[RANK_TEXTURE] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				//頂点バッファへのポインタ
D3DXVECTOR3 g_posRank[RANK_NUM];								//各順位ごとの座標
int g_nRanking[RANK_NUM];										//ランクの値
DWORD g_Rank_Time;												//ランキング表示時間

//ランキングの初期化処理
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int aTexU[RANK_DEJIT];	//ランキング表示に使用

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RankingBg.jpg", &g_pTextureRanking[RANK_BG_NUM]);	//背景画像
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureRanking[RANK_NUMBER]);	//数字画像

	//スコアの情報の初期化およびランキングファイルの読み込み
	ScanRanking();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TEX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRanking, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//背景画像の設定
	{
		//頂点座標の設定
		{
			pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, F_ZERO);
			pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, F_ZERO);
			pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, F_ZERO);
			pVtx[SAN].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, F_ZERO);
		}

		//rhwの設定
		{
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;
		}

		//頂点カラーの設定
		{
			pVtx[ZERO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[ONE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[SEC].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[SAN].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[SAN].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める

	//各順位の初期値
	g_posRank[ZERO] = D3DXVECTOR3(NO_1POSX, NO_1POSY, POS_Z);
	g_posRank[ONE] = D3DXVECTOR3(NO_2POSX, NO_2POSY, POS_Z);
	g_posRank[SEC] = D3DXVECTOR3(NO_3POSX, NO_3POSY, POS_Z);
	g_posRank[SAN] = D3DXVECTOR3(NO_4POSX, NO_4POSY, POS_Z);
	g_posRank[FOUR] = D3DXVECTOR3(NO_5POSX, NO_5POSY, POS_Z);

	//ランキングに使用する各順位の各桁の情報初期化
	for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
	{		
		aTexU[0] = g_nRanking[nCntRanking] / 10000000;
		aTexU[1] = g_nRanking[nCntRanking] % 10000000 / 1000000;
		aTexU[2] = g_nRanking[nCntRanking] % 1000000 / 100000;
		aTexU[3] = g_nRanking[nCntRanking] % 100000 / 10000;
		aTexU[4] = g_nRanking[nCntRanking] % 10000 / 1000;
		aTexU[5] = g_nRanking[nCntRanking] % 1000 / 100;
		aTexU[6] = g_nRanking[nCntRanking] % 100 / 10;
		aTexU[7] = g_nRanking[nCntRanking] % 10;

		for (int mCntRanking = 0; mCntRanking < RANK_DEJIT; mCntRanking++)
		{
			//頂点座標の設定
			{
				pVtx[0].pos.x = g_posRank[nCntRanking].x - (RANK_WIDTH  - (float)nCntRanking * RANK_SIZE) + ((float)mCntRanking * (WIDTH__DIS - (float)nCntRanking * DIS_SIZE));
				pVtx[0].pos.y = g_posRank[nCntRanking].y - (RANK_HEIGHT - (float)nCntRanking * RANK_SIZE) + ((float)nCntRanking * (HEIGHT_DIS));
				pVtx[1].pos.x = g_posRank[nCntRanking].x + (RANK_WIDTH  - (float)nCntRanking * RANK_SIZE) + ((float)mCntRanking * (WIDTH__DIS - (float)nCntRanking * DIS_SIZE));
				pVtx[1].pos.y = g_posRank[nCntRanking].y - (RANK_HEIGHT - (float)nCntRanking * RANK_SIZE) + ((float)nCntRanking * (HEIGHT_DIS));
				pVtx[2].pos.x = g_posRank[nCntRanking].x - (RANK_WIDTH  - (float)nCntRanking * RANK_SIZE) + ((float)mCntRanking * (WIDTH__DIS - (float)nCntRanking * DIS_SIZE));
				pVtx[2].pos.y = g_posRank[nCntRanking].y + (RANK_HEIGHT - (float)nCntRanking * RANK_SIZE) + ((float)nCntRanking * (HEIGHT_DIS));
				pVtx[3].pos.x = g_posRank[nCntRanking].x + (RANK_WIDTH  - (float)nCntRanking * RANK_SIZE) + ((float)mCntRanking * (WIDTH__DIS - (float)nCntRanking * DIS_SIZE));
				pVtx[3].pos.y = g_posRank[nCntRanking].y + (RANK_HEIGHT - (float)nCntRanking * RANK_SIZE) + ((float)nCntRanking * (HEIGHT_DIS));
			}

			//rhwの設定
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//各順位のカラー設定
			switch (nCntRanking)
			{
			case 0:					//No.1(黄色)
				//頂点カラーの設定
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				}
				break;

			case 1:					//No.2(灰色)
				//頂点カラーの設定
				{
					pVtx[0].col = D3DXCOLOR(GRAY, GRAY, GRAY, 1.0f);
					pVtx[1].col = D3DXCOLOR(GRAY, GRAY, GRAY, 1.0f);
					pVtx[2].col = D3DXCOLOR(GRAY, GRAY, GRAY, 1.0f);
					pVtx[3].col = D3DXCOLOR(GRAY, GRAY, GRAY, 1.0f);
				}
				break;

			case 2:					//No.3(オレンジ)
				//頂点カラーの設定
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
				}
				break;

			case 3:					//NO.4(薄青)
			case 4:					//No.5(薄青)
				//頂点カラーの設定
				{
					pVtx[0].col = D3DXCOLOR(BLUCK, BLUCK, BLUCK, 1.0f);
					pVtx[1].col = D3DXCOLOR(BLUCK, BLUCK, BLUCK, 1.0f);
					pVtx[2].col = D3DXCOLOR(BLUCK, BLUCK, BLUCK, 1.0f);
					pVtx[3].col = D3DXCOLOR(BLUCK, BLUCK, BLUCK, 1.0f);
				}
				break;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[mCntRanking], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[mCntRanking], 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[mCntRanking], 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[mCntRanking], 1.0f);
			}

			pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffRanking->Unlock();

	g_Rank_Time = timeGetTime();	//表示時間測定開始
}

//ランキングの終了処理
void UninitRanking(void)
{
	//テクスチャの破棄(2枚)
	for (int nCntRankTex = START_CNT; nCntRankTex < RANK_TEXTURE; nCntRankTex++)
	{
		if (g_pTextureRanking[nCntRankTex] != NULL)
		{
			g_pTextureRanking[nCntRankTex]->Release();
			g_pTextureRanking[nCntRankTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//ランキングの更新処理
void UpdateRanking(void)
{
	DWORD EndRankTime = timeGetTime();	//ランキング表示経過時間

	if (GetKeyboardTrigger(DIK_RETURN) == true || EndRankTime - g_Rank_Time >= RANK_TIME_LIMIT)
	{
		SetFade(MODE_TITLE);	//タイトル画面に移行
	}
}

//ランキングの描画処理
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffRanking, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//背景画像の設定
	pDevice->SetTexture(ZERO, g_pTextureRanking[RANK_BG_NUM]);

	//背景の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, RANK_BG_NUM, SEC);

	//数字テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking[RANK_NUMBER]);

	//桁数分のポリゴンの描画
	for (nCntScore = START_CNT; nCntScore < ALL_NUM; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * NUM_VERTEX + NUM_VERTEX, SEC);
	}
}

//ランキングの設定処理
void ScanRanking(void)
{
	FILE *pFile;

	pFile = fopen(RANK_DATA, "r");	//ランキングファイル読み込み

	if (pFile != NULL)
	{
		for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
		{
			fscanf(pFile, "%d", &g_nRanking[nCntRanking]);		//ランキング配列に格納
		}

		fclose(pFile);

		DownSort();		//並び替え処理に移動
	}
}

//並び替え処理
void DownSort(void) 
{
	int PlayerRank = GetScore();	//プレイヤーのランキング

	if (PlayerRank > g_nRanking[FOUR])
	{//プレイヤーのスコアが、読み込まれた数値の最小値より大きかったら入れ替え
		g_nRanking[FOUR] = PlayerRank;

		for (int nCntRanking = START_CNT; nCntRanking < FOUR; nCntRanking++)				//比較される数値(基準の値)
		{
			for (int mCntRanking = nCntRanking + ONE; mCntRanking < RANK_NUM; mCntRanking++)	//比較する数値
			{
				//比較した数値が基準の値より大きければバブルソートを行う
				if (g_nRanking[nCntRanking] < g_nRanking[mCntRanking])
				{
					int nTmp = g_nRanking[nCntRanking];					//基準の値を一時保管場所に格納

					g_nRanking[nCntRanking] = g_nRanking[mCntRanking];	//基準の値の場所を上書き

					g_nRanking[mCntRanking] = nTmp;						//比較する数値の場所を上書き
				}
			}
		}
		
		FILE *pFile;	//ファイルポインタ

		pFile = fopen(RANK_DATA, "w");

		//ランキング書き換え処理
		if (pFile != NULL)
		{
			for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
			{
				fprintf(pFile, "%.8d\n", g_nRanking[nCntRanking]);		//順位ごとのスコアを書き出し
			}

			fclose(pFile);
		}
	}
}