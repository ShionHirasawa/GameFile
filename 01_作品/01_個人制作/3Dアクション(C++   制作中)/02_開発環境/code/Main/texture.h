//================================================================================================
//
//全テクスチャ管理処理[texture.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//テクスチャの最大数
#define MAX_TEXTURE	(64)

//テクスチャクラス
class CTexture
{
public:
	CTexture();
	~CTexture();

	/* テクスチャを破棄する */	void UnLoad(void);

	/* 特定のテクスチャ登録 */	int Regist(char *pFileName);
	/* テクスチャを取得する */	LPDIRECT3DTEXTURE9 GetAddress(const int nIdx);

private:
	/*		テクスチャを保存	*/ LPDIRECT3DTEXTURE9	m_apTexture[MAX_TEXTURE];
	/*		テクスチャの総数	*/ static int			m_nNumAll;
	/*	テクスチャのパスを保存	*/ static char			*m_pTexPathStrage[MAX_TEXTURE];
};

#endif