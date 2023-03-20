/*==========================================================================================================================================================

														���U���g��ʏ���[result.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "input.h"
#include "fade.h"
#include "game.h"

#define NUM_RESULT			(2)		//���U���g�摜�̎��

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;				//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9			g_pTextureResult[EndGAME_MAX] = {};		//�e�N�X�`���̃|�C���^

const char *c_apResultTexName[EndGAME_MAX] = {
	"data/TEXTURE/GAME_CLEAR.png",
	"data/TEXTURE/GAME_OVER.png",
};

//------------------------------------------------
//				���U���g�̏���������
//------------------------------------------------
void InitResult(void)
{
	//�f�o�C�X�ւ̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_RESULT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�e�N�X�`��(�Q��)�̓ǂݍ���
	for (int nCntRuselt = START_CNT; nCntRuselt < EndGAME_MAX; nCntRuselt++, pVtx += VTX_MAX)
	{
		D3DXCreateTextureFromFile(pDevice, c_apResultTexName[nCntRuselt], &g_pTextureResult[nCntRuselt]);

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);
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
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
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
	g_pVtxBuffResult->Unlock();
}

//------------------------------------------------
//				���U���g�̏I������
//------------------------------------------------
void UninitResult(void)
{
	//�e�N�X�`��(�Q��)�̔j��
	for (int nCntResult = START_CNT; nCntResult < EndGAME_MAX; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//------------------------------------------------
//				���U���g�̍X�V����
//------------------------------------------------
void UpdateResult(void)
{
	//Enter�������ꂽ�@�������́@�Q�[���p�b�h��START or A �{�^���������ꂽ
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
	{
		//�^�C�g����ʂɈڍs
		SetFade(MODE_TITLE);
	}
}

//------------------------------------------------
//				���U���g�̕`�揈��
//------------------------------------------------
void DrawResult(void)
{
	//���U���g�̎�ގ擾
	int nTypeResult = EndGameResult();

	//�f�o�C�X�̃|�C���^ + �擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffResult, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureResult[nTypeResult]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}