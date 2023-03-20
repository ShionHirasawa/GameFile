/*==========================================================================================================================

												�p�[�e�B�N������[main.cpp]
													Author:���V����

==========================================================================================================================*/
#include "explosion.h"
#include "particle.h"

//�}�N����`
#define MAX_PARTICLE				(2048)			//�p�[�e�B�N���̍ő吔
#define PARTICLE_LIFE				(100)			//�p�[�e�B�N���̎���
#define MAX_RADIUS					(30.0f)			//���a�̏����l
#define MINUS_RADIUS				(1.0f)			//���a�̏k���l
#define ANGLE						(629)			//�p�[�e�B�N�����ړ��������
#define FIX_ANGLE					(314)			//ANGLE�̊p�x���C��
#define FIX_FLOAT					(100.0f)		//�p�x��float�^�ɏC��
#define MOVE_PARTICLE				(5.0f)			//�p�[�e�B�N���̊�{�ړ���

//�p�[�e�B�N���̍\����
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu(�����ʒu)
	D3DXVECTOR3 move;				//�ړ���
	D3DXCOLOR col;					//�F
	int nLife;						//����(��������)
	float fRadius;					//�p�[�e�B�N���̔��a
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Particle;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Particle g_aParticle[MAX_PARTICLE];					//�p�[�e�B�N���̏��

//�p�[�e�B�N���̏�����
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntParticle;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureParticle);

	//�p�[�e�B�N���̏��̏�����
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ̏�����
		g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;					//�����̏�����
		g_aParticle[nCntParticle].fRadius = MAX_RADIUS;						//���a�̏�����
		g_aParticle[nCntParticle].bUse = false;								//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffParticle, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
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
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f);
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
	g_pVtxBuffParticle->Unlock();
}

//�p�[�e�B�N���̏I������
void UninitParticle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//�p�[�e�B�N���̍X�V����
void UpdateParticle(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += NUM_VERTEX)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//�p�[�e�B�N�����ړ�
			g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
			g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;

			//���a���k��
			g_aParticle[nCntParticle].fRadius -= MINUS_RADIUS;

			//����������������
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].fRadius <= 0.0f || g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}

			//���_���W�̐ݒ�
			{
				pVtx[0].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
				pVtx[0].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
				pVtx[1].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
				pVtx[1].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
				pVtx[2].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
				pVtx[2].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;
				pVtx[3].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
				pVtx[3].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;
			}			
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffParticle->Unlock();
}

//�p�[�e�B�N���̕`�揈��
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureParticle);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * NUM_VERTEX, 2);
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//�p�[�e�B�N���̐ݒ菈��
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, ParticleType nType)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(0, 0, (void* *)&pVtx, 0);
		
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += NUM_VERTEX)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//�p�[�e�B�N�����g�p����Ă��Ȃ�
			float fAngleParticle;
			float fMoveParticle;

			//�p�[�e�B�N���̈ړ��ɂ��Ă̎d�l
			switch (nType)
			{
			case PARTICLETYPE_STAY:	//�����Ȃ�
				fAngleParticle = 0.0f;		//�p�[�e�B�N�����ړ�����p�x��ݒ�
				fMoveParticle  = 0.0f;	//�p�[�e�B�N���̈ړ���
				break;

			case PARTICLETYPE_MOVE:	//����
				fAngleParticle = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;		//�p�[�e�B�N�����ړ�����p�x��ݒ�
				fMoveParticle = (float)(rand() % ANGLE) / FIX_FLOAT + MOVE_PARTICLE;	//�p�[�e�B�N���̈ړ���
				break;
			}

			 //�p�[�e�B�N�����̐ݒ�
			g_aParticle[nCntParticle].pos = pos;									//�ʒu
			g_aParticle[nCntParticle].move.x = sinf(fAngleParticle) * fMoveParticle;//�ړ���
			g_aParticle[nCntParticle].move.y = cosf(fAngleParticle) * fMoveParticle;//�ړ���
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;						//����
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;							//���a
			g_aParticle[nCntParticle].bUse = true;									//�g�p���Ă����Ԃɂ���

			//���_���̐ݒ�
			{
				//���_���W�̐ݒ�
				pVtx[0].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
				pVtx[0].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
				pVtx[1].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
				pVtx[1].pos.y = g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius;
				pVtx[2].pos.x = g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius;
				pVtx[2].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;
				pVtx[3].pos.x = g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius;
				pVtx[3].pos.y = g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius;

				//���_���W�̐ݒ�
				pVtx[0].col = col;
				pVtx[1].col = col;
				pVtx[2].col = col;
				pVtx[3].col = col;
			}
			break;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffParticle->Unlock();
}