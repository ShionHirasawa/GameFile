/*==========================================================================================================================================================

															��C�̏���[cannon.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _CANNON_H_
#define _CANNON_H_

#include "bullet.h"
#include "game.h"

//��C�̏�ԍ\����
typedef enum
{
	CANNONSTATE_NORMAL = ZERO,	// �ʏ�
	CANNONSTATE_RELOADING,		// ���ˏ�����
	CANNONSTATE_SHOOTING,		// �_���[�W���
	CANNONSTATE_MAX
}CANNONSTATE;

//���f���̍\����
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		MinPos;			// �ŏ��̈ʒu
	D3DXVECTOR3		MaxPos;			// �ő�̈ʒu
	D3DXVECTOR3		rot;			// ����
	bool			bRotate;		// ��]�������ǂ���[ 0 ���� : 1 ������ ]
	int				nStateCounter;	// ��Ԃ̃J�E���^
	CANNONSTATE		state;			// ���
}Cannon;

void InitCannon(void);
void LoadCannon(void);
void SearchCannon_MinAndMax_Pos(void);
void UninitCannon(void);
void UpdateCannon(void);
void DrawCannon(void);

void SwitchStateCannon(int nCntCannon);
void RotCannon(int nCntCannon);
bool CollisionCannon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType, bool *pLand, bool *pJump);
void ColliCannon_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType);
void ColliCannon_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType, bool *pLand, bool *pJump);
void ColliCannonShadow(int nIdxShadow, D3DXVECTOR3 pos, float *pDrawPosY);
bool ColliCannonToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner);

#endif