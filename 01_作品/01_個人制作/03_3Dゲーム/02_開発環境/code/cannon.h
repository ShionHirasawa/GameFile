/*==========================================================================================================================================================

															大砲の処理[cannon.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _CANNON_H_
#define _CANNON_H_

#include "bullet.h"
#include "game.h"

//大砲の状態構造体
typedef enum
{
	CANNONSTATE_NORMAL = ZERO,	// 通常
	CANNONSTATE_RELOADING,		// 発射準備中
	CANNONSTATE_SHOOTING,		// ダメージ状態
	CANNONSTATE_MAX
}CANNONSTATE;

//モデルの構造体
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		MinPos;			// 最小の位置
	D3DXVECTOR3		MaxPos;			// 最大の位置
	D3DXVECTOR3		rot;			// 向き
	bool			bRotate;		// 回転したかどうか[ 0 正面 : 1 横向き ]
	int				nStateCounter;	// 状態のカウンタ
	CANNONSTATE		state;			// 状態
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