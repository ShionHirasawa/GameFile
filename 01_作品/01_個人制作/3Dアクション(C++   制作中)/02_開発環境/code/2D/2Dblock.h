//================================================================================================
//
//2Dブロックオブジェクト処理[2Dblock.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _2D_BLOCK_H_
#define _2D_BLOCK_H_

//2Dブロッククラス
class CBlock2D : public CObject2D
{
public:
	CBlock2D();
	~CBlock2D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);

	static CBlock2D *Create(void);
	void Set(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void CollisionPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR2 size);

private:
	static int m_nTextureID;	// テクスチャ番号
};

#endif