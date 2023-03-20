/*==========================================================================================================================

�v���C���[�̏���[player.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "player.h"
#include "particle.h"
#include "fade.h"
#include "hako.h"
#include "input.h"
#include "gravity.h"

//�}�N����`
//�v���C���[�̏��}�N��
#define PLAYER_POS				D3DXVECTOR3(300.0f, 0.0f, F_ZERO)
#define PLAYER_MOVE				D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO)
#define PLAYER_TEX				"data/TEXTURE/�v���C���[�f�[�^.png"
#define MOVE_MAX_TEX			(10)		//�������[�V�����̃e�N�X�`���ő吔
#define PLAYER_WIDTH			(35.0f)		//�v���C���[�̕�(���S���獶�E�ɐL�т�̂Œ[����[�̔���)
#define PLAYER_HEIGHT			(75.0f)		//�v���C���[�̍���
#define COLLISION_PLAYER		(5.0f)		//�v���C���[�̓����蔻�蒲���}�N�� ��
#define COLLISION_HEIGHT		(40.0f)		//�v���C���[�̓����蔻�蒲���}�N�� �c
#define COLLISION_SIZE			(1.31f)		//�����蔻��̔{��
#define FALL_DEATH_POS			(720.0f)	//���������̃p�[�e�B�N������������Y���W
#define BIRTH_PARTICLE_PLAYER	(20)		//�p�[�e�B�N���̔�����

//���̑��̃}�N��
#define MOVE_SPEED				(0.8f)		//�ړ���
#define DEATH_TIME				(60)		//���S�̎���
#define APPEARCNT				(200)		//�o�����o����
#define MOVE_LIMIT_R			(800.0f)	//�v���C���[�̉�ʒ[�ړ����E�@�E
#define MOVE_LIMIT_L			(0.0f)		//�v���C���[�̉�ʒ[�ړ����E�@��
#define MOVE_LIMIT_CEILING		(0.0f)		//�v���C���[�̉�ʒ[�ړ����E�@�V��
#define MOVE_LIMIT_FLOOR		(800.0f)	//�v���C���[�̉�ʒ[�ړ����E�@��

//�A�j���[�V�����̍\����
typedef enum
{
	PLAYER_ANIM_NORMAL = ZERO,	//�ʏ펞
	PLAYER_ANIM_JUMP,			//�W�����v
	PLAYER_ANIM_DELIVERY,		//�Y�ݏo��
	PLAYER_ANIM_SLEEP,			//����
	PLAYER_ANIM_SIT,			//���� or �ڊo��
	PLAYER_ANIM_HAPPY,			//������
	PLAYER_ANIM_DAMAGE,			//�_���[�W��
}PLAYERANIM;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTexturePlayer;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Player g_player;

//�v���C���[�̏���������
void InitPlayer(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���C���[���̏�����
	g_player.pos = PLAYER_POS;					//���݂̈ʒu������������
	g_player.posOld = PLAYER_POS;				//�O��̈ʒu��������
	g_player.move = PLAYER_MOVE;				//�ړ��ʂ�����������
	g_player.state = PLAYERSTATE_APPEAR;		//��ԏ�����
	g_player.AppearCnt = APPEARCNT;				//�_�ŕ\�����ԃJ�E���^������
	g_player.Rot = PLAYER_ROT_FORWARD;			//�v���C���[�̌���������(����
	g_player.OldRot = PLAYER_ROT_FORWARD;		//�v���C���[�̑O��̌���������(����
	g_player.nCounterAnim = ZERO;				//�A�j���[�V�����J�E���^�[������
	g_player.nPatternAnim = PLAYER_ANIM_SLEEP;	//�A�j���[�V�����p�^�[��������
	g_player.nCounterState = ZERO;				//��ԊǗ��J�E���^������
	g_player.DeathCounter = ZERO;				//���S�J�E���g
	g_player.bJump = false;						//�W�����v�����Ă��Ȃ���Ԃ�
	g_player.bOnBlock = false;					//�u���b�N�ɏ���Ă��Ȃ�
	g_player.bOnHako = false;					//�n�R�ɏ���Ă��Ȃ�
	g_player.bSideBlock = false;				//�u���b�N�ɂԂ����Ă��Ȃ�
	g_player.bDisp = true;						//�\��ON/OFF(�ŏ���ON)
	g_player.fWidth = PLAYER_WIDTH;				//�v���C���[�̕���ݒ肷��
	g_player.fHeight = PLAYER_HEIGHT;			//�v���C���[�̍�����ݒ肷��
	g_player.pBlock = NULL;						//�Ώۃu���b�N�ւ̃A�h���X��NULL	

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &g_pTexturePlayer);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		/*pVtx[ZERO].pos.x = g_player.pos.x + sinf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ZERO].pos.y = g_player.pos.y + cosf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ZERO].pos.z = ORIGIN_POS;
		pVtx[ONE].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.z = ORIGIN_POS;
		pVtx[SEC].pos.x = g_player.pos.x + sinf(g_player.rot.z + (ORIGIN_POS - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.y = g_player.pos.y + cosf(g_player.rot.z + (ORIGIN_POS - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.z = ORIGIN_POS;
		pVtx[SAN].pos.x = g_player.pos.x + sinf(g_player.rot.z + (ORIGIN_POS + g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.y = g_player.pos.y + cosf(g_player.rot.z + (ORIGIN_POS + g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.z = ORIGIN_POS;*/

		pVtx[ZERO].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y - g_player.fHeight, ORIGIN_POS);
		pVtx[ONE].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth,  g_player.pos.y - g_player.fHeight, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth,  g_player.pos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth,  g_player.pos.y, ORIGIN_POS);
	}

	//rhw�̐ݒ�
	{
		pVtx[ZERO].rhw = RHW_NUM;
		pVtx[ONE].rhw = RHW_NUM;
		pVtx[SEC].rhw = RHW_NUM;
		pVtx[SAN].rhw = RHW_NUM;
	}

	//���_�J���[�̐ݒ�
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
	}

	//�e�N�X�`���̍��W
	{
		pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		pVtx[ONE].tex = D3DXVECTOR2(0.1f, ORIGIN_POS);
		pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, 0.25f);
		pVtx[SAN].tex = D3DXVECTOR2(0.1f, 0.25f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//�v���C���[�̏I������
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//�v���C���[�̍X�V�ōŏ��ɍs������
void FirstUpdatePlayer(void)
{
	g_player.posOld = g_player.pos;		//�O��̈ʒu���L��
	g_player.bOnBlock = false;			//�u���b�N�ɏ���Ă��Ȃ�
	g_player.bOnHako = false;			//�n�R�ɏ���Ă��Ȃ�
}

//�v���C���[�̍X�V����
void UpdatePlayer(void)
{
	//�ŏ��ɍs���X�V����
	FirstUpdatePlayer();

	if (g_player.pBlock != NULL)
	{//�u���b�N���ړ������������v���C���[�ɉ��Z����
		g_player.move.x += (g_player.pBlock->pos.x - g_player.pBlock->posOld.x) * 0.1f;
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�v���C���[�̈ړ�����
	MovePlayer();

	//�W�����v������Ȃ��Ƃ��Ƀn�R�𐶂ݏo������A�W�����v������
	if (g_player.bJump == false)
	{
		//�n�R�𐶂ݏo��
		if ((GetKeyboardPress(DIK_RETURN) == true || GetGamePadPress(XINPUT_GAMEPAD_X) == true) 
			&& g_player.state != PLAYERSTATE_CARRY && g_player.state != PLAYERSTATE_APPEAR && g_player.state != PLAYERSTATE_GOAL && g_player.state != PLAYERSTATE_THROW)
		{
			if (g_player.Rot != PLAYER_ROT_FORWARD)
			{
				g_player.OldRot = g_player.Rot;			//�O��̌����X�V
			}

			g_player.state = PLAYERSTATE_DELIVERY;		//�n�R�Y�ݏo����Ԃ�
			g_player.Rot = PLAYER_ROT_FORWARD;			//���ʂ�����

			//�n�R�̐���
			SetHako();
			
			g_player.nPatternAnim = PLAYER_ANIM_DELIVERY;
		}

		else
		{
			FixHakoCol();
		}

		//�X�y�[�X�L�[�������ꂽ
		if ((GetKeyboardPress(DIK_SPACE) == true || GetGamePadPress(XINPUT_GAMEPAD_A) == true) && g_player.state != PLAYERSTATE_APPEAR)
		{
			g_player.bJump = true;							//�W�����v���ɂ���
			g_player.move.y = JUMP_MOVE;					//��яオ�点��
		}

		//���Ⴊ��
		if ((GetKeyboardPress(DIK_S) == true || GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN) == true) && g_player.bJump == false && g_player.state != PLAYERSTATE_DELIVERY)
		{
			if (g_player.Rot != PLAYER_ROT_FORWARD)
			{
				g_player.OldRot = g_player.Rot;			//�O��̌����X�V
			}

			g_player.Rot = PLAYER_ROT_FORWARD;			//���ʌ���

			g_player.nPatternAnim = PLAYER_ANIM_SIT;
		}

		//�O��̌����ɒ���
		if (GetKeyboardRelease(DIK_S) == true || GetKeyboardRelease(DIK_RETURN) == true || 
			GetGamePadRelease(XINPUT_GAMEPAD_DPAD_DOWN) == true || GetGamePadRelease(XINPUT_GAMEPAD_X) == true)
		{
			g_player.Rot = g_player.OldRot;		
		}
	}
	
	//�ړ��ʂ��X�V(����������)
	g_player.move.x += (F_ZERO - g_player.move.x) * 0.2f;

	//�d�͂���ɉ�����
	CalculationGravity(&g_player.move);

	//Y���W�̈ʒu�X�V�Ɠ����蔻��
	CollisionAndPos_y();

	//X���W�̈ʒu�X�V�Ɠ����蔻��
	CollisionAndPos_x();

	//�u���b�N�ɂ��n�R�ɂ�����Ă��Ȃ�
	if(g_player.bOnBlock == false && g_player.bOnHako == false)
	{
		g_player.bJump = true;		//�󒆂ɕ����Ă��邽�߃W�����v��Ԃɂ���
		g_player.nCounterAnim = ZERO;
		g_player.nPatternAnim = PLAYER_ANIM_JUMP;
	}

	else
	{
		//�����������Ă���Ƃ��ɃA�j���[�V�����J�E���^�[�𑝂₷
		if (fabsf(g_player.move.x) >= F_ONE)
		{
			g_player.nCounterAnim++;
		}
	}

	//�v���C���[�̏�ԏ�񕪊�
	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_player.AppearCnt--;
		g_player.nPatternAnim = PLAYER_ANIM_SLEEP;

		//�ڊo�܂��A�j���[�V����
		if ((g_player.AppearCnt < 40 && g_player.AppearCnt >= 35) || g_player.AppearCnt < 20)
		{
			g_player.nPatternAnim = PLAYER_ANIM_SIT;
		}

		//��ыN���A�j���[�V����
		if (g_player.AppearCnt == ZERO)
		{
			g_player.nPatternAnim = PLAYER_ANIM_JUMP;
			g_player.bJump = true;							//�W�����v���ɂ���
			g_player.move.y = JUMP_MOVE;					//��яオ�点��
		}

		//���n�����瑀��J�n
		else if(g_player.AppearCnt < ZERO)
		{
			g_player.nPatternAnim = PLAYER_ANIM_JUMP;

			if (g_player.bJump == false)
			{
				g_player.state = PLAYERSTATE_NORMAL;
			}
		}
		break;

	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_THROW:
		if (GetKeyboardRelease(DIK_RETURN) == true || GetGamePadRelease(XINPUT_GAMEPAD_X) == true)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nPatternAnim = PLAYER_ANIM_DAMAGE;
		if (g_player.move.y > F_ZERO)
		{
			SetParticle(g_player.pos, BIRTH_PARTICLE_PLAYER);
			g_player.state = PLAYERSTATE_DEATH;
			g_player.DeathCounter = DEATH_TIME;
		}
		break;

	case PLAYERSTATE_FALL:
		SetParticle(D3DXVECTOR3(g_player.pos.x , FALL_DEATH_POS, g_player.pos.z), BIRTH_PARTICLE_PLAYER);
		g_player.state = PLAYERSTATE_DEATH;
		g_player.DeathCounter = DEATH_TIME;
		break;

	case PLAYERSTATE_DEATH:
		g_player.DeathCounter--;
		g_player.bDisp = false;

		if (g_player.DeathCounter <= ZERO)
		{
			SetFade(MODE_GAME);
		}
		break;

	case PLAYERSTATE_GOAL:
		g_player.nPatternAnim = PLAYER_ANIM_HAPPY;
		g_player.Rot = PLAYER_ROT_FORWARD;
		break;
	}

	//�A�j���[�V�����ݒ�
	if (g_player.nCounterAnim == 8)
	{
		g_player.nCounterAnim = ZERO;	//�A�j���[�V�����J�E���^�[���[���ɂ���
		g_player.nPatternAnim++;		//�A�j���[�V�����p�^�[���X�V

		//�e�N�X�`���p�^�[���������𒴂���
		if (g_player.nPatternAnim >= MOVE_MAX_TEX)
		{//�e�N�X�`�����W�������l�ɖ߂�
			g_player.nPatternAnim = ZERO;
		}
	}

	//�v���C���[�̌���(�e�N�X�`�����W)�̐ݒ�
	switch (g_player.Rot)
	{
	case PLAYER_ROT_FORWARD:
		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS + g_player.nPatternAnim * 0.1f, 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(0.1f        + g_player.nPatternAnim * 0.1f, 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS  + g_player.nPatternAnim * 0.1f, 0.25f);
			pVtx[SAN].tex = D3DXVECTOR2(0.1f        + g_player.nPatternAnim * 0.1f, 0.25f);
		}
		break;

	case PLAYER_ROT_RIGHT:
	case PLAYER_ROT_LEFT:
		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS + g_player.nPatternAnim * 0.1f, ((float)g_player.Rot * 0.25f));
			pVtx[ONE].tex = D3DXVECTOR2(0.1f + g_player.nPatternAnim * 0.1f,        ((float)g_player.Rot * 0.25f));
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS + g_player.nPatternAnim * 0.1f,  ((float)g_player.Rot * 0.25f) + 0.25f);
			pVtx[SAN].tex = D3DXVECTOR2(0.1f + g_player.nPatternAnim * 0.1f,        ((float)g_player.Rot * 0.25f) + 0.25f);
		}
		break;
	}

	//��ʒ[����
	{
		//���[����
		{
			//��ʒ[�Ȃ�A�����߂�
			if (g_player.pos.x > MOVE_LIMIT_R)
			{//�E�[
				g_player.pos.x = MOVE_LIMIT_R;
				//g_player.move.x = F_ZERO;
			}

			if (g_player.pos.x - g_player.fWidth + COLLISION_PLAYER < MOVE_LIMIT_L)
			{//���[
				g_player.pos.x = MOVE_LIMIT_L + g_player.fWidth - COLLISION_PLAYER;
			}
		}

		//�c�[����
		{
			//�V��𒴂���
			if (g_player.pos.y < MOVE_LIMIT_CEILING)
			{//���
				g_player.pos.y = MOVE_LIMIT_CEILING;
				g_player.move = ZERO_SET;
			}

			//�n�ʂɒ��n
			if (g_player.pos.y > MOVE_LIMIT_FLOOR && g_player.state != PLAYERSTATE_FALL && g_player.state != PLAYERSTATE_DEATH && g_player.state != PLAYERSTATE_DAMAGE)
			{//���
				g_player.state = PLAYERSTATE_FALL;
			}
		}
	}

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y - g_player.fHeight, ORIGIN_POS);
		pVtx[ONE].pos  = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y - g_player.fHeight, ORIGIN_POS);
		pVtx[SEC].pos  = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y, ORIGIN_POS);
		pVtx[SAN].pos  = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y, ORIGIN_POS);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//�v���C���[�̕`�揈��
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice= GetDevice();			//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffPlayer, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.bDisp == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTexturePlayer);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
	}
}

//�v���C���[�̎擾
Player *GetPlayer(void)
{
	return &g_player;
}

//�v���C���[�̃q�b�g����
void HitPlayer(PLAYERSTATE state)
{
	if (g_player.state != PLAYERSTATE_DAMAGE && g_player.state != PLAYERSTATE_FALL && g_player.state != PLAYERSTATE_DEATH)
	{
		//�Q�[����Ԃ̐ݒ�
		g_player.state = state;
		g_player.nPatternAnim = PLAYER_ANIM_DAMAGE;
		g_player.Rot = PLAYER_ROT_FORWARD;

		if (state ==  PLAYERSTATE_DAMAGE)
		{//�ɂ��Ĕ�яオ�������������o
			g_player.move.x *= (float)MINUS_ONE;
			g_player.move.y = JUMP_MOVE;
		}
	}
	
}

void MovePlayer(void)
{
	//�v���C���[�̏�Ԃ�������ꍇ
	if ((g_player.state == PLAYERSTATE_NORMAL || g_player.state == PLAYERSTATE_CARRY || g_player.state == PLAYERSTATE_THROW)
		&& GetKeyboardPress(DIK_RETURN) == false && GetGamePadPress(XINPUT_GAMEPAD_X) == false &&
		GetKeyboardPress(DIK_S) == false && GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN) == false)
	{
		//���Ɉړ�
		if (GetKeyboardPress(DIK_D) == true || GetGamePadPress(XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{//D�L�[�������ꂽ
			g_player.move.x += sinf(D3DX_PI * RIGHT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * RIGHT) * MOVE_SPEED;

			//�E������
			g_player.Rot = PLAYER_ROT_RIGHT;
		}

		//�E�Ɉړ�
		else if (GetKeyboardPress(DIK_A) == true || GetGamePadPress(XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{//A�L�[�������ꂽ

			g_player.move.x += sinf(D3DX_PI * LEFT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * LEFT) * MOVE_SPEED;

			//��������
			g_player.Rot = PLAYER_ROT_LEFT;
		}

		//�����Ă��Ȃ�
		else
		{//�e�N�X�`�����W��A�j���[�V�����J�E���^�[���[���ɂ���
			g_player.move.x = F_ZERO;
			g_player.nCounterAnim = ZERO;
			g_player.nPatternAnim = ZERO;
		}
	}
}

//���̓����蔻��ƍ��W�X�V
void CollisionAndPos_x(void)
{
	//X���W�X�V
	g_player.pos.x += g_player.move.x;

	//�u���b�N�Ƃ̓����蔻��@��
	g_player.bSideBlock = X_CollisionBlockToPlayer(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fWidth - COLLISION_PLAYER, g_player.fHeight, &g_player.pBlock);

	//�n�R�Ƃ̓����蔻�� ��
	X_CollisionHako();
}

//�c�̓����蔻��ƍ��W�X�V
void CollisionAndPos_y(void)
{
	//Y�X�V���X�V
	g_player.pos.y += g_player.move.y;

	//�u���b�N�Ƃ̓����蔻�� + �W�����v�ł��邩�ǂ���
	if (Y_CollisionBlockToPlayer(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fWidth - COLLISION_PLAYER, g_player.fHeight, &g_player.pBlock) == true)
	{
		g_player.bJump = false;				//���n��Ԃɂ���

		g_player.bOnBlock = true;			//�u���b�N�ɏ���Ă���
	}

	//�n�R�Ƃ̓����蔻��
	if (Y_CollisionHako() == true)
	{
		g_player.bJump = false;		//���n��Ԃɂ���

		g_player.bOnHako = true;	//�n�R�ɏ���Ă���
	}
}