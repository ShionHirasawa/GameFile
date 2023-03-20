/*==========================================================================================================================================================

丸太トゲの処理[logthorns.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _LOGTHORNS_H_
#define _LOGTHORNS_H_

#include "bullet.h"
#include "game.h"

//丸太トゲの情報構造体
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		posOld;			// 前回の位置
	D3DXVECTOR3		move;			// 移動量
	D3DXVECTOR3		rot;			// 向き
	D3DXVECTOR3		MinPos;			// 最小の位置
	D3DXVECTOR3		MaxPos;			// 最大の位置
	bool			bUse;			// 使用するかどうか
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