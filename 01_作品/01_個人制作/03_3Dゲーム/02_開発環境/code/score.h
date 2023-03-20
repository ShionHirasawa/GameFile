/*==========================================================================================================================================================

�X�R�A�\������[score.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

#define DEJIT_DIVISION			(10)								//���̈ʂ�������

//���C�t�̏��\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int			nScore;		//�X�R�A
	int			Dejit;		//��
}Score;

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void FixDejit(void);
Score *GetScore(void);

#endif