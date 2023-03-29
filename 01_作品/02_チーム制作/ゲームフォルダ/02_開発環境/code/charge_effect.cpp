//==========================================
//
//エフェクトプログラム[charge_effect.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "charge_effect.h"
#include "color.h"
#include "PvP_player.h"
#include "input.h"

//テクスチャの情報
#define NUM_CHARGE_EFFECT				(4)			//テクスチャの最大表示数

#define CHARGE_EFFECT_SIZE				(80.0f)		//エフェクトのサイズ
#define EFFECT_CHARGE_MOVE				(3.5f)		//エフェクトのチャージタイプの変化量

//チャージエフェクトのテクスチャパス
#define	CHARGE_EFFECT_TEX_PASS		"data\\TEXTURE\\charge_effect002.png"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChargeEffect = NULL;			//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureChargeEffect = NULL;			//テクスチャのポインタ
D3DXMATRIX				mtxWorldChargeEffect;					//ワールドマトリックス
ChargeEffect			g_ChargeEffect[NUM_CHARGE_EFFECT];		//エフェクトの情報

//=================================
//エフェクトの初期化処理
//=================================
void InitChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得	

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, CHARGE_EFFECT_TEX_PASS, &g_pTextureChargeEffect);	
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_CHARGE_EFFECT,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffChargeEffect, NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffChargeEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		g_ChargeEffect[nCntEffect].pos = ZERO_SET;				//位置初期化
		g_ChargeEffect[nCntEffect].fSize = CHARGE_EFFECT_SIZE;	//サイズ初期化
		g_ChargeEffect[nCntEffect].fAlpha = 1.0f;				//透明度初期化
		g_ChargeEffect[nCntEffect].bUse = false;				//使われていない状態に
		g_ChargeEffect[nCntEffect].bUseCharge = false;			//使われていない状態に

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_ChargeEffect[nCntEffect].fSize, 0.0f, +g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_ChargeEffect[nCntEffect].fSize, 0.0f, +g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_ChargeEffect[nCntEffect].fSize, 0.0f, -g_ChargeEffect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_ChargeEffect[nCntEffect].fSize, 0.0f, -g_ChargeEffect[nCntEffect].fSize);

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
	g_pVtxBuffChargeEffect->Unlock();
}

//=================================
//エフェクトの終了処理
//=================================
void UninitChargeEffect(void)
{	
	//テクスチャの破棄
	if (g_pTextureChargeEffect != NULL)
	{
		g_pTextureChargeEffect->Release();
		g_pTextureChargeEffect = NULL;
	}	

	//バッファの破棄
	if (g_pVtxBuffChargeEffect != NULL)
	{
		g_pVtxBuffChargeEffect->Release();
		g_pVtxBuffChargeEffect = NULL;
	}
}

//=================================
//エフェクトの更新処理
//=================================
void UpdateChargeEffect(void)
{
	//エフェクトの位置を設定
	SetChargeEffectPos();

	//エフェクトのサイズ更新  (頂点座標の更新もするので、このUpdate関数の最後が望ましい)
	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		//エフェクトが使われている
		if (g_ChargeEffect[nCntEffect].bUse)
		{
			//サイズを更新する
			UpdateChargeEffectSize(nCntEffect);
		}
	}
}

//エフェクトのサイズ更新
void UpdateChargeEffectSize(int nEffect)
{
	//エフェクトの大きさを拡大
	g_ChargeEffect[nEffect].fSize += EFFECT_CHARGE_MOVE;

	//エフェクトの大きさがゼロになった
	if (g_ChargeEffect[nEffect].fSize >= CHARGE_EFFECT_SIZE)
	{
		//エフェクト本来の大きさに直す
		g_ChargeEffect[nEffect].fSize = CHARGE_EFFECT_SIZE;
		g_ChargeEffect[nEffect].bUse = false;
	}

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffChargeEffect->Lock(0, 0, (void* *)&pVtx, 0);

	//ポインターを合わせる
	pVtx += VTX_MAX * nEffect;

	//プレイヤーのゲージ量を格納
	float Gauge = GetPlayer()[nEffect].moveGauge;

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_ChargeEffect[nEffect].fSize, 0.0f, +g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_ChargeEffect[nEffect].fSize, 0.0f, +g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_ChargeEffect[nEffect].fSize, 0.0f, -g_ChargeEffect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_ChargeEffect[nEffect].fSize, 0.0f, -g_ChargeEffect[nEffect].fSize);

	//エフェクト色設定
	pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = 
	pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f * Gauge, 1.0f - Gauge * 0.3f, 1.0f - Gauge, g_ChargeEffect[nEffect].fAlpha);

	//頂点バッファをアンロックする
	g_pVtxBuffChargeEffect->Unlock();
}

//=================================
//エフェクトの描画処理
//=================================
void DrawChargeEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX  mtxTrans;			//計算用マトリックス

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffChargeEffect, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureChargeEffect);
	
	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		//エフェクトが使われている
		if (g_ChargeEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldChargeEffect);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_ChargeEffect[nCntEffect].pos.x, g_ChargeEffect[nCntEffect].pos.y, g_ChargeEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&mtxWorldChargeEffect, &mtxWorldChargeEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldChargeEffect);

			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, VTX_MAX * nCntEffect, 2);
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//エフェクトの位置設定
void SetChargeEffectPos()
{
	for (int nCntEffect = 0; nCntEffect < NUM_CHARGE_EFFECT; nCntEffect++)
	{
		//対象のエフェクトが使われている
		if (g_ChargeEffect[nCntEffect].bUse == true)
		{
			//エフェクトの位置をプレイヤーの位置にする
			g_ChargeEffect[nCntEffect].pos = GetPlayer()[nCntEffect].pos;
		}
	}
}

//エフェクトの設定処理
void SetChargeEffect(D3DXVECTOR3 pos, int nCntType)
{
	//対象のエフェクトが使われていない
	if (g_ChargeEffect[nCntType].bUse == false)
	{
		g_ChargeEffect[nCntType].fSize = 0.0f;		//サイズを初期化
		g_ChargeEffect[nCntType].fAlpha = 1.0f;		//透明度
		g_ChargeEffect[nCntType].bUse = true;		//使われている状態に
	}
}