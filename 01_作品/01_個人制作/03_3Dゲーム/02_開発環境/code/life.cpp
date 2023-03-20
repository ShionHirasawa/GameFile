/*==========================================================================================================================================================

														ライフ表示処理[life.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "life.h"
#include "player.h"

//アイコンの座標マクロ
#define LIFE_POS_X			(1120.0f)
#define LIFE_POS_Y			(680.0f)
#define LIFE_WIDTH			(140.0f)			//中心座標からのHPアイコンの幅
#define LIFE_HEIGHT			(25.0f)				//中心座標からのHPアイコンの高さ
#define SEPALATE_LIFE_TEX	(7)					//ライフアイコンのテクスチャ分割数

//残り体力
typedef enum
{
	Remain_Life_NONE = ZERO,
	Remain_Life_1,
	Remain_Life_2,
	Remain_Life_3,
	Remain_Life_4,
	Remain_Life_MAX,
}Remain_Life;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureLife = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3				g_posLife;					//ライフの位置

//残り体力別テクスチャ座標の開始位置
const float c_apStartTexLife[SEPALATE_LIFE_TEX] = {
	0.000f,
	0.166f,
	0.332f,
	0.498f,
	0.664f,
	0.830f,
	1.000f,
};

//--------------------------------------------------------
//					ライフの初期化処理
//--------------------------------------------------------
void InitLife(void)
{
	//デバイスの情報取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/lifegage002.png", &g_pTextureLife);
	
	//HPの位置の初期化
	g_posLife = D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y, NIL_F);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLife, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_posLife.x - LIFE_WIDTH, g_posLife.y - LIFE_HEIGHT, NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_posLife.x + LIFE_WIDTH, g_posLife.y - LIFE_HEIGHT, NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_posLife.x - LIFE_WIDTH, g_posLife.y + LIFE_HEIGHT, NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_posLife.x + LIFE_WIDTH, g_posLife.y + LIFE_HEIGHT, NIL_F);
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
		pVtx[VTX_LE_UP].col = D3DXCOLOR(MAX_F, MAX_F, MAX_F, MAX_F);
		pVtx[VTX_RI_UP].col = D3DXCOLOR(MAX_F, MAX_F, MAX_F, MAX_F);
		pVtx[VTX_LE_DO].col = D3DXCOLOR(MAX_F, MAX_F, MAX_F, MAX_F);
		pVtx[VTX_RI_DO].col = D3DXCOLOR(MAX_F, MAX_F, MAX_F, MAX_F);
	}

	//テクスチャの座標
	{//最大体力部分を表示
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(NIL_F, c_apStartTexLife[Remain_Life_MAX]);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_F, c_apStartTexLife[Remain_Life_MAX]);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(NIL_F, c_apStartTexLife[Remain_Life_MAX + ONE]);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_F, c_apStartTexLife[Remain_Life_MAX + ONE]);
	}

	//頂点バッファをロックする
	g_pVtxBuffLife->Unlock();
}

//--------------------------------------------------------
//					ライフの終了処理
//--------------------------------------------------------
void UninitLife(void)
{
	//テクスチャの破棄
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//--------------------------------------------------------
//					ライフの更新処理
//--------------------------------------------------------
void UpdateLife(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//プレイヤーの体力取得
	int nLife = GetPlayer()->nLife;

	//テクスチャ座標の設定
	pVtx[VTX_LE_UP].tex = D3DXVECTOR2(NIL_F, c_apStartTexLife[nLife]);
	pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_F, c_apStartTexLife[nLife]);
	pVtx[VTX_LE_DO].tex = D3DXVECTOR2(NIL_F, c_apStartTexLife[nLife + ONE]);
	pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_F, c_apStartTexLife[nLife + ONE]);

	//頂点バッファをロックする
	g_pVtxBuffLife->Unlock();
}

//--------------------------------------------------------
//					ライフの描画処理
//--------------------------------------------------------
void DrawLife(void)
{
	//デバイスの情報取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffLife, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureLife);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}