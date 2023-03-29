/*==========================================================================================================================================================

														ポリゴン処理[polygon.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"

#define POLYGON_POS		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define POLYGON_WIDTH	(80.0f)			//幅サイズ
#define POLYGON_DEPTH	(800.0f)		//奥行きサイズ

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;		//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTexturePolygon = NULL;		//テクスチャ
D3DXVECTOR3				g_posPolygon;					//位置
D3DXVECTOR3				g_rotPolygon;					//向き
D3DXMATRIX				g_mtxWorldPolygon;				//ワールドマトリックス

//------------------------------------------------
//				ポリゴンの初期化処理
//------------------------------------------------
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	g_posPolygon = POLYGON_POS;		//位置初期化
	g_rotPolygon = ZERO_SET;		//向き初期化

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field001.jpg", &g_pTexturePolygon);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_posPolygon.x - POLYGON_WIDTH, g_posPolygon.y, g_posPolygon.z + POLYGON_DEPTH);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_posPolygon.x + POLYGON_WIDTH, g_posPolygon.y, g_posPolygon.z + POLYGON_DEPTH);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_posPolygon.x - POLYGON_WIDTH, g_posPolygon.y, g_posPolygon.z - POLYGON_DEPTH);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_posPolygon.x  + POLYGON_WIDTH, g_posPolygon.y, g_posPolygon.z - POLYGON_DEPTH);
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
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col =  RGBA_WHITE;
	}

	//テクスチャの座標
	{
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.1f, ORIGIN_POS);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.1f, F_TEN);
	}

	//頂点バッファをロックする
	g_pVtxBuffPolygon->Unlock();
}

//------------------------------------------------
//				ポリゴンの終了処理
//------------------------------------------------
void UninitPolygon(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
	
	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//------------------------------------------------
//				ポリゴンの更新処理
//------------------------------------------------
void UpdatePolygon(void)
{

}

//------------------------------------------------
//				ポリゴンの描画処理
//------------------------------------------------
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);
	
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffPolygon, ZERO, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);	
}