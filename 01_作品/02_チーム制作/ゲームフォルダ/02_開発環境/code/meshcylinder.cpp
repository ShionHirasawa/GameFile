//--------------------------------------------------------------------------------------------------------
//
//メッシュシリンダーの地面の断面[meshcylinder.cpp]
//Author:菊地陽人
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshcylinder.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;
D3DXVECTOR3 g_posMeshCylinder;
D3DXVECTOR3 g_rotMeshCylinder;
D3DXMATRIX g_mtxWorldMeshCylinder;	//ワールドマトリックス

//--------------------------------------------------------------------------------------------------------
//ポリゴンの初期化処理
//--------------------------------------------------------------------------------------------------------
void InitMeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky002.png", &g_pTextureMeshCylinder);

	//ポリゴンの初期化
	g_posMeshCylinder = 
	g_rotMeshCylinder = ZERO_SET;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESHSYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshCylinder, NULL);

	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MESHSYLINDER_SPLIT + 1; nCntMeshCylinder++)
	{
		pVtx[nCntMeshCylinder].pos = D3DXVECTOR3(
			sinf(D3DX_PI * (1.0f - (2.0f / MESHSYLINDER_SPLIT * nCntMeshCylinder))) *MESHSYLINDER_WIDTH,
			MESHSYLINDER_HEIGHT,
			cosf(D3DX_PI * (1.0f - (2.0f / MESHSYLINDER_SPLIT * nCntMeshCylinder))) * MESHSYLINDER_WIDTH);

		pVtx[MESHSYLINDER_SPLIT + nCntMeshCylinder + 1].pos = D3DXVECTOR3(
			sinf(D3DX_PI * (1.0f - (2.0f / MESHSYLINDER_SPLIT * nCntMeshCylinder))) *MESHSYLINDER_WIDTH,
			0.0f,
			cosf(D3DX_PI * (1.0f - (2.0f / MESHSYLINDER_SPLIT * nCntMeshCylinder))) * MESHSYLINDER_WIDTH);

		if (nCntMeshCylinder == MESHSYLINDER_SPLIT)
		{
			pVtx[nCntMeshCylinder].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) *MESHSYLINDER_WIDTH,
				MESHSYLINDER_HEIGHT,
				cosf(D3DX_PI * 1.0f) * MESHSYLINDER_WIDTH);

			pVtx[MESHSYLINDER_SPLIT + 1 + nCntMeshCylinder].pos = D3DXVECTOR3(
				sinf(D3DX_PI * 1.0f) *MESHSYLINDER_WIDTH,
				0.0f,
				cosf(D3DX_PI * 1.0f) * MESHSYLINDER_WIDTH);
		}

		//法線ベクトルの設定
		pVtx[nCntMeshCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[MESHSYLINDER_SPLIT + 1 + nCntMeshCylinder].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntMeshCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[MESHSYLINDER_SPLIT + 1 + nCntMeshCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[nCntMeshCylinder].tex = D3DXVECTOR2(nCntMeshCylinder * (MESHSYLINDER_TEX_RESOLUTION / MESHSYLINDER_SPLIT), 0.0f);
		pVtx[MESHSYLINDER_SPLIT + 1 + nCntMeshCylinder].tex = D3DXVECTOR2(nCntMeshCylinder * (MESHSYLINDER_TEX_RESOLUTION / MESHSYLINDER_SPLIT), 1.0f);
	}

	//頂点バッファのアンロック
	g_pVtxBuffMeshCylinder->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MESHSYLINDER_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshCylinder, NULL);

	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MESHSYLINDER_SPLIT * 2 + 2; nCntMeshCylinder++)
	{
		if (nCntMeshCylinder % EVENPARITY == NOPARITY)
		{
			pIdx[nCntMeshCylinder] = nCntMeshCylinder / EVENPARITY;
		}
		if (nCntMeshCylinder % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntMeshCylinder] = (nCntMeshCylinder / EVENPARITY) + ODDPARITY + MESHSYLINDER_SPLIT;
		}
	}

	//インデックスバッファのアンロック
	g_pIdxBuffMeshCylinder->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//ポリゴンの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitMeshCylinder(void)
{
	//インデックスの破棄
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//ポリゴンの更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateMeshCylinder(void)
{

}

//--------------------------------------------------------------------------------------------------------
//ポリゴンの描画処理
//--------------------------------------------------------------------------------------------------------
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshCylinder);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshCylinder.y, g_rotMeshCylinder.x, g_rotMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshCylinder.x, g_posMeshCylinder.y, g_posMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCylinder);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (MESHSYLINDER_SPLIT * 2 + 2), 0, (MESHSYLINDER_SPLIT * 2 + 2));
}