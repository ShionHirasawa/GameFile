/*==========================================================================================================================

�`���[�g���A������[tutorial.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "tutorial.h"
#include "player.h"
#include "input.h"
#include "move.h"

//�}�N����`
#define NUM_TUTORIAL		(3)					//�`���[�g���A���̉摜��
#define FADE_TUTORIAL		(30)				//���u�����h�̕ω��l
#define UTEX_WIDTH			(0.1f)				//�e�N�X�`����U���W�̕�
#define APPEAR_LENGTH		(200.0f)			//�摜��\�����鋗��

//�W�����v�`���[�g���A���̃}�N��
#define TUTORIAL_WIDTH		(200.0f)							//�`���[�g���A���̕\����
#define TUTORIAL_HEIGHT		(200.0f)							//�`���[�g���A���̕\������
#define JUMP_POS			(D3DXVECTOR3(900.0f, 400.0f, 0.0f))	//�W�����v�`���[�g���A���̕\���ꏊ
#define JUMP_ANIM_CNT		(8)									//�W�����v�A�j���[�V�����摜�̕ω���

//�n�R����`���[�g���A���̃}�N��
#define SPAWN_POS			(D3DXVECTOR3(1700.0f, 200.0f, 0.0f))	//�Y�ݏo���`���[�g���A���̕\���ꏊ
#define HAKO_ANIM_CNT		(50)									//�W�����v�A�j���[�V�����摜�̕ω���

//��������`���[�g���A���̃}�N��
#define HOWTO_POS			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//��������̕\���ꏊ
#define HOWTO_WIDTH			(180.0f)							//��������̕\����
#define HOWTO_HEIGHT		(100.0f)							//��������̕\������

//�`���[�g���A���摜�̎�ނ̍\����
typedef enum
{
	TUTORIAL_JUMP = ZERO,	//�W�����v�̐���
	TUTORIAL_HAKO,			//�n�R�o���̐���
	TUTORIAL_HOWTO,			//�������(�s��)
	TUTORIAL_MAX
}TUTORIAL_TEX;

//�`���[�g���A���摜�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int aCol;				//�e�N�X�`���̃��l
	float TexU;				//�e�N�X�`����U���W
	float UWidth;			//�e�N�X�`���摜�̕�
	int nCounterAnim;		//�A�j���[�V�����J�E���^
	bool bUse;				//�g�p����Ă��邩�ǂ���
}Tutorial;

//�n�R����A�j���[�V�����̍\����
typedef enum
{
	ANIM_HAKO_THROW = ZERO,		//������
	ANIM_HAKO_SET,				//�u��
	ANIM_HAKO_DELETE,			//����
	ANIM_HAKO_MAX,
}ANIM_HAKO;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureTutorial[NUM_TUTORIAL] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTutorial = NULL;				//���_�o�b�t�@�ւ̃|�C���^
Tutorial					g_Tutorial[NUM_TUTORIAL];				//�`���[�g���A���̍\����
int							g_AnimHako;								//�n�R����A�j���[�V����

//�`���[�g���A���̏���������
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^ + �擾

	//�`���[�g���A���摜�̏ꏊ������				//�`���[�g���A���摜�̓����x������				//�A�j���[�V�����J�E���^������
	g_Tutorial[TUTORIAL_JUMP].pos = JUMP_POS;		g_Tutorial[TUTORIAL_JUMP].aCol = COLOR_MIN;		g_Tutorial[TUTORIAL_JUMP].nCounterAnim = ZERO;
	g_Tutorial[TUTORIAL_HAKO].pos = SPAWN_POS;		g_Tutorial[TUTORIAL_HAKO].aCol = COLOR_MIN;		g_Tutorial[TUTORIAL_HAKO].nCounterAnim = ZERO;
	g_Tutorial[TUTORIAL_HOWTO].pos = HOWTO_POS;		g_Tutorial[TUTORIAL_HOWTO].aCol = COLOR_MAX;	g_Tutorial[TUTORIAL_HOWTO].nCounterAnim = ZERO;

	//�e�N�X�`��U���W�̏�����						//�e�N�X�`���摜�̕�������						//�`���[�g���A���摜�̎g�p�̉�
	g_Tutorial[TUTORIAL_JUMP].TexU = F_ZERO;		g_Tutorial[TUTORIAL_JUMP].UWidth = UTEX_WIDTH;	 g_Tutorial[TUTORIAL_JUMP].bUse = false;
	g_Tutorial[TUTORIAL_HAKO].TexU = F_ZERO; 		g_Tutorial[TUTORIAL_HAKO].UWidth = UTEX_WIDTH;	 g_Tutorial[TUTORIAL_HAKO].bUse = false;
	g_Tutorial[TUTORIAL_HOWTO].TexU = F_ZERO; 		g_Tutorial[TUTORIAL_HOWTO].UWidth = F_TEN;		 g_Tutorial[TUTORIAL_HOWTO].bUse = true;

	//�n�R����A�j���[�V�����̏�����
	g_AnimHako = ANIM_HAKO_THROW;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/�W�����v�`���[�g���A��.png", &g_pTextureTutorial[TUTORIAL_JUMP]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/�n�R�`���[�g���A��.png", &g_pTextureTutorial[TUTORIAL_HAKO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/�������.png", &g_pTextureTutorial[TUTORIAL_HOWTO]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_TUTORIAL, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL);

	//���_���W�̐ݒ�
	Vertextutorial();
}

//�`���[�g���A���̏I������
void UninitTutorial(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTutorial = START_CNT; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		if (g_pTextureTutorial[nCntTutorial] != NULL)
		{
			g_pTextureTutorial[nCntTutorial]->Release();
			g_pTextureTutorial[nCntTutorial] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//�`���[�g���A���̍X�V����
void UpdateTutorial(void)
{
	//�`���[�g���A���摜�̃t�F�[�h����
	FadeTutorial();

	//�W�����v�`���[�g���A���̃A�j���[�V��������
	AnimJumpTutorial();

	//�n�R����̃`���[�g���A���̃A�j���[�V��������
	AnimHakoTutorial();

	//���_���W�̐ݒ�
	Vertextutorial();
}

//�`���[�g���A���̕`�揈��
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^ + �擾

											//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTutorial, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorial = START_CNT; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureTutorial[nCntTutorial]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorial * NUM_VERTEX, SEC);
	}
}

//�`���[�g���A���摜�̃t�F�[�h�ݒ�
void FadeTutorial(void)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��

	for (int nCntTutorial = START_CNT; nCntTutorial < TUTORIAL_HOWTO; nCntTutorial++)
	{
		//�v���C���[���`���[�g���A���摜�̕\�������ɂ���
		if (fabsf(pPlayer->pos.x - g_Tutorial[nCntTutorial].pos.x) <= 400.0f)
		{			
			g_Tutorial[nCntTutorial].aCol += FADE_TUTORIAL;		//�摜���o��������
			g_Tutorial[nCntTutorial].bUse = true;				//�g�p����Ă����Ԃɂ���

			if (g_Tutorial[nCntTutorial].aCol > COLOR_MAX)
			{
				g_Tutorial[nCntTutorial].aCol = COLOR_MAX;
			}
		}

		else
		{
			//�t�F�[�h�A�E�g�����Ă���
			g_Tutorial[nCntTutorial].aCol -= FADE_TUTORIAL;

			if (g_Tutorial[nCntTutorial].aCol < COLOR_MIN)
			{
				g_Tutorial[nCntTutorial].bUse = false;				//�g�p����Ă��Ȃ���Ԃɂ���
				g_Tutorial[nCntTutorial].aCol = COLOR_MIN;
			}
		}
	}
}

//--------------------------------------------------------------------
//				�`���[�g���A���摜�̃A�j���[�V��������
//--------------------------------------------------------------------
//�W�����v�`���[�g���A���̃A�j���[�V��������
void AnimJumpTutorial(void)
{
	//�摜���g���Ă���
	if (g_Tutorial[TUTORIAL_JUMP].bUse == true)
	{
		//�J�E���^�[���X�V
		g_Tutorial[TUTORIAL_JUMP].nCounterAnim++;

		if (g_Tutorial[TUTORIAL_JUMP].nCounterAnim == JUMP_ANIM_CNT)
		{
			//�J�E���^�[���[���ɂ���
			g_Tutorial[TUTORIAL_JUMP].nCounterAnim = ZERO;

			//�e�N�X�`��U���W�X�V
			g_Tutorial[TUTORIAL_JUMP].TexU += F_ONE;

			if (g_Tutorial[TUTORIAL_JUMP].TexU >= F_TEN)
			{
				//�e�N�X�`��U���W������
				g_Tutorial[TUTORIAL_JUMP].TexU = F_ZERO;
			}
		}
	}
}

//�n�R����̃`���[�g���A���̃A�j���[�V��������
void AnimHakoTutorial(void)
{
	//�摜���g���Ă���
	if (g_Tutorial[TUTORIAL_HAKO].bUse == true)
	{
		//�J�E���^�[���X�V
		g_Tutorial[TUTORIAL_HAKO].nCounterAnim++;

		if (g_Tutorial[TUTORIAL_HAKO].nCounterAnim == HAKO_ANIM_CNT)
		{
			//�J�E���^�[���[���ɂ���
			g_Tutorial[TUTORIAL_HAKO].nCounterAnim = ZERO;

			//�e�N�X�`��U���W�X�V
			g_Tutorial[TUTORIAL_HAKO].TexU += F_ONE;

			switch (g_AnimHako)
			{
			case ANIM_HAKO_THROW:
				if (g_Tutorial[TUTORIAL_HAKO].TexU > F_FOUR)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_ZERO;	//�ŏ��ɖ߂�

					g_AnimHako = ANIM_HAKO_SET;					//�n�R�u���A�j���[�V�����ɕύX
				}
				break;

			case ANIM_HAKO_SET:
				//�n�R�����A�j���[�V�������΂�
				if (g_Tutorial[TUTORIAL_HAKO].TexU == F_FOUR)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_FIVE;
				}

				if (g_Tutorial[TUTORIAL_HAKO].TexU > F_SIX)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_ZERO;	//�ŏ��ɖ߂�

					g_AnimHako = ANIM_HAKO_DELETE;				//�n�R�����A�j���[�V�����ɕύX
				}
				break;

			case ANIM_HAKO_DELETE:
				//�n�R�����E�n�R�u���A�j���[�V�������΂�
				if (g_Tutorial[TUTORIAL_HAKO].TexU == F_FOUR)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_SEV;

					//�ŏ��̃n�R�����A�j���[�V�����̕\�����Ԃ�L�΂�
					g_Tutorial[TUTORIAL_HAKO].nCounterAnim = -HAKO_ANIM_CNT;
				}

				if (g_Tutorial[TUTORIAL_HAKO].TexU > F_TEN)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_ZERO;	//�ŏ��ɖ߂�

					g_AnimHako = ANIM_HAKO_THROW;				//�n�R�����A�j���[�V�����ɕύX
				}

				break;
			}
		}
	}
}

//�`���[�g���A���̒��_�ݒ�
void Vertextutorial(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntTutorial = START_CNT; nCntTutorial < NUM_TUTORIAL; nCntTutorial++, pVtx += NUM_VERTEX)
	{
		float fWidth, fHeight;		//�e�N�X�`���摜�̕��ƍ���

		//���W���X�V
		if (nCntTutorial < TUTORIAL_HOWTO)
		{
			fWidth = TUTORIAL_WIDTH;
			fHeight = TUTORIAL_HEIGHT;
			moveObject(&g_Tutorial[nCntTutorial].pos);
		}
		else
		{
			fWidth = HOWTO_WIDTH;
			fHeight = HOWTO_HEIGHT;
		}

		//���_���W�̐ݒ�
		{
			//X���W�̐ݒ�
			pVtx[ZERO].pos.x = g_Tutorial[nCntTutorial].pos.x;
			pVtx[ONE].pos.x = g_Tutorial[nCntTutorial].pos.x + fWidth;
			pVtx[SEC].pos.x = g_Tutorial[nCntTutorial].pos.x;
			pVtx[SAN].pos.x = g_Tutorial[nCntTutorial].pos.x + fWidth;

			//Y���W�̐ݒ�
			pVtx[ZERO].pos.y = g_Tutorial[nCntTutorial].pos.y;
			pVtx[ONE].pos.y = g_Tutorial[nCntTutorial].pos.y;
			pVtx[SEC].pos.y = g_Tutorial[nCntTutorial].pos.y + fHeight;
			pVtx[SAN].pos.y = g_Tutorial[nCntTutorial].pos.y + fHeight;

			//Z���W�̐ݒ�(�ω��Ȃ�)
			pVtx[ZERO].pos.z = g_Tutorial[nCntTutorial].pos.z;
			pVtx[ONE].pos.z = g_Tutorial[nCntTutorial].pos.z;
			pVtx[SEC].pos.z = g_Tutorial[nCntTutorial].pos.z;
			pVtx[SAN].pos.z = g_Tutorial[nCntTutorial].pos.z;
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
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Tutorial[nCntTutorial].aCol);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Tutorial[nCntTutorial].aCol);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Tutorial[nCntTutorial].aCol);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Tutorial[nCntTutorial].aCol);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(g_Tutorial[nCntTutorial].TexU,										ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(g_Tutorial[nCntTutorial].TexU + g_Tutorial[nCntTutorial].UWidth,	ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(g_Tutorial[nCntTutorial].TexU,										F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(g_Tutorial[nCntTutorial].TexU + g_Tutorial[nCntTutorial].UWidth,	F_TEN);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTutorial->Unlock();
}