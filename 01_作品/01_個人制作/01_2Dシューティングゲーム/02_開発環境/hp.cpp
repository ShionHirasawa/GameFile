/*==========================================================================================================================

												HP�\������[score.cpp]
													Author:���V����

==========================================================================================================================*/
#include "hp.h"
#include "player.h"

//�}�N����`

//HP�ʃe�N�X�`�����W�̃}�N��
#define HP_0		(0.000f)
#define HP_1		(0.166f)
#define HP_2		(0.332f)
#define HP_3		(0.498f)
#define HP_4		(0.664f)
#define HP_MAX		(0.830f)
#define TEX_0		(0.0f)
#define TEX_1		(1.0f)

//�A�C�R���̍��W�}�N��
#define HP_POS_X	(1230.0f)
#define HP_POS_Y	(100.0f)
#define HP_SIZE		(30)						//���S���W�����HP�A�C�R���̑傫��

typedef enum
{
	LIFE_0 = 0,
	LIFE_1,
	LIFE_2,
	LIFE_3,
	LIFE_4,
	LIFE_MAX
}Life;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureHp = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHp = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posHp;								//�X�R�A�̈ʒu

//�X�R�A�̏���������
void InitHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/�̗̓Q�[�W001.png", &g_pTextureHp);

	//HP�̏��̏�����
	g_posHp = D3DXVECTOR3(HP_POS_X, HP_POS_Y, 0.0f);				//HP�̈ʒu�̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHp, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHp->Lock(0, 0, (void* *)&pVtx, 0);

	//���_���W�̐ݒ�
	{
		pVtx[0].pos = D3DXVECTOR3(g_posHp.x - HP_SIZE, g_posHp.y - HP_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posHp.x + HP_SIZE, g_posHp.y - HP_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posHp.x - HP_SIZE, g_posHp.y + HP_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posHp.x + HP_SIZE, g_posHp.y + HP_SIZE, 0.0f);
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�e�N�X�`���̍��W
	{
		pVtx[0].tex = D3DXVECTOR2(HP_MAX, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(TEX_1 , TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_MAX, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(TEX_1 , TEX_1);
	}
   
	//���_�o�b�t�@�����b�N����
	g_pVtxBuffHp->Unlock();
}

//HP�̏I������
void UninitHp(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureHp != NULL)
	{
		g_pTextureHp->Release();
		g_pTextureHp = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHp != NULL)
	{
		g_pVtxBuffHp->Release();
		g_pVtxBuffHp = NULL;
	}
}

//HP�̍X�V����
void UpdateHp(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHp->Lock(0, 0, (void* *)&pVtx, 0);

	Player *pPlayer = GetPlayer();	//�v���C���[�̏��

	//�̗͕ʃe�N�X�`�����W�̍X�V
	switch (pPlayer->nLife)
	{
	case LIFE_0:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_0, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_1, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_0, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_1, TEX_1);
	}
		break;
	case LIFE_1:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_1, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_2, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_1, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_2, TEX_1);
	}
		break;
	case LIFE_2:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_2, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_3, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_2, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_3, TEX_1);
	}
		break;
	case LIFE_3:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_3, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_4, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_3, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_4, TEX_1);
	}
		break;
	case LIFE_4:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_4  , TEX_0);
		pVtx[1].tex = D3DXVECTOR2(HP_MAX, TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_4  , TEX_1);
		pVtx[3].tex = D3DXVECTOR2(HP_MAX, TEX_1);
	}
		break;
	case LIFE_MAX:
	{
		pVtx[0].tex = D3DXVECTOR2(HP_MAX, TEX_0);
		pVtx[1].tex = D3DXVECTOR2(TEX_1 , TEX_0);
		pVtx[2].tex = D3DXVECTOR2(HP_MAX, TEX_1);
		pVtx[3].tex = D3DXVECTOR2(TEX_1 , TEX_1);
	}
		break;
	}
	
	//���_�o�b�t�@�����b�N����
	g_pVtxBuffHp->Unlock();
}

//HP�̕`�揈��
void DrawHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHp, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureHp);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, SEC);
}