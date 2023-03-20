#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//�|�[�Y���j���[�̍\����
typedef enum
{
	PAUSE_MENU_BG = ZERO,		//�|�[�Y��ʂ̔w�i
	PAUSE_MENU_CONTINUE,		//�R���e�B�j���[(�|�[�Y��ʂ��I��)����
	PAUSE_MENU_RETRY,			//�Q�[������蒼��
	PAUSE_MENU_LOAD,			//�Q�[�����I������(���[�f�B���O��ʂɖ߂�)
	PAUSE_MENU_MAX
}PAUSE_MENU;

//�v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
PAUSE_MENU *GetPause(void);
void SetPause(PAUSE_MENU SetPause);

#endif