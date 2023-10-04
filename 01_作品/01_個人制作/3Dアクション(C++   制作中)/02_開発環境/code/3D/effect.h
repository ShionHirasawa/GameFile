//================================================================================================
//
//�G�t�F�N�g����[effect.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//�G�t�F�N�g�N���X
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

	static int	m_nTextureID;	// �e�N�X�`���ԍ�
	int			m_nLife;		// ����
	float		m_fSize;		// �|���S���T�C�Y
	D3DXCOLOR	m_col;			// �J���[
};

#endif