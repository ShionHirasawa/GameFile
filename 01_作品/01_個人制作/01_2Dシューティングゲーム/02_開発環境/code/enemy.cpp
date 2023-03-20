/*==========================================================================================================================

												�G�o���@���@�����蔻�菈��[bullet.cpp]
														   Author:���V����

==========================================================================================================================*/
#include "enemy.h"
#include "bullet.h"
#include "beam.h"
#include "explosion.h"
#include "score.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "particle.h"
#include "input.h"
#include "boss.h"
#include <stdlib.h>
#include <string.h>

//�}�N����`
#define ENEMY_TYPE		(4)								//�G�̎�ސݒ�
#define ENEMY_HP		(4)								//�G�̗̑͂̐ݒ�
#define SCORE_DEFO		(1000)							//�X�R�A�̊�l
#define HOMING			(20)							//�G���z�[�~���O�e��L����m���v�Z�Ɏg�p
#define ENEMY_SIZE		(30.0f)							//�G�̃T�C�Y
#define ENEMY_LENGTH	ENEMY_SIZE * ENEMY_SIZE			//�G�̑Ίp���̒����v�Z�Ɏg�p
#define STANDARD_POS	(100.0f)						//�G�����E�ړ�����Y���W
#define ST_POS_DIS		(10.0f)							//�X�^���_�[�h�|�W�V�����̊Ԋu
#define OUTSIDE_FALL	(1.0f)							//��ʊO�ō~�����鑬�x
#define FALL_APPEAR		(1.6f)							//�G����ʓ��ɋ��Ĕ��ˉ\��ԂɂȂ�܂ō~�����鑬�x
#define FALL_ENEMY		(0.6f)							//�G����ʓ��ō~�����鑬�x
#define RETURN_ENEMY	(8.0f)							//�G�����ɐ܂�Ԃ����x
#define TURN_LEFT_SIDE	(80.0f)							//�E�ɐ܂�Ԃ�
#define TURN_RIGHT_SIDE	(1200.0f)						//���ɐ܂�Ԃ�

//�e�̑����̃}�N��
#define ENEMY_BULLET_SPEED_NORMAL	(3.0f)				//���ʁE�O�A�e��łG�̒e�̑���
#define ENEMY_BULLET_SPEED_SPEED	(5.0f)				//�{���e��łG�̒e�̑���
#define ENEMY_BULLET_SPEED_BEAM		(8.0f)				//�r�[���e��łG�̒e�̑���

#define MOJI			(300)							//�ǂݎ�镶���̏����
#define ENEMY_INFO		"data/�G�̔z�u���.csv"			//�G�̏�񂪏����ꂽ�t�@�C����
#define ENEMY_TEX		"data/TEXTURE/enemy000.png"		//�G�̉摜

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureEnemy = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Enemy g_aEnemy[MAX_ENEMY];							//�G�̏��
DWORD g_StartTime;									//�G�o���̎���
int g_nNumEnemy;									//�G�̑���

//�G�̏���������
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�����_���֐��̃V�[�h���������ݒ�
	srand((unsigned int)time(NULL));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEX, &g_pTextureEnemy);

	//���������p�̕ϐ����񂽂�
	float Stpos = 0.0f;
	FILE *pFile;
	char aData[MOJI], *f;

	pFile = fopen(ENEMY_INFO, "r");

	//�G�̏��̏�����
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);

		for (int nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);

			f = strtok(&aData[ZERO], ",");		//�ݒ蕶��

			Stpos = (float)(rand() % HOMING) * ST_POS_DIS + STANDARD_POS;		//�X�^���_�[�h�|�W�V�����̏�����
			g_aEnemy[nCntEnemy].Standardpos = Stpos;	//�ʒu�̏�����

			g_aEnemy[nCntEnemy].pos.x = atof(strtok(NULL, ","));		//�o���ʒu�̏�����
			g_aEnemy[nCntEnemy].pos.y = atof(strtok(NULL, ","));		//�o���ʒu�̏�����
			g_aEnemy[nCntEnemy].pos.z = F_ZERO;							//�o���ʒu�̏�����
			g_aEnemy[nCntEnemy].nLife = atoi(strtok(NULL, ","));		//���C�t������
			g_aEnemy[nCntEnemy].nLifeMax = g_aEnemy[nCntEnemy].nLife;	//���C�t������
			g_aEnemy[nCntEnemy].nType = atoi(strtok(NULL, ","));		//��ޏ�����
			g_aEnemy[nCntEnemy].move.x = atof(strtok(NULL, ","));		//�G�̈ړ��ʂ̏�����
			g_aEnemy[nCntEnemy].move.y = atof(strtok(NULL, ","));		//�G�̈ړ��ʂ̏�����
			g_aEnemy[nCntEnemy].enBullet = BULLETTYPE_ENEMY;			//�e�̎�ނ̏�����
			g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;				//�U���^�[���̏�����
			g_aEnemy[nCntEnemy].nCounterState = ZERO;					//��ԊǗ��J�E���^�[�̏�����
			g_aEnemy[nCntEnemy].Return = F_ZERO;						//�ړ��ϐ��̌��Z�l�̏�����
			g_aEnemy[nCntEnemy].bUse = false;							//�g�p���Ă��Ȃ���Ԃɂ���
			g_aEnemy[nCntEnemy].fLength = sqrtf(ENEMY_LENGTH + ENEMY_LENGTH) * F_SEV;	//�Ίp���̒���
			g_aEnemy[nCntEnemy].fAngle = atan2f(ENEMY_SIZE, ENEMY_SIZE);				//�Ίp���̌v�Z

			//�z�[�~���O���\�A���@�r�[���e��łG�ȊO�ɕt�^
			if (Stpos == STANDARD_POS && g_aEnemy[nCntEnemy].nType != SAN)
			{
				g_aEnemy[nCntEnemy].homingtype = HOMING_POSSIBLE;
			}
			//�z�[�~���O���\�i�V
			else
			{
				g_aEnemy[nCntEnemy].homingtype = HOMING_IMPOSSIBLE;
			}
		}

		fclose(pFile);
	}

	g_nNumEnemy = ZERO;				//�G�̑������N���A

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(-D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[ZERO].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(-D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[ZERO].pos.z = ORIGIN_POS;
			pVtx[ONE].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[ONE].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[ONE].pos.z = ORIGIN_POS;
			pVtx[SEC].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(ORIGIN_POS - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[SEC].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(ORIGIN_POS - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[SEC].pos.z = ORIGIN_POS;
			pVtx[SAN].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(ORIGIN_POS + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[SAN].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(ORIGIN_POS + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
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
			pVtx[ZERO].col = D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS);
			pVtx[ONE].col = D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS);
			pVtx[SEC].col = D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS);
			pVtx[SAN].col = D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(MAX_POS, ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, MAX_POS);
			pVtx[SAN].tex = D3DXVECTOR2(MAX_POS   , MAX_POS);
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//�Q�[���J�n���Ԃ̎���
	g_StartTime = timeGetTime();

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//�G�̏I������
void UninitEnemy(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//�G�̍X�V����
void UpdateEnemy(void)
{
	int nCntEnemy;
	int	BulletSound = ZERO, BeamSound = ZERO;		//�e�ƃr�[�����̃T�E���h�J�E���^
	float EnemyBulletAngle;
	Player *pPlayer = GetPlayer();
	DWORD SetEnemyTime = timeGetTime();		//�G�z�u�̎���

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//�G���~��
			if (g_aEnemy[nCntEnemy].pos.y < g_aEnemy[nCntEnemy].Standardpos)
			{
				if (g_aEnemy[nCntEnemy].pos.y < ORIGIN_POS)
				{
					g_aEnemy[nCntEnemy].pos.y += OUTSIDE_FALL;
				}
				else
				{
					g_aEnemy[nCntEnemy].pos.y += FALL_APPEAR;
				}
			}

			//���E�ړ���e�̔��˂Ȃ�
			else
			{
				//�G�̒e���ˏ���
				if (pPlayer->state != PLAYERSTATE_APPEAR)
				{
					if (g_aEnemy[nCntEnemy].pos.y >= STANDARD_POS)
					{
						g_aEnemy[nCntEnemy].enemyAtackCounter++;
						//�G�̒e�𔭎�
						//�G�̍����Ƀv���C���[
						if (g_aEnemy[nCntEnemy].pos.x - 100.0f > pPlayer->pos.x)
						{
							//����Ƀv���C���[
							if (g_aEnemy[nCntEnemy].pos.y - 100.0f > pPlayer->pos.y)
							{
								EnemyBulletAngle = -0.75f;
							}

							//�����Ƀv���C���[
							else if (g_aEnemy[nCntEnemy].pos.y + 100.0f < pPlayer->pos.y)
							{
								EnemyBulletAngle = -0.25f;
							}

							//�G�Ɠ������炢��X���W�Ƀv���C���[
							else
							{
								EnemyBulletAngle = -0.5f;
							}
						}

						//�G�̉E���Ƀv���C���[
						else if (g_aEnemy[nCntEnemy].pos.x + 100.0f < pPlayer->pos.x)
						{
							//�E��Ƀv���C���[
							if (g_aEnemy[nCntEnemy].pos.y - 100.0f > pPlayer->pos.y)
							{
								EnemyBulletAngle = 0.75f;
							}

							//�E���Ƀv���C���[
							else if (g_aEnemy[nCntEnemy].pos.y + 100.0f < pPlayer->pos.y)
							{
								EnemyBulletAngle = 0.25f;
							}

							//�G�Ɠ������炢��X���W�Ƀv���C���[
							else
							{
								EnemyBulletAngle = 0.5f;
							}
						}

						//�G�Ɠ������炢��Y���W�Ƀv���C���[
						else
						{
							if (g_aEnemy[nCntEnemy].pos.y + 30.0f < pPlayer->pos.y)
							{
								EnemyBulletAngle = 0.0f;
							}
							else
							{
								EnemyBulletAngle = 1.0f;
							}

						}						

						//�G�̎�ނɂ��e�̔��ˊԊu�ω�
						switch (g_aEnemy[nCntEnemy].nType)
						{
						case 0:		//�ʏ푬�x�̔���	�ԐF
							if (g_aEnemy[nCntEnemy].enemyAtackCounter % BULLETINTERVAL_NORMAL == ZERO)
							{
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_NORMAL, cosf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_NORMAL, ORIGIN_POS), D3DXCOLOR(MAX_POS, ORIGIN_POS, ORIGIN_POS, MAX_POS), g_aEnemy[nCntEnemy].enBullet, g_aEnemy[nCntEnemy].homingtype, BULLETEFFECT_ENEMY);
								BulletSound = ONE;
								g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;
							}

							//���_�J���[�̐ݒ�( �I�����W )
							{
								pVtx[0].col = D3DXCOLOR(MAX_POS, F_SEC, F_FIVE, MAX_POS);
								pVtx[1].col = D3DXCOLOR(MAX_POS, F_SEC, F_FIVE, MAX_POS);
								pVtx[2].col = D3DXCOLOR(MAX_POS, F_SEC, F_FIVE, MAX_POS);
								pVtx[3].col = D3DXCOLOR(MAX_POS, F_SEC, F_FIVE, MAX_POS);
							}
							break;

						case 1:		//�ʏ�̔{�̑��x�̔���	���F
							if (g_aEnemy[nCntEnemy].enemyAtackCounter % BULLETINTERVAL_SPEED == ZERO)
							{
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_SPEED, cosf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_SPEED, ORIGIN_POS), D3DXCOLOR(ORIGIN_POS, MAX_POS, MAX_POS, MAX_POS), g_aEnemy[nCntEnemy].enBullet, g_aEnemy[nCntEnemy].homingtype, BULLETEFFECT_FAST_ENEMY);
								BulletSound = ONE;
								g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;
							}

							//���_�J���[�̐ݒ�( �� )
							{
								pVtx[0].col = D3DXCOLOR(ORIGIN_POS, ORIGIN_POS, MAX_POS, MAX_POS);
								pVtx[1].col = D3DXCOLOR(ORIGIN_POS, ORIGIN_POS, MAX_POS, MAX_POS);
								pVtx[2].col = D3DXCOLOR(ORIGIN_POS, ORIGIN_POS, MAX_POS, MAX_POS);
								pVtx[3].col = D3DXCOLOR(ORIGIN_POS, ORIGIN_POS, MAX_POS, MAX_POS);
							}
							break;

						case 2:		//3�A�e����		���F
							if (g_aEnemy[nCntEnemy].enemyAtackCounter == BULLETINTERVAL_RUSH1 || g_aEnemy[nCntEnemy].enemyAtackCounter == BULLETINTERVAL_RUSH2 || g_aEnemy[nCntEnemy].enemyAtackCounter == BULLETINTERVAL_RUSH3)
							{
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_NORMAL, cosf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_NORMAL, ORIGIN_POS), D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS), g_aEnemy[nCntEnemy].enBullet, g_aEnemy[nCntEnemy].homingtype, BULLETEFFECT_ENEMY);
								BulletSound = ONE;
								if (g_aEnemy[nCntEnemy].enemyAtackCounter == BULLETINTERVAL_RUSH3)
								{
									g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;
								}
							}

							//���_�J���[�̐ݒ�( �� )
							{
								pVtx[0].col = D3DXCOLOR(ORIGIN_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[1].col = D3DXCOLOR(ORIGIN_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[2].col = D3DXCOLOR(ORIGIN_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[3].col = D3DXCOLOR(ORIGIN_POS, MAX_POS, ORIGIN_POS, MAX_POS);
							}
							break;

						case 3:		//�r�[������		�ΐF
							if (g_aEnemy[nCntEnemy].enemyAtackCounter % BULLETINTERVAL_BEAM == ZERO)
							{
								SetBeam(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, D3DX_PI * EnemyBulletAngle), D3DXVECTOR3(sinf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_BEAM, cosf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_BEAM, ORIGIN_POS), D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS));

								BeamSound = ONE;
								g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;
							}

							//���_�J���[�̐ݒ�( �� )
							{
								pVtx[0].col = D3DXCOLOR(MAX_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[1].col = D3DXCOLOR(MAX_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[2].col = D3DXCOLOR(MAX_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[3].col = D3DXCOLOR(MAX_POS, MAX_POS, ORIGIN_POS, MAX_POS);
							}
							break;
						}
					}
				}

				//�G�̈ړ��ړ�
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

				//�~��
				g_aEnemy[nCntEnemy].pos.y += FALL_ENEMY;

				//�G����ʊO�ɍs���Ă��܂���
				if (g_aEnemy[nCntEnemy].pos.y >= SCREEN_HEIGHT || g_aEnemy[nCntEnemy].pos.x <= ORIGIN_POS || g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH)
				{
					g_nNumEnemy--;						//�G�̑��������炷
					g_aEnemy[nCntEnemy].bUse = false;	//�G�𖢎g�p��Ԃɐݒ�

					if (g_nNumEnemy <= ZERO)
					{
						//�{�X������
						SetBoss();
					}
				}

				//�z�[�~���O���\�A���Ȃ�G�̐F����F�ɕς���
				if (g_aEnemy[nCntEnemy].homingtype == HOMING_POSSIBLE && g_aEnemy[nCntEnemy].state == ENEMYSTATE_NORMAL)
				{
					//���_�J���[�̐ݒ�
					{
						pVtx[0].col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f);
					}
				}
			}

			//�G�̃_���[�W��ԑJ��
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= ZERO)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					//�|���S����ʏ�F��
					//���_�J���[�̐ݒ�
					{
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}

				break;
			}

			//���_���W�̐ݒ�
			{
				pVtx[ZERO].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(-D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[ZERO].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(-D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[ONE].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[ONE].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[SEC].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(ORIGIN_POS - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[SEC].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(ORIGIN_POS - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[SAN].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(ORIGIN_POS + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[SAN].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(ORIGIN_POS + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			}
		}
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEnemy->Unlock();

	if (BulletSound == ONE)
	{
		PlaySound(SOUND_LABEL_SE_SHOT);
		BulletSound = 0;
	}

	if (BeamSound == ONE)
	{
		PlaySound(SOUND_LABEL_BEAM_SHOT);
		BeamSound = 0;
	}
}

//�G�̕`�揈��
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEnemy);

		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * NUM_VERTEX, SEC);
		}
	}
}

//�G�̐ݒ菈��
void SetEnemy()
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//�G���g�p����Ă��Ȃ�

			//�G���̐ݒ�
			g_aEnemy[nCntEnemy].enBullet = BULLETTYPE_ENEMY;	//�e�̎�ނ̐ݒ�

			//���_���̐ݒ�
			//���_���W�̐ݒ�
			{
				pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - ENEMY_VERTEX;
				pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - ENEMY_VERTEX;
				pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + ENEMY_VERTEX;
				pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - ENEMY_VERTEX;
				pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - ENEMY_VERTEX;
				pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + ENEMY_VERTEX;
				pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + ENEMY_VERTEX;
				pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + ENEMY_VERTEX;
			}

			g_aEnemy[nCntEnemy].bUse = true;			//�g�p���Ă����Ԃɂ���
			g_nNumEnemy++;
			break;
		}
		pVtx += NUM_VERTEX;
	}
	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//�G�̎擾
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];		//�G�̏��̐擪�A�h���X��Ԃ�
}

//�G�̃q�b�g����
void HitEnemy(int nCntEnemy, int nDamage, int nType, REFLECTBULLET type)
{
	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);	//�p�[�e�B�N�����T�񔭐�
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);
		g_nNumEnemy--;
		g_aEnemy[nCntEnemy].bUse = false;
		
		//���t���N�g�{�[�i�X
		int BonusScore = 1;	
		if (type == REFLECTBULLET_YES)
		{
			BonusScore++;
		}

		AddScore(SCORE_DEFO * nType * nDamage * BonusScore);

		if (g_nNumEnemy <= 0)
		{
			//�{�X������
			SetBoss();
		}
	}

	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		//�_���[�W�F�ݒ�
		
		VERTEX_2D *pVtx;

		//���_�A�����b�N
		g_pVtxBuffEnemy->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += nCntEnemy * NUM_VERTEX;

		//���_�J���[�̐ݒ�
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}

		//���_�o�b�t�@�����b�N����
		g_pVtxBuffEnemy->Unlock();
	}
}

//�G�̑�����Ԃ�
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}