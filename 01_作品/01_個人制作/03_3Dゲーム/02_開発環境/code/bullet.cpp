/*==========================================================================================================================================================

															弾処理[bullet.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "bullet.h"
#include "billboard.h"
#include "color.h"
#include "player.h"
#include "input.h"
#include "particle.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "wall.h"
#include "game.h"
#include "slope.h"
#include "sound.h"
#include "cannon.h"
#include "model.h"
#include "meshfield.h"

//マクロ定義
#define MAX_BULLET				(256)		//弾の最大数
#define BULLET_HP				(200)		//弾の体力
#define BULLET_SIZE				(4.0f)		//弾の幅
#define BULLET_EFFECT_LIFE		(40)		//弾の軌跡の寿命

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//頂点バッファへのポインタ
D3DXMATRIX				g_mtxWorldBullet;				//ワールドマトリックス
Bullet					g_aBullet[MAX_BULLET];			//弾の情報

//------------------------------------------------
//					弾の初期化処理
//------------------------------------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &g_pTextureBullet);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//弾の情報の初期化
		g_aBullet[nCntBullet].pos = ZERO_SET;				// 位置初期化
		g_aBullet[nCntBullet].posOld = ZERO_SET;			// 前回の位置初期化
		g_aBullet[nCntBullet].move = ZERO_SET;				// 移動量初期化
		g_aBullet[nCntBullet].nLife = BULLET_HP;			// 寿命初期化
		g_aBullet[nCntBullet].size = F_ZERO;				// 弾のサイズ初期化
		g_aBullet[nCntBullet].bUse = false;					// 使用していない状態にする
		g_aBullet[nCntBullet].owner = BULLET_OWNER_MAX;		// オーナー初期化
		g_aBullet[nCntBullet].nIdxShadow = SetShadow();		// 影の設定

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].size, +g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].size, +g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].size, -g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].size, -g_aBullet[nCntBullet].size, F_ZERO);
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
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += VTX_MAX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffBullet->Unlock();
}

//------------------------------------------------
//					弾の終了処理
//------------------------------------------------
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//------------------------------------------------
//					弾の更新処理
//------------------------------------------------
void UpdateBullet(void)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			//前回の位置更新
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//弾の位置の更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//頂点座標の設定
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].size, +g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].size, +g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].size, -g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].size, -g_aBullet[nCntBullet].size, F_ZERO);

			//頂点カラーの設定
			pVtx[VTX_LE_UP].col = g_aBullet[nCntBullet].col;
			pVtx[VTX_RI_UP].col = g_aBullet[nCntBullet].col;
			pVtx[VTX_LE_DO].col = g_aBullet[nCntBullet].col;
			pVtx[VTX_RI_DO].col = g_aBullet[nCntBullet].col;

			//寿命を減らす
			g_aBullet[nCntBullet].nLife--;

			//弾の当たり判定まとめ
			WholeBulletCollision(nCntBullet);

			//影描画
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size, g_aBullet[nCntBullet].size);

			//エフェクト描画
			SetEffect(g_aBullet[nCntBullet].pos, XCOL_YELLOW, g_aBullet[nCntBullet].size * F_DOUBLE, BULLET_EFFECT_LIFE);

			//使われなくなった弾の初期化
			if (g_aBullet[nCntBullet].bUse == false)
			{
				//爆発設定
				SetExplosion(g_aBullet[nCntBullet].pos, XCOL_WHITE);

				BreakShadow(g_aBullet[nCntBullet].nIdxShadow);	//影の使用を破棄する
			}
		}

		//頂点データのポインタを4つ分進める
		pVtx += VTX_MAX;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//弾の当たり判定まとめ
void WholeBulletCollision(int nCntBullet)
{
	MeshField *Null;	//引数だけ

	if (CollisionWall(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move) == true ||					//壁に当たった
		ColliMeshField(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, &Null) == true ||					//地面に当たった
		ColliPlayerToBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size, g_aBullet[nCntBullet].owner) == true ||					//プレイヤーに当たった
		ColliCannonToBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size, g_aBullet[nCntBullet].owner) == true ||					//プレイヤーに当たった
		CollisionModel(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, COLLITYPE_BULLET) == true ||	//モデルに当たった
		CollisionSlope(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, &g_aBullet[nCntBullet].rot, COLLITYPE_BULLET) == true ||	//坂道に当たった
		g_aBullet[nCntBullet].nLife <= ZERO)																								//寿命が尽きている
	{
		g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
	}
}

//------------------------------------------------
//					弾の描画処理
//------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用

	//αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ビルボードの描画設定
	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBullet);

			//ビューマトリックスをGet
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldBullet, NULL, &mtxView);	//逆行列を求める
			g_mtxWorldBullet._41 = F_ZERO;		//→↓
			g_mtxWorldBullet._42 = F_ZERO;		//	反転させて求めた逆行列の不要な部分をゼロにする
			g_mtxWorldBullet._43 = F_ZERO;		//→↑

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBullet);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(ZERO, g_pVtxBuffBullet, ZERO, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * VTX_MAX, SEC);
		}
	}

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//弾の設定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size, BULLET_OWNER owner)
{
	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない

			//弾の情報を設定
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].rot.z = ZERO;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].size = size;
			g_aBullet[nCntBullet].owner = owner;

			g_aBullet[nCntBullet].nLife = BULLET_HP;
			g_aBullet[nCntBullet].bUse = true;			//使用している状態にする

			//弾の発射音再生
			PlaySound(SOUND_LABEL_SE_FILING);
			break;
		}
	}
}