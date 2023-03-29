/*==========================================================================================================================================================

���ˑ�̏���[launchpad.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _LAUNCHPAD_H_
#define _LAUNCHPAD_H_

#include "bullet.h"
#include "game.h"

//���ˑ�̏�ԍ\����
typedef enum
{
	LAUNCHPADSTATE_WAIT = 0,	//�ҋ@���
	LAUNCHPADSTATE_FIRE,		//����
	LAUNCHPADSTATE_MAX
}LAUNCHPADSTATE;

//���ˑ�̏��\����
typedef struct
{
	//���ˑ䎩�̂̕ϐ�
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	LAUNCHPADSTATE	state;			// ���
	int				nCntWait;		// �ҋ@����
	int				nCntDelay;		// �x������
	/*���˕��ɓn���ϐ�*/
	D3DXVECTOR3		Setmove;	//�ړ���
}LaunchPad;

void InitLaunchPad(void);
void LoadLaunchPad(void);
void UninitLaunchPad(void);
void UpdateLaunchPad(void);
void DrawLaunchPad(void);

//------------------------------------
//		��Ԃɂ�镪�򏈗�
//------------------------------------
void LaunchPad_Wait(int nCntLaunchPad);
void LaunchPad_Fire(int nCntLaunchPad);

#endif