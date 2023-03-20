#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "block.h"

#define JUMP_MOVE			(-15.0f)	//�W�����v�̍���

//�v���C���[�̏�ԍ\����
typedef enum
{
	PLAYERSTATE_APPEAR = ZERO,	//�o����ԁi�_�Łj
	PLAYERSTATE_NORMAL,			//�ʏ���
	PLAYERSTATE_DELIVERY,		//�n�R�Y�ݏo����
	PLAYERSTATE_CARRY,			//�n�R���^�ђ�
	PLAYERSTATE_THROW,			//�n�R������
	PLAYERSTATE_GOAL,			//�S�[������
	PLAYERSTATE_DAMAGE,			//�_���[�W��
	PLAYERSTATE_FALL,			//������
	PLAYERSTATE_DEATH,			//���S
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̈ړ������\����
typedef enum
{
	PLAYER_ROT_FORWARD = ZERO,	//����(�ҋ@���[�V����)
	PLAYER_ROT_RIGHT,			//�ړ������@�E
	PLAYER_ROT_LEFT,			//�ړ������@��
	PLAYER_ROT_MAX
}PLAYER_ROT;

//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;			//���݂̈ʒu
	D3DXVECTOR3 posOld;			//�O��̈ʒu
	D3DXVECTOR3 move;			//�ړ���
	PLAYERSTATE state;			//�v���C���[�̏��
	int AppearCnt;				//�_�ŕ\�����Ԃ̃J�E���g
	PLAYER_ROT Rot;				//�v���C���[�̌���
	PLAYER_ROT OldRot;			//�v���C���[�̑O��̌���
	int nCounterAnim;			//�A�j���[�V�����J�E���^�[
	int nPatternAnim;			//�A�j���[�V�����p�^�[��No.
	int nCounterState;			//��ԊǗ��J�E���g
	int DeathCounter;			//���S�J�E���g
	float fWidth;				//���̒���(���S���獶�E�ɕ������̂Œ[����[�̔����Ɂj
	float fHeight;				//�����̒���
	bool bJump;					//�W�����v�����ǂ���
	bool bOnBlock;				//�u���b�N�ɏ�������ǂ���
	bool bOnHako;				//�n�R�ɏ�������ǂ���
	bool bSideBlock;			//�n�R�ɉ����瓖��������
	Block *pBlock;				//�Ώۃu���b�N�ւ̃|�C���^
	bool bDisp;					//�v���C���[��\��ON/OFF
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void FirstUpdatePlayer(void);
void MovePlayer(void);
void CollisionAndPos_x(void);
void CollisionAndPos_y(void);
void HitPlayer(PLAYERSTATE state);

#endif