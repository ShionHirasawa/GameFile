//================================================================================================
//
//�y���색�C�u�����z�v�Z����[S_H_Calculation.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include"../Main/main.h"
#include "S_H_Calculation.h"

//========================
//float�^�͈̔͌v�Z
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
float Range(float *pValue, float fAbs)
{
	return Range(pValue, -fAbs, fAbs);
}

//========================
//float�^�͈̔͌v�Z
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
float Range(float *pValue, float fMin, float fMax)
{
	//���Ƃ��Ɣ͈͂Ɏ��܂��Ă���΂��̂܂ܕԂ�
	if(fMin <= *pValue && *pValue <= fMax)	return *pValue;

	//�ŏ��l��������Ă��邩
	else if (*pValue < fMin) return *pValue = fMin;

	//�ő�l�������Ă���
	return *pValue = fMax;
}

//========================
//D3DXVECTOR3�^�͈̔͌v�Z
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, float fAbs)
{
	//�e�x�N�g����͈͂Ɏ��߂�
	Range(&pValue->x, -fAbs, fAbs);
	Range(&pValue->y, -fAbs, fAbs);
	Range(&pValue->z, -fAbs, fAbs);

	//�C�����ʂ�Ԃ�
	return *pValue;
}

//========================
//D3DXVECTOR3�^�͈̔͌v�Z
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, float fMin, float fMax)
{
	//�e�x�N�g����͈͂Ɏ��߂�
	pValue->x = Range(&pValue->x, fMin, fMax);
	pValue->y = Range(&pValue->y, fMin, fMax);
	pValue->z = Range(&pValue->z, fMin, fMax);

	//�C�����ʂ�Ԃ�
	return *pValue;
}

//========================
//float�^�͈̔͌v�Z
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, D3DXVECTOR3 Abs)
{
	//�e�x�N�g����͈͂Ɏ��߂�
	Range(&pValue->x, -Abs.x, Abs.x);
	Range(&pValue->y, -Abs.y, Abs.y);
	Range(&pValue->z, -Abs.z, Abs.z);

	//�C�����ʂ�Ԃ�
	return *pValue;
}

//========================
//D3DXVECTOR3�^�͈̔͌v�Z
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	//�e�x�N�g����͈͂Ɏ��߂�
	pValue->x = Range(&pValue->x, Min.x, Max.x);
	pValue->y = Range(&pValue->y, Min.y, Max.y);
	pValue->z = Range(&pValue->z, Min.z, Max.z);

	//�C�����ʂ�Ԃ�
	return *pValue;
}

//========================
//�t�̊p�x�����߂�
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
float Inverse(float rot)
{
	//���~���̊p�x�𑫂��āA�P�����̊p�x�Ŋ��������܂����
	rot = fmodf(rot + D3DX_PI, FIX_ROT);

	//�p�x�C���������̂�Ԃ�
	return FixRotate(&rot);
}

//========================
//�p�x�C��
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
float FixRotate(float *pRot)
{
	//�p�x�C��
	return *pRot = fmodf(*pRot + D3DX_PI * 3, D3DX_PI * 2) - D3DX_PI;
}

//========================
//�p�x�C��
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
D3DXVECTOR3 FixRotate(D3DXVECTOR3 *pRot)
{
	FixRotate(&pRot->x);//�]���̉�]�l�C��
	FixRotate(&pRot->y);//�x���̉�]�l�C��
	FixRotate(&pRot->z);//�y���̉�]�l�C��

	return *pRot;		//�p�x��Ԃ�
}

//========================
//���[���h���W���擾
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
D3DXVECTOR3 GetWorldPosition(float Rot, D3DXVECTOR3 size, D3DXVECTOR3 Pos)
{
	D3DXVECTOR3 pos = VEC3_INIT;

	//�e���W�̃��[���h���W���v�Z
	pos.x = roundf(sinf(Rot) * size.x + Pos.x);
	pos.z = roundf(cosf(Rot) * size.x + Pos.z);

	//���W��Ԃ�
	return pos;
}

//========================
//��ԋ߂��p�x��Ԃ�
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
float NearRotate(float Rot, float fFrontLength, D3DXVECTOR3 Pos, D3DXVECTOR3 RangeLeft, D3DXVECTOR3 RangeRight)
{
	//���[�̐��ʈʒu
	D3DXVECTOR3 vecFront;
	vecFront.x = sinf(Rot) * -fFrontLength + RangeLeft.x;
	vecFront.z = cosf(Rot) * -fFrontLength + RangeLeft.z;

	//���[����A���[�̐��ʂ܂ł̃x�N�g��
	D3DXVECTOR3 vecFrontLine = vecFront - RangeLeft;

	//���[����ʒu�܂ł̃x�N�g��
	D3DXVECTOR3 vecPos = Pos - RangeLeft;

	//���݈ʒu�܂ł̌������A���E���x�N�g���̍����ɋ��Ȃ���Ώ������΂�
	if (vecFrontLine.x * vecPos.z - vecFrontLine.z * vecPos.x < 0.0f)
	{
		//�p�x�v�Z�������ʂ����@�܂��͍����ւ̊p�x���v�Z
		return atan2f(Pos - RangeLeft);
	}

	//�E�[�̐��ʈʒu
	vecFront.x = sinf(Rot) * -fFrontLength + RangeRight.x;
	vecFront.z = cosf(Rot) * -fFrontLength + RangeRight.z;

	//�E�[����A�E�[�̐��ʂ܂ł̃x�N�g��
	vecFrontLine = vecFront - RangeRight;

	//�E�[����ʒu�܂ł̃x�N�g��
	vecPos = Pos - RangeRight;

	//�͈͂̉E�����E�ɂ���ꍇ�A�E���ւ̊p�x����ԋ߂��p�x�Ƃ���
	if (vecFrontLine.x * vecPos.z - vecFrontLine.z * vecPos.x > 0.0f)
	{
		//�p�x�v�Z�������ʂ����@�܂��͍����ւ̊p�x���v�Z
		return atan2f(Pos - RangeRight);
	}

	//���E�̓����ɂ���̂ŋt�p�x����ԋ߂��p�x�Ƃ���
	return Inverse(Rot);
}

//========================
//��_�����߂鏈��
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
D3DXVECTOR3 CrossPoint(D3DXVECTOR3 pos, D3DXVECTOR3 VecMove, D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	/*���E���x�N�g��*/ const D3DXVECTOR3 VecLine = EndPos - StartPos;
	/*�Ώۈʒu����J�n���W�܂ł̃x�N�g��*/ const D3DXVECTOR3 VecPosStart = StartPos - pos;
	/*�I�[���W����Ώۈʒu�܂ł̃x�N�g��*/ const D3DXVECTOR3 VecPosEnd = pos - EndPos;

	//�ړ��x�N�g�������ɂ����A�I�n�̒��_�܂ł̎O�p�`�̖ʐς��v�Z����
	const float fAReaStart = ((VecPosStart.x * VecMove.z) - (VecPosStart.z * VecMove.x)) * HALF;
	const float fAReaEnd = ((VecPosEnd.x * VecMove.z) - (VecPosEnd.z * VecMove.x)) * HALF;

	//�Q�̎O�p�`�̍��v�l�����߂�
	const float fAreaAll = fAReaStart + fAReaEnd;

	//����Z�v�Z�ɂO�Ŋ���Ƃ����Ȃ��̂ŁA�ʒu��Ԃ�
	if (fAreaAll == 0.0f)	return pos;

	//�J�n���W���̎O�p�`�̖ʐς̊������v�Z����
	const float fRateStart = fAReaStart / fAreaAll;

	D3DXVECTOR3 CrossPos  = VEC3_INIT;

	/*��_�]�E�y���W���Z�o����*/
	CrossPos.x = StartPos.x + (VecLine.x * fRateStart);
	CrossPos.z = StartPos.z + (VecLine.z * fRateStart);

	//��_���W��Ԃ�
	return CrossPos;
}

//========================
//D3DXVECTOR3�^�̊p�x�v�Z
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
float atan2f(D3DXVECTOR3 v)
{
	//�p�x���v�Z���Ēl��Ԃ�
	return atan2f(v.x, v.z);
}

//========================
//D3DXVECTOR3�^�̊e�x�N�g���̐�Βl��Ԃ�
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
D3DXVECTOR3 fabsV3(D3DXVECTOR3 v)
{
	//�e�x�N�g���̐�Βl���擾���ĕԂ�
	return D3DXVECTOR3(fabsf(v.x),
					   fabsf(v.y),
					   fabsf(v.z));
}

//========================
//���͈͓̔��ɑΏە������邩�ǂ����{������
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
bool Search(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos, float fLeftAngle, float fRightAngle, float fLength)
{
	//���[�̈ʒu�i�ȉ��ϐ�����LRP�Ƃ���
	const D3DXVECTOR3 LeftRangePos = D3DXVECTOR3(sinf(fLeftAngle) * fLength + pos.x,	pos.y,
												 cosf(fLeftAngle) * fLength + pos.z);

	//�E�[�̈ʒu�i�ȉ��ϐ�����RRP�Ƃ���
	const D3DXVECTOR3 RightRangePos = D3DXVECTOR3(sinf(fRightAngle) * fLength + pos.x,	pos.y,
												  cosf(fRightAngle) * fLength + pos.z);

	//���E�͈̔͂̌��E���W�܂ł̃x�N�g��
	const D3DXVECTOR3 VecLRP = LeftRangePos - pos;
	const D3DXVECTOR3 VecRRP = RightRangePos - pos;

	//�ڕW�܂ł̃x�N�g���Ƌ���
	D3DXVECTOR3 VecTarget = TargetPos - pos;
	const float VecTargetLength = D3DXVec3Length(&VecTarget);

	//�ڕW�܂ł̋������͈͊O�Ȃ珈�����΂�
	if (fLength < VecTargetLength)	return false;

	//�͈͓��ɂ��邩�ǂ�������
	if (VecLRP.x * VecTarget.z - VecLRP.z * VecTarget.x <= 0.0f &&	//�͈͂̒��ɋ���
		VecRRP.x * VecTarget.z - VecRRP.z * VecTarget.x >= 0.0f)	return true;

	//�͈͂̊O�ɋ���
	return false;
}

//========================
//�����𒲂ׂ�
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
int Dejit(int nData)
{
	//������ۑ�
	int nDejit = 0;

	do
	{
		//�P�������Č�������
		nData /= 10;
		nDejit++;
	} while (nData > 0);

	//������Ԃ�
	return nDejit;
}

//========================
//�Ώە��̒��ɂ߂荞��ł��邩�ǂ�������
//�֐��̐����Ȃǂ̓w�b�_�t�@�C����
//========================
bool IsBoxCollision(D3DXVECTOR3 MinPos, D3DXVECTOR3 MaxPos, D3DXVECTOR3 TargetMinPos, D3DXVECTOR3 TargetMaxPos)
{
	//�߂荞��ł��邩����
	if (MinPos.x < TargetMaxPos.x && TargetMinPos.x < MaxPos.x &&
		MinPos.z < TargetMaxPos.z && TargetMinPos.z < MaxPos.z)return true;

	else return false;
}

//=========================
//���b�V���̎�ޕʒ��_���ƃC���f�b�N�X�����擾
//=========================
CMesh::Buffer GetMeshBuffer(int nColumn, int nRow)
{
	//���b�V���̊e������ۑ�
	CMesh::Buffer  Buffer = { 0, 0, 0 };

	//�d�Ȃ钸�_�̐�
	const int nMultiTop = (nColumn - 2) + (nRow - 2);

	//�P�s�̃|���S����
	const int nColumnPoly = (nColumn - 1) * 2;

	//���_���i��@�~�@�s
	Buffer.nNumVtx = nColumn * nRow;

	//�|���S�����i�P�s�̃|���S���@�~�@�i��-1�j�@�{�@�d�Ȃ钸�_�̐��@�~�@�P�̒��_�ɏd�Ȃ鐔
	Buffer.nNumPoly = nColumnPoly * (nRow - 1) + nMultiTop * 2;

	//�C���f�b�N�X���i�P�s�̃|���S���@�~�@��@�{�@//�d�Ȃ钸�_�̐�
	Buffer.nNumIdx = nColumnPoly * nRow + nMultiTop;

	//�o�b�t�@�[��Ԃ�
	return Buffer;
}

//=========================
//�V�����_�[�^�̃��b�V���o�b�t�@�[���擾
//=========================
CMesh::Buffer GetMeshBuffer(int nDevide)
{
	//���b�V���̊e������ۑ�
	CMesh::Buffer  Buffer = { 0, 0, 0 };

	//�o�b�t�@�[�v�Z�i�S�ē����H
	Buffer.nNumIdx = Buffer.nNumPoly = Buffer.nNumVtx = (nDevide + 1) * 2;

	//�o�b�t�@�[��Ԃ�
	return Buffer;
}

//=========================
//�����_���Ŋp�x���擾
//=========================
float RandomRotate(void)
{
	//�p�x��Ԃ�
	return (float)((rand() % FULL_ANGLE - FIX_ANGLE) * FIX_FLOAT);
}