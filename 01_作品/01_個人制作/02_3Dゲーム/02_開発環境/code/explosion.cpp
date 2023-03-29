/*==========================================================================================================================================================

															爆発処理[explosion.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "explosion.h"
#include "shadow.h"
#include "color.h"
#include "billboard.h"
#include "sound.h"
#include "wall.h"
#include "input.h"

//マクロ定義
#define MAX_EXPLOSION		(128)		//爆発の最大数
#define EXPLOSION_SIZE		(10.0f)		//爆発のサイズ
#define EXPLO_ANIM_STEP		(6)			//爆発のアニメーション変化カウンタ
#define EXPLO_ANIM_MAX		(8)			//爆発のアニメーション最大値
#define EXPLO_TEX_WIDTH		(0.125f)	//爆発のテクスチャの幅
#define EXPLO_ALPHATEST		(100)		//爆発のαテスト値

//爆発構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXCOLOR	col;			//色
	int			nCounterAnim;	//アニメーションカウンター
	int			nPatternAnim;	//アニメーションパターンNo
	bool		bUse;			//使用しているかどうか
}Explosion;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		//頂点バッファへのポインタ
D3DXMATRIX				g_mtxWorldExplosion;			//ワールドマトリックス
Explosion				g_aExplosion[MAX_EXPLOSION];	//爆発の情報

//--------------------------------------------------------
//					爆発の初期化処理
//--------------------------------------------------------
void InitExplosion(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion002.png", &g_pTextureExplosion);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//爆発の情報の初期化
	for (int nCntExplosion = START_CNT; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = ZERO_SET;				//位置の初期化
		g_aExplosion[nCntExplosion].col = XCOL_WHITE;			//色の初期化
		g_aExplosion[nCntExplosion].nCounterAnim = ZERO;		//アニメーションカウンターの初期化
		g_aExplosion[nCntExplosion].nPatternAnim = ZERO;		//アニメーションパターンNoの初期化
		g_aExplosion[nCntExplosion].bUse = false;				//使用していない状態にする

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-EXPLOSION_SIZE, +EXPLOSION_SIZE, F_ZERO);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+EXPLOSION_SIZE, +EXPLOSION_SIZE, F_ZERO);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-EXPLOSION_SIZE, -EXPLOSION_SIZE, F_ZERO);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+EXPLOSION_SIZE, -EXPLOSION_SIZE, F_ZERO);
		}

		//norの設定
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
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		}

		pVtx += VTX_MAX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffExplosion->Unlock();
}

//--------------------------------------------------------
//					爆発の終了処理
//--------------------------------------------------------
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//--------------------------------------------------------
//					爆発の更新処理
//--------------------------------------------------------
void UpdateExplosion(void)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntExplosion = START_CNT; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている
			//アニメーションカウンタ更新
			g_aExplosion[nCntExplosion].nCounterAnim++;

			//カウンターが更新カウントと同じになった
			if (g_aExplosion[nCntExplosion].nCounterAnim == EXPLO_ANIM_STEP)
			{
				g_aExplosion[nCntExplosion].nPatternAnim++;			//パターンNo.更新

				g_aExplosion[nCntExplosion].nCounterAnim = ZERO;	//カウンター初期化

				//パターンNo.が最大値以上になった
				if (g_aExplosion[nCntExplosion].nPatternAnim >= EXPLO_ANIM_MAX)
				{
					g_aExplosion[nCntExplosion].nPatternAnim = ZERO;	//パターンNo.初期化

					g_aExplosion[nCntExplosion].bUse = false;			//使用していない状態にする
				}
			}

			//テクスチャ座標の更新
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim, ORIGIN_POS);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim + EXPLO_TEX_WIDTH, ORIGIN_POS);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim, MAX_POS);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim + EXPLO_TEX_WIDTH, MAX_POS);
		}

		//頂点データのポインタを4つ分進める
		pVtx += VTX_MAX;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//--------------------------------------------------------
//					爆発の描画処理
//--------------------------------------------------------
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用
	BillTest *pBill = GetAlphaTest();			//ビルボードのα・Zテスト情報を取得

	//αテストを有効にする
	if (pBill->bAlphaTest == true)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, EXPLO_ALPHATEST);
	}

	//Zテストを無効にする
	if (pBill->bZTest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Zバッファの有効無効設定
	}

	//ビルボードの描画設定
	for (int nCntExplosion = START_CNT; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true &&					//爆発が使用されている
			WallDrawObject(g_aExplosion[nCntExplosion].pos) == true)	//爆発とカメラの間に壁がない
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldExplosion);

			//ビューマトリックスをGet
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_mtxWorldExplosion, NULL, &mtxView);	//逆行列を求める
			g_mtxWorldExplosion._41 = F_ZERO;		//→↓
			g_mtxWorldExplosion._42 = F_ZERO;		//	反転させて求めた逆行列の不要な部分をゼロにする
			g_mtxWorldExplosion._43 = F_ZERO;		//→↑

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldExplosion, &g_mtxWorldExplosion, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldExplosion);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(ZERO, g_pVtxBuffExplosion, ZERO, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureExplosion);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * VTX_MAX, SEC);
		}
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL は 「<=」のこと
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zバッファの有効無効設定

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
}

//爆発の設定処理
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntExplosion = START_CNT; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//爆発が使用されていない

		 //爆発情報の設定
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = ZERO;
			g_aExplosion[nCntExplosion].nPatternAnim = ZERO;

			//頂点情報の設定
			//頂点カラーの設定
			{
				pVtx[VTX_LE_UP].col = g_aExplosion[nCntExplosion].col;
				pVtx[VTX_RI_UP].col = g_aExplosion[nCntExplosion].col;
				pVtx[VTX_LE_DO].col = g_aExplosion[nCntExplosion].col;
				pVtx[VTX_RI_DO].col = g_aExplosion[nCntExplosion].col;
			}

			//テクスチャの座標
			{
				pVtx[VTX_LE_UP].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim, ORIGIN_POS);
				pVtx[VTX_RI_UP].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim + EXPLO_TEX_WIDTH, ORIGIN_POS);
				pVtx[VTX_LE_DO].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim, MAX_POS);
				pVtx[VTX_RI_DO].tex = D3DXVECTOR2(EXPLO_TEX_WIDTH * g_aExplosion[nCntExplosion].nPatternAnim + EXPLO_TEX_WIDTH, MAX_POS);
			}

			g_aExplosion[nCntExplosion].bUse = true;			//使用している状態にする

			//爆発音再生
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			break;
		}
		pVtx += VTX_MAX;
	}

	//頂点バッファをロックする
	g_pVtxBuffExplosion->Unlock();
}