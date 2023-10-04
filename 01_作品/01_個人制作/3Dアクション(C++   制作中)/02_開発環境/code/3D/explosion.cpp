//================================================================================================
//
//��������[explosion.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/billboard.h"
#include "explosion.h"
#include "object3D.h"
#include "wall.h"
#include "camera.h"

//==============================
//�}�N��
//==============================
#define PRIORITY_EXPLOSION	(3)		//�����̏����D�揇��

#define EXPLOSION_WIDTH		(10.0f)	//�|���S���̕�
#define EXPLOSION_HEIGHT	(10.0f)	//�|���S���̍���

//�ő�̗�
#define MAX_LIFE		(100)

//�e�N�X�`���̃p�X
#define TEX_PATH		"data\\TEXTURE\\explosion.png"
//�e�N�X�`���̕�����
#define TEX_WIDTH		(1.0f / 8.0f)
//�A�j���[�V�����̐؂�ւ���Ԋu
#define ANIM_INTERVAL	(8)
//�A�j���[�V�����̃p�^�[��
#define ANIM_PATTERN	(8)

//�ÓI�����o�ϐ�
int	CExplosion::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CExplosion::CExplosion() : CObjectBillboard()
{
	//�����o�ϐ��N���A
	m_nAnimCounter = 0;
	m_nAnimPattern = 0;
}

//=======================================
//�f�X�g���N�^
//=======================================
CExplosion::~CExplosion()
{

}

//=======================================
//��������
//=======================================
CExplosion *CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, bool bSound)
{
	//�e�C���X�^���X�̐���
	CExplosion *pExplosion = new CExplosion;

	//�C���X�^���X�������������ꂽ
	if (pExplosion != NULL)
	{
		//����������
		if (FAILED(pExplosion->Init()))
		{
			pExplosion->Uninit();	//�I������
			pExplosion->Release();	//�j��
			pExplosion = NULL;		//�|�C���^��NULL��
			return NULL;			//NULL��Ԃ�
		}

		//�e�N�X�`�����蓖��
		pExplosion->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));

		//�����o�ϐ�������
		/*�J�E���^�[*/	pExplosion->m_nAnimCounter = 0;
		/*�p�^�[��*/	pExplosion->m_nAnimPattern = 0;
		/*�ʒu�ݒ�*/	pExplosion->Set(pos, move);

		//�������Đ�
		if(bSound)	CManager::GetSound()->Play(CSound::SE_EXPLOSION);
	}

	//�e��Ԃ�
	return pExplosion;
}

//=======================================
//����������
//=======================================
HRESULT CExplosion::Init(void)
{
	//���_���W�̏�����
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(TEX_WIDTH, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_EXPLOSION);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CExplosion::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObjectBillboard::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CExplosion::Update(void)
{
	//�ړ�����^�C�v�̂��߂Ɉʒu�X�V
	Move();
	SetVertexInfo(EXPLOSION_WIDTH, EXPLOSION_WIDTH, 0.0f);

	//�����A�j���[�V����
	if (Animation())
	{
		//�Ȃ�����
		this->Release();
	}
	//�A�j���[�V�������I����ĂȂ�������
	else
	{
		//�e�N�X�`�����W�ݒ�
		SetVertexInfo(D3DXVECTOR2(TEX_WIDTH * (float)m_nAnimPattern, 0.0f), TEX_WIDTH, 1.0f);
	}
}

//---------------------
//�A�j���[�V��������
//---------------------
bool CExplosion::Animation(void)
{
	//�A�j���[�V�������I�������
	bool bEndAnim = false;

	//�J�E���^�[���񂵂��玟�̃R�}�ɐi�ގ��ԂɂȂ���
	if ((++m_nAnimCounter %= ANIM_INTERVAL) == 0)
	{
		//�p�^�[���𑝂₵�čő吔�ɒB������A�g��Ȃ�����
		if (++m_nAnimPattern == ANIM_PATTERN)
		{
			//�A�j���[�V�������I�����
			bEndAnim = true;
		}
	}

	//�I��������ǂ����Ԃ�
	return bEndAnim;
}

//=======================================
//�`�揈��
//=======================================
void CExplosion::Draw(void)
{
	//�����ƃJ�����̊ԂɃJ������������Ε`��
	if (CWall::IsAmongWall(CManager::GetCamera()->GetPosV(), GetPosition())) return;

	//������`��
	CObjectBillboard::Draw();
}

//=======================================
//�ʒu�ݒ�
//=======================================
void CExplosion::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	//�ʒu�E�ړ��ʁE������ݒ�
	CObjectBillboard::SetTransrate(pos, move, VEC3_INIT);

	//���_���W�ݒ�
	SetVertexInfo(EXPLOSION_WIDTH, EXPLOSION_HEIGHT, 0.0f);

	//�A�j���[�V�����J�E���^�ƃp�^�[����������
	m_nAnimCounter = m_nAnimPattern = 0;
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CExplosion::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}