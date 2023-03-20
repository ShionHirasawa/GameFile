/*==========================================================================================================================

												�v���C���[�̏���[player.cpp]
													Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "game.h"
#include "explosion.h"
#include "enemy.h"
#include "beam.h"
#include "particle.h"

//�}�N����`
#define PLAYER_POS			D3DXVECTOR3(640.0f, 450.0f, F_ZERO)
#define PLAYER_MOVE			D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO)
#define PLAYER_ROT			D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO)
#define PLAYER_TEX			"data/TEXTURE/���@.png"
#define PLAYER_LENGTH		(60.0f)		//�v���C���[�̑傫��
#define COLLISION_PLAYER	(30.0f)		//�v���C���[�̓����蔻��
#define COLLISION_SIZE		(1.31f)		//�����蔻��̔{��
#define PLAYER_HP			(5)			//�̗�
#define DAMAGE_TIME			(60)		//�_���[�W��Ԃ̎���
#define MOVE_SPIN			(-18.0f)	//�X�s���̉�]�l
#define MOVE_SPEED			(1.0f)		//�v���C���[�̈ړ���
#define ADD_SPIN			(0.2f)		//��]�̉��Z�l
#define LIMIT_SPIN			(8)			//���˕Ԃ����
#define SPIN_INTERVAL		(100)		//�X�s���̃C���^�[�o��
#define APPEARCNT			(120)		//�o�����̓_�Ŏ���

#define MOVE_LIMIT_R		(1240.0f)	//�v���C���[�̉�ʒ[�ړ����E�@�E
#define MOVE_LIMIT_L		(40.0f)		//�v���C���[�̉�ʒ[�ړ����E�@��
#define MOVE_LIMIT_CEILING	(60.0f)		//�v���C���[�̉�ʒ[�ړ����E�@��
#define MOVE_LIMIT_FLOOR	(680.0f)	//�v���C���[�̉�ʒ[�ړ����E�@��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTexturePlayer;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Player g_player;

//�v���C���[�̏���������
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�v���C���[���̏�����
	{
		g_player.pos = PLAYER_POS;								//�ʒu������������
		g_player.move = PLAYER_MOVE;							//�ړ��ʂ�����������
		g_player.rot = PLAYER_ROT;								//����������������(Z�l���g�p)
		g_player.myBullet = BULLETTYPE_PLAYER;					//�e�̎�ނ�ݒ�
		g_player.homingtype = HOMING_IMPOSSIBLE;				//�z�[�~���O���\�̏������i�ŏ��̓i�V)
		g_player.LimitSpin = LIMIT_SPIN;						//���˕Ԃ��̏��
		g_player.SpinInterval = ZERO;							//�X�s���C���^�[�o���̏�����
		g_player.nLife = PLAYER_HP;								//�̗͏�����
		g_player.state = PLAYERSTATE_APPEAR;					//��ԏ�����
		g_player.PlayerSwitch = ONE;							//�_�ŕ\���J�E���^������
		g_player.AppearCnt = ZERO;								//�_�ŕ\�����ԃJ�E���^������
		g_player.nCounterState = ZERO;							//��ԊǗ��J�E���^������
		g_player.bDisp = true;									//�\��ON/OFF(�ŏ���ON)
		g_player.fLength = sqrtf(PLAYER_LENGTH * PLAYER_LENGTH + PLAYER_LENGTH * PLAYER_LENGTH) * F_FIVE;	//�Ίp���̒������Z�o����
		g_player.fAngle = atan2f(PLAYER_LENGTH, PLAYER_LENGTH);	//�Ίp���̊p�x���Z�o����
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &g_pTexturePlayer);
		
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos.x = g_player.pos.x + sinf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ZERO].pos.y = g_player.pos.y + cosf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ZERO].pos.z = ORIGIN_POS;
		pVtx[ONE].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.z = ORIGIN_POS;
		pVtx[SEC].pos.x = g_player.pos.x + sinf(g_player.rot.z + (ORIGIN_POS - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.y = g_player.pos.y + cosf(g_player.rot.z + (ORIGIN_POS - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.z = ORIGIN_POS;
		pVtx[SAN].pos.x = g_player.pos.x + sinf(g_player.rot.z + (ORIGIN_POS + g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.y = g_player.pos.y + cosf(g_player.rot.z + (ORIGIN_POS + g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.z = ORIGIN_POS;
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
		pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		pVtx[ONE].tex = D3DXVECTOR2(MAX_POS, ORIGIN_POS);
		pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, MAX_POS);
		pVtx[SAN].tex = D3DXVECTOR2(MAX_POS   , MAX_POS);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//�v���C���[�̏I������
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//�v���C���[�̍X�V����
void UpdatePlayer(void)
{
	if (GetKeyboardPress(DIK_D) == true)
	{//D�L�[�������ꂽ
		if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ
			g_player.move.x += sinf(D3DX_PI * UP_RIGHT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * UP_RIGHT) * MOVE_SPEED;
		}

		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ
		 //�E������
			g_player.move.x += sinf(D3DX_PI * DOWN_RIGHT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * DOWN_RIGHT) * MOVE_SPEED;
		}

		else
		{
			g_player.move.x += sinf(D3DX_PI * RIGHT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * RIGHT) * MOVE_SPEED;
		}
	}

	else if (GetKeyboardPress(DIK_A) == true)
	{//A�L�[�������ꂽ
		if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ
		 //�������
			g_player.move.x += sinf(D3DX_PI * UP_LEFT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * UP_LEFT) * MOVE_SPEED;
		}

		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ
		 //��������
			g_player.move.x += sinf(D3DX_PI * DOWN_LEFT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * DOWN_LEFT) * MOVE_SPEED;
		}

		else
		{
			g_player.move.x += sinf(D3DX_PI * LEFT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * LEFT) * MOVE_SPEED;
		}
	}

	else if (GetKeyboardPress(DIK_W) == true)
	{//W�L�[�������ꂽ
		g_player.move.y += cosf(-D3DX_PI * UP) * MOVE_SPEED;
	}

	else if (GetKeyboardPress(DIK_S) == true)
	{//S�L�[�������ꂽ
		g_player.move.y += cosf(D3DX_PI * DOWN) * MOVE_SPEED;
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�ʒu���X�V
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	//�ړ��ʂ��X�V(����������)
	g_player.move.x += (F_ZERO - g_player.move.x) * F_ONE;
	g_player.move.y += (F_ZERO - g_player.move.y) * F_ONE;
	Enemy *pEnemy = GetEnemy();

	//�v���C���[�̏�ԏ�񕪊�
	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_player.AppearCnt++;

		//�v���C���[��_�ł�����
		if (g_player.AppearCnt % FOUR == ZERO)
		{
			g_player.PlayerSwitch = g_player.PlayerSwitch ^ ONE;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX * g_player.PlayerSwitch);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX * g_player.PlayerSwitch);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX * g_player.PlayerSwitch);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX * g_player.PlayerSwitch);
		}

		if (g_player.AppearCnt >= APPEARCNT)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
		break;

	case PLAYERSTATE_NORMAL:
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			if (g_player.nLife > 0)
			{
				SetBullet(D3DXVECTOR3(g_player.pos.x - 5.0f, g_player.pos.y - 20.0f, g_player.pos.z), D3DXVECTOR3(sinf(g_player.rot.z + (D3DX_PI * 1.0f)) * 6.0f, cosf(g_player.rot.z + (D3DX_PI * 1.0f)) * 6.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), g_player.myBullet, g_player.homingtype, BULLETEFFECT_PLAYER);
				SetBullet(D3DXVECTOR3(g_player.pos.x + 5.0f, g_player.pos.y - 20.0f, g_player.pos.z), D3DXVECTOR3(sinf(g_player.rot.z + (D3DX_PI * 1.0f)) * 6.0f, cosf(g_player.rot.z + (D3DX_PI * 1.0f)) * 6.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), g_player.myBullet, g_player.homingtype, BULLETEFFECT_PLAYER);
			}
		}

		if (g_player.SpinInterval > ZERO && g_player.state == PLAYERSTATE_NORMAL)
		{
			g_player.SpinInterval--;

			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, ZERO, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, ZERO, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, ZERO, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, ZERO, COLOR_MAX);
		}
		else
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		if (GetKeyboardPress(DIK_SPACE) == true && g_player.SpinInterval <= ZERO)
		{
			//��Ԃ��X�s����Ԃ�(�U���s�\)
			g_player.state = PLAYERSTATE_SPIN;

			//���˕Ԃ������ݒ�
			g_player.LimitSpin = LIMIT_SPIN;

			//�������X�V(Z�l����]������)
			g_player.rot.z = MOVE_SPIN;
		}

		//�G�ƃv���C���[�̓����蔻��
		for (int nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				//�G�ƃv���C���[�̋����Ƃ��ꂼ��̔��a�̒����̍��v
				float pos = sqrtf((g_player.pos.x - pEnemy->pos.x) * (g_player.pos.x - pEnemy->pos.x) + (g_player.pos.y - pEnemy->pos.y) * (g_player.pos.y - pEnemy->pos.y)) * COLLISION_SIZE,
					len = pEnemy->fLength + g_player.fLength;

				if (len >= pos)
				{
					HitPlayer(ONE);
				}
			}
		}
		break;

	case PLAYERSTATE_SPIN:
		if (g_player.rot.z <= F_ZERO)
		{
			g_player.rot.z += ADD_SPIN;

			if (g_player.rot.z > F_ZERO)
			{
				//��Ԃ𕁒ʂ�
				g_player.state = PLAYERSTATE_NORMAL;
				g_player.rot.z = F_ZERO;
				g_player.SpinInterval = SPIN_INTERVAL;
			}
		}
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;

		//�Ԃ��_�ł�����
		if (g_player.nCounterState % FIVE == ZERO)
		{
			//��ʂɈڂ����̔���
			g_player.bDisp = g_player.bDisp ? false : true;
		}

		//�_���[�W��Ԃ̎��Ԃ��[���ɂȂ���
		if (g_player.nCounterState <= ZERO)
		{
			g_player.state = PLAYERSTATE_NORMAL;		//�v���C���[�̏�Ԃ��m�[�}���Ɏ���
			g_player.bDisp = true;						//��������ʂɈڂ�

			//�F�����ɖ߂�
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		break;

	case PLAYERSTATE_DEATH:
		break;
	}

	//���_���W�̐ݒ�
	{
		pVtx[ZERO].pos.x = g_player.pos.x + sinf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ZERO].pos.y = g_player.pos.y + cosf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.x = g_player.pos.x + sinf(g_player.rot.z + (F_ZERO - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.y = g_player.pos.y + cosf(g_player.rot.z + (F_ZERO - g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.x = g_player.pos.x + sinf(g_player.rot.z + (F_ZERO + g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.y = g_player.pos.y + cosf(g_player.rot.z + (F_ZERO + g_player.fAngle)) * g_player.fLength;
	}

	//��ʒ[����
	{
		//���[����
		{
			//��ʒ[�Ȃ�A�����߂�
			if (g_player.pos.x > MOVE_LIMIT_R)
			{//�E�[
				g_player.pos.x = MOVE_LIMIT_R;
			}

			if (g_player.pos.x < MOVE_LIMIT_L)
			{//���[
				g_player.pos.x = MOVE_LIMIT_L;
			}
		}

		//�c�[����
		{
			//��ʒ[�Ȃ�A�����߂�
			if (g_player.pos.y > MOVE_LIMIT_FLOOR)
			{//���
				g_player.pos.y = MOVE_LIMIT_FLOOR;
			}

			if (g_player.pos.y < MOVE_LIMIT_CEILING)
			{//�V��
				g_player.pos.y = MOVE_LIMIT_CEILING;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//�v���C���[�̕`�揈��
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffPlayer, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.bDisp == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTexturePlayer);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
	}
}

//�v���C���[�̎擾
Player *GetPlayer(void)
{
	return &g_player;
}

//�v���C���[�̃q�b�g����
void HitPlayer(int nDamage)
{
	g_player.nLife -= nDamage;	

	if (g_player.nLife <= ZERO)
	{
		g_player.nLife = ZERO;
		SetExplosion(g_player.pos, D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN));
		//�Q�[����Ԃ̐ݒ�
		SetGameState(GAMESTATE_END, END_FADE);
		g_player.state = PLAYERSTATE_DEATH;
		g_player.bDisp = false;
	}

	else
	{
		//�|�C���^��ݒ�
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = DAMAGE_TIME;

		//���_�J���[�̐ݒ�
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, ZERO, ZERO, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, ZERO, ZERO, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, ZERO, ZERO, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, ZERO, ZERO, COLOR_MAX);
		}

		//���_�o�b�t�@�����b�N����
		g_pVtxBuffPlayer->Unlock();
	}
}