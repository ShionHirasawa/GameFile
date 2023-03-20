/*==========================================================================================================================================================

�X�R�A�\������[score.cpp]
Author:���V����

============================================================================================================================================================*/
#include "score.h"
#include "input.h"

//�}�N����`
#define NUM_DEJIT_PLACE			(8)									//�ő包��
#define MAX_DEJIT				(100000000)							//�ő�̌�

#define SCORE_POS				(D3DXVECTOR3(1000.0f, 3.0f, 0.0f))	//�X�R�A�̈ʒu
#define SCORE_WIDTH				(35.0f)								//��
#define SCORE_HEIGHT			(60.0f)								//����
#define SCORE_TEX_DIVISION		(0.1f)								//�e�N�X�`���̕������i10�����j
#define SCORE_COLOR				(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f))	//�X�R�A�̐F

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Score					g_Score;					//�X�R�A�̏��

//--------------------------------------------------------
//					�X�R�A�̏���������
//--------------------------------------------------------
void InitScore(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	//�X�R�A�̏��̏�����
	g_Score.pos = SCORE_POS;
	g_Score.nScore = NULL;
	FixDejit();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_DEJIT_PLACE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void* *)&pVtx, 0);

	//���_���̐ݒ�
	for (int nCntScore = 0; nCntScore < NUM_DEJIT_PLACE; nCntScore++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos.x = (g_Score.pos.x) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_LE_UP].pos.y = (g_Score.pos.y);
			pVtx[VTX_RI_UP].pos.x = (g_Score.pos.x + SCORE_WIDTH) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_RI_UP].pos.y = (g_Score.pos.y);
			pVtx[VTX_LE_DO].pos.x = (g_Score.pos.x) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_LE_DO].pos.y = (g_Score.pos.y + SCORE_HEIGHT);
			pVtx[VTX_RI_DO].pos.x = (g_Score.pos.x + SCORE_WIDTH) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_RI_DO].pos.y = (g_Score.pos.y + SCORE_HEIGHT);
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
			pVtx[VTX_LE_UP].col = SCORE_COLOR;
			pVtx[VTX_RI_UP].col = SCORE_COLOR;
			pVtx[VTX_LE_DO].col = SCORE_COLOR;
			pVtx[VTX_RI_DO].col = SCORE_COLOR;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(SCORE_TEX_DIVISION * g_Score.nScore, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(SCORE_TEX_DIVISION + SCORE_TEX_DIVISION * g_Score.nScore, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(SCORE_TEX_DIVISION * g_Score.nScore, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(SCORE_TEX_DIVISION + SCORE_TEX_DIVISION * g_Score.nScore, 1.0f);
		}

		pVtx += VTX_MAX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffScore->Unlock();
}

//--------------------------------------------------------
//					�X�R�A�̏I������
//--------------------------------------------------------
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//--------------------------------------------------------
//					�X�R�A�̍X�V����
//--------------------------------------------------------
void UpdateScore(void)
{

}

//--------------------------------------------------------
//					�X�R�A�̕`�揈��
//--------------------------------------------------------
void DrawScore(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntScore = START_CNT; nCntScore < NUM_DEJIT_PLACE; nCntScore++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * VTX_MAX, SEC);
	}
}

//�X�R�A�̉��Z����
void AddScore(int nValue)
{
	g_Score.nScore += nValue;		//�X�R�A�����Z

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���̐ݒ�
	for (int nCntScore = 0; nCntScore < NUM_DEJIT_PLACE; nCntScore++)
	{
		//�X�R�A�̌����𒊏o
		int aTexU = g_Score.nScore % g_Score.Dejit / (g_Score.Dejit / DEJIT_DIVISION);

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos.x = (g_Score.pos.x) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_LE_UP].pos.y = (g_Score.pos.y);
			pVtx[VTX_RI_UP].pos.x = (g_Score.pos.x + SCORE_WIDTH) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_RI_UP].pos.y = (g_Score.pos.y);
			pVtx[VTX_LE_DO].pos.x = (g_Score.pos.x) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_LE_DO].pos.y = (g_Score.pos.y + SCORE_HEIGHT);
			pVtx[VTX_RI_DO].pos.x = (g_Score.pos.x + SCORE_WIDTH) + nCntScore * SCORE_WIDTH;
			pVtx[VTX_RI_DO].pos.y = (g_Score.pos.y + SCORE_HEIGHT);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(SCORE_TEX_DIVISION * aTexU, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(SCORE_TEX_DIVISION + SCORE_TEX_DIVISION * aTexU, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(SCORE_TEX_DIVISION * aTexU, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(SCORE_TEX_DIVISION + SCORE_TEX_DIVISION * aTexU, 1.0f);
		}

		pVtx += VTX_MAX;					//���_�f�[�^�̃|�C���^��4���i�߂�
		g_Score.Dejit /= DEJIT_DIVISION;	//�������������
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffScore->Unlock();

	//�ő包���ɏ�����
	FixDejit();
}

//�����𒼂�����
void FixDejit(void)
{
	//�ő包���ɏ�����
	g_Score.Dejit = MAX_DEJIT;
}

//�X�R�A�̒l���l��
Score *GetScore(void)
{
	return &g_Score;	//�X�R�A������Ԃ�
}