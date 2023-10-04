//================================================================================================
//
//X�t�@�C���̃r���Q���f���Ǘ�����[buildsX.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "objectX.h"
#include "jewel.h"
#include "../2D/object2D.h"
#include "../2D/fade.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "../Mode/result.h"

//��΂��擾���鋗��
#define TOUCH_LENGTH	(30.0f)

//�ÓI�����o�ϐ��錾
D3DXVECTOR3 CJewel::m_pos = VEC3_INIT;
bool		CJewel::m_bDisp = true;
float		CJewel::m_fGoalLength = 0.0f;
D3DXVECTOR3 CJewel::m_GoalPoint = VEC3_INIT;
CMesh		*CJewel::m_pMesh = NULL;

//=======================================
//�R���X�g���N�^
//=======================================
CJewel::CJewel()
{
	//�����o�ϐ��N���A
	m_pos = VEC3_INIT;
	m_bDisp = true;
	m_GoalPoint = VEC3_INIT;
	m_fGoalLength = 0.0f;
	m_pMesh = NULL;
}

//=======================================
//�f�X�g���N�^
//=======================================
CJewel::~CJewel()
{

}

//=======================================
//����������
//=======================================
HRESULT CJewel::Init(char *pXFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 GoalPoint, float fLength)
{
	//�I�u�W�F�N�g�]������
	if (FAILED(CObjectX::Init(pXFilePath, XMODELTYPE_NONE, 0)))
	{
		//���������s
		return E_FAIL;
	}

	//�ʒu�E�ړ��ʁE�����ݒ�
	SetTransrate(pos, VEC3_INIT, VEC3_INIT);
	m_pos = pos;

	//�S�[���n�_�E���a�ݒ�
	m_GoalPoint = GoalPoint;
	m_fGoalLength = fLength;

	//�S�[������
	m_pMesh = new CMesh;
	m_pMesh->Init(m_GoalPoint, GetColor(COL_WHITE), 16, m_fGoalLength * 0.5f, 150.0f, 3.0f, 1.0f);
	m_pMesh->DispSwitch(false);

	//��ސݒ�
	CObject::SetType(TYPE_JEWEL);

	return S_OK;
}

//=======================================
//�I������
//=======================================
void CJewel::Uninit(void)
{
	CObjectX::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CJewel::Update(void)
{
	//�S�[�����`�悳��Ă��Ȃ��Ȃ�����Ԃ�
	if (m_bDisp || m_pMesh == NULL) return;

	//��
	D3DXVECTOR3 rot = m_pMesh->GetRotation();

	rot.y += 0.01f;

	FixRotate(&rot.y);

	m_pMesh->SetRotation(rot);
}

//=======================================
//�`�揈��
//=======================================
void CJewel::Draw(void)
{
	//�`�悷��
	if (m_bDisp)
	{
		//�`�揈��
		CObjectX::Draw(NULL);
	}
}

//=======================================
//�����蔻�菈��
//=======================================
void CJewel::Collision(D3DXVECTOR3 pos)
{
	//�ʒu�̍������擾
	D3DXVECTOR3 posDiff = pos - m_pos;

	//�����������A�����蔻�薢��
	if (D3DXVec3Length(&posDiff) <= TOUCH_LENGTH && m_bDisp)
	{
		//��΃Q�b�g�i�`�悳���Ȃ�
		m_bDisp = false;
		CManager::GetSound()->Play(CSound::SE_JEWEL_GET);

		//�S�[���o��
		m_pMesh->DispSwitch(true);
	}
}

//=======================================
//�S�[���B������
//=======================================
void CJewel::Goal(D3DXVECTOR3 pos)
{
	//�܂���΂������Ă��Ȃ�
	if (m_bDisp) return;

	//�ʒu�̍���
	D3DXVECTOR3 posDiff = pos - m_GoalPoint;

	//�S�[���̔��a�̒��ɓ����Ă���
	if (D3DXVec3Length(&posDiff) <= m_fGoalLength)
	{
		//���̃��[�h���擾
		int nMode = (CManager::GetMode() + 1) % CScene::MAX;

		//���̃��[�h�ֈڍs
		CFade::Set((CScene::MODE)nMode, CFade::FADE_OUT);
		CResult::SetState(CResult::STATE_CLEAR);
	}
}