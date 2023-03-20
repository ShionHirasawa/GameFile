/*==========================================================================================================================

オブジェクトの移動処理[move.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "move.h"
#include "input.h"
#include "player.h"

#define DROW_PLAYER_POS	(500.0f)//プレイヤーと標準座標の限界距離

D3DXVECTOR3 StandardPos = ZERO_SET;

void InitMove(void)
{
	StandardPos = ZERO_SET;
}

void moveObject(D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 fMove = ZERO_SET;	//移動量
	Player *pPlayer = GetPlayer();	//プレイヤーの情報

	//プレイヤーがブロックに当たっていない
	if (pPlayer->bSideBlock == false)
	{
		if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_D) == true ||
			GetGamePadPress(XINPUT_GAMEPAD_DPAD_LEFT) == true || GetGamePadPress(XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{
			fMove.x = MINUS_ONE * (pPlayer->move.x);
		}
	}

	if (StandardPos.x > DROW_PLAYER_POS)
	{
		pPos->x += fMove.x;
	}

	StandardPos.x -= fMove.x;

}