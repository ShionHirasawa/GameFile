/*==========================================================================================================================

���[�f�B���O����(�������[�h���Ă��Ȃ�)[loading.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "loading.h"
#include "input.h"
#include "fade.h"

//�}�N����`
#define NUM_LOAD_TEX			(6)									//���[�f�B���O�摜�̃e�N�X�`���̐�
#define LOADING_COUNT			(200)								//���[�f�B���O��ʕ\������
#define LAODING_HEIGHT			(100.0f)							//���[�f�B���O�摜�̕\������

//�W�����v�e�N�X�`���̃}�N��
#define LOAD_WAVE_TEXTURE		"data/TEXTURE/Wave.png"				//�E�F�[�u�̃e�N�X�`���p�X
#define LOAD_WAVE_POS			(D3DXVECTOR3(700.0f, 500.0f, 0.0f))	//�E�F�[�u�̕\���ꏊ
#define LOAD_WAVE_WIDTH			(500.0f)							//�E�F�[�u�̕\����
#define LOAD_WAVE_HEIGHT		(80.0f)								//�E�F�[�u�̕\������
#define	LOAD_WAVE_TEXU			(1.0f)								//�e�N�X�`����U���W�̕�
#define LOAD_WAVE_ANIM			(8)									//�E�F�[�u�̃A�j���[�V�����ω��J�E���g

//Loading...�e�N�X�`���̃}�N��
#define LOAD_Load_TEXTURE		"data/TEXTURE/Loading.png"			//�E�F�[�u�̃e�N�X�`���p�X
#define LOAD_Load_POS			(D3DXVECTOR3(800.0f, 600.0f, 0.0f))	//�E�F�[�u�̕\���ꏊ
#define LOAD_Load_WIDTH			(300.0f)							//�E�F�[�u�̕\����
#define LOAD_Load_TEXU			(0.625f)							//�e�N�X�`����U���W�̕�
#define LOAD_ADD_TEXU			(0.125f)							//�e�N�X�`���̉��Z�l
#define LOAD_Load_ANIM			(14)								//Loading�̃A�j���[�V�����ω��J�E���g

//PRESS ENTER�̃}�N��
#define LOAD_ENTER_TEXTURE		"data/TEXTURE/PressEnter.png"		//�E�F�[�u�̃e�N�X�`���p�X
#define LOAD_ENTER_POS			(D3DXVECTOR3(800.0f, 600.0f, 0.0f))	//�E�F�[�u�̕\���ꏊ
#define LOAD_ENTER_WIDTH		(300.0f)							//�E�F�[�u�̕\����
#define LOAD_ENTER_TEXU			(1.0f)								//�e�N�X�`����U���W�̕�
#define FLASH_ENTER				(5)									//�\����_�ł�����

//----------------------------------------------------------------------
//							�R�}�̃}�N��
//----------------------------------------------------------------------
#define BIRTH_KOMA_COUNT		(10)								//�R�}���\������铧���x���Z��
#define KOMA_WIDTH				(200.0f)							//�R�}�̕\����
#define KOMA_HEIGHT				(200.0f)							//�R�}�̕\������
#define KOMA_TEXU				(1.0f)								//�e�N�X�`����U���W�̕�

//�R�}�P�ڂ̃}�N��
#define LOAD_KOMA000_TEXTURE	"data/TEXTURE/Scene000.png"			//�R�}1�ڂ̃e�N�X�`���p�X
#define LOAD_KOMA000_POS		(D3DXVECTOR3(250.0f, 200.0f, 0.0f))	//�R�}1�ڂ̕\���ꏊ
#define LOAD_KOMA000_BIRTH		(20)								//�摜�������^�C�~���O

//�R�}2�ڂ̃}�N��
#define LOAD_KOMA001_TEXTURE	"data/TEXTURE/Scene001.png"			//�R�}2�ڂ̃e�N�X�`���p�X
#define LOAD_KOMA001_POS		(D3DXVECTOR3(550.0f, 200.0f, 0.0f))	//�R�}2�ڂ̕\���ꏊ
#define LOAD_KOMA001_BIRTH		(80)								//�摜�������^�C�~���O

//�R�}3�ڂ̃}�N��
#define LOAD_KOMA002_TEXTURE	"data/TEXTURE/Scene002.png"			//�R�}2�ڂ̃e�N�X�`���p�X
#define LOAD_KOMA002_POS		(D3DXVECTOR3(850.0f, 200.0f, 0.0f))	//�R�}2�ڂ̕\���ꏊ
#define LOAD_KOMA002_BIRTH		(140)								//�摜�������^�C�~���O

//���[�f�B���O�e�N�X�`���̎�ނ̍\����
typedef enum
{
	LOAD_TEX_WAVE = ZERO,
	LOAD_TEX_Load,
	LOAD_TEX_ENTER,
	LOAD_TEX_KOMA000,
	LOAD_TEX_KOMA001,
	LOAD_TEX_KOMA002,
	LOAD_TEX_KOMA003,
	LOAD_TEX_MAX,
}LOAD_TEX;

//���[�f�B���O�摜�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	float fHeight;			//����
	float fWidht;			//��
	float TexU;				//�e�N�X�`����U���W
	float UWidth;			//�e�N�X�`���̕�
	int aCol;				//�e�N�X�`���̓����x
	int nCounterAnim;		//�A�j���[�V�����J�E���^
	bool bUse;				//�g�p����Ă��邩�ǂ���
}Loading;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureLoading[NUM_LOAD_TEX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffLoading = NULL;				//���_�o�b�t�@�ւ̃|�C���^
Loading						g_Loading[NUM_LOAD_TEX];				//���[�f�B���O�̍\����
int							g_nLoadingCounter;						//���[�f�B���O�\������
bool						g_bFlash;								//PressEnter�̓_�Ő܂�Ԃ�

//���[�f�B���O�̏���������
void InitLoading(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^ + �擾

	//���[�f�B���O���̏�����
	InitInfoLoading();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, LOAD_WAVE_TEXTURE, &g_pTextureLoading[LOAD_TEX_WAVE]);		//�E�F�[�u�̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, LOAD_Load_TEXTURE, &g_pTextureLoading[LOAD_TEX_Load]);		//Loading... �̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, LOAD_ENTER_TEXTURE, &g_pTextureLoading[LOAD_TEX_ENTER]);		//PRESS ENTER �̃e�N�X�`��

	//----------------------------------------------------------------------
	//						�R�}�̃e�N�X�`���ǂݍ���
	//----------------------------------------------------------------------
	D3DXCreateTextureFromFile(pDevice, LOAD_KOMA000_TEXTURE, &g_pTextureLoading[LOAD_TEX_KOMA000]);	//�R�}1�� �̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, LOAD_KOMA001_TEXTURE, &g_pTextureLoading[LOAD_TEX_KOMA001]);	//�R�}2�� �̃e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, LOAD_KOMA002_TEXTURE, &g_pTextureLoading[LOAD_TEX_KOMA002]);	//�R�}3�� �̃e�N�X�`��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_LOAD_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLoading, NULL);

	//���_���W�̐ݒ�
	VertexLoading();
}

//���[�f�B���O���̏�����
void InitInfoLoading(void)
{
	//���[�f�B���O�̏�񏉊���
	g_nLoadingCounter = ZERO;								//���[�f�B���O�\�����ԏ�����

	//�E�F�[�u����摜�̏�����
	g_Loading[LOAD_TEX_WAVE].pos			 = LOAD_WAVE_POS;	//�E�F�[�u�摜�̈ʒu������
	g_Loading[LOAD_TEX_WAVE].fWidht			 = LOAD_WAVE_WIDTH;	//�\�����̏�����
	g_Loading[LOAD_TEX_WAVE].fHeight		 = LOAD_WAVE_HEIGHT;//�\�����̏�����
	g_Loading[LOAD_TEX_WAVE].TexU			 = F_ZERO;			//�e�N�X�`���̍��W������
	g_Loading[LOAD_TEX_WAVE].UWidth			 = LOAD_WAVE_TEXU;	//�e�N�X�`���̕�������
	g_Loading[LOAD_TEX_WAVE].aCol			 = COLOR_MAX;		//�����x������
	g_Loading[LOAD_TEX_WAVE].nCounterAnim	 = ZERO;			//�A�j���[�V�����J�E���^�[������
	g_Loading[LOAD_TEX_WAVE].bUse			 = true;			//�g�p����Ă����Ԃ�

	//Loading...�@�̉摜�̏�����
	g_Loading[LOAD_TEX_Load].pos			 = LOAD_Load_POS;	//Loading...�摜�̈ʒu������
	g_Loading[LOAD_TEX_Load].fWidht			 = LOAD_Load_WIDTH;	//�\�����̏�����
	g_Loading[LOAD_TEX_Load].fHeight		 = LAODING_HEIGHT;	//�\�������̏�����
	g_Loading[LOAD_TEX_Load].TexU			 = F_ZERO;			//�e�N�X�`���̍��W������
	g_Loading[LOAD_TEX_Load].UWidth			 = LOAD_Load_TEXU;	//�e�N�X�`���̕�������
	g_Loading[LOAD_TEX_Load].aCol			 = COLOR_MAX;		//�����x������
	g_Loading[LOAD_TEX_Load].nCounterAnim	 = ZERO;			//�A�j���[�V�����J�E���^�[������
	g_Loading[LOAD_TEX_Load].bUse			 = true;			//�g�p����Ă����Ԃ�

	//PRESS ENTER �̉摜�̏�����
	g_Loading[LOAD_TEX_ENTER].pos			 = LOAD_ENTER_POS;		//PRESS ENTER�摜�̈ʒu������
	g_Loading[LOAD_TEX_ENTER].fWidht		 = LOAD_ENTER_WIDTH;	//�\�����̏�����
	g_Loading[LOAD_TEX_ENTER].fHeight		 = LAODING_HEIGHT;		//�\�������̏�����
	g_Loading[LOAD_TEX_ENTER].TexU			 = F_ZERO;				//�e�N�X�`���̍��W������
	g_Loading[LOAD_TEX_ENTER].UWidth		 = LOAD_ENTER_TEXU;		//�e�N�X�`���̕�������
	g_Loading[LOAD_TEX_ENTER].aCol			 = COLOR_MIN;			//�����x������
	g_Loading[LOAD_TEX_ENTER].nCounterAnim	 = ZERO;				//�A�j���[�V�����J�E���^�[������
	g_Loading[LOAD_TEX_ENTER].bUse			 = false;				//�g�p����ĂȂ���Ԃ�
	g_bFlash = false;												//�_�ł̐܂�Ԃ�������

	//----------------------------------------------------------------------
	//							�R�}�̏�񏉊���
	//----------------------------------------------------------------------
	//�R�}1�� �̉摜�̏�����
	g_Loading[LOAD_TEX_KOMA000].pos			 = LOAD_KOMA000_POS;	//1�ڂ̃R�}�̈ʒu������
	g_Loading[LOAD_TEX_KOMA000].fWidht		 = KOMA_WIDTH;			//�\�����̏�����
	g_Loading[LOAD_TEX_KOMA000].fHeight		 = KOMA_HEIGHT;			//�\�������̏�����
	g_Loading[LOAD_TEX_KOMA000].TexU		 = F_ZERO;				//�e�N�X�`���̍��W������
	g_Loading[LOAD_TEX_KOMA000].UWidth		 = KOMA_TEXU;			//�e�N�X�`���̕�������
	g_Loading[LOAD_TEX_KOMA000].aCol		 = COLOR_MIN;			//�����x������
	g_Loading[LOAD_TEX_KOMA000].nCounterAnim = ZERO;				//�A�j���[�V�����J�E���^�[������
	g_Loading[LOAD_TEX_KOMA000].bUse		 = false;				//�g�p����ĂȂ���Ԃ�

	//�R�}2�� �̉摜�̏�����
	g_Loading[LOAD_TEX_KOMA001].pos			 = LOAD_KOMA001_POS;	//2�ڂ̃R�}�̈ʒu������
	g_Loading[LOAD_TEX_KOMA001].fWidht		 = KOMA_WIDTH;			//�\�����̏�����
	g_Loading[LOAD_TEX_KOMA001].fHeight		 = KOMA_HEIGHT;			//�\�������̏�����
	g_Loading[LOAD_TEX_KOMA001].TexU		 = F_ZERO;				//�e�N�X�`���̍��W������
	g_Loading[LOAD_TEX_KOMA001].UWidth		 = KOMA_TEXU;			//�e�N�X�`���̕�������
	g_Loading[LOAD_TEX_KOMA001].aCol		 = COLOR_MIN;			//�����x������
	g_Loading[LOAD_TEX_KOMA001].nCounterAnim = ZERO;				//�A�j���[�V�����J�E���^�[������
	g_Loading[LOAD_TEX_KOMA001].bUse		 = false;				//�g�p����ĂȂ���Ԃ�

	//�R�}2�� �̉摜�̏�����
	g_Loading[LOAD_TEX_KOMA002].pos			 = LOAD_KOMA002_POS;	//3�ڂ̃R�}�̈ʒu������
	g_Loading[LOAD_TEX_KOMA002].fWidht		 = KOMA_WIDTH;			//�\�����̏�����
	g_Loading[LOAD_TEX_KOMA002].fHeight		 = KOMA_HEIGHT;			//�\�������̏�����
	g_Loading[LOAD_TEX_KOMA002].TexU		 = F_ZERO;				//�e�N�X�`���̍��W������
	g_Loading[LOAD_TEX_KOMA002].UWidth		 = KOMA_TEXU;			//�e�N�X�`���̕�������
	g_Loading[LOAD_TEX_KOMA002].aCol		 = COLOR_MIN;			//�����x������
	g_Loading[LOAD_TEX_KOMA002].nCounterAnim = ZERO;				//�A�j���[�V�����J�E���^�[������
	g_Loading[LOAD_TEX_KOMA002].bUse		 = false;				//�g�p����ĂȂ���Ԃ�
}

//���[�f�B���O�̏I������
void UninitLoading(void)
{
	//�e�N�X�`���̔j��
	for (int nCntLoad = START_CNT; nCntLoad < NUM_LOAD_TEX; nCntLoad++)
	{
		if (g_pTextureLoading[nCntLoad] != NULL)
		{
			g_pTextureLoading[nCntLoad]->Release();
			g_pTextureLoading[nCntLoad] = NULL;
		}

	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLoading != NULL)
	{
		g_pVtxBuffLoading->Release();
		g_pVtxBuffLoading = NULL;
	}
}

//���[�f�B���O�̍X�V����
void UpdateLoading(void)
{
	//Wave�̍X�V����
	UpdateTexWave();

	//Loading...�̍X�V����
	UpdateTexLoad();
	
	//PRSS ENTER�̍X�V����
	UpdateTexEnter();

	//�R�}�S�̂̍X�V����
	UpdateTexKoma();

	//���[�f�B���O�\�����Ԍv��
	g_nLoadingCounter++;

	if (g_nLoadingCounter >= LOADING_COUNT)
	{
		g_Loading[LOAD_TEX_Load].bUse = false;		//Loading... �͕\�������Ȃ�
		g_Loading[LOAD_TEX_ENTER].bUse = true;		//PRESS ENTER�͕\��������
		XINPUT_STATE *xInput = GetXInputState();	//XInput�̃|�C���^

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START))
		{
			//�Q�[����ʂɈڍs
			SetFade(MODE_GAME);
		}
	}

	//���_���W�̐ݒ�
	VertexLoading();
}

//���[�f�B���O�̕`�揈��
void DrawLoading(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^ + �擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffLoading, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLoad = START_CNT; nCntLoad < NUM_LOAD_TEX; nCntLoad++)
	{
		//�g�p����Ă���
		if (g_Loading[nCntLoad].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureLoading[nCntLoad]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLoad * NUM_VERTEX, SEC);
		}
	}
}

//���[�f�B���O�̒��_�ݒ�
void VertexLoading(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLoading->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntLoad = START_CNT; nCntLoad < NUM_LOAD_TEX; nCntLoad++, pVtx += NUM_VERTEX)
	{
		if (g_Loading[nCntLoad].bUse == true)
		{
			//���_���W�̐ݒ�
			{
				//X���W�̐ݒ�
				pVtx[ZERO].pos.x = g_Loading[nCntLoad].pos.x;
				pVtx[ONE].pos.x = g_Loading[nCntLoad].pos.x + g_Loading[nCntLoad].fWidht;
				pVtx[SEC].pos.x = g_Loading[nCntLoad].pos.x;
				pVtx[SAN].pos.x = g_Loading[nCntLoad].pos.x + g_Loading[nCntLoad].fWidht;

				//Y���W�̐ݒ�
				pVtx[ZERO].pos.y = g_Loading[nCntLoad].pos.y;
				pVtx[ONE].pos.y = g_Loading[nCntLoad].pos.y;
				pVtx[SEC].pos.y = g_Loading[nCntLoad].pos.y + g_Loading[nCntLoad].fHeight;
				pVtx[SAN].pos.y = g_Loading[nCntLoad].pos.y + g_Loading[nCntLoad].fHeight;

				//Z���W�̐ݒ�(�ω��Ȃ�)
				pVtx[ZERO].pos.z = g_Loading[nCntLoad].pos.z;
				pVtx[ONE].pos.z = g_Loading[nCntLoad].pos.z;
				pVtx[SEC].pos.z = g_Loading[nCntLoad].pos.z;
				pVtx[SAN].pos.z = g_Loading[nCntLoad].pos.z;
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
				
				pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Loading[nCntLoad].aCol);
				pVtx[ONE].col =  D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Loading[nCntLoad].aCol);
				pVtx[SEC].col =  D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Loading[nCntLoad].aCol);
				pVtx[SAN].col =  D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Loading[nCntLoad].aCol);
			}

			//�e�N�X�`���̍��W
			{
				pVtx[ZERO].tex = D3DXVECTOR2(g_Loading[nCntLoad].TexU, ORIGIN_POS);
				pVtx[ONE].tex = D3DXVECTOR2(g_Loading[nCntLoad].TexU + g_Loading[nCntLoad].UWidth, ORIGIN_POS);
				pVtx[SEC].tex = D3DXVECTOR2(g_Loading[nCntLoad].TexU, F_TEN);
				pVtx[SAN].tex = D3DXVECTOR2(g_Loading[nCntLoad].TexU + g_Loading[nCntLoad].UWidth, F_TEN);
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffLoading->Unlock();
}

//�E�F�[�u�e�N�X�`���̍X�V����
void UpdateTexWave(void)
{
	g_Loading[LOAD_TEX_WAVE].nCounterAnim++;

	if (g_Loading[LOAD_TEX_WAVE].nCounterAnim == LOAD_WAVE_ANIM)
	{
		g_Loading[LOAD_TEX_WAVE].nCounterAnim = ZERO;

		g_Loading[LOAD_TEX_WAVE].TexU -= F_ONE;

		if (g_Loading[LOAD_TEX_WAVE].TexU <= F_ZERO)
		{
			g_Loading[LOAD_TEX_WAVE].TexU = F_TEN;
		}
	}
}

//Loading...�e�N�X�`���̍X�V����
void UpdateTexLoad(void)
{
	g_Loading[LOAD_TEX_Load].nCounterAnim++;

	if (g_Loading[LOAD_TEX_Load].nCounterAnim == LOAD_Load_ANIM)
	{
		g_Loading[LOAD_TEX_Load].nCounterAnim = ZERO;

		g_Loading[LOAD_TEX_Load].pos.x -= 10.0f;
		g_Loading[LOAD_TEX_Load].UWidth += LOAD_ADD_TEXU;
		g_Loading[LOAD_TEX_Load].fWidht += 40.0f;

		if (g_Loading[LOAD_TEX_Load].UWidth > F_TEN)
		{
			g_Loading[LOAD_TEX_Load].pos	= LOAD_Load_POS;
			g_Loading[LOAD_TEX_Load].UWidth = LOAD_Load_TEXU;
			g_Loading[LOAD_TEX_Load].fWidht = LOAD_Load_WIDTH;
		}
	}
}

//PRESS ENTER�e�N�X�`���̍X�V����
void UpdateTexEnter(void)
{
	if (g_Loading[LOAD_TEX_ENTER].bUse == true)
	{
		//�����x�����Z(�����)
		if (g_bFlash == false)
		{
			//�ǂ�ǂ񌻂��
			g_Loading[LOAD_TEX_ENTER].aCol += FLASH_ENTER;

			//�����x�܂�Ԃ�
			if (g_Loading[LOAD_TEX_ENTER].aCol >= COLOR_MAX)
			{
				g_Loading[LOAD_TEX_ENTER].aCol = COLOR_MAX;
				g_bFlash = true;
			}
		}

		else
		{
			//�ǂ�ǂ񔖂�Ă���
			g_Loading[LOAD_TEX_ENTER].aCol -= FLASH_ENTER;
			
			//�����x�܂�Ԃ�
			if (g_Loading[LOAD_TEX_ENTER].aCol <= COLOR_MIN)
			{
				g_Loading[LOAD_TEX_ENTER].aCol = COLOR_MIN;
				g_bFlash = false;
			}
		}
	}
}

//----------------------------------------------------------------------
//							�R�}�̍X�V����
//----------------------------------------------------------------------

//�R�}�S�̂̍X�V����
void UpdateTexKoma(void)
{
	//�R�}1�ڂ̍X�V����
	Koma000Update();

	//�R�}2�ڂ̍X�V����
	Koma001Update();

	//�R�}3�ڂ̍X�V����
	Koma002Update();
}

//�R�}1�ڂ̍X�V����
void Koma000Update(void)
{
	if (g_nLoadingCounter >= LOAD_KOMA000_BIRTH)
	{
		g_Loading[LOAD_TEX_KOMA000].bUse = true;

		g_Loading[LOAD_TEX_KOMA000].aCol += BIRTH_KOMA_COUNT;

		if (g_Loading[LOAD_TEX_KOMA000].aCol > COLOR_MAX)
		{
			g_Loading[LOAD_TEX_KOMA000].aCol = COLOR_MAX;
		}
	}
}

//�R�}2�ڂ̍X�V����
void Koma001Update(void)
{
	if (g_nLoadingCounter >= LOAD_KOMA001_BIRTH)
	{
		g_Loading[LOAD_TEX_KOMA001].bUse = true;

		g_Loading[LOAD_TEX_KOMA001].aCol += BIRTH_KOMA_COUNT;

		if (g_Loading[LOAD_TEX_KOMA001].aCol > COLOR_MAX)
		{
			g_Loading[LOAD_TEX_KOMA001].aCol = COLOR_MAX;
		}
	}
}

//�R�}3�ڂ̍X�V����
void Koma002Update(void)
{
	if (g_nLoadingCounter >= LOAD_KOMA002_BIRTH)
	{
		g_Loading[LOAD_TEX_KOMA002].bUse = true;

		g_Loading[LOAD_TEX_KOMA002].aCol += BIRTH_KOMA_COUNT;

		if (g_Loading[LOAD_TEX_KOMA002].aCol > COLOR_MAX)
		{
			g_Loading[LOAD_TEX_KOMA002].aCol = COLOR_MAX;
		}
	}
}