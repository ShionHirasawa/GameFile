/*==========================================================================================================================

													�����L���O�\������[ranking.cpp]
														Author:���V����

==========================================================================================================================*/
#include "score.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"

//�}�N����`
#define GRAY			(0.7f)						//�D�F
#define BLUCK			(0.4f)						//���F
#define RANK_HEIGHT		(50.0f)						//���ʂ̐����̍���
#define RANK_WIDTH		(40.0f)						//���ʂ̐����̕�
#define HEIGHT_DIS		(80.0f)						//���l�Ɛ��l�̋���
#define WIDTH__DIS		(80.0f)						//���l�Ɛ��l�̋���
#define RANK_SIZE		(5.0f)						//���ʂ��Ƃ̃T�C�Y(���ʂ������邲�Ƃɏk������Ă���)
#define DIS_SIZE		(7.9f)						//�e���ʂ̉��Ƃ̕�(���ʂ������邲�Ƃɋ��܂�)
#define RANK_DEJIT		(8)							//�����L���O�X�R�A�̌���
#define RANK_NUM		(5)							//���ʂ̐�
#define ALL_NUM			(RANK_DEJIT * RANK_NUM)		//�����L���O�Ɏg�p����S����
#define RANK_DATA		"data/RANKING.txt"			//�����L���O�t�@�C��
#define RANK_TIME_LIMIT	(6000)						//�����L���O�\������(10�b)	
#define RANK_BG_NUM		(0)							//�w�i�摜�̔ԍ�
#define RANK_NUMBER		(1)							//�����L���O�Ɏg�p����摜�̔ԍ�
#define RANK_TEXTURE	(2)							//�����L���O�Ɏg�p����摜�̐�
#define TEX_MAX			(41)						//�`�悷��|���S���̐�

//���ʂ��Ƃ̍��W�}�N��
//	X���W
#define NO_1POSX	(350.0f)	//No.1�̃|�W�V���� x
#define NO_2POSX	(380.0f)	//No.2�̃|�W�V���� x
#define NO_3POSX	(410.0f)	//No.3�̃|�W�V���� x
#define NO_4POSX	(430.0f)	//No.4�̃|�W�V���� x
#define NO_5POSX	(460.0f)	//No.5�̃|�W�V���� x

//	Y���W
#define NO_1POSY	(150.0f)	//No.1�̃|�W�V���� y
#define NO_2POSY	(200.0f)	//No.2�̃|�W�V���� y
#define NO_3POSY	(240.0f)	//No.3�̃|�W�V���� y
#define NO_4POSY	(270.0f)	//No.4�̃|�W�V���� y
#define NO_5POSY	(300.0f)	//No.5�̃|�W�V���� y

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureRanking[RANK_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posRank[RANK_NUM];								//�e���ʂ��Ƃ̍��W
int g_nRanking[RANK_NUM];										//�����N�̒l
DWORD g_Rank_Time;												//�����L���O�\������

//�����L���O�̏���������
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int aTexU[RANK_DEJIT];	//�����L���O�\���Ɏg�p

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/RankingBg.jpg", &g_pTextureRanking[RANK_BG_NUM]);	//�w�i�摜
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureRanking[RANK_NUMBER]);	//�����摜

	//�X�R�A�̏��̏���������у����L���O�t�@�C���̓ǂݍ���
	ScanRanking();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TEX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRanking, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�w�i�摜�̐ݒ�
	{
		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, F_ZERO);
			pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, F_ZERO);
			pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, F_ZERO);
			pVtx[SAN].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, F_ZERO);
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
			pVtx[ZERO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[ONE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[SEC].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[SAN].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[SAN].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�

	//�e���ʂ̏����l
	g_posRank[ZERO] = D3DXVECTOR3(NO_1POSX, NO_1POSY, POS_Z);
	g_posRank[ONE] = D3DXVECTOR3(NO_2POSX, NO_2POSY, POS_Z);
	g_posRank[SEC] = D3DXVECTOR3(NO_3POSX, NO_3POSY, POS_Z);
	g_posRank[SAN] = D3DXVECTOR3(NO_4POSX, NO_4POSY, POS_Z);
	g_posRank[FOUR] = D3DXVECTOR3(NO_5POSX, NO_5POSY, POS_Z);

	//�����L���O�Ɏg�p����e���ʂ̊e���̏�񏉊���
	for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
	{		
		aTexU[0] = g_nRanking[nCntRanking] / 10000000;
		aTexU[1] = g_nRanking[nCntRanking] % 10000000 / 1000000;
		aTexU[2] = g_nRanking[nCntRanking] % 1000000 / 100000;
		aTexU[3] = g_nRanking[nCntRanking] % 100000 / 10000;
		aTexU[4] = g_nRanking[nCntRanking] % 10000 / 1000;
		aTexU[5] = g_nRanking[nCntRanking] % 1000 / 100;
		aTexU[6] = g_nRanking[nCntRanking] % 100 / 10;
		aTexU[7] = g_nRanking[nCntRanking] % 10;

		for (int mCntRanking = 0; mCntRanking < RANK_DEJIT; mCntRanking++)
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].pos.x = g_posRank[nCntRanking].x - (RANK_WIDTH  - (float)nCntRanking * RANK_SIZE) + ((float)mCntRanking * (WIDTH__DIS - (float)nCntRanking * DIS_SIZE));
				pVtx[0].pos.y = g_posRank[nCntRanking].y - (RANK_HEIGHT - (float)nCntRanking * RANK_SIZE) + ((float)nCntRanking * (HEIGHT_DIS));
				pVtx[1].pos.x = g_posRank[nCntRanking].x + (RANK_WIDTH  - (float)nCntRanking * RANK_SIZE) + ((float)mCntRanking * (WIDTH__DIS - (float)nCntRanking * DIS_SIZE));
				pVtx[1].pos.y = g_posRank[nCntRanking].y - (RANK_HEIGHT - (float)nCntRanking * RANK_SIZE) + ((float)nCntRanking * (HEIGHT_DIS));
				pVtx[2].pos.x = g_posRank[nCntRanking].x - (RANK_WIDTH  - (float)nCntRanking * RANK_SIZE) + ((float)mCntRanking * (WIDTH__DIS - (float)nCntRanking * DIS_SIZE));
				pVtx[2].pos.y = g_posRank[nCntRanking].y + (RANK_HEIGHT - (float)nCntRanking * RANK_SIZE) + ((float)nCntRanking * (HEIGHT_DIS));
				pVtx[3].pos.x = g_posRank[nCntRanking].x + (RANK_WIDTH  - (float)nCntRanking * RANK_SIZE) + ((float)mCntRanking * (WIDTH__DIS - (float)nCntRanking * DIS_SIZE));
				pVtx[3].pos.y = g_posRank[nCntRanking].y + (RANK_HEIGHT - (float)nCntRanking * RANK_SIZE) + ((float)nCntRanking * (HEIGHT_DIS));
			}

			//rhw�̐ݒ�
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//�e���ʂ̃J���[�ݒ�
			switch (nCntRanking)
			{
			case 0:					//No.1(���F)
				//���_�J���[�̐ݒ�
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				}
				break;

			case 1:					//No.2(�D�F)
				//���_�J���[�̐ݒ�
				{
					pVtx[0].col = D3DXCOLOR(GRAY, GRAY, GRAY, 1.0f);
					pVtx[1].col = D3DXCOLOR(GRAY, GRAY, GRAY, 1.0f);
					pVtx[2].col = D3DXCOLOR(GRAY, GRAY, GRAY, 1.0f);
					pVtx[3].col = D3DXCOLOR(GRAY, GRAY, GRAY, 1.0f);
				}
				break;

			case 2:					//No.3(�I�����W)
				//���_�J���[�̐ݒ�
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
				}
				break;

			case 3:					//NO.4(����)
			case 4:					//No.5(����)
				//���_�J���[�̐ݒ�
				{
					pVtx[0].col = D3DXCOLOR(BLUCK, BLUCK, BLUCK, 1.0f);
					pVtx[1].col = D3DXCOLOR(BLUCK, BLUCK, BLUCK, 1.0f);
					pVtx[2].col = D3DXCOLOR(BLUCK, BLUCK, BLUCK, 1.0f);
					pVtx[3].col = D3DXCOLOR(BLUCK, BLUCK, BLUCK, 1.0f);
				}
				break;
			}

			//�e�N�X�`���̍��W
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[mCntRanking], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[mCntRanking], 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[mCntRanking], 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[mCntRanking], 1.0f);
			}

			pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffRanking->Unlock();

	g_Rank_Time = timeGetTime();	//�\�����ԑ���J�n
}

//�����L���O�̏I������
void UninitRanking(void)
{
	//�e�N�X�`���̔j��(2��)
	for (int nCntRankTex = START_CNT; nCntRankTex < RANK_TEXTURE; nCntRankTex++)
	{
		if (g_pTextureRanking[nCntRankTex] != NULL)
		{
			g_pTextureRanking[nCntRankTex]->Release();
			g_pTextureRanking[nCntRankTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//�����L���O�̍X�V����
void UpdateRanking(void)
{
	DWORD EndRankTime = timeGetTime();	//�����L���O�\���o�ߎ���

	if (GetKeyboardTrigger(DIK_RETURN) == true || EndRankTime - g_Rank_Time >= RANK_TIME_LIMIT)
	{
		SetFade(MODE_TITLE);	//�^�C�g����ʂɈڍs
	}
}

//�����L���O�̕`�揈��
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffRanking, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�w�i�摜�̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureRanking[RANK_BG_NUM]);

	//�w�i�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, RANK_BG_NUM, SEC);

	//�����e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRanking[RANK_NUMBER]);

	//�������̃|���S���̕`��
	for (nCntScore = START_CNT; nCntScore < ALL_NUM; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * NUM_VERTEX + NUM_VERTEX, SEC);
	}
}

//�����L���O�̐ݒ菈��
void ScanRanking(void)
{
	FILE *pFile;

	pFile = fopen(RANK_DATA, "r");	//�����L���O�t�@�C���ǂݍ���

	if (pFile != NULL)
	{
		for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
		{
			fscanf(pFile, "%d", &g_nRanking[nCntRanking]);		//�����L���O�z��Ɋi�[
		}

		fclose(pFile);

		DownSort();		//���ёւ������Ɉړ�
	}
}

//���ёւ�����
void DownSort(void) 
{
	int PlayerRank = GetScore();	//�v���C���[�̃����L���O

	if (PlayerRank > g_nRanking[FOUR])
	{//�v���C���[�̃X�R�A���A�ǂݍ��܂ꂽ���l�̍ŏ��l���傫�����������ւ�
		g_nRanking[FOUR] = PlayerRank;

		for (int nCntRanking = START_CNT; nCntRanking < FOUR; nCntRanking++)				//��r����鐔�l(��̒l)
		{
			for (int mCntRanking = nCntRanking + ONE; mCntRanking < RANK_NUM; mCntRanking++)	//��r���鐔�l
			{
				//��r�������l����̒l���傫����΃o�u���\�[�g���s��
				if (g_nRanking[nCntRanking] < g_nRanking[mCntRanking])
				{
					int nTmp = g_nRanking[nCntRanking];					//��̒l���ꎞ�ۊǏꏊ�Ɋi�[

					g_nRanking[nCntRanking] = g_nRanking[mCntRanking];	//��̒l�̏ꏊ���㏑��

					g_nRanking[mCntRanking] = nTmp;						//��r���鐔�l�̏ꏊ���㏑��
				}
			}
		}
		
		FILE *pFile;	//�t�@�C���|�C���^

		pFile = fopen(RANK_DATA, "w");

		//�����L���O������������
		if (pFile != NULL)
		{
			for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
			{
				fprintf(pFile, "%.8d\n", g_nRanking[nCntRanking]);		//���ʂ��Ƃ̃X�R�A�������o��
			}

			fclose(pFile);
		}
	}
}