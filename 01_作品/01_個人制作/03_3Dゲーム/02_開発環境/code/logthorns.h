/*==========================================================================================================================================================

�ۑ��g�Q�̏���[logthorns.h]
Author:���V����

============================================================================================================================================================*/
#ifndef _LOGTHORNS_H_
#define _LOGTHORNS_H_

#include "bullet.h"
#include "game.h"

//�ۑ��g�Q�̏��\����
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		posOld;			// �O��̈ʒu
	D3DXVECTOR3		move;			// �ړ���
	D3DXVECTOR3		rot;			// ����
	D3DXVECTOR3		MinPos;			// �ŏ��̈ʒu
	D3DXVECTOR3		MaxPos;			// �ő�̈ʒu
	bool			bUse;			// �g�p���邩�ǂ���
}LogThorns;

void InitLogThorns(void);
void SearchLogThorns_MinAndMax_Pos(void);
void UninitLogThorns(void);
void UpdateLogThorns(void);
void DrawLogThorns(void);

void MoveLogthorns(int nCntLogThorns);
void RollLogThorns(int nCntLogThorns);
void WholeColliLogThorns(int nCntLogThorns);

bool CollisionLogThorns(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType, bool *pLand, bool *pJump);
void ColliLogThorns_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType);
void ColliLogThorns_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType, bool *pLand, bool *pJump);
bool ColliLogThornsToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner);

void SetLogThorns(D3DXVECTOR3 Setpos, D3DXVECTOR3 Setmove, D3DXVECTOR3 Setrot);

#endif