/*==========================================================================================================================================================

														�|�[�Y��ʏ���[pause.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "color.h"

//�}�N����`
#define NUM_PAUSE		(4)
#define NO_SELECT_COLOR	D3DCOLOR_RGBA(80, 80, 80, 255)		// �I������Ă��Ȃ��Ƃ��̃��j���[�̐F

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTexturePause[NUM_PAUSE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
PAUSE_MENU				g_Pause;							// �I������Ă��郁�j���[
bool					g_bDispPause;						// �|�[�Y��ʂ�`�悷�邩�ǂ���

//�|�[�Y���j���[�̈ʒu
const D3DXVECTOR3 g_PauseMenuPos[NUM_PAUSE] = {
	D3DXVECTOR3(600.0f, 350.0f, NIL_F),			//�|�[�Y���j���[�̔w�i�ʒu
	D3DXVECTOR3(600.0f, 300.0f, NIL_F),			//�|�[�Y���j���[��"Continue" �ʒu
	D3DXVECTOR3(580.0f, 400.0f, NIL_F),			//�|�[�Y���j���[��"Retry" �ʒu
	D3DXVECTOR3(580.0f, 520.0f, NIL_F),			//�|�[�Y���j���[��"Quit" �ʒu
};

//�|�[�Y���j���[��UI�̕�
const float g_PauseMenuWidth[NUM_PAUSE] = {
	400.0f,										//�|�[�Y���j���[�̔w�i�̕�
	120.0f,										//�|�[�Y���j���["Continue" �̕�
	80.0f,										//�|�[�Y���j���["Retry" �̕�
	65.0f,										//�|�[�Y���j���["Quit" �̕�
};

//�|�[�Y���j���[��UI�̍���
const float g_PauseMenuHeight[NUM_PAUSE] = {
	250.0f,										//�|�[�Y���j���[�̔w�i�̍���
	40.0f,										//�|�[�Y���j���["Continue" �̍���
	45.0f,										//�|�[�Y���j���["Retry" �̍���
	45.0f,										//�|�[�Y���j���["Quit" �̍���
};

//�|�[�Y���j���[�ɕ\������UI�̃t�@�C����
const char *c_apPauseTextureName[NUM_PAUSE] = 
{
	"data/TEXTURE/�|�[�Y��ʔw�i.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/retry.png",
	"data/TEXTURE/quit.png",
};

//------------------------------------------------
//			�|�[�Y���j���[�̏���������
//------------------------------------------------
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_PAUSE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�|�[�Y���j���[�̏��X�̏�����
	g_Pause = PAUSE_MENU_CONTINUE;		// �I������Ă���|�[�Y�̏�Ԃ̏�����
	g_bDispPause = true;				// �|�[�Y��ʂ�`�悷��

	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++, pVtx += VTX_MAX)
	{
		//�e�N�X�`���摜�̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apPauseTextureName[nCntPause], &g_pTexturePause[nCntPause]);

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_PauseMenuPos[nCntPause].x - g_PauseMenuWidth[nCntPause], g_PauseMenuPos[nCntPause].y - g_PauseMenuHeight[nCntPause], NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_PauseMenuPos[nCntPause].x + g_PauseMenuWidth[nCntPause], g_PauseMenuPos[nCntPause].y - g_PauseMenuHeight[nCntPause], NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_PauseMenuPos[nCntPause].x - g_PauseMenuWidth[nCntPause], g_PauseMenuPos[nCntPause].y + g_PauseMenuHeight[nCntPause], NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_PauseMenuPos[nCntPause].x + g_PauseMenuWidth[nCntPause], g_PauseMenuPos[nCntPause].y + g_PauseMenuHeight[nCntPause], NIL_F);
		}

		//rhw�̐ݒ�
		{
			pVtx[VTX_LE_UP].rhw = RHW_NUM;
			pVtx[VTX_RI_UP].rhw = RHW_NUM;
			pVtx[VTX_LE_DO].rhw = RHW_NUM;
			pVtx[VTX_RI_DO].rhw = RHW_NUM;
		}

		//�I������Ă��郁�j���[�Ɣw�i����������
		if (g_Pause == nCntPause || nCntPause == PAUSE_MENU_BG)
		{
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}

		//�I������Ă��Ȃ����͈̂Â�����
		else
		{
			pVtx[0].col = NO_SELECT_COLOR;
			pVtx[1].col = NO_SELECT_COLOR;
			pVtx[2].col = NO_SELECT_COLOR;
			pVtx[3].col = NO_SELECT_COLOR;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = ORIGIN_TEX;
			pVtx[VTX_RI_UP].tex = MA_ZE_TEX;
			pVtx[VTX_LE_DO].tex = ZE_MA_TEX;
			pVtx[VTX_RI_DO].tex = LIMIT_TEX;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPause->Unlock();
}

//------------------------------------------------
//			�|�[�Y���j���[�̏I������
//------------------------------------------------
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

//------------------------------------------------
//			�|�[�Y���j���[�̍X�V����
//------------------------------------------------
void UpdatePause(void)
{
	//�|�[�Y���j���[���\������Ă���
	if (g_bDispPause == true)
	{
		//�I������Ă��郁�j���[�̍X�V(��)
		SelectPauseMenu_Down();

		//�I������Ă��郁�j���[�̍X�V(��)
		SelectPauseMenu_UP();

		//���ڌ���
		SelectPauseMenu_Decide();

		//�|�C���^��ݒ�
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPause->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

		//���j���[��UI�̍X�V
		for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++, pVtx += VTX_MAX)
		{
			//�I������Ă��郁�j���[�Ɣw�i����������
			if (g_Pause == nCntPause || nCntPause == PAUSE_MENU_BG)
			{
				pVtx[VTX_LE_UP].col = XCOL_WHITE;
				pVtx[VTX_RI_UP].col = XCOL_WHITE;
				pVtx[VTX_LE_DO].col = XCOL_WHITE;
				pVtx[VTX_RI_DO].col = XCOL_WHITE;
			}

			//�I������Ă��Ȃ����͈̂Â�����
			else
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}
		}

		//���_�o�b�t�@�����b�N����
		g_pVtxBuffPause->Unlock();
	}

	//�|�[�Y��ʕ\�� ON / OFF
	if (GetKeyboardTrigger(DIK_F9) == true)
	{
		g_bDispPause = g_bDispPause ? false : true;
	}
}

//�|�[�Y���j���[���Ɉړ�
void SelectPauseMenu_Down(void)
{
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//�R���e�B�j���[ �� ���g���C
			g_Pause = PAUSE_MENU_RETRY;
			break;

		case PAUSE_MENU_RETRY:				//���g���C �� ���[�h
			g_Pause = PAUSE_MENU_TITLE;
			break;

		case PAUSE_MENU_TITLE:				//���[�h �� �R���e�B�j���[ 
			g_Pause = PAUSE_MENU_CONTINUE;
			break;

		default:							//��Ԃ��R���e�B�j���[�Ɏw��
			g_Pause = PAUSE_MENU_CONTINUE;
			break;
		}
	}
}

//�|�[�Y���j���[��Ɉړ�
void SelectPauseMenu_UP(void)
{
	if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//�R���e�B�j���[ �� �^�C�g��
			g_Pause = PAUSE_MENU_TITLE;
			break;

		case PAUSE_MENU_RETRY:				//���g���C�� �R���e�B�j���[ 
			g_Pause = PAUSE_MENU_CONTINUE;
			break;

		case PAUSE_MENU_TITLE:				//�^�C�g�� �� ���g���C
			g_Pause = PAUSE_MENU_RETRY;
			break;

		default:							//��Ԃ��R���e�B�j���[�Ɏw��
			g_Pause = PAUSE_MENU_CONTINUE;
			break;
		}
	}
}

//�|�[�Y���j���[����
void SelectPauseMenu_Decide(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
	{//ENTER�L�[�������ꂽ
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:		//�|�[�Y��ʂ����
			break;

		case PAUSE_MENU_RETRY:			//���g���C(�ŏ������蒼��)
			SetFade(MODE_GAME);			//�Q�[����ʂ��ŏ�����
			break;

		case PAUSE_MENU_TITLE:			//�^�C�g����ʂɖ߂�
			SetFade(MODE_TITLE);
			break;
		}
	}
}

//------------------------------------------------
//			�|�[�Y���j���[�̕`�揈��
//------------------------------------------------
void DrawPause(void)
{
	//�|�[�Y��ʂ̕`��ON�@�Ȃ�`�悷��
	if (g_bDispPause == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(ZERO, g_pVtxBuffPause, ZERO, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTexturePause[nCntPause]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * VTX_MAX, SEC);
		}
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

//�|�[�Y��ʂ�\�����邩�ǂ���
bool GetDispPause(void)
{
	return g_bDispPause;	//�\���X�C�b�`��Ԃ�
}