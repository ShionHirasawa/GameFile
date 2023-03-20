/*==========================================================================================================================

												ゲーム背景の処理[bg.cpp]
													Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "Bg.h"
#include "input.h"

//マクロ定義
#define NUM_BG		(3)			//背景の数
#define F_MOVE_ONE	(0.001f)
#define F_MOVE_SEC	(0.002f)
#define F_MOVE_SAN	(0.003f)


//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureBg[NUM_BG] = { NULL, NULL ,NULL};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBg = NULL;						//頂点バッファへのポインタ		
float g_aTexV[NUM_BG];													//テクスチャ座標の開始位置（V値）
int g_BackAnimNum;
int g_BackTexNum;

//背景の初期化処理
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntBG;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureBg[ZERO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureBg[ONE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureBg[SEC]);

	//テクスチャ座標開始位置（V値）の初期化
	for(nCntBG = START_CNT;nCntBG < NUM_BG;nCntBG++)
	{
		g_aTexV[nCntBG] = F_ZERO;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_BG, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBG = ZERO; nCntBG < NUM_BG; nCntBG++)
	{
		//頂点座標の設定
		{
			pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, ORIGIN_POS);
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
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX , COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX , COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX , COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, g_aTexV[nCntBG]);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , g_aTexV[nCntBG]);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , g_aTexV[nCntBG] + F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , g_aTexV[nCntBG] + F_TEN);
		}

		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffBg->Unlock();
}

//背景の終了処理
void UninitBg(void)
{
	//テクスチャ(３枚分)の破棄
	for (int nCntBG = START_CNT; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_pTextureBg[nCntBG] != NULL)
		{
			g_pTextureBg[nCntBG]->Release();
			g_pTextureBg[nCntBG] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{			  
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//背景の更新処理
void UpdateBg(void)
{
	int nCntBG;
	D3DXVECTOR2 moveWidth (F_MOVE_SAN, -F_MOVE_SAN);			//背景を横に動かす

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBG = START_CNT; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャ座標の開始位置の更新
		g_aTexV[nCntBG] -= F_MOVE_ONE + (nCntBG * F_MOVE_SEC);

		if (GetKeyboardPress(DIK_A) == true)
		{
			pVtx[ZERO].tex.x -= moveWidth.x;
			pVtx[ONE].tex.x  -= moveWidth.x;
			pVtx[SEC].tex.x  -= moveWidth.x;
			pVtx[SAN].tex.x  -= moveWidth.x;
			pVtx[ZERO].tex.y += moveWidth.y;
			pVtx[ONE].tex.y  += moveWidth.y;
			pVtx[SEC].tex.y  += moveWidth.y;
			pVtx[SAN].tex.y  += moveWidth.y;

			g_aTexV[nCntBG] += moveWidth.y + F_MOVE_ONE + (nCntBG * F_MOVE_SEC);
		}

		else if (GetKeyboardPress(DIK_D) == true)
		{
			pVtx[ZERO].tex += moveWidth;
			pVtx[ONE].tex += moveWidth;
			pVtx[SEC].tex += moveWidth;
			pVtx[SAN].tex += moveWidth;

			g_aTexV[nCntBG] += moveWidth.y + F_MOVE_ONE + (nCntBG * F_MOVE_SEC);
		}

		//テクスチャの座標
		else
		{
			pVtx[ZERO].tex.y = g_aTexV[nCntBG];
			pVtx[ONE].tex.y = g_aTexV[nCntBG];
			pVtx[SEC].tex.y = g_aTexV[nCntBG] + F_TEN;
			pVtx[SAN].tex.y = g_aTexV[nCntBG] + F_TEN;
		}

		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffBg->Unlock();
}

//背景の描画処理
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntBG;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBg, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = START_CNT; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureBg[nCntBG]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * NUM_VERTEX, SEC);
	}
}