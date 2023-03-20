/*==========================================================================================================================

�Q�[���w�i�̏���[bg.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "gravity.h"
#include "input.h"

//�d�͂̃}�N���֐�
#define RIZE_ADD_GRAVITY	(5.0f)		//�d�͉����x
#define RIZE_MAGNIFICATION	(0.10f)		//��яオ�鎞�̏d�͉����x�̔{��
#define FALL_MAGNIFICATION	(0.13f)		//�����鎞�̏d�͉����x�̔{��
#define CHANGE_GRAVITY		(0.0f)		//������u�Ԃ̈ړ����x
#define FALL_ADD_GRAVITY	(8.0f)		//�����鎞�̏d�͉����x

void CalculationGravity(D3DXVECTOR3 *pMove)
{
	float AddGravity = RIZE_ADD_GRAVITY,				//�d�͉����x
		  GravityMagnification = RIZE_MAGNIFICATION;	//�d�͉����x�̔{��

	//���ɗ����Ă���
	if (pMove->y > CHANGE_GRAVITY)
	{
		AddGravity = FALL_ADD_GRAVITY;				//�d�͉����x
		GravityMagnification = FALL_MAGNIFICATION;	//�d�͉����x�̔{��
	}

	//�d�͂̌v�Z
	pMove->y += (AddGravity - pMove->y) * GravityMagnification;
}