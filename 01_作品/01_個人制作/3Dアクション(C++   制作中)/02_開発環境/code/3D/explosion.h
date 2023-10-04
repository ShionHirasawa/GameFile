//================================================================================================
//
//爆発処理[explosion.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//爆発の最大数
#define MAX_EXPLOSION	(20)

//爆発クラス
class CExplosion : public CObjectBillboard
{
public:
	CExplosion();
	~CExplosion();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	static CExplosion *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move = VEC3_INIT, bool bSound = true);

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move = VEC3_INIT);

private:
	bool Animation(void);

	static int	m_nTextureID;	// テクスチャ番号
	int			m_nAnimCounter;	// カウンター
	int			m_nAnimPattern;	// パターン
};

#endif