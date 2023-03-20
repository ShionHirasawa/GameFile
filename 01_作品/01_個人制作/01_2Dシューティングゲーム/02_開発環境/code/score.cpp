/*==========================================================================================================================

													スコア表示処理[score.cpp]
														Author:平澤詩苑

==========================================================================================================================*/
#include "score.h"

//マクロ定義
#define NUM_PLACE				(8)		//スコアの桁数

#define TENMILLIONSPLACE		10000000
#define MILLIONS_PLACE			10000000 / 1000000
#define HUNDREDTHOUSANDS_PLACE	1000000 / 100000
#define TENTHOUSANDS_PLACE		100000 / 10000
#define THOUSANDS_PLACE			10000 / 1000
#define HUNDREDS_PLACE			1000 / 100
#define TENS_PLACE				100 / 10
#define ONES_PLACE				10


//百万の位までの構造体
typedef enum
{
	DEJIT_PLACE_TENMILLION		= ZERO,	//千万の位
	DEJIT_PLACE_MILLION			,			//百万の位
	DEJIT_PLACE_HUNDREDTHOUSAND	,	//十万の位
	DEJIT_PLACE_TENTHOUSAND		,		//万の位
	DEJIT_PLACE_THOUSAND		,			//千の位
	DEJIT_PLACE_HUNDRED			,			//百の位
	DEJIT_PLACE_TEN				,				//十の位
	DEJIT_PLACE_ONE				,				//一の位
	DEJIT_PLACE_MAX
}DEJIT_PLACE;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posScore;								//スコアの位置
int g_nScore;										//スコアの値

//スコアの初期化処理
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	//スコアの情報の初期化
	g_posScore = D3DXVECTOR3(1000.0f, 3.0f, 0.0f);				//スコアの位置の初期化
	g_nScore = 0;												//スコアの初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_PLACE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//頂点座標の設定
		{
			pVtx[0].pos.x = (g_posScore.x     ) + nCntScore * 35;
			pVtx[0].pos.y = (g_posScore.y     );
			pVtx[1].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[1].pos.y = (g_posScore.y     );
			pVtx[2].pos.x = (g_posScore.x     ) + nCntScore * 35;
			pVtx[2].pos.y = (g_posScore.y + 60);
			pVtx[3].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[3].pos.y = (g_posScore.y + 60);
		}

		//rhwの設定
		{
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
		}

		//頂点カラーの設定
		{
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		}

		//テクスチャの座標
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_nScore, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * g_nScore, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * g_nScore, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * g_nScore, 1.0f);
		}

		pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffScore->Unlock();
}

//スコアの終了処理
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

//スコアの更新処理
void UpdateScore(void)
{
	
}

//スコアの描画処理
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffScore, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureScore);

	for (nCntScore = START_CNT; nCntScore < NUM_PLACE; nCntScore++)
	{
		 //ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * NUM_VERTEX, SEC);
	}
}

//スコアの設定処理
void SetScore(int nScore)
{
	int nCntScore;
	int aTexU[NUM_PLACE];			//各桁の数字を格納
	g_nScore = nScore;

	//スコアの各桁の抽出
	aTexU[DEJIT_PLACE_TENMILLION]		= g_nScore / TENMILLIONSPLACE;
	aTexU[DEJIT_PLACE_MILLION]			= g_nScore % MILLIONS_PLACE;
	aTexU[DEJIT_PLACE_HUNDREDTHOUSAND]	= g_nScore % HUNDREDTHOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_TENTHOUSAND]		= g_nScore % TENTHOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_THOUSAND]			= g_nScore % THOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_HUNDRED]			= g_nScore % HUNDREDS_PLACE;
	aTexU[DEJIT_PLACE_TEN]				= g_nScore % TENS_PLACE;
	aTexU[DEJIT_PLACE_ONE]				= g_nScore % ONES_PLACE;
	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntScore = START_CNT; nCntScore < NUM_PLACE; nCntScore++)
	{
		//頂点情報の設定
		//頂点座標の設定
		{
			pVtx[ZERO].pos.x = (g_posScore.x) + nCntScore * 35;
			pVtx[ZERO].pos.y = (g_posScore.y);
			pVtx[ONE].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[ONE].pos.y = (g_posScore.y);
			pVtx[SEC].pos.x = (g_posScore.x) + nCntScore * 35;
			pVtx[SEC].pos.y = (g_posScore.y + 60);
			pVtx[SAN].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[SAN].pos.y = (g_posScore.y + 60);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[nCntScore], 1.0f);
			pVtx[SAN].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 1.0f);
		}
		
		pVtx += NUM_VERTEX;
	}
	//頂点バッファをロックする
	g_pVtxBuffScore->Unlock();
}

//スコアの加算処理
void AddScore(int nValue)
{
	int nCntScore;
	int aTexU[NUM_PLACE];			//各桁の数字を格納
	g_nScore += nValue;

	aTexU[DEJIT_PLACE_TENMILLION] = g_nScore / TENMILLIONSPLACE;
	aTexU[DEJIT_PLACE_MILLION] = g_nScore % MILLIONS_PLACE;
	aTexU[DEJIT_PLACE_HUNDREDTHOUSAND] = g_nScore % HUNDREDTHOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_TENTHOUSAND] = g_nScore % TENTHOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_THOUSAND] = g_nScore % THOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_HUNDRED] = g_nScore % HUNDREDS_PLACE;
	aTexU[DEJIT_PLACE_TEN] = g_nScore % TENS_PLACE;
	aTexU[DEJIT_PLACE_ONE] = g_nScore % ONES_PLACE;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntScore = START_CNT; nCntScore < NUM_PLACE; nCntScore++)
	{
		//頂点情報の設定
		//頂点座標の設定
		{
			pVtx[ZERO].pos.x = (g_posScore.x) + nCntScore * 35;
			pVtx[ZERO].pos.y = (g_posScore.y);
			pVtx[ONE].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[ONE].pos.y = (g_posScore.y);
			pVtx[SEC].pos.x = (g_posScore.x) + nCntScore * 35;
			pVtx[SEC].pos.y = (g_posScore.y + 60);
			pVtx[SAN].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[SAN].pos.y = (g_posScore.y + 60);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[nCntScore], 1.0f);
			pVtx[SAN].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 1.0f);
		}

		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffScore->Unlock();
}

//スコアの値を獲得
int GetScore(void)
{
	return g_nScore;	//プレイヤーのスコアを返す
}