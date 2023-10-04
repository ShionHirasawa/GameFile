//================================================================================================
//
//�w�i����[bg.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "object2D.h"
#include "bg.h"

/***	�w�i�̏����D�揇��	***/
#define PRIORITY_BG		(0)

//�ÓI�����o�ϐ��錾
//�e�N�X�`��
int		CBg::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CBg::CBg() : CObject2D(PRIORITY_BG)
{
	//�����o�ϐ��N���A
}

//=======================================
//�f�X�g���N�^
//=======================================
CBg::~CBg()
{

}

//=======================================
//��������
//=======================================
CBg *CBg::Create(void)
{
	//�w�i�C���X�^���X�̐���
	CBg *pBg = new CBg;

	//�C���X�^���X���������ꂽ
	if (pBg != NULL)
	{
		//����������
		pBg->Init();

		//�e�N�X�`�����蓖��
		pBg->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//�w�i��Ԃ�
	return pBg;
}

//=======================================
//����������
//=======================================
HRESULT CBg::Init(void)
{
	if (FAILED(CObject2D::Init(D3DXVECTOR3(HALF_WIDTH, HALF_HEIGHT, 0.0f), HALF_WIDTH, HALF_HEIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_BG);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CBg::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObject2D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CBg::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CBg::Draw(void)
{
	//������`��
	CObject2D::Draw();
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CBg::Load(char *pPath)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(pPath);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}