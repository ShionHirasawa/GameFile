/*==========================================================================================================================================================

														�Q�[����ʏ���[game.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "color.h"
#include "particle.h"
#include "model.h"
#include "shadow.h"
#include "player.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "meshfield.h"
#include "slope.h"
#include "slopewall.h"
#include "fade.h"
#include "game.h"
#include "pause.h"
#include "cannon.h"
#include "sound.h"
#include "life.h"
#include "grassland.h"
#include "score.h"
#include "timelimit.h"
#include "logthorns.h"
#include "launchpad.h"

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
bool					g_bFPS = true;					// FPS��\�����邩�ǂ���
bool					g_bPause = false;				// �|�[�Y
EndGAME					g_EndGame;						// �Q�[���I���̗��R

#if _DEBUG
PLAYMODE		g_PlayMode = PLAYMODE_MODEL;//�v���C���[�h�ݒ�
#else
PLAYMODE		g_PlayMode = PLAYMODE_MAX;	//�v���C���[�h�ݒ�
#endif

//------------------------------------------------
//				�Q�[���̏���������
//------------------------------------------------
void InitGame(void)
{
	InitBillboard();	// �r���{�[�h�̏���������
	InitGrassLand();	// �����̏���������
	InitShadow();		// �e�̏���������
	InitPlayer();		// �v���C���[�̏���������
	InitCamera();		// �J�����̏���������
	InitLight();		// ���C�g�̏���������
	InitWall();			// �ǂ̏���������
	InitParticle();		// �p�[�e�B�N���̏���������
	InitBullet();		// �e�̏���������
	InitExplosion();	// �����̏���������
	InitEffect();		// �G�t�F�N�g�̏���������
	InitMeshField();	// ���b�V���t�B�[���h�̏���������
	InitModel();		// ���f���̏���������
	InitCannon();		// ��C�̏���������
	InitLogThorns();	// �ۑ��g�Q�̏���������
	InitLaunchPad();	// ���ˑ�̏���������
	InitSlope();		// �⓹�̏���������
	InitSlopeWall();	// �⓹�̕ǂ̏���������

	InitPause();		// �|�[�Y��ʏ�����
	InitLife();			// ���C�t�A�C�R���̏�����
	InitScore();		// �X�R�A�̏���������
	InitTimeLimit();	// �������Ԃ̏���������

	g_bPause			= false;			// �|�[�Y�̏�����
	g_bFPS				= false;			// FPS�\���̏�����
	g_EndGame			= EndGAME_CLEAR;	// �Q�[���I�����R�̏�����

	//�Q�[��BGM�J�n
	PlaySound(SOUND_LABEL_GAMEBGM);
}

//------------------------------------------------
//				�Q�[���̏I������
//------------------------------------------------
void UninitGame(void)
{
	/*======================================================================
							�e��I�u�W�F�N�g�̏I������
	========================================================================*/
	
	/*�G�t�F�N�g�Ȃǂ̔|���S���̏I������*/
	UninitParticle();	// �p�[�e�B�N���̏I������
	UninitExplosion();	// �����̏I������
	UninitEffect();		// �G�t�F�N�g�̏I������
	UninitShadow();		// �e�̏I������

	/*UI�֌W�̏I������*/
	UninitPause();		// �|�[�Y��ʂ̏I������
	UninitLife();		// ���C�t�A�C�R���̏I������
	UninitScore();		// �X�R�A�̏I������
	UninitTimeLimit();	// �������Ԃ̏I������

	/*�X�e�[�W�֌W�̏I������*/
	UninitGrassLand();	// �����̏I������
	UninitWall();		// �ǂ̏I������
	UninitBillboard();	// �r���{�[�h�̏I������
	UninitMeshField();	// ���b�V���t�B�[���h�̏I������
	UninitSlopeWall();	// �⓹�̕ǂ̏I������
	UninitSlope();		// �⓹�̏I������
	UninitLight();		// ���C�g�̏I������

	UninitCamera();		// �J�����̏I������
	UninitPlayer();		// �v���C���[�̏I������
	UninitBullet();		// �e�̏I������
	UninitModel();		// ���f���̏I������
	UninitCannon();		// ��C�̏I������
	UninitLogThorns();	// �ۑ��g�Q�̏I������
	UninitLaunchPad();	// ���ˑ�̏I������

	//�Q�[��BGM��~
	StopSound(SOUND_LABEL_GAMEBGM);
	StopSound(SOUND_LABEL_GAMEBGM_SPEED);
}

//------------------------------------------------
//				�Q�[���̍X�V����
//------------------------------------------------
void UpdateGame(void)
{
	FADE fadeState = GetFade();
	Player *pPlayer = GetPlayer();

	//�|�[�Y��OFF
	if (g_bPause == false)
	{
#ifdef _DEBUG
		//�v���C���[�h�ɂ��X�V��������
		switch (g_PlayMode)
		{
		case PLAYMODE_MODEL:
			UpdatePlayer();		//�v���C���[�̍X�V����
			UpdateCamera();		//�J�����̍X�V����
			UpdateModel();		//���f���̍X�V����
			break;

		case PLAYMODE_FIELD:
			break;

		case PLAYMODE_WALL:
			break;

		case PLAYMODE_CAMERA:
			UpdateCamera();			//�J�����̍X�V����
			break;

		case PLAYMODE_STAGE:
			UpdateMeshField();		//���b�V���t�B�[���h�̍X�V����
			UpdateWall();			//�ǂ̍X�V����
			break;

			//�S���X�V
		case PLAYMODE_MAX:
			UpdateModel();			//���f���̍X�V����
			UpdatePlayer();			//�v���C���[�̍X�V����
			UpdateCamera();			//�J�����̍X�V����
			break;
		}
#else
		UpdatePlayer();		//�v���C���[�̍X�V����
		UpdateCamera();		//�J�����̍X�V����
		UpdateModel();		//���f���̍X�V����
		UpdateMeshField();	//���b�V���t�B�[���h�̍X�V����
		UpdateWall();		//�ǂ̍X�V����
#endif

		UpdateCannon();			// ��C�̍X�V����
		UpdateLogThorns();		// �ۑ��g�Q�̍X�V����
		UpdateLaunchPad();		// ���ˑ�̍X�V����
		UpdateSlope();			// �⓹�̍X�V����
		UpdateSlopeWall();		// �⓹�̕ǂ̍X�V����
		UpdateBullet();			// �e�̍X�V����
		UpdateExplosion();		// �����̍X�V����
		UpdateEffect();			// �G�t�F�N�g�̍X�V����
		UpdateParticle();		// �p�[�e�B�N���̍X�V����
		UpdateShadow();			// �e�̍X�V����
		UpdateLife();			// ���C�t�A�C�R���̍X�V����
		UpdateScore();			// �X�R�A�̍X�V����
		UpdateTimeLimit();		// �������Ԃ̍X�V����
	}

	else
	{
		//�|�[�Y��ʂ̍X�V����
		UpdatePause();

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

	UpdateLight();			// ���C�g�̍X�V����

#ifdef _DEBUG
	// �v���C���[�h�ύX
	PlayMode();
#endif

	//�|�[�Y��Ԑ؂�ւ�
	if (GetKeyboardTrigger(DIK_P) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true)
	{
		g_bPause = g_bPause ? false : true;
	}
}

//------------------------------------------------
//				�Q�[���̕`�揈��
//------------------------------------------------
void DrawGame(void)
{
	/*======================================================================
							�e��I�u�W�F�N�g�̕`�揈��
	========================================================================*/
	//�Q�[�����I�u�W�F�N�g�̕`�揈��
	SetCamera();		// �J�����̐ݒ菈��
	
	DrawMeshField();	// ���b�V���t�B�[���h�̕`�揈��
	DrawGrassLand();	// �����̕`�揈��
	DrawSlope();		// �⓹�̕`�揈��
	DrawSlopeWall();	// �⓹�̕ǂ̕`�揈��
	DrawWall();			// �ǂ̕`�揈��

	DrawModel();		// ���f���̕`�揈��
	DrawCannon();		// ��C�̕`�揈��
	DrawLogThorns();	// �ۑ��g�Q�̕`�揈��
	DrawLaunchPad();	// ���ˑ�̕`�揈��

	DrawShadow();		// �e�̕`�揈��
	DrawLife();			// ���C�t�A�C�R���̕`�揈��
	DrawScore();		// �X�R�A�̕`�揈��
	DrawTimeLimit();	// �������Ԃ̕`�揈��

	DrawEffect();		// �G�t�F�N�g�̕`�揈��
	DrawExplosion();	// �����̕`�揈��
	DrawBullet();		// �e�̕`�揈��
	DrawParticle();		// �p�[�e�B�N���̕`�揈��

	DrawPlayer();		// �v���C���[�̕`�揈��

#if _DEBUG
	//FPS�\��������
	if (g_bFPS == true)
	{
		DrawFPS(g_PlayMode);		//FPS�`�揈��
		//DrawFPSSecond(g_PlayMode);	//�E���Ƀf�o�b�O�\��
	}

	//FPS�\�� ON / OFF
	if (GetKeyboardTrigger(DIK_F1) == true)
	{
		g_bFPS = g_bFPS ? false : true;
	}
#endif

	//�|�[�Y��ʂ̕\����ON
	if (GetDispPause() == true && g_bPause == true)
	{
		DrawPause();		//�|�[�Y��ʂ̕`�揈��
	}

	//�|�[�Y��ʂ��\������Ă��Ȃ���΃J�������ړ�������
	else if (GetDispPause() == false)
	{
		MovePosV();		//���_�ړ�
		SpinPosV();		//���_��]
	}
}

//�v���C���[�h�ύX
void PlayMode(void)
{
	//�v���C���[���[�h�ύX
	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		switch (g_PlayMode)
		{
		case PLAYMODE_MODEL:
			g_PlayMode = PLAYMODE_FIELD;	//���f���@���@�t�B�[���h
			break;

		case PLAYMODE_FIELD:
			g_PlayMode = PLAYMODE_WALL;		//�t�B�[���h�@���@��
			break;

		case PLAYMODE_WALL:
			g_PlayMode = PLAYMODE_STAGE;	//�ǁ@���@�ǂƃt�B�[���h
			break;

		case PLAYMODE_STAGE:
			g_PlayMode = PLAYMODE_CAMERA;	//�ǂƃt�B�[���h�@���@�J����
			break;

		case PLAYMODE_CAMERA:
			g_PlayMode = PLAYMODE_MAX;		//�J�����@���@�S��
			break;

		case PLAYMODE_MAX:
			g_PlayMode = PLAYMODE_MODEL;	//�S�́@���@���f��
			break;
		}
	}

	//�v���C���[�h��S�̂�
	if (GetKeyboardTrigger(DIK_F6) == true)
	{
		g_PlayMode = PLAYMODE_MAX;		//�v���C���[�h �S��
	}
}

//�Q�[�����I���������R��Ԃ�
void SetEndGame(EndGAME end)
{
	g_EndGame = end;
}

//�Q�[���I���������R��Ԃ�
int EndGameResult(void)
{
	return (int)g_EndGame;
}