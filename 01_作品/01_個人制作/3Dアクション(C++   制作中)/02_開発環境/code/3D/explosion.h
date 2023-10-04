//================================================================================================
//
//��������[explosion.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//�����̍ő吔
#define MAX_EXPLOSION	(20)

//�����N���X
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

	static int	m_nTextureID;	// �e�N�X�`���ԍ�
	int			m_nAnimCounter;	// �J�E���^�[
	int			m_nAnimPattern;	// �p�^�[��
};

#endif