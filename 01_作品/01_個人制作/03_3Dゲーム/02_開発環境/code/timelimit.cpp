/*==========================================================================================================================================================

�������ԏ���[timelimit.cpp]
Author:���V����

============================================================================================================================================================*/
#include "timelimit.h"
#include "color.h"
#include "input.h"
#include "sound.h"
#include "score.h"
#include "fade.h"
#include "game.h"

//�}�N����`
#define TIMELIMIT_POS				(D3DXVECTOR3(620.0f, 3.0f, 0.0f))	//�X�R�A�̈ʒu
#define TIMELIMIT_WIDTH				(35.0f)								//��
#define TIMELIMIT_HEIGHT			(60.0f)								//����
#define TIMELIMIT_TEX_DIVISION		(0.1f)								//�e�N�X�`���̕������i10�����j

#define TIMELIMIT_NUM_DEJIT			(3)									//�������Ԃ̌���
#define TIMELIMIT_MAX_DEJIT			(1000)								//���ő�
#define TIMELIMIT_MAX				(200 * 60)							//������������ �~ �t���[��
#define TIMELIMIT_SPEED				(100)								//�c�萧�����ԂP�O�O�b��BGM�̑��x��ς���

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureTimeLimit = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeLimit = NULL;		//���_�o�b�t�@�ւ̃|�C���^
TimeLimit				g_TimeLimit;					//�������Ԃ̏��

//--------------------------------------------------------
//					�������Ԃ̏���������
//--------------------------------------------------------
void InitTimeLimit(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureTimeLimit);

	//�X�R�A�̏��̏�����
	g_TimeLimit.pos = TIMELIMIT_POS;
	g_TimeLimit.nTime = TIMELIMIT_MAX;
	g_TimeLimit.nDejit = TIMELIMIT_MAX_DEJIT;
	g_TimeLimit.bSoundAlarm = false;
	g_TimeLimit.nFromAlarm = NULL;
	g_TimeLimit.bSoundSpeed = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * TIMELIMIT_NUM_DEJIT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTimeLimit, NULL);

	//���_���W��ݒ�
	SetTimeLimit_Vertex();
}

//--------------------------------------------------------
//					�������Ԃ̏I������
//--------------------------------------------------------
void UninitTimeLimit(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTimeLimit != NULL)
	{
		g_pTextureTimeLimit->Release();
		g_pTextureTimeLimit = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTimeLimit != NULL)
	{
		g_pVtxBuffTimeLimit->Release();
		g_pVtxBuffTimeLimit = NULL;
	}
}

//--------------------------------------------------------
//					�������Ԃ̍X�V����
//--------------------------------------------------------
void UpdateTimeLimit(void)
{
	g_TimeLimit.nTime--;	//�������Ԍ���

	SetTimeLimit_Vertex();	//���_���W��ݒ�

	TimeLimit_Sound_Change();	//�c��̐������Ԃɂ��BGM�̒�~�E�Đ�

	//�������Ԃ��؂ꂽ��Q�[���I��
	Timelimit_TheEnd();
}

//�c��̐������Ԃɂ�鉹�y�̍Đ�
void TimeLimit_Sound_Change(void)
{
	//�c�莞�Ԃ��P�O�O�b�ɂȂ���
	if (g_TimeLimit.nTime / FPS_60 <= TIMELIMIT_SPEED && g_TimeLimit.bSoundAlarm == false)
	{
		StopSound(SOUND_LABEL_GAMEBGM);			//�ʏ�Q�[��BGM���~

		PlaySound(SOUND_LABEL_SE_TIME_ALARM);	//�������ԃA���[�����Đ�
		g_TimeLimit.bSoundAlarm = true;			//�Đ�����
	}

	//�A���[�������Đ����ꂽ				�{��BGM�͗���Ă��Ȃ�
	if (g_TimeLimit.bSoundAlarm == true && g_TimeLimit.bSoundSpeed == false)
	{
		g_TimeLimit.nFromAlarm++;	//�o�ߎ��ԑ���

		//100�t���[���o�߂�����{���a�f�l�𗬂�
		if (TIMELIMIT_SPEED < g_TimeLimit.nFromAlarm)
		{
			PlaySound(SOUND_LABEL_GAMEBGM_SPEED);
			g_TimeLimit.bSoundSpeed = true;			//�{���a�f�l�𗬂���
		}
	}
}

//�������Ԃ̏I������
void Timelimit_TheEnd(void)
{
	//�������Ԃ��؂ꂽ
	if (g_TimeLimit.nTime <= 0)
	{
		SetFade(MODE_RESULT);		//���U���g��ʂ֑J��
		SetEndGame(EndGAME_OVER);	//�Q�[���I�[�o�[�ɐݒ�
	}
}

//--------------------------------------------------------
//					�������Ԃ̕`�揈��
//--------------------------------------------------------
void DrawTimeLimit(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimeLimit, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTimeLimit);

	for (int nCntTimeLimit = START_CNT; nCntTimeLimit < TIMELIMIT_NUM_DEJIT; nCntTimeLimit++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimeLimit * VTX_MAX, SEC);
	}
}

//�������Ԃ̏����擾
TimeLimit *GetTimeLimit(void)
{
	return &g_TimeLimit;	//�������Ԃ̏�������Ԃ�
}

//���_���W��ݒ�
void SetTimeLimit_Vertex(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimeLimit->Lock(0, 0, (void* *)&pVtx, 0);

	//���_���̐ݒ�
	for (int nCntTimeLimit = 0; nCntTimeLimit < TIMELIMIT_NUM_DEJIT; nCntTimeLimit++)
	{
		//�^�C���̊e���𒊏o
		int aTexU = (g_TimeLimit.nTime / FPS_60) % g_TimeLimit.nDejit / (g_TimeLimit.nDejit / DEJIT_DIVISION);

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos.x = (g_TimeLimit.pos.x) + nCntTimeLimit * TIMELIMIT_WIDTH;
			pVtx[VTX_LE_UP].pos.y = (g_TimeLimit.pos.y);
			pVtx[VTX_RI_UP].pos.x = (g_TimeLimit.pos.x + TIMELIMIT_WIDTH) + nCntTimeLimit * TIMELIMIT_WIDTH;
			pVtx[VTX_RI_UP].pos.y = (g_TimeLimit.pos.y);
			pVtx[VTX_LE_DO].pos.x = (g_TimeLimit.pos.x) + nCntTimeLimit * TIMELIMIT_WIDTH;
			pVtx[VTX_LE_DO].pos.y = (g_TimeLimit.pos.y + TIMELIMIT_HEIGHT);
			pVtx[VTX_RI_DO].pos.x = (g_TimeLimit.pos.x + TIMELIMIT_WIDTH) + nCntTimeLimit * TIMELIMIT_WIDTH;
			pVtx[VTX_RI_DO].pos.y = (g_TimeLimit.pos.y + TIMELIMIT_HEIGHT);
		}

		//rhw�̐ݒ�
		{
			pVtx[VTX_LE_UP].rhw = RHW_NUM;
			pVtx[VTX_RI_UP].rhw = RHW_NUM;
			pVtx[VTX_LE_DO].rhw = RHW_NUM;
			pVtx[VTX_RI_DO].rhw = RHW_NUM;
		}

		//���_�J���[�̐ݒ�
		{
			//�������Ԃ�100�b�ȏ�̏ꍇ�̐F
			if (TIMELIMIT_SPEED < g_TimeLimit.nTime / FPS_60)
			{
				pVtx[VTX_LE_UP].col = TIMELIMIT_NORMAL_COLOR;
				pVtx[VTX_RI_UP].col = TIMELIMIT_NORMAL_COLOR;
				pVtx[VTX_LE_DO].col = TIMELIMIT_NORMAL_COLOR;
				pVtx[VTX_RI_DO].col = TIMELIMIT_NORMAL_COLOR;
			}

			//�������Ԃ�100�b�ȉ��̐F
			else
			{
				pVtx[VTX_LE_UP].col = TIMELIMIT_SPEED_COLOR;
				pVtx[VTX_RI_UP].col = TIMELIMIT_SPEED_COLOR;
				pVtx[VTX_LE_DO].col = TIMELIMIT_SPEED_COLOR;
				pVtx[VTX_RI_DO].col = TIMELIMIT_SPEED_COLOR;
			}
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(TIMELIMIT_TEX_DIVISION * aTexU, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(TIMELIMIT_TEX_DIVISION + TIMELIMIT_TEX_DIVISION * aTexU, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(TIMELIMIT_TEX_DIVISION * aTexU, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(TIMELIMIT_TEX_DIVISION + TIMELIMIT_TEX_DIVISION * aTexU, 1.0f);
		}

		pVtx += VTX_MAX;						//���_�f�[�^�̃|�C���^��4���i�߂�
		g_TimeLimit.nDejit /= DEJIT_DIVISION;	//���o���������ɉ�����
	}

	g_TimeLimit.nDejit = TIMELIMIT_MAX_DEJIT;	//�����ő�ɒ���

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTimeLimit->Unlock();
}