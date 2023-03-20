/*==========================================================================================================================

												�e�̔��ˏ���[bullet.cpp]
													Author:���V����

==========================================================================================================================*/
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "effect.h"
#include <math.h>					//��Βl���Q�Ƃ���̂ɕK�v
#include "particle.h"
#include "boss.h"

//�}�N����`
#define MAX_BULLET			(256)		//�e�̍ő吔
#define BULLET_HP			(130)		//�e�̗̑�
#define REFRECT_BULLET_HP	(200)		//���˕Ԃ��ꂽ�e�̗̑�
#define FCORRECTION			(1.0f)		//�z�[�~���O�e�̕␳�W���̐��l
#define FHOMINGMOVE			(5.0f)		//�z�[�~���O�e�̈ړ���
#define FANGLE_CORRECT		(6.28f)		//�p�x�̏C���Ɏg�p
#define DAMAGE				(1)			//�e�̍U����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];							//�e�̏��
float g_fLengthBullet;				//�Ίp���̒���
float g_fAngleBullet;				//�Ίp���̊p�x
bool g_F4;							//���GON/OFF

//�e�̏���������
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &g_pTextureBullet);

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = BULLET_HP;
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].EffectAlphaColor = 0.5f;				//�e�̋O�Ղ̃��l���w�肷��ϐ�
		g_aBullet[nCntBullet].homingtype = HOMING_IMPOSSIBLE;		//�z�[�~���O���\�i�V�̏�Ԃɐݒ�
		g_aBullet[nCntBullet].Reflect = REFLECTBULLET_NO;			//���˕Ԃ���Ă��Ȃ���Ԃɐݒ�
		g_aBullet[nCntBullet].bUse = false;							//�g�p���Ă��Ȃ���Ԃɂ���
	}

	g_F4 = false;					//���GON/OFF�̏�����

	//�Ίp���̒������Z�o����
	g_fLengthBullet = sqrtf(20 * 20 + 20 * 20) * 0.3f;

	//�Ίp���̊p�x���Z�o����
	g_fAngleBullet = atan2f(20, 20);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[0].pos = D3DXVECTOR3(  0.0f,   0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(  0.0f,   0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(  0.0f,   0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(  0.0f,   0.0f, 0.0f);
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
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//�e�̏I������
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{			 
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}			 
				 
	if (g_pVtxBuffBullet != NULL)
	{			 
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//�e�̍X�V����
void UpdateBullet(void)
{
	int nCntBullet;
	int DeadCount = ZERO;

	//�v���C���[�̖��GON/OFF
	if (GetKeyboardTrigger(DIK_F4) == true)
	{
		g_F4 = g_F4 == true ? false : true;
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			//�e�̎�ނɂ�铖���蔻��
			switch (g_aBullet[nCntBullet].type)
			{
			case BULLETTYPE_PLAYER:
				//�G�̎擾
				CollisionEnemy(&g_aBullet[nCntBullet]);
				break;

			case BULLETTYPE_ENEMY:
			case BULLETTYPE_BOSS:
				//�v���C���[�̎擾
				CollisionPlayer(&g_aBullet[nCntBullet]);
				break;
			}

			//�e�̈ʒu�̍X�V
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			/*--------------------------------------------------
								�z�[�~���O����
			----------------------------------------------------*/
			if (g_aBullet[nCntBullet].homingtype == HOMING_POSSIBLE)
				{
					float fRotmove, fRotDest, fRotDiff;		//�z�[�~���O�����ɕK�v�ȕϐ����񂽂�
					float fCorrection = 0.2f;				//�␳�W��
					Player *pPlayer = GetPlayer();			//�v���C���[�̏��
					bool bEnemyUse = false;					//�G�����邩�ǂ����̃t���O

					fRotmove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);		//���݂̈ړ�����(�p�x)

					//�G�̒e�̏ꍇ
					if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
					{
						fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);	//�ڕW�̈ړ�����(�p�x)
					
						bEnemyUse = true;
					}

					//�v���C���[�̒e�̏ꍇ
					else if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
					{
						Enemy *pEnemy = GetEnemy();
						BOSS *pBoss = GetBoss();
						int nCntEnemy;
						float fposXDiff, fposYDiff;								//�G�ƃv���C���[��X�AY���W�̍���
						D3DXVECTOR3 nTargetEnemypos;							//�^�[�Q�b�g�̍��W���L������

						//�v���C���[�Ɉ�ԋ߂��G�����G
						for (nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
						{
							//�G���g�p����Ă��邩
							if (pEnemy->bUse == true)
							{
								fposXDiff = pPlayer->pos.x - pEnemy->pos.x;		//X���W�̍������v�Z
								fposYDiff = pPlayer->pos.y - pEnemy->pos.y;		//Y���W�̍������v�Z

								bEnemyUse = true;	//�G�����邩�ǂ����̔���

								//��������������ΓG�Ƃ̋������߂��Ƃ�������
								if (fabsf(fposXDiff) < fabsf(g_aBullet[nCntBullet].pos.x - nTargetEnemypos.x) && fabsf(fposYDiff) < fabsf(g_aBullet[nCntBullet].pos.y - nTargetEnemypos.y))
								{									
									nTargetEnemypos = pEnemy->pos;		//nTargetEnemypos�ɓG�̍��W����
								}
							}

							//�{�X���g�p����Ă��邩�ǂ���
							else if (pBoss->bDisp == true)
							{
								fposXDiff = pPlayer->pos.x - pBoss->pos.x;		//X���W�̍������v�Z
								fposYDiff = pPlayer->pos.y - pBoss->pos.y;		//Y���W�̍������v�Z

								bEnemyUse = true;	//�G�����邩�ǂ����̔���

								//��������������ΓG�Ƃ̋������߂��Ƃ�������
								if (fabsf(fposXDiff) < fabsf(g_aBullet[nCntBullet].pos.x - nTargetEnemypos.x) && fabsf(fposYDiff) < fabsf(g_aBullet[nCntBullet].pos.y - nTargetEnemypos.y))
								{
									//nTargetEnemypos�ɓG�̍��W����
									nTargetEnemypos = pBoss->pos;
								}

								fCorrection = FCORRECTION;	//�{�X�Ȃ�␳�W�����ő��
							}
						}

						fRotDest = atan2f(nTargetEnemypos.x - g_aBullet[nCntBullet].pos.x, nTargetEnemypos.y - g_aBullet[nCntBullet].pos.y);//�ڕW�̈ړ�����(�p�x)
					}

					//�G������Ȃ�z�[�~���O
					if (bEnemyUse == true && pPlayer->bDisp == true)
					{
						//�ڕW�̈ړ������ւ̍���
						fRotDiff = fRotDest - fRotmove;

						//�����̊p�x���C��
						if (fRotDiff <= -D3DX_PI)
						{//-3.14�ȉ��Ȃ�
							fRotDiff = fRotDiff + FANGLE_CORRECT;
						}
						else if (fRotDiff >= D3DX_PI)
						{//3.14�ȏ�Ȃ�
							fRotDiff = fRotDiff - FANGLE_CORRECT;
						}

						//�ړ�����(�p�x)�̕␳
						fRotmove += fRotDiff * fCorrection;

						//�ړ������̊p�x�̏C��
						if (fRotmove <= -D3DX_PI)
						{//-3.14�ȉ��Ȃ�
							fRotmove = fRotmove + FANGLE_CORRECT;
						}
						else if (fRotmove >= D3DX_PI)
						{//3.14�ȏ�Ȃ�
							fRotmove = fRotmove - FANGLE_CORRECT;
						}

						g_aBullet[nCntBullet].move.x = sinf(fRotmove) * FHOMINGMOVE;
						g_aBullet[nCntBullet].move.y = cosf(fRotmove) * FHOMINGMOVE;
					}
				}

			//���_���W�̐ݒ�
			{
				pVtx[ZERO].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_fAngleBullet)) * g_fLengthBullet;
				pVtx[ZERO].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_fAngleBullet)) * g_fLengthBullet;
				pVtx[ONE].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_fAngleBullet)) * g_fLengthBullet;
				pVtx[ONE].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_fAngleBullet)) * g_fLengthBullet;
				pVtx[SEC].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (F_ZERO - g_fAngleBullet)) * g_fLengthBullet;
				pVtx[SEC].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (F_ZERO - g_fAngleBullet)) * g_fLengthBullet;
				pVtx[SAN].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (F_ZERO + g_fAngleBullet)) * g_fLengthBullet;
				pVtx[SAN].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (F_ZERO + g_fAngleBullet)) * g_fLengthBullet;
			}

			pVtx[ZERO].col = g_aBullet[nCntBullet].col;
			pVtx[ONE].col = g_aBullet[nCntBullet].col;
			pVtx[SEC].col = g_aBullet[nCntBullet].col;
			pVtx[SAN].col = g_aBullet[nCntBullet].col;

			//�e����ʊO�ɏo���ꍇ
			if (g_aBullet[nCntBullet].pos.y >  730.0f || g_aBullet[nCntBullet].pos.y < F_ZERO
			 || g_aBullet[nCntBullet].pos.x > 1290.0f || g_aBullet[nCntBullet].pos.x < F_ZERO)
			{
				g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//�����̃J�E���g�_�E��
			g_aBullet[nCntBullet].nLife--;			

			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(F_TEN, F_TEN, F_TEN, g_aBullet[nCntBullet].EffectAlphaColor), 8.0f, 60);

			//�������s����
			if (g_aBullet[nCntBullet].nLife <= ZERO)
			{
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//�g���Ȃ��Ȃ����e�̏�����
			if (g_aBullet[nCntBullet].bUse == false)
			{
				pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
				pVtx[ONE].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
				pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
				pVtx[SAN].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
			}
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//�e�̕`�揈��
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBullet, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureBullet);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * NUM_VERTEX, SEC);
		}
	}
}

//�e�̐ݒ菈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE type, HOMINGTYPE homing, float Alpha)
{
	int nCntBullet;
	
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			//�e�̏���ݒ�
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].rot.z = ZERO;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].EffectAlphaColor = Alpha;
			g_aBullet[nCntBullet].homingtype = homing;
			g_aBullet[nCntBullet].Reflect = REFLECTBULLET_NO;			
			
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aBullet[nCntBullet].move.x = move.x;
			g_aBullet[nCntBullet].move.y = move.y;
			g_aBullet[nCntBullet].move.z = F_ZERO;

			g_aBullet[nCntBullet].nLife = BULLET_HP;
			g_aBullet[nCntBullet].bUse = true;			//�g�p���Ă����Ԃɂ���
			break;
		}
		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//�G�̓����蔻��
void CollisionEnemy(Bullet *pBullet)
{
	Enemy *pEnemy = GetEnemy();
	BOSS *pBoss = GetBoss();
	int nCntEnemy;
	int nDamage = DAMAGE;		//�U����
	//���˕Ԃ��ꂽ�e�Ȃ�_���[�W�Q�{
	if (pBullet->Reflect == REFLECTBULLET_YES)
	{
		nDamage++;
	}

	for (nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�G���g�p����Ă���
			if (pBullet->pos.x >= pEnemy->pos.x - 30.0f &&
				pBullet->pos.x <= pEnemy->pos.x + 30.0f &&
				pBullet->pos.y >= pEnemy->pos.y - 30.0f &&
				pBullet->pos.y <= pEnemy->pos.y + 30.0f)
			{
				HitEnemy(nCntEnemy, nDamage, pEnemy->nLifeMax, pBullet->Reflect);
				
				if (pEnemy->nLife > ONE)
				{
					SetParticle(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f), PARTICLETYPE_STAY);
				}

				PlaySound(SOUND_LABEL_SE_HIT);
				pBullet->bUse = false;
			}
		}
	}

	if (pBoss->state == BOSSSTATE_SPIN && pBullet->type == BULLETTYPE_PLAYER)
	{
		//�{�X�̒e�Ƃ̓����蔻��
		if (pBullet->pos.x >= pBoss->pos.x - 50.0f &&
			pBullet->pos.x <= pBoss->pos.x + 50.0f &&
			pBullet->pos.y >= pBoss->pos.y - 20.0f &&
			pBullet->pos.y <= pBoss->pos.y + 20.0f)
		{
			//���ł��������֒��˕Ԃ�
			pBullet->move *= -1.0f;

			//�e�̎�������
			pBullet->nLife = REFRECT_BULLET_HP;

			//�e�̏���h��ւ�
			pBullet->type = BULLETTYPE_BOSS;
			pBullet->homingtype = HOMING_POSSIBLE;
			pBullet->Reflect = REFLECTBULLET_YES;
			pBullet->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

			PlaySound(SOUND_LABEL_SE_REFLECT);	//���˕Ԃ�����炷
		}
	}

	//�{�X���g���Ă��� & �����蔻��
	else if (pBullet->pos.x >= pBoss->pos.x - 50.0f &&
			 pBullet->pos.x <= pBoss->pos.x + 50.0f &&
			 pBullet->pos.y >= pBoss->pos.y - 20.0f &&
			 pBullet->pos.y <= pBoss->pos.y + 20.0f && pBoss->bDisp == true && pBoss->state == BOSSSTATE_NORMAL)
	{
		HitBoss(nDamage, pBullet->Reflect);
		PlaySound(SOUND_LABEL_SE_HIT);

		if (pBoss->nLife > ONE)
		{
			SetParticle(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f), PARTICLETYPE_STAY);
		}

		pBullet->bUse = false;
	}
}

//�v���C���[�̓����蔻��
void CollisionPlayer(Bullet *pBullet)
{
	Player *pPlayer = GetPlayer();

	//�X�s����
	if (pPlayer->state == PLAYERSTATE_SPIN && pBullet->type != BULLETTYPE_PLAYER)
	{
		//�G�̒e�Ƃ̓����蔻��
		if (pBullet->pos.x >= pPlayer->pos.x - 30.0f &&
			pBullet->pos.x <= pPlayer->pos.x + 30.0f &&
			pBullet->pos.y >= pPlayer->pos.y - 30.0f &&
			pBullet->pos.y <= pPlayer->pos.y + 30.0f)
		{
			if (g_F4 == false)
			{
				int nDamage = 1;

				pPlayer->LimitSpin--;	//���˕Ԃ��񐔂�����

				if (pBullet->Reflect == REFLECTBULLET_YES)
				{
					nDamage = nDamage + nDamage;
					pPlayer->LimitSpin--;	//���˕Ԃ��񐔂�����
				}

				//����𒴂����瓖���蔻�蕜��
				if (pPlayer->LimitSpin <= ZERO)
				{
					//��]�l���O��
					pPlayer->rot.z = F_ZERO;

					HitPlayer(nDamage);
					PlaySound(SOUND_LABEL_SE_HIT);	//�q�b�g����炷
				}

				//���˕Ԃ�
				else
				{
					//���ł��������֒��˕Ԃ�
					pBullet->move *= -1.0f;

					//�e�̎�������
					pBullet->nLife = REFRECT_BULLET_HP;

					//�e�̏���h��ւ�
					pBullet->type = BULLETTYPE_PLAYER;
					pBullet->homingtype = HOMING_IMPOSSIBLE;
					pBullet->Reflect = REFLECTBULLET_YES;
					pBullet->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

					PlaySound(SOUND_LABEL_SE_REFLECT);	//���˕Ԃ�����炷
				}
			}
		}
	}

	//�����蔻��
	else if (pPlayer->state == PLAYERSTATE_NORMAL && pPlayer->bDisp == true)
	{
		//�G�̒e�Ƃ̓����蔻��
		if (pBullet->pos.x >= pPlayer->pos.x - 30.0f &&
			pBullet->pos.x <= pPlayer->pos.x + 30.0f &&
			pBullet->pos.y >= pPlayer->pos.y - 30.0f &&
			pBullet->pos.y <= pPlayer->pos.y + 30.0f)
		{
			//�����̐ݒ�
			if (g_F4 == false)
			{
				HitPlayer(1);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
			pBullet->bUse = false;
		}
	}
}