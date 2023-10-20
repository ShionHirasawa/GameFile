/*==========================================================================================================================

�|�[�Y��ʏ���[pause.cpp]
Author:���V����

==========================================================================================================================*/
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//�}�N����`
#define NUM_PAUSE		(4)
#define SELECT_COLOR	D3DCOLOR_RGBA(255, 255, 255, 255)	//�I������Ă���Ƃ��̃��j���[�̐F
#define NO_SELECT_COLOR	D3DCOLOR_RGBA(80, 80, 80, 255)		//�I������Ă��Ȃ��Ƃ��̃��j���[�̐F

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTexturePause[NUM_PAUSE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//���_�o�b�t�@�ւ̃|�C���^
PAUSE_MENU g_Pause;											//�I������Ă��郁�j���[

//�|�[�Y�̏���������
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/PauseBG.png", &g_pTexturePause[PAUSE_MENU_BG]);	//�w�i�摜
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/continue.png", &g_pTexturePause[PAUSE_MENU_CONTINUE]);	//�R���e�B�j���[�摜
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/retry.png", &g_pTexturePause[PAUSE_MENU_RETRY]);		//���g���C�摜
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/quit.png", &g_pTexturePause[PAUSE_MENU_LOAD]);			//���[�h�摜

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_PAUSE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�|�[�Y���j���[�̏��X�̏�����
	g_Pause = PAUSE_MENU_CONTINUE;		//�I������Ă���|�[�Y�̏�Ԃ̏�����

	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++, pVtx += NUM_VERTEX)
	{
		switch (nCntPause)
		{
		case PAUSE_MENU_BG:			//�|�[�Y���j���[�w�i
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos = D3DXVECTOR3(200.0f, 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(200.0f, 600.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(1000.0f, 600.0f, 0.0f);
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = RHW_NUM;
				pVtx[1].rhw = RHW_NUM;
				pVtx[2].rhw = RHW_NUM;
				pVtx[3].rhw = RHW_NUM;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = SELECT_COLOR;
				pVtx[1].col = SELECT_COLOR;
				pVtx[2].col = SELECT_COLOR;
				pVtx[3].col = SELECT_COLOR;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_CONTINUE:	//�R���e�B�j���[�{�^��
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos = D3DXVECTOR3(480.0f, 260.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(720.0f, 260.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(480.0f, 340.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(720.0f, 340.0f, 0.0f);
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = SELECT_COLOR;
				pVtx[1].col = SELECT_COLOR;
				pVtx[2].col = SELECT_COLOR;
				pVtx[3].col = SELECT_COLOR;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_RETRY:		//��蒼���{�^��
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos = D3DXVECTOR3(500.0f, 355.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(660.0f, 355.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(500.0f, 445.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(660.0f, 445.0f, 0.0f);
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_LOAD:		//�I���{�^��
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos = D3DXVECTOR3(515.0f, 475.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(645.0f, 475.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(515.0f, 565.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(645.0f, 565.0f, 0.0f);
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPause->Unlock();
}

//�|�[�Y�̏I������
void UninitPause(void)
{
	//�e�N�X�`���̔j��
	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//�|�[�Y�̍X�V����
void UpdatePause(void)
{
	//�I������Ă��郁�j���[�̍X�V(��)
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//�R���e�B�j���[ �� ���g���C
			g_Pause = PAUSE_MENU_RETRY;
			break;

		case PAUSE_MENU_RETRY:				//���g���C �� ���[�h
			g_Pause = PAUSE_MENU_LOAD;
			break;

		case PAUSE_MENU_LOAD:				//���[�h �� �R���e�B�j���[ 
			g_Pause = PAUSE_MENU_CONTINUE;
			break;

		default:							//��Ԃ��R���e�B�j���[�Ɏw��
			g_Pause = PAUSE_MENU_CONTINUE;
			break;
		}
	}

	//�I������Ă��郁�j���[�̍X�V(��)
	else if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//�R���e�B�j���[ �� ���[�h
			g_Pause = PAUSE_MENU_LOAD;
			break;

		case PAUSE_MENU_RETRY:				//���g���C�� �R���e�B�j���[ 
			g_Pause = PAUSE_MENU_CONTINUE;
			break;

		case PAUSE_MENU_LOAD:				//���[�h �� ���g���C
			g_Pause = PAUSE_MENU_RETRY;
			break;

		default:							//��Ԃ��R���e�B�j���[�Ɏw��
			g_Pause = PAUSE_MENU_CONTINUE;
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
	{//ENTER�L�[�������ꂽ
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//�|�[�Y��ʂ����
			break;

		case PAUSE_MENU_RETRY:				//���g���C(�ŏ������蒼��)
			//�Q�[����ʂ��ŏ�����
			SetFade(MODE_GAME);
			break;

		case PAUSE_MENU_LOAD:				//���[�f�B���O��ʂɖ߂�
			SetFade(MODE_LOADING);
			break;
		}
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���j���[�̃t�H���g�̍X�V
	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++, pVtx += NUM_VERTEX)
	{
		switch (nCntPause)
		{
		case PAUSE_MENU_BG:			//�|�[�Y���j���[�w�i
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos = D3DXVECTOR3(200.0f, 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(200.0f, 600.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(1000.0f, 600.0f, 0.0f);
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = SELECT_COLOR;
				pVtx[1].col = SELECT_COLOR;
				pVtx[2].col = SELECT_COLOR;
				pVtx[3].col = SELECT_COLOR;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_CONTINUE:	//�R���e�B�j���[�{�^��
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos = D3DXVECTOR3(480.0f, 260.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(720.0f, 260.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(480.0f, 340.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(720.0f, 340.0f, 0.0f);
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_RETRY:		//��蒼���{�^��
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos = D3DXVECTOR3(500.0f, 355.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(660.0f, 355.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(500.0f, 445.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(660.0f, 445.0f, 0.0f);
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_LOAD:		//�I���{�^��
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos = D3DXVECTOR3(515.0f, 475.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(645.0f, 475.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(515.0f, 565.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(645.0f, 565.0f, 0.0f);
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;
		}

		//�I������Ă��郁�j���[����������
		if (g_Pause == nCntPause)
		{
			pVtx[0].col = SELECT_COLOR;
			pVtx[1].col = SELECT_COLOR;
			pVtx[2].col = SELECT_COLOR;
			pVtx[3].col = SELECT_COLOR;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPause->Unlock();
}

//�|�[�Y�̕`�揈��
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * NUM_PAUSE, SEC);
	}
}

//�|�[�Y��Ԃ̎擾
PAUSE_MENU *GetPause(void)
{
	return &g_Pause;
}

//�|�[�Y��Ԃ̐ݒ�
void SetPause(PAUSE_MENU SetPause)
{
	g_Pause = SetPause;
}