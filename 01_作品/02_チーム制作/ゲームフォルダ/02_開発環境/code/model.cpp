//==========================================
//
//モデル関係プログラム[model.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "model.h"
#include "file.h"

//マクロ
#define MAX_PLACE_OBJ		(256)
#define MAX_TEMPSTR_CHAR	(4096)

//グローバル変数
Model g_aAnimalModel[ANIMAL_MAX];		//動物モデル構造体
Model g_aObjModel[OBJECTTYPE_MAX];		//オブジェクトモデル設計図構造体

//========================
//初期化処理
//========================
void InitModel(void)
{
	//動物モデルの初期化
	InitAnimalModel();

	//オブジェクトモデルの初期化
	InitObjectModel();
}

//========================
//動物モデル初期化処理
//========================
void InitAnimalModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//読み込み
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aAnimalModel[nCntModel].aParts[nCntParts] = {};
			g_aAnimalModel[nCntModel].aParts[nCntParts].bUse = false;
		}

		//使用するモーション類読み込み
		LoadMotionViewerFile(GetMotionFilePath(nCntModel), &g_aAnimalModel[nCntModel]);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (SUCCEEDED(D3DXLoadMeshFromX(
				&g_aAnimalModel[nCntModel].aModelFileName[nCntParts][0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat,
				NULL,
				&g_aAnimalModel[nCntModel].aParts[nCntParts].dwNumMatModel,
				&g_aAnimalModel[nCntModel].aParts[nCntParts].pMesh)))
			{//読み込み成功したら
				//テクスチャ読み込み
				D3DXMATERIAL *pMat;	//マテリアルポインタ
				
				//マテリアル情報に対するポインタ取得
				pMat = (D3DXMATERIAL *)g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat->GetBufferPointer();

				for (int nCntTex = 0; nCntTex < (int)g_aAnimalModel[nCntModel].aParts[nCntParts].dwNumMatModel; nCntTex++)
				{
					if (pMat[nCntTex].pTextureFilename != NULL)
					{
						//テクスチャ読み込み
						D3DXCreateTextureFromFile(pDevice,
							pMat[nCntTex].pTextureFilename,
							&g_aAnimalModel[nCntModel].aParts[nCntParts].apTexture[nCntTex]);
					}
				}

				//使用している状態にする
				g_aAnimalModel[nCntModel].aParts[nCntParts].bUse = true;
			}
			else
			{//失敗したら1モデル分終了
				break;
			}
		}
	}
}

//========================
//オブジェクト初期化処理
//========================
void InitObjectModel(void)
{//備考：aPartsはキャラクターモデルように複数個用意してあるだけなのでオブジェクトに関しては1つしか使わないので0でいい
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	for (int nCntModel = 0; nCntModel < OBJECTTYPE_MAX; nCntModel++)
	{
		g_aObjModel[nCntModel].aParts[0] = {};
		g_aObjModel[nCntModel].aParts[0].bUse = false;

		if (SUCCEEDED(D3DXLoadMeshFromX(
			GetModelFilePath(nCntModel),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aObjModel[nCntModel].aParts[0].pBuffMat,
			NULL,
			&g_aObjModel[nCntModel].aParts[0].dwNumMatModel,
			&g_aObjModel[nCntModel].aParts[0].pMesh)))
		{//読み込み成功したら
			//テクスチャ読み込み
			D3DXMATERIAL *pMat;	//マテリアルポインタ

			//マテリアル情報に対するポインタ取得
			pMat = (D3DXMATERIAL *)g_aObjModel[nCntModel].aParts[0].pBuffMat->GetBufferPointer();

			for (int nCntTex = 0; nCntTex < (int)g_aObjModel[nCntModel].aParts[0].dwNumMatModel; nCntTex++)
			{
				if (pMat[nCntTex].pTextureFilename != NULL)
				{
					//テクスチャ読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntTex].pTextureFilename,
						&g_aObjModel[nCntModel].aParts[0].apTexture[nCntTex]);
				}
			}

			//使用している状態にする
			g_aObjModel[nCntModel].aParts[0].bUse = true;
		}
	}
}

//========================
//終了処理（ここのみ破棄（Release）すればGetModelで取得した方はNULL入れるだけでOK）
//========================
void UninitModel(void)
{
	//動物
	for (int nCntModel = 0; nCntModel < ANIMAL_MAX; nCntModel++)
	{
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			//テクスチャ破棄
			for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
			{
				if (g_aAnimalModel[nCntModel].aParts[nCntParts].apTexture[nCntTex] != NULL)
				{
					g_aAnimalModel[nCntModel].aParts[nCntParts].apTexture[nCntTex]->Release();
					g_aAnimalModel[nCntModel].aParts[nCntParts].apTexture[nCntTex] = NULL;
				}
			}

			//メッシュの破棄
			if (g_aAnimalModel[nCntModel].aParts[nCntParts].pMesh != NULL)
			{
				g_aAnimalModel[nCntModel].aParts[nCntParts].pMesh->Release();
				g_aAnimalModel[nCntModel].aParts[nCntParts].pMesh = NULL;
			}

			//マテリアルの破棄
			if (g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat != NULL)
			{
				g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat->Release();
				g_aAnimalModel[nCntModel].aParts[nCntParts].pBuffMat = NULL;
			}
		}
	}

	//オブジェクト
	//設計図
	for (int nCntObj = 0; nCntObj < OBJECTTYPE_MAX; nCntObj++)
	{
		//テクスチャ破棄
		for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
		{
			if (g_aObjModel[nCntObj].aParts[0].apTexture[nCntTex] != NULL)
			{
				g_aObjModel[nCntObj].aParts[0].apTexture[nCntTex]->Release();
				g_aObjModel[nCntObj].aParts[0].apTexture[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_aObjModel[nCntObj].aParts[0].pMesh != NULL)
		{
			g_aObjModel[nCntObj].aParts[0].pMesh->Release();
			g_aObjModel[nCntObj].aParts[0].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aObjModel[nCntObj].aParts[0].pBuffMat != NULL)
		{
			g_aObjModel[nCntObj].aParts[0].pBuffMat->Release();
			g_aObjModel[nCntObj].aParts[0].pBuffMat = NULL;
		}
	}
}

//========================
//動物取得処理
//========================
//指定した動物のモデルすべてを返します
//構造体とかに保存とかしなくても一時的に取り込んでおいて使い終わったら捨てるぐらいでいいよ
//========================
Model GetAnimal(ANIMAL animal)
{
	return g_aAnimalModel[animal];
}

//========================
//オブジェクト取得処理
//========================
//上と同じ
//========================
Model GetXObject(OBJECTTYPE objtype)
{
	return g_aObjModel[objtype];
}