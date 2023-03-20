/*==========================================================================================================================

												�v���C���[�̏���[boss.cpp]
													Author:���V����

==========================================================================================================================*/
#include "boss.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "game.h"
#include "explosion.h"
#include "enemy.h"
#include "particle.h"
#include "score.h"

//�}�N����`
#define BOSS_HP			(90)		//�̗�
#define DESTROY_SCORE	(80000)		//�{�X���j���̃X�R�A
#define MOVE_SPIN		(-60.0f)	//�X�s���̉�]�l
#define ADD_SPIN		(0.5f)		//��]�̉��Z�l
#define BULLET_SP_BOSS	(10.0f)		//�{�X�̒e�̑���
#define SPIN_INTERVAL	(300)		//�X�s���̃C���^�[�o��
#define NUM_PARTICLE	(50)		//�{�X���j���̃p�[�e�B�N��������
#define BOSS_TEX		"data/TEXTURE/Boss.png"		//�{�X�̉摜��

#define NORMAL_BONUS	(1)			//�s�ʂɓ|�������̃{�[�i�X�X�R�A
#define SPECIAL_BONUS	(3)			//���t���N�g�e�œ|�������̃{�[�i�X�X�R�A�{��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureBoss;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;	//���_�o�b�t�@�ւ̃|�C���^
BOSS g_Boss;									//�{�X�̏��

//�{�X�̏���������
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�{�X���̏�����
	{
		g_Boss.pos = D3DXVECTOR3(600.0f, -100.0f, 0.0f);//�ʒu������������
		g_Boss.move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);	//�ړ��ʂ�����������
		g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����������������(Z�l���g�p)
		g_Boss.BulletType = BULLETTYPE_BOSS;			//�e�̎�ނ�ݒ�
		g_Boss.homingtype = HOMING_IMPOSSIBLE;			//�z�[�~���O���\�̏������i�ŏ��̓i�V)
		g_Boss.SpinInterval = SPIN_INTERVAL;			//�X�s���C���^�[�o���̏�����
		g_Boss.BossAttackCnt = 30;						//�{�X�̍U���Ԋu�̏�����
		g_Boss.nLife = BOSS_HP;							//�̗͏�����
		g_Boss.state = BOSSSTATE_APPEAR;				//��ԏ�����
		g_Boss.nCounterState = 0;						//��ԊǗ��J�E���^������
		g_Boss.bDisp = false;							//�\��ON/OFF(�ŏ���OFF)
	}

	//�Ίp���̒������Z�o����
	g_Boss.fLength = sqrtf(60 * 60 + 60 * 60) * 1.0f;

	//�Ίp���̊p�x���Z�o����
	g_Boss.fAngle = atan2f(60, 60);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BOSS_TEX, &g_pTextureBoss);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBoss, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void* *)&pVtx, 0);

	//���_���W�̐ݒ�
	{
		pVtx[0].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (-D3DX_PI + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[0].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (-D3DX_PI + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[0].pos.z = 0.0f;
		
		pVtx[1].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z +  (D3DX_PI - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[1].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z +  (D3DX_PI - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[1].pos.z = 0.0f;
		
		pVtx[2].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (0 - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[2].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (0 - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[2].pos.z = 0.0f;
		
		pVtx[3].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (0 + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[3].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (0 + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[3].pos.z = 0.0f;
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//�e�N�X�`���̍��W
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBoss->Unlock();
}

//�{�X�̏I������
void UninitBoss(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBoss != NULL)
	{
		g_pTextureBoss->Release();
		g_pTextureBoss = NULL;
	}

	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//�{�X�̍X�V����
void UpdateBoss(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void* *)&pVtx, 0);

	Player *pPlayer = GetPlayer();

	//�{�X�̏�ԏ�񕪊�
	switch (g_Boss.state)
	{
	case BOSSSTATE_APPEAR:
		//���_�J���[�̐ݒ�
		g_Boss.pos.y += 1.0f;

		if (g_Boss.pos.y >= 120.0f)
		{
			g_Boss.state = BOSSSTATE_NORMAL;
		}
		break;

	case BOSSSTATE_NORMAL:
		//�{�X�ƃv���C���[�̓����蔻��
		if (pPlayer->pos.x - ENEMY_VERTEX >= g_Boss.pos.x - 30.0f &&
			pPlayer->pos.x + ENEMY_VERTEX <= g_Boss.pos.x + 30.0f &&
			pPlayer->pos.y - ENEMY_VERTEX >= g_Boss.pos.y - 30.0f &&
			pPlayer->pos.y + ENEMY_VERTEX <= g_Boss.pos.y + 30.0f)
		{
			HitPlayer(1);
		}

		//�e����
		if (g_Boss.nLife > 0)
		{
			g_Boss.BossAttackCnt--;

			if (g_Boss.BossAttackCnt <= 0)
			{
				SetBullet(D3DXVECTOR3(g_Boss.pos.x - 40.0f, g_Boss.pos.y + 30.0f, g_Boss.pos.z), D3DXVECTOR3(sinf(g_Boss.rot.z + (D3DX_PI * 0.0f)) * BULLET_SP_BOSS, cosf(g_Boss.rot.z + (D3DX_PI * 0.0f)) * BULLET_SP_BOSS, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_Boss.BulletType, g_Boss.homingtype, BULLETEFFECT_PLAYER);
				SetBullet(D3DXVECTOR3(g_Boss.pos.x + 40.0f, g_Boss.pos.y + 30.0f, g_Boss.pos.z), D3DXVECTOR3(sinf(g_Boss.rot.z + (D3DX_PI * 0.0f)) * BULLET_SP_BOSS, cosf(g_Boss.rot.z + (D3DX_PI * 0.0f)) * BULLET_SP_BOSS, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_Boss.BulletType, g_Boss.homingtype, BULLETEFFECT_PLAYER);
				
				g_Boss.BossAttackCnt = 30;
			}
		}

		//�C���^�[�o������
		g_Boss.SpinInterval--;

		//�X�s���J�n
		if (g_Boss.SpinInterval <= 0)
		{
			//��Ԃ��X�s����Ԃ�(�e���܂��U�炷)
			g_Boss.state = BOSSSTATE_SPIN;
			g_Boss.SpinInterval = SPIN_INTERVAL;
			g_Boss.BossAttackCnt = 3;

			//�������X�V(Z�l����]������)
			g_Boss.rot.z = MOVE_SPIN;
		}
		break;

	case BOSSSTATE_SPIN:
		if (g_Boss.rot.z < 0.0f)
		{
			g_Boss.rot.z += ADD_SPIN;
			g_Boss.BossAttackCnt--;

			if (g_Boss.BossAttackCnt == 0)
			{
				SetBullet(D3DXVECTOR3(g_Boss.pos.x - 15.0f, g_Boss.pos.y + 30.0f, g_Boss.pos.z), D3DXVECTOR3(sinf(g_Boss.rot.z + (D3DX_PI * 1.0f)) * BULLET_SP_BOSS, cosf(g_Boss.rot.z + (D3DX_PI * 1.0f)) * BULLET_SP_BOSS, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), g_Boss.BulletType, HOMING_IMPOSSIBLE, BULLETEFFECT_PLAYER);
			
				g_Boss.BossAttackCnt = 1;
			}

			if (g_Boss.rot.z >= 0.0f)
			{
				//��Ԃ𕁒ʂ�
				g_Boss.state = BOSSSTATE_NORMAL;
				g_Boss.rot.z = 0.0f;
				g_Boss.SpinInterval = SPIN_INTERVAL;
				g_Boss.BossAttackCnt = 60;
			}
		}
		break;

	case BOSSSTATE_DAMAGE:
		g_Boss.nCounterState--;

		if (g_Boss.nCounterState <= 0)
		{
			g_Boss.state = BOSSSTATE_NORMAL;

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case BOSSSTATE_DEATH:
		g_Boss.nLife = 0;
		g_Boss.bDisp = false;
	}

	if (g_Boss.state != BOSSSTATE_APPEAR)
	{
		//�ʒu���X�V
		g_Boss.pos.x += g_Boss.move.x;

		if (g_Boss.pos.x >= 1240.0f || g_Boss.pos.x <= 50.0f)
		{
			g_Boss.move *= -1.0f;
		}
	}

	//���_���W�̐ݒ�
	{
		pVtx[0].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (-D3DX_PI + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[0].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (-D3DX_PI + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[1].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (D3DX_PI - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[1].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (D3DX_PI - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[2].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (0 - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[2].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (0 - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[3].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (0 + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[3].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (0 + g_Boss.fAngle)) * g_Boss.fLength;
	}

	//��ʔ͈͊O����
	{
		//���͈̔͊O����
		{
			//��ʊO�ɏo����A���Α��ɖ߂�
			if (g_Boss.pos.x > 1240.0f)
			{//�E�[
				g_Boss.pos.x = 1240.0f;
			}

			if (g_Boss.pos.x < 40.0f)
			{//���[
				g_Boss.pos.x = 40.0f;
			}
		}

		//�c�͈̔͊O����
		{
			//��ʊO�ɏo����A���Α��ɖ߂�
			if (g_Boss.pos.y > 680.0f)
			{//���
				g_Boss.pos.y = 680.0f;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBoss->Unlock();
}

//�{�X�̕`�揈��
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Boss.bDisp == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBoss);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, SEC);
	}
}

//�{�X�̐ݒ菈��
void SetBoss(void)
{
	g_Boss.pos = D3DXVECTOR3(600.0f, -100.0f, 0.0f);//�ʒu������������
	g_Boss.move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);	//�ړ��ʂ�����������
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����������������(Z�l���g�p)
	g_Boss.BulletType = BULLETTYPE_BOSS;			//�e�̎�ނ�ݒ�
	g_Boss.homingtype = HOMING_IMPOSSIBLE;			//�z�[�~���O���\�̏������i�ŏ��̓i�V)
	g_Boss.SpinInterval = SPIN_INTERVAL;			//�X�s���C���^�[�o���̏�����
	g_Boss.BossAttackCnt = 30;						//�{�X�̍U���Ԋu�̏�����
	g_Boss.nLife = BOSS_HP;							//�̗͏�����
	g_Boss.state = BOSSSTATE_APPEAR;				//��ԏ�����
	g_Boss.nCounterState = 0;						//��ԊǗ��J�E���^������
	g_Boss.bDisp = true;
}

//�{�X�̎擾
BOSS *GetBoss(void)
{
	return &g_Boss;
}

//�{�X�̃q�b�g����
void HitBoss(int nDamage, REFLECTBULLET type)
{
	g_Boss.nLife -= nDamage;

	if (g_Boss.nLife <= 0)
	{
		SetExplosion(g_Boss.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//�Q�[����Ԃ̐ݒ�
		SetGameState(GAMESTATE_END, 60);
		g_Boss.state = BOSSSTATE_DEATH;

		//���t���N�g�{�[�i�X
		int BonusScore = NORMAL_BONUS;

		if (type == REFLECTBULLET_YES)
		{
			BonusScore = SPECIAL_BONUS;
		}

		AddScore(DESTROY_SCORE * BonusScore);
		g_Boss.bDisp = false;

		for (int nCntBoss = 0; nCntBoss < NUM_PARTICLE; nCntBoss++)
		{
			D3DXVECTOR3 fRandom = D3DXVECTOR3((float)(rand() % 20) - 5.0f, (float)(rand() % 20) - 5.0f, 0.0f); //�p�[�e�B�N��STAY�̔����ꏊ

			SetParticle(g_Boss.pos, D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);	//�p�[�e�B�N���𔭐�
			SetParticle(g_Boss.pos + fRandom, D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_STAY);	//�p�[�e�B�N���𔭐�
		}
	}

	else
	{
		//�|�C���^��ݒ�
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffBoss->Lock(0, 0, (void* *)&pVtx, 0);

		g_Boss.state = BOSSSTATE_DAMAGE;
		g_Boss.nCounterState = 2;

		//���_�J���[�̐ݒ�
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		}

		//���_�o�b�t�@�����b�N����
		g_pVtxBuffBoss->Unlock();
	}
}