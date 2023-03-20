/*==========================================================================================================================================================

														�v���C���[�̏���[player.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "player.h"
#include "model.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "model.h"
#include "wall.h"
#include "slope.h"
#include "color.h"
#include "fade.h"
#include "meshfield.h"
#include "billboard.h"
#include "sound.h"
#include "game.h"
#include "cannon.h"

#define PLAYER_START_POS		(D3DXVECTOR3(-550.0f, 0.0f, -550.0f))
//#define PLAYER_START_POS		(D3DXVECTOR3(-0.0f, 180.0f, -400.0f))		//�f�o�b�O�p�����ʒu
#define PLAYER_TEX_MAX			(10)		//���f���̃e�N�X�`���̍ő吔
#define PLAYER_SPEED			(0.5f)		//�v���C���[�̈ړ��X�s�[�h
#define PLAYER_JUMP				(14.0f)		//�v���C���[�̃W�����v��
#define MAX_SPEED				(4.0f)		//�v���C���[�̌��E�ړ���
#define CROSS_SPEED_COR			(0.9f)		//�΂ߕ����ւ̈ړ��ʕ␳�l
#define PLAYER_LIFE				(5)			//�v���C���[�̗̑�
#define PLAYER_STATE_COUNTER	(90)		//�����Ԃ̎���
#define FLASH_PLAYER_COUNT		(3)			//�v���C���[���_�ł���Ԋu

#define PLAYER_SHADOW_SIZE		(10.0f)		//�v���C���[�̉e�̑傫��

#define PLAYER_ROT_CORRECT_MOVE	(0.3f)		//�v���C���[�̐i�s�����ւ̌����C���̕␳�l
#define ERROR_PI				(3.14)		//�ړ������ɂ�����s�K�؂ȋ���������p�x
#define PLAYER_ERROR_ROT_COR	(0.01f)		//�ړ������ŃG���[�ɂȂ������̉��Z�␳�l

#define LIMIT_PLAYER_LEFT		(-600.0f)	//�v���C���[�̈ړ����E�n�_�@��
#define LIMIT_PLAYER_RIGHT		(600.0f)	//�v���C���[�̈ړ����E�n�_�@�E
#define LIMIT_PLAYER_CEELING	(100.0f)	//�v���C���[�̈ړ����E�n�_�@��
#define LIMIT_PLAYER_FLOAR		(0.0f)		//�v���C���[�̈ړ����E�n�_�@��
#define LIMIT_PLAYER_FRONT		(600.0f)	//�v���C���[�̈ړ����E�n�_�@��O
#define LIMIT_PLAYER_BACK		(-600.0f)	//�v���C���[�̈ړ����E�n�_�@��

//�v���C���[�̏d�̓}�N��
//�㏸
#define PLAYER_RIZE_GRAVITY			(-3.0f)		//�v���C���[�̏㏸���ɂ�����d��
#define PLAYER_RIZE_GRA_MAG			(0.2f)		//�v���C���[�̏㏸���ɂ�����d�͂̔{��
//���~
#define PLAYER_FALL_GRAVITY			(-5.0f)		//�v���C���[�̉��~���ɂ�����d��
#define PLAYER_FALL_GRA_MAG			(0.4f)		//�v���C���[�̉��~���ɂ�����d�͂̔{��

//�v���C���[�̒e�̔��˂ɂ�����}�N��
#define BULLET_COLOR					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0)	//�e�̐F
#define PLAYER_BULLET_SIZE				(4.0f)								//�e�̃T�C�Y
#define PLAYER_BULLET_SPEED				(-5.0f)								//�e�̔��ˑ��x
#define PLAYER_BULLET_CORRECT_HEIGHT	(9.0f)								//�e�̔��ˈʒu�̕␳�l

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9			g_pTexturePlayer[PLAYER_TEX_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
LPD3DXMESH					g_pMeshPlayer = NULL;					//���b�V��(���_���)�ւ̃|�C���^
D3DXMATRIX					g_mtxWorldPlayer;						//���[���h�}�g���b�N�X
LPD3DXBUFFER				g_pBuffMatPlayer = NULL;				//�}�e���A���ւ̃|�C���^
DWORD						g_dwNumMatPlayer = ZERO;				//�}�e���A���̐�
Player						g_player;								//�v���C���[�̏��

//--------------------------------------------------------
//					�v���C���[�̏���������
//--------------------------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	g_pMeshPlayer = NULL;							//���b�V��������
	g_pBuffMatPlayer = NULL;						//�}�e���A���ւ̃|�C���^������
	g_dwNumMatPlayer = ZERO;						//�}�e���A���̐�������
	ResetPlayer();									//�v���C���[�̏�񏉊���
	g_player.state = PLAYERSTATE_APPEAR;			//�o����Ԃɐݒ�
	g_player.nStateCounter = PLAYER_STATE_COUNTER;	//��ԃJ�E���^�[��ݒ�

	//X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/floating_car.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatPlayer, NULL, &g_dwNumMatPlayer, &g_pMeshPlayer);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntPlayer = START_CNT; nCntPlayer < (int)g_dwNumMatPlayer; nCntPlayer++)
	{
		if (pMat[nCntPlayer].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntPlayer].pTextureFilename, &g_pTexturePlayer[nCntPlayer]);
		}
	}
}

//--------------------------------------------------------
//					�v���C���[�̏I������
//--------------------------------------------------------
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	for (int nCntPlayer = START_CNT; nCntPlayer < PLAYER_TEX_MAX; nCntPlayer++)
	{
		if (g_pTexturePlayer[nCntPlayer] != NULL)
		{
			g_pTexturePlayer[nCntPlayer]->Release();
			g_pTexturePlayer[nCntPlayer] = NULL;
		}
	}

	//���b�V���̔j��
	if (g_pMeshPlayer != NULL)
	{
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatPlayer != NULL)
	{
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//--------------------------------------------------------
//					�v���C���[�̍X�V����
//--------------------------------------------------------
void UpdatePlayer(void)
{
	//�O��̈ʒu�X�V
	g_player.posOld = g_player.pos;

	//�v���C���[�̏�Ԃɂ�镪�򏈗�
	SwitchPlayerState();

	//�v���C���[�����S���Ă��Ȃ�
	if (g_player.state != PLAYERSTATE_DEAD)
	{
		//�v���C���[�̈ړ�����
		MovePlayer();

		//�v���C���[�̉�]����
		RotPlayer();

		//�ʒu�X�V
		g_player.pos += g_player.move;

		//�v���C���[�̏d�͏���
		PlayerGravity();

		//�v���C���[�̈ʒu�C��
		FixPlayerPos();

		//�v���C���[�Ƒ��ɑ΂��铖���蔻��܂Ƃ�
		WholePlayerCollision();

		//�e����
		if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_B)) == true)
		{
			//�e�̈ړ������E��
			D3DXVECTOR3 BulletMove;

			BulletMove.x = sinf(g_player.rot.y) * PLAYER_BULLET_SPEED;
			BulletMove.z = cosf(g_player.rot.y) * PLAYER_BULLET_SPEED;
			BulletMove.y = sinf(-g_player.rot.x) * PLAYER_BULLET_SPEED;

			SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + PLAYER_BULLET_CORRECT_HEIGHT, g_player.pos.z), BulletMove, BULLET_COLOR, PLAYER_BULLET_SIZE, BULLET_OWNER_PLAYER);
		}
	}
	
#if _DEBUG
	//�v���C���[�̃��Z�b�g����
	if (GetKeyboardTrigger(DIK_N) == true)
	{
		ResetPlayer();
	}
#endif
}

//�v���C���[�̏�Ԃɂ�镪�򏈗�
void SwitchPlayerState(void)
{
	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:	//�ʏ���
		break;

	case PLAYERSTATE_DAMEGE:	//�_���[�W���
	case PLAYERSTATE_APPEAR:	//�o�����
		//��ԃJ�E���^�[����
		g_player.nStateCounter--;

		//�J�E���^�[���[���ɂȂ���
		if (g_player.nStateCounter <= ZERO)
		{
			g_player.bDisp = true;							//�\������
			g_player.state = PLAYERSTATE_NORMAL;			//�ʏ��Ԃ�
			g_player.nStateCounter = PLAYER_STATE_COUNTER;	//�J�E���^�[�������l�ɖ߂�
		}

		//��Ԍp����
		else if(g_player.nStateCounter % FLASH_PLAYER_COUNT == ZERO)
		{
			//�v���C���[��_�ł�����
			g_player.bDisp = g_player.bDisp ? false : true;
		}

		break;

	case PLAYERSTATE_DEAD:		//���S���
		//�\�����Ȃ�
		g_player.bDisp = false;	

		//��ԃJ�E���^�[����
		g_player.nStateCounter--;

		//�J�E���^�[���[���ɂȂ���
		if (g_player.nStateCounter <= ZERO)
		{
			//���U���g��ʂֈڍs
			SetFade(MODE_RESULT);
		}
		break;
	}
}

//�v���C���[�̈ړ�����
void MovePlayer(void)
{
	float fCameraRot = GetCamera()->rot.y;	//�J�����̊p�x���
	float fMoveRotcor = F_ZERO;				//
	float fPi = F_ZERO;						//�i�s�����̊p�x����
	bool bMoveRot = false;					//�i�s�����Ɍ������邩�ǂ���

	//���Ɉړ��E�����]��
	if ((GetKeyboardPress(DIK_A) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_LEFT)) == true)
	{
		//�����Ɉړ�
		if ((GetKeyboardPress(DIK_W) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_UP)) == true)
		{
			g_player.move.x += sinf(LEFT_DOWN - fCameraRot) * PLAYER_SPEED;	//X�����Ɉړ�
			g_player.move.z += cosf(LEFT_DOWN - fCameraRot) * PLAYER_SPEED;	//Z�����Ɉړ�

			fPi = RIGHT_UP;		//�E�����̊p�x����
		}
		
		//����O�Ɉړ�
		else if ((GetKeyboardPress(DIK_S) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN)) == true)
		{
			g_player.move.x += sinf(LEFT_UP  - fCameraRot) * PLAYER_SPEED;	//X�����Ɉړ�
			g_player.move.z += cosf(LEFT_UP - fCameraRot) * PLAYER_SPEED;	//Z�����Ɉړ�

			fPi = RIGHT_DOWN;		//�E�����̊p�x����
		}

		//���Ɉړ�
		else
		{
			g_player.move.x += sinf(LEFT - fCameraRot) * PLAYER_SPEED;	//X�����Ɉړ�
			g_player.move.z += cosf(LEFT - fCameraRot) * PLAYER_SPEED;	//Z�����Ɉړ�

			fPi = RIGHT;		//�E�����̊p�x����
		}
		
		bMoveRot = true;	//�i�s�����Ɍ�������
	}

	//�E�Ɉړ��E�����]��
	else if ((GetKeyboardPress(DIK_D) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_RIGHT)) == true)
	{
		//�����Ɉړ�
		if ((GetKeyboardPress(DIK_W) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_UP)) == true)
		{
			g_player.move.x += sinf(RIGHT_DOWN - fCameraRot) * PLAYER_SPEED;	//X�����Ɉړ�
			g_player.move.z += cosf(RIGHT_DOWN - fCameraRot) * PLAYER_SPEED;	//Z�����Ɉړ�

			fPi = LEFT_UP;		//�������̊p�x����
		}

		//����O�Ɉړ�
		else if ((GetKeyboardPress(DIK_S) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN)) == true)
		{
			g_player.move.x += sinf(RIGHT_UP - fCameraRot) * PLAYER_SPEED;	//X�����Ɉړ�
			g_player.move.z += cosf(RIGHT_UP - fCameraRot) * PLAYER_SPEED;	//Z�����Ɉړ�

			fPi = LEFT_DOWN;		//�������̊p�x����
		}

		//���Ɉړ�
		else
		{
			g_player.move.x += sinf(RIGHT + fCameraRot) * PLAYER_SPEED;
			g_player.move.z += cosf(RIGHT - fCameraRot) * PLAYER_SPEED;

			fPi = LEFT;		//�������̊p�x����
		}

		bMoveRot = true;	//�i�s�����Ɍ�������
	}

	//���Ɉړ��E�����]��
	else if ((GetKeyboardPress(DIK_W) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_UP)) == true)
	{
		g_player.move.x += sinf(DOWN - fCameraRot) * PLAYER_SPEED;
		g_player.move.z += cosf(DOWN - fCameraRot) * PLAYER_SPEED;

		float f = sinf((DOWN - fCameraRot) - g_player.rot.y);

		fPi += UP;			//������̊p�x����
		bMoveRot = true;	//�i�s�����Ɍ�������
	}

	//��O�Ɉړ��E�����]��
	else if ((GetKeyboardPress(DIK_S) || GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN)) == true)
	{
		//�ړ�
		g_player.move.x += sinf(UP - fCameraRot) * PLAYER_SPEED;
		g_player.move.z += cosf(UP - fCameraRot) * PLAYER_SPEED;

		fPi += DOWN;			//�������̊p�x����
		bMoveRot = true;	//�i�s�����Ɍ�������
	}

	//�㏸
	if ((GetKeyboardTrigger(DIK_SPACE) || GetGamePadPress(XINPUT_GAMEPAD_A)) == true && g_player.bLand == true && g_player.bJump == false)
	{
		//g_player.move.y = PLAYER_JUMP;	//�v���C���[�̃W�����v�ʂ�ݒ�
		g_player.bLand = false;
		g_player.bJump = true;
	}

	//R�L�[��������Ă�����������Œ肷�� ���� �i�s�����Ɍ������ėǂ�
	if (GetKeyboardPress(DIK_R) == false && bMoveRot == true)
	{
		float fCorrectPlus = F_ZERO;	//�����̉��Z�␳�l

		//�G���[�̊p�x���Z�o���ꂽ
		if (ERROR_PI <= fabsf((fPi - fCameraRot) - g_player.rot.y))
		{
			fCorrectPlus += PLAYER_ERROR_ROT_COR;
		}

		g_player.rot.y += sinf((fPi - fCameraRot) - g_player.rot.y) * PLAYER_ROT_CORRECT_MOVE + fCorrectPlus;	//�ړ������Ɍ�������
	}

	//�v���C���[�̈ړ��ʏC��
	FixPlayerMove();

	//�v���C���[�̈ړ���������
	InertiaPlayerMove();
}

//�v���C���[�̏d�͏���
void PlayerGravity(void)
{
	//�v���C���[���㏸��
	if (g_player.move.y > F_ZERO)
	{
		//���X�ɏ㏸�X�s�[�h�𗎂Ƃ�
		g_player.move.y += (PLAYER_RIZE_GRAVITY - g_player.move.y) * PLAYER_RIZE_GRA_MAG;
	}

	//�v���C���[�����~��
	else
	{
		//���X�ɏ㏸�X�s�[�h�𗎂Ƃ�
		g_player.move.y += (PLAYER_FALL_GRAVITY - g_player.move.y) * PLAYER_FALL_GRA_MAG;
	}
}

//�v���C���[�̈ړ��ʏC��
void FixPlayerMove(void)
{
	//�v���C���[�̈ړ��ʂ�2.0f�ȏ�ɂȂ���
	if (g_player.move.x >= MAX_SPEED)
	{
		g_player.move.x = MAX_SPEED;
	}

	//�v���C���[�̈ړ��ʂ� -2.0f�ȉ��ɂȂ���
	if (g_player.move.x <= -MAX_SPEED)
	{
		g_player.move.x = -MAX_SPEED;
	}

	//�v���C���[�̈ړ��ʂ�2.0f�ȏ�ɂȂ���
	if (g_player.move.z >= MAX_SPEED)
	{
		g_player.move.z = MAX_SPEED;
	}

	//�v���C���[�̈ړ��ʂ� -2.0f�ȉ��ɂȂ���
	if (g_player.move.z <= -MAX_SPEED)
	{
		g_player.move.z = -MAX_SPEED;
	}
}

//�v���C���[�̈ړ���������
void InertiaPlayerMove(void)
{
	g_player.move.x += (F_ZERO - g_player.move.x) * 0.1f;
	g_player.move.z += (F_ZERO - g_player.move.z) * 0.1f;
}

//�v���C���[�̈ʒu�C��
void FixPlayerPos(void)
{
	//�u�ړ����E�n�_�@���v�𒴂���
	if (g_player.pos.x < LIMIT_PLAYER_LEFT)
	{
		g_player.pos.x = LIMIT_PLAYER_LEFT;
	}

	//�u�ړ����E�n�_�@�E�v�𒴂���
	if (g_player.pos.x > LIMIT_PLAYER_RIGHT)
	{
		g_player.pos.x = LIMIT_PLAYER_RIGHT;
	}

	//�u�ړ����E�n�_�@��O�v�𒴂���
	if (g_player.pos.z > LIMIT_PLAYER_FRONT)
	{
		g_player.pos.z = LIMIT_PLAYER_FRONT;
	}

	//�u�ړ����E�n�_�@���v�𒴂���
	if (g_player.pos.z < LIMIT_PLAYER_BACK)
	{
		g_player.pos.z = LIMIT_PLAYER_BACK;
	}
}

//�v���C���[�̉�]����
void RotPlayer(void)
{
	//������
	if ((GetKeyboardPress(DIK_LSHIFT) || GetGamePadPress(XINPUT_GAMEPAD_LEFT_THUMB)) == true)
	{
		g_player.rot.y += sinf(LEFT) * 0.1f;
	}

	//�E����
	if ((GetKeyboardPress(DIK_RSHIFT) || GetGamePadPress(XINPUT_GAMEPAD_RIGHT_THUMB)) == true)
	{
		g_player.rot.y += sinf(RIGHT) * 0.1f;
	}

	/*==============================================
						�p�x�C��
	================================================*/
	//--------------------------------
	//		�p�x��3.14��������
	//--------------------------------
	// X �̊p�x����
	if (g_player.rot.x >= D3DX_PI)
	{
		g_player.rot.x -= D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	// Y �̊p�x����
	if (g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	// Z �̊p�x����
	if (g_player.rot.z >= D3DX_PI)
	{
		g_player.rot.z -= D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	//---------------------------------
	//		�p�x��-3.14���������
	//---------------------------------
	// X �̊p�x����
	if (g_player.rot.x <= -D3DX_PI)
	{
		g_player.rot.x += D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	// Y �̊p�x����
	if (g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}

	// Z �̊p�x����
	if (g_player.rot.z <= -D3DX_PI)
	{
		g_player.rot.z += D3DX_PI * F_DOUBLE;		//�p�x���P�������炷
	}
}

//�v���C���[�̃��Z�b�g����
void ResetPlayer(void)
{
	g_player.pos			= PLAYER_START_POS;		// �ʒu������
	g_player.posOld			= ZERO_SET;				// �O��̈ʒu������
	g_player.move			= ZERO_SET;				// �ړ��ʏ�����
	g_player.rot			= ZERO_SET;				// ����������
	g_player.state			= PLAYERSTATE_NORMAL;	// �v���C���[�̏�ԏ�����
	g_player.pField			= NULL;					// �t�B�[���h�̏��擾
	g_player.nStateCounter	= ZERO;					// ��ԃJ�E���^�[������
	g_player.nLife			= PLAYER_LIFE;			// �v���C���[�̗̑͏�����
	g_player.bLand			= true;					// ���n���Ă���
	g_player.bJump			= false;				// �W�����v���Ă��Ȃ�
	g_player.bDisp			= true;					// �\������
}

//�S�̂̓����蔻�菈��
void WholePlayerCollision(void)
{
	//�����蔻��ɂ���ăW�����v�ł��邩
	if ((CollisionSlope(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.rot, COLLITYPE_PLAYER) ||	//�⓹�̓����蔻�菈��
		ColliMeshField(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.pField)) == true)	//���b�V���t�B�[���h�Ƃ̓����蔻��
	{
		g_player.bLand = true;
		g_player.bJump = false;
	}

	else
	{
		g_player.bLand = false;
		g_player.bJump = true;
	}

	//�⓹�̕ǂƂ̓����蔻��

	//�ǂƂ̓����蔻��
	CollisionWall(&g_player.pos, &g_player.posOld, &g_player.move);
	
	//���f���Ƃ̓����蔻��
	CollisionModel(&g_player.pos, &g_player.posOld, &g_player.move, COLLITYPE_PLAYER);

	//��C�̓����蔻��
	CollisionCannon(&g_player.pos, &g_player.posOld, &g_player.move, COLLITYPE_PLAYER, &g_player.bLand, &g_player.bJump);
}

//--------------------------------------------------------
//					�v���C���[�̕`�揈��
//--------------------------------------------------------
void DrawPlayer(void)
{
	//�v���C���[�̕\����	ON
	if (g_player.bDisp == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
		
		/*------------------------------------------------------------------
								�v���C���[�̕`��ݒ�
		--------------------------------------------------------------------*/
		D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
		D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
		D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
		D3DXCOLOR pMatStrage;						//�}�e���A���f�[�^�ۑ��p
		BillTest *pBill = GetAlphaTest();			//�r���{�[�h�̃��EZ�e�X�g�����擾

													//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldPlayer);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);

		D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);

		D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
		{
			//�ۑ��p�X�g���[�W�ɑ��
			pMatStrage = pMat[nCntMat].MatD3D.Diffuse;

			//�_���[�W��Ԃł���
			if (g_player.state == PLAYERSTATE_DAMEGE)
			{
				//�ԐF�ɐݒ�
				pMat[nCntMat].MatD3D.Diffuse = XCOL_RED;
			}

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTexturePlayer[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshPlayer->DrawSubset(nCntMat);

			//�F��߂�
			pMat[nCntMat].MatD3D.Diffuse = pMatStrage;
		}

		//�}�e���A����߂�
		pDevice->SetMaterial(&matDef);

		/*------------------------------------------------------------------
									�e�̕`��ݒ�
		--------------------------------------------------------------------*/
		D3DXVECTOR3 pos = ZERO_SET;

		//�t�B�[���h�̃A�h���X�������Ă���
		if (g_player.pField != NULL)
		{
			//�����Ă���t�B�[���h�̏��̈ʒu���A��ɂ���
			if (g_player.pField->pos.y <= g_player.pos.y)
			{
				pos = g_player.pField->pos;
			}

			//�t�B�[���h��艺�ɋ�����A�A�h���X�������āA�e��`�悵�Ȃ�
			else
			{
				g_player.pField = NULL;		//�A�h���X������
				return;						//�e��`�悹���ɏI������
			}
		}

		else
		{
			return;
		}

		D3DXMATRIX	mtxShadow;		//�V���h�E�}�g���b�N�X
		D3DLIGHT9	light;			//���C�g���
		D3DXVECTOR4	posLight;		//���C�g�̈ʒu
		D3DXVECTOR3 normal;			//�@���x�N�g��
		D3DXPLANE	plane;			//���ʏ��

		//���C�g�̈ʒu��ݒ�
		pDevice->GetLight(0, &light);
		posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

		normal = SET_NOR;
		D3DXPlaneFromPointNormal(&plane, &pos, &normal);

		//�V���h�E�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxShadow);

		//�V���h�E�}�g���b�N�X�̍쐬
		D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		D3DXMatrixMultiply(&mtxShadow, &g_mtxWorldPlayer, &mtxShadow);

		//�V���h�E�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
		{
			//�ۑ��p�X�g���[�W�ɑ��
			pMatStrage = pMat[nCntMat].MatD3D.Diffuse;

			//�ԐF�ɐݒ�
			pMat[nCntMat].MatD3D.Diffuse = XCOL_BLACK;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTexturePlayer[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshPlayer->DrawSubset(nCntMat);

			//�F��߂�
			pMat[nCntMat].MatD3D.Diffuse = pMatStrage;
		}

		//�}�e���A����߂�
		pDevice->SetMaterial(&matDef);

	}
}

//�v���C���[�̏��
Player *GetPlayer(void)
{
	return &g_player;
}

//�v���C���[�ƒe�̓����蔻��
bool ColliPlayerToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner)
{
	D3DXVECTOR3 posDiff = g_player.pos - pos;	//�v���C���[�ƒe�̋����̍�
	bool bCollision = false;					//���������ǂ����̔���

	//�v���C���[�ƒe�Ƃ̋������e�̃T�C�Y���Ɏ��܂�
	if (-size <= posDiff.x && posDiff.x <= size &&
		-size <= posDiff.y && posDiff.y <= size &&
		-size <= posDiff.z && posDiff.z <= size &&
		ZERO < g_player.nLife && owner == BULLET_OWNER_CANNON &&	//�v���C���[�̗̑͂��c���Ă���	����	�e����C��������ꂽ����
		g_player.state != PLAYERSTATE_DAMEGE)						//�v���C���[���_���[�W��Ԃł͂Ȃ�
	{
		g_player.nLife--;			//�̗͂����炷

		//�̗͂��s���Ă��܂���
		if (g_player.nLife <= ZERO)
		{
			//���S��Ԃ�
			g_player.state = PLAYERSTATE_DEAD;

			//�Q�[���I�[�o�[
			SetEndGame(EndGAME_OVER);
		}

		//�܂��̗͂�����
		else
		{
			//�_���[�W��Ԃ�
			g_player.state = PLAYERSTATE_DAMEGE;
		}

		//�q�b�g���Đ�
		PlaySound(SOUND_LABEL_SE_HIT);

		bCollision = true;			//��������
	}

	//�����������ǂ����̔����Ԃ�
	return bCollision;
}