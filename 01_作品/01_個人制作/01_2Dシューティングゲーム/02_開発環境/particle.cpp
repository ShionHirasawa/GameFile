/*==========================================================================================================================

												パーティクル処理[main.cpp]
													Author:平澤詩苑

==========================================================================================================================*/
#include "explosion.h"
#include "particle.h"

//マクロ定義
#define MAX_PARTICLE				(2048)			//パーティクルの最大数
#define PARTICLE_LIFE				(100)			//パーティクルの寿命
#define MAX_RADIUS					(30.0f)			//半径の初期値
#define MINUS_RADIUS				(1.0f)			//半径の縮小値
#define ANGLE						(629)			//パーティクルが移動する向き
#define FIX_ANGLE					(314)			//ANGLEの角度を修正
#define FIX_FLOAT					(100.0f)		//角度をfloat型に修正
#define MOVE_PARTICLE				(5.0f)			//パーティクルの基本移動量

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;				//位置(発生位置)
	D3DXVECTOR3 move;				//移動量
	D3DXCOLOR col;					//色
	int nLife;						//寿命(発生時間)
	float fRadius;					//パーティクルの半径
	bool bUse;						//使用しているかどうか
}Particle;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	//頂点バッファへのポインタ
Particle g_aParticle[MAX_PARTICLE];					//パーティクルの情報

//パーティクルの初期化
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntParticle;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureParticle);

	//パーティクルの情報の初期化
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期化
		g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;					//寿命の初期化
		g_aParticle[nCntParticle].fRadius = MAX_RADIUS;						//半径の初期化
		g_aParticle[nCntParticle].bUse = false;								//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffParticle, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
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
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f);
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
	g_pVtxBuffParticle->Unlock();
}

//パーティクルの終了処理
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//パーティクルの更新処理
void UpdateParticle(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += NUM_VERTEX)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//パーティクルを移動
			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;

			//半径を縮小
			g_aParticle[nCntParticle].fRadius -= MINUS_RADIUS;

			//寿命を減少させる
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].fRadius <= 0.0f || g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}

			//頂点座標の設定
			{
				pVtx[0].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
				pVtx[0].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
				pVtx[1].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
				pVtx[1].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
				pVtx[2].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
				pVtx[2].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;
				pVtx[3].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
				pVtx[3].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;
			}			
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffParticle->Unlock();
}

//パーティクルの描画処理
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureParticle);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//エフェクトが使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * NUM_VERTEX, 2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//パーティクルの設定処理
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, ParticleType nType)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void* *)&pVtx, 0);
		
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += NUM_VERTEX)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//パーティクルが使用されていない
			float fAngleParticle;
			float fMoveParticle;

			//パーティクルの移動についての仕様
			switch (nType)
			{
			case PARTICLETYPE_STAY:	//動かない
				fAngleParticle = 0.0f;		//パーティクルが移動する角度を設定
				fMoveParticle  = 0.0f;	//パーティクルの移動量
				break;

			case PARTICLETYPE_MOVE:	//動く
				fAngleParticle = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;		//パーティクルが移動する角度を設定
				fMoveParticle = (float)(rand() % ANGLE) / FIX_FLOAT + MOVE_PARTICLE;	//パーティクルの移動量
				break;
			}

			 //パーティクル情報の設定
			g_aParticle[nCntParticle].pos = pos;									//位置
			g_aParticle[nCntParticle].move.x = sinf(fAngleParticle) * fMoveParticle;//移動量
			g_aParticle[nCntParticle].move.y = cosf(fAngleParticle) * fMoveParticle;//移動量
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;						//寿命
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;							//半径
			g_aParticle[nCntParticle].bUse = true;									//使用している状態にする

			//頂点情報の設定
			{
				//頂点座標の設定
				pVtx[0].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
				pVtx[0].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
				pVtx[1].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
				pVtx[1].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
				pVtx[2].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
				pVtx[2].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;
				pVtx[3].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
				pVtx[3].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;

				//頂点座標の設定
				pVtx[0].col = col;
				pVtx[1].col = col;
				pVtx[2].col = col;
				pVtx[3].col = col;
			}
			break;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffParticle->Unlock();
}