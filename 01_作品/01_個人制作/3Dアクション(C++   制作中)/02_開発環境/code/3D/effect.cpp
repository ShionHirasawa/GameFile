//================================================================================================
//
//�G�t�F�N�g����[effect.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/billboard.h"
#include "effect.h"
#include "explosion.h"
#include "object3D.h"
#include "wall.h"
#include "camera.h"

//==============================
//�}�N��
//==============================
//�G�t�F�N�g�̏����D�揇��
#define PRIORITY_EFFECT	(3)

//�G�t�F�N�g�|���S���̃T�C�Y
#define EFFECT_SIZE		(10.0f)	

//�e�N�X�`���̃p�X
#define TEX_PATH	"data\\TEXTURE\\effect.jpg"

//�ő�̗�
#define MAX_LIFE		(30)

//�k���X�s�[�h
#define SHRINK_SIZE		(1.0f)

//�ÓI�����o�ϐ�
int		CEffect::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CEffect::CEffect() : CObjectBillboard(PRIORITY_EFFECT)
{
	//�����o�ϐ��N���A
	m_col = GetColor(COL_CLEAR);
	m_nLife = 0;
	m_fSize = 0.0f;
}

//=======================================
//�f�X�g���N�^
//=======================================
CEffect::~CEffect()
{

}

//=======================================
//���������i�Î~����G�t�F�N�g
//=======================================
CEffect *CEffect::Create(const D3DXVECTOR3 pos)
{
	//�G�t�F�N�g�C���X�^���X�̐���
	CEffect *pEffect = new CEffect;

	//�C���X�^���X��������
	if (pEffect != NULL)
	{
		//����������
		pEffect->Init();
		pEffect->Set(pos);

		//�e�N�X�`�����蓖��
		pEffect->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//�G�t�F�N�g��Ԃ�
	return pEffect;
}

//=======================================
//����������
//=======================================
HRESULT CEffect::Init(void)
{
	//���_���W�̏�����
	if (FAILED(CObjectBillboard::Init(GetPosition(), EFFECT_SIZE, EFFECT_SIZE, 0.0f, GetColor(COL_EFFECT), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_EFFECT);

	/*������������*/	m_nLife = 0;
	/*�T�C�Y��������*/	m_fSize = EFFECT_SIZE;
	/*�J���[������*/	m_col = GetColor(COL_EFFECT);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CEffect::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObjectBillboard::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CEffect::Update(void)
{
	//�����Ǘ�
	if (!LifeManage())
	{//�������s���Ȃ����� or �T�C�Y�������Ȃ�Ȃ�������
	 //���_���ݒ�
		SetVertexInfo(m_fSize, m_fSize, 0.0f);
	}
}

//=======================================
//�����Ǘ�
//=======================================
bool CEffect::LifeManage(void)
{
	//���������炷
	m_nLife--;

	//�T�C�Y���k������
	m_fSize -= SHRINK_SIZE;

	//�������s����	or	�T�C�Y�����ɂȂ���
	if (m_nLife <= 0 || m_fSize <= 0.0f)
	{
		//�Ȃ�����
		this->Release();

		//���S���Ȃ�ɂȂ���
		return true;
	}

	//�܂�������
	return false;
}

//=======================================
//�`�揈��
//=======================================
void CEffect::Draw(void)
{
	//�����ƃJ�����̊ԂɃJ������������Ε`��
	if (CWall::IsAmongWall(CManager::GetCamera()->GetPosV(), GetPosition())) return;

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���u�����f�B���O�����Z�����ɐݒ�
	CManager::GetRenderer()->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_SET);

	//������`��
	CObjectBillboard::Draw();

	//���u�����f�B���O�����ɖ߂�
	CManager::GetRenderer()->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_RESET);
}

//=======================================
//�ʒu�ݒ�
//=======================================
void CEffect::Set(const D3DXVECTOR3 pos)
{
	//�ʒu�E�ړ��ʁE������ݒ�
	CObjectBillboard::SetPosition(pos);

	m_col = GetColor(COL_EFFECT);//�J���[�ݒ�
	m_nLife = MAX_LIFE;			 //�̗͐ݒ�
	m_fSize = EFFECT_SIZE;		 //�T�C�Y�ݒ�

	SetVertexInfo(m_fSize, m_fSize, 0.0f);//���_���W�ݒ�
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CEffect::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}