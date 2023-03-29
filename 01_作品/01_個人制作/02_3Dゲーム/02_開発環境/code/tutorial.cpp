/*==========================================================================================================================================================

�`���[�g���A������[tutorial.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "color.h"
#include "sound.h"

#define TUTORIAL_MOVE_SPEED	(20.0f)	//�`���[�g���A���摜�̈ړ��X�s�[�h

//�`���[�g���A���摜�̎�ޕ���
typedef enum
{
	TUTO_TEX_PLAYER = ZERO,		//�v���C���[�̃`���[�g���A��
	TUTO_TEX_CAMERA,			//�J�����E���̑��̃`���[�g���A��
	TUTO_TEX_MAX
}TUTO_TEX;

//�`���[�g���A���y�[�W�̍\����
typedef enum
{
	TutorialPage_LEFT = ZERO,	//���y�[�W�ɂ���
	TutorialPage_TurnLEFT,		//���߂���
	TutorialPage_RIGHT,			//�E�y�[�W�ɂ���
	TutorialPage_TurnRIGHT,		//�E�߂���
	TutorialPage_MAX,
}TutorialPage;

//�y�[�W���̍\����
typedef struct
{
	D3DXVECTOR3 pos;
}Tutorial;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureTutorial[TUTO_TEX_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				//���_�o�b�t�@�ւ̃|�C���^
TutorialPage			g_TutorialPage;							//�y�[�W�̏��
Tutorial				g_Tutorial[TUTO_TEX_MAX];				//�`���[�g���A���̏��

//�`���[�g���A���摜�̃e�N�X�`���t�@�C����
const char *c_apTutorialTexName[TUTO_TEX_MAX] = {
	"data/TEXTURE/tutorial000.png",
	"data/TEXTURE/tutorial001.png",
};

//------------------------------------------------
//				�`���[�g���A������������
//------------------------------------------------
void InitTutorial(void)
{
	//�y�[�W�̏�����
	g_TutorialPage = TutorialPage_LEFT;	//���y�[�W��Ԃɐݒ�

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * TUTO_TEX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�`���[�g���A�����̏�����
	for (int nCntTutorial = START_CNT; nCntTutorial < TUTO_TEX_MAX; nCntTutorial++, pVtx += VTX_MAX)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, c_apTutorialTexName[nCntTutorial], &g_pTextureTutorial[nCntTutorial]);

		g_Tutorial[nCntTutorial].pos = D3DXVECTOR3(SCREEN_WIDTH * nCntTutorial, NIL_F, NIL_F);

		//���_���W�̐ݒ�
		//�]���W�̐ݒ�
		pVtx[VTX_LE_UP].pos.x = g_Tutorial[nCntTutorial].pos.x;
		pVtx[VTX_RI_UP].pos.x = g_Tutorial[nCntTutorial].pos.x + SCREEN_WIDTH;
		pVtx[VTX_LE_DO].pos.x = g_Tutorial[nCntTutorial].pos.x;
		pVtx[VTX_RI_DO].pos.x = g_Tutorial[nCntTutorial].pos.x + SCREEN_WIDTH;

		//�x���W�̐ݒ�
		pVtx[VTX_LE_UP].pos.y = NIL_F;
		pVtx[VTX_RI_UP].pos.y = NIL_F;
		pVtx[VTX_LE_DO].pos.y = SCREEN_HEIGHT;
		pVtx[VTX_RI_DO].pos.y = SCREEN_HEIGHT;

		//Z���W�̐ݒ�
		pVtx[VTX_LE_UP].pos.z = NIL_F;
		pVtx[VTX_RI_UP].pos.z = NIL_F;
		pVtx[VTX_LE_DO].pos.z = NIL_F;
		pVtx[VTX_RI_DO].pos.z = NIL_F;

		//rhw�̐ݒ�
		pVtx[VTX_LE_UP].rhw = RHW_NUM;
		pVtx[VTX_RI_UP].rhw = RHW_NUM;
		pVtx[VTX_LE_DO].rhw = RHW_NUM;
		pVtx[VTX_RI_DO].rhw = RHW_NUM;

		//���_�J���[�̐ݒ�
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//�e�N�X�`���̍��W
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(F_TEN, F_ZERO);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(F_ZERO, F_TEN);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(F_TEN, F_TEN);
	}
	
	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTutorial->Unlock();
}

//------------------------------------------------
//				�`���[�g���A���I������
//------------------------------------------------
void UninitTutorial(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = START_CNT; nCntTex < TUTO_TEX_MAX; nCntTex++)
	{
		if (g_pTextureTutorial[nCntTex] != NULL)
		{
			g_pTextureTutorial[nCntTex]->Release();
			g_pTextureTutorial[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//�^�C�g��BGM��~
	StopSound(SOUND_LABEL_TITLEBGM);
}

//------------------------------------------------
//				�`���[�g���A���X�V����
//------------------------------------------------
void UpdateTutorial(void)
{
	//�L�[�{�[�h��ENTER�@���@�Q�[���p�b�h�́@A�{�^���@���@START�{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true &&
		(g_TutorialPage == TutorialPage_LEFT || g_TutorialPage == TutorialPage_RIGHT))	//�y�[�W�����E�ǂ��炩���\������Ă����Ԃł���
	{//ENTER�L�[�������ꂽ
		//�{����鉹���Đ�
		PlaySound(SOUND_LABEL_SE_PAGE_Close);

		//���[�h�ݒ�i�Q�[����ʂɑJ��)
		SetFade(MODE_GAME);
	}

	//������Ă��Ȃ��Ƃ��Ƀy�[�W���߂���
	else
	{
		//�؁[�����߂��鏈��
		TurnPageTutorial();
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�摜�ʒu�X�V
	for (int nCntTutorial = START_CNT; nCntTutorial < TUTO_TEX_MAX; nCntTutorial++, pVtx += VTX_MAX)
	{
		//�]���W�̐ݒ�
		pVtx[VTX_LE_UP].pos.x = g_Tutorial[nCntTutorial].pos.x;
		pVtx[VTX_RI_UP].pos.x = g_Tutorial[nCntTutorial].pos.x + SCREEN_WIDTH;
		pVtx[VTX_LE_DO].pos.x = g_Tutorial[nCntTutorial].pos.x;
		pVtx[VTX_RI_DO].pos.x = g_Tutorial[nCntTutorial].pos.x + SCREEN_WIDTH;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTutorial->Unlock();
}

//�y�[�W���߂���
void TurnPageTutorial(void)
{
	/*-------------------------------------------
				�y�[�W���߂��锻��
	---------------------------------------------*/
	//���ݍ��y�[�W�ɂ���A�E���͂������ꂽ
	if (g_TutorialPage == TutorialPage_LEFT && (GetKeyboardTrigger(DIK_RIGHT) || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT)) == true)
	{
		//�E�y�[�W�ւ߂���  �����Đ�
		g_TutorialPage = TutorialPage_TurnRIGHT;
		PlaySound(SOUND_LABEL_SE_PAGE_FlipRight);
	}

	//���݉E�y�[�W�ɂ���A�����͂������ꂽ
	else if (g_TutorialPage == TutorialPage_RIGHT && (GetKeyboardTrigger(DIK_LEFT) || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_LEFT)) == true)
	{
		//���y�[�W�ւ߂���  �����Đ�
		g_TutorialPage = TutorialPage_TurnLEFT;
		PlaySound(SOUND_LABEL_SE_PAGE_FlipLeft);
	}

	/*-------------------------------------------
				�y�[�W���߂��鏈��
	---------------------------------------------*/
	//�E�y�[�W�ɂ߂���
	if (g_TutorialPage == TutorialPage_TurnRIGHT)
	{
		//�y�[�W���ړ�������
		for (int nCntPage = START_CNT; nCntPage < TUTO_TEX_MAX; nCntPage++)
		{
			g_Tutorial[nCntPage].pos.x -= TUTORIAL_MOVE_SPEED;
		}

		//�摜�̂P���ڂ���ʂ̍L�������ɍs�������A�Q���ڂ����_�ʒu��荶�ɍs���Ă��܂���
		if (g_Tutorial[TUTO_TEX_PLAYER].pos.x <= -SCREEN_WIDTH || g_Tutorial[TUTO_TEX_CAMERA].pos.x <= ORIGIN_POS)
		{
			//�E�y�[�W�ɂ���
			g_TutorialPage = TutorialPage_RIGHT;

			//���ꂼ��̉摜�̈ʒu�C��
			g_Tutorial[TUTO_TEX_PLAYER].pos.x = -SCREEN_WIDTH;
			g_Tutorial[TUTO_TEX_CAMERA].pos.x = ORIGIN_POS;
		}
	}

	//���y�[�W�ɂ߂���
	else if (g_TutorialPage == TutorialPage_TurnLEFT)
	{
		//�y�[�W���ړ�������
		for (int nCntPage = START_CNT; nCntPage < TUTO_TEX_MAX; nCntPage++)
		{
			g_Tutorial[nCntPage].pos.x += TUTORIAL_MOVE_SPEED;
		}

		//�摜�̂P���ڂ����_�ʒu���E�ɍs�������A�Q���ڂ���ʂ̉E�[���E�ɍs���Ă��܂�����
		if (g_Tutorial[TUTO_TEX_PLAYER].pos.x >= ORIGIN_POS || g_Tutorial[TUTO_TEX_CAMERA].pos.x >= SCREEN_WIDTH)
		{
			//�E�y�[�W�ɂ���
			g_TutorialPage = TutorialPage_LEFT;

			//���ꂼ��̉摜�̈ʒu�C��
			g_Tutorial[TUTO_TEX_PLAYER].pos.x = ORIGIN_POS;
			g_Tutorial[TUTO_TEX_CAMERA].pos.x = SCREEN_WIDTH;
		}
	}

}

//------------------------------------------------
//				�`���[�g���A���`�揈��
//------------------------------------------------
void DrawTutorial(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTutorial, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = START_CNT; nCntTex < TUTO_TEX_MAX; nCntTex++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureTutorial[nCntTex]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * VTX_MAX, SEC);

	}
}