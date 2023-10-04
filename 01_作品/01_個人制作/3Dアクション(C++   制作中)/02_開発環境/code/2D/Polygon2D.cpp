//================================================================================================
//
//�|���S���֘A�̐ꑮ����[number.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "Polygon2D.h"

//=======================================
//�R���X�g���N�^
//=======================================
CPoly2D::CPoly2D(const int nPriority) : CObject2D(nPriority)
{
	//�����o�ϐ��N���A
	m_bDisp = true;
}

//=======================================
//�f�X�g���N�^
//=======================================
CPoly2D::~CPoly2D()
{

}

//=======================================
//��������
//=======================================
CPoly2D *CPoly2D::Create(char *pPath)
{
	//�|���S���C���X�^���X����
	CPoly2D *pPoly2D = new CPoly2D;

	//�C���X�^���X�𐶐��ł���
	if (pPoly2D != NULL)
	{
		//����������
		pPoly2D->Init();

		//�e�N�X�`���ݒ�
		pPoly2D->SetTexture(pPath);
	}

	//�C���X�^���X��Ԃ�
	return pPoly2D;
}

//=======================================
//����������
//=======================================
HRESULT CPoly2D::Init(void)
{
	//���_���W�̏�����
	if (FAILED(CObject2D::Init(VEC3_INIT, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_POLY_2D);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CPoly2D::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObject2D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CPoly2D::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CPoly2D::Draw(void)
{
	if (!m_bDisp) return;

	//���e�X�gON
	CManager::GetRenderer()->SetAlphaTest(true, 0);

	//���_�o�b�t�@�̔j���Ȃ�
	CObject2D::Draw();

	//���e�X�gOFF
	CManager::GetRenderer()->SetAlphaTest(false, 255);
}

//=======================================
//�|���S�����̐ݒ菈��
//=======================================
void CPoly2D::SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, D3DXVECTOR2 fTexPos, float TexWidth, float TexHeight)
{
	//�ʒu�ݒ�
	SetPosition(pos);

	//���_���W�E�e�N�X�`�����W�̐ݒ�
	SetVertexInfo(fWidth, fHeight, fTexPos, TexWidth, TexHeight);
}

//=======================================
//�e�N�X�`���ݒ菈��
//=======================================
void CPoly2D::SetTexture(char *pPath)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(pPath);

	//�e�N�X�`�����蓖��
	BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
}