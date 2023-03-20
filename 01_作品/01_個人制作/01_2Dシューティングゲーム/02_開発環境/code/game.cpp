/*==========================================================================================================================

�Q�[����ʏ���[game.cpp]
Author:���V����

==========================================================================================================================*/
#include "game.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "hp.h"
#include "pause.h"
#include "particle.h"
#include "boss.h"
#include "beam.h"

//�}�N����`
//�J�E���g�_�E���u�R�E�Q�E�P�v�̍��W
#define COUNT_THREE_POS_ZERO	D3DXVECTOR3(590.0f, 300.0f, 0.0f)
#define COUNT_THREE_POS_ONE		D3DXVECTOR3(690.0f, 300.0f, 0.0f)
#define COUNT_THREE_POS_SEC		D3DXVECTOR3(590.0f, 400.0f, 0.0f)
#define COUNT_THREE_POS_SAN		D3DXVECTOR3(690.0f, 400.0f, 0.0f)

//�J�E���g�_�E���uSTART!�v�̍��W
#define COUNT_START_POS_ZERO	D3DXVECTOR3(490.0f, 300.0f, 0.0f)
#define COUNT_START_POS_ONE		D3DXVECTOR3(790.0f, 300.0f, 0.0f)
#define COUNT_START_POS_SEC		D3DXVECTOR3(490.0f, 400.0f, 0.0f)
#define COUNT_START_POS_SAN		D3DXVECTOR3(790.0f, 400.0f, 0.0f)

//�J�E���g�_�E���u�R�v�̃e�N�X�`�����W
#define COUNT_THREE_TEX_ZERO	D3DXVECTOR2(0.0f, 0.0f)
#define COUNT_THREE_TEX_ONE		D3DXVECTOR2(0.2f, 0.0f)
#define COUNT_THREE_TEX_SEC		D3DXVECTOR2(0.0f, 0.5f)
#define COUNT_THREE_TEX_SAN		D3DXVECTOR2(0.2f, 0.5f)

//�J�E���g�_�E���u2�v�̃e�N�X�`�����W
#define COUNT_SEC_TEX_ZERO		D3DXVECTOR2(0.4f, 0.0f)
#define COUNT_SEC_TEX_ONE		D3DXVECTOR2(0.6f, 0.0f)
#define COUNT_SEC_TEX_SEC		D3DXVECTOR2(0.4f, 0.5f)
#define COUNT_SEC_TEX_SAN		D3DXVECTOR2(0.6f, 0.5f)

//�J�E���g�_�E���u1�v�̃e�N�X�`�����W
#define COUNT_ONE_TEX_ZERO		D3DXVECTOR2(0.8f, 0.0f)
#define COUNT_ONE_TEX_ONE		D3DXVECTOR2(1.0f, 0.0f)
#define COUNT_ONE_TEX_SEC		D3DXVECTOR2(0.8f, 0.5f)
#define COUNT_ONE_TEX_SAN		D3DXVECTOR2(1.0f, 0.5f)

//�J�E���g�_�E���uSTART!�v�̃e�N�X�`�����W
#define COUNT_START_TEX_ZERO	D3DXVECTOR2(0.0f, 0.5f)
#define COUNT_START_TEX_ONE		D3DXVECTOR2(1.0f, 0.5f)
#define COUNT_START_TEX_SEC		D3DXVECTOR2(0.0f, 1.0f)
#define COUNT_START_TEX_SAN		D3DXVECTOR2(1.0f, 1.0f)

#define COUNTDOWN_TEX			"data/TEXTURE/�Q�[���J�n�J�E���g�_�E��.png"	//�J�E���g�_�E���̃e�N�X�`��
#define COUNTDOWN_NUM			(25)		//�J�E���g�_�E���̃^�C�~���O
#define NUM_SET_ENEMY			(40)		//�Z�b�g����G�̐�
#define DECIDE_TYPE				(4)			//�G�̎�ނ����߂�

//�Q�[���I���̃N���A����̍\����
typedef enum
{
	RESULT_OVER = ZERO,
	RESULT_CLEAR,
	RESULT_MAX
}RESULT;

typedef enum
{
	COUNT_SAN = ZERO,
	COUNT_SEC,
	COUNT_ONE,
	COUNT_MAX,
}COUNTDOWN;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureGame = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;	//���_�o�b�t�@�ւ̃|�C���^
GAMESTATE g_gameState = GAMESTATE_NONE;			//�Q�[���̏��
int g_nCounterGameState = ZERO;					//��ԊǗ��J�E���^�[
RESULT g_numResult;								//���U���g����
bool g_bPause;									//�|�[�Y��Ԃ�ON/OFF


void InitGame(void)
{
	//�Q�[����ʂɎg����I�u�W�F�N�g�̏���������
	InitBg();			//�w�i������
	InitPlayer();		//�v���C���[�̏�����
	InitBullet();		//�e�̏�����
	InitBeam();			//�r�[���̏�����
	InitExplosion();	//�����̏�����
	InitEnemy();		//�G�̏���������
	InitScore();		//�X�R�A�̏���������
	SetScore(ZERO);		//�������X�R�A�̏����l��0
	InitEffect();		//�G�t�F�N�g�̏���������
	InitParticle();		//�p�[�e�B�N���̏�����
	InitHp();			//HP�A�C�R���̏���������	
	InitPause();		//�|�[�Y��ʂ̏���������
	InitBoss();			//�{�X�̏���������

	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

														//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, COUNTDOWN_TEX, &g_pTextureGame);

	g_gameState = GAMESTATE_START;			//�Q�[����Ԃ̏�����
	g_nCounterGameState = ZERO;				//��ԊǗ��J�E���^�[�̏�����
	g_bPause = false;						//�|�[�Y�̏�����

											//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGame, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGame->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = COUNT_THREE_POS_ZERO;
		pVtx[ONE].pos = COUNT_THREE_POS_ONE;
		pVtx[SEC].pos = COUNT_THREE_POS_SEC;
		pVtx[SAN].pos = COUNT_THREE_POS_SAN;
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
		pVtx[ZERO].tex = COUNT_THREE_TEX_ZERO;
		pVtx[ONE].tex = COUNT_THREE_TEX_ONE;
		pVtx[SEC].tex = COUNT_THREE_TEX_SEC;
		pVtx[SAN].tex = COUNT_THREE_TEX_SAN;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffGame->Unlock();

	//�����_���֐��̃V�[�h���������ݒ�
	srand((unsigned int)time(NULL));

	//�G�̔z�u
	for (int nCnt = START_CNT; nCnt < NUM_SET_ENEMY; nCnt++)
	{
		SetEnemy();
	}

	StopSound(SOUND_LABEL_BGM000);	//�^�C�g��BGM���I��
	PlaySound(SOUND_LABEL_BGM001);	//�Q�[���pBGM�J�n
}

void UninitGame(void)
{
	/*======================================================================
	�e��I�u�W�F�N�g�̏I������
	========================================================================*/
	//�Q�[����ʂɎg�����I�u�W�F�N�g�̏I������
	UninitBg();			//�w�i�̏I������
	UninitPlayer();		//�v���C���[�̏I������
	UninitBullet();		//�e�̏I������	
	UninitBeam();		//�r�[���̏�����
	UninitExplosion();	//�����̏I������	
	UninitEnemy();		//�G�̏I������
	UninitBoss();		//�{�X�̏I������
	UninitScore();		//�X�R�A�̏I������	
	UninitEffect();		//�G�t�F�N�g�̏I������
	UninitParticle();	//�p�[�e�B�N���̏I������
	UninitHp();			//HP�A�C�R���̏I������
	UninitPause();		//�|�[�Y��ʂ̏I������
}

void UpdateGame(void)
{
	FADE fadeState = GetFade();
	//�G or �������S��
	Player *pPlayer = GetPlayer();
	int NumEnemy = GetNumEnemy();

	//�|�[�Y���ł͂Ȃ�
	if (g_bPause == false)
	{
		/*======================================================================
		�e��I�u�W�F�N�g�̍X�V����
		========================================================================*/
		UpdateBg();			//�w�i�̍X�V����
		UpdatePlayer();		//�v���C���[�̍X�V����
		UpdateBullet();		//�e�̍X�V����
		UpdateBeam();		//�r�[���̍X�V����
		UpdateExplosion();	//�����̍X�V����
		UpdateEnemy();		//�G�̍X�V����
		UpdateScore();		//�X�R�A�̍X�V����
		UpdateEffect();		//�G�t�F�N�g�̍X�V����
		UpdateParticle();	//�p�[�e�B�N���̍X�V����
		UpdateHp();			//Hp�A�C�R���̍X�V����

							//�{�X�o��
		if (NumEnemy <= ZERO)
		{
			UpdateBoss();

			BOSS *pBoss = GetBoss();

			if (pBoss->nLife <= ZERO)
			{
				g_numResult = RESULT_CLEAR;
			}
		}
	}

	//�|�[�Y���ł���A�G��|�����]�C���ł͂Ȃ�
	else
	{
		int numEnemy = GetNumEnemy();

		//�|�[�Y����������Ƃ��Ƀ��j���[���R���e�B�j���[�ɐݒ�
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			SetPause(PAUSE_MENU_CONTINUE);
		}

		if (g_nCounterGameState == ZERO)
		{
			UpdatePause();

			PAUSE_MENU *pPause = GetPause();

			if (*pPause == PAUSE_MENU_CONTINUE)
			{
				if (GetKeyboardTrigger(DIK_RETURN) == true)
				{
					g_bPause = false;
				}
			}
		}

		else
		{
			g_bPause = false;
		}
	}

	if (g_gameState == GAMESTATE_START)
	{
		//�|�C���^��ݒ�
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffGame->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

		switch (pPlayer->AppearCnt / COUNTDOWN_NUM)
		{
		case COUNT_SAN:				//�J�E���g�_�E���u3�v
			break;

		case COUNT_SEC:				//�J�E���g�_�E���u2�v
									//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = COUNT_SEC_TEX_ZERO;
			pVtx[ONE].tex = COUNT_SEC_TEX_ONE;
			pVtx[SEC].tex = COUNT_SEC_TEX_SEC;
			pVtx[SAN].tex = COUNT_SEC_TEX_SAN;
		}
		break;

		case COUNT_ONE:				//�J�E���g�_�E���u1�v
									//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = COUNT_ONE_TEX_ZERO;
			pVtx[ONE].tex = COUNT_ONE_TEX_ONE;
			pVtx[SEC].tex = COUNT_ONE_TEX_SEC;
			pVtx[SAN].tex = COUNT_ONE_TEX_SAN;
		}
		break;

		default:			//�uSTART!�v
							//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = COUNT_START_POS_ZERO;
			pVtx[ONE].pos = COUNT_START_POS_ONE;
			pVtx[SEC].pos = COUNT_START_POS_SEC;
			pVtx[SAN].pos = COUNT_START_POS_SAN;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = COUNT_START_TEX_ZERO;
			pVtx[ONE].tex = COUNT_START_TEX_ONE;
			pVtx[SEC].tex = COUNT_START_TEX_SEC;
			pVtx[SAN].tex = COUNT_START_TEX_SAN;
		}

		if (pPlayer->state == PLAYERSTATE_NORMAL)
		{
			g_gameState = GAMESTATE_NORMAL;
		}
		break;
		}

		//���_�o�b�t�@�����b�N����
		g_pVtxBuffGame->Unlock();
	}

	//�v���C���[�̗̑͂��O�ɂȂ���
	if (pPlayer->nLife <= ZERO)
	{
		g_numResult = RESULT_OVER;
	}

	//�Q�[���̏��
	switch (g_gameState)
	{
	case GAMESTATE_START:
		break;

	case GAMESTATE_NORMAL:	//�ʏ���
		break;

	case GAMESTATE_END:		//�I�����
		g_nCounterGameState--;

		if (g_nCounterGameState == ZERO)
		{
			g_gameState = GAMESTATE_NONE;		//�������Ă��Ȃ����

												//�t�F�[�h�ݒ�i���U���g�ֈڍs)
			SetFade(MODE_RESULT);
		}
		break;
	}

	//�|�[�Y�L�[��ON/OFF
	if (GetKeyboardTrigger(DIK_P) == true && g_nCounterGameState == ZERO)
	{//P�L�[�������ꂽ & �G��|�����]�C���ł͂Ȃ�
		SetPause(PAUSE_MENU_CONTINUE);	//�|�[�Y���łȂ��Ƃ��̓|�[�Y���j���[���R���e�B�j���[�ɍX�V

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
	DrawPlayer();	//�v���C���[�̕`�揈��
	DrawEffect();	//�G�t�F�N�g�̕`�揈��
	DrawParticle();	//�p�[�e�B�N���̕`�揈��
	DrawBullet();	//�e�̕`�揈��7
	DrawBeam();		//�r�[���̕`�揈��
	DrawExplosion();//�����̕`�揈��
	DrawEnemy();	//�G�̕`�揈��
	DrawBoss();		//�{�X�̕`�揈��
	DrawScore();	//�X�R�A�̕`�揈��
	DrawHp();		//Hp�̕`�揈��

					//�Q�[���J�n�̃J�E���g�_�E��
	if (g_gameState == GAMESTATE_START)
	{
		LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

															//�f�o�C�X�̎擾
		pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(ZERO, g_pVtxBuffGame, ZERO, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureGame);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
	}

	if (g_bPause == true)
	{
		DrawPause();
	}
}

void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterGameState = nCounter;
}

int ResultChoice(void)
{
	return g_numResult;
}

GAMESTATE GetGameState(void)
{
	return g_gameState;
}