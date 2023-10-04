//================================================================================================
//
//�e����[bullet.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/billboard.h"
#include "object3D.h"
#include "bullet.h"
#include "explosion.h"
#include "particle.h"
#include "effect.h"
#include "wall.h"
#include "../XFile/objectX.h"
#include "../XFile/enemy.h"

//==============================
//�}�N��
//==============================
//�e�̏����D�揇��
#define PRIORITY_BULLET	(3)

//�e�|���S���̃T�C�Y
#define BULLET_SIZE		(3.0f)

//�ő�̗�
#define MAX_LIFE		(100)

//�e�N�X�`���̃p�X
#define TEX_PATH		"data\\TEXTURE\\bullet.png"

//�ő�ړ����x
#define BULLET_SPEED	(5.0f)

/***		�G�ɓ��������Ƃ��̃}�N��		***/
//�G�t�F�N�g�̊g�U���i�G�ɓ��������Ƃ�����
#define NUM_EFFECT_SPREAD	(10)
//�p�[�e�B�N���̃��C�t
#define PARTICLE_LIFE		(5)

//�ÓI�����o�ϐ�
int	CBullet::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CBullet::CBullet() : CObjectBillboard(PRIORITY_BULLET)
{
	//�����o�ϐ��N���A
	m_nLife = 0;
}

//=======================================
//�f�X�g���N�^
//=======================================
CBullet::~CBullet()
{

}

//=======================================
//��������
//=======================================
CBullet *CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//�e�C���X�^���X�̐���
	CBullet *pBullet = new CBullet;

	//����������
	pBullet->Init();

	//�e�N�X�`�����蓖��
	pBullet->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	pBullet->Set(pos, rot);

	//�e��Ԃ�
	return pBullet;
}

//=======================================
//����������
//=======================================
HRESULT CBullet::Init(void)
{
	//���_���W�̏�����
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, BULLET_SIZE, BULLET_SIZE, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_BULLET);

	/*������������*/	m_nLife = 0;
	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CBullet::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObjectBillboard::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CBullet::Update(void)
{
	//�O��ʒu�X�V
	UpdatePosOld();

	//�ړ�����
	Move();

	//�����Ǘ�
	if (!LifeManage())
	{//�������s���Ȃ�������
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 PosOld = GetPosOld();
		D3DXVECTOR3 move = PosOld - pos;

		//�����蔻��
		if (!CollisionEnemy(pos) &&	// �G�ɓ�����Ȃ�����
			!CWall::Collision(&pos, GetPosOld(), &move, BULLET_SIZE, pos.y + BULLET_SIZE))//�ǂɓ�����Ȃ�����
		{//�G�ɓ�����Ȃ�����
			//���_���ݒ�
			SetVertexInfo(BULLET_SIZE, BULLET_SIZE, 0.0f);
		}
		else
		{
			//�ǂ����̓����蔻��ɂЂ���������
			//�����ݒ�
			CExplosion *pExplosion = CExplosion::Create(pos);

			//�Ȃ�����
			this->Release();
		}
	}
}

//=======================================
//�����Ǘ�
//=======================================
bool CBullet::LifeManage(void)
{
	//�������s�������ǂ���
	bool bDead = false;

	//���������炷
	m_nLife--;

	//�������s������
	if (m_nLife <= 0)
	{
		//�����ݒ�
		CExplosion *pExplosion = CExplosion::Create(CObjectBillboard::GetPosition());

		//�Ȃ�����
		this->Release();

		//�������s����
		bDead = true;
	}

	//������Ԃ�
	return bDead;
}

//=======================================
//�`�揈��
//=======================================
void CBullet::Draw(void)
{
	//������`��
	CObjectBillboard::Draw();
}

//=======================================
//�ʒu�ݒ�
//=======================================
void CBullet::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//�ړ��ʂ��v��
	D3DXVECTOR3 move;

	move.x = sinf(rot.y + PI_UP) * BULLET_SPEED;
	move.z = cosf(rot.y + PI_UP) * BULLET_SPEED;
	move.y = 0.0f;

	//�ʒu�E�ړ��ʁE������ݒ�
	CObjectBillboard::SetTransrate(pos, move, rot);
	SetVertexInfo(BULLET_SIZE, BULLET_SIZE, 0.0f);

	//�̗͐ݒ�
	m_nLife = MAX_LIFE;

	//���ˉ��Đ�
	CManager::GetSound()->Play(CSound::SE_SHOT);
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CBullet::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}

//=======================================
//�G�Ƃ̓����蔻��
//=======================================
bool CBullet::CollisionEnemy(const D3DXVECTOR3 pos)
{
	//�I�u�W�F�N�g�S�̂𔻒�
	for (int nCntPriority = 0; nCntPriority < CObject::OBJECT_PRIORIRY_MAX; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
		{
			//�G�̃|�C���^�����炤
			CEnemyRobot *pEnemy = (CEnemyRobot *)GetObject(nCntPriority, nCntObj);

			//�G�̃|�C���^���Ⴆ�Ȃ�����
			if (pEnemy == NULL || pEnemy->GetType() != TYPE_ROBOT) continue;

			//�ʒu�̍������i�[
			D3DXVECTOR3 PosDiff = pEnemy->GetPosition() - pos;
			PosDiff.y = 0.0f;

			//�G�̓�����͈͂ƒe�̃T�C�Y�̍��v���Z�o
			float fMargin = pEnemy->GetXModelMargin(pEnemy->GetMyXModelID()) + BULLET_SIZE;

			//�G�Ƃ̋�����������͈͈ȉ�
			if (fMargin >= D3DXVec3Length(&PosDiff))
			{
				//�_���[�W��^����
				pEnemy->GiveDamege(1);

				//�p�[�e�B�N����ʂ��ăG�t�F�N�g�𕡐�����
				CParticle *pPar = CParticle::Create(pos, true, NUM_EFFECT_SPREAD, PARTICLE_LIFE);

				return true;	//��������
			}
		}
	}

	//�p�[�e�B�N����ʂ��ăG�t�F�N�g��1����
	CParticle::Create(GetPosition(), false);

	//�N�ɂ�������Ȃ�����
	return false;
}

//=======================================
//�u���b�N�Ƃ̓����蔻��
//=======================================
bool CBullet::CollisionBlock(const D3DXVECTOR3 pos)
{
	//�I�u�W�F�N�g�S�̂𔻒�
	for (int nCntPriority = 0; nCntPriority < CObject::OBJECT_PRIORIRY_MAX; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
		{
			//�I�u�W�F�N�g�̃|�C���^�����炤
			CObject *pObj = GetObject(nCntPriority, nCntObj);

			//�I�u�W�F�N�g�̃|�C���^�����炦��
			if (pObj != NULL)
			{
				//�I�u�W�F�N�g�̎�ނ��擾
				TYPE type = pObj->GetType();

				//�I�u�W�F�N�g�̃^�C�v���u���b�N������
				if (type == CObject::TYPE_BLOCK)
				{
					/*�u���b�N�̍���ʒu���i�[*/ D3DXVECTOR3 BlockLeftOver;		pObj->GetVertexPos(BlockLeftOver, 0);
					/*�u���b�N�̉E���ʒu���i�[*/ D3DXVECTOR3 BlockRightUnder;	pObj->GetVertexPos(BlockRightUnder, 3);

					//�����蔻��
					if ((pos.x >= BlockLeftOver.x  &&	// �u���b�N�̍��[����E�ɁA
						pos.x <= BlockRightUnder.x) &&	// �u���b�N�̉E�[���獶�ɁA
						(pos.y >= BlockLeftOver.y  &&	// �u���b�N�̏㕔���牺�ɁA
						pos.y <= BlockRightUnder.y))	// �u���b�N�̉��������ɁA�e������
					{
						return true;	//��������
					}
				}
			}
		}
	}
	//�N�ɂ�������Ȃ�����
	return false;
}