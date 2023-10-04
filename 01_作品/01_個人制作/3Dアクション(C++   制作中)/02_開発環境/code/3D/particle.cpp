//================================================================================================
//
//�p�[�e�B�N������[particle.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/billboard.h"
#include "effect.h"
#include "particle.h"
#include "explosion.h"
#include "../S_H_Lib/S_H_Calculation.h"

//==============================
//�}�N��
//==============================
//�p�[�e�B�N���̏����D�揇��
#define PRIORITY_PARTICLE	(3)
//��x�ɐ�������ő吔
#define MAX_SET				(20)
//�g�U�X�s�[�h
#define SPREAD_SPEED		(700)
//�Œ�g�U�X�s�[�h
#define MIN_SPREAD_SPEED	(0.5f)

//=======================================
//�R���X�g���N�^
//=======================================
CParticle::CParticle() : CObjectBillboard(PRIORITY_PARTICLE)
{
	//�����o�ϐ��N���A
	m_nLife = 0;
	m_pos = VEC3_INIT;
	m_move = VEC3_INIT;
}

//=======================================
//�f�X�g���N�^
//=======================================
CParticle::~CParticle()
{

}

//=======================================
//��������
//=======================================
CParticle *CParticle::Create(const D3DXVECTOR3 pos, const bool bMove, const int nNumSet, const int nParticleLife)
{
	//�ݒ肷�鐔���[�v
	for (int nCntParticle = 0; nCntParticle < nNumSet; nCntParticle++)
	{
		//������������𒴂���
		if (nCntParticle >= MAX_SET)
		{//�����𔲂���
			break;
		}

		//�p�[�e�B�N���̃C���X�^���X����
		CParticle *pParticle = new CParticle;

		//�C���X�^���X��������
		if (pParticle != NULL)
		{
			/* �ʒu �ݒ�*/pParticle->m_pos = pos;
			/* ���� �ݒ�*/pParticle->m_nLife = nParticleLife;
			/*�^�C�v�ݒ�*/pParticle->SetType(TYPE_PARTICLE);

			//�ړ������Ȃ��ꍇ
			if (!bMove)
			{
				//�ړ������Ȃ�
				pParticle->m_move = VEC3_INIT;
			}
			else
			{
				//�ړ��ʂ��v��
				pParticle->m_move.x = sinf(RandomRotate()) * (float)(rand() % SPREAD_SPEED) * FIX_FLOAT + MIN_SPREAD_SPEED;
				pParticle->m_move.y = cosf(RandomRotate()) * (float)(rand() % SPREAD_SPEED) * FIX_FLOAT + MIN_SPREAD_SPEED;
				pParticle->m_move.z = cosf(RandomRotate()) * (float)(rand() % SPREAD_SPEED) * FIX_FLOAT + MIN_SPREAD_SPEED;
			}

			//����������
			pParticle->Init();
		}
	}

	//�p�[�e�B�N����Ԃ�
	return NULL;
}

//=======================================
//����������
//=======================================
HRESULT CParticle::Init(void)
{
	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CParticle::Uninit(void)
{
	//�������g��j��
	this->Release();
}

//=======================================
//�X�V����
//=======================================
void CParticle::Update(void)
{
	//�G�t�F�N�g����
	CEffect *pEff = CEffect::Create(m_pos);

	//�ړ�
	m_pos += m_move;

	//��������
	m_nLife--;

	//�������s����
	if (m_nLife <= 0)
	{
		//������j��
		this->Release();
	}
}

//=======================================
//�`�揈��
//=======================================
void CParticle::Draw(void)
{

}