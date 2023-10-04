//================================================================================================
//
//�J��������[camera.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "object3D.h"
#include "../Player/player.h"
#include "camera.h"

#define POS_POSV			D3DXVECTOR3(0.0f, 50.0f, -330.0f)	//���_�̏����ʒu
#define POS_POSR			D3DXVECTOR3(0.0f, 0.0f, 0.0f)		//�����_�̏����ʒu
#define POS_POSR_OFFSET		D3DXVECTOR3(0.0f, 50.0f, 0.0f)		//�����_�I�t�Z�b�g�̏����ʒu
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//������x�N�g���̏����l
#define VIEW_ANGLE			(45.0f)				//����p
#define MOVE_CAMERA_SPEED	(0.3f)				//�J�����E�����_���W�̈ړ���
#define MIN_DRAW_Z			(10.0f)				//�`�揈���̍Œቜ�s��
#define MAX_DRAW_Z			(2000.0f)			//�`�揈���̍ō����s��
#define MIN_LENGTH			(300.0f)			//���_�ƒ����_�̍Œ዗��
#define MAX_LENGTH			(400.0f)			//���_�ƒ����_�̍ő勗��
#define DEFAULT_LENGTH		(350.0f)			//���_�ƒ����_�̕W������
#define CHANGE_LENGHT		(0.1f)				//�����̈ړ���
#define SPIN_ROT			(0.005f)			//��]��
#define ROT_X_MAX_RANGE		(PI_RIGHT - 0.0001f)//�]���̍ő��]�͈́i���S�Ȕ��~���ƌ��E�l�ɂȂ����Ƃ��ɕ`��ł��Ȃ��Ȃ�̂ŁA�␳�l�������Ă���
#define ROT_X_MIN_RANGE		(0.00001f)			//�]���̍Œ��]�͈́i���ɂ߂荞�܂Ȃ��悤�ɒ���

const float CCamera::MOUSE_ROT_SPEED = 0.005f;
const float CCamera::PAD_ROT_SPEED = 0.05f;

//=======================================
//�R���X�g���N�^
//=======================================
CCamera::CCamera()
{

}

//=======================================
//�f�X�g���N�^
//=======================================
CCamera::~CCamera()
{

}

//=======================================
//�J�����̏���������
//=======================================
HRESULT CCamera::Init(void)
{
	//�J������񏉊���
	ResetPos();

	//����������
	return S_OK;
}

//=======================================
//�J�����̏I������
//=======================================
void CCamera::Uninit(void)
{

}

//=======================================
//�J�����̍X�V����
//=======================================
void CCamera::Update(void)
{
	/*�L�[�{�[�h�擾*/	CInputMouse *pMouse = CManager::GetMouse();

	/* ���_�ړ�	*/	MovePosV(pMouse);
	/* ���_��]	*/	SpinPosV(pMouse);
	/*�����_�ړ�*/	MovePosR(pMouse);

	//�J�������ړ�
	Move();
}

//=======================================
//���_�ړ�
//=======================================
void CCamera::MovePosV(CInputMouse *pMouse)
{
	//��������
	Length();
}

//=======================================
//���_��]
//=======================================
void CCamera::SpinPosV(CInputMouse *pMouse)
{
	//�p�b�h�擾
	CInputGamePad *pPad = CManager::GetGamePad();

	//�p�b�h�̉E�X�e�B�b�N���|��Ă���
	if (pPad->GetRStickX() != 0 || pPad->GetRStickY() != 0)
	{
		//�E�X�e�B�b�N�̊p�x���擾
		float fRot = pPad->GetRStickRot();

		//������ς���
		m_rot.x -= cosf(fRot) * PAD_ROT_SPEED;
		m_rot.y -= sinf(fRot) * PAD_ROT_SPEED;
	}

	//���N���b�N��������ĂāA�E�N���b�N�͉�����Ă��Ȃ�
	else if (pMouse->GetPress(pMouse->CLICK_LEFT))
	{
		//������ς���
		m_rot.x += pMouse->GetMove().y * MOUSE_ROT_SPEED;
		m_rot.y -= pMouse->GetMove().x * MOUSE_ROT_SPEED;
	}

	//�ړ����a�𒴂��Ă��Ȃ����`�F�b�N
	Range(&m_rot.x, ROT_X_MIN_RANGE, ROT_X_MAX_RANGE);

	//�p�x�C��
	FixRotate(&m_rot.y);
}

//=======================================
//�����_�ړ�
//=======================================
void CCamera::MovePosR(CInputMouse *pMouse)
{
	if (m_pPlayer == NULL)
	{
		//�����_�̈ʒu��  �����_�̏����ʒu�@�{�@�I�t�Z�b�g�ʒu�@�ɐݒ肷��
		m_posR = POS_POSR + m_posROffset;
	}
	else
	{
		//�����_�̈ʒu��  �v���C���[�̈ʒu�@�{�@�I�t�Z�b�g�ʒu�@�ɐݒ肷��
		m_posR = m_pPlayer->GetPosition() + m_posROffset;
	}
}

//=======================================
//�J�����̈ړ�����
//=======================================
void CCamera::Move(void)
{
	//�J�����̋����Čv�Z�i�J�����̈ړ��Ɋ�����������
	{
		//���ۂ̎��_�Ԃ̋��������߂�
		D3DXVECTOR3 PosDiff = m_posV - m_posR;
		float fLength = D3DXVec3Length(&PosDiff);

		//������͈͓��Ɏ��߂�
		Range(&fLength, MIN_LENGTH, MAX_LENGTH);

		//�v���C���[���ړ��������Ƃ��m�F�ł���
		if (m_pPlayer != NULL)
		{
			//�v���C���[�̈ړ��ʂ̒������擾
			float fVecMoveLength = m_pPlayer->GetMoveLength();

			//�قڎ~�܂��Ă���
			if (fVecMoveLength <= 0.5f)
			{
				//�J�����̋�����␳
				m_fLength += (DEFAULT_LENGTH - m_fLength) * 0.05f;
			}
			//�ړ���
			else
			{
				//�J�����̋�����␳
				m_fLength += (MAX_LENGTH - (fLength + fVecMoveLength)) * 0.1f;
			}

			//������͈͓��Ɏ��߂�
			Range(&m_fLength, MIN_LENGTH, MAX_LENGTH);
		}
	}

	//�w�E�y���̔��a
	float fFlatLength = cosf(m_rot.x) * m_fLength;

	m_posV.x = m_posR.x + sinf(D3DX_PI - m_rot.y) * fFlatLength;//���W���v�Z
	m_posV.z = m_posR.z + cosf(D3DX_PI - m_rot.y) * fFlatLength;//���W���v�Z
	m_posV.y = m_posR.y + sinf(D3DX_PI - m_rot.x) * m_fLength;	//���W���v�Z
}

//=======================================
//�J�����̈ʒu���Z�b�g
//=======================================
void CCamera::ResetPos(void)
{
	/*�I�t�Z�b�g*/	m_posROffset = POS_POSR_OFFSET;
	/*���_*/		m_posV = POS_POSV;
	/*����*/		m_rot = VEC3_INIT;
	/*������x�N�g��*/ m_vecU = POS_VECU;

	//�v���C���[�̃|�C���^������
	if (m_pPlayer != NULL)
	{
		//�v���C���[�̈ʒu�ɒ����_��u��
		m_posR = m_pPlayer->GetPosition() + m_posROffset;
	}
	//������Ό��_���W�ɂ���
	else
	{
		m_posR = VEC3_INIT + m_posROffset;
	}

	//�������Z�o
	m_fLength = D3DXVec3Length(&(m_posR - m_posV));

	//�p�x���Z�o����
	CalculateRotate();

	//�ʒu��ݒ肷��
	Move();
}

//=======================================
//���_�Ԃ̒�������
//=======================================
void CCamera::Length(void)
{
	//������͈͓��Ɏ��߂�
	Range(&m_fLength, MIN_LENGTH, MAX_LENGTH);
}

//=======================================
//�p�x���v�Z����
//=======================================
void CCamera::CalculateRotate(void)
{
	//���_�ƒ����_�̈ʒu�̍�
	D3DXVECTOR3 fDiffPos = m_posR - m_posV;

	//�p�x���v�Z����
	m_rot.x = +atan2f(fDiffPos.y, fDiffPos.z);
	m_rot.y = -atan2f(fDiffPos.x, fDiffPos.z);
}

//=======================================
//�J�����̐ݒ菈��
//=======================================
void CCamera::Set(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(VIEW_ANGLE), SCREEN_WIDTH_F / SCREEN_HEIGHT_F, MIN_DRAW_Z, MAX_DRAW_Z);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxview);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxview, &m_posV, &m_posR, &m_vecU);

	//�r���[�}�g���b�N�X
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxview);
}

//=======================================
//�J�����̋t�p�x���擾����
//=======================================
float CCamera::GetInverseRotate(void)
{
	return Inverse(m_rot.y);
}