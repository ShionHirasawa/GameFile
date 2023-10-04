//================================================================================================
//
//�y���색�C�u�����z�v�Z����[S_H_Control.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _S_H_CONTROL_H_
#define _S_H_CONTROL_H_

class CMotion;

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
bool Move_XZPlane(float NowRot, float fMoveSpeed, D3DXVECTOR3 *pMove, float *pMoveRot = NULL, CMotion *pMotion = NULL, int nMotionType = ERROR_ID);

//XZ���ʂ̈ړ������ŃT�u�ňړ���������
struct XZPlane_SubInfo
{
	int		Key1;		//���C���ɑ΂��āA�x�N�g��������������T�u�L�[�P
	int		Key2;		//���C���ɑ΂��āA�x�N�g��������������T�u�L�[�Q
	int		Pad1;		//���C���ɑ΂��āA�x�N�g��������������T�u�p�b�h�P
	int		Pad2;		//���C���ɑ΂��āA�x�N�g��������������T�u�p�b�h�Q
	float	MoveRot1;	//�T�u�P�̈ړ�����
	float	MoveRot2;	//�T�u�Q�̈ړ�����
};

//========================
//XZ���ʂ̈ړ������i�\���L�[�E�\���p�b�h��p
//------------------------
//	�����P	MainKey		�F�ړ������郁�C���̃L�[
//	�����Q	MainPad		�F�ړ������郁�C���̃p�b�h
//	�����R	MainMoveRot	�F���C���̈ړ�����
//	�����S	NowRot		�F�ړ�������҂̌��݂̌���
//	�����T	fMoveSpeed	�F��{�̈ړ���
//	�����U	*pMove		�F�e�x�N�g���̈ړ��ʂ��i�[����|�C���^�i�Ȃ���Ώ������s��Ȃ�
//	�����V	*pSub		�F�T�u��񂪓������\���̂̃|�C���^�i�����Ă����v
//	�����W	*pMoveRot	�F�ړ��������i�[����|�C���^�i�����Ă����v
//	�����X	*pMotion	�F���[�V���������i�[����|�C���^�i�����Ă����v
//	����10	nMotionType	�F���[�V�����̎�ށi�����Ă����v
//	�Ԃ�l�F���C���̃L�[�������ꂽ���ǂ����Ԃ�
//========================
bool Move_XZPlane(int MainKey, int MainPad, float MainMoveRot, float NowRot, float fMoveSpeed, D3DXVECTOR3 *pMove, XZPlane_SubInfo *pSub = NULL, float *pMoveRot = NULL, CMotion *pMotion = NULL, int nMotionType = ERROR_ID);

//========================
//���݂̌�����i�s�����Ɍ������鏈��
//------------------------
//	�����P	*pCurRot�F���݂̊p�x
//	�����Q	fDirRot �F�i�s����
//	�����R	fRotSpeed�F��]���x
//========================
void OrientToDir(float *pCurRot, float fDirRot, float fRotSpeed);

#endif