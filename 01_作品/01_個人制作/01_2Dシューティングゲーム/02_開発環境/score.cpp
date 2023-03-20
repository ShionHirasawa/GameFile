/*==========================================================================================================================

													�X�R�A�\������[score.cpp]
														Author:���V����

==========================================================================================================================*/
#include "score.h"

//�}�N����`
#define NUM_PLACE				(8)		//�X�R�A�̌���

#define TENMILLIONSPLACE		10000000
#define MILLIONS_PLACE			10000000 / 1000000
#define HUNDREDTHOUSANDS_PLACE	1000000 / 100000
#define TENTHOUSANDS_PLACE		100000 / 10000
#define THOUSANDS_PLACE			10000 / 1000
#define HUNDREDS_PLACE			1000 / 100
#define TENS_PLACE				100 / 10
#define ONES_PLACE				10


//�S���̈ʂ܂ł̍\����
typedef enum
{
	DEJIT_PLACE_TENMILLION		= ZERO,	//�疜�̈�
	DEJIT_PLACE_MILLION			,			//�S���̈�
	DEJIT_PLACE_HUNDREDTHOUSAND	,	//�\���̈�
	DEJIT_PLACE_TENTHOUSAND		,		//���̈�
	DEJIT_PLACE_THOUSAND		,			//��̈�
	DEJIT_PLACE_HUNDRED			,			//�S�̈�
	DEJIT_PLACE_TEN				,				//�\�̈�
	DEJIT_PLACE_ONE				,				//��̈�
	DEJIT_PLACE_MAX
}DEJIT_PLACE;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posScore;								//�X�R�A�̈ʒu
int g_nScore;										//�X�R�A�̒l

//�X�R�A�̏���������
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureScore);

	//�X�R�A�̏��̏�����
	g_posScore = D3DXVECTOR3(1000.0f, 3.0f, 0.0f);				//�X�R�A�̈ʒu�̏�����
	g_nScore = 0;												//�X�R�A�̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_PLACE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void* *)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[0].pos.x = (g_posScore.x     ) + nCntScore * 35;
			pVtx[0].pos.y = (g_posScore.y     );
			pVtx[1].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[1].pos.y = (g_posScore.y     );
			pVtx[2].pos.x = (g_posScore.x     ) + nCntScore * 35;
			pVtx[2].pos.y = (g_posScore.y + 60);
			pVtx[3].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[3].pos.y = (g_posScore.y + 60);
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
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * g_nScore, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * g_nScore, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * g_nScore, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * g_nScore, 1.0f);
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffScore->Unlock();
}

//�X�R�A�̏I������
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//�X�R�A�̍X�V����
void UpdateScore(void)
{
	
}

//�X�R�A�̕`�揈��
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffScore, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureScore);

	for (nCntScore = START_CNT; nCntScore < NUM_PLACE; nCntScore++)
	{
		 //�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * NUM_VERTEX, SEC);
	}
}

//�X�R�A�̐ݒ菈��
void SetScore(int nScore)
{
	int nCntScore;
	int aTexU[NUM_PLACE];			//�e���̐������i�[
	g_nScore = nScore;

	//�X�R�A�̊e���̒��o
	aTexU[DEJIT_PLACE_TENMILLION]		= g_nScore / TENMILLIONSPLACE;
	aTexU[DEJIT_PLACE_MILLION]			= g_nScore % MILLIONS_PLACE;
	aTexU[DEJIT_PLACE_HUNDREDTHOUSAND]	= g_nScore % HUNDREDTHOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_TENTHOUSAND]		= g_nScore % TENTHOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_THOUSAND]			= g_nScore % THOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_HUNDRED]			= g_nScore % HUNDREDS_PLACE;
	aTexU[DEJIT_PLACE_TEN]				= g_nScore % TENS_PLACE;
	aTexU[DEJIT_PLACE_ONE]				= g_nScore % ONES_PLACE;
	
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntScore = START_CNT; nCntScore < NUM_PLACE; nCntScore++)
	{
		//���_���̐ݒ�
		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos.x = (g_posScore.x) + nCntScore * 35;
			pVtx[ZERO].pos.y = (g_posScore.y);
			pVtx[ONE].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[ONE].pos.y = (g_posScore.y);
			pVtx[SEC].pos.x = (g_posScore.x) + nCntScore * 35;
			pVtx[SEC].pos.y = (g_posScore.y + 60);
			pVtx[SAN].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[SAN].pos.y = (g_posScore.y + 60);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[nCntScore], 1.0f);
			pVtx[SAN].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 1.0f);
		}
		
		pVtx += NUM_VERTEX;
	}
	//���_�o�b�t�@�����b�N����
	g_pVtxBuffScore->Unlock();
}

//�X�R�A�̉��Z����
void AddScore(int nValue)
{
	int nCntScore;
	int aTexU[NUM_PLACE];			//�e���̐������i�[
	g_nScore += nValue;

	aTexU[DEJIT_PLACE_TENMILLION] = g_nScore / TENMILLIONSPLACE;
	aTexU[DEJIT_PLACE_MILLION] = g_nScore % MILLIONS_PLACE;
	aTexU[DEJIT_PLACE_HUNDREDTHOUSAND] = g_nScore % HUNDREDTHOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_TENTHOUSAND] = g_nScore % TENTHOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_THOUSAND] = g_nScore % THOUSANDS_PLACE;
	aTexU[DEJIT_PLACE_HUNDRED] = g_nScore % HUNDREDS_PLACE;
	aTexU[DEJIT_PLACE_TEN] = g_nScore % TENS_PLACE;
	aTexU[DEJIT_PLACE_ONE] = g_nScore % ONES_PLACE;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntScore = START_CNT; nCntScore < NUM_PLACE; nCntScore++)
	{
		//���_���̐ݒ�
		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos.x = (g_posScore.x) + nCntScore * 35;
			pVtx[ZERO].pos.y = (g_posScore.y);
			pVtx[ONE].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[ONE].pos.y = (g_posScore.y);
			pVtx[SEC].pos.x = (g_posScore.x) + nCntScore * 35;
			pVtx[SEC].pos.y = (g_posScore.y + 60);
			pVtx[SAN].pos.x = (g_posScore.x + 35) + nCntScore * 35;
			pVtx[SAN].pos.y = (g_posScore.y + 60);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(0.0f + 0.1f * aTexU[nCntScore], 1.0f);
			pVtx[SAN].tex = D3DXVECTOR2(0.1f + 0.1f * aTexU[nCntScore], 1.0f);
		}

		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffScore->Unlock();
}

//�X�R�A�̒l���l��
int GetScore(void)
{
	return g_nScore;	//�v���C���[�̃X�R�A��Ԃ�
}