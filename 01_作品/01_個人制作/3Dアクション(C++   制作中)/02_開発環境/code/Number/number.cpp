//================================================================================================
//
//�����֘A�̐ꑮ����[number.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "number.h"

//�e�N�X�`���̃p�X
#define TEX_PATH	"data\\TEXTURE\\number.png"
//�e�N�X�`���̕�����
#define TEX_WIDTH		(1.0f / 10.0f)

//�ÓI�����o�ϐ�
//�e�N�X�`��
int CNumber::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CNumber::CNumber(const int nPriority)
{
	//�����o�ϐ��N���A
	m_nNumber = 0;
}

//=======================================
//�f�X�g���N�^
//=======================================
CNumber::~CNumber()
{

}

//=======================================
//��������
//=======================================
CNumber *CNumber::Create(void)
{
	//�����C���X�^���X����
	CNumber *pNumber = new CNumber;

	//�C���X�^���X�𐶐��ł���
	if (pNumber != NULL)
	{
		//����������
		pNumber->Init();

		//�e�N�X�`�����蓖��
		pNumber->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//�C���X�^���X��Ԃ�
	return pNumber;
}

//=======================================
//����������
//=======================================
HRESULT CNumber::Init(void)
{
	//���_���W�̏�����
	if (FAILED(CObject2D::Init(VEC3_INIT, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(TEX_WIDTH, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_NUMBER);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CNumber::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObject2D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CNumber::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CNumber::Draw(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObject2D::Draw();
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CNumber::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}

//=======================================
//�������̐ݒ菈��
//=======================================
void CNumber::SetInfo(const int nNumber, const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	//�����̔ԍ���ݒ�
	m_nNumber = nNumber;

	//�ʒu�ݒ�
	SetPosition(pos);

	//���_���W�E�e�N�X�`�����W�̐ݒ�
	SetVertexInfo(fWidth, fHeight, D3DXVECTOR2(TEX_WIDTH * (float)m_nNumber, 1.0f), TEX_WIDTH, 1.0f);
}

//=======================================
//�e�N�X�`�����W�̐ݒ�
//=======================================
void CNumber::SetTexturePos(const int nNumber)
{
	//�����̔ԍ���ݒ�
	m_nNumber = nNumber;

	//�e�N�X�`�����W�̐ݒ�
	SetVertexInfo(D3DXVECTOR2(TEX_WIDTH * (float)m_nNumber, 1.0f), TEX_WIDTH, 1.0f);
}