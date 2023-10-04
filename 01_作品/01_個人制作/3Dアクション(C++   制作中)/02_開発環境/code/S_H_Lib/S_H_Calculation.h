//================================================================================================
//
//�y���색�C�u�����z�v�Z����[S_H_Calculation.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _S_H_CALCULATION_H
#define _S_H_CALCULATION_H

#include "../3D/mesh.h"

//========================
//float�^�͈̔͌v�Z
// �T�v	�F��ڂ̈�����͈͂̐�Βl�Ɏ��߂ĕԂ�
//------------------------
//	�����P	*pValue	�F�v�Z�Ώۂ̕ϐ��̃|�C���^
//	�����Q	fAbs	�F�͈͂̐�Βl
//========================
float Range(float *pValue, float fAbs);

//========================
//float�^�͈̔͌v�Z
// �T�v	�F1�ڂ̈������A�Q�E�R�ڂ̈����͈̔͂Ɏ��߂ĕԂ�
//���I�[�o�[���[�h���Ă���Range�֐��͂��̊֐��ɏW�񂷂�
//------------------------
//	�����P	*pValue	�F�v�Z�Ώۂ̕ϐ��̃|�C���^
//	�����Q	fMin	�F�͈͂̍ŏ��l
//	�����R	fMax	�F�͈͂̍ő�l
//========================
float Range(float *pValue, float fMin, float fMax);

//========================
//D3DXVECTOR3�^�͈̔͌v�Z
// �T�v	�F�R�̈����̊e�x�N�g����͈͂̐�Βl�Ɏ��߂ĕԂ�
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, float fAbs);

//========================
//D3DXVECTOR3�^�͈̔͌v�Z
// �T�v	�F�R�̈����̊e�x�N�g����͈͂Ɏ��߂�
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, float fMin, float fMax);

//========================
//float�^�͈̔͌v�Z
// �T�v	�F��ڂ̈����̊e�x�N�g����͈͂̐�Βl�Ɏ��߂ĕԂ�
//------------------------
//	�����P	*pValue	�F�v�Z�Ώۂ̕ϐ��̃|�C���^
//	�����Q	Abs		�F�͈͂̐�Βl
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, D3DXVECTOR3 Abs);

//========================
//D3DXVECTOR3�^�͈̔͌v�Z
// �T�v	�F�R�̈����̊e�x�N�g����͈͂Ɏ��߂�
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, D3DXVECTOR3 Min, D3DXVECTOR3 Max);

//========================
//�t�̊p�x�����߂�
//------------------------
// �����P	rot�F�C������p�x��������D3DXVECTOR3�^�̃|�C���^
// �Ԃ�l	�����̔��΂̊p�x��Ԃ�
//========================
float Inverse(float rot);

//========================
//�p�x�C��
//------------------------
// �����P	*pRot�F�C������p�x��������float�^�̃|�C���^
// �Ԃ�l	�������p�x��Ԃ�
//========================
float FixRotate(float *pRot);

//========================
//�p�x�C��
// �T�v	�F�e�x�N�g���̐������p�x���I�[�o�[���[�h���Ă���float�^�́uFixRotate�֐��v�Ŏ擾����
//------------------------
// �����P	*pRot�F�C������p�x��������D3DXVECTOR3�^�̃|�C���^
// �Ԃ�l	�e�x�N�g���̐������p�x��Ԃ�
//========================
D3DXVECTOR3 FixRotate(D3DXVECTOR3 *pRot);

//========================
//���[���h���W���擾
//------------------------
// �����P	Rot�F�I�u�W�F�N�g�̊p�x
// �����Q	size�F�I�u�W�F�N�g�̃T�C�Y
// �����R	Pos�F�I�u�W�F�N�g�̈ʒu
// �Ԃ�l	�v�Z�������[���h���W��Ԃ�
//========================
D3DXVECTOR3 GetWorldPosition(float Rot, D3DXVECTOR3 size, D3DXVECTOR3 Pos);

//========================
//��ԋ߂��p�x��Ԃ�
//------------------------
// �����P	Rot			�F��̊p�x
// �����Q	fFrontLength�F�[���琳�ʂ܂ł̒���
// �����R	Pos			�F������s�����W
// �����S	RangeLeft	�F������s���͈͂̍���
// �����T	RangeRight	�F������s���͈͂̉E��
// �Ԃ�l	�͈͂̒��ň�ԋ߂��p�x��Ԃ�
//				�͈͓��̏ꍇ�F��p�x�̋t��Ԃ�
//				�͈͊O�̏ꍇ�F�͈͂𒴂��Ă���ق��̊p�x��Ԃ�
//========================
float NearRotate(float Rot, float fFrontLength, D3DXVECTOR3 Pos, D3DXVECTOR3 RangeLeft, D3DXVECTOR3 RangeRight);

//========================
//��_�����߂鏈��
// �T�v	�F�Q�̈قȂ�x�N�g�����D��Ȃ��A�O�p�`���g���Č�_�����߂�
//------------------------
// �����P	pos		�F��̈ʒu
// �����Q	VecMove	�F��`�Ώۈʒu�܂ł̈ړ��x�N�g��
// �����R	StartPos�F������̃x�N�g���̊J�n���W
// �����S	EndPos	�F������̃x�N�g���̏I�[���W
// �Ԃ�l	�Q�̃x�N�g���̌�_��Ԃ�
//========================
D3DXVECTOR3 CrossPoint(D3DXVECTOR3 pos, D3DXVECTOR3 VecMove, D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);

//========================
//D3DXVECTOR3�^�̊p�x�v�Z
// �T�v	�F���̊֐���XZ���ʂ̑z��Ōv�Z���s���A���̊p�x��Ԃ�
//------------------------
// �����P	v�F���W���i�[�����ϐ�
// �Ԃ�l	XZ���ʂ̊p�x��Ԃ�
//========================
float atan2f(D3DXVECTOR3 v);

//========================
//D3DXVECTOR3�^�̊e�x�N�g���̐�Βl��Ԃ�
//------------------------
// �����P	v�F��Βl���擾�������ϐ�
// �Ԃ�l	�e�x�N�g�����ufabsf�֐��v���g�p���ē�����Βl��Ԃ�
//========================
D3DXVECTOR3 fabsV3(D3DXVECTOR3 v);

//========================
//���͈͓̔��ɑΏە������邩�ǂ����{������
//------------------------
// �����P	pos			�F���S�ʒu
// �����Q	TargetPos	�F�Ώە��̈ʒu
// �����R	fLeftAngle	�F���[�̊p�x
// �����S	fRightAngle	�F�E�[�̊p�x
// �����T	fLength		�F�͈͂̒���
// �Ԃ�l	�{���͈͓��ɑΏۂ����邩�ǂ������ʂ�Ԃ�	true�F�Ώۂ�����	false�F�Ώۂ����Ȃ�
//========================
bool Search(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos, float fLeftAngle, float fRightAngle, float fLength);

//========================
//�����𒲂ׂ�
//------------------------
//�����@�F�����𒲂ׂ���int�^�̕ϐ�
//�Ԃ�l�F����
//========================
int Dejit(int nData);

//========================
//�Ώە��̒��ɂ߂荞��ł��邩�ǂ�������
//------------------------
// �����P	MinPos			�F�����̍ŏ��ʒu
// �����Q	MaxPos			�F�����̍ő�ʒu
// �����R	TargetMinPos	�F�Ώە��̍ŏ��ʒu
// �����S	TargetMaxPos	�F�Ώە��̍ő�ʒu
// �Ԃ�l	�Ώە��ɂ߂肱��ł��邩�ǂ���	true�F�߂肱��ł���	false�F�߂肱��ł��Ȃ�
//========================
bool IsBoxCollision(D3DXVECTOR3 MinPos, D3DXVECTOR3 MaxPos, D3DXVECTOR3 TargetMinPos, D3DXVECTOR3 TargetMaxPos);

//=========================
//�t�B�[���h�^�̃��b�V���o�b�t�@�[���擾
//=========================
CMesh::Buffer GetMeshBuffer(int nColumn, int nRow);

//=========================
//�V�����_�[�^�̃��b�V���o�b�t�@�[���擾
//=========================
CMesh::Buffer GetMeshBuffer(int nDevide);

//=========================
//�����_���Ŋp�x���擾
//�T�v�F�S�̂̊p�x��3.14�`-3.14��6.28
//		���̊p�x���Z�o���邽�߂ɁA������628�܂ł̐��l�𗐐��Ŏ擾���A
//		������u-314�v���ė������u314�`-314�v�̊Ԃɒ����A�ŏI�I�Ɂu��100�v���āu3.14�`-3.14�v�ɂ������̂�Ԃ�
//=========================
float RandomRotate(void);
#endif