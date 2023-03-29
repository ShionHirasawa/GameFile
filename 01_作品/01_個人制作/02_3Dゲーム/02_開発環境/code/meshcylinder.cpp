/*==========================================================================================================================================================

													メッシュシリンダー処理[meshcylinder.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "meshcylinder.h"

#define CYLINDER_POS		D3DXVECTOR3(0.0f, 0.0f, 0.0f)	//初期位置
#define CYLINDER_WIDTH		(600.0f)						//幅サイズ
#define CYLINDER_DEPTH		(600.0f)						//奥行きサイズ
#define MESHFIELD_VERTEX	(14)							//メッシュフィールドの頂点数
#define SEPALATE_MESH		(12)							//メッシュフィールドの分割数

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;		//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureMeshCylinder = NULL;		//テクスチャ
D3DXMATRIX				g_mtxWorldMeshCylinder;				//ワールドマトリックス
MeshCylinder			g_MeshCylinder;						//メッシュシリンダーの情報

//------------------------------------------------
//		メッシュシリンダーの初期化処理
//------------------------------------------------
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	g_MeshCylinder.pos = CYLINDER_POS;	//位置初期化
	g_MeshCylinder.rot = ZERO_SET;		//向き初期化

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/木箱000.png", &g_pTextureMeshCylinder);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshCylinder, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[0].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, NIL_F);
		pVtx[1].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, +CYLINDER_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(NIL_F, NIL_F, NIL_F);
		pVtx[3].pos = D3DXVECTOR3(NIL_F, NIL_F, +CYLINDER_DEPTH);
		pVtx[4].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, NIL_F);
		pVtx[5].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, +CYLINDER_DEPTH);
		pVtx[6].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, +CYLINDER_DEPTH);
		pVtx[7].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, -CYLINDER_DEPTH);
		pVtx[8].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, -CYLINDER_DEPTH);
		pVtx[9].pos = D3DXVECTOR3(-CYLINDER_WIDTH, NIL_F, NIL_F);
		pVtx[10].pos = D3DXVECTOR3(NIL_F, NIL_F, -CYLINDER_DEPTH);
		pVtx[11].pos = D3DXVECTOR3(NIL_F, NIL_F, NIL_F);
		pVtx[12].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, -CYLINDER_DEPTH);
		pVtx[13].pos = D3DXVECTOR3(+CYLINDER_WIDTH, NIL_F, NIL_F);
	}

	//norの設定
	{
		pVtx[0].nor = SET_NOR;
		pVtx[1].nor = SET_NOR;
		pVtx[2].nor = SET_NOR;
		pVtx[3].nor = SET_NOR;
		pVtx[4].nor = SET_NOR;
		pVtx[5].nor = SET_NOR;
		pVtx[6].nor = SET_NOR;
		pVtx[7].nor = SET_NOR;
		pVtx[8].nor = SET_NOR;
		pVtx[9].nor = SET_NOR;
		pVtx[10].nor = SET_NOR;
		pVtx[11].nor = SET_NOR;
		pVtx[12].nor = SET_NOR;
		pVtx[13].nor = SET_NOR;
	}

	//頂点カラーの設定
	{
		pVtx[0].col = RGBA_WHITE;
		pVtx[1].col = RGBA_WHITE;
		pVtx[2].col = RGBA_WHITE;
		pVtx[3].col = RGBA_WHITE;
		pVtx[4].col = RGBA_WHITE;
		pVtx[5].col = RGBA_WHITE;
		pVtx[6].col = RGBA_WHITE;
		pVtx[7].col = RGBA_WHITE;
		pVtx[8].col = RGBA_WHITE;
		pVtx[9].col = RGBA_WHITE;
		pVtx[10].col = RGBA_WHITE;
		pVtx[11].col = RGBA_WHITE;
		pVtx[12].col = RGBA_WHITE;
		pVtx[13].col = RGBA_WHITE;
	}

	//テクスチャの座標
	{
		//テクスチャ座標  0.0f   0.0f		//テクスチャ座標  0.5f   0.0f		//テクスチャ座標  1.0f   0.0f
		pVtx[1].tex = ORIGIN_TEX;			pVtx[3].tex = HA_ZE_TEX;			pVtx[5].tex = MA_ZE_TEX;
		pVtx[6].tex = MA_ZE_TEX;

		//テクスチャ座標  0.0f   0.5f		//テクスチャ座標  0.5f   0.5f		//テクスチャ座標  1.0f   0.5f
		pVtx[0].tex = ZE_HA_TEX;			pVtx[2].tex = HALF_TEX;				pVtx[4].tex = MA_HA_TEX;
		pVtx[9].tex = ZE_HA_TEX;			pVtx[11].tex = HALF_TEX;			pVtx[13].tex = MA_HA_TEX;

		//テクスチャ座標  0.0f   1.0f		//テクスチャ座標  0.5f   1.0f		//テクスチャ座標  1.0f   1.0f
		pVtx[7].tex = ZE_MA_TEX;			pVtx[10].tex = HA_MA_TEX;			pVtx[12].tex = LIMIT_TEX;
		pVtx[8].tex = ZE_MA_TEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffMeshCylinder->Unlock();
}

//------------------------------------------------
//		メッシュシリンダーの終了処理
//------------------------------------------------
void UninitMeshCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
}

//------------------------------------------------
//		メッシュシリンダーの更新処理
//------------------------------------------------
void UpdateMeshCylinder(void)
{

}

//------------------------------------------------
//		メッシュシリンダーの描画処理
//------------------------------------------------
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshCylinder);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshCylinder.rot.y, g_MeshCylinder.rot.x, g_MeshCylinder.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshCylinder.pos.x, g_MeshCylinder.pos.y, g_MeshCylinder.pos.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCylinder);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffMeshCylinder, ZERO, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureMeshCylinder);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEPALATE_MESH);
}