//================================================================================================
//
//ライド処理[light.cpp]
//Author:平澤詩苑
//
//================================================================================================
#include "../Main/manager.h"
#include "object3D.h"
#include "light.h"

//マクロ定義
//ライトの設定情報ファイル
#define LIGHT_INFOFILE	"data\\CSV\\light.csv"

//=======================================
//コンストラクタ
//=======================================
CLight::CLight()
{
	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		m_LightVec[nCntLight] = VEC3_INIT;
	}
}

//=======================================
//デストラクタ
//=======================================
CLight::~CLight()
{

}

//=======================================
//ライトの初期化処理
//=======================================
HRESULT CLight::Init(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ライトのベクトル読み込み
	LoadVec();

	//ライトの情報をクリアする
	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//メモリー初期化
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		m_light[nCntLight].Diffuse = GetColor(COL_WHITE);

		//ライトの方向を設定
		D3DXVec3Normalize(&m_LightVec[nCntLight], &m_LightVec[nCntLight]);		//ベクトルを正規化する
		m_light[nCntLight].Direction = m_LightVec[nCntLight];

		//ライトを設定
		if (FAILED(pDevice->SetLight(nCntLight, &m_light[nCntLight])))
		{
			//設定失敗
			return E_FAIL;
		}

		//ライトを有効にする
		if (FAILED(pDevice->LightEnable(nCntLight, TRUE)))
		{
			//失敗
			return E_FAIL;
		}
	}

	//初期化成功
	return S_OK;
}

//=======================================
//ライトの向き読み込み
//=======================================
void CLight::LoadVec(void)
{
	FILE *pFile;
	char aData[MAX_STR];		//読み込んだ文字を格納

	pFile = fopen(LIGHT_INFOFILE, "r");

	//ファイルが開けた
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);		//１行目に書かれた、各列のタイトルを読み取り

		for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
		{
			fscanf(pFile, "%s", &aData[0]);	//一行読み込み

			m_LightVec[nCntLight].x = (float)atof(strtok(&aData[0], CSV_DELIMITER));// ベクトル読み込み
			m_LightVec[nCntLight].y = (float)atof(strtok(NULL, CSV_DELIMITER));		// ベクトル読み込み
			m_LightVec[nCntLight].z = (float)atof(strtok(NULL, CSV_DELIMITER));		// ベクトル読み込み
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//=======================================
//ライトの終了処理
//=======================================
void CLight::Uninit(void)
{

}

//=======================================
//ライトの更新処理
//=======================================
void CLight::Update(void)
{

}