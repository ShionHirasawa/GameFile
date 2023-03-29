/*==========================================================================================================================================================

															影の処理[shadow.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "input.h"
#include "color.h"
#include "shadow.h"
#include "meshfield.h"
#include "model.h"
#include "slope.h"
#include "player.h"

#define SHADOW_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define SHADOW_DISP_MIN_Y	(-200.0f)						//影を表示できる最低Y座標
#define SHADOW_ALPTH		(200)							//影のα値
#define SHADOW_ALPHATEST	(0)								//影のαテストの基準値
#define MAX_SHADOW			(512)							//影の最大数

//影の構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	int				aColor;		// 影のα値
	float			fWidht;		// 幅
	float			fHeight;	// 坂道の時だけに使用
	float			fDepth;		// 奥行き
	bool			bSet;		// 対象の影が決まっているかどうか
	bool			bUse;		// 使われているかどうか
}Shadow;

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	//テクスチャ
D3DXMATRIX				g_mtxWorldShadow;			//ワールドマトリックス
Shadow					g_aShadow[MAX_SHADOW];		//影の情報
int g_nAlphaShadow = 0;

//------------------------------------------------
//				影の初期化処理
//------------------------------------------------
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_SHADOW, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx+= VTX_MAX)
	{
		g_aShadow[nCntShadow].pos = SHADOW_POS;			//位置初期化
		g_aShadow[nCntShadow].rot = ZERO_SET;			//向き初期化
		g_aShadow[nCntShadow].fWidht = NIL_F;			//幅初期化
		g_aShadow[nCntShadow].fHeight = NIL_F;			//幅初期化
		g_aShadow[nCntShadow].fDepth = NIL_F;			//奥行き初期化
		g_aShadow[nCntShadow].aColor = SHADOW_ALPTH;	//影のα値初期化
		g_aShadow[nCntShadow].bSet = false;				//使用されていない
		g_aShadow[nCntShadow].bUse = false;				//使用されていない

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidht, +g_aShadow[nCntShadow].fHeight, +g_aShadow[nCntShadow].fDepth);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidht, +g_aShadow[nCntShadow].fHeight, +g_aShadow[nCntShadow].fDepth);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidht, -g_aShadow[nCntShadow].fHeight, -g_aShadow[nCntShadow].fDepth);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidht, -g_aShadow[nCntShadow].fHeight, -g_aShadow[nCntShadow].fDepth);
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
			pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(F_TEN, F_TEN);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffShadow->Unlock();

	g_nAlphaShadow = 0;
}

//------------------------------------------------
//				影の終了処理
//------------------------------------------------
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//------------------------------------------------
//				影の更新処理
//------------------------------------------------
void UpdateShadow(void)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++ ,pVtx += VTX_MAX)
	{
		//対象の影が使われていて、何に使う影か決まっている
		if (g_aShadow[nCntShadow].bUse == true && g_aShadow[nCntShadow].bSet == true)
		{
			//頂点座標の設定
			{
				pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidht, +g_aShadow[nCntShadow].fHeight, +g_aShadow[nCntShadow].fDepth);
				pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidht, +g_aShadow[nCntShadow].fHeight, +g_aShadow[nCntShadow].fDepth);
				pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidht, -g_aShadow[nCntShadow].fHeight, -g_aShadow[nCntShadow].fDepth);
				pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidht, -g_aShadow[nCntShadow].fHeight, -g_aShadow[nCntShadow].fDepth);
			}

			//頂点カラーの設定
			{
				pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
				pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
				pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
				pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffShadow->Unlock();
}

//------------------------------------------------
//				影の描画処理
//------------------------------------------------
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, g_nAlphaShadow);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldShadow);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(ZERO, g_pVtxBuffShadow, ZERO, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureShadow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * VTX_MAX, SEC);
		}
	}

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, g_nAlphaShadow);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//影の設定処理
int SetShadow(void)
{
	int nCntShadow = MINUS_ONE;

	for (int mCntShadow = START_CNT; mCntShadow < MAX_SHADOW; mCntShadow++)
	{
		nCntShadow++;

		if (g_aShadow[mCntShadow].bUse == false && g_aShadow[mCntShadow].bSet == false)
		{
			g_aShadow[mCntShadow].pos = SHADOW_POS;	//位置初期化
			g_aShadow[mCntShadow].rot = ZERO_SET;	//向き初期化
			g_aShadow[mCntShadow].bSet = true;		//対象が決まった
			break;
		}
	}

	return nCntShadow;
}

//影の位置設定処理
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fWIdth, float fDepth) 
{
	g_aShadow[nIdxShadow].pos = pos;

	//影を描画するY座標の設定
	DrawPos_YShadow(nIdxShadow);

	//影と影を使っているもののY座標の距離
	float fLength = (pos.y - g_aShadow[nIdxShadow].pos.y) * 1.3f;

	//影とその影の持ち主の距離が離れすぎている
	if (fLength >= SHADOW_ALPTH)
	{
		fLength = SHADOW_ALPTH;
	}

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	pVtx += VTX_MAX * nIdxShadow;		//頂点座標をインデックス分進める

	g_aShadow[nIdxShadow].fWidht = fWIdth;									//幅調整
	g_aShadow[nIdxShadow].fDepth = fDepth;									//奥行き調整
	g_aShadow[nIdxShadow].aColor = SHADOW_ALPTH - (int)fLength;				//α値調整
	g_aShadow[nIdxShadow].bUse = true;										//使用する

	//頂点座標の設定
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidht, +g_aShadow[nIdxShadow].fHeight, +g_aShadow[nIdxShadow].fDepth);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aShadow[nIdxShadow].fWidht, +g_aShadow[nIdxShadow].fHeight, +g_aShadow[nIdxShadow].fDepth);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidht, -g_aShadow[nIdxShadow].fHeight, -g_aShadow[nIdxShadow].fDepth);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aShadow[nIdxShadow].fWidht, -g_aShadow[nIdxShadow].fHeight, -g_aShadow[nIdxShadow].fDepth);
	}

	//頂点カラーの設定
	{
		pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
	}

	//頂点バッファをロックする
	g_pVtxBuffShadow->Unlock();
}

//影を描画するY座標の位置設定
void DrawPos_YShadow(int nIdxShadow)
{	
	float FixPosY = SHADOW_DISP_MIN_Y;		//修正する位置
	g_aShadow[nIdxShadow].fHeight = F_ZERO;	//高さ初期化
	g_aShadow[nIdxShadow].rot = ZERO_SET;	//向き初期化

	//メッシュフィールドに影を描画するかの判定
	ColliFieldShadow(g_aShadow[nIdxShadow].pos, &FixPosY);

	//モデルの上部に影を描画するかの判定
	ColliModelShadow(nIdxShadow, g_aShadow[nIdxShadow].pos, &FixPosY);

	//坂道に影を描画する
	ColliSlopeShadow(g_aShadow[nIdxShadow].pos, &g_aShadow[nIdxShadow].rot, &FixPosY, &g_aShadow[nIdxShadow].fHeight);

	//描画する位置修正
	g_aShadow[nIdxShadow].pos.y = FixPosY;
}

//影の使用を無くす
void BreakShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}