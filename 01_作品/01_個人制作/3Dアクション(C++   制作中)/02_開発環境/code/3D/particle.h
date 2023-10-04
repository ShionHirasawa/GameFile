//================================================================================================
//
//パーティクル処理[particle.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//パーティクルクラス
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
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	int			m_nLife;//寿命
};

#endif