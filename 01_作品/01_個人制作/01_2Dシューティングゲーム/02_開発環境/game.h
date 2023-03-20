#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "time.h"

//�}�N����`
#define END_FADE	(60)		//�I���]�C

//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,		//�������Ă��Ȃ����
	GAMESTATE_NORMAL,		//�ʏ���
	GAMESTATE_END,			//�I�����
	GAMESTATE_START,		//�Q�[���J�n
	GAMESTATE_MAX
}GAMESTATE;

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
int ResultChoice(void);
GAMESTATE GetGameState(void);

#endif