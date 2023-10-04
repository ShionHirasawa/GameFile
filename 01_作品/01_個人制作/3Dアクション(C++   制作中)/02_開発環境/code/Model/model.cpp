//================================================================================================
//
//���f���Ǘ�����[model.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "model.h"

//=======================================
//�R���X�g���N�^
//=======================================
CModel::CModel()
{
	//�����o�ϐ��N���A
	m_posOffSet = m_pos = VEC3_INIT;
	m_rotOffSet = m_rot = VEC3_INIT;
	m_pParent = NULL;
}

//=======================================
//�f�X�g���N�^
//=======================================
CModel::~CModel()
{

}

//=======================================
//��������
//=======================================
CModel *CModel::Create(const char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//���f���C���X�^���X�̐���
	CModel *pModel = new CModel;

	//�C���X�^���X���������ꂽ
	if (pModel != NULL)
	{
		//����������
		if (FAILED(pModel->Init(pFilePath)))
		{
			//���s������
			delete pModel;	//�������폜
			pModel = NULL;	//�|�C���^��NULL��
			return NULL;	//NULL��Ԃ�
		}

		//�ʒu�E������ݒ�i���̊֐��ɓn���ꂽ�����̒l�����f���̊�l�Ȃ̂ŁA�����l��n��
		pModel->InitSetPosRot(pos, rot);
	}

	//���f����Ԃ�
	return pModel;
}

//=======================================
//����������
//=======================================
HRESULT CModel::Init(const char *pFilePath)
{
	//X�t�@�C���ǂݍ���
	if (FAILED(LoadModelParts(pFilePath)))
	{
		//���������s
		return E_FAIL;
	}

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CModel::Uninit(void)
{
	//���b�V���̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`��ID���g���Ă���
	if (m_pTextureID != NULL)
	{
		delete[] m_pTextureID;
		m_pTextureID = NULL;
	}
}

//=======================================
//�X�V����
//=======================================
void CModel::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CModel::Draw(void)
{
	/*  �f�o�C�X���擾  */	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	/* �e�̃}�g���b�N�X */	D3DXMATRIX mtxParent;
	/*�v�Z�p�}�g���b�N�X*/	D3DXMATRIX mtxRot, mtxTrans;
	/*�}�e���A���|�C���^*/	D3DXMATERIAL *pMat;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�e�����݂���
	if (m_pParent != NULL)
	{
		//�e�̃}�g���b�N�X���擾
		mtxParent = m_pParent->GetMtxWorld();
	}
	//�e�����݂��Ȃ�
	else
	{	
		//���݁i�ŐV�j�̃}�g���b�N�X���擾����[ = �v���C���[�̃}�g���b�N�X]
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�����Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(m_pTextureID[nCntMat]));

		//���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}
}

//=======================================
//�e�̐ݒ�
//=======================================
void CModel::SetParent(CModel *pModel)
{
	//�e���f������
	m_pParent = pModel;
}

//=======================================
//���[���h�}�g���b�N�X���擾
//=======================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	//���[���h�}�g���b�N�X��Ԃ�
	return m_mtxWorld;
}

//=======================================
//���f���p�[�c�ǂݍ���
//=======================================
HRESULT CModel::LoadModelParts(const char *pFilePath)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�]�t�@�C�����ǂݍ��߂Ȃ�����
	if (D3DXLoadMeshFromX(pFilePath, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_dwNumMat, &m_pMesh) != D3D_OK)
	{
		//���������s
		return E_FAIL;
	}

	//�}�e���A���̐������|�C���^���m��
	m_pTextureID = new int[m_dwNumMat];

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//�G���[�ԍ��������l�Ƃ��đ��
		m_pTextureID[nCntMat] = ERROR_ID;

		//�e�N�X�`���t�@�C�������݂���
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//�e�N�X�`���ԍ��i�[ <- �e�N�X�`���̓ǂݍ���
			m_pTextureID[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
		}
	}

	//����������
	return S_OK;
}

//=======================================
//�ʒu�E�p�x�i��l���܂ށj�̏����ݒ�
//=======================================
void CModel::InitSetPosRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{	
	m_posOffSet = m_pos = pos;//�ʒu�ݒ�
	m_rotOffSet = m_rot = rot;//�p�x�ݒ�
}