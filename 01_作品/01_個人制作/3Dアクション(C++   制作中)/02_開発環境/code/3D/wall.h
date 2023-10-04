//================================================================================================
//
//�Ǐ���[wall.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _WALL_H
#define _WALL_H

//�ǃN���X
class CWall : public CObject3D
{
public:
	CWall();
	~CWall();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT Load(void);

	static CWall *Create(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const D3DXVECTOR2 texSize);
	static bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, float fMargin, float fHeight);

	//==========================================
	//�Ώۂ܂ł̊Ԃɕǂ����邩�ǂ�������
	//�����P�@�n�_�ʒu
	//�����Q�@�I�_�ʒu
	//�Ԃ�l�F�����ƖڕW�̊Ԃɕǂ����邩�ǂ����@true�F�ǂ�����@false�F�ǂ͂Ȃ�
	//==========================================
	static bool IsAmongWall(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);

private:
	int	m_nTextureID;	// �e�N�X�`���ԍ�
};

#endif