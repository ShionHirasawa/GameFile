//================================================================================================
//
//�V�����_�[����[number.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "Cylinder.h"

//=======================================
//�R���X�g���N�^
//=======================================
CCylinder::CCylinder(const int nPriority) : CObject3D(nPriority)
{
	//�����o�ϐ��N���A
	m_bDisp = true;
}

//=======================================
//�f�X�g���N�^
//=======================================
CCylinder::~CCylinder()
{

}

//=======================================
//��������
//=======================================
CCylinder *CCylinder::Create(char *pPath)
{
	//�|���S���C���X�^���X����
	CCylinder *pCylinder = new CCylinder;

	//�C���X�^���X�𐶐��ł���
	if (pCylinder != NULL)
	{
		//����������
		pCylinder->Init();

		//�e�N�X�`���ǂݍ���
		pCylinder->m_nTextureID = CManager::GetTexture()->Regist(pPath);

		//�e�N�X�`�����蓖��
		pCylinder->BindTexture(CManager::GetTexture()->GetAddress(pCylinder->m_nTextureID));
	}

	//�C���X�^���X��Ԃ�
	return pCylinder;
}

//=======================================
//����������
//=======================================
HRESULT CCylinder::Init(void)
{
	//���_���W�̏�����
	if (FAILED(CObject3D::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_POLY_3D);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CCylinder::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObject3D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CCylinder::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CCylinder::Draw(void)
{
	if (!m_bDisp) return;

	//���_�o�b�t�@�̔j���Ȃ�
	CObject3D::Draw();
}

//=======================================
//�|���S�����̐ݒ菈��
//=======================================
void CCylinder::SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, D3DXVECTOR2 fTexPos, float TexWidth, float TexHeight)
{
	//�ʒu�ݒ�
	SetPosition(pos);

	//���_���W�E�e�N�X�`�����W�̐ݒ�
	SetVertexInfo(fWidth, fHeight, fDepth, fTexPos, TexWidth, TexHeight);
}