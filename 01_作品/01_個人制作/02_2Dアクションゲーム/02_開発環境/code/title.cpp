/*==========================================================================================================================

�^�C�g����ʂ̏���[title.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "title.h"
#include "input.h"
#include "player.h"
#include "move.h"

//�}�N����`
#define TITLE_POS		(D3DXVECTOR3(100.0f, 100.0f, 0.0f))	//�^�C�g���̈ʒu
#define TITLE_WIDTH		(600.0f)							//�^�C�g���̉���
#define TITLE_HEIGHT	(250.0f)							//�^�C�g���̍���
#define FADE_TITLE_A	(3)									//�^�C�g���̃��l�̕ω���

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9			g_pTextureTitle = NULL;	//�e�N�X�`���̃|�C���^
D3DXVECTOR3 g_TitlePos;								//�^�C�g���̈ʒu
int g_aTexV;										//�e�N�X�`���̃��l

//�^�C�g���̏���������
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^ + �擾

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/�^�C�g�����.png", &g_pTextureTitle);
		
	g_TitlePos = TITLE_POS;	//�^�C�g���̈ʒu������
	g_aTexV = ZERO;			//�e�N�X�`���̃��l������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = g_TitlePos;
		pVtx[ONE].pos = D3DXVECTOR3(g_TitlePos.x + TITLE_WIDTH,   g_TitlePos.y, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_TitlePos.x,  TITLE_HEIGHT + g_TitlePos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_TitlePos.x + TITLE_WIDTH,   g_TitlePos.y + TITLE_HEIGHT, ORIGIN_POS);
	}

	//rhw�̐ݒ�
	{
		pVtx[ZERO].rhw = RHW_NUM;
		pVtx[ONE].rhw = RHW_NUM;
		pVtx[SEC].rhw = RHW_NUM;
		pVtx[SAN].rhw = RHW_NUM;
	}

	//���_�J���[�̐ݒ�   COLOR_MIN
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
	}

	//�e�N�X�`�����W
	{
		pVtx[ZERO].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[ONE].tex  = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[SEC].tex  = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[SAN].tex  = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//�^�C�g���̏I������
void UninitTitle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//�^�C�g���̍X�V����
void UpdateTitle(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��

	//���W�X�V
	moveObject(&g_TitlePos);

	if (fabsf(pPlayer->pos.x - g_TitlePos.x) < 400.0f)
	{
		g_aTexV += FADE_TITLE_A;

		if (g_aTexV > COLOR_MAX)
		{
			g_aTexV = COLOR_MAX;
		}
	}

	else
	{
		g_aTexV -= FADE_TITLE_A;

		if (g_aTexV < COLOR_MIN)
		{
			g_aTexV = COLOR_MIN;
		}
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = g_TitlePos;
		pVtx[ONE].pos = D3DXVECTOR3(g_TitlePos.x + TITLE_WIDTH, g_TitlePos.y, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_TitlePos.x, TITLE_HEIGHT + g_TitlePos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_TitlePos.x + TITLE_WIDTH, g_TitlePos.y + TITLE_HEIGHT, ORIGIN_POS);
	}

	//���_�J���[�̐ݒ�   COLOR_MIN
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aTexV);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//�^�C�g���̕`�揈��
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^ + �擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTitle, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureTitle);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}