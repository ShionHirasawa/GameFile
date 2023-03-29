//--------------------------------------------------------------------------------------------------------
//
//メッシュの断面図[meshfault.h]
//Author:菊地陽人　平澤詩苑　石原颯馬
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshfault.h"
#include "meshfield.h"
#include "color.h"
#include "file.h"

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureMeshFault = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshFault = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshFault = NULL;
Fault					g_Fault;

//--------------------------------------------------------------------------------------------------------
//断面の初期化処理
//--------------------------------------------------------------------------------------------------------
void InitMeshFault(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\fault002.png", &g_pTextureMeshFault);

	//断面情報初期化の初期化
	g_Fault.pos = ZERO_SET;
	g_Fault.rot = ZERO_SET;

	//断面情報の取得（モデルオリジナルファイル読み込み後に行うこと）
	g_Fault = GetFaultData();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESHFAULT_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshFault, NULL);

	//頂点情報の設定処理
	SetMeshFaultVertex();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MESHFAULT_SPLIT * 2 + 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshFault, NULL);

	//インデックス番号の設定処理
	SetMeshFaultIndex();
}

//断面の頂点情報の設定処理
void SetMeshFaultVertex(void)
{
	//半径を設定
	g_Fault.fRadius = GetMeshField()->fRadius;

	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffMeshFault->Lock(0, 0, (void**)&pVtx, 0);

	float Rot = D3DX_PI;	//Y軸の角度

	//頂点座標の設定
	for (int nCntMeshFault = 0; nCntMeshFault <= MESHFAULT_SPLIT; nCntMeshFault++)
	{
		float	VtxPos_X = sinf(Rot) * g_Fault.fRadius,					//Ｘ座標
				VtxPos_Z = cosf(Rot) * g_Fault.fRadius;					//Ｚ座標
		int		nNumBottomVtx = MESHFAULT_SPLIT + nCntMeshFault + 1;	//対象の頂点の真下の頂点番号

		//原点位置と同じ高さの頂点座標を設定
		pVtx[nCntMeshFault].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//上で設定した頂点座標の真下の頂点座標を設定
		pVtx[nNumBottomVtx].pos = D3DXVECTOR3(VtxPos_X, MESHFAULT_BOTTOM, VtxPos_Z);

		//１周したときの頂点座標
		if (nCntMeshFault == MESHFAULT_SPLIT)
		{
			pVtx[nCntMeshFault].pos = pVtx[0].pos;						//最初の頂点座標を代入
			pVtx[nNumBottomVtx].pos = pVtx[MESHFAULT_SPLIT + 1].pos;	//最初の足元の頂点座標を代入
		}

		//法線ベクトルの設定
		pVtx[nCntMeshFault].nor =
		pVtx[nNumBottomVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntMeshFault].col = 
		pVtx[nNumBottomVtx].col = XCOL_WHITE;

		//テクスチャ座標の設定
		pVtx[nCntMeshFault].tex = D3DXVECTOR2(nCntMeshFault * (MESHFAULT_TEX_RESOLUTION / MESHFAULT_SPLIT), 0.0f);
		pVtx[nNumBottomVtx].tex = D3DXVECTOR2(nCntMeshFault * (MESHFAULT_TEX_RESOLUTION / MESHFAULT_SPLIT), 1.0f);

		//角度を　全体の角度÷分割数で割った答え分、引く
		Rot -= ONE_LAP / MESHFAULT_SPLIT;
	}

	//頂点バッファのアンロック
	g_pVtxBuffMeshFault->Unlock();
}

//断面のインデックス番号の設定処理
void SetMeshFaultIndex(void)
{
	WORD*pIdx;

	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffMeshFault->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	for (int nCntMeshFault = 0; nCntMeshFault < MESHFAULT_SPLIT * 2 + 2; nCntMeshFault++)
	{
		if (nCntMeshFault % EVENPARITY == NOPARITY)
		{
			pIdx[nCntMeshFault] = nCntMeshFault / EVENPARITY;
		}
		if (nCntMeshFault % EVENPARITY == ODDPARITY)
		{
			pIdx[nCntMeshFault] = (nCntMeshFault / EVENPARITY) + ODDPARITY + MESHFAULT_SPLIT;
		}
	}

	//インデックスバッファのアンロック
	g_pIdxBuffMeshFault->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//断面の終了処理
//--------------------------------------------------------------------------------------------------------
void UninitMeshFault(void)
{
	//インデックスの破棄
	if (g_pIdxBuffMeshFault != NULL)
	{
		g_pIdxBuffMeshFault->Release();
		g_pIdxBuffMeshFault = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureMeshFault != NULL)
	{
		g_pTextureMeshFault->Release();
		g_pTextureMeshFault = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshFault != NULL)
	{
		g_pVtxBuffMeshFault->Release();
		g_pVtxBuffMeshFault = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//断面の更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateMeshFault(void)
{
	//頂点情報の設定処理
	SetMeshFaultVertex();
}

//--------------------------------------------------------------------------------------------------------
//断面の描画処理
//--------------------------------------------------------------------------------------------------------
void DrawMeshFault(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Fault.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Fault.rot.y, g_Fault.rot.x, g_Fault.rot.z);
	D3DXMatrixMultiply(&g_Fault.mtxWorld, &g_Fault.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Fault.pos.x, g_Fault.pos.y, g_Fault.pos.z);
	D3DXMatrixMultiply(&g_Fault.mtxWorld, &g_Fault.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Fault.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshFault, 0, sizeof(VERTEX_3D));

	//裏面カリングをON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshFault);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshFault);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (MESHFAULT_SPLIT * 2 + 2), 0, (MESHFAULT_SPLIT * 2 + 2));

	//普通のカリングモードにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}