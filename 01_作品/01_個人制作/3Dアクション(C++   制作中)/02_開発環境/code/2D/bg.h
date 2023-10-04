//================================================================================================
//
//背景処理[bg.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _BG_H_
#define _BG_H_

//背景クラス
class CBg : public CObject2D
{
public:
	CBg();
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(char *pPath);

	static CBg *Create(void);

private:
	static int m_nTextureID;	// テクスチャ番号
};

#endif