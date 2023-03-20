/*==========================================================================================================================================================

														���C�t�\������[life.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "life.h"
#include "player.h"

//�A�C�R���̍��W�}�N��
#define LIFE_POS_X			(1120.0f)
#define LIFE_POS_Y			(680.0f)
#define LIFE_WIDTH			(140.0f)			//���S���W�����HP�A�C�R���̕�
#define LIFE_HEIGHT			(25.0f)				//���S���W�����HP�A�C�R���̍���
#define SEPALATE_LIFE_TEX	(7)					//���C�t�A�C�R���̃e�N�X�`��������

//�c��̗�
typedef enum
{
	Remain_Life_NONE = ZERO,
	Remain_Life_1,
	Remain_Life_2,
	Remain_Life_3,
	Remain_Life_4,
	Remain_Life_MAX,
}Remain_Life;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureLife = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_posLife;					//���C�t�̈ʒu

//�c��̗͕ʃe�N�X�`�����W�̊J�n�ʒu
const float c_apStartTexLife[SEPALATE_LIFE_TEX] = {
	0.000f,
	0.166f,
	0.332f,
	0.498f,
	0.664f,
	0.830f,
	1.000f,
};

//--------------------------------------------------------
//					���C�t�̏���������
//--------------------------------------------------------
void InitLife(void)
{
	//�f�o�C�X�̏��擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/lifegage002.png", &g_pTextureLife);
	
	//HP�̈ʒu�̏�����
	g_posLife = D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y, NIL_F);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLife, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_posLife.x - LIFE_WIDTH, g_posLife.y - LIFE_HEIGHT, NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_posLife.x + LIFE_WIDTH, g_posLife.y - LIFE_HEIGHT, NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_posLife.x - LIFE_WIDTH, g_posLife.y + LIFE_HEIGHT, NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_posLife.x + LIFE_WIDTH, g_posLife.y + LIFE_HEIGHT, NIL_F);
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
		pVtx[VTX_LE_UP].col = D3DXCOLOR(MAX_F, MAX_F, MAX_F, MAX_F);
		pVtx[VTX_RI_UP].col = D3DXCOLOR(MAX_F, MAX_F, MAX_F, MAX_F);
		pVtx[VTX_LE_DO].col = D3DXCOLOR(MAX_F, MAX_F, MAX_F, MAX_F);
		pVtx[VTX_RI_DO].col = D3DXCOLOR(MAX_F, MAX_F, MAX_F, MAX_F);
	}

	//�e�N�X�`���̍��W
	{//�ő�̗͕�����\��
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(NIL_F, c_apStartTexLife[Remain_Life_MAX]);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_F, c_apStartTexLife[Remain_Life_MAX]);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(NIL_F, c_apStartTexLife[Remain_Life_MAX + ONE]);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_F, c_apStartTexLife[Remain_Life_MAX + ONE]);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffLife->Unlock();
}

//--------------------------------------------------------
//					���C�t�̏I������
//--------------------------------------------------------
void UninitLife(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//--------------------------------------------------------
//					���C�t�̍X�V����
//--------------------------------------------------------
void UpdateLife(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�v���C���[�̗͎̑擾
	int nLife = GetPlayer()->nLife;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[VTX_LE_UP].tex = D3DXVECTOR2(NIL_F, c_apStartTexLife[nLife]);
	pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_F, c_apStartTexLife[nLife]);
	pVtx[VTX_LE_DO].tex = D3DXVECTOR2(NIL_F, c_apStartTexLife[nLife + ONE]);
	pVtx[VTX_RI_DO].tex = D3DXVECTOR2(MAX_F, c_apStartTexLife[nLife + ONE]);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffLife->Unlock();
}

//--------------------------------------------------------
//					���C�t�̕`�揈��
//--------------------------------------------------------
void DrawLife(void)
{
	//�f�o�C�X�̏��擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffLife, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureLife);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}