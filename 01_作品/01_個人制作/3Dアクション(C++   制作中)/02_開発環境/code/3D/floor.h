//================================================================================================
//
//������[floor.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _FLOOR_H
#define _FLOOR_H

//���N���X
class CFloor : public CObject3D
{
public:
	CFloor();
	~CFloor();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT Load(char *pPath);

	static CFloor *Create(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const D3DXVECTOR2 texSize);

	static bool LandField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

private:
	static int	m_nTextureID;	// �e�N�X�`���ԍ�
	float m_fLandHeight;	//���݂̒��n�_
	float m_fLandHeightOld;	//�O��̒��n�_
	float m_fAddHeight;		//���_�P�E�Q���グ��������ϐ�
	float m_fAddHeightOld;	//�O��̏グ��������ϐ��̒l
};

#endif