//================================================================================================
//
//ビルボードの木ポリゴン処理[treeBillboard.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TREE_BILLBOARD_
#define _TREE_BILLBOARD_

//ビルボードの木クラス
class CTree : public CObjectBillboard
{
public:
	CTree();
	~CTree();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);

	static CTree *Create(void);
	void Set(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth);

private:
	static int	m_nTextureID;	// テクスチャ番号
};

#endif