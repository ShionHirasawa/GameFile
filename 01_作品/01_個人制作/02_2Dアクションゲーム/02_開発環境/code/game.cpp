/*==========================================================================================================================

�Q�[����ʏ���[game.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "game.h"
#include "input.h"
#include "player.h"
#include "hako.h"
#include "bg.h"
#include "title.h"
#include "tutorial.h"
#include "door.h"
#include "move.h"
#include "block.h"
#include "fade.h"
#include "pause.h"
#include "particle.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;	//���_�o�b�t�@�ւ̃|�C���^
GAMESTATE g_gameState = GAMESTATE_NONE;			//�Q�[���̏��
int g_nCounterGameState = ZERO;					//��ԊǗ��J�E���^�[
bool g_bPause;									//�|�[�Y��Ԃ�ON/OFF

void InitGame(void)
{
	//�Q�[����ʂɎg����I�u�W�F�N�g�̏���������
	InitMove();		//�ړ������̏���������
	InitPlayer();	//�v���C���[�̏���������
	InitBg();		//�w�i�̏���������
	InitTitle();	//�^�C�g���̏�����
	InitTutorial();	//�`���[�g���A���̏�����
	InitDoor();		//���̏���������
	InitBlock();	//�u���b�N�̏���������
	InitHako();		//�n�R������
	InitParticle();	//�p�[�e�B�N���̏�����
	InitPause();	//�|�[�Y��ʂ̏�����

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_gameState = GAMESTATE_START;			//�Q�[����Ԃ̏�����
	g_nCounterGameState = ZERO;				//��ԊǗ��J�E���^�[�̏�����
	g_bPause = false;						//�|�[�Y�̏�����
}

void UninitGame(void)
{
	/*======================================================================
							�e��I�u�W�F�N�g�̏I������
	========================================================================*/
	//�Q�[����ʂɎg�����I�u�W�F�N�g�̏I������
	UninitBlock();		//�u���b�N�̏I������
	UninitPlayer();		//�v���C���[�̏I������
	UninitTitle();		//�^�C�g���I��
	UninitTutorial();	//�`���[�g���A���̏I������
	UninitBg();			//�w�i�̏I������
	UninitDoor();		//���̏I������
	UninitHako();		//�n�R�I������
	UninitParticle();	//�p�[�e�B�N���̏I������
	UninitPause();		//�{�[�Y�̏I������
}

void UpdateGame(void)
{
	FADE fadeState = GetFade();
	Player *pPlayer = GetPlayer();

	if (g_bPause == false)
	{
		UpdatePlayer();		//�v���C���[�̍X�V����
		UpdateHako();		//�v���C���[�̃n�R�X�V����
		UpdateDoor();		//���̍X�V����
		UpdateBlock();		//�u���b�N�̍X�V����
		UpdateTitle();		//�^�C�g���̍X�V����
		UpdateTutorial();	//�`���[�g���A���̍X�V����
		UpdateBg();			//�w�i�̍X�V����
		UpdateParticle();	//�p�[�e�B�N���̍X�V����
	}
	else
	{
		UpdatePause();		//�|�[�Y��ʂ̍X�V����

		//�|�[�Y����������Ƃ��Ƀ��j���[���R���e�B�j���[�ɐݒ�
		if (GetKeyboardTrigger(DIK_P) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true)
		{
			SetPause(PAUSE_MENU_CONTINUE);
		}		

		if (*GetPause() == PAUSE_MENU_CONTINUE)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
			{
				g_bPause = false;
			}
		}
	}

	//�|�[�Y��Ԑ؂�ւ�
	if (GetKeyboardTrigger(DIK_P) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true)
	{
		g_bPause = g_bPause ? false : true;
	}
}

void DrawGame(void)
{
	/*======================================================================
							�e��I�u�W�F�N�g�̕`�揈��
	========================================================================*/
	//�Q�[�����I�u�W�F�N�g�̕`�揈��		
	DrawBg();		//�w�i�̕`�揈��
	DrawTitle();	//�^�C�g����ʕ`��
	DrawTutorial();	//�`���[�g���A���̕`�揈��
	DrawDoor();		//���̕`�揈��
	DrawBlock();	//�u���b�N�̕`�揈��
	DrawHako();		//�v���C���[�̃n�R�`�揈��
	DrawPlayer();	//�v���C���[�̕`�揈��
	DrawParticle();	//�p�[�e�B�N���̕`�揈��
	DrawFPS();		//FPS�\��
	
	//�|�[�Y��
	if (g_bPause == true)
	{
		DrawPause();		//�|�[�Y��ʂ̕`�揈��
	}
}

void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterGameState = nCounter;
}

GAMESTATE GetGameState(void)
{
	return g_gameState;
}