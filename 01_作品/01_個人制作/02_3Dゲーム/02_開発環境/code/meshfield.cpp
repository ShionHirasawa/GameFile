/*==========================================================================================================================================================

													メッシュフィールド処理[meshfield.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "meshfield.h"

#define NUM_MESHFIELD				(4)						//メッシュフィールドの数
#define NUM_MESHFIELD_TEX			(3)						//メッシュフィールドのテクスチャ数
#define MESHFIELD_DIVISION_WIDTH	(3)						//横の分割数
#define MESHFIELD_DIVISION_DEPTH	(3)						//奥行きの分割数
#define MESHFIELD_CAVEIN_HEIGHT		(1.0f)					//当たり判定に使うめり込む深さ
#define MESHFIELD_FILE				"data/CSV/meshfield.csv"//メッシュフィールドの情報が入っているファイル名

#define MESHFIELD_VERTEX			(9)						//メッシュフィールドの頂点数
#define MESHFIELD_INDEX_VTX			(14)					//インデックスバッファの数
#define SEPALATE_MESHFIELD			(12)					//メッシュフィールドの分割数

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;						//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureMeshField[NUM_MESHFIELD_TEX] = {};	//テクスチャ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;						//インデックスバッファへのポインタ
D3DXMATRIX				g_mtxWorldMeshField;							//ワールドマトリックス
MeshField				g_MeshField[NUM_MESHFIELD];						//メッシュフィールドの情報

//地面のテクスチャ名まとめ
const char *c_apMeshFieldTexName[NUM_MESHFIELD_TEX] = {
	"data/TEXTURE/cross_section.png",			//地面のテクスチャ名
	"data/TEXTURE/grassland000.png",			//少し色あせた草原のテクスチャ名
	"data/TEXTURE/grassland001.png",			//草原のテクスチャ名
};

//------------------------------------------------
//		メッシュフィールドの初期化処理
//------------------------------------------------
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//メッシュフィールドの情報読み込み
	LoadMeshField();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX * NUM_MESHFIELD, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshField, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//テクスチャの読み込み
	for (int nCntTex = START_CNT; nCntTex < NUM_MESHFIELD_TEX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apMeshFieldTexName[nCntTex], &g_pTextureMeshField[nCntTex]);
	}

	//それぞれのメッシュフィールドの頂点情報の設定
	for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++, pVtx += MESHFIELD_VERTEX)
	{
		//頂点情報の設定
		for (int nCntVtx = START_CNT; nCntVtx < MESHFIELD_VERTEX; nCntVtx++)
		{
			//頂点座標の設定
			pVtx[nCntVtx].pos.x = -g_MeshField[nCntMeshField].fWidth * (ONE - (nCntVtx % MESHFIELD_DIVISION_WIDTH));	//カウンターを "横の分割数で割った余り" を使って横の座標を割り出す
			pVtx[nCntVtx].pos.z = +g_MeshField[nCntMeshField].fDepth * (ONE - (nCntVtx / MESHFIELD_DIVISION_WIDTH));	//カウンターを "奥の分割数で割った答え" を使って奥の座標を割り出す
			pVtx[nCntVtx].pos.y = NIL_F;																				//平面なので、Y座標は変わらず

			//nor の設定
			pVtx[nCntVtx].nor = SET_NOR;

			//col の設定
			pVtx[nCntVtx].col = RGBA_WHITE;

			//テクスチャ座標の座標
			pVtx[nCntVtx].tex.x = g_MeshField[nCntMeshField].fTexPos.x + g_MeshField[nCntMeshField].fTexWidth * (nCntVtx % MESHFIELD_DIVISION_WIDTH);		//カウンターを "横の分割数で割った余り" を使って 基準位置から横の座標を割り出す
			pVtx[nCntVtx].tex.y = g_MeshField[nCntMeshField].fTexPos.y + g_MeshField[nCntMeshField].fTexHeight * (nCntVtx / MESHFIELD_DIVISION_DEPTH);		//カウンターを "奥の分割数で割った答え" を使って 基準位置から奥の座標を割り出す
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffMeshField->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_VTX * NUM_MESHFIELD, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshField, NULL);

	//インデックス情報のバッファ
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pIdx, ZERO);

	//インデックスバッファの設定
	for (int nCntIdx = START_CNT; nCntIdx < NUM_MESHFIELD; nCntIdx++, pIdx += MESHFIELD_INDEX_VTX)
	{
		int nNumVtx = ZERO;		//頂点番号

		for (int mCntIdx = START_CNT; mCntIdx < MESHFIELD_INDEX_VTX; mCntIdx++)
		{
			//縮退ポリゴンにならない、通常の頂点
			if (mCntIdx != 6 && mCntIdx != 7)
			{
				//カウンターが偶数の場合
				if (mCntIdx % EVENPARITY == NOPARITY)
				{
					//インデックス番号設定
					pIdx[mCntIdx] = nNumVtx + MESHFIELD_DIVISION_WIDTH;		//インデックス番号 = 基準の頂点番号 + 幅の分割数
				}

				//カウンターが奇数の場合
				else if (mCntIdx % EVENPARITY == ODDPARITY)
				{
					//インデックス番号設定
					pIdx[mCntIdx] = nNumVtx;								//インデックス番号 = 基準の頂点番号

					//基準の頂点番号加算
					nNumVtx++;
				}
			}

			//縮退ポリゴンの最初の重複頂点はひとつ前のインデックス番号と同じ
			else if (mCntIdx == 6)
			{
				pIdx[mCntIdx] = pIdx[nCntIdx - ONE];
			}

			//縮退ポリゴンの最後の重複頂点は次のインデックス番号と同じ
			else if (mCntIdx == 7)
			{
				pIdx[mCntIdx] = pIdx[nCntIdx + ONE];
			}
		}
	}

	//インデックスバッファをロックする
	g_pIdxBuffMeshField->Unlock();
}

//メッシュフィールドの情報読み込み
void LoadMeshField(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(MESHFIELD_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルタイトル読み込み

		for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			//配置場所の初期化
			g_MeshField[nCntMeshField].pos.x = atof(strtok(&aData[ZERO], ","));	// 位置初期化
			g_MeshField[nCntMeshField].pos.y = atof(strtok(NULL, ","));			// 位置初期化
			g_MeshField[nCntMeshField].pos.z = atof(strtok(NULL, ","));			// 位置初期化

			g_MeshField[nCntMeshField].rot = ZERO_SET;							// 向き初期化

			g_MeshField[nCntMeshField].fWidth = atof(strtok(NULL, ","));		// 幅の初期化
			g_MeshField[nCntMeshField].fDepth = atof(strtok(NULL, ","));		// 奥行きの初期化

			g_MeshField[nCntMeshField].fTexPos.x = atof(strtok(NULL, ","));		// テクスチャの位置初期化
			g_MeshField[nCntMeshField].fTexPos.y = atof(strtok(NULL, ","));		// テクスチャの位置初期化
			g_MeshField[nCntMeshField].fTexWidth = atof(strtok(NULL, ","));		// テクスチャの幅初期化
			g_MeshField[nCntMeshField].fTexHeight = atof(strtok(NULL, ","));	// テクスチャの高さ初期化
			g_MeshField[nCntMeshField].nTexType = atoi(strtok(NULL, ","));		// テクスチャのタイプ
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//------------------------------------------------
//		メッシュフィールドの終了処理
//------------------------------------------------
void UninitMeshField(void)
{
	//テクスチャの破棄
	for (int nCntTex = START_CNT; nCntTex < NUM_MESHFIELD_TEX; nCntTex++)
	{
		if (g_pTextureMeshField[nCntTex] != NULL)
		{
			g_pTextureMeshField[nCntTex]->Release();
			g_pTextureMeshField[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//------------------------------------------------
//		メッシュフィールドの更新処理
//------------------------------------------------
void UpdateMeshField(void)
{

}

//------------------------------------------------
//		メッシュフィールドの描画処理
//------------------------------------------------
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldMeshField);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField[nCntMeshField].rot.y, g_MeshField[nCntMeshField].rot.x, g_MeshField[nCntMeshField].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_MeshField[nCntMeshField].pos.x, g_MeshField[nCntMeshField].pos.y, g_MeshField[nCntMeshField].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(ZERO, g_pVtxBuffMeshField, ZERO, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshField);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureMeshField[g_MeshField[nCntMeshField].nTexType]);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, nCntMeshField * MESHFIELD_VERTEX, ZERO, MESHFIELD_VERTEX * NUM_MESHFIELD, ZERO, SEPALATE_MESHFIELD);
	}
}

//メッシュフィールドの情報取得
MeshField *GetMeshField(void)
{
	return &g_MeshField[0];
}

//メッシュフィールドとの当たり判定
bool ColliMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, MeshField **pField)
{
	for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++)
	{
		//前回は対象のメッシュフィールドより上に居て、今は下にめり込んでしまっている
		if (g_MeshField[nCntMeshField].pos.x - g_MeshField[nCntMeshField].fWidth <= pPos->x &&	//左端より右にいる
			g_MeshField[nCntMeshField].pos.x + g_MeshField[nCntMeshField].fWidth >= pPos->x &&	//右端より左にいる
			g_MeshField[nCntMeshField].pos.z - g_MeshField[nCntMeshField].fDepth <= pPos->z &&	//手前より奥にいる
			g_MeshField[nCntMeshField].pos.z + g_MeshField[nCntMeshField].fDepth >= pPos->z &&	//奥側より前にいる
			g_MeshField[nCntMeshField].pos.y - MESHFIELD_CAVEIN_HEIGHT <= pPosOld->y &&			//めり込み位置以上でもあり
			g_MeshField[nCntMeshField].pos.y >= pPos->y)				//フィールド位置以上でもある
		{
			//メッシュフィールドのY座標の位置に戻す
			pPos->y = g_MeshField[nCntMeshField].pos.y;

			//Y軸の移動量をゼロにする
			pMove->y = F_ZERO;

			//メッシュフィールドの情報を渡す
			*pField = &g_MeshField[nCntMeshField];

			//着地した
			return true;
		}
	}

	//どのフィールドにも乗っていないのでポインタを消去する
	pField = NULL;

	//着地判定を返す
	return false;
}

//メッシュフィールドに影を描画する
void ColliFieldShadow(D3DXVECTOR3 Pos, float *pDrawPosY)
{
	//すべてのフィールドで判定
	for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++)
	{
		if (g_MeshField[nCntMeshField].pos.x - g_MeshField[nCntMeshField].fWidth <= Pos.x &&	//左端より右にいる
			g_MeshField[nCntMeshField].pos.x + g_MeshField[nCntMeshField].fWidth >= Pos.x &&	//右端より左にいる
			g_MeshField[nCntMeshField].pos.z - g_MeshField[nCntMeshField].fDepth <= Pos.z &&	//手前より奥にいる
			g_MeshField[nCntMeshField].pos.z + g_MeshField[nCntMeshField].fDepth >= Pos.z &&	//奥側より前にいる
			g_MeshField[nCntMeshField].pos.y <= Pos.y)											//前回は上に居て、いまは下にめり込んでる
		{
			//修正する位置が対象のメッシュフィールドより下にいる時、そのメッシュフィールドの位置を修正位置とする
			if (*pDrawPosY <= g_MeshField[nCntMeshField].pos.y)
			{
				//修正位置代入
				*pDrawPosY = g_MeshField[nCntMeshField].pos.y;
			}
		}
	}
}