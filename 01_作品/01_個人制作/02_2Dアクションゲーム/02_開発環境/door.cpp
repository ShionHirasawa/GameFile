/*==========================================================================================================================

�S�[��(�X�e�[�W�����)�̃h�A�̏���[door.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "door.h"
#include "player.h"
#include "move.h"
#include "input.h"
#include "fade.h"

//�}�N����`
#define NUM_DOOR		(1)										//���̐�
#define DOOR_POS		(D3DXVECTOR3(4800.0f,430.0f, 0.0f))	//���̈ʒu
#define DOOR_WIDTH		(150.0f)								//�h�A�̉���
#define DOOR_HEIGHT		(100.0f)								//�h�A�̍���

typedef struct
{
	D3DXVECTOR3 pos;	//�h�A�̈ʒu
	float TexDoor;		//�h�A�̃e�N�X�`�����W
	int nCounterAnim;	//�A�j���[�V�����J�E���^
	int nJumpCnt;		//�W�����v�̉�
	bool bOpen;			//�h�A���J���Ă��邩
}Door;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureDoor = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffDoor = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Door g_Door;										//�h�A�̏��

//���̏���������
void InitDoor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^ + �擾

	//�h�A�̏�񏉊���
	g_Door.pos = DOOR_POS;			//�h�A�̍��W
	g_Door.TexDoor = F_ZERO;		//�h�A�͕��Ă���
	g_Door.nCounterAnim = ZERO;		//�A�j���[�V�����J�E���^�[�[��
	g_Door.nJumpCnt = ZERO;			//�W�����v���Ă��Ȃ�(�v���C���[��)
	g_Door.bOpen = false;			//�h�A���J���Ă��Ȃ�

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/door.png", &g_pTextureDoor);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_DOOR, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffDoor, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDoor->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = g_Door.pos;
		pVtx[ONE].pos = D3DXVECTOR3(g_Door.pos.x + DOOR_WIDTH,   g_Door.pos.y, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_Door.pos.x,  DOOR_HEIGHT + g_Door.pos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_Door.pos.x + DOOR_WIDTH,   DOOR_HEIGHT + g_Door.pos.y, ORIGIN_POS);
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
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
	}

	//�e�N�X�`�����W
	{
		pVtx[ZERO].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[ONE]. tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[SEC]. tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[SAN]. tex = D3DXVECTOR2(0.25f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffDoor->Unlock();
}

//���̏I������
void UninitDoor(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureDoor != NULL)
	{
		g_pTextureDoor->Release();
		g_pTextureDoor = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffDoor != NULL)
	{
		g_pVtxBuffDoor->Release();
		g_pVtxBuffDoor = NULL;
	}
}

//���̍X�V����
void UpdateDoor(void)
{
	Player *pPlayer = GetPlayer();		//�v���C���[�̏��

	if (g_Door.bOpen == true)
	{
		g_Door.bOpen = true;

		g_Door.nCounterAnim++;

		pPlayer->nPatternAnim = 5;

		if (g_Door.nCounterAnim == 5)
		{
			g_Door.nCounterAnim = ZERO;

			if (g_Door.TexDoor < 0.75f)
			{
				g_Door.TexDoor += 0.25f;
			}

			if (g_Door.TexDoor >= 0.75f && pPlayer->bJump == false && g_Door.nJumpCnt >= 3)
			{
				g_Door.bOpen = false;

				SetFade(MODE_GAME);
			}
		}

		//�v���C���[���R��W�����v������
		if (pPlayer->bJump == false && g_Door.nJumpCnt < 3)
		{
			pPlayer->move.y = JUMP_MOVE;
			pPlayer->bJump = true;
			pPlayer->state = PLAYERSTATE_GOAL;
			g_Door.nJumpCnt++;

		}
	}

	if (pPlayer->state == PLAYERSTATE_APPEAR)
	{
		g_Door.pos = DOOR_POS;
	}

	//�h�A�ɓ���
	EnterDoor();
	
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDoor->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���W���X�V
	moveObject(&g_Door.pos);

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos = g_Door.pos;
		pVtx[ONE].pos = D3DXVECTOR3(g_Door.pos.x + DOOR_WIDTH, g_Door.pos.y, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_Door.pos.x,  DOOR_HEIGHT + g_Door.pos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_Door.pos.x + DOOR_WIDTH,   DOOR_HEIGHT + g_Door.pos.y, ORIGIN_POS);
	}

	//�e�N�X�`�����W
	{
		pVtx[ZERO].tex = D3DXVECTOR2(g_Door.TexDoor, 0.0f);
		pVtx[ONE].tex = D3DXVECTOR2(g_Door.TexDoor + 0.25f, 0.0f);
		pVtx[SEC].tex = D3DXVECTOR2(g_Door.TexDoor, 1.0f);
		pVtx[SAN].tex = D3DXVECTOR2(g_Door.TexDoor + 0.25f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffDoor->Unlock();
}

//���̕`�揈��
void DrawDoor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^ + �擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffDoor, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureDoor);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}

//�h�A�ɓ���
void EnterDoor(void)
{
	Player *pPlayer = GetPlayer();		//�v���C���[�̏��

	if (pPlayer->pos.x >= g_Door.pos.x && pPlayer->pos.x <= g_Door.pos.x + DOOR_WIDTH && pPlayer->bJump == false)
	{
		if ((GetKeyboardTrigger(DIK_W) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP)) && pPlayer->state != PLAYERSTATE_GOAL)
		{
			//�v���C���[�̐ݒ�
			pPlayer->state = PLAYERSTATE_GOAL;
			pPlayer->Rot = PLAYER_ROT_FORWARD;
			pPlayer->nPatternAnim = 5;
			pPlayer->move.y = JUMP_MOVE;
			pPlayer->bJump = true;

			//�h�A�̐ݒ�
			g_Door.TexDoor = F_ZERO;		//�h�A�̃e�N�X�`���͕�����Ԃ���
			g_Door.nCounterAnim = ZERO;		//�A�j���[�V�����J�E���^�[�̓[��
			g_Door.nJumpCnt = ONE;			//�W�����v�񐔂͂P
			g_Door.bOpen = true;			//�h�A���J����
		}
	}
}