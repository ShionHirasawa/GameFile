//--------------------------------------------------------------------------------------------------------
//
//メッシュのリザルト用背景[chargecylinder.cpp]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "resultCylinder.h"
#include "color.h"

//マクロ定義
#define RESULTCYLINDER_HEIGHT				(1000.0f)					// シリンダーの高さ
#define RESULTCYLINDER_SPLIT				(32)						// シリンダーの分割数
#define RESULTCYLINDER_TEX_RESOLUTION		(1.0f)						// シリンダーのテクスチャの幅
#define NUM_RESULT_CYLINDER					(1)							// シリンダーの数
#define RESULTCYLINDER_ALL_VTX		(RESULTCYLINDER_SPLIT * 2 + 2)		// シリンダーの最大頂点・インデックス数
#define RESULT_MAX_RADIUS					(400.0f)					// 最大半径

//リザルト用背景情報の構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	float			fRadius;	// 半径の大きさ
	float			fHeight;	// 高さ
}ResultCylinder;

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureResultCylinder = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultCylinder = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffResultCylinder = NULL;
D3DXMATRIX				g_mtxWorldResultCylinder;	//ワールドマトリックス
ResultCylinder			g_ResultCylinder[NUM_RESULT_CYLINDER];

//--------------------------------------------------------------------------------------------------------
//リザルト用背景の初期化処理
//--------------------------------------------------------------------------------------------------------
void InitResultCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky002.png", &g_pTextureResultCylinder);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * RESULTCYLINDER_ALL_VTX * NUM_RESULT_CYLINDER, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffResultCylinder, NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * RESULTCYLINDER_ALL_VTX * NUM_RESULT_CYLINDER, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffResultCylinder, NULL);

	//リザルト用背景の情報の初期化
	for (int nCntCylinder = 0; nCntCylinder < NUM_RESULT_CYLINDER; nCntCylinder++)
	{
		g_ResultCylinder[nCntCylinder].pos = ZERO_SET;
		g_ResultCylinder[nCntCylinder].rot = ZERO_SET;
		g_ResultCylinder[nCntCylinder].fRadius = RESULT_MAX_RADIUS;
		g_ResultCylinder[nCntCylinder].fHeight = RESULTCYLINDER_HEIGHT;

		//頂点情報の設定処理
		SetResultCylinderVertex(nCntCylinder);

		//インデックス番号の設定処理
		SetResultCylinderIndex(nCntCylinder);
	}
}

//断面の頂点情報の設定処理
void SetResultCylinderVertex(int nCntCylinder)
{
	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffResultCylinder->Lock(0, 0, (void* *)&pVtx, 0);

	//ポインターをずらす
	pVtx += RESULTCYLINDER_ALL_VTX * nCntCylinder;

	float Rot = D3DX_PI;	//Y軸の角度	
	float LapSepRot = ONE_LAP / RESULTCYLINDER_SPLIT;						//１周の分割割合を求める
	float TexSep_U = RESULTCYLINDER_TEX_RESOLUTION / RESULTCYLINDER_SPLIT;	//テクスチャの横の分割の割合を求める

	//頂点座標の設定
	for (int nCntVtx = 0; nCntVtx <= RESULTCYLINDER_SPLIT; nCntVtx++)
	{
		float	VtxPos_X = sinf(Rot) * g_ResultCylinder[nCntCylinder].fRadius,	//Ｘ座標
				VtxPos_Z = cosf(Rot) * g_ResultCylinder[nCntCylinder].fRadius;	//Ｚ座標
		int		nNumTopVtx = RESULTCYLINDER_SPLIT + nCntVtx + 1;		//対象の頂点の上部の頂点番号

																		//原点位置と同じ高さの頂点座標を設定
		pVtx[nCntVtx].pos = D3DXVECTOR3(VtxPos_X, 0.0f, VtxPos_Z);

		//上で設定した頂点座標の真上の頂点座標を設定
		pVtx[nNumTopVtx].pos = D3DXVECTOR3(VtxPos_X, g_ResultCylinder[nCntCylinder].fHeight, VtxPos_Z);

		//１周したときの頂点座標
		if (nCntVtx == RESULTCYLINDER_SPLIT)
		{
			pVtx[nCntVtx].pos = pVtx[0].pos;							//最初の頂点座標を代入
			pVtx[nNumTopVtx].pos = pVtx[RESULTCYLINDER_SPLIT + 1].pos;	//最初の上部の頂点座標を代入
		}

		//法線ベクトルの設定
		pVtx[nCntVtx].nor =
			pVtx[nNumTopVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[nCntVtx].col = pVtx[nNumTopVtx].col = XCOL_WHITE;

		//テクスチャ座標の設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(nCntVtx * TexSep_U, 1.0f);
		pVtx[nNumTopVtx].tex = D3DXVECTOR2(nCntVtx * TexSep_U, 0.0f);

		//角度を　全体の角度÷分割数で割った答え分、引く
		Rot -= LapSepRot;
	}

	//頂点バッファのアンロック
	g_pVtxBuffResultCylinder->Unlock();
}

//断面のインデックス番号の設定処理
void SetResultCylinderIndex(int nCntCylinder)
{
	WORD *pIdx;
	//インデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffResultCylinder->Lock(0, 0, (void* *)&pIdx, 0);

	pIdx += RESULTCYLINDER_ALL_VTX * nCntCylinder;			//ポインターをずらす
	int nBottomNumber = ODDPARITY + RESULTCYLINDER_SPLIT;	//シリンダー下部の基準頂点番号

															//インデックス番号データの設定
	for (int nCntIdx = 0; nCntIdx < RESULTCYLINDER_ALL_VTX; nCntIdx++)
	{
		//カウンターが偶数の場合、後半以降の計算式 ↓ の答えがゼロになる	奇数の場合は、シリンダー下部の基準頂点番号が加算される
		pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + (nBottomNumber * (nCntIdx % EVENPARITY));
	}

	//インデックスバッファのアンロック
	g_pIdxBuffResultCylinder->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//リザルト用背景の終了処理
//--------------------------------------------------------------------------------------------------------
void UninitResultCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureResultCylinder != NULL)
	{
		g_pTextureResultCylinder->Release();
		g_pTextureResultCylinder = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResultCylinder != NULL)
	{
		g_pVtxBuffResultCylinder->Release();
		g_pVtxBuffResultCylinder = NULL;
	}

	//インデックスの破棄
	if (g_pIdxBuffResultCylinder != NULL)
	{
		g_pIdxBuffResultCylinder->Release();
		g_pIdxBuffResultCylinder = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//リザルト用背景の更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateResultCylinder(void)
{

}

//--------------------------------------------------------------------------------------------------------
//リザルト用背景の描画処理
//--------------------------------------------------------------------------------------------------------
void DrawResultCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultCylinder, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffResultCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultCylinder);

	//裏面カリングをON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntCylinder = 0; nCntCylinder < NUM_RESULT_CYLINDER; nCntCylinder++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldResultCylinder);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ResultCylinder[nCntCylinder].rot.y, g_ResultCylinder[nCntCylinder].rot.x, g_ResultCylinder[nCntCylinder].rot.z);
		D3DXMatrixMultiply(&g_mtxWorldResultCylinder, &g_mtxWorldResultCylinder, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_ResultCylinder[nCntCylinder].pos.x, g_ResultCylinder[nCntCylinder].pos.y, g_ResultCylinder[nCntCylinder].pos.z);
		D3DXMatrixMultiply(&g_mtxWorldResultCylinder, &g_mtxWorldResultCylinder, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldResultCylinder);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, RESULTCYLINDER_ALL_VTX, 0, RESULTCYLINDER_ALL_VTX);
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//通常カリングにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}