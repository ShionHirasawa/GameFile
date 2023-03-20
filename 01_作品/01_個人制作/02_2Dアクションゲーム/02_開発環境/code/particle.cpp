/*==========================================================================================================================

パーティクル処理[main.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "particle.h"
#include "player.h"

//マクロ定義
#define NUM_PARTICLE				(3)				//パーティクルの種類
#define MAX_PARTICLE				(100)			//パーティクルの最大数
#define PARTICLE_LIFE				(60)			//パーティクルの寿命
#define MAX_RADIUS					(30.0f)			//半径の初期値
#define MINUS_RADIUS				(0.3f)			//半径の縮小値
#define ANGLE						(629)			//パーティクルが移動する向き
#define FIX_ANGLE					(314)			//ANGLEの角度を修正
#define FIX_FLOAT					(100.0f)		//角度をfloat型に修正
#define MOVE_PARTICLE				(2.0f)			//パーティクルの基本移動量
#define PARTICLE_COL				(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f))

#define SHADE_PASS_THIN					"data/TEXTURE/effect000.jpg"
#define SHADE_PASS_NORMAL				"data/TEXTURE/effect001.jpg"
#define SHADE_PASS_DARK					"data/TEXTURE/effect002.jpg"

//パーティクルの濃淡の構造体
typedef enum
{
	SHADE_THIN  = ZERO,		//薄い
	SHADE_NORMAL,			//普通
	SHADE_DARK  ,			//濃い
	SHADE_MAX
}SHADE_PARTICLE;

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
LPDIRECT3DTEXTURE9		g_pTextureParticle[NUM_PARTICLE] = {};	//テクスチャへのポインタ
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
	D3DXCreateTextureFromFile(pDevice, SHADE_PASS_THIN,		&g_pTextureParticle[SHADE_THIN]);		//薄いパーティクル
	D3DXCreateTextureFromFile(pDevice, SHADE_PASS_NORMAL,	&g_pTextureParticle[SHADE_NORMAL]);		//普通のパーティクル
	D3DXCreateTextureFromFile(pDevice, SHADE_PASS_DARK,		&g_pTextureParticle[SHADE_DARK]);		//濃いパーティクル


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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_PARTICLE * NUM_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffParticle, NULL);

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
	//テクスチャ(3枚)の破棄
	for (int nCntParticle = START_CNT; nCntParticle < NUM_PARTICLE; nCntParticle++)
	{
		if (g_pTextureParticle[nCntParticle] != NULL)
		{
			g_pTextureParticle[nCntParticle]->Release();
			g_pTextureParticle[nCntParticle] = NULL;
		}
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

			if (g_aParticle[nCntParticle].nLife <= 40 || GetPlayer()->state == PLAYERSTATE_FALL)
			{
				g_aParticle[nCntParticle].move.x *= F_ONE;
				g_aParticle[nCntParticle].move.y = cosf(D3DX_PI * UP) * MOVE_PARTICLE;
			}

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

	//αブレンディングをわけのわからない合成に設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//エフェクトが使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle[nCntParticle % SAN]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * NUM_VERTEX, SEC);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//パーティクルの設定処理
void SetParticle(D3DXVECTOR3 pos, int nNum)
{
	VERTEX_2D *pVtx;
	
	//発生量カウント
	int nCntBirth = ZERO;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += NUM_VERTEX)
	{
		//パーティクルの発生量を調整
		if (nCntBirth < nNum)
		{
			if (g_aParticle[nCntParticle].bUse == false)
			{

				float fAngleParticle = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;		//パーティクルが移動する角度を設定
				float fMoveParticle = (float)(rand() % ANGLE) / FIX_FLOAT + MOVE_PARTICLE;	//パーティクルの移動量

				//パーティクル情報の設定
				g_aParticle[nCntParticle].pos = pos;									//位置
				g_aParticle[nCntParticle].move.x = sinf(fAngleParticle) * fMoveParticle;//移動量
				g_aParticle[nCntParticle].move.y = cosf(fAngleParticle) * fMoveParticle;//移動量
				g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;						//寿命
				g_aParticle[nCntParticle].fRadius = MAX_RADIUS;							//半径
				g_aParticle[nCntParticle].bUse = true;									//使用している状態にする

				nCntBirth++;
				//頂点情報の設定
				{
					//頂点座標の設定
					pVtx[ZERO].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
					pVtx[ZERO].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
					pVtx[ONE].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
					pVtx[ONE].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
					pVtx[SEC].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
					pVtx[SEC].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;
					pVtx[SAN].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
					pVtx[SAN].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;

					//頂点座標の設定
					pVtx[ZERO].col = PARTICLE_COL;
					pVtx[ONE].col = PARTICLE_COL;
					pVtx[SEC].col = PARTICLE_COL;
					pVtx[SAN].col = PARTICLE_COL;
				}
			}
		}
		else
		{
			break;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffParticle->Unlock();
}