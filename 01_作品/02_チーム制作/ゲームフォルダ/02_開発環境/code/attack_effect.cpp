//==========================================
//
//エフェクトプログラム[charge_effect.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "attack_effect.h"
#include "color.h"
#include "PvP_player.h"
#include "input.h"

//テクスチャの情報
#define NUM_ATTACK_EFFECT				(4)			//テクスチャの最大表示数
#define ATTACK_EFFECT_SIZE				(50.0f)		//エフェクトのサイズ
#define ATTACK_EFFECT_MAX_SIZE			(150.0f)	//エフェクトの最大サイズ
#define EFFECT_ATTACK_MOVE				(3.0f)		//エフェクトのアタックタイプの変化量
#define EFFECT_CLEAR_ACCEL				(0.1f)		//エフェクトの透明加速度

//テクスチャファイルパス
#define	ATTACK_EFFECT_TEX_PASS			"data\\TEXTURE\\AttackEffect.png"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAttackEffect = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureAttackEffect = NULL;		//テクスチャのポインタ
D3DXMATRIX				mtxWorldAttackEffect;				//ワールドマトリックス
AttackEffect			g_AttackEffect[NUM_ATTACK_EFFECT];	//エフェクトの情報

//=================================
//エフェクトの初期化処理
//=================================
void InitAttackEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ATTACK_EFFECT_TEX_PASS, &g_pTextureAttackEffect);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_ATTACK_EFFECT,	//頂点数
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffAttackEffect, NULL);
	
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffAttackEffect->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		g_AttackEffect[nCntEffect].pos = ZERO_SET;				//位置初期化
		g_AttackEffect[nCntEffect].nCntLoop = 0;				//ループ回数初期化
		g_AttackEffect[nCntEffect].fSize = ATTACK_EFFECT_SIZE;	//サイズ初期化
		g_AttackEffect[nCntEffect].fAlpha = 1.0f;				//透明度初期化
		g_AttackEffect[nCntEffect].fResize = 0.0f;				//サイズ変更値
		g_AttackEffect[nCntEffect].bUse = false;				//使われていない状態に

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_AttackEffect[nCntEffect].fSize, 0.0f, +g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_AttackEffect[nCntEffect].fSize, 0.0f, +g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_AttackEffect[nCntEffect].fSize, 0.0f, -g_AttackEffect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_AttackEffect[nCntEffect].fSize, 0.0f, -g_AttackEffect[nCntEffect].fSize);

		//nor(法線)の設定
		pVtx[VTX_LE_UP].nor = pVtx[VTX_RI_UP].nor = 
		pVtx[VTX_LE_DO].nor = pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = 
		pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = RGBA_RED;

		//テクスチャ頂点座標の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffAttackEffect->Unlock();
}

//=================================
//エフェクトの終了処理
//=================================
void UninitAttackEffect(void)
{	
	//テクスチャの破棄
	if (g_pTextureAttackEffect != NULL)
	{
		g_pTextureAttackEffect->Release();
		g_pTextureAttackEffect = NULL;
	}	

	//バッファの破棄
	if (g_pVtxBuffAttackEffect != NULL)
	{
		g_pVtxBuffAttackEffect->Release();
		g_pVtxBuffAttackEffect = NULL;
	}
}

//=================================
//エフェクトの更新処理
//=================================
void UpdateAttackEffect(void)
{
	//エフェクトの位置を設定
	SetAttackEffectPos();

	//エフェクトのサイズ更新  (頂点座標の更新もするので、このUpdate関数の最後が望ましい)
	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		//エフェクトのサイズ更新
		UpdateAttackEffectSize(nCntEffect);
	}
}

//=================================
//エフェクトのサイズ更新
//=================================
void UpdateAttackEffectSize(int nEffect)
{
	//エフェクトのサイズが最大値以下
	if (g_AttackEffect[nEffect].fSize <= ATTACK_EFFECT_MAX_SIZE)
	{
		//サイズの可変値を加算
		g_AttackEffect[nEffect].fResize += EFFECT_ATTACK_MOVE;

		//エフェクトの大きさを拡大
		g_AttackEffect[nEffect].fSize += g_AttackEffect[nEffect].fResize;
	}
	
	//エフェクトの大きさが規定値になった
	if (g_AttackEffect[nEffect].fSize >= ATTACK_EFFECT_SIZE)
	{
		//徐々に透明にしていく
		g_AttackEffect[nEffect].fAlpha -= EFFECT_CLEAR_ACCEL;

		//透明になった
		if (g_AttackEffect[nEffect].fAlpha <= 0.0f)
		{			
			g_AttackEffect[nEffect].fSize = ATTACK_EFFECT_SIZE;	//エフェクト本来の大きさに直す
			g_AttackEffect[nEffect].bUse = false;				//使わなくする
		}
	}

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffAttackEffect->Lock(0, 0, (void* *)&pVtx, 0);

	//ポインターを合わせる
	pVtx += VTX_MAX * nEffect;

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_AttackEffect[nEffect].fSize, 0.0f, +g_AttackEffect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_AttackEffect[nEffect].fSize, 0.0f, +g_AttackEffect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_AttackEffect[nEffect].fSize, 0.0f, -g_AttackEffect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_AttackEffect[nEffect].fSize, 0.0f, -g_AttackEffect[nEffect].fSize);

	//頂点カラーの設定		エフェクトのサイズが規定量を超えていれば、徐々に透明になっていく
	pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = 
	pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_AttackEffect[nEffect].fAlpha);

	//頂点バッファをアンロックする
	g_pVtxBuffAttackEffect->Unlock();
}

//=================================
//エフェクトの描画処理
//=================================
void DrawAttackEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX  mtxTrans;					//計算用マトリックス
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAttackEffect, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureAttackEffect);
	
	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		//エフェクトが使われている
		if (g_AttackEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldAttackEffect);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_AttackEffect[nCntEffect].pos.x, g_AttackEffect[nCntEffect].pos.y, g_AttackEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&mtxWorldAttackEffect, &mtxWorldAttackEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldAttackEffect);

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
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
}

//=================================
//エフェクトの位置設定
//=================================
void SetAttackEffectPos()
{
	for (int nCntEffect = 0; nCntEffect < NUM_ATTACK_EFFECT; nCntEffect++)
	{
		//対象のエフェクトが使われている
		if (g_AttackEffect[nCntEffect].bUse == true)
		{
			//エフェクトの位置をプレイヤーの位置にする
			g_AttackEffect[nCntEffect].pos = GetPlayer()[nCntEffect].pos;
		}
	}
}

//=================================
//エフェクトの設定処理
//=================================
void SetAttackEffect(D3DXVECTOR3 pos, int nCntType)
{
	//対象のエフェクトが使われていない
	if (g_AttackEffect[nCntType].bUse == false)
	{
		g_AttackEffect[nCntType].fSize = 0.0f;		//サイズを初期化
		g_AttackEffect[nCntType].nCntLoop = 0;		//ループ回数初期化
		g_AttackEffect[nCntType].fAlpha = 1.0f;		//透明度
		g_AttackEffect[nCntType].fResize = 0.0f;	//サイズ変更値初期化
		g_AttackEffect[nCntType].bUse = true;		//使われている状態に
	}
}