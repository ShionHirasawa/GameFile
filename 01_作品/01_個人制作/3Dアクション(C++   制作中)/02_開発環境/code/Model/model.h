//================================================================================================
//
//���f���Ǘ�����[model.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//���f���̑���
#define MAX_PARTS		(20)

//���f���N���X
class CModel
{
public:
	CModel();
	~CModel();

	HRESULT Init(const char *pFilePath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModel *Create(const char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetParent(CModel *pModel);
	D3DXMATRIX GetMtxWorld(void);

	HRESULT LoadModelParts(const char *pFilePath);

	void OffSetTranslate(D3DXVECTOR3 Setpos, D3DXVECTOR3 SetRot) { m_pos = m_posOffSet + Setpos; m_rot = m_rotOffSet + SetRot; }

private:
	void InitSetPosRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	LPD3DXMESH		m_pMesh;		//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER	m_pBuffMat;		//�}�e���A���̃|�C���^
	DWORD			m_dwNumMat;		//�}�e���A���̐�
	int				*m_pTextureID;	//�e�N�X�`��ID�̃|�C���^

	D3DXVECTOR3 m_pos, m_posOffSet;	//���ݒn�A��ʒu
	D3DXVECTOR3 m_rot, m_rotOffSet;	//���݊p�x�A��p�x
	D3DXMATRIX	m_mtxWorld;
	CModel *m_pParent;
};

#endif