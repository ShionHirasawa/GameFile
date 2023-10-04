//================================================================================================
//
//�y���색�C�u�����z�v�Z����[S_H_Control.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Model/motion.h"
#include "S_H_Calculation.h"
#include "S_H_Control.h"

//========================
//XZ���ʂ̈ړ������i�p�b�h�̃X�e�B�b�N��p
//------------------------
//	�����P	NowRot		�F�ړ�������҂̌��݂̌���
//	�����Q	fMoveSpeed	�F��{�̈ړ���
//	�����R	*pMove		�F�e�x�N�g���̈ړ��ʂ��i�[����|�C���^�i�Ȃ���Ώ������s��Ȃ�
//	�����S	*pMoveRot	�F�ړ��������i�[����|�C���^�i�����Ă����v
//	�����T	*pMotion	�F���[�V���������i�[����|�C���^�i�����Ă����v
//	�����U	nMotionType	�F���[�V�����̎�ށi�����Ă����v
//	�Ԃ�l�F���C���̃L�[�������ꂽ���ǂ����Ԃ�
//========================
bool Move_XZPlane(float NowRot, float fMoveSpeed, D3DXVECTOR3 *pMove, float *pMoveRot, CMotion *pMotion, int nMotionType)
{
	//�ړ��ʂ�ݒ肷��|�C���^�������Ȃ�����Ԃ�
	if (pMove == NULL) return false;

	//�p�b�h�擾
	CInputGamePad *pPad = CManager::GetGamePad();

	//�X�e�B�b�N���|��Ă��Ȃ�
	if (pPad->GetLStickX() == 0 && pPad->GetLStickY() == 0) return false;

	//�|��Ă��炱�̐恫�̏��������s

	//�ړ��������i�[
	float fMoveRot = pPad->GetLStickRot() - NowRot;
	FixRotate(&fMoveRot);

	pMove->x -= sinf(fMoveRot) * fMoveSpeed;	//�]�̈ړ��ʂ���
	pMove->z -= cosf(fMoveRot) * fMoveSpeed;	//�y�̈ړ��ʂ���

	if (pMoveRot != NULL) *pMoveRot = fMoveRot;	//�ړ���������
	if (pMotion != NULL) pMotion->Set(nMotionType);//���[�V�����ݒ�\�Ȃ�ݒ肷��

	return true;//���삵�����ǂ����Ԃ�
}

//========================
//XZ���ʂ̈ړ������i�\���L�[�E�\���p�b�h��p
//------------------------
//	�����P	MainKey		�F�ړ������郁�C���̃L�[
//	�����Q	MainMoveRot	�F���C���̈ړ�����
//	�����R	NowRot		�F�ړ�������҂̌��݂̌���
//	�����S	fMoveSpeed	�F��{�̈ړ���
//	�����T	*pMove		�F�e�x�N�g���̈ړ��ʂ��i�[����|�C���^�i�Ȃ���Ώ������s��Ȃ�
//	�����U	*pSub		�F�T�u��񂪓������\���̂̃|�C���^�i�����Ă����v
//	�����V	*pMoveRot	�F�ړ��������i�[����|�C���^�i�����Ă����v
//	�����W	*pMotion	�F���[�V���������i�[����|�C���^�i�����Ă����v
//	�����W	*pMotionType�F���[�V�����̎�ނ��i�[���ꂽ�|�C���^�i�����Ă����v
//	�Ԃ�l�F���C���̃L�[�������ꂽ���ǂ����Ԃ�
//========================
bool Move_XZPlane(int MainKey, int MainPad, float MainMoveRot, float NowRot, float fMoveSpeed, D3DXVECTOR3 *pMove, XZPlane_SubInfo *pSub, float *pMoveRot, CMotion *pMotion, int nMotionType)
{
	//�ړ��ʂ�ݒ肷��|�C���^�������Ȃ�����Ԃ�
	if (pMove == NULL) return false;

	//���C���̃L�[/�p�b�h��������Ȃ�����
	if (!CManager::IsInputPress(MainKey, MainPad)) return false;

	//�����ꂽ�炱�̐恫�̏��������s
	//�ړ��������i�[
	float fMoveRot = 0.0f;

	//�e�L�[�����ړ�������������
	if		(pSub != NULL && CManager::IsInputPress(pSub->Key1, pSub->Pad1))	fMoveRot = pSub->MoveRot1 - NowRot;//�T�u�P�����Ɉړ�
	else if (pSub != NULL && CManager::IsInputPress(pSub->Key2, pSub->Pad2))	fMoveRot = pSub->MoveRot2 - NowRot;//�T�u�Q�����Ɉړ�
	else	fMoveRot = MainMoveRot - NowRot;	//���C�������Ɉړ�

	pMove->x -= sinf(fMoveRot) * fMoveSpeed;	//�]�̈ړ��ʂ���
	pMove->z -= cosf(fMoveRot) * fMoveSpeed;	//�y�̈ړ��ʂ���

	if (pMoveRot != NULL) *pMoveRot = fMoveRot;	//�ړ���������
	if (pMotion != NULL) pMotion->Set(nMotionType);//���[�V�����ݒ�\�Ȃ�ݒ肷��

	//���삵�����ǂ����Ԃ�
	return true;
}

//========================
//���݂̌�����i�s�����Ɍ������鏈��
//------------------------
//	�����P	*pCurRot�F���݂̊p�x
//	�����Q	fDirRot �F�i�s����
//	�����R	fRotSpeed�F��]���x
//========================
void OrientToDir(float *pCurRot, float fDirRot, float fRotSpeed)
{
	//�����ړ������ւ̍��������𒴂��Ă�����
	if (fabsf(fDirRot - *pCurRot) > D3DX_PI)
	{
		//�ړ��p�x�ւ̌��������A�t���������������
		fDirRot = *pCurRot - fDirRot;
	}
	else
	{
		//�ړ������ƃv���C���[�̌��݂̊p�x�̍����v�Z����
		fDirRot -= *pCurRot;
	}

	//�v���C���[�̊p�x���ړ������֌�����
	*pCurRot += fDirRot * fRotSpeed;

	//�p�x�C��
	FixRotate(pCurRot);
}