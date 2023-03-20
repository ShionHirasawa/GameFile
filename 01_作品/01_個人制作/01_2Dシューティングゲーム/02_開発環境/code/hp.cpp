/*==========================================================================================================================

												HP表示処理[score.cpp]
													Author:平澤詩苑

==========================================================================================================================*/
#include "hp.h"
#include "player.h"

//マクロ定義

//HP別テクスチャ座標のマクロ
#define HP_0		(0.000f)
#define HP_1		(0.166f)
#define HP_2		(0.332f)
#define HP_3		(0.498f)
#define HP_4		(0.664f)
#define HP_MAX		(0.830f)
#define TEX_0		(0.0f)
#define TEX_1		(1.0f)

//アイコンの座標マクロ
#define HP_POS_X	(1230.0f)
#define HP_POS_Y	(100.0f)
#define HP_SIZE		(30)						//中心座標からのHPアイコンの大きさ

typedef enum
{
	LIFE_0 = 0,
	LIFE_1,
	LIFE_2,
	LIFE_3,
	LIFE_4,
	LIFE_MAX
}Life;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureHp = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHp = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posHp;								//スコアの位置

//スコアの初期化処理
void InitHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/体力ゲージ001.png", &g_pTextureHp);

	//HPの情報の初期化
	g_posHp = D3DXVECTOR3(HP_POS_X, HP_POS_Y, 0.0f);				//HPの位置の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHp, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void* *)&pVtx, 0);

	//頂点座標の設定
	{
		pVtx[0].pos = D3DXVECTOR3(g_posHp.x - HP_SIZE, g_posHp.y - HP_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posHp.x + HP_SIZE, g_posHp.y - HP_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posHp.x - HP_SIZE, g_posHp.y + HP_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posHp.x + HP_SIZE, g_posHp.y + HP_SIZE, 0.0f);
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャの座標
	{
		pVtx[0].tex = D3DXVECTOR2(HP_MAX, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(TEX_1 , TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_MAX, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(TEX_1 , TEX_1);
	}
   
	//頂点バッファをロックする
	g_pVtxBuffHp->Unlock();
}

//HPの終了処理
void UninitHp(void)
{
	//テクスチャの破棄
	if (g_pTextureHp != NULL)
	{
		g_pTextureHp->Release();
		g_pTextureHp = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHp != NULL)
	{
		g_pVtxBuffHp->Release();
		g_pVtxBuffHp = NULL;
	}
}

//HPの更新処理
void UpdateHp(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void* *)&pVtx, 0);

	Player *pPlayer = GetPlayer();	//プレイヤーの情報

	//体力別テクスチャ座標の更新
	switch (pPlayer->nLife)
	{
	case LIFE_0:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_0, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_1, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_0, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_1, TEX_1);
	}
		break;
	case LIFE_1:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_1, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_2, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_1, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_2, TEX_1);
	}
		break;
	case LIFE_2:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_2, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_3, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_2, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_3, TEX_1);
	}
		break;
	case LIFE_3:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_3, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_4, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_3, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_4, TEX_1);
	}
		break;
	case LIFE_4:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_4  , TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_MAX, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_4  , TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_MAX, TEX_1);
	}
		break;
	case LIFE_MAX:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_MAX, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(TEX_1 , TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_MAX, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(TEX_1 , TEX_1);
	}
		break;
	}
	
	//頂点バッファをロックする
	g_pVtxBuffHp->Unlock();
}

//HPの描画処理
void DrawHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHp, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureHp);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, SEC);
}