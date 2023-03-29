/*==========================================================================================================================================================

														草原表示処理[grassland.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "grassland.h"

#define NUM_GRASSLAND			(3)
#define NUM_GRASSLAND_TEX		(20)
#define NUM_GRASSLAND_XFILE		(3)
#define GRASSLAND_INFOFILE		"data/CSV/grassland.csv"

//草原の情報構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	int			nIdxGrassLand;	//使う草原の種類の番号
}GrassLand;

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureGrassLand[NUM_GRASSLAND_TEX] = {};	// テクスチャへのポインタ
LPD3DXMESH				g_pMeshGrassLand[NUM_GRASSLAND_XFILE];			// メッシュ(頂点情報)へのポインタ
D3DXMATRIX				g_mtxWorldGrassLand;							// ワールドマトリックス
LPD3DXBUFFER			g_pBuffMatGrassLand[NUM_GRASSLAND_XFILE];		// マテリアルへのポインタ
DWORD					g_dwNumMatGrassLand[NUM_GRASSLAND_XFILE];		// マテリアルの数
GrassLand				g_GrassLand[NUM_GRASSLAND];						// 草原の情報

//草原のXファイル名
const char *c_apGrassLandTexName[NUM_GRASSLAND_XFILE] = {
	"data/MODEL/GrassLands000.x",
	"data/MODEL/GrassLands001.x",
	"data/MODEL/GrassLands002.x",
};

//----------------------------------------------------
//					草原の初期化処理
//----------------------------------------------------
void InitGrassLand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//草原の情報初期化
	LoadGrassLand();

	for (int nCntGrassLand = START_CNT; nCntGrassLand < NUM_GRASSLAND_XFILE; nCntGrassLand++)
	{
		g_pMeshGrassLand[nCntGrassLand] = NULL;				// メッシュ初期化
		g_pBuffMatGrassLand[nCntGrassLand] = NULL;			// マテリアルへのポインタ初期化
		g_dwNumMatGrassLand[nCntGrassLand] = ZERO;			// マテリアルの数初期化

		// Xファイル読み込み
		D3DXLoadMeshFromX(c_apGrassLandTexName[nCntGrassLand], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatGrassLand[nCntGrassLand], NULL, &g_dwNumMatGrassLand[nCntGrassLand], &g_pMeshGrassLand[nCntGrassLand]);

		//モデルのテクスチャ設定
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatGrassLand[nCntGrassLand]; nCntMat++)
		{
			//マテリアルデータへのポインタを取得
			D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatGrassLand[nCntGrassLand]->GetBufferPointer();

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
			 //テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureGrassLand[nCntMat]);
			}
		}
	}
}

//草原の情報読み込み処理
void LoadGrassLand(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(GRASSLAND_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルタイトル読み込み

		for (int nCntGrassLand = START_CNT; nCntGrassLand < NUM_GRASSLAND; nCntGrassLand++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			g_GrassLand[nCntGrassLand].pos.x = atof(strtok(&aData[ZERO], ","));		// 開始位置初期化
			g_GrassLand[nCntGrassLand].pos.y = atof(strtok(NULL, ","));				// 開始位置初期化
			g_GrassLand[nCntGrassLand].pos.z = atof(strtok(NULL, ","));				// 開始位置初期化

			g_GrassLand[nCntGrassLand].rot.x = atof(strtok(NULL, ",")) / FIX_FLOAT;	// 開始向き初期化
			g_GrassLand[nCntGrassLand].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;	// 開始向き初期化
			g_GrassLand[nCntGrassLand].rot.z = atof(strtok(NULL, ",")) / FIX_FLOAT;	// 開始向き初期化

			g_GrassLand[nCntGrassLand].nIdxGrassLand = atoi(strtok(NULL, ","));		// 使用するモデル番号設定
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//----------------------------------------------------
//					草原の終了処理
//----------------------------------------------------
void UninitGrassLand(void)
{
	for (int nCntGrassLand = START_CNT; nCntGrassLand < NUM_GRASSLAND_XFILE; nCntGrassLand++)
	{
		//メッシュ破棄
		if (g_pMeshGrassLand[nCntGrassLand] != NULL)
		{
			g_pMeshGrassLand[nCntGrassLand]->Release();
			g_pMeshGrassLand[nCntGrassLand] = NULL;
		}

		//マテリアル破棄
		if (g_pBuffMatGrassLand[nCntGrassLand] != NULL)
		{
			g_pBuffMatGrassLand[nCntGrassLand]->Release();
			g_pBuffMatGrassLand[nCntGrassLand] = NULL;
		}
	}

	//テクスチャ破棄
	for (int nCntTex = START_CNT; nCntTex < NUM_GRASSLAND_TEX; nCntTex++)
	{
		if (g_pTextureGrassLand[nCntTex] != NULL)
		{
			g_pTextureGrassLand[nCntTex]->Release();
			g_pTextureGrassLand[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------------
//					草原の描画処理
//----------------------------------------------------
void DrawGrassLand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntGrassLand = START_CNT; nCntGrassLand < NUM_GRASSLAND; nCntGrassLand++)
	{
		//草原のモデル番号
		int Idx = g_GrassLand[nCntGrassLand].nIdxGrassLand;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldGrassLand);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_GrassLand[nCntGrassLand].rot.y, g_GrassLand[nCntGrassLand].rot.x, g_GrassLand[nCntGrassLand].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldGrassLand, &g_mtxWorldGrassLand, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_GrassLand[nCntGrassLand].pos.x, g_GrassLand[nCntGrassLand].pos.y, g_GrassLand[nCntGrassLand].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldGrassLand, &g_mtxWorldGrassLand, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldGrassLand);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)g_pBuffMatGrassLand[Idx]->GetBufferPointer();

		//マテリアル設定
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatGrassLand[Idx]; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureGrassLand[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshGrassLand[Idx]->DrawSubset(nCntMat);
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}