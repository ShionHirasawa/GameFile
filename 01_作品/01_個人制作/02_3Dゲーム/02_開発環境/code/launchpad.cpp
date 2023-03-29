/*==========================================================================================================================================================

発射台の処理[launchpad.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "launchpad.h"
#include "logthorns.h"
#include "player.h"
#include "meshfield.h"
#include "sound.h"
#include "slope.h"

#define LAUNCHPAD_TEX			(10)						// 発射台のテクスチャの最大数
#define LAUNCHPAD_NUM			(4)							// 発射台の総数
#define LAUNCHPAD_XFILE			"data/MODEL/LaunchPad.x"	// 発射台のファイル名
#define LAUNCHPAD_INFOFILE		"data/CSV/launchpad.csv"	// 発射台の情報ファイル名
#define LAUNCHPAD_WAIT_TIME		(300)						// 発射台のチャージ時間

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureLaunchPad[LAUNCHPAD_TEX] = {};// テクスチャへのポインタ
LPD3DXMESH				g_pMeshLaunchPad;						// メッシュ(頂点情報)へのポインタ
D3DXMATRIX				g_mtxWorldLaunchPad;					// ワールドマトリックス
LPD3DXBUFFER			g_pBuffMatLaunchPad;					// マテリアルへのポインタ
DWORD					g_dwNumMatLaunchPad;					// マテリアルの数
LaunchPad				g_LaunchPad[LAUNCHPAD_TEX];				// 発射台の情報

//----------------------------------------------------
//				発射台の初期化処理
//----------------------------------------------------
void InitLaunchPad(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//情報初期化
	LoadLaunchPad();

	g_pMeshLaunchPad = NULL;		// メッシュ初期化
	g_pBuffMatLaunchPad = NULL;		// マテリアルへのポインタ初期化
	g_dwNumMatLaunchPad = NULL;		// マテリアルの数初期化

									// Xファイル読み込み
	D3DXLoadMeshFromX(LAUNCHPAD_XFILE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatLaunchPad, NULL, &g_dwNumMatLaunchPad, &g_pMeshLaunchPad);

	//発射台のテクスチャ設定
	for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatLaunchPad; nCntMat++)
	{
		//マテリアルデータへのポインタを取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatLaunchPad->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureLaunchPad[nCntMat]);
		}
	}
}

//情報読み込み処理
void LoadLaunchPad(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(LAUNCHPAD_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルタイトル読み込み

		for (int nCntLaunchPad = START_CNT; nCntLaunchPad < LAUNCHPAD_NUM; nCntLaunchPad++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			//****************
			//位置初期化
			//****************
			g_LaunchPad[nCntLaunchPad].pos.x = atof(strtok(&aData[ZERO], ","));
			g_LaunchPad[nCntLaunchPad].pos.y = atof(strtok(NULL, ","));
			g_LaunchPad[nCntLaunchPad].pos.z = atof(strtok(NULL, ","));

			//****************
			//向き初期化
			//****************
			g_LaunchPad[nCntLaunchPad].rot.x = NIL_F;
			g_LaunchPad[nCntLaunchPad].rot.z = NIL_F;
			g_LaunchPad[nCntLaunchPad].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;
		
			//----------------
			//状態関連初期化
			//----------------
			g_LaunchPad[nCntLaunchPad].state = LAUNCHPADSTATE_WAIT;
			g_LaunchPad[nCntLaunchPad].nCntWait = atoi(strtok(NULL, ","));
			g_LaunchPad[nCntLaunchPad].nCntDelay = atoi(strtok(NULL, ","));

			//****************
			//設定する移動量初期化
			//****************
			float fMove = atof(strtok(NULL, ","));

			g_LaunchPad[nCntLaunchPad].Setmove = D3DXVECTOR3(
				sinf(D3DX_PI - g_LaunchPad[nCntLaunchPad].rot.y) * fMove,		// 移動量初期化
				atof(strtok(NULL, ",")),										// 発射時の上昇量初期化
				cosf(D3DX_PI - g_LaunchPad[nCntLaunchPad].rot.y) * fMove);		// 移動量初期化
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//----------------------------------------------
//				発射台の終了処理
//----------------------------------------------
void UninitLaunchPad(void)
{
	//メッシュ破棄
	if (g_pMeshLaunchPad != NULL)
	{
		g_pMeshLaunchPad->Release();
		g_pMeshLaunchPad = NULL;
	}

	//マテリアル破棄
	if (g_pBuffMatLaunchPad != NULL)
	{
		g_pBuffMatLaunchPad->Release();
		g_pBuffMatLaunchPad = NULL;
	}

	//テクスチャ破棄
	for (int nCntTex = START_CNT; nCntTex < LAUNCHPAD_TEX; nCntTex++)
	{
		if (g_pTextureLaunchPad[nCntTex] != NULL)
		{
			g_pTextureLaunchPad[nCntTex]->Release();
			g_pTextureLaunchPad[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------
//				発射台の更新処理
//----------------------------------------------
void UpdateLaunchPad(void)
{
	//更新処理
	for (int nCntLaunchPad = START_CNT; nCntLaunchPad < LAUNCHPAD_NUM; nCntLaunchPad++)
	{
		switch (g_LaunchPad[nCntLaunchPad].state)
		{
		//----------------------
		//		待機状態
		//----------------------
		case LAUNCHPADSTATE_WAIT:	
			LaunchPad_Wait(nCntLaunchPad);
			break;

		//----------------------
		//		発射状態
		//----------------------
		case LAUNCHPADSTATE_FIRE:
			LaunchPad_Fire(nCntLaunchPad);
			break;
		}
	}
}

//待機処理
void LaunchPad_Wait(int nCntLaunchPad)
{
	//遅延させる
	if (g_LaunchPad[nCntLaunchPad].nCntDelay > 0)
	{
		//遅延時間減少
		g_LaunchPad[nCntLaunchPad].nCntDelay--;
	}

	//遅延時間が無くなった
	else
	{
		//待機時間減少
		g_LaunchPad[nCntLaunchPad].nCntWait--;

		//待機時間が０になった
		if (g_LaunchPad[nCntLaunchPad].nCntWait <= 0)
		{
			g_LaunchPad[nCntLaunchPad].state = LAUNCHPADSTATE_FIRE;	//発射状態にする
		}
	}
}

//発射処理
void LaunchPad_Fire(int nCntLaunchPad)
{
	//発射する
	SetLogThorns(g_LaunchPad[nCntLaunchPad].pos, g_LaunchPad[nCntLaunchPad].Setmove, g_LaunchPad[nCntLaunchPad].rot);

	//待機状態に
	g_LaunchPad[nCntLaunchPad].state = LAUNCHPADSTATE_WAIT;		//待機状態にする
	g_LaunchPad[nCntLaunchPad].nCntWait = LAUNCHPAD_WAIT_TIME;	//待機時間設定
}

//----------------------------------------------
//				発射台の描画処理
//----------------------------------------------
void DrawLaunchPad(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntLaunchPad = START_CNT; nCntLaunchPad < LAUNCHPAD_NUM; nCntLaunchPad++)
	{
		//使われている
			//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldLaunchPad);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_LaunchPad[nCntLaunchPad].rot.y, g_LaunchPad[nCntLaunchPad].rot.x, g_LaunchPad[nCntLaunchPad].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldLaunchPad, &g_mtxWorldLaunchPad, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_LaunchPad[nCntLaunchPad].pos.x, g_LaunchPad[nCntLaunchPad].pos.y, g_LaunchPad[nCntLaunchPad].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldLaunchPad, &g_mtxWorldLaunchPad, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldLaunchPad);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)g_pBuffMatLaunchPad->GetBufferPointer();

		//マテリアル設定
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatLaunchPad; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureLaunchPad[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshLaunchPad->DrawSubset(nCntMat);
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}