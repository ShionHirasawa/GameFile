/*==========================================================================================================================

													�^�C�g����ʏ���[title.cpp]
														Author:���V����

==========================================================================================================================*/
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//�}�N����`
#define TITLE_LOGO		(3)			//�^�C�g����ʂŎg���e�N�X�`���̐�

//�^�C�g�����̃}�N��
#define SET_TITLE_POS	D3DXVECTOR3(640.0f, 230.0f, F_ZERO)	//�^�C�g�����̃Z�b�g�|�W�V����

#define TITLE_POSX		(640.0f)	// X���W�����ʒu
#define TITLE_POSY		(-50.0f)	// Y���W�����ʒu
#define TITLE_WIDTH		(400.0f)	// ��
#define TITLE_HEIGHT	(110.0f)	// ����
#define FALL_TITLE		(2.0f)		//�^�C�g�����������l
#define LENGTH_TIMES	(0.5f)		//�����̌v�Z�ɗp����{��

//ENTER���S�̃}�N��
#define ENTER_POS_ZERO	D3DXVECTOR3(540.0f, 550.0f, 0.0f)
#define ENTER_POS_ONE	D3DXVECTOR3(740.0f, 550.0f, 0.0f)
#define ENTER_POS_SEC	D3DXVECTOR3(540.0f, 620.0f, 0.0f)
#define ENTER_POS_SAN	D3DXVECTOR3(740.0f, 620.0f, 0.0f)

//�F�֘A�̃}�N��
#define COLOR_ON		(255)		//�L�F
#define COLOR_OFF		(0)			//���F
#define FLASH_CNT		(-3)		//�_�ŃJ�E���^�̏����l
#define FLASH_INTERVAL	(3)			//�_�ł̊Ԋu
#define SLOW_FLASH		(4)			//�������_�ł����邽�߂̃}�N��
#define MAKE_CLEAR		(250)		//�����ɂ�������]���n
#define MAKE_OPAQUE		(20)		//�s�����ɂ�������]���n
#define FLASH_TIME		(100)		//�_�ŕ\������( ENTER ��������Ă���t�F�[�h�Ɉڍs����܂�)

//�^�C�g�����̃A�j���[�V�����}�N��
#define ADDSPIN			(0.19f)		//��]�̉��Z�l
#define LIMITSPIN		(31.35f)	//��]�̏���l
#define DEFSPIN			(31.42f)	//��]�̊�l
#define MINUSLENGTH		(1.0f)		//�����̌��Z�l
#define MINUSCROSS		(2.6f)		//�����̌��Z�l
#define CROSS_POS		(400.0f)	//�΂߂̏����l
#define FDEFAULT		(0.0f)		//��l

#define TITLE_COLOR		(130)		//�^�C�g����R�J���[

#define TITLE_BG_TEX	"data/TEXTURE/�w�i.png"        	
#define TITLE_NAME_TEX	"data/TEXTURE/��i��.png"      
#define TITLE_ENTER_TEX	"data/TEXTURE/�G���^�[����.png"

//�^�C�g�����\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	float fAngle;			//���S���璸�_�܂ł̊p�x
	float flength;			//���S���璸�_�܂ł̒���
	float fLengthMinus;		//���������X�Ɍ��ɖ߂����߂̕ϐ�
}TitleName;

//�^�C�g���W��
typedef enum
{
	TITLE_DISPLAY = ZERO,
	TITLE_NAME,
	TITLE_ENTER,
	TITLE_MAX
}TITLE;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureTitle[TITLE_LOGO] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				//���_�o�b�t�@�ւ̃|�C���^
TitleName g_Title;
float g_CrossDown;											//�΂߉E�ォ��^�C�g���\��
int g_FlashCnt;												//PRESS ENTER��_�ł�����J�E���^
int g_SlowFlash;											//�������_�ł�����
int g_aFlash;												//�_�ŗp�̕ϐ�

void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TITLE_BG_TEX   , &g_pTextureTitle[TITLE_DISPLAY]);	//�^�C�g����ʔw�i
	D3DXCreateTextureFromFile(pDevice, TITLE_NAME_TEX , &g_pTextureTitle[TITLE_NAME]);		//�Q�[���^�C�g��
	D3DXCreateTextureFromFile(pDevice, TITLE_ENTER_TEX, &g_pTextureTitle[TITLE_ENTER]);		//PRESS THE ENTER

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TITLE_LOGO, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	g_Title.pos  = D3DXVECTOR3(TITLE_POSX, TITLE_POSY, F_ZERO);
	g_Title.move = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);		//�ړ��ʂ�����������
	g_Title.rot  = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);		//����������������(Z�l���g�p)

	//�������Z�o
	g_Title.flength = sqrtf(TITLE_WIDTH * TITLE_WIDTH + TITLE_HEIGHT * TITLE_HEIGHT) * LENGTH_TIMES;

	//�����ɕω���t����ϐ��̏�����
	g_Title.fLengthMinus = g_Title.flength;

	//�p�x���Z�o
	g_Title.fAngle = atan2f(TITLE_WIDTH, TITLE_HEIGHT);

	//�΂߂���̍��W��������
	g_CrossDown = CROSS_POS;

	//���ꂼ���UI��������
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++, pVtx += NUM_VERTEX)
	{
		switch (nCnt)
		{
		case TITLE_DISPLAY:		//�^�C�g�����
			{
			//���_���W�̐ݒ�
			pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS , ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS  , SCREEN_HEIGHT, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);

			//rhw�̐ݒ�
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;
			
			//���_�J���[�̐ݒ�
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			
			//�e�N�X�`���̍��W
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , F_TEN);
			}
			break;

		case TITLE_NAME:		//�Q�[���^�C�g��
		{
			//���_���W�̐ݒ�
			pVtx[ZERO].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (-D3DX_PI + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
			pVtx[ZERO].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (-D3DX_PI + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
			pVtx[ONE].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (D3DX_PI - g_Title.fAngle))  * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
			pVtx[ONE].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (D3DX_PI - g_Title.fAngle))  * (g_Title.flength - g_Title.fLengthMinus);
			pVtx[SEC].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (ZERO - g_Title.fAngle))     * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
			pVtx[SEC].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (ZERO - g_Title.fAngle))     * (g_Title.flength - g_Title.fLengthMinus);
			pVtx[SAN].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (ZERO + g_Title.fAngle))     * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
			pVtx[SAN].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (ZERO + g_Title.fAngle))     * (g_Title.flength - g_Title.fLengthMinus);

			//rhw�̐ݒ�
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;

			//���_�J���[�̐ݒ�
			pVtx[ZERO].col = D3DCOLOR_RGBA(TITLE_COLOR, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[ONE].col = D3DCOLOR_RGBA(TITLE_COLOR, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SEC].col = D3DCOLOR_RGBA(TITLE_COLOR, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SAN].col = D3DCOLOR_RGBA(TITLE_COLOR, COLOR_ON, COLOR_ON, COLOR_ON);

			//�e�N�X�`���̍��W
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , F_TEN);
		}
			break;

		case TITLE_ENTER:		//PRESS THE ENTER
			{
			//���_���W�̐ݒ�
			pVtx[ZERO].pos = ENTER_POS_ZERO;
			pVtx[ONE].pos = ENTER_POS_ONE;
			pVtx[SEC].pos = ENTER_POS_SEC;
			pVtx[SAN].pos = ENTER_POS_SAN;

			//rhw�̐ݒ�
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;

			//���_�J���[�̐ݒ�
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);

			//�e�N�X�`���̍��W
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN, F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO, F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
			}
			break;
		}
	}
	g_FlashCnt = FLASH_CNT;			//�_�ŃJ�E���^�̏�����
	g_aFlash = COLOR_ON;			//�_�ł̐F������
	g_SlowFlash = SLOW_FLASH;		//�X���[�_�ł̏�����

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM000);
}

void UninitTitle(void)
{
	//�e�N�X�`��(�O��)�̔j��
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

void UpdateTitle(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�w�i�ȊO���X�V���邽��
	pVtx += NUM_VERTEX;

	//�������������߂�
	if (g_Title.fLengthMinus > FDEFAULT)
	{
		g_Title.fLengthMinus -= MINUSLENGTH;
	}

	//��]�l�����Z
	if (g_Title.rot.z < LIMITSPIN)
	{
		g_Title.rot.z += ADDSPIN;

		if (g_Title.rot.z >= LIMITSPIN)
		{
			g_Title.rot.z = DEFSPIN;
		}
	}

	//�΂߂���̗��������Z
	if (g_CrossDown >= FDEFAULT)
	{
		g_CrossDown -= MINUSCROSS;
	}

	pVtx[ZERO].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (-D3DX_PI + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
	pVtx[ZERO].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (-D3DX_PI + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
	pVtx[ONE].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + ( D3DX_PI - g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
	pVtx[ONE].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + ( D3DX_PI - g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
	pVtx[SEC].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (    ZERO - g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
	pVtx[SEC].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (    ZERO - g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
	pVtx[SAN].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (    ZERO + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
	pVtx[SAN].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (    ZERO + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
	
	//PRESS THE ENTER �̒��_�|�C���^
	pVtx += NUM_VERTEX;

	//�������_�ł�����
	if (g_FlashCnt == FLASH_CNT)
	{
		g_aFlash += g_SlowFlash;

		//�s�����ɂ���
		if (g_aFlash < MAKE_OPAQUE)
		{
			g_aFlash = MAKE_OPAQUE;

			g_SlowFlash *= MINUS_ONE;		//�����]��
		}

		//�����ɂ���
		else if (g_aFlash > MAKE_CLEAR)
		{
			g_aFlash = MAKE_CLEAR;

			g_SlowFlash *= MINUS_ONE;		//�����]��
		}
	}

	//�Q�[���J�n
	if (g_Title.pos == SET_TITLE_POS)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true && g_FlashCnt == FLASH_CNT)
		{//ENTER�L�[�������ꂽ
			g_FlashCnt++;
			PlaySound(SOUND_LABEL_SE_DECIDE);	//���艹��炷
		}

		if (g_FlashCnt > FLASH_CNT)
		{
			g_FlashCnt++;

			//�_�ł�����
			if (g_FlashCnt % FLASH_INTERVAL == COLOR_OFF)
			{
				g_aFlash = g_aFlash ? COLOR_OFF : COLOR_ON;
			}

			//���̉�ʂɈڍs
			if (g_FlashCnt >= FLASH_TIME)
			{
				//�t�F�[�h�Ăяo��
				SetFade(MODE_TUTORIAL);
			}
		}
	}

	else
	{
		//���_���W�̐ݒ�
		g_Title.pos.y += FALL_TITLE;

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//�^�C�g���Ƃ���Ɋ֘A����ϐ���W����
			g_Title.pos = SET_TITLE_POS;
			g_Title.rot.z = DEFSPIN;
			g_Title.fLengthMinus = FDEFAULT;
			g_CrossDown = FDEFAULT;
		}
	}

	//PRESS THE ENTER�̒��_�J���[
	pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, g_aFlash);
	pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, g_aFlash);
	pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, g_aFlash);
	pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, g_aFlash);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTitle->Unlock();
}

void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTitle, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
		
	//�e�N�X�`���̐ݒ�
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++)
	{
		pDevice->SetTexture(ZERO, g_pTextureTitle[nCnt]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * NUM_VERTEX, SEC);
	}
}