/*==========================================================================================================================================================

�������ԏ���[timelimit.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _TIMELIMIT_H_
#define _TIMELIMIT_H_

#include "main.h"

//���C�t�̏��\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	int			nTime;			//�c�莞��
	int			nDejit;			//����
	bool		bSoundAlarm;	//�A���[�������Đ�������
	int			nFromAlarm;		//�A���[�������Đ����Ă���o�߂�������
	bool		bSoundSpeed;	//�T�E���h�X�s�[�h����
}TimeLimit;

//�v���g�^�C�v�錾
void InitTimeLimit(void);		void SetTimeLimit_Vertex(void);
void UninitTimeLimit(void);
void UpdateTimeLimit(void);		void TimeLimit_Sound_Change(void);		void Timelimit_TheEnd(void);
void DrawTimeLimit(void);
TimeLimit *GetTimeLimit(void);

#endif