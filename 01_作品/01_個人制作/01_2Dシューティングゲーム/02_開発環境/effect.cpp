/*==========================================================================================================================

													エフェクト処理[bullet.cpp]
														Author:平澤詩苑

==========================================================================================================================*/
#include "effect.h"

//マクロ定義
#define MAX_EFFECT		(4096)		//エフェクトの最大数
#define EFFECT_VERTEX	(5.0f)		//エフェクトの大きさ
#define EFFECT_LIFE		(50)		//エフェクトの寿命

//エフェクトの構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//色
	float fRadius;			//半径(大きさ)
	int nLife;				//寿命(表示時間)
	bool bUse;				//使用しているかどうか
}Effect;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];							//エフェクトの情報

//エフェクトの初期化処理
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	//エフェクトの情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色の初期化
		g_aEffect[nCntEffect].fRadius = 0.0f;							//半径(大きさ)の初期化
		g_aEffect[nCntEffect].nLife = EFFECT_LIFE;						//寿命の初期化
		g_aEffect[nCntEffect].bUse = false;								//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		//テクスチャの座標
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffEffect->Unlock();
}

//エフェクトの終了処理
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//エフェクトの更新処理
void UpdateEffect(void)
{
	int nCntEffect;

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
			//頂点座標の設定
			{
				pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
				pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
				pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
				pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
				pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
				pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
				pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
				pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = g_aEffect[nCntEffect].col;
				pVtx[1].col = g_aEffect[nCntEffect].col;
				pVtx[2].col = g_aEffect[nCntEffect].col;
				pVtx[3].col = g_aEffect[nCntEffect].col;
			}
			
			//寿命と大きさを減らしていく
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].fRadius >= 0.0f)
			{
				g_aEffect[nCntEffect].fRadius -= 0.2f;
			}

			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].fRadius / 200.0f;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;		//使用していない状態にする
			}
		}

		//頂点データのポインタを4つ分進める
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//エフェクトの描画処理
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * NUM_VERTEX, 2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//エフェクトの設定処理
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//エフェクトが使用されていない

			//エフェクト情報の設定
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = EFFECT_LIFE;
			g_aEffect[nCntEffect].bUse = true;			//使用している状態にする

			//頂点情報の設定
			//頂点座標の設定
			{
				pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - fRadius;
				pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - fRadius;
				pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + fRadius;
				pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - fRadius;
				pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - fRadius;
				pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + fRadius;
				pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + fRadius;
				pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + fRadius;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = g_aEffect[nCntEffect].col;
				pVtx[1].col = g_aEffect[nCntEffect].col;
				pVtx[2].col = g_aEffect[nCntEffect].col;
				pVtx[3].col = g_aEffect[nCntEffect].col;
			}
			
			break;
		}
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffEffect->Unlock();
}