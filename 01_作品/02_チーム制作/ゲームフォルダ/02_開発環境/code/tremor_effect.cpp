//==========================================
//
//エフェクトプログラム[tremor_effect.cpp]
//Author:石原颯馬（ベース：飯田洲暉）
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "tremor_effect.h"
#include "color.h"
#include "PvP_player.h"
#include "input.h"

//テクスチャの情報
#define MAX_TREMOR_EFFECT				(4)			//テクスチャの最大表示数

#define TREMOR_EFFECT_SIZE				(50.0f)		//エフェクトのサイズ
#define EFFECT_TREMOR_MOVE				(2.5f)		//エフェクトのチャージタイプの変化量

//マクロ定義
#define	TREMOR_EFFECT_TEX_PASS		"data\\TEXTURE\\charge_effect001.png"
#define TREMOR_FADEOUT_TIME				(15)		//フェードアウト開始時間（フレーム単位）

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTremorEffect = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureTremorEffect = NULL;		//テクスチャのポインタ
D3DXMATRIX				mtxWorldTremorEffect;				//ワールドマトリックス
TremorEffect			g_TremorEffect[MAX_TREMOR_EFFECT];	//エフェクトの情報

//=================================
//エフェクトの初期化処理
//=================================
void InitTremorEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得	

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TREMOR_EFFECT_TEX_PASS, &g_pTextureTremorEffect);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_TREMOR_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,D3DPOOL_MANAGED, &g_pVtxBuffTremorEffect, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTremorEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_TREMOR_EFFECT; nCntEffect++, pVtx += VTX_MAX)
	{
		g_TremorEffect[nCntEffect].nCounter = 0;				//経過時間初期化
		g_TremorEffect[nCntEffect].fSize = TREMOR_EFFECT_SIZE;	//サイズ初期化
		g_TremorEffect[nCntEffect].fAlpha = 1.0f;				//透明度初期化
		g_TremorEffect[nCntEffect].bUse = false;				//使われていない状態に

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_TremorEffect[nCntEffect].fSize, 0.0f, +g_TremorEffect[nCntEffect].fSize);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_TremorEffect[nCntEffect].fSize, 0.0f, +g_TremorEffect[nCntEffect].fSize);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_TremorEffect[nCntEffect].fSize, 0.0f, -g_TremorEffect[nCntEffect].fSize);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_TremorEffect[nCntEffect].fSize, 0.0f, -g_TremorEffect[nCntEffect].fSize);

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

	//頂点バッファをアンロック
	g_pVtxBuffTremorEffect->Unlock();
}

//=================================
//エフェクトの終了処理
//=================================
void UninitTremorEffect(void)
{	
	//テクスチャの破棄
	if (g_pTextureTremorEffect != NULL)
	{
		g_pTextureTremorEffect->Release();
		g_pTextureTremorEffect = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffTremorEffect != NULL)
	{
		g_pVtxBuffTremorEffect->Release();
		g_pVtxBuffTremorEffect = NULL;
	}
}

//=================================
//エフェクトの更新処理
//=================================
void UpdateTremorEffect(void)
{
	//エフェクト処理
	for (int nCntEffect = 0; nCntEffect < MAX_TREMOR_EFFECT; nCntEffect++)
	{
		if (g_TremorEffect[nCntEffect].bUse == true)
		{
			//カウント増やす
			g_TremorEffect[nCntEffect].nCounter++;

			//エフェクトのサイズ更新
			UpdateTremorEffectSize(nCntEffect);

			//時間過ぎたら消す
			if (g_TremorEffect[nCntEffect].nCounter > TREMOR_EFFECT_TIME)
			{
				g_TremorEffect[nCntEffect].bUse = false;
			}

			//フェードアウト開始したら透明度減らす
			else if (g_TremorEffect[nCntEffect].nCounter > TREMOR_FADEOUT_TIME)
			{
				g_TremorEffect[nCntEffect].fAlpha = 1.0f - (float)(g_TremorEffect[nCntEffect].nCounter - TREMOR_FADEOUT_TIME) / (TREMOR_EFFECT_TIME - TREMOR_FADEOUT_TIME);
			}
		}
	}
}

//エフェクトのサイズ更新
void UpdateTremorEffectSize(int nEffect)
{
	//エフェクトの大きさを拡大
	g_TremorEffect[nEffect].fSize += EFFECT_TREMOR_MOVE;

	//エフェクトの大きさがゼロになった
	if (g_TremorEffect[nEffect].fSize >= TREMOR_EFFECT_SIZE)
	{
		//エフェクト本来の大きさに直す
		g_TremorEffect[nEffect].fSize = TREMOR_EFFECT_SIZE;
	}

	VERTEX_3D *pVtx;							//頂点情報へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTremorEffect->Lock(0, 0, (void**)&pVtx, 0);

	//ポインターを合わせる
	pVtx += VTX_MAX * nEffect;

	//頂点座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_TremorEffect[nEffect].fSize, 0.0f, +g_TremorEffect[nEffect].fSize);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_TremorEffect[nEffect].fSize, 0.0f, +g_TremorEffect[nEffect].fSize);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_TremorEffect[nEffect].fSize, 0.0f, -g_TremorEffect[nEffect].fSize);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_TremorEffect[nEffect].fSize, 0.0f, -g_TremorEffect[nEffect].fSize);

	//頂点カラーの設定		エフェクトのサイズが規定量を超えていれば、徐々に透明になっていく
	pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col =
	pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_TremorEffect[nEffect].fAlpha);

	//頂点バッファをアンロックする
	g_pVtxBuffTremorEffect->Unlock();
}

//=================================
//エフェクトの描画処理
//=================================
void DrawTremorEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス

	for (int nCntEffect = 0; nCntEffect < MAX_TREMOR_EFFECT; nCntEffect++)
	{
		if (g_TremorEffect[nCntEffect].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxWorldTremorEffect);

			//Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_TremorEffect[nCntEffect].pos.x, g_TremorEffect[nCntEffect].pos.y, g_TremorEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&mtxWorldTremorEffect, &mtxWorldTremorEffect, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorldTremorEffect);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffTremorEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTremorEffect);

			//描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, VTX_MAX * nCntEffect, 2);

			//Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
		}
	}
}

//エフェクトの設定処理
void SetTremorEffect(D3DXVECTOR3 pos)
{
	for (int nCntEffect = 0; nCntEffect < MAX_TREMOR_EFFECT; nCntEffect++)
	{
		//対象のエフェクトが使われていない
		if (g_TremorEffect[nCntEffect].bUse == false)
		{
			g_TremorEffect[nCntEffect].pos = pos;			//位置設定
			g_TremorEffect[nCntEffect].fSize = 0.0f;		//サイズを初期化			
			g_TremorEffect[nCntEffect].nCounter = 0;		//ループ回数初期化
			g_TremorEffect[nCntEffect].fAlpha = 1.0f;		//透明度
			g_TremorEffect[nCntEffect].bUse = true;			//使われている状態に
			break;
		}
	}
}