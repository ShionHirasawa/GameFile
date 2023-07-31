#ifndef _PLAYERBLOCK_H_
#define _PLAYERBLOCK_H_

#include "block.h"

//���������ݏo�����u���b�N�̍\����
typedef enum
{
	HAKO_NONE = ZERO,	//���ݏo����Ă��Ȃ�
	HAKO_SPAWN,			//�u���b�N�𐶂ݏo��
	HAKO_CARRY,			//�����^�ׂ�
	HAKO_SET,			//����	
	HAKO_MAX
}HAKO;

//�Y�ݏo���ꂽ����
typedef enum
{
	DIRECTION_OVER  = ZERO,
	DIRECTION_UNDER ,
	DIRECTION_LEFT  ,
	DIRECTION_RIGHT ,
	DIRECTION_MAX
}DIRECTION;

//�n�R�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	float fWidth;			//��
	float fHeight;			//����
	HAKO myBlockState;		//������������u���b�N�̏��
	DIRECTION nDirect;		//����
	Block *pBlock;			//�Ώۃu���b�N�ւ̃|�C���^
	int nCol;				//�J���[
	bool bShade;			//�J���[�̐܂�Ԃ�
	bool bUse;				//�g�p����Ă��邩�ǂ���
}Hako;

//�v���g�^�C�v�錾
void InitHako(void);
void UninitHako(void);
void UpdateHako(void);
void DrawHako(void);
void SetHako(void);
void X_CollisionHako(void);
bool Y_CollisionHako(void);
Hako *GetHako(void);
void HakoMove(float fHakoMove);
void ConnectSide(D3DXVECTOR3 ParentPos);
void HakoSpawnSound(void);

//�X�V�������ōs������
void FadeHako(void);
void ResetHako(void);
void LiftUpHako(void);
void CarryHako(void);
void GravityHako(void);
void SwitchHako(int nSwitchNum);
void FixHakoCol(void);

#endif