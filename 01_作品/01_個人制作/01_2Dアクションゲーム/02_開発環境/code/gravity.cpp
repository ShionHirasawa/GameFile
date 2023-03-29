/*==========================================================================================================================

ゲーム背景の処理[bg.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "gravity.h"
#include "input.h"

//重力のマクロ関数
#define RIZE_ADD_GRAVITY	(5.0f)		//重力加速度
#define RIZE_MAGNIFICATION	(0.10f)		//飛び上がる時の重力加速度の倍率
#define FALL_MAGNIFICATION	(0.13f)		//落ちる時の重力加速度の倍率
#define CHANGE_GRAVITY		(0.0f)		//落ちる瞬間の移動速度
#define FALL_ADD_GRAVITY	(8.0f)		//落ちる時の重力加速度

void CalculationGravity(D3DXVECTOR3 *pMove)
{
	float AddGravity = RIZE_ADD_GRAVITY,				//重力加速度
		  GravityMagnification = RIZE_MAGNIFICATION;	//重力加速度の倍率

	//下に落ちている
	if (pMove->y > CHANGE_GRAVITY)
	{
		AddGravity = FALL_ADD_GRAVITY;				//重力加速度
		GravityMagnification = FALL_MAGNIFICATION;	//重力加速度の倍率
	}

	//重力の計算
	pMove->y += (AddGravity - pMove->y) * GravityMagnification;
}