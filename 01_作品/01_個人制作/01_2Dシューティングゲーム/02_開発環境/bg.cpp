/*==========================================================================================================================

												�Q�[���w�i�̏���[bg.cpp]
													Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "Bg.h"
#include "input.h"

//�}�N����`
#define NUM_BG		(3)			//�w�i�̐�
#define F_MOVE_ONE	(0.001f)
#define F_MOVE_SEC	(0.002f)
#define F_MOVE_SAN	(0.003f)


//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureBg[NUM_BG] = { NULL, NULL ,NULL};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBg = NULL;						//���_�o�b�t�@�ւ̃|�C���^		
float g_aTexV[NUM_BG];													//�e�N�X�`�����W�̊J�n�ʒu�iV�l�j
int g_BackAnimNum;
int g_BackTexNum;

//�w�i�̏���������
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntBG;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureBg[ZERO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureBg[ONE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureBg[SEC]);

	//�e�N�X�`�����W�J�n�ʒu�iV�l�j�̏�����
	for(nCntBG = START_CNT;nCntBG < NUM_BG;nCntBG++)
	{
		g_aTexV[nCntBG] = F_ZERO;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_BG, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBG = ZERO; nCntBG < NUM_BG; nCntBG++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, ORIGIN_POS);
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
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX , COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX , COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX , COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, g_aTexV[nCntBG]);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , g_aTexV[nCntBG]);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , g_aTexV[nCntBG] + F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , g_aTexV[nCntBG] + F_TEN);
		}

		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBg->Unlock();
}

//�w�i�̏I������
void UninitBg(void)
{
	//�e�N�X�`��(�R����)�̔j��
	for (int nCntBG = START_CNT; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_pTextureBg[nCntBG] != NULL)
		{
			g_pTextureBg[nCntBG]->Release();
			g_pTextureBg[nCntBG] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg != NULL)
	{			  
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//�w�i�̍X�V����
void UpdateBg(void)
{
	int nCntBG;
	D3DXVECTOR2 moveWidth (F_MOVE_SAN, -F_MOVE_SAN);			//�w�i�����ɓ�����

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBG = START_CNT; nCntBG < NUM_BG; nCntBG++)
	{
		//�e�N�X�`�����W�̊J�n�ʒu�̍X�V
		g_aTexV[nCntBG] -= F_MOVE_ONE + (nCntBG * F_MOVE_SEC);

		if (GetKeyboardPress(DIK_A) == true)
		{
			pVtx[ZERO].tex.x -= moveWidth.x;
			pVtx[ONE].tex.x  -= moveWidth.x;
			pVtx[SEC].tex.x  -= moveWidth.x;
			pVtx[SAN].tex.x  -= moveWidth.x;
			pVtx[ZERO].tex.y += moveWidth.y;
			pVtx[ONE].tex.y  += moveWidth.y;
			pVtx[SEC].tex.y  += moveWidth.y;
			pVtx[SAN].tex.y  += moveWidth.y;

			g_aTexV[nCntBG] += moveWidth.y + F_MOVE_ONE + (nCntBG * F_MOVE_SEC);
		}

		else if (GetKeyboardPress(DIK_D) == true)
		{
			pVtx[ZERO].tex += moveWidth;
			pVtx[ONE].tex += moveWidth;
			pVtx[SEC].tex += moveWidth;
			pVtx[SAN].tex += moveWidth;

			g_aTexV[nCntBG] += moveWidth.y + F_MOVE_ONE + (nCntBG * F_MOVE_SEC);
		}

		//�e�N�X�`���̍��W
		else
		{
			pVtx[ZERO].tex.y = g_aTexV[nCntBG];
			pVtx[ONE].tex.y = g_aTexV[nCntBG];
			pVtx[SEC].tex.y = g_aTexV[nCntBG] + F_TEN;
			pVtx[SAN].tex.y = g_aTexV[nCntBG] + F_TEN;
		}

		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBg->Unlock();
}

//�w�i�̕`�揈��
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntBG;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBg, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = START_CNT; nCntBG < NUM_BG; nCntBG++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureBg[nCntBG]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * NUM_VERTEX, SEC);
	}
}