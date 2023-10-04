//================================================================================================
//
//�e����[bullet.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//�e�N���X
class CBullet : public CObjectBillboard
{
public:
	CBullet();
	~CBullet();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	static HRESULT Load(void);
	static CBullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

	bool CollisionEnemy(const D3DXVECTOR3 pos);
	bool CollisionBlock(const D3DXVECTOR3 pos);
private:
	bool LifeManage(void);

	static int	m_nTextureID;	// �e�N�X�`���ԍ�
	int			m_nLife;		// ����
};

#endif