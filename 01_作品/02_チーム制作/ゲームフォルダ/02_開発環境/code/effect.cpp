//==========================================
//
//エフェクトプログラム[effect.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "effect.h"
#include "color.h"
#include "PvP_player.h"
#include "input.h"

//テクスチャの情報
#define NUM_EFFECT				(4)			//テクスチャの最大表示数

#define EFFECT_SIZE				(80.0f)		//エフェクトのサイズ
#define EFFECT_CHARGE_MOVE		(3.5f)		//エフェクトのチャージタイプの変化量
#define EFFECT_ATTACK_MOVE		(8.0f)		//エフェクトのアタックタイプの変化量

//マクロ定義
#define	CHARGE_EFFECT_TEX_PASS		"data\\TEXTURE\\charge_effect002.png"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	//テクスチャのポインタ
D3DXMATRIX				mtxWorldEffect;				//ワールドマトリックス
Effect					g_Effect[NUM_EFFECT];		//エフェクトの情報

//=================================
//エフェクトの初期化処理
//=================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得	

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, CHARGE_EFFECT_TEX_PASS, &g_pTextureEffect);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		g_Effect[nCntEffect].nType = EFFECTTYPE_CHARGE;	//種類初期化
		g_Effect[nCntEffect].pos = ZERO_SET;			//位置初期化
		g_Effect[nCntEffect].nCntLoop = 0;				//ループ回数初期化
		g_Effect[nCntEffect].fSize = EFFECT_SIZE;		//サイズ初期化
		g_Effect[nCntEffect].fAlpha = 1.0f;				//透明度初期化
		g_Effect[nCntEffect].bUse = false;				//使われていない状態に
		g_Effect[nCntEffect].bUseCharge = false;		//使われていない状態に

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fSize, 0.0f, +g_Effect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fSize, 0.0f, +g_Effect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Effect[nCntEffect].fSize, 0.0f, -g_Effect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Effect[nCntEffect].fSize, 0.0f, -g_Effect[nCntEffect].fSize);

		//nor(法線)の設定
		pVtx[VTX_LE_UP].nor = pVtx[VTX_RI_UP].nor = 
		pVtx[VTX_LE_DO].nor = pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = 
		pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//テクスチャ頂点座標の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//=================================
//エフェクトの終了処理
//=================================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=================================
//エフェクトの更新処理
//=================================
void UpdateEffect(void)
{
	//エフェクトの位置を設定
	SetEffectPos();

	//エフェクトのサイズ更新  (頂点座標の更新もするので、このUpdate関数の最後が望ましい)
	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		UpdateEffectSize(nCntEffect);
	}
}

//エフェクトのサイズ更新
void UpdateEffectSize(int nEffect)
{
	//エフェクトの大きさを拡大
	g_Effect[nEffect].fSize += EFFECT_CHARGE_MOVE;

	//エフェクトの大きさが最大値を超えた
	if (g_Effect[nEffect].fSize >= EFFECT_SIZE)
	{
		//エフェクト本来の最大値に直す
		g_Effect[nEffect].fSize = EFFECT_SIZE;
		g_Effect[nEffect].bUse = false;
	}

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void* *)&pVtx, 0);

	//ポインターを合わせる
	pVtx += VTX_MAX * nEffect;

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Effect[nEffect].fSize, 0.0f, +g_Effect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Effect[nEffect].fSize, 0.0f, +g_Effect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Effect[nEffect].fSize, 0.0f, -g_Effect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Effect[nEffect].fSize, 0.0f, -g_Effect[nEffect].fSize);

	//頂点カラーの設定		エフェクトのサイズが規定量を超えていれば、徐々に透明になっていく
	pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col =
	pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Effect[nEffect].fAlpha);

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//=================================
//エフェクトの描画処理
//=================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX  mtxTrans;			//計算用マトリックス

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		//エフェクトが使われている
		if (g_Effect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldEffect);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Effect[nCntEffect].pos.x, g_Effect[nCntEffect].pos.y, g_Effect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&mtxWorldEffect, &mtxWorldEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldEffect);

			//描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, VTX_MAX * nCntEffect, 2);
		}
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//エフェクトの位置設定
void SetEffectPos()
{
	for (int nCntEffect = 0; nCntEffect < NUM_EFFECT; nCntEffect++)
	{
		//対象のエフェクトが使われている
		if (g_Effect[nCntEffect].bUse == true)
		{
			//エフェクトの位置をプレイヤーの位置にする
			g_Effect[nCntEffect].pos = GetPlayer()[nCntEffect].pos;
		}
	}
}

//エフェクトの設定処理
void SetEffect(D3DXVECTOR3 pos, int nCntType, EFFECTTYPE type)
{
	//対象のエフェクトが使われていない
	if (g_Effect[nCntType].bUse == false)
	{
		//種類がチャージ
		if (type == EFFECTTYPE_CHARGE)
		{
			g_Effect[nCntType].fSize = EFFECT_SIZE;	//サイズを初期化
		}

		//種類が当たった時のもの
		else if (type == EFFECTTYPE_ATTACK)
		{
			g_Effect[nCntType].fSize = 0.0f;	//サイズを初期化
		}

		g_Effect[nCntType].nType = type;	//種類設定
		g_Effect[nCntType].nCntLoop = 0;	//ループ回数初期化
		g_Effect[nCntType].fAlpha = 1.0f;	//透明度
		g_Effect[nCntType].bUse = true;		//使われている状態に
	}
}