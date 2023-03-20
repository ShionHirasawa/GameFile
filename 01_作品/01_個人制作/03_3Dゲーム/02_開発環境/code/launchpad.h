/*==========================================================================================================================================================

発射台の処理[launchpad.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _LAUNCHPAD_H_
#define _LAUNCHPAD_H_

#include "bullet.h"
#include "game.h"

//発射台の状態構造体
typedef enum
{
	LAUNCHPADSTATE_WAIT = 0,	//待機状態
	LAUNCHPADSTATE_FIRE,		//発射
	LAUNCHPADSTATE_MAX
}LAUNCHPADSTATE;

//発射台の情報構造体
typedef struct
{
	//発射台自体の変数
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	LAUNCHPADSTATE	state;			// 状態
	int				nCntWait;		// 待機時間
	int				nCntDelay;		// 遅延時間
	/*発射物に渡す変数*/
	D3DXVECTOR3		Setmove;	//移動量
}LaunchPad;

void InitLaunchPad(void);
void LoadLaunchPad(void);
void UninitLaunchPad(void);
void UpdateLaunchPad(void);
void DrawLaunchPad(void);

//------------------------------------
//		状態による分岐処理
//------------------------------------
void LaunchPad_Wait(int nCntLaunchPad);
void LaunchPad_Fire(int nCntLaunchPad);

#endif