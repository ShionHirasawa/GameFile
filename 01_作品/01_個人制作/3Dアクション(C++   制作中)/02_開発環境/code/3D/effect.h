//================================================================================================
//
//エフェクト処理[effect.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//エフェクトクラス
class CEffect : public CObjectBillboard
{
public:
	CEffect();
	~CEffect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	void Set(const D3DXVECTOR3 pos);
	static CEffect *Create(const D3DXVECTOR3 pos);

private:
	bool LifeManage(void);

	static int	m_nTextureID;	// テクスチャ番号
	int			m_nLife;		// 寿命
	float		m_fSize;		// ポリゴンサイズ
	D3DXCOLOR	m_col;			// カラー
};

#endif