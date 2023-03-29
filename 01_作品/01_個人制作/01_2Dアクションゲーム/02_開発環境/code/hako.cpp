/*==========================================================================================================================

�u���b�N�̏���[block.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "hako.h"
#include "block.h"
#include "player.h"
#include "input.h"
#include "gravity.h"
#include "move.h"
#include "sound.h"
#include <string.h>
#include "particle.h"

//�}�N����`
#define MAX_MyBLOCK			(5)							//�n�R�̍ő吔
#define BIRTH_PARTICLE_HAKO	(5)							//�p�[�e�B�N���̔�����
#define MOJI_MAX			(300)						//�ǂݍ��ލۂ̍ő啶����
#define MyHAKO_SIZE			(60.0f)						//�n�R�̃T�C�Y
#define HAKO_SPAWN_RIGHT	(60.0f)						//�n�R���E�ɐ��܂�鋗������
#define HAKO_SPAWN_LEFT		(55.0f)						//�n�R�����ɐ��܂�鋗������
#define HAKO_SPAWN_OVER		(30.0f)						//�n�R����ɐ��܂�鋗������
#define THROW_MOVE			(8.0f)						//������X�s�[�h
#define LIFT_UP				(10.0f)						//�����グ�鍂��
#define UNLOAD				(10.0f)						//�n�R���~�낷����
#define FIX_MARGIN			(D3DXVECTOR2(0.0f, 1.0f))	//�v���C���[�̗]������
#define SHADE_HAKO			(8)							//�n�R�̃t�F�[�h�ω���

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureHako = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffHako = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Hako g_Hako[MAX_MyBLOCK];							//�v���C���[�̃u���b�N�\����
int g_nDeliveryCnt;									//�o�Y�J�E���^
bool g_bGravity;									//�n�R�ɏd�͂������邩�ǂ���  false:�d�͂�������   true:�d�͂������Ȃ�

#define SIZEOF_HAKO			(sizeof g_Hako / sizeof(Hako))	//�n�R�̃f�[�^�T�C�Y

//�n�R�̏���������
void InitHako(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/playerHAKO.png", &g_pTextureHako);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * SIZEOF_HAKO, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHako, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHako->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		//�v���C���[�����ݏo���n�R�̏�񏉊���
		{
			g_Hako[nCntHako].pos = ZERO_SET;				//�ʒu������
			g_Hako[nCntHako].posOld = ZERO_SET;				//�O��̈ʒu��������
			g_Hako[nCntHako].move = ZERO_SET;				//�ړ��ʏ�����
			g_Hako[nCntHako].fWidth = MyHAKO_SIZE;			//��������
			g_Hako[nCntHako].fHeight = MyHAKO_SIZE;			//����������
			g_Hako[nCntHako].myBlockState = HAKO_NONE;		//���܂�Ă��Ȃ���Ԃɏ�����
			g_Hako[nCntHako].nDirect = DIRECTION_MAX;		//�����͌��܂��Ă��Ȃ����Ƃ�
			g_Hako[nCntHako].pBlock = NULL;					//�u���b�N�̏�񖳂�
			g_Hako[nCntHako].nCol = COLOR_MAX;				//�F�̏�����
			g_Hako[nCntHako].bShade = false;				//�g�p����Ă��Ȃ���Ԃɏ�����
			g_Hako[nCntHako].bUse = false;					//�g�p����Ă��Ȃ���Ԃɏ�����
		}

		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = g_Hako[nCntHako].pos;
			pVtx[ONE].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth, g_Hako[nCntHako].pos.y, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x,  g_Hako[nCntHako].pos.y + g_Hako[nCntHako].fHeight, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth, g_Hako[nCntHako].pos.y + g_Hako[nCntHako].fHeight, ORIGIN_POS);
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
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN,       ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS,  F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN,       F_TEN);
		}

		pVtx += NUM_VERTEX;
	}

	g_nDeliveryCnt = ZERO;		//�o�Y�J�E���^������
	g_bGravity = false;			//�d�͏������菉����

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffHako->Unlock();
}

//�n�R�̏I������
void UninitHako(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureHako != NULL)
	{
		g_pTextureHako->Release();
		g_pTextureHako = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHako != NULL)
	{
		g_pVtxBuffHako->Release();
		g_pVtxBuffHako = NULL;
	}
}

//�n�R�̍X�V����
void UpdateHako(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHako->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���ׂẴn�R���폜 & �F�C��
	if (GetKeyboardTrigger(DIK_DELETE) == true || GetGamePadTrigger(XINPUT_GAMEPAD_Y) == true || pPlayer->state == PLAYERSTATE_GOAL)
	{
		ResetHako();
	}
	
	//���ׂẴn�R�������グ�ړ�������
	LiftUpHako();

	//�n�R�^�ђ�
	CarryHako();

	//�n�R�̏d�͏���(�u���b�N�̓����蔻���������)
	GravityHako();

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++, pVtx += NUM_VERTEX)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			g_Hako[nCntHako].posOld = g_Hako[nCntHako].pos;		//�O��̈ʒu���L��

			//�n�R�̏�Ԃɂ�鏈������
			SwitchHako(nCntHako);

			//���W�X�V
			moveObject(&g_Hako[nCntHako].pos);

			//���_���W�̐ݒ�
			{
				pVtx[ZERO].pos = g_Hako[nCntHako].pos;
				pVtx[ONE].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + MyHAKO_SIZE,  g_Hako[nCntHako].pos.y,  ORIGIN_POS);
				pVtx[SEC].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x,  MyHAKO_SIZE + g_Hako[nCntHako].pos.y , ORIGIN_POS);
				pVtx[SAN].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + MyHAKO_SIZE,  g_Hako[nCntHako].pos.y + MyHAKO_SIZE, ORIGIN_POS);
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
				pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
				pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
				pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffHako->Unlock();
}

//�n�R�̕`�揈��
void DrawHako(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffHako, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureHako);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHako * NUM_VERTEX, SEC);
		}
	}
}

//�n�R�̎Y�ݏo������
void SetHako(void)
{	
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHako->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//��ڐ���
	if (g_nDeliveryCnt == ZERO)
	{	//�E�ɏo��
		if (GetKeyboardTrigger(DIK_RIGHT) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{
			//�n�R�����ׂď�������
			ResetHako();
			
			//�n�R�̐��ݏo�����y
			HakoSpawnSound();

			g_Hako[g_nDeliveryCnt].pos.x = pPlayer->pos.x + HAKO_SPAWN_RIGHT;
			g_Hako[g_nDeliveryCnt].pos.y = pPlayer->pos.y - MyHAKO_SIZE;
			g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_RIGHT;	//�E�����ɐ��܂ꂽ
			g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//���܂ꂽ��Ԃ�
			g_Hako[g_nDeliveryCnt].bUse = true;
			g_nDeliveryCnt++;
		}

		//���ɏo��
		else if (GetKeyboardTrigger(DIK_LEFT) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{
			//�n�R�����ׂď�������
			ResetHako();

			//�n�R�̐��ݏo�����y
			HakoSpawnSound();

			g_Hako[g_nDeliveryCnt].pos.x = pPlayer->pos.x - (pPlayer->fWidth + HAKO_SPAWN_LEFT);
			g_Hako[g_nDeliveryCnt].pos.y = pPlayer->pos.y - MyHAKO_SIZE;
			g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_LEFT;	//�������ɐ��܂ꂽ
			g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//���܂ꂽ��Ԃ�
			g_Hako[g_nDeliveryCnt].bUse = true;
			g_nDeliveryCnt++;
		}

		//��ɏo��
		else if (GetKeyboardTrigger(DIK_UP) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
		{
			//�n�R�����ׂď�������
			ResetHako();

			//�n�R�̐��ݏo�����y
			HakoSpawnSound();

			g_Hako[g_nDeliveryCnt].pos.x = pPlayer->pos.x - HAKO_SPAWN_OVER;
			g_Hako[g_nDeliveryCnt].pos.y = pPlayer->pos.y - (MyHAKO_SIZE + MyHAKO_SIZE);
			g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_OVER;		//������ɐ��܂ꂽ
			g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;		//���܂ꂽ��Ԃ�
			g_Hako[g_nDeliveryCnt].bUse = true;
			g_nDeliveryCnt++;
		}

		//�������삪�s���Ă��Ȃ�
		else
		{
			FadeHako();								//�n�R�𐶂ݏo���܂Ŋ����̃n�R���t�F�[�h������
			pPlayer->state = PLAYERSTATE_NORMAL;	//�ʏ탂�[�h
		}
	}

	//��ڈȍ~����
	else if(g_nDeliveryCnt <= MAX_MyBLOCK)
	{//�Q�ڈȍ~�͂ЂƂO�̃n�R�����ɐ���
		//�E�ɐ���
		if (GetKeyboardTrigger(DIK_RIGHT) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{
			//�ЂƂO�̃n�R�̕������t�����ɐ��܂�Ă���
			if (g_Hako[g_nDeliveryCnt - ONE].nDirect == DIRECTION_LEFT)
			{//�ЂƂO�̃n�R������
				g_Hako[g_nDeliveryCnt - ONE].bUse = false;
				g_nDeliveryCnt--;
			}
			else if(g_nDeliveryCnt < MAX_MyBLOCK)
			{
				//�n�R�̐��ݏo�����y
				HakoSpawnSound();

				g_Hako[g_nDeliveryCnt].pos.x = g_Hako[g_nDeliveryCnt - ONE].pos.x + MyHAKO_SIZE;
				g_Hako[g_nDeliveryCnt].pos.y = g_Hako[g_nDeliveryCnt - ONE].pos.y;
				g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_RIGHT;	//�E�����ɐ��܂ꂽ
				g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//���܂ꂽ��Ԃ�
				g_Hako[g_nDeliveryCnt].bUse = true;
				g_nDeliveryCnt++;
			}
		}

		//���ɐ���
		else if (GetKeyboardTrigger(DIK_LEFT) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{
			//�ЂƂO�̃n�R�̕������t�����ɐ��܂�Ă���
			if (g_Hako[g_nDeliveryCnt - ONE].nDirect == DIRECTION_RIGHT)
			{//�ЂƂO�̃n�R������
				g_Hako[g_nDeliveryCnt - ONE].bUse = false;
				g_nDeliveryCnt--;
			}
			else if (g_nDeliveryCnt < MAX_MyBLOCK)
			{
				//�n�R�̐��ݏo�����y
				HakoSpawnSound();

				g_Hako[g_nDeliveryCnt].pos.x = g_Hako[g_nDeliveryCnt - ONE].pos.x - MyHAKO_SIZE;
				g_Hako[g_nDeliveryCnt].pos.y = g_Hako[g_nDeliveryCnt - ONE].pos.y;
				g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_LEFT;	//�������ɐ��܂ꂽ
				g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//���܂ꂽ��Ԃ�
				g_Hako[g_nDeliveryCnt].bUse = true;
				g_nDeliveryCnt++;
			}
		}

		//��ɐ���
		else if (GetKeyboardTrigger(DIK_UP) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
		{
			//�ЂƂO�̃n�R�̕������t�����ɐ��܂�Ă���
			if (g_Hako[g_nDeliveryCnt - ONE].nDirect == DIRECTION_UNDER)
			{//�ЂƂO�̃n�R������
				g_Hako[g_nDeliveryCnt - ONE].bUse = false;
				g_nDeliveryCnt--;
			}
			else if (g_nDeliveryCnt < MAX_MyBLOCK)
			{
				//�n�R�̐��ݏo�����y
				HakoSpawnSound();

				g_Hako[g_nDeliveryCnt].pos.x = g_Hako[g_nDeliveryCnt - ONE].pos.x;
				g_Hako[g_nDeliveryCnt].pos.y = g_Hako[g_nDeliveryCnt - ONE].pos.y - MyHAKO_SIZE;
				g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_OVER;	//������ɐ��܂ꂽ
				g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//���܂ꂽ��Ԃ�
				g_Hako[g_nDeliveryCnt].bUse = true;
				g_nDeliveryCnt++;
			}
		}

		//���ɐ���
		else if (GetKeyboardTrigger(DIK_DOWN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{
			//�ЂƂO�̃n�R�̕������t�����ɐ��܂�Ă���
			if (g_Hako[g_nDeliveryCnt - ONE].nDirect == DIRECTION_OVER)
			{//�ЂƂO�̃n�R������
				g_Hako[g_nDeliveryCnt - ONE].bUse = false;
				g_nDeliveryCnt--;
			}
			else if (g_nDeliveryCnt < MAX_MyBLOCK)
			{
				//�n�R�̐��ݏo�����y
				HakoSpawnSound();

				g_Hako[g_nDeliveryCnt].pos.x = g_Hako[g_nDeliveryCnt - ONE].pos.x;
				g_Hako[g_nDeliveryCnt].pos.y = g_Hako[g_nDeliveryCnt - ONE].pos.y + MyHAKO_SIZE;
				g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_UNDER;	//�E�����ɐ��܂ꂽ
				g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//���܂ꂽ��Ԃ�
				g_Hako[g_nDeliveryCnt].bUse = true;
				g_nDeliveryCnt++;
			}			
		}
	}

	//�n�R�̐ݒ�
	for (int nCntHako = START_CNT; nCntHako < g_nDeliveryCnt; nCntHako++, pVtx += NUM_VERTEX)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			g_Hako[nCntHako].posOld = g_Hako[nCntHako].pos;

			//���_���W�̐ݒ�
			{
				pVtx[ZERO].pos = g_Hako[nCntHako].pos;
				pVtx[ONE].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth, g_Hako[nCntHako].pos.y, ORIGIN_POS);
				pVtx[SEC].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x,  g_Hako[nCntHako].pos.y + g_Hako[nCntHako].fHeight, ORIGIN_POS);
				pVtx[SAN].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth, g_Hako[nCntHako].pos.y + g_Hako[nCntHako].fHeight, ORIGIN_POS);
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
				pVtx[ONE].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
				pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
				pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffHako->Unlock();
}

/*�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P
				�n�R�̓����蔻��
_______________________________________________*/
//��
void X_CollisionHako(void)
{
	Player *pPlayer = GetPlayer();					//�v���C���[�̏��

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		float HeadPos     = pPlayer->pos.y - pPlayer->fHeight,		//���݂̓��̈ʒu
			  fWidth_R    = pPlayer->pos.x + pPlayer->fWidth,		//���݂̑�(�E��)�̉���
			  fWidth_L    = pPlayer->pos.x - pPlayer->fWidth,		//���݂̑�(����)�̉���
			  fOldWidth_R = pPlayer->posOld.x + pPlayer->fWidth,	//�O��̑�(�E��)�̉���
			  fOldWidth_L = pPlayer->posOld.x - pPlayer->fWidth;	//�O��̑�(����)�̉���

		if (g_Hako[nCntHako].bUse == true)
		{
			//���݂̈ʒu���n�R�̓����蔻��͈͓̔�
			if (g_Hako[nCntHako].pos.x < fWidth_R && fWidth_L < g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth)
			{
				//�n�R�̒��蓪�������A�n�R�̓V���葫���Ⴂ
				if (HeadPos < g_Hako[nCntHako].pos.y + MyHAKO_SIZE && pPlayer->pos.y > g_Hako[nCntHako].pos.y)
				{
					//�n�R�ɍ����瓖����
					if (g_Hako[nCntHako].posOld.x >= fOldWidth_R &&	//�O��̈ʒu���n�R��������
						g_Hako[nCntHako].pos.x < fWidth_R)			//���݂̈ʒu���n�R�ɂ߂荞��ł���
					{
						//�v���C���[���u���b�N�ɏ���Ă���
						if (pPlayer->bOnBlock == true)
						{
							if (pPlayer->bOnHako == false)
							{
								//�n�R�𓮂���
								HakoMove((pPlayer->pos.x + pPlayer->fWidth) - g_Hako[nCntHako].pos.x);
							}
						}
						//�v���C���[���u���b�N�ɏ���Ă��Ȃ�
						else
						{
							pPlayer->pos.x = g_Hako[nCntHako].pos.x - pPlayer->fWidth;	//�n�R�̈ʒu�ɖ߂�
							pPlayer->bSideBlock = true;									//�n�R�ɓ�������
						}
					}

					//�n�R�ɉE���瓖����
					if (g_Hako[nCntHako].posOld.x + MyHAKO_SIZE <= fOldWidth_L	//�O��̑�(�E��)�̈ʒu���n�R�����E��
						&& g_Hako[nCntHako].pos.x + MyHAKO_SIZE > fWidth_L)	//���݂̑�(�E��)�̈ʒu���n�R�ɂ߂荞��ł���
					{
						//�v���C���[���u���b�N�ɏ���Ă���
						if (pPlayer->bOnBlock == true)
						{
							if (pPlayer->bOnHako == false)
							{
								//�n�R�𓮂���
								HakoMove((pPlayer->pos.x - pPlayer->fWidth) - (g_Hako[nCntHako].pos.x + MyHAKO_SIZE));
							}
						}
						//�v���C���[���u���b�N�ɏ���Ă��Ȃ�
						else
						{
							pPlayer->pos.x = g_Hako[nCntHako].pos.x + MyHAKO_SIZE + pPlayer->fWidth;	//�n�R�̈ʒu�ɖ߂�
							pPlayer->bSideBlock = true;													//�n�R�ɓ�������
						}
					}
				}
			}
		}
	}
}

//�c
bool Y_CollisionHako(void)
{
	bool bLand = false;								//���n���Ă��Ȃ���Ԃ�
	Player *pPlayer = GetPlayer();					//�v���C���[�̏��

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		float HeadPos    = pPlayer->pos.y - pPlayer->fHeight,		//���݂̓��̈ʒu
			  fWidth_R   = pPlayer->pos.x + pPlayer->fWidth,		//���݂̑�(�E��)�̉���
			  fWidth_L   = pPlayer->pos.x - pPlayer->fWidth,		//���݂̑�(����)�̉���
			  HeadPosOld = pPlayer->posOld.y - pPlayer->fHeight;	//�O��̓��̈ʒu

		if (g_Hako[nCntHako].bUse == true)
		{
			//���݂̈ʒu���n�R�̓����蔻��͈͓̔�
			if (g_Hako[nCntHako].pos.x < fWidth_R && fWidth_L < g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth)
			{//���݂̃n�R�̈ʒu(��Ɖ�)
			 //�n�R�ɑ����߂荞��(�������)
				if (g_Hako[nCntHako].posOld.y >= pPlayer->posOld.y	//�O��̈ʒu���n�R�������
					&& g_Hako[nCntHako].pos.y < pPlayer->pos.y)	//���݂̈ʒu���n�R�ɂ߂荞��ł���
				{
					bLand = true;								//���n����
					pPlayer->pos.y = g_Hako[nCntHako].pos.y;	//�n�R�̈ʒu�ɖ߂�
					pPlayer->move.y = 0.0f;						//�ړ���Y���O�ɂ���
				}

				//�n�R�ɓ����߂荞��
				else if (g_Hako[nCntHako].posOld.y + MyHAKO_SIZE <= HeadPosOld	//�O��̓��̈ʒu���n�R��������
					  && g_Hako[nCntHako].pos.y + MyHAKO_SIZE > HeadPos)		//���݂̓��̈ʒu���n�R�ɓ˂��h�����Ă���
				{
					g_Hako[nCntHako].pos.y = HeadPos - MyHAKO_SIZE;	//���ɂ̂���
				}
			}
		}
	}

	return bLand;	//���n�������ǂ�����Ԃ�
}

//�n�R�𓮂���
void HakoMove(float fHakoMove)
{
	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		g_Hako[nCntHako].pos.x += fHakoMove;		//���ׂẴn�R������
	}
}

//�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P
//			�X�V�����̒��ōs���֐�
//__________________________________________
//�n�R�̐ڐ�����
void ConnectSide(D3DXVECTOR3 ParentPos)
{
	for (int ConnectCnt = START_CNT; ConnectCnt < SIZEOF_HAKO; ConnectCnt++)
	{
		//�e�̈ʒu�ƍŏ��̃n�R�̈ʒu���Ⴄ
		if (ParentPos != g_Hako[ConnectCnt].pos)
		{
			//���܂ꂽ�����ɂ��ʒu����
			switch (g_Hako[ConnectCnt].nDirect)
			{
			case DIRECTION_LEFT:
				g_Hako[ConnectCnt].pos.x = ParentPos.x - MyHAKO_SIZE;	//�e�̍����Ɉʒu����
				g_Hako[ConnectCnt].pos.y = ParentPos.y;					//Y���̈ʒu�͓���
				break;

			case DIRECTION_RIGHT:
				g_Hako[ConnectCnt].pos.x = ParentPos.x + MyHAKO_SIZE;	//�e�̉E���Ɉʒu����
				g_Hako[ConnectCnt].pos.y = ParentPos.y;					//Y���̈ʒu�͓���
				break;

			case DIRECTION_OVER:
				g_Hako[ConnectCnt].pos.x = ParentPos.x;					//X���̈ʒu�͓���
				g_Hako[ConnectCnt].pos.y = ParentPos.y - MyHAKO_SIZE;	//�e�̏㑤�Ɉʒu����
				break;

			case DIRECTION_UNDER:
				g_Hako[ConnectCnt].pos.x = ParentPos.x;					//X���̈ʒu�͓���
				g_Hako[ConnectCnt].pos.y = ParentPos.y + MyHAKO_SIZE;	//�e�̉����Ɉʒu����
				break;
			}
		}

		//�e���X�V
		ParentPos = g_Hako[ConnectCnt].pos;
	}
}

//�n�R�̃t�F�[�h����
void FadeHako(void)
{
	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			if (g_Hako[nCntHako].bShade == false)
			{
				//�s�����Ɍ���
				g_Hako[nCntHako].nCol -= SHADE_HAKO;

				if (g_Hako[nCntHako].nCol < COLOR_MIN)
				{
					g_Hako[nCntHako].nCol = COLOR_MIN;
					g_Hako[nCntHako].bShade = true;
				}
			}
			else
			{
				//�s�����Ɍ���
				g_Hako[nCntHako].nCol += SHADE_HAKO;

				if (g_Hako[nCntHako].nCol > COLOR_MAX)
				{
					g_Hako[nCntHako].nCol = COLOR_MAX;
					g_Hako[nCntHako].bShade = false;
				}
			}
		}
	}
}

//�n�R�̐F�C��
void FixHakoCol(void)
{
	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		g_Hako[nCntHako].nCol = COLOR_MAX;			//�F�̏�����
		g_Hako[nCntHako].bShade = false;			//�g�p����Ă��Ȃ���Ԃɏ�����
	}
}

//�n�R�̏�����
void ResetHako(void)
{
	Player *pPlayer = GetPlayer();		//�v���C���[�̏�Ԃ��m�[�}����

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			SetParticle(g_Hako[nCntHako].pos, BIRTH_PARTICLE_HAKO);

			g_Hako[nCntHako].pos = ZERO_SET;			//�ʒu������
			g_Hako[nCntHako].posOld = ZERO_SET;			//�O��ʒu������
			g_Hako[nCntHako].move = ZERO_SET;			//�ړ��ʏ�����
			g_Hako[nCntHako].myBlockState = HAKO_NONE;	//�����Ȃ���Ԃ�
			g_Hako[nCntHako].bUse = false;				//�g�p����Ă��Ȃ���Ԃ�
		}
	}
	FixHakoCol();							//�n�R�̐F�C��
	if (pPlayer->state != PLAYERSTATE_GOAL)
	{
		pPlayer->state = PLAYERSTATE_NORMAL;	//�v���C���[��Ԃ�ʏ��
	}

	g_nDeliveryCnt = ZERO;					//�o�Y�J�E���^���[���ɂ���
	g_bGravity = false;						//�d�͏������菉����
}

//�n�R�̎����グ����
void LiftUpHako(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾

	if ((GetKeyboardRelease(DIK_RETURN) == true || GetGamePadRelease(XINPUT_GAMEPAD_X) == true) &&
		pPlayer->state == PLAYERSTATE_DELIVERY && g_Hako[ZERO].myBlockState == HAKO_SPAWN)
	{
		for (int nCntHako = START_CNT; nCntHako < MAX_MyBLOCK; nCntHako++)
		{
			if (g_Hako[nCntHako].bUse == true)
			{
				g_Hako[nCntHako].pos.y = g_Hako[nCntHako].pos.y - LIFT_UP;	//�n�R�̎����グ
				g_Hako[nCntHako].myBlockState = HAKO_CARRY;					//�n�R�^�΂ꒆ
				pPlayer->state = PLAYERSTATE_CARRY;							//�n�R�^�ђ�
			}
		}

		pPlayer->Rot = pPlayer->OldRot;	//�O��̌����ɏC��
		g_nDeliveryCnt = ZERO;			//�o�Y�J�E���^���[���ɂ���
	}
}

//�n�R�̉^�я���
void CarryHako(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾
	int nCntHako;

	if (pPlayer->state == PLAYERSTATE_CARRY)
	{
		float ThrowMoveRot = F_ZERO;

		//�n�R�𓊂���
		if (GetKeyboardTrigger(DIK_RETURN) == true ||		//�G���^�[�����ꂽ
			GetGamePadTrigger(XINPUT_GAMEPAD_X) == true)	//X�{�^�������ꂽ
		{
			switch (pPlayer->Rot)
			{
			case PLAYER_ROT_LEFT:		//���ɓ�����
				ThrowMoveRot = -THROW_MOVE;
				break;

			case PLAYER_ROT_RIGHT:		//�E�ɓ�����
				ThrowMoveRot = THROW_MOVE;
				break;

			default:
				ThrowMoveRot = F_ZERO;
				break;
			}

			for (nCntHako = START_CNT; nCntHako < MAX_MyBLOCK; nCntHako++)
			{
				if (g_Hako[nCntHako].bUse == true)
				{
					g_Hako[nCntHako].move.x += ThrowMoveRot;	//���ɓ�����
					g_Hako[nCntHako].move.y += -15.0f;			//��ɓ�����
					g_Hako[nCntHako].myBlockState = HAKO_SET;	//�n�R�̏��L���������
				}
			}
			g_nDeliveryCnt = ZERO;
			pPlayer->state = PLAYERSTATE_THROW;	//�����郂�[�h
			PlaySound(SOUND_LABEL_THROW);		//�����鉹�Đ�
		}

		//�n�R��u��
		if (GetKeyboardTrigger(DIK_S) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{
			float fUnload = UNLOAD;		//�n�R�𗎂Ƃ�����	

			//�ŏ��̃n�R��^��ɎY�񂾏ꍇ�A�]���ʂ�ɒu���Ƒ̂ɂ߂荞��ł��܂����߁A������ς������̂܂܎����
			if (g_Hako[ZERO].nDirect == DIRECTION_OVER)
			{
				fUnload = -1.0f;
			}

			for (nCntHako = START_CNT; nCntHako < MAX_MyBLOCK; nCntHako++)
			{
				if (g_Hako[nCntHako].bUse == true)
				{
					g_Hako[nCntHako].pos.y = g_Hako[nCntHako].pos.y + fUnload;	//�n�R�𑫌��Ɠ��������ɒu��
					g_Hako[nCntHako].myBlockState = HAKO_SET;					//�n�R�̏��L���������
				}
			}

			g_nDeliveryCnt = ZERO;					//�o�Y�J�E���^���[���ɂ���
			pPlayer->state = PLAYERSTATE_NORMAL;	//�ʏ탂�[�h
			PlaySound(SOUND_LABEL_SET);				//�����鉹�Đ�
		}
	}
}

//�n�R�̏d�͏���
void GravityHako(void)
{
	int LandCounter = ZERO;	//�u���b�N�ɏ���Ă��鐔�J�E���^

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			//�u���b�N�ɏ���Ă���
			if ((CollisionBlockToHako(&g_Hako[nCntHako].pos, &g_Hako[nCntHako].posOld, &g_Hako[nCntHako].move,
				MyHAKO_SIZE, MyHAKO_SIZE, &g_Hako[nCntHako].pBlock) == true))	//�ʂ̃n�R�ɂ�����Ă��Ȃ�
			{
				LandCounter++;	//�J�E���^�𑝂₷
			}
		}
	}

	//��ȏ�̃n�R���u���b�N�ɏ���Ă���
	if (LandCounter > ZERO)
	{
		//���ׂẴn�R�̈ړ��ʂ��[����
		for (int zeroMoveCnt = START_CNT; zeroMoveCnt < SIZEOF_HAKO; zeroMoveCnt++)
		{
			g_Hako[zeroMoveCnt].move = ZERO_SET;
		}

		//�d�͂��������Ȃ�����
		g_bGravity = true;
	}
	else
	{//�d�͂���������悤�ɂ���
		g_bGravity = false;
	}
}

//�n�R�̏�ԕ��򏈗�
void SwitchHako(int nSwitchNum)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾
	float fCorrection = F_ZERO;
	
	if (g_Hako[ZERO].nDirect == DIRECTION_RIGHT)
	{
		fCorrection = 10.0f;		//�E�ɏo���Ƃ��̏o���ʒu����
	}

	switch (g_Hako[nSwitchNum].myBlockState)
	{
	case HAKO_NONE:
		break;
	case HAKO_SPAWN:
		//�n�R�̐ڐ������킹��
		ConnectSide(D3DXVECTOR3((pPlayer->pos.x - pPlayer->fWidth) + FIX_MARGIN.x + fCorrection,	//���_������ɍ��킹��
								 pPlayer->pos.y - MyHAKO_SIZE,	//���_������ɍ��킹��
								 pPlayer->pos.z));
		break;
	case HAKO_CARRY:
		//�n�R�̐ڐ������킹��
		ConnectSide(D3DXVECTOR3((pPlayer->pos.x - pPlayer->fWidth)  + FIX_MARGIN.x + fCorrection,	//���_������ɍ��킹��
								(pPlayer->pos.y - pPlayer->fHeight) + FIX_MARGIN.y,	//���_������ɍ��킹��
								 pPlayer->pos.z));
		break;
	case HAKO_SET:
		//�d�͂�������
		if (g_bGravity == false)
		{
			CalculationGravity(&g_Hako[nSwitchNum].move);
		}

		//�n�R�̐ڐ������킹��
		if (nSwitchNum == ZERO)
		{
			ConnectSide(g_Hako[ZERO].pos);
		}

		//�ʒu�X�V
		g_Hako[nSwitchNum].pos += g_Hako[nSwitchNum].move;

		//���̈ړ��ʌ���(���z�ł͂P�u���b�N����ɗ�����C���[�W)
		g_Hako[nSwitchNum].move.x += (ZERO - g_Hako[nSwitchNum].move.x) * 0.1f;

		//�ړ��ʂ��啪���Ȃ��Ȃ���
		if (fabsf(g_Hako[nSwitchNum].move.x) < F_ONE)
		{
			g_Hako[nSwitchNum].move.x = F_ZERO;	//�ړ��ʂ��[���ɂ���
		}
		break;
	}
}

//�n�R�̏��擾
Hako *GetHako(void)
{
	return &g_Hako[0];	//�ŏ��̃n�R��Ԃ�
}

//�n�R�̎Y�ݏo��������
void HakoSpawnSound(void)
{
	switch (g_nDeliveryCnt)
	{
	case ZERO:
		PlaySound(SOUND_LABEL_HAKO001);
		break;

	case ONE:
		PlaySound(SOUND_LABEL_HAKO002);
		break;

	case SEC:
		PlaySound(SOUND_LABEL_HAKO003);
		break;

	case SAN:
		PlaySound(SOUND_LABEL_HAKO004);
		break;

	case FOUR:
		PlaySound(SOUND_LABEL_HAKO005);
		break;

	}

}