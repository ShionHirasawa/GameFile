/*==========================================================================================================================

														���U���g��ʏ���[result.cpp]
															Author:���V����

==========================================================================================================================*/
#include "result.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "fade.h"

//�}�N����`
#define RESULTTIME_LIMIT	(6000)

//�Q�[���I���̎�ލ\����
typedef enum
{
	RESULT_TYPE_OVER = ZERO,	//�Q�[���I�[�o�[
	RESULT_TYPE_CLEAR,			//�Q�[���N���A
	RESULT_TYPE_MAX
}RESULT_TYPE;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureResult[RESULT_TYPE_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//���_�o�b�t�@�ւ̃|�C���^
DWORD g_ResultTime;											//���U���g�\������

void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GAME_OVER.png",  &g_pTextureResult[RESULT_TYPE_OVER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GAME_CLEAR.png", &g_pTextureResult[RESULT_TYPE_CLEAR]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * RESULT_TYPE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCnt = START_CNT; nCnt < RESULT_TYPE_MAX; nCnt++, pVtx += NUM_VERTEX)
	{
		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS , ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS  , SCREEN_HEIGHT, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);
		}

		//rhw�̐ݒ�
		{
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw =  RHW_NUM;
			pVtx[SEC].rhw =  RHW_NUM;
			pVtx[SAN].rhw =  RHW_NUM;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA( COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA( COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA( COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , F_TEN);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffResult->Unlock();

	//���ݎ������擾
	g_ResultTime = timeGetTime();

	if (ResultChoice() == RESULT_TYPE_OVER)
	{
		StopSound(SOUND_LABEL_BGM001);	//�Q�[����ʂ�BGM���~

		PlaySound(SOUND_LABEL_BGM002);	//���S����炷
	}
}

void UninitResult(void)
{
	for (int nCntResult = START_CNT; nCntResult < RESULT_TYPE_MAX; nCntResult++)
	{
		//�e�N�X�`���̔j��
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

void UpdateResult(void)
{
	DWORD EndResultTime = timeGetTime();				//���U���g�\���̐�������
	DWORD ResultTimeDiff = EndResultTime - g_ResultTime;//���U���g��������I���܂ł̍���

	if (GetKeyboardTrigger(DIK_RETURN) == true || ResultTimeDiff > RESULTTIME_LIMIT)
	{//ENTER�L�[�������ꂽ
	 //���[�h�ݒ�i�����L���O��ʂɑJ��)
		SetFade(MODE_RANKING);
	}
}

void DrawResult(void)
{
	int result = ResultChoice();

	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffResult, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureResult[result]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}