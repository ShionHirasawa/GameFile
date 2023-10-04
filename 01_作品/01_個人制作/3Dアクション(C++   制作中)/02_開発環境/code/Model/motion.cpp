//================================================================================================
//
//���[�V�����Ǘ�����[motion.h]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "motion.h"

//========================
//�}�N����`
//========================
//�u�����h�ɗv���鎞��
#define BLEND_TIME	(30)

//========================
//�R���X�g���N�^
//========================
CMotion::CMotion()
{
	//�����o�ϐ��N���A
	//�e���[�V�������N���A
	for (int nCntInfo = 0; nCntInfo < MOTIONTYPE_MAX; nCntInfo++)
	{
		/*���[�V�������N���A*/InitMotionInfo(&m_aInfo[nCntInfo]);
		/*���[�v���N���A*/m_aInfo[nCntInfo].bLoop = false;
		/*�L�[�̑����N���A*/m_aInfo[nCntInfo].nNumKey = 0;
	}

	m_nNumAll = 0;
	m_nType = 0;
	m_bLoop = false;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nCounter = 0;
	m_bFinish = false;	//�I���������ǂ���
	m_nBlendType = 0;
	m_bBlend = false;
	m_bAction = false;

	m_ppModel = NULL;	//���f���ւ̃|�C���^
	m_nNumModel = 0;	//���f���̑���

	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		m_OffSetPos[nCntParts] = VEC3_INIT;	//�����ʒu
		m_OffSetRot[nCntParts] = VEC3_INIT;	//�����p�x

		m_TargetPos[nCntParts] = m_MovePos[nCntParts] = VEC3_INIT;//�p�[�c�̌��݈ʒu�E�ڕW�ʒu�E�ړ��ʂ��N���A
		m_TargetRot[nCntParts] = m_MoveRot[nCntParts] = VEC3_INIT;//�p�[�c�̌��ݕ����E�ڕW�����E�ړ��ʂ��N���A
	}
}

//========================
//�f�X�g���N�^
//========================
CMotion::~CMotion()
{

}

//========================
//�X�V����
//========================
void CMotion::Update(void)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		//���݈ʒu���X�V����
		m_Pos[nCntParts] += m_MovePos[nCntParts];
		m_Rot[nCntParts] += m_MoveRot[nCntParts];

		//�ʒu�E��]�l��ݒ�
		m_ppModel[nCntParts]->OffSetTranslate(m_Pos[nCntParts], m_Rot[nCntParts]);
	}

	//���[�V�����J�E���^�[����
	m_nCounter++;

	//���[�V�����J�E���^�[�����������L�[�̍ő�t���[���ɒB����
	if (m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame <= m_nCounter)
	{
		//�u�����h��������
		if (m_bBlend)
		{
			m_bBlend = false;		//�u�����h�I��
			m_nType = m_nBlendType;	//��ނ�ݒ肷��
			m_nBlendType = 0;		//�u�����h�����ޏ�����
			m_nKey = -1;
		}

		StepKey();				//���̃L�[��
	}

	//�A�N�V�������[�V�����̃L�[�ԍ��P�ɂȂ����Ƃ��ɁA�U���A�N�V�����\
	if (m_nType == MOTIONTYPE_ACTION && m_nKey == 1 && m_nCounter % 9 == 0)
	{
		m_bAction = true;
	}
	//�����𖞂����Ȃ��Ƃ��́A�A�N�V�������o���Ȃ�����
	else
	{
		m_bAction = false;
	}
}

//========================
//���̃��[�V�����L�[��
//========================
void CMotion::StepKey(void)
{
	m_nCounter = 0;	//�J�E���^�[������
	m_nKey++;		//���̕�������

					//�S�̕������ɒB����
	if (m_aInfo[m_nType].nNumKey <= m_nKey)
	{
		/*�L�[���ŏ��ɖ߂�*/	m_nKey = 0;
		/*���[�V�����Đ��I��*/	m_bFinish = true;

		//���[�V���������[�v���Ȃ��ꍇ
		if (!m_aInfo[m_nType].bLoop)
		{
			//�ҋ@���[�V������
			m_nType = MOTIONTYPE_NEUTRAL;
		}
	}

	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		//�e�p�[�c�̖ڕW�l��ݒ肷��
		m_TargetPos[nCntParts].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosX;
		m_TargetPos[nCntParts].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosY;
		m_TargetPos[nCntParts].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosZ;
		m_TargetRot[nCntParts].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotX;
		m_TargetRot[nCntParts].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotY;
		m_TargetRot[nCntParts].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotZ;
	}

	//�ړ��ʂ��v�Z����
	SetMoveParts();
}

//========================
//���[�V�����̎�ނ�ݒ�
//========================
void CMotion::Set(int nType) 
{
	if (m_nType != nType || //���݂̃��[�V�����̎�ނƁA�ݒ肵�������[�V�����̎�ނ��Ⴄ
		(m_nType == nType && m_bBlend))	//���݂̃��[�V�����Ɠ����ł��A���[�V�����u�����h���ł���
	{
		m_nKey = 0;			//�L�[�ԍ�������
		m_nCounter = 0;		//���[�V�����J�E���^�[������
		m_nType = nType;	//��ނ�ݒ�
		m_nBlendType = nType;//�u�����h��������ނ��������̂�ݒ�
		m_bBlend = false;	//�u�����h��ԉ���
		m_bFinish = false;	//�I������

		for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
		{
			//�e�p�[�c�̖ڕW�l��ݒ肷��
			m_TargetPos[nCntParts].x = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fPosX;
			m_TargetPos[nCntParts].y = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fPosY;
			m_TargetPos[nCntParts].z = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fPosZ;
			m_TargetRot[nCntParts].x = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fRotX;
			m_TargetRot[nCntParts].y = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fRotY;
			m_TargetRot[nCntParts].z = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fRotZ;
		}

		//�e�p�[�c�̈ړ��ʂ��v�Z����
		SetMoveParts();
	}
}

//========================
//���[�V�����̎�ނ�ݒ�
//========================
void CMotion::SetPos(void)
{
	//�e�p�[�c�̌��݂̈ʒu��ݒ肷��
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		m_Pos[nCntParts].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosX;
		m_Pos[nCntParts].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosY;
		m_Pos[nCntParts].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosZ;
		m_Rot[nCntParts].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotX;
		m_Rot[nCntParts].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotY;
		m_Rot[nCntParts].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotZ;
	}

	//�ړ��ʂ�ݒ肷��
	SetMoveParts();
}

//========================
//���[�V�����̏���ݒ肷��
//========================
void CMotion::SetInfo(MOTION_INFO info)
{
	//���[�V�������ݒ�
	m_aInfo[m_nType] = info;
}

//========================
//���[�V���������郂�f����ݒ肷��
//========================
void CMotion::SetModel(CModel **ppModel, int nNumModel)
{
	m_ppModel = ppModel;	//���f���̐擪�A�h���X���L������
	m_nNumModel = nNumModel;//���f���̑������L������
}

//========================
//�p�[�c�̏����ʒu�E�p�x�ݒ�
//========================
void CMotion::SetOffset(D3DXVECTOR3 offSet, D3DXVECTOR3 offRot, int nCntSet)
{
	m_OffSetPos[nCntSet] = offSet;	//�����ʒu�ݒ�
	m_OffSetRot[nCntSet] = offRot;	//�����p�x�ݒ�
}

//========================
//���[�V�����̃u�����h�ݒ�
//========================
void CMotion::SetBlend(int nType)
{
	//���݂̃��[�V�����̎�ނƃu�����h���������[�V�����������Ȃ�A�u�����h���Ȃ�
	if (m_nType == nType)
	{
		return;
	}

	//�u�����h�����ނ�ݒ�
	m_nBlendType = nType;
	m_bBlend = true;
}

//========================
//�ړ��ʂ�ݒ肷��
//========================
void CMotion::SetMoveParts(void)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		D3DXVECTOR3 *pMovePos = &m_MovePos[nCntParts];
		D3DXVECTOR3 *pMoveRot = &m_MoveRot[nCntParts];

		//�e�p�[�c���Ƃ̍��W�E�����̈ړ��ʂ����̃��[�V�����̃t���[��������Ɍv�Z����
		m_MovePos[nCntParts] = (m_TargetPos[nCntParts] - m_Pos[nCntParts]) / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame;
		m_MoveRot[nCntParts] = (m_TargetRot[nCntParts] - m_Rot[nCntParts]) / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame;
	}
}

//========================
//�p�[�c��񃊃Z�b�g
//========================
void CMotion::ResetParts(KEY *pKEY)
{
	pKEY->fPosX = 0.0f;
	pKEY->fPosY = 0.0f;
	pKEY->fPosZ = 0.0f;
	pKEY->fRotX = 0.0f;
	pKEY->fRotY = 0.0f;
	pKEY->fRotZ = 0.0f;
}

//========================
//���[�V������񏉊���
//========================
void CMotion::InitMotionInfo(MOTION_INFO *pInfo)
{
	//���������̃|�C���^��NULL��������
	if (pInfo == NULL)
	{
		//���������������Ԃ�
		return;
	}

	//�L�[���N���A
	for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
	{
		//�e�L�[�̍��W�E��]�������Z�b�g
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			ResetParts(&pInfo->aKeyInfo[nCntKey].aKey[nCntParts]);
		}

		//�t���[�����N���A
		pInfo->aKeyInfo[nCntKey].nFrame = 0;
	}
}