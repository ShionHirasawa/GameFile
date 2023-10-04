//================================================================================================
//
//�r���{�[�h�̖؃|���S������[treeBillboard.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "billboard.h"
#include "treeBillboard.h"

//�t�B�[���h�̏����D�揇��
#define PRIORITY_FIELD	(2)
//�e�N�X�`���̃p�X
#define TEX_PATH		"data\\TEXTURE\\tree.png"

//�ÓI�����o�ϐ��錾
int CTree::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CTree::CTree() : CObjectBillboard(PRIORITY_FIELD)
{

}

//=======================================
//�f�X�g���N�^
//=======================================
CTree::~CTree()
{

}

//=======================================
//��������
//=======================================
CTree *CTree::Create(void)
{
	//�C���X�^���X����
	CTree *pTree = new CTree;

	//�C���X�^���X���������ꂽ
	if (pTree != NULL)
	{
		//�����������Ɏ��s
		if (FAILED(pTree->Init()))
		{
			delete pTree;//�����������
			pTree = NULL;//NULL�|�C���^��
			return NULL;	//NULL��Ԃ�
		}

		//�e�N�X�`�����蓖��
		pTree->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//�C���X�^���X��Ԃ�
	return pTree;
}

//=======================================
//����������
//=======================================
HRESULT CTree::Init(void)
{
	//����������
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{
		//���������s
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_BLOCK);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CTree::Uninit(void)
{
	//�I������
	CObjectBillboard::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CTree::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CTree::Draw(void)
{
	//�`�揈��
	CObjectBillboard::Draw();
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CTree::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}

//=======================================
//�ݒ菈��
//=======================================
void CTree::Set(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth)
{
	/* �ʒu�ݒ� */	SetPosition(pos);
	/*�T�C�Y�ݒ�*/	SetSize(D3DXVECTOR3(fWidth, fHeight, fDepth));

	//���_���W�ݒ�
	SetVertexInfo(fWidth, fHeight, fDepth);
}