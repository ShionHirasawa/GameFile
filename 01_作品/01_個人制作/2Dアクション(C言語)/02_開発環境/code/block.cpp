/*==========================================================================================================================

�u���b�N�̏���[block.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "block.h"
#include "player.h"
#include "input.h"
#include "move.h"
#include <string.h>
#include "hako.h"

//�}�N����`
#define NUM_BLOCK				(3)		//�w�i�̐�
#define MAX_BLOCK				(22)	//�u���b�N�̍ő吔
#define MOJI_MAX				(300)	//�ǂݍ��ލۂ̍ő啶����
#define LIGHTEN_MAGNIFICATION	(0.02f)	//�ړ��ʂ�����������Ƃ��̔{��
#define REVERSAL_MAGNIFICATION	(0.2f)	//�ړ��ʂ��t�]�����������̔{��

#define BLOCK_FILE	"data/stage000.csv"	//�u���b�N�̔z�u���Ȃǂ������ꂽ�t�@�C��

//�v���C���[�̈ʒu��T��
void SearchPlayer(Hako pHako);

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureBlock[NUM_BLOCK] = { NULL, NULL ,NULL };	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBlock = NULL;								//���_�o�b�t�@�ւ̃|�C���^		
Block g_Block[MAX_BLOCK];

#define SIZEOF_BLOCK			(sizeof g_Block / sizeof(Block))	//�u���b�N�f�[�^�̃T�C�Y

//�u���b�N�̏���������
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	//�u���b�N�̐ݒ菈���Ăяo��
	SetBlock();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Wall.png", &g_pTextureBlock[ZERO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Spike.png", &g_pTextureBlock[ONE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.jpg", &g_pTextureBlock[SEC]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BLOCK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBlock, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBlock = START_CNT; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = g_Block[nCntBlock].pos;
			pVtx[ONE].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x,  g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, ORIGIN_POS);
		}

		//rhw�̐ݒ�
		{
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(g_Block[nCntBlock].Tex.x, 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , g_Block[nCntBlock].Tex.y);
			pVtx[SAN].tex = D3DXVECTOR2(g_Block[nCntBlock].Tex.x, g_Block[nCntBlock].Tex.y);
		}

		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//�u���b�N�̏I������
void UninitBlock(void)
{
	//�e�N�X�`��(�R����)�̔j��
	for (int nCntBlock = START_CNT; nCntBlock < NUM_BLOCK; nCntBlock++)
	{
		if (g_pTextureBlock[nCntBlock] != NULL)
		{
			g_pTextureBlock[nCntBlock]->Release();
			g_pTextureBlock[nCntBlock] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//�u���b�N�̍X�V����
void UpdateBlock(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	Player *pPlayer = GetPlayer();

	for (int nCntBlock = START_CNT; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += NUM_VERTEX)
	{
		g_Block[nCntBlock].posOld = g_Block[nCntBlock].pos;		//�O��̈ʒu���L��
		float ReturnMagnification = F_ZERO;						//�܂�Ԃ������̔{��

		//���W���X�V
		moveObject(&g_Block[nCntBlock].pos);

		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = g_Block[nCntBlock].pos;
			pVtx[ONE].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, ORIGIN_POS);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//�u���b�N�̕`�揈��
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^
	int nCntBlock;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBlock, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = START_CNT; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_Block[nCntBlock].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureBlock[g_Block[nCntBlock].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * NUM_VERTEX, SEC);
		}
	}
}

//�u���b�N�̐ݒ菈��
void SetBlock(void)
{
	FILE *pFile;
	char aData[MOJI_MAX];

	pFile = fopen(BLOCK_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntBlock = START_CNT; nCntBlock < MAX_BLOCK; nCntBlock++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���
			
			//�z�u�ꏊ�̏�����
			g_Block[nCntBlock].pos.x = (float)atof(strtok(&aData[ZERO], ","));	//�ʒu������
			g_Block[nCntBlock].pos.y = (float)atof(strtok(NULL, ","));			//�ʒu������

			g_Block[nCntBlock].nType = atoi(strtok(NULL, ","));			//��ނ̏�����
			g_Block[nCntBlock].fWidth = (float)atof(strtok(NULL, ","));		//���̏�����
			g_Block[nCntBlock].fHeight = (float)atof(strtok(NULL, ","));		//�����̏�����

			//�e�N�X�`���̍��W
			g_Block[nCntBlock].Tex.x = (float)atof(strtok(NULL, ","));			//��̃u���b�N�ɑ΂���e�N�X�`���̕`�搔��ǂݍ���
			g_Block[nCntBlock].Tex.y = (float)atof(strtok(NULL, ","));			//��̃u���b�N�ɑ΂���e�N�X�`���̕`�搔��ǂݍ���

			g_Block[nCntBlock].posOld = g_Block[nCntBlock].pos;			//�O��̈ʒu�����݂̈ʒu�ɏ�����
			g_Block[nCntBlock].bUse = true;								//�g�p���Ă����Ԃ�
		}
	}
}

/*�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P
			�u���b�N�̓����蔻��
______________________________________________*/

//��
bool X_CollisionBlockToPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight, Block **pBlock)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��
	bool bSide = false;				//���n���Ă��Ȃ���Ԃ�

	for (int nCntBlock = START_CNT; nCntBlock < SIZEOF_BLOCK; nCntBlock++)
	{
		float HeadPos     = pPos->y - fHeight,		//���݂̓��̈ʒu
			  fWidth_R    = pPos->x + fWidth,		//���݂̑�(�E��)�̉���
			  fWidth_L    = pPos->x - fWidth,		//���݂̑�(����)�̉���
			  HeadPosOld  = pPosold->y - fHeight,	//�O��̓��̈ʒu
			  fOldWidth_R = pPosold->x + fWidth,	//�O��̑�(�E��)�̉���
			  fOldWidth_L = pPosold->x - fWidth;	//�O��̑�(����)�̉���

		if (g_Block[nCntBlock].bUse == true)
		{
			//���݂̈ʒu���u���b�N�̓����蔻��͈͓̔�
			if (g_Block[nCntBlock].pos.x < fWidth_R && fWidth_L < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
			{//���݂̃u���b�N�̈ʒu(��Ɖ�)
				//���݂̃u���b�N�̈ʒu(���ƉE)
				if (HeadPos < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight && pPos->y > g_Block[nCntBlock].pos.y)
				{//�u���b�N�̒��蓪�������A�u���b�N�̓V���葫���Ⴂ
				 //�u���b�N�ɍ����瓖����
					if (g_Block[nCntBlock].posOld.x >= fOldWidth_R &&	//�O��̈ʒu���u���b�N��������
						g_Block[nCntBlock].pos.x <= fWidth_R)		//���݂̈ʒu���u���b�N�ɂ߂荞��ł���
					{
						pPos->x = g_Block[nCntBlock].pos.x - fWidth;//�u���b�N�̈ʒu+�������ɖ߂�
						bSide = true;					//�u���b�N�ɂԂ�����
						pMove->x = 0.0f;				//�ړ���X���O�ɂ���
					}

					//�u���b�N�ɉE���瓖����
					if (g_Block[nCntBlock].posOld.x + g_Block[nCntBlock].fWidth <= fOldWidth_L	//�O��̑�(�E��)�̈ʒu���u���b�N�����E��
					 && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth >= fWidth_L)	//���݂̑�(�E��)�̈ʒu���u���b�N�ɂ߂荞��ł���
					{
						pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + fWidth;	//�u���b�N�̉����̈ʒu+�������߂�
						bSide = true;					//�u���b�N�ɂԂ�����
						pMove->x = 0.0f;				//�ړ���X���O�ɂ���
					}
				}
			}
		}
	}

	return bSide;	//���n�������ǂ�����Ԃ�
}

//�c
bool Y_CollisionBlockToPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight, Block **pBlock)
{
	bool bLand = false;		//���n���Ă��Ȃ���Ԃ�

	for (int nCntBlock = START_CNT; nCntBlock < SIZEOF_BLOCK; nCntBlock++)
	{
		float HeadPos = pPos->y - fHeight,		//���݂̓��̈ʒu
			fWidth_R = pPos->x + fWidth,		//���݂̑�(�E��)�̉���
			fWidth_L = pPos->x - fWidth,		//���݂̑�(����)�̉���
			HeadPosOld = pPosold->y - fHeight,	//�O��̓��̈ʒu
			fOldWidth_R = pPosold->x + fWidth,	//�O��̑�(�E��)�̉���
			fOldWidth_L = pPosold->x - fWidth;	//�O��̑�(����)�̉���

		if (g_Block[nCntBlock].bUse == true)
		{
			//���݂̈ʒu���u���b�N�̓����蔻��͈͓̔�
			if (g_Block[nCntBlock].pos.x < fWidth_R && fWidth_L < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
			{//���݂̃u���b�N�̈ʒu(��Ɖ�)
			 //�u���b�N�ɑ����߂荞��(�������)
				if (g_Block[nCntBlock].posOld.y >= pPosold->y &&	//�O��̈ʒu���u���b�N�������
					g_Block[nCntBlock].pos.y <= pPos->y)		//���݂̈ʒu���u���b�N�ɂ߂荞��ł���
				{

					if (g_Block[nCntBlock].nType == 1)
					{
						HitPlayer(PLAYERSTATE_DAMAGE);
					}
					else
					{
						bLand = true;							//���n����
						pPos->y = g_Block[nCntBlock].pos.y;		//�u���b�N�̈ʒu�ɖ߂�
						pMove->y = 0.0f;						//�ړ���Y���O�ɂ���

						if (pBlock != NULL)
						{//�u���b�N�̃A�h���X������ꏊ������
							*pBlock = &g_Block[nCntBlock];
						}
					}
				}

				//�u���b�N�ɓ����߂荞��
				else if (g_Block[nCntBlock].posOld.y + g_Block[nCntBlock].fHeight <= HeadPosOld	//�O��̓��̈ʒu���u���b�N��������
					&& g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight >= HeadPos)		//���݂̓��̈ʒu���u���b�N�ɓ˂��h�����Ă���
				{
					pPos->y += g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight - HeadPos;	//�u���b�N�̒ꂩ�瓪�̈ʒu���������Ƃ���ɖ߂�
					pMove->y = 0.0f;						//�ړ���Y���O�ɂ���
				}
			}
		}
	}

	return bLand;	//���n�������ǂ�����Ԃ�
}

//�u���b�N�ƃn�R�Ƃ̓����蔻��
bool CollisionBlockToHako(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight, Block **pBlock)
{
	D3DXVECTOR3 FixMove = ZERO_SET;	//�n�R�̈ʒu�C���ϐ�
	bool bLand = false;		//���n���Ă��Ȃ���Ԃ�

	for (int nCntBlock = START_CNT; nCntBlock < SIZEOF_BLOCK; nCntBlock++)
	{
		float BottomPos    = pPos->y + fHeight,		//���݂̒�̈ʒu
			  fWidth_R     = pPos->x + fWidth,		//���݂̑�(�E��)�̉���
			  BottomPosOld = pPosold->y + fHeight,	//�O��̒�̈ʒu
			  fOldWidth_R  = pPosold->x + fWidth;	//�O��̑�(�E��)�̉���

		if (g_Block[nCntBlock].bUse == true)
		{
			//���݂̈ʒu���u���b�N�̓����蔻��͈͓̔�
			if (g_Block[nCntBlock].pos.x < fWidth_R && pPos->x < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
			{//���݂̃u���b�N�̈ʒu(��Ɖ�)
			 //�u���b�N�ɒꂪ�߂荞��(�������)
				if (g_Block[nCntBlock].posOld.y >= BottomPosOld &&		//�O��̒�̈ʒu���u���b�N�������
					g_Block[nCntBlock].pos.y <= BottomPos)				//���݂̒�̈ʒu���u���b�N�ɂ߂荞��ł���
				{
					bLand = true;										//���n����
					pMove->y = 0.0f;									//�ړ���Y���O�ɂ���

					FixMove.y = g_Block[nCntBlock].pos.y - BottomPos;	//�C���ʌv�Z

					FixHakoPos(FixMove);		//�n�R�S���C��

					if (pBlock != NULL)
					{//�u���b�N�̃A�h���X������ꏊ������
						*pBlock = &g_Block[nCntBlock];
					}
				}

				//�u���b�N�Ƀn�R�̓V�䂪�߂荞��
				else if (g_Block[nCntBlock].posOld.y + g_Block[nCntBlock].fHeight <= pPosold->y	//�O��̓V��̈ʒu���u���b�N��������
					  && g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight >= pPos->y)		//���݂̓V��̈ʒu���u���b�N�ɂ߂荞��ł���
				{
					FixMove.y = (g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight) - pPos->y;	//�C���ʌv�Z

					FixHakoPos(FixMove);		//�n�R�S���C��
				}

				//���݂̃u���b�N�̈ʒu(���ƉE)
				if (pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight && BottomPos > g_Block[nCntBlock].pos.y)
				{//�u���b�N�̒���n�R�̓V�䂪�����A�u���b�N�̓V����n�R�̒ꂪ�Ⴂ		/* Visual Studio�̃}�[�N�݂����Ȋ��� */

					 //�u���b�N�ɍ����瓖����
					if (g_Block[nCntBlock].posOld.x >= fOldWidth_R &&	//�O��̈ʒu���u���b�N��������
						g_Block[nCntBlock].pos.x <= fWidth_R)		//���݂̈ʒu���u���b�N�ɂ߂荞��ł���
					{
						FixMove.x = g_Block[nCntBlock].pos.x - fWidth_R;	//�C���ʌv�Z

						FixHakoPos(FixMove);		//�n�R�S���C��
					}

					//�u���b�N�ɉE���瓖����
					if (g_Block[nCntBlock].posOld.x + g_Block[nCntBlock].fWidth <= pPosold->x	//�O��̈ʒu���u���b�N�����E��
					 && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth >= pPos->x)	//���݂̈ʒu���u���b�N�ɂ߂荞��ł���
					{
						FixMove.x = (g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth) - pPos->x;	//�C���ʌv�Z

						FixHakoPos(FixMove);		//�n�R�S���C��
					}
				}
			}
		}
	}

	return bLand;	//���n�������ǂ�����Ԃ�
}

//�n�R�̈ʒu�C��
void FixHakoPos(D3DXVECTOR3 FixMove)
{
	Player *pPlayer = GetPlayer();
	Hako *pHako = GetHako();

	for (int nCntHako = START_CNT; nCntHako < FIVE; nCntHako++, pHako++)
	{
		if (pHako->bUse == true)
		{//�n�R���g���Ă���
			 //�n�R�̈ʒu�C��
			pHako->pos += FixMove;
			pHako->move = ZERO_SET;

			//�v���C���[���n�R�ɖ��܂��Ă��邩���ׂ�
			SearchPlayer(*pHako);
		}
		else
		{//�g���Ȃ���ΏC���I��
			break;
		}
	}

}

//�v���C���[�̈ʒu��T��
void SearchPlayer(Hako pHako)
{
	Player *pPlayer = GetPlayer();

	//�v���C���[�̓����蔻��
	float HeadPos	  = pPlayer->pos.y	  - pPlayer->fHeight,	//���݂̓��̈ʒu
		  Width_R	  = pPlayer->pos.x	  + pPlayer->fWidth,	//���݂̑�(�E��)�̉���
		  Width_L	  = pPlayer->pos.x	  - pPlayer->fWidth,	//���݂̑�(����)�̉���
		  OldWidth_R  = pPlayer->posOld.x + pPlayer->fWidth,	//�O��̑�(�E��)�̉���
		  OldWidth_L  = pPlayer->posOld.x - pPlayer->fWidth;	//�O��̑�(����)�̉���

	//�n�R�̓����蔻��
	float BottomPos	   = pHako.pos.y	+ pHako.fHeight,	//���݂̒�̈ʒu
		  Hako_R	   = pHako.pos.x	+ pHako.fWidth,		//���݂̑�(�E��)�̉���
		  BottomPosOld = pHako.posOld.y	+ pHako.fHeight,	//�O��̒�̈ʒu
		  HakoOld_R   = pHako.posOld.x	+ pHako.fWidth;		//�O��̑�(�E��)�̉���

	//�v���C���[�̈ʒu�C��
	if (HeadPos < BottomPos && pPlayer->pos.y > pHako.pos.y)
	{
		//�n�R�ɍ����瓖����
		if (pHako.posOld.x >= OldWidth_R &&	//�O��̈ʒu���n�R��������
			pHako.pos.x < Width_R)				//���݂̈ʒu���n�R�ɂ߂荞��ł���
		{
			//�n�R�̍��̈ʒu�ɍ��킹��
			pPlayer->pos.x = pHako.pos.x - pPlayer->fWidth;
		}

		//�n�R�ɉE���瓖����
		if (HakoOld_R <= OldWidth_L	//�O��̑�(�E��)�̈ʒu���n�R�����E��
			&& Hako_R >  Width_L)	//���݂̑�(�E��)�̈ʒu���n�R�ɂ߂荞��ł���
		{
			//�n�R�̉E�̈ʒu�ɍ��킹��
			pPlayer->pos.x = Hako_R + pPlayer->fWidth;
		}
	}
}