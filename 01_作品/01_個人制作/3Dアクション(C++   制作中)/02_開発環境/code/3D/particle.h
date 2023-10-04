//================================================================================================
//
//�p�[�e�B�N������[particle.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//�p�[�e�B�N���N���X
class CParticle : public CObjectBillboard
{
public:
	CParticle();
	~CParticle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CParticle *Create(const D3DXVECTOR3 pos, const bool bMove, const int nNumSet = 1, const int nParticleLife = 1);

private:
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ���
	int			m_nLife;//����
};

#endif