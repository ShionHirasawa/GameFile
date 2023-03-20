/*==========================================================================================================================

													�G�t�F�N�g����[bullet.cpp]
														Author:���V����

==========================================================================================================================*/
#include "effect.h"

//�}�N����`
#define MAX_EFFECT		(4096)		//�G�t�F�N�g�̍ő吔
#define EFFECT_VERTEX	(5.0f)		//�G�t�F�N�g�̑傫��
#define EFFECT_LIFE		(50)		//�G�t�F�N�g�̎���

//�G�t�F�N�g�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	float fRadius;			//���a(�傫��)
	int nLife;				//����(�\������)
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Effect;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];							//�G�t�F�N�g�̏��

//�G�t�F�N�g�̏���������
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	//�G�t�F�N�g�̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F�̏�����
		g_aEffect[nCntEffect].fRadius = 0.0f;							//���a(�傫��)�̏�����
		g_aEffect[nCntEffect].nLife = EFFECT_LIFE;						//�����̏�����
		g_aEffect[nCntEffect].bUse = false;								//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//�G�t�F�N�g�̏I������
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//�G�t�F�N�g�̍X�V����
void UpdateEffect(void)
{
	int nCntEffect;

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			//���_���W�̐ݒ�
			{
				pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
				pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
				pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
				pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
				pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
				pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
				pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
				pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = g_aEffect[nCntEffect].col;
				pVtx[1].col = g_aEffect[nCntEffect].col;
				pVtx[2].col = g_aEffect[nCntEffect].col;
				pVtx[3].col = g_aEffect[nCntEffect].col;
			}
			
			//�����Ƒ傫�������炵�Ă���
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].fRadius >= 0.0f)
			{
				g_aEffect[nCntEffect].fRadius -= 0.2f;
			}

			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].fRadius / 200.0f;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//�G�t�F�N�g�̕`�揈��
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * NUM_VERTEX, 2);
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//�G�t�F�N�g�̐ݒ菈��
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�

			//�G�t�F�N�g���̐ݒ�
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = EFFECT_LIFE;
			g_aEffect[nCntEffect].bUse = true;			//�g�p���Ă����Ԃɂ���

			//���_���̐ݒ�
			//���_���W�̐ݒ�
			{
				pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - fRadius;
				pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - fRadius;
				pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + fRadius;
				pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - fRadius;
				pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - fRadius;
				pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + fRadius;
				pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + fRadius;
				pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + fRadius;
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[0].col = g_aEffect[nCntEffect].col;
				pVtx[1].col = g_aEffect[nCntEffect].col;
				pVtx[2].col = g_aEffect[nCntEffect].col;
				pVtx[3].col = g_aEffect[nCntEffect].col;
			}
			
			break;
		}
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEffect->Unlock();
}