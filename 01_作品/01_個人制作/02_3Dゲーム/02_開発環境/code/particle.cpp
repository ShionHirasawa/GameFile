/*==========================================================================================================================================================

														パーティクル処理[parthicle.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "particle.h"
#include "billboard.h"
#include "wall.h"
#include "color.h"

//マクロ定義
#define MAX_PARTICLE				(5000)		//パーティクルの最大数
#define PARTICLE_LIFE				(60)		//パーティクルの寿命

#define MAX_RADIUS					(10.0f)		//半径の初期値
#define MINUS_RADIUS				(0.1f)		//半径の縮小値

//パーティクルの発生に使う移動方向のマクロ
#define ANGLE						(629)		//パーティクルが移動する向き
#define FIX_ANGLE					(314)		//ANGLEの角度を修正

#define TIP_UP						(3)			//跳ね上げる移動量
#define MIN_TIP						(2)			//最低限の移動量
#define FIX_TIP_UP					(3)			//TIP_UPの倍率修正
#define NORMAL_MOVE_PARTICLE		(2.0f)		//パーティクルの基本移動量
#define SLOW_MOVE_PARTICLE			(0.5f)		//パーティクルのゆっくり移動量
#define PARTICLE_NUM_EXPLOSION		(30)		//爆発的なパーティクルの発生量

//パーティクルの重力マクロ
#define GRAVITY_PARTICLE			(-4.0f)		//パーティクルの基本重力加速度
#define NORMAL_GRAVITY_PART_COR		(0.2f)		//パーティクルの基本重力係数
#define SLOW_GRAVITY_PART_COR		(0.05f)		//パーティクルのゆっくり重力係数
#define BOUND_PARTICLE_COR			(-0.9f)		//パーティクルのバウンド補正値

#define REFLECT_PARTICEL			(-1.0f)		//跳ね返し係数

#define PARTICLE_LIMIT_WALL			(600.0f)	//壁の位置
#define PARTICLE_LIMIT_FLOOR		(000.0f)	//床の座標

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	//頂点バッファへのポインタ
D3DXMATRIX				g_mtxWorldParticle;			// ワールドマトリックス
Particle				g_aParticle[MAX_PARTICLE];	//パーティクルの情報

//--------------------------------------------------------
//				パーティクルの初期化処理
//--------------------------------------------------------
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle000.jpg", &g_pTextureParticle);	//薄いパーティクル
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffParticle, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//パーティクルの情報の初期化
		g_aParticle[nCntParticle].pos		= ZERO_SET;			// 位置の初期化
		g_aParticle[nCntParticle].move		= ZERO_SET;			// 移動量の初期化
		g_aParticle[nCntParticle].col		= PARTICLE_COL;		// 色の初期化
		g_aParticle[nCntParticle].nLife		= PARTICLE_LIFE;	// 寿命の初期化
		g_aParticle[nCntParticle].fRadius	= MAX_RADIUS;		// 半径の初期化
		g_aParticle[nCntParticle].nLife		= SET_PARTICLE_MAX;	// タイプ初期化
		g_aParticle[nCntParticle].bUse		= false;			// 使用していない状態にする

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, +g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aParticle[nCntParticle].fRadius, +g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		}

		//rhwの設定
		{
			pVtx[VTX_LE_UP].nor = SET_NOR;
			pVtx[VTX_RI_UP].nor = SET_NOR;
			pVtx[VTX_LE_DO].nor = SET_NOR;
			pVtx[VTX_RI_DO].nor = SET_NOR;
		}

		//頂点カラーの設定
		{
			pVtx[VTX_LE_UP].col = g_aParticle[nCntParticle].col;
			pVtx[VTX_RI_UP].col = g_aParticle[nCntParticle].col;
			pVtx[VTX_LE_DO].col = g_aParticle[nCntParticle].col;
			pVtx[VTX_RI_DO].col = g_aParticle[nCntParticle].col;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = ORIGIN_TEX;
			pVtx[VTX_RI_UP].tex = MA_ZE_TEX;
			pVtx[VTX_LE_DO].tex = ZE_MA_TEX;
			pVtx[VTX_RI_DO].tex = LIMIT_TEX;
		}

		pVtx += VTX_MAX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffParticle->Unlock();
}

//--------------------------------------------------------
//				パーティクルの終了処理
//--------------------------------------------------------
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

//--------------------------------------------------------
//				パーティクルの更新処理
//--------------------------------------------------------
void UpdateParticle(void)
{
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//パーティクルを移動
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//ゆっくり爆発
			if (g_aParticle[nCntParticle].nType == SET_PARTICLE_SLOW_EX)
			{
				//パーティクルに通常の重力を加える
				g_aParticle[nCntParticle].move.y += (GRAVITY_PARTICLE - g_aParticle[nCntParticle].move.y) * SLOW_GRAVITY_PART_COR;
			}
			else
			{
				//パーティクルに通常の重力を加える
				g_aParticle[nCntParticle].move.y += (GRAVITY_PARTICLE - g_aParticle[nCntParticle].move.y) * NORMAL_GRAVITY_PART_COR;
			}

			//パーティクルが 壁・床に当たったか判定
			BoundParticle(nCntParticle);

			//半径を縮小
			g_aParticle[nCntParticle].fRadius -= MINUS_RADIUS;

			//半径が0になった
			if (g_aParticle[nCntParticle].fRadius <= F_ZERO)
			{
				g_aParticle[nCntParticle].bUse = false;			//使用していない状態にする
			}

			//半径を使って頂点座標を設定
			SetParticleRadius(nCntParticle);
		}
	}
}

//--------------------------------------------------------
//				パーティクルの描画処理
//--------------------------------------------------------
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用

	//αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Zバッファの有効無効設定
	
	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//ビルボードの描画設定
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true &&				//パーティクルが使用されている
			WallDrawObject(g_aParticle[nCntParticle].pos) == true)	//パーティクルとカメラの間に壁がない
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldParticle);

			//ビューマトリックスをGet
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldParticle, NULL, &mtxView);	//逆行列を求める
			g_mtxWorldParticle._41 = F_ZERO;		//→↓
			g_mtxWorldParticle._42 = F_ZERO;		//	反転させて求めた逆行列の不要な部分をゼロにする
			g_mtxWorldParticle._43 = F_ZERO;		//→↑

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(ZERO, g_pVtxBuffParticle, ZERO, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureParticle);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * VTX_MAX, SEC);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL は 「<=」のこと
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zバッファの有効無効設定

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//パーティクルの設定処理
void SetParticle(D3DXVECTOR3 pos, SET_PARTICLE nType, D3DXCOLOR col)
{
	//発生パターン分岐
	switch (nType)
	{
	case SET_PARTICLE_NORMAL:		//パーティクル通常発生
		SetParticleNor(pos, col, nType);
		break;

	case SET_PARTICLE_SLOW_EX:
		SetParticleExplosion(pos, col, SLOW_MOVE_PARTICLE, nType);	//パーティクル爆発発生
		break;

	case SET_PARTICLE_EXPLOSION:
		SetParticleExplosion(pos, col, NORMAL_MOVE_PARTICLE, nType);	//パーティクル爆発発生
		break;
	}
}

//"通常" 発生のパーティクル設定
void SetParticleNor(D3DXVECTOR3 pos, D3DXCOLOR col, SET_PARTICLE nType)
{
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			D3DXVECTOR3 fAngle;		//パーティクルが移動する角度を設定

			fAngle.x = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;	// それぞれの方向の角度を決定
			fAngle.y = (float)(rand() % TIP_UP + MIN_TIP);				// それぞれの方向の角度を決定
			fAngle.z = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;	// それぞれの方向の角度を決定

			float fMove = (float)(rand() % ANGLE) / FIX_FLOAT + NORMAL_MOVE_PARTICLE;	// パーティクルの移動量

			//パーティクル情報の設定
			g_aParticle[nCntParticle].pos = pos;							// 位置設定
			g_aParticle[nCntParticle].nType = nType;						// 種類設定
			g_aParticle[nCntParticle].move.x = (fAngle.x) * fMove;			// 移動量設定
			g_aParticle[nCntParticle].move.y = (fAngle.y) * fMove;			// 移動量設定
			g_aParticle[nCntParticle].move.z = (fAngle.z) * fMove;			// 移動量設定
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;				// 寿命設定
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;					// 半径設定
			g_aParticle[nCntParticle].bUse = true;							// 使用している状態にする

			//ループを抜ける
			break;
		}
	}
}

//"爆発" 発生のパーティクル設定
void SetParticleExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fSpeed, SET_PARTICLE nType)
{
	//発生量カウント
	int nCntBirth = ZERO;

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			D3DXVECTOR3 fAngle;		//パーティクルが移動する角度を設定
				
			fAngle.x = (float)(rand() %  ANGLE  - FIX_ANGLE ) / FIX_FLOAT;	// それぞれの方向の角度を決定
			fAngle.y = (float)(rand() %  TIP_UP +  MIN_TIP  );				// それぞれの方向の角度を決定
			fAngle.z = (float)(rand() %  ANGLE  - FIX_ANGLE ) / FIX_FLOAT;	// それぞれの方向の角度を決定

			float fMove = (float)(rand() % ANGLE) / FIX_FLOAT + fSpeed;		// パーティクルの移動量

			//パーティクル情報の設定
			g_aParticle[nCntParticle].pos = pos;							// 位置設定
			g_aParticle[nCntParticle].col = col;							// 色設定
			g_aParticle[nCntParticle].move.x = (fAngle.x) * fMove;			// 移動量設定
			g_aParticle[nCntParticle].move.y = (fAngle.y) * fMove;			// 移動量設定
			g_aParticle[nCntParticle].move.z = (fAngle.z) * fMove;			// 移動量設定
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;				// 寿命設定
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;					// 半径設定
			g_aParticle[nCntParticle].bUse = true;							// 使用している状態にする

			//ゆっくり爆発のタイプなら Yの移動量を増やす
			if (nType == SET_PARTICLE_SLOW_EX)
			{
				g_aParticle[nCntParticle].move.y += g_aParticle[nCntParticle].move.y;
			}

			//発生量増加
			nCntBirth++;

			//発生量が規定量を超えたら終了
			if (nCntBirth >= PARTICLE_NUM_EXPLOSION)
			{
				break;
			}
		}
	}
}

//パーティクルが壁に当たった時に跳ね返す
void BoundParticle(int nCntParticle)
{
	//X座標の判定
	if (g_aParticle[nCntParticle].pos.x >= PARTICLE_LIMIT_WALL || g_aParticle[nCntParticle].pos.x <= -PARTICLE_LIMIT_WALL)
	{
		g_aParticle[nCntParticle].move.x *= REFLECT_PARTICEL;		//移動量を逆転させる
	}

	//Y座標の判定
	if (g_aParticle[nCntParticle].pos.y <= PARTICLE_LIMIT_FLOOR)
	{//バウンドさせる
		g_aParticle[nCntParticle].move.y = (GRAVITY_PARTICLE + g_aParticle[nCntParticle].move.y) * BOUND_PARTICLE_COR;
	}

	//Z座標の判定
	if (g_aParticle[nCntParticle].pos.z >= PARTICLE_LIMIT_WALL || g_aParticle[nCntParticle].pos.z <= -PARTICLE_LIMIT_WALL)
	{
		g_aParticle[nCntParticle].move.z *= REFLECT_PARTICEL;		//移動量を逆転させる
	}
}

//パーティクルの半径処理
void SetParticleRadius(int nCntParticle)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	pVtx += nCntParticle * VTX_MAX;	//頂点バッファをパーティクルの番号の数増やす

	//頂点座標の設定
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
	}

	//頂点カラーの設定
	{
		pVtx[VTX_LE_UP].col = g_aParticle[nCntParticle].col;
		pVtx[VTX_RI_UP].col = g_aParticle[nCntParticle].col;
		pVtx[VTX_LE_DO].col = g_aParticle[nCntParticle].col;
		pVtx[VTX_RI_DO].col = g_aParticle[nCntParticle].col;
	}

	//頂点バッファをロックする
	g_pVtxBuffParticle->Unlock();
}