//================================================================================================
//
//全テクスチャ管理処理[texture.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "manager.h"
#include "texture.h"

//静的メンバ変数
/*		テクスチャの総数	*/ int CTexture::m_nNumAll = 0;
/*	テクスチャのパスを保存	*/ char *CTexture::m_pTexPathStrage[MAX_TEXTURE] = {};

//=======================================
//コンストラクタ
//=======================================
CTexture::CTexture()
{
	m_nNumAll = 0;

	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		m_apTexture[nCntTex] = NULL;
		m_pTexPathStrage[nCntTex] = NULL;
	}
}

//=======================================
//デストラクタ
//=======================================
CTexture::~CTexture()
{

}

//=======================================
//テクスチャ破棄処理
//=======================================
void CTexture::UnLoad(void)
{
	//全テクスチャを破棄
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		//メモリが使われていたら、破棄する
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();//破棄する
			m_apTexture[nCntTex] = NULL;	//NULLポインタに
		}

		//メモリが使われていたら、破棄する
		if (m_pTexPathStrage[nCntTex] != NULL)
		{
			delete m_pTexPathStrage[nCntTex];
			m_pTexPathStrage[nCntTex] = NULL;
		}
	}

	//総数クリア
	m_nNumAll = 0;
}

//=======================================
//特定のテクスチャ読み込み
//=======================================
int CTexture::Regist(char *pFileName)
{
	//パスが無い
	if (pFileName == NULL) return ERROR_ID;

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ番号
	int nIdxTex = 0;

	//すでに登録されたテクスチャと同じものがないか確認
	for (nIdxTex = 0; nIdxTex < m_nNumAll; nIdxTex++)
	{
		//指定されたファイル名と既に登録されてあるファイル名が一致する
		if (strcmp(pFileName, m_pTexPathStrage[nIdxTex]) == 0)
		{
			//既に登録されているテクスチャ番号を返す
			return nIdxTex;
		}
	}

	//==============================================
	//新規テクスチャ登録
	//==============================================
	if (/*総数が最大数未満*/	m_nNumAll < MAX_TEXTURE &&
		/*アドレスが未使用*/	m_apTexture[nIdxTex] == NULL &&
		/*読み込みに成功した*/	D3DXCreateTextureFromFile(pDevice, pFileName, &m_apTexture[nIdxTex]) == D3D_OK)
	{
		//総数を増やす
		m_nNumAll++;

		//もしもメモリが使われていたら、破棄する
		if (m_pTexPathStrage[nIdxTex] != NULL)
		{
			delete m_pTexPathStrage[nIdxTex];
			m_pTexPathStrage[nIdxTex] = NULL;
		}

		//ファイルのパスの長さ + 終端文字  分のメモリを確保する
		m_pTexPathStrage[nIdxTex] = new char[(int)strlen(pFileName) + 1];

		//テクスチャのパスを保存
		strcpy(m_pTexPathStrage[nIdxTex], pFileName);

		// 新規テクスチャ番号を返す
		return nIdxTex;
	}

	//新規登録失敗でエラー番号を返す
	return ERROR_ID;
}

//=======================================
//テクスチャを取得
//=======================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(const int nIdx)
{
	//配列の範囲外か、総数より多い番号が指定された
	if (nIdx <= ERROR_ID || nIdx >= m_nNumAll)
	{
		//そんなものはないので、NULLを返す
		return NULL;
	}

	//正しく指定されたテクスチャを返す
	return m_apTexture[nIdx];
}