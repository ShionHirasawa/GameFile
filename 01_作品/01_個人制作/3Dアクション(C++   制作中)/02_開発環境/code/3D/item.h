//================================================================================================
//
//アイテム処理[item.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//アイテムクラス
class CItem : public CObjectBillboard
{
public:
	CItem();
	~CItem();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static CItem *Create(const D3DXVECTOR3 pos);

	bool CollisionPlayer(const D3DXVECTOR3 pos);
private:
	static int	m_nTextureID;	// テクスチャ番号
};

#endif