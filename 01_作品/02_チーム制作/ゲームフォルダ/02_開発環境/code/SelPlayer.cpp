//==========================================
//
//プレイヤー選択プログラム[SelPlayer.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "SelPlayer.h"
#include "conversioninput.h"
#include "color.h"
#include "file.h"

#include "TypeFrame.h"
#include "SelPlayer_bg.h"
#include "SelPlayer_camera.h"
#include "model.h"

//グローバル
D3DXMATRIX g_mtxWorldSelPlayer[MAX_USE_GAMEPAD];
D3DXMATRIX g_mtxWorldModelSelPlayer[MAX_USE_GAMEPAD][MAX_PARTS];

//プレイヤー位置向き
const D3DXVECTOR3 c_aPosRotSelPlayer[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-75.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(-25.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(25.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
	{ D3DXVECTOR3(75.0f,0.0f,0.0f) ,D3DXVECTOR3(0.0f,0.0f,0.0f) },
};

//========================
//初期化処理
//========================
void InitSelPlayer(void)
{
	InitFile();				//ファイル初期化
	LoadModelViewerFile("data\\model.txt");	// モデルビューワーファイル読み込み（各オブジェクト初期化前に行うこと！）
	InitAnimalModel();		//動物モデル初期化

	InitTypeFrame();
	InitSelPlayer_Bg();
	InitSelPlayer_Camera();
	InitSelPlayer_SetCameraPos(D3DXVECTOR3(0.0f, 50.0f, 150.0f), D3DXVECTOR3(0.0f, 20.0f, 0.0f));
}

//========================
//終了処理
//========================
void UninitSelPlayer(void)
{
	UninitSelPlayer_Camera();
	UninitSelPlayer_Bg();
	UninitTypeFrame();

	UninitModel();				//動物モデル終了処理
}

//========================
//更新処理
//========================
void UpdateSelPlayer(void)
{
	UpdateTypeFrame();
	UpdateSelPlayer_Bg();
	UpdateSelPlayer_Camera();
}

//========================
//描画処理
//========================
void DrawSelPlayer(void)
{
	//背景
	DrawSelPlayer_Bg();

	//タイプ枠
	DrawTypeFrame();

	//カメラ設定
	SetSelPlayer_Camera();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//モデル描画
	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//プレイヤーの数だけ繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (GetUsePlayer(nCntPlayer) == true)
		{
			Model useAnimal = GetAnimal(ANIMAL_WILDBOAR);

			//"プレイヤーの"ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_mtxWorldSelPlayer[nCntPlayer]);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, c_aPosRotSelPlayer[nCntPlayer][1].y, c_aPosRotSelPlayer[nCntPlayer][1].x, c_aPosRotSelPlayer[nCntPlayer][1].z);
			D3DXMatrixMultiply(&g_mtxWorldSelPlayer[nCntPlayer], &g_mtxWorldSelPlayer[nCntPlayer], &mtxRot);

			//位置反映
			D3DXMatrixTranslation(&mtxTrans, c_aPosRotSelPlayer[nCntPlayer][0].x, c_aPosRotSelPlayer[nCntPlayer][0].y, c_aPosRotSelPlayer[nCntPlayer][0].z);
			D3DXMatrixMultiply(&g_mtxWorldSelPlayer[nCntPlayer], &g_mtxWorldSelPlayer[nCntPlayer], &mtxTrans);

			//"プレイヤーの"ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSelPlayer[nCntPlayer]);

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				if (useAnimal.aParts[nCntParts].bUse == true)
				{
					D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
					D3DXMATRIX mtxParent;					//親のマトリ

					//"モデルの"ワールドマトリックス初期化
					D3DXMatrixIdentity(&g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts]);

					//向きを反映
					D3DXMatrixRotationYawPitchRoll(&mtxRotModel, useAnimal.aParts[nCntParts].rotOffset.y, useAnimal.aParts[nCntParts].rotOffset.x, useAnimal.aParts[nCntParts].rotOffset.z);
					D3DXMatrixMultiply(&g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &mtxRotModel);

					//位置反映
					D3DXMatrixTranslation(&mtxTransModel, useAnimal.aParts[nCntParts].posOffset.x, useAnimal.aParts[nCntParts].posOffset.y, useAnimal.aParts[nCntParts].posOffset.z);
					D3DXMatrixMultiply(&g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &mtxTransModel);

					//パーツの親マトリ設定
					if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
					{
						mtxParent = g_mtxWorldModelSelPlayer[nCntPlayer][useAnimal.aParts[nCntParts].mIdxModelParent];
					}
					else
					{
						mtxParent = g_mtxWorldSelPlayer[nCntPlayer];
					}

					//パーツのマトリと親マトリをかけ合わせる
					D3DXMatrixMultiply(&g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts], &mtxParent);

					//"モデルの"ワールドマトリックス設定
					pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModelSelPlayer[nCntPlayer][nCntParts]);

					//マテリアルデータへのポインタ取得
					pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
					{
						//ゴースト用
						D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

						if (nCntParts == MAX_PARTS - 1)
						{//ゼッケンの時は色変更
							matChange.Diffuse = c_aColPlayer[nCntPlayer];
						}

						//マテリアル設定
						pDevice->SetMaterial(&matChange);

						//テクスチャ設定
						pDevice->SetTexture(0, useAnimal.aParts[nCntParts].apTexture[nCntMat]);

						//モデル描画
						useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
					}
				}
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}