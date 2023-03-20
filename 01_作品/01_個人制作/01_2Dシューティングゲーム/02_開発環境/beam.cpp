/*==========================================================================================================================

											�r�[�����ˏ���[beam.cpp]
												Author:���V����

==========================================================================================================================*/
#include "enemy.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "effect.h"
#include "particle.h"
#include "boss.h"
#include "beam.h"

//�}�N����`
#define MAX_BEAM			(256)		//�r�[���̍ő吔
#define FCORRECTION			(1.0f)		//�z�[�~���O�e�̕␳�W���̐��l
#define FHOMINGMOVE			(0.3f)		//�z�[�~���O�e�̈ړ���
#define FANGLE_CORRECT		(6.28f)		//�p�x�̏C���Ɏg�p
#define BEAM_HEIGHT			(100.0f)	//�r�[���̍���
#define BEAM_WIDTH			(30.0f)		//�r�[���̕�
#define BEAM_DAMAGE			(2)			//�r�[���̍U����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureBeam = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBeam = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Beam g_aBeam[MAX_BEAM];							//�r�[���̏��
float g_fLengthBeam;				//�Ίp���̒���
float g_fAngleBeam;					//�Ίp���̊p�x
bool g_f4;							//���GON/OFF

//�r�[���̏���������
void InitBeam(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntBeam;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/beam.png", &g_pTextureBeam);

	//�e�̏��̏�����
	for (nCntBeam = START_CNT; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		g_aBeam[nCntBeam].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
		g_aBeam[nCntBeam].move = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
		g_aBeam[nCntBeam].rot = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
		g_aBeam[nCntBeam].col = D3DXCOLOR(F_ZERO, F_ZERO, F_ZERO, F_ZERO);
		g_aBeam[nCntBeam].type = BULLETTYPE_ENEMY;
		g_aBeam[nCntBeam].Reflect = REFLECTBULLET_NO;			//���˕Ԃ���Ă��Ȃ���Ԃɐݒ�
		g_aBeam[nCntBeam].bUse = false;							//�g�p���Ă��Ȃ���Ԃɂ���
	}

	g_f4 = false;					//���GON/OFF�̏�����

	//�Ίp���̒������Z�o����
	g_fLengthBeam = sqrtf(BEAM_WIDTH * BEAM_WIDTH + BEAM_HEIGHT * BEAM_HEIGHT) * F_ATE;

	//�Ίp���̊p�x���Z�o����
	g_fAngleBeam = atan2f(BEAM_WIDTH, BEAM_HEIGHT);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BEAM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBeam, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBeam->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBeam = START_CNT; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		//���_���W�̐ݒ�
		{
			pVtx[ZERO].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
			pVtx[ONE].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
			pVtx[SEC].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
			pVtx[SAN].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
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
			pVtx[ZERO].col = D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN);
			pVtx[ONE].col = D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN);
			pVtx[SEC].col = D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN);
			pVtx[SAN].col = D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , F_TEN);
		}

		pVtx += NUM_VERTEX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBeam->Unlock();
}

//�r�[���̏I������
void UninitBeam(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBeam != NULL)
	{
		g_pTextureBeam->Release();
		g_pTextureBeam = NULL;
	}

	//���_���W�̔j��
	if (g_pVtxBuffBeam != NULL)
	{
		g_pVtxBuffBeam->Release();
		g_pVtxBuffBeam = NULL;
	}
}

//�r�[���̍X�V����
void UpdateBeam(void)
{
	int nCntBeam;
	int DeadCount = ZERO;

	//�v���C���[�̖��GON/OFF
	if (GetKeyboardTrigger(DIK_F4) == true)
	{
		g_f4 = g_f4 ? false : true;
	}

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBeam->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBeam = ZERO; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		if (g_aBeam[nCntBeam].bUse == true)
		{//�e���g�p����Ă���
		 //�e�̎�ނɂ�铖���蔻��
			switch (g_aBeam[nCntBeam].type)
			{
			case BULLETTYPE_PLAYER:
				//�G�̎擾
				CollisionEnemy_Beam(&g_aBeam[nCntBeam]);
				break;

			case BULLETTYPE_ENEMY:
			case BULLETTYPE_BOSS:
				//�v���C���[�̎擾
				CollisionPlayer_Beam(&g_aBeam[nCntBeam]);
				break;
			}

			//�e�̈ʒu�̍X�V
			g_aBeam[nCntBeam].pos.x += g_aBeam[nCntBeam].move.x;
			g_aBeam[nCntBeam].pos.y += g_aBeam[nCntBeam].move.y;

			//���_���W�̐ݒ�
			{
				pVtx[ZERO].pos.x = g_aBeam[nCntBeam].pos.x + sinf(g_aBeam[nCntBeam].rot.z + (-D3DX_PI + g_fAngleBeam)) * g_fLengthBeam;
				pVtx[ZERO].pos.y = g_aBeam[nCntBeam].pos.y + cosf(g_aBeam[nCntBeam].rot.z + (-D3DX_PI + g_fAngleBeam)) * g_fLengthBeam;
				pVtx[ONE].pos.x = g_aBeam[nCntBeam].pos.x + sinf(g_aBeam[nCntBeam].rot.z + ( D3DX_PI - g_fAngleBeam)) * g_fLengthBeam;
				pVtx[ONE].pos.y = g_aBeam[nCntBeam].pos.y + cosf(g_aBeam[nCntBeam].rot.z + ( D3DX_PI - g_fAngleBeam)) * g_fLengthBeam;
				pVtx[SEC].pos.x = g_aBeam[nCntBeam].pos.x + sinf(g_aBeam[nCntBeam].rot.z + (F_ZERO - g_fAngleBeam)) * g_fLengthBeam;
				pVtx[SEC].pos.y = g_aBeam[nCntBeam].pos.y + cosf(g_aBeam[nCntBeam].rot.z + (F_ZERO - g_fAngleBeam)) * g_fLengthBeam;
				pVtx[SAN].pos.x = g_aBeam[nCntBeam].pos.x + sinf(g_aBeam[nCntBeam].rot.z + (F_ZERO + g_fAngleBeam)) * g_fLengthBeam;
				pVtx[SAN].pos.y = g_aBeam[nCntBeam].pos.y + cosf(g_aBeam[nCntBeam].rot.z + (F_ZERO + g_fAngleBeam)) * g_fLengthBeam;
			}

			pVtx[ZERO].col = g_aBeam[nCntBeam].col;
			pVtx[ONE].col = g_aBeam[nCntBeam].col;
			pVtx[SEC].col = g_aBeam[nCntBeam].col;
			pVtx[SAN].col = g_aBeam[nCntBeam].col;

			//�e����ʊO�ɏo���ꍇ
			if (g_aBeam[nCntBeam].pos.y >  730.0f || g_aBeam[nCntBeam].pos.y < 0.0f
			 || g_aBeam[nCntBeam].pos.x > 1290.0f || g_aBeam[nCntBeam].pos.x < 0.0f)
			{
				g_aBeam[nCntBeam].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//�r�[�����g�p����Ȃ��Ȃ�����̏�����
			if (g_aBeam[nCntBeam].bUse == false)
			{
				pVtx[ZERO].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
				pVtx[ONE].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
				pVtx[SEC].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
				pVtx[SAN].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
			}
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBeam->Unlock();
}

//�r�[���̕`�揈��
void DrawBeam(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntBeam;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBeam, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureBeam);

	for (nCntBeam = START_CNT; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		if (g_aBeam[nCntBeam].bUse == true)
		{//�e���g�p����Ă���
		 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBeam * NUM_VERTEX, SEC);
		}
	}
}

//�e�̐ݒ菈��
void SetBeam(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col)
{
	int nCntBeam;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBeam->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBeam = START_CNT; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		if (g_aBeam[nCntBeam].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			//�e�̏���ݒ�
			g_aBeam[nCntBeam].pos.x = pos.x;
			g_aBeam[nCntBeam].pos.y = pos.y;
			g_aBeam[nCntBeam].rot.z = pos.z;	//SetBeam��ǂ񂾃L�����̊p�x��pos.z�ɓ����Ă���
			g_aBeam[nCntBeam].type = BULLETTYPE_ENEMY;
			g_aBeam[nCntBeam].col = col;
			g_aBeam[nCntBeam].Reflect = REFLECTBULLET_NO;

			pVtx[ZERO].col = col;
			pVtx[ONE].col = col;
			pVtx[SEC].col = col;
			pVtx[SAN].col = col;

			g_aBeam[nCntBeam].move.x = move.x;
			g_aBeam[nCntBeam].move.y = move.y;
			g_aBeam[nCntBeam].move.z = F_ZERO;

			g_aBeam[nCntBeam].bUse = true;			//�g�p���Ă����Ԃɂ���
			break;
		}
		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += NUM_VERTEX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBeam->Unlock();
}

//�G�̓����蔻��
void CollisionEnemy_Beam(Beam *pBeam)
{
	Enemy *pEnemy = GetEnemy();
	BOSS *pBoss = GetBoss();
	int nCntEnemy;
	int nDamage = BEAM_DAMAGE;		//�U����

	//���˕Ԃ��ꂽ�e�Ȃ�_���[�W�Q�{
	if (pBeam->Reflect == REFLECTBULLET_YES)
	{
		nDamage = nDamage + nDamage;
	}

	for (nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�G���g�p����Ă���
			if (pBeam->pos.x >= pEnemy->pos.x - 30.0f &&
				pBeam->pos.x <= pEnemy->pos.x + 30.0f &&
				pBeam->pos.y >= pEnemy->pos.y - 30.0f &&
				pBeam->pos.y <= pEnemy->pos.y + 30.0f)
			{
				HitEnemy(nCntEnemy, nDamage, pEnemy->nLifeMax, pBeam->Reflect);

				if (pEnemy->nLife > ONE)
				{
					SetParticle(pBeam->pos, D3DXCOLOR(F_TEN, F_TEN, F_ZERO, F_ATE), PARTICLETYPE_STAY);
				}

				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
	}

	if (pBoss->state == BOSSSTATE_SPIN && pBeam->type == BULLETTYPE_PLAYER)
	{
		//�r�[���e�ƃ{�X�Ƃ̓����蔻��
		if (pBeam->pos.x >= pBoss->pos.x - 50.0f &&
			pBeam->pos.x <= pBoss->pos.x + 50.0f &&
			pBeam->pos.y >= pBoss->pos.y - 20.0f &&
			pBeam->pos.y <= pBoss->pos.y + 20.0f)
		{
			//���ł��������֒��˕Ԃ�
			pBeam->move *= -1.0f;

			//�e�̏���h��ւ�
			pBeam->type = BULLETTYPE_BOSS;
			pBeam->Reflect = REFLECTBULLET_YES;
			pBeam->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		}
	}

	//�{�X���g���Ă��� & �����蔻��
	else if (pBeam->pos.x >= pBoss->pos.x - 50.0f &&
			 pBeam->pos.x <= pBoss->pos.x + 50.0f &&
			 pBeam->pos.y >= pBoss->pos.y - 20.0f &&
			 pBeam->pos.y <= pBoss->pos.y + 20.0f && pBoss->bDisp == true && pBoss->state == BOSSSTATE_NORMAL)
	{
		HitBoss(nDamage, pBeam->Reflect);

		if (pBoss->nLife > 1)
		{
			SetParticle(pBeam->pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f), PARTICLETYPE_STAY);
		}
	}
}

//�v���C���[�̓����蔻��
void CollisionPlayer_Beam(Beam *pBeam)
{
	Player *pPlayer = GetPlayer();

	//�X�s����
	if (pPlayer->state == PLAYERSTATE_SPIN && pBeam->type != BULLETTYPE_PLAYER)
	{
		//�G�̒e�Ƃ̓����蔻��
		if (pBeam->pos.x >= pPlayer->pos.x - BEAM_WIDTH &&
			pBeam->pos.x <= pPlayer->pos.x + BEAM_WIDTH &&
			pBeam->pos.y >= pPlayer->pos.y - BEAM_HEIGHT &&
			pBeam->pos.y <= pPlayer->pos.y + BEAM_HEIGHT)
		{
			if (g_f4 == false)
			{
				pPlayer->LimitSpin--;	//���˕Ԃ��񐔂�����

				//����𒴂����瓖���蔻�蕜��
				if (pPlayer->LimitSpin <= 0)
				{
					//��]�l���O��
					pPlayer->rot.z = 0.0f;

					HitPlayer(BEAM_DAMAGE);
					PlaySound(SOUND_LABEL_SE_HIT);
				}

				//���˕Ԃ�
				else
				{
					//���ł��������֒��˕Ԃ�
					pBeam->move *= -1.0f;

					//�e�̏���h��ւ�
					pBeam->type = BULLETTYPE_PLAYER;
					pBeam->Reflect = REFLECTBULLET_YES;
					if (pBeam->rot.z <= 0.0f)
					{
						pBeam->rot.z = pBeam->rot.z + D3DX_PI;
					}
					else
					{
						pBeam->rot.z = pBeam->rot.z - D3DX_PI;
					}

					pBeam->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				}
			}
		}
	}

	//�����蔻��
	else if (pPlayer->state == PLAYERSTATE_NORMAL && pPlayer->bDisp == true)
	{
		//�G�̒e�Ƃ̓����蔻��
		if (pBeam->pos.x >= pPlayer->pos.x - BEAM_WIDTH &&
			pBeam->pos.x <= pPlayer->pos.x + BEAM_WIDTH &&
			pBeam->pos.y >= pPlayer->pos.y - BEAM_HEIGHT &&
			pBeam->pos.y <= pPlayer->pos.y + BEAM_HEIGHT)
		{
			//�����̐ݒ�
			if (g_f4 == false)
			{
				HitPlayer(BEAM_DAMAGE);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
			pBeam->bUse = false;
		}
	}
}