//================================================================================================
//
//3D�|���S������[Polygon3D.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "Polygon3D.h"

//=======================================
//�R���X�g���N�^
//=======================================
CPoly3D::CPoly3D(const int nPriority) : CObject3D(nPriority)
{
	//�����o�ϐ��N���A
	m_bDisp = true;
	m_bAlphaTest = false;	//���e�X�g���s�����ǂ���
	m_bAddBlend = false;	//���Z�������s�����ǂ���
	m_bSubBlend = false;	//���Z�������s�����ǂ���
}

//=======================================
//�f�X�g���N�^
//=======================================
CPoly3D::~CPoly3D()
{

}

//=======================================
//��������
//=======================================
CPoly3D *CPoly3D::Create(char *pPath)
{
	//�|���S���C���X�^���X����
	CPoly3D *pPoly3D = new CPoly3D;

	//�C���X�^���X�𐶐��ł���
	if (pPoly3D != NULL)
	{
		//����������
		pPoly3D->Init();

		//�e�N�X�`���ǂݍ���
		pPoly3D->m_nTextureID = CManager::GetTexture()->Regist(pPath);

		//�e�N�X�`�����蓖��
		pPoly3D->BindTexture(CManager::GetTexture()->GetAddress(pPoly3D->m_nTextureID));
	}

	//�C���X�^���X��Ԃ�
	return pPoly3D;
}

//=======================================
//����������
//=======================================
HRESULT CPoly3D::Init(void)
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
void CPoly3D::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObject3D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CPoly3D::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CPoly3D::Draw(void)
{
	if (!m_bDisp) return;

	//�����_�[�ݒ�
	SetDrawRender(true);

	//���_�o�b�t�@�̔j���Ȃ�
	CObject3D::Draw();

	//�����_�[�ݒ����
	SetDrawRender(false);
}

//=======================================
//�����_���[�̐ݒ���s��
//=======================================
void CPoly3D::SetDrawRender(bool bSet)
{
	//�����_���[�擾
	CRenderer *pRender = CManager::GetRenderer();

	//�����_���[�ݒ�
	if (bSet)
	{
		//���Z�������s��
		if (m_bAddBlend)	pRender->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_SET);

		//���Z�������s��
		else if (m_bSubBlend)pRender->SetAlphaBlend(CRenderer::TYPE_SUB, CRenderer::VALUE_SET);

		//���e�X�g�L����
		if (m_bAlphaTest) pRender->SetAlphaTest(true, 0);
	}
	//�ݒ����
	else
	{
		//���e�X�g������
		if (m_bAlphaTest) pRender->SetAlphaTest(false, 0);

		//���Z����������
		if (m_bAddBlend)	pRender->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_RESET);

		//���Z����������
		else if (m_bSubBlend)	pRender->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_RESET);
	}
}

//=======================================
//�|���S�����̐ݒ菈��
//=======================================
void CPoly3D::SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, D3DXVECTOR2 fTexPos, float TexWidth, float TexHeight)
{
	//�ʒu�ݒ�
	SetPosition(pos);

	//���_���W�E�e�N�X�`�����W�̐ݒ�
	SetVertexInfo(fWidth, fHeight, fDepth, fTexPos, TexWidth, TexHeight);
}