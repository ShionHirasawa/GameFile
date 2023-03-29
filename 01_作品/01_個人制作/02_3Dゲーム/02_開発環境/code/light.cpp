/*==========================================================================================================================================================

															ライド処理[light.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "light.h"
#include "color.h"

//マクロ定義
#define NUM_LIGHT		(3)		//ライトの数
#define LIGHT_INFOFILE	"data/CSV/light.csv"

//グローバル変数
D3DXVECTOR3	g_LightVec[NUM_LIGHT];	//ライトのベクトル
D3DLIGHT9	g_light[NUM_LIGHT];		//ライトの情報

//------------------------------------------------
//				ライトの初期化処理
//------------------------------------------------
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXVECTOR3 vecDir[NUM_LIGHT];				//設定用方向ベクトル

	//ライトのベクトル読み込み
	LoadLightVec();

	//ライトの情報をクリアする
	for (int nCntLight = START_CNT; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの方向代入
		vecDir[nCntLight] = g_LightVec[nCntLight];

		//メモリー初期化
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		g_light[nCntLight].Diffuse = XCOL_WHITE;

		//ライトの方向を設定
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		//ベクトルを正規化する
		g_light[nCntLight].Direction = vecDir[nCntLight];

		//ライトを設定
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//ライトの向き読み込み
void LoadLightVec(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(LIGHT_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);		//最初のセルタイトル読み込み

		for (int nCntLight = START_CNT; nCntLight < NUM_LIGHT; nCntLight++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			g_LightVec[nCntLight].x = atof(strtok(&aData[ZERO], ","));		// ベクトル初期化
			g_LightVec[nCntLight].y = atof(strtok(NULL, ","));				// ベクトル初期化
			g_LightVec[nCntLight].z = atof(strtok(NULL, ","));				// ベクトル初期化
		}

		//ファイルを閉じる
		fclose(pFile);
	}

}

//------------------------------------------------
//				ライトの終了処理
//------------------------------------------------
void UninitLight(void)
{

}

//------------------------------------------------
//				ライトの更新処理
//------------------------------------------------
void UpdateLight(void)
{

}