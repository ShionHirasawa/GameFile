/*==========================================================================================================================================================

														�|�[�Y��ʏ���[pause.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//�|�[�Y���j���[�̍\����
typedef enum
{
	PAUSE_MENU_BG = ZERO,		//�|�[�Y��ʂ̔w�i
	PAUSE_MENU_CONTINUE,		//�R���e�B�j���[(�|�[�Y��ʂ��I��)����
	PAUSE_MENU_RETRY,			//�Q�[������蒼��
	PAUSE_MENU_TITLE,			//�Q�[�����I������(���[�f�B���O��ʂɖ߂�)
	PAUSE_MENU_MAX
}PAUSE_MENU;

//�v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SelectPauseMenu_Down(void);	// �|�[�Y���j���[�̉��̍��ڂɃJ�[�\�������킹��
void SelectPauseMenu_UP(void);		// �|�[�Y���j���[�̏�̍��ڂɃJ�[�\�������킹��
void SelectPauseMenu_Decide(void);	// �|�[�Y���j���[�̍��ڂ�����
PAUSE_MENU *GetPause(void);
void SetPause(PAUSE_MENU SetPause);
bool GetDispPause(void);

#endif