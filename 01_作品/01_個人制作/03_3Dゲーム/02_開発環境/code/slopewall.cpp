/*==========================================================================================================================================================

															�⓹�̕ǂ̏���[slopewall.cpp]
																Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"
#include "slopewall.h"

//�}�N����`
#define NUM_SLOPEWALL		(2)									// �⓹�̕ǂ̐�
#define SLOPEWALL_NOR		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// �⓹�̕ǂ�nor

#define SLOPEWALL_TEX_NAME	"data/TEXTURE/fault000.png"	// �f�ʂ̃t�@�C����
#define SLOPEWALL_ORI_POS	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// �f�ʂ̌��_���W
#define SLOPEWALL_FILE		"data/CSV/slopewall.csv"			// �f�ʂ̏�񂪓������t�@�C����

//�⓹�̕�(�f��)�̒��_�񋓌^
typedef enum
{
	SLOPEWALL_VTX_START = ZERO,		// �Ζʂ̂ӂ��Ƃ̃X�^�[�g���W
	SLOPEWALL_VTX_START_TOP,		// �Ζʂ̒���ŁA�X�^�[�g���W����݂����̒��_
	SLOPEWALL_VTX_START_LOW,		// �ЂƂO�̒��_�̐^���̒��_
	SLOPEWALL_VTX_START_DIAGONAL,	// �X�^�[�g���W�̑Ίp���̈ʒu�ɂ��钸�_
	SLOPEWALL_VTX_LOW_DIAGONAL,		// �X�^�[�g���W�̒�ӂ̑Ίp��
	SLOPEWALL_VTX_SAME_DIAGONAL,	// �k�ރ|���S���ɂ��邽�� 4�Ԗ�( ���_�ԍ���3��)�Ɠ���
	SLOPEWALL_VTX_LAST,				// �Ζʂ̂ӂ��Ƃ̃��X�g���W
	SLOPEWALL_VTX_MAX
}SLOPEWALL_VTX;

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSlopeWall = NULL;					//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureSlopeWall = NULL;	//�e�N�X�`��
SlopeWall				g_SlopeWall[NUM_SLOPEWALL];					//�⓹�̕ǂ̏��

//------------------------------------------------
//				�⓹�̒f�ʂ̏���������
//------------------------------------------------
void InitSlopeWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^	

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SLOPEWALL_VTX_MAX * NUM_SLOPEWALL, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffSlopeWall, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSlopeWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	/*------------------------------------
			�⓹�̕ǂ̏��ǂݍ���
	------------------------------------*/
	LoadSlopeWall();

	D3DXCreateTextureFromFile(pDevice, SLOPEWALL_TEX_NAME, &g_pTextureSlopeWall);	//�e�N�X�`���̓ǂݍ���

	//�e����E���_���W�̏�����
	for (int nCntSlopeWall = START_CNT; nCntSlopeWall < NUM_SLOPEWALL; nCntSlopeWall++, pVtx += SLOPEWALL_VTX_MAX)
	{
		float TexWidth = F_ZERO;	//�e�N�X�`���̕�

		//���_���W�̐ݒ�
		for (int nCntVtx = START_CNT; nCntVtx < SLOPEWALL_VTX_MAX; nCntVtx++)
		{
			//-------------------------
			//		X���W�̐ݒ�
			//-------------------------
			//�⓹�̕�������O�����̏ꍇ
			if (g_SlopeWall[nCntSlopeWall].sloperot == SLOPEROT_FRONT_BACK)
			{
				//�ŏ��̎O�p�`���`�����钸�_�Ƃ��̑��̒��_�͌��_����̕����t
				if (nCntVtx <= SLOPEWALL_VTX_START_LOW)
				{
					pVtx[nCntVtx].pos.x = +g_SlopeWall[nCntSlopeWall].fWidth;
				}
				else
				{
					pVtx[nCntVtx].pos.x = -g_SlopeWall[nCntSlopeWall].fWidth;
				}
			}

			//�⓹�̕��������E�����̏ꍇ
			else
			{
				//�ŏ��ƍŌ�̒��_�������_���猩�ĉ��s��������
				if (nCntVtx % SLOPEWALL_VTX_LAST == ZERO)
				{
					pVtx[nCntVtx].pos.x = g_SlopeWall[nCntSlopeWall].fWidth;
				}
				else
				{
					pVtx[nCntVtx].pos.x = NIL_F;
				}
			}

			//-------------------------
			//		Y���W�̐ݒ�
			//-------------------------
			//���_�ԍ�����̂��̂�������������
			if (nCntVtx % EVENPARITY == ODDPARITY)
			{
				pVtx[nCntVtx].pos.y = g_SlopeWall[nCntSlopeWall].fHeight;
			}
			else
			{
				pVtx[nCntVtx].pos.y = NIL_F;
			}

			//-------------------------
			//		Z���W�̐ݒ�
			//-------------------------
			//�⓹�̕�������O�����̏ꍇ
			if (g_SlopeWall[nCntSlopeWall].sloperot == SLOPEROT_FRONT_BACK)
			{
				//�ŏ��ƍŌ�̒��_�������_���猩�ĉ��s��������
				if (nCntVtx % SLOPEWALL_VTX_LAST == ZERO)
				{
					pVtx[nCntVtx].pos.z = g_SlopeWall[nCntSlopeWall].fDepth;
				}
				else
				{
					pVtx[nCntVtx].pos.z = NIL_F;
				}
			}

			//�⓹�̕��������E�����̏ꍇ
			else
			{
				//�ŏ��̎O�p�`���`�����钸�_�Ƃ��̑��̒��_�͌��_����̕����t
				if (nCntVtx <= SLOPEWALL_VTX_START_LOW)
				{
					pVtx[nCntVtx].pos.z = -g_SlopeWall[nCntSlopeWall].fDepth;
				}
				else
				{
					pVtx[nCntVtx].pos.z = +g_SlopeWall[nCntSlopeWall].fDepth;
				}
			}

			//-------------------------
			//		nor�̐ݒ�
			//-------------------------
			pVtx[nCntVtx].nor = SLOPEWALL_NOR;

			//-------------------------
			//	  ���_�J���[�̐ݒ�
			//-------------------------
			pVtx[nCntVtx].col = RGBA_WHITE;

			//-------------------------
			//	�e�N�X�`�����W�̐ݒ�
			//-------------------------
			pVtx[nCntVtx].tex.x = TexWidth;
			pVtx[nCntVtx].tex.y = nCntVtx / EVENPARITY;

			//���_�ԍ�����̎��ɕ��𑫂�
			if (nCntVtx % EVENPARITY == NOPARITY)
			{
				TexWidth++;
			}
		}

		pVtx[4].tex = D3DXVECTOR2(2.0f, 1.0f);
		pVtx[5].tex = D3DXVECTOR2(2.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(2.0f, 0.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffSlopeWall->Unlock();
}

//------------------------------------------------
//				�⓹�̒f�ʂ̏I������
//------------------------------------------------
void UninitSlopeWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureSlopeWall != NULL)
	{
		g_pTextureSlopeWall->Release();
		g_pTextureSlopeWall = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSlopeWall != NULL)
	{
		g_pVtxBuffSlopeWall->Release();
		g_pVtxBuffSlopeWall = NULL;
	}
}

//------------------------------------------------
//				�⓹�̒f�ʂ̍X�V����
//------------------------------------------------
void UpdateSlopeWall(void)
{

}

//------------------------------------------------
//				�⓹�̒f�ʂ̕`�揈��
//------------------------------------------------
void DrawSlopeWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	for (int nCntSlopeWall = START_CNT; nCntSlopeWall < NUM_SLOPEWALL; nCntSlopeWall++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_SlopeWall[nCntSlopeWall].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_SlopeWall[nCntSlopeWall].rot.y, g_SlopeWall[nCntSlopeWall].rot.x, g_SlopeWall[nCntSlopeWall].rot.z);

		D3DXMatrixMultiply(&g_SlopeWall[nCntSlopeWall].mtxWorld, &g_SlopeWall[nCntSlopeWall].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_SlopeWall[nCntSlopeWall].pos.x, g_SlopeWall[nCntSlopeWall].pos.y, g_SlopeWall[nCntSlopeWall].pos.z);

		D3DXMatrixMultiply(&g_SlopeWall[nCntSlopeWall].mtxWorld, &g_SlopeWall[nCntSlopeWall].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_SlopeWall[nCntSlopeWall].mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(ZERO, g_pVtxBuffSlopeWall, ZERO, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureSlopeWall);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSlopeWall * SLOPEWALL_VTX_MAX, 5);
	}
}

//�⓹�̕ǂ̓����蔻�菈��
bool CollisionSlopeWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	bool bCollision = false;	//�����������ǂ����̔���
	D3DXVECTOR3 PeakPos;		//����̈ʒu
	D3DXVECTOR3 BasePos;		//�ӂ��Ƃ̈ʒu�i���_�̈ʒu�Ƃ͑Ίp����̈ʒu

	//�S�⓹�̕ǂ̓����蔻��
	for (int nCntSlopeWall = START_CNT; nCntSlopeWall < NUM_SLOPEWALL; nCntSlopeWall++)
	{
		//�⓹�̕ǂ̌����ʏ�������
		switch (g_SlopeWall[nCntSlopeWall].sloperot)
		{//���������ʁE������
		case SLOPEROT_FRONT_BACK:
		{
			//--------------------------------------------------									------------------------------------------------
			//					����̈ʒu�ݒ�																		�ӂ��Ƃ̈ʒu�ݒ�
			//--------------------------------------------------									------------------------------------------------
			PeakPos.x = g_SlopeWall[nCntSlopeWall].pos.x - g_SlopeWall[nCntSlopeWall].fWidth;		BasePos.x = g_SlopeWall[nCntSlopeWall].pos.x + g_SlopeWall[nCntSlopeWall].fWidth;
			PeakPos.y = g_SlopeWall[nCntSlopeWall].pos.y + g_SlopeWall[nCntSlopeWall].fHeight;		BasePos.y = g_SlopeWall[nCntSlopeWall].pos.y;
			PeakPos.z = g_SlopeWall[nCntSlopeWall].pos.z;											BasePos.z = g_SlopeWall[nCntSlopeWall].pos.z - g_SlopeWall[nCntSlopeWall].fDepth;

			//�����X���W�Ƃӂ��Ƃ�X���W�̊ԂɃv���C���[������@���@�����Z���W����O�ɂ���
			if (PeakPos.x <= pPos->x && pPos->x <= BasePos.x && BasePos.z <= pPos->z && pPos->z <= PeakPos.z)
			{
				//�����X���W�̔���
				if (pPosOld->x <= PeakPos.x)
				{
					pPos->x = PeakPos.x;		//���_��X���W�ɖ߂�
				}

				//�ӂ��Ƃ�X���W�̔���
				if (BasePos.x <= pPosOld->x)
				{
					pPos->x = BasePos.x;		//�ӂ��Ƃ�X���W�ɖ߂�
				}

				//�⓹�̂S�p�`�̕ǂ̔���i�T�C�h�͂R�p�`�j
				if (PeakPos.z <= pPosOld->z)
				{
					pPos->z = PeakPos.z;		//���_��Z���W�ɖ߂�
				}
			}
		}
			break;

		case SLOPEROT_RIGHT_LEFT:	//���������E�������Ă���
		{
			//--------------------------------------------------									------------------------------------------------
			//					����̈ʒu�ݒ�																		�ӂ��Ƃ̈ʒu�ݒ�
			//--------------------------------------------------									------------------------------------------------
			PeakPos.x = g_SlopeWall[nCntSlopeWall].pos.x;											BasePos.x = g_SlopeWall[nCntSlopeWall].pos.x + g_SlopeWall[nCntSlopeWall].fWidth;
			PeakPos.y = g_SlopeWall[nCntSlopeWall].pos.y + g_SlopeWall[nCntSlopeWall].fHeight;		BasePos.y = g_SlopeWall[nCntSlopeWall].pos.y;
			PeakPos.z = g_SlopeWall[nCntSlopeWall].pos.z + g_SlopeWall[nCntSlopeWall].fDepth;		BasePos.z = g_SlopeWall[nCntSlopeWall].pos.z - g_SlopeWall[nCntSlopeWall].fDepth;

			//�����Z���W�Ƃӂ��Ƃ�Z���W�̊ԂɃv���C���[������@���@
			if (PeakPos.z <= pPos->z && pPos->z <= BasePos.z && BasePos.x <= pPos->x && pPos->x <= PeakPos.x)
			{
				//�����X���W�̔���
				if (pPosOld->x <= PeakPos.x)
				{
					pPos->x = PeakPos.x;		//���_��X���W�ɖ߂�
				}

				//�ӂ��Ƃ�X���W�̔���
				if (BasePos.x <= pPosOld->x)
				{
					pPos->x = BasePos.x;		//�ӂ��Ƃ�X���W�ɖ߂�
				}

				//�⓹�̂S�p�`�̕ǂ̔���i�T�C�h�͂R�p�`�j
				if (PeakPos.x <= pPosOld->z)
				{
					pPos->z = PeakPos.z;		//���_��Z���W�ɖ߂�
				}
			}
		}
			break;
		}
	}

	//�����Ԃ�
	return bCollision;
}

//�⓹�̕ǂ̏��
SlopeWall *GetSlopeWall(void)
{
	//�⓹�̏��̐擪�A�h���X��Ԃ�
	return &g_SlopeWall[0];
}

//�⓹�̕ǂ̏��ǂݍ��ݏ���
void LoadSlopeWall(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(SLOPEWALL_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntSlopeWall = START_CNT; nCntSlopeWall < NUM_SLOPEWALL; nCntSlopeWall++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			//�z�u�ꏊ�̏�����
			g_SlopeWall[nCntSlopeWall].pos.x = atof(strtok(&aData[ZERO], ","));			//�ʒu������
			g_SlopeWall[nCntSlopeWall].pos.y = atof(strtok(NULL, ","));					//�ʒu������
			g_SlopeWall[nCntSlopeWall].pos.z = atof(strtok(NULL, ","));					//�ʒu������

			//�z�u�����̏�����
			g_SlopeWall[nCntSlopeWall].rot.x = atof(strtok(NULL, ",")) / FIX_FLOAT;		//����������
			g_SlopeWall[nCntSlopeWall].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;		//����������
			g_SlopeWall[nCntSlopeWall].rot.z = atof(strtok(NULL, ",")) / FIX_FLOAT;		//����������

			g_SlopeWall[nCntSlopeWall].fWidth = atof(strtok(NULL, ","));				//���̏�����
			g_SlopeWall[nCntSlopeWall].fHeight = atof(strtok(NULL, ","));				//�����̏�����
			g_SlopeWall[nCntSlopeWall].fDepth = atof(strtok(NULL, ","));				//���s���̏�����

			g_SlopeWall[nCntSlopeWall].sloperot = (SLOPEROT)(atoi(strtok(NULL, ",")));	//�⓹�̕���������
		}

		//�t�@�C�������
		fclose(pFile);
	}
}