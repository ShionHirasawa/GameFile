/*==========================================================================================================================

�I�u�W�F�N�g�̈ړ�����[move.cpp]
Author:���V����

==========================================================================================================================*/
#include "move.h"
#include "input.h"
#include "player.h"

#define DROW_PLAYER_POS	(500.0f)//�v���C���[�ƕW�����W�̌��E����

D3DXVECTOR3 StandardPos = ZERO_SET;

void InitMove(void)
{
	StandardPos = ZERO_SET;
}

void moveObject(D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 fMove = ZERO_SET;	//�ړ���
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��

	//�v���C���[���u���b�N�ɓ������Ă��Ȃ�
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