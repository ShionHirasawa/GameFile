//--------------------------------------------------------------------------------------------------------
//
//ライト処理[light.cpp]
//Author:菊地陽人
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "light.h"
#include "input.h"
#include "color.h"

//マクロ定義
#define	NUM_LIGHT		(4)		//ライトの数

//グローバル変数
D3DLIGHT9 g_aLight[NUM_LIGHT];

////ライトの向き格納
//const D3DXVECTOR3 c_apLightVec[NUM_LIGHT] = {
//	D3DXVECTOR3(0.2f, -0.8f, 0.4f),
//	D3DXVECTOR3(-0.2f, 0.8f, -0.4f),
//	D3DXVECTOR3(0.8f, -0.1f, 0.4f),
//};

//--------------------------------------------------------------------------------------------------------
//ライトの初期化処理
//--------------------------------------------------------------------------------------------------------
void InitLight(void)
{
#if 0
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXVECTOR3 aVecDir[NUM_LIGHT];		//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの方向を設定
		aVecDir[nCntLight] = c_apLightVec[nCntLight];

		//ライトの情報をクリアする
		ZeroMemory(&g_aLight[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		g_aLight[nCntLight].Diffuse = XCOL_WHITE;

		D3DXVec3Normalize(&aVecDir[nCntLight], &aVecDir[nCntLight]);		//ベクトルを正規化する
		g_aLight[nCntLight].Direction = aVecDir[nCntLight];

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
#endif
}

//--------------------------------------------------------------------------------------------------------
//ライトの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitLight(void)
{

}

//--------------------------------------------------------------------------------------------------------
//ライトの更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateLight(void)
{

}

//--------------------------------------------------------------------------------------------------------
//ライトの設定処理
//Author:石原颯馬
//--------------------------------------------------------------------------------------------------------
void SetLight(int nLightNum, D3DLIGHT9 light)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ライトの情報をクリアする
	ZeroMemory(&g_aLight[nLightNum], sizeof(D3DLIGHT9));

	//引数のライト情報を代入
	g_aLight[nLightNum] = light;

	//ライトの種類を設定
	g_aLight[nLightNum].Type = D3DLIGHT_DIRECTIONAL;

	//ライトを設定する
	pDevice->SetLight(nLightNum, &g_aLight[nLightNum]);

	//ライトを有効にする
	pDevice->LightEnable(nLightNum, TRUE);
}