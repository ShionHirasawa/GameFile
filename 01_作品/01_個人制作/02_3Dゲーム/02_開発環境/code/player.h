/*==========================================================================================================================================================

														�v���C���[�̏���[player.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "bullet.h"
#include "meshfield.h"

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_NORMAL = ZERO,	//�ʏ�
	PLAYERSTATE_DAMEGE,			//�_���[�W
	PLAYERSTATE_APPEAR,			//�o��
	PLAYERSTATE_DEAD,			//���S
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3		pos;					//�ʒu
	D3DXVECTOR3		posOld;					//�O��̈ʒu
	D3DXVECTOR3		move;					//�ړ���
	D3DXVECTOR3		rot;					//����
	PLAYERSTATE		state;					//�v���C���[�̏��
	MeshField		*pField;				//�t�B�[���h�̏��
	int				nStateCounter;			//��ԃJ�E���^�[
	int				nLife;					//�̗�
	bool			bLand;					//��������Ă��邩�ǂ���
	bool			bJump;					//�W�����v�����ǂ���
	bool			bDisp;					//�\�����邩�ǂ���
}Player;

void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

void SwitchPlayerState(void);
void MovePlayer(void);
void PlayerGravity(void);
void FixPlayerMove(void);
void InertiaPlayerMove(void);	// Inertia �́u�����v�Ƃ����Ӗ�
void FixPlayerPos(void);
void RotPlayer(void);
void ResetPlayer(void);
bool ColliPlayerToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner);
void WholePlayerCollision(void);// �v���C���[�̓����蔻��܂Ƃ� Whole �́u�S�́v�Ƃ����Ӗ�

#endif 