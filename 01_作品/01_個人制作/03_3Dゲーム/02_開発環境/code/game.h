/*==========================================================================================================================================================

														�Q�[����ʏ���[game.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "time.h"

//�}�N����`
#define END_FADE		(60)		// �I���]�C
#define THE_END_HELL	(-400.0f)	// �ޗ��|�C���g

//�Q�[����ʂ����Ƃ��̗񋓌^
typedef enum
{
	EndGAME_CLEAR = ZERO,	//�Q�[���N���A
	EndGAME_OVER,			//�Q�[���I�[�o�[
	EndGAME_MAX,
}EndGAME;

//�����蔻��̎��
typedef enum
{
	COLLITYPE_PLAYER = ZERO,	// �v���C���[
	COLLITYPE_BULLET,			// �e
	COLLITYPE_ENEMY,			// �G
	COLLITYPE_MAX
}COLLITYPE;

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void PlayMode(void);
void SetEndGame(EndGAME end);
int EndGameResult(void);

#endif