/*==========================================================================================================================================================

															�e����[bullet.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "bullet.h"
#include "billboard.h"
#include "color.h"
#include "player.h"
#include "input.h"
#include "particle.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "wall.h"
#include "game.h"
#include "slope.h"
#include "sound.h"
#include "cannon.h"
#include "model.h"
#include "meshfield.h"

//�}�N����`
#define MAX_BULLET				(256)		//�e�̍ő吔
#define BULLET_HP				(200)		//�e�̗̑�
#define BULLET_SIZE				(4.0f)		//�e�̕�
#define BULLET_EFFECT_LIFE		(40)		//�e�̋O�Ղ̎���

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldBullet;				//���[���h�}�g���b�N�X
Bullet					g_aBullet[MAX_BULLET];			//�e�̏��

//------------------------------------------------
//					�e�̏���������
//------------------------------------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &g_pTextureBullet);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//�e�̏��̏�����
		g_aBullet[nCntBullet].pos = ZERO_SET;				// �ʒu������
		g_aBullet[nCntBullet].posOld = ZERO_SET;			// �O��̈ʒu������
		g_aBullet[nCntBullet].move = ZERO_SET;				// �ړ��ʏ�����
		g_aBullet[nCntBullet].nLife = BULLET_HP;			// ����������
		g_aBullet[nCntBullet].size = F_ZERO;				// �e�̃T�C�Y������
		g_aBullet[nCntBullet].bUse = false;					// �g�p���Ă��Ȃ���Ԃɂ���
		g_aBullet[nCntBullet].owner = BULLET_OWNER_MAX;		// �I�[�i�[������
		g_aBullet[nCntBullet].nIdxShadow = SetShadow();		// �e�̐ݒ�

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].size, +g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].size, +g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].size, -g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].size, -g_aBullet[nCntBullet].size, F_ZERO);
		}

		//rhw�̐ݒ�
		{
			pVtx[VTX_LE_UP].nor = SET_NOR;
			pVtx[VTX_RI_UP].nor = SET_NOR;
			pVtx[VTX_LE_DO].nor = SET_NOR;
			pVtx[VTX_RI_DO].nor = SET_NOR;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += VTX_MAX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//------------------------------------------------
//					�e�̏I������
//------------------------------------------------
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

//------------------------------------------------
//					�e�̍X�V����
//------------------------------------------------
void UpdateBullet(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���

			//�O��̈ʒu�X�V
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//�e�̈ʒu�̍X�V
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//���_���W�̐ݒ�
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].size, +g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].size, +g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].size, -g_aBullet[nCntBullet].size, F_ZERO);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].size, -g_aBullet[nCntBullet].size, F_ZERO);

			//���_�J���[�̐ݒ�
			pVtx[VTX_LE_UP].col = g_aBullet[nCntBullet].col;
			pVtx[VTX_RI_UP].col = g_aBullet[nCntBullet].col;
			pVtx[VTX_LE_DO].col = g_aBullet[nCntBullet].col;
			pVtx[VTX_RI_DO].col = g_aBullet[nCntBullet].col;

			//���������炷
			g_aBullet[nCntBullet].nLife--;

			//�e�̓����蔻��܂Ƃ�
			WholeBulletCollision(nCntBullet);

			//�e�`��
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size, g_aBullet[nCntBullet].size);

			//�G�t�F�N�g�`��
			SetEffect(g_aBullet[nCntBullet].pos, XCOL_YELLOW, g_aBullet[nCntBullet].size * F_DOUBLE, BULLET_EFFECT_LIFE);

			//�g���Ȃ��Ȃ����e�̏�����
			if (g_aBullet[nCntBullet].bUse == false)
			{
				//�����ݒ�
				SetExplosion(g_aBullet[nCntBullet].pos, XCOL_WHITE);

				BreakShadow(g_aBullet[nCntBullet].nIdxShadow);	//�e�̎g�p��j������
			}
		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += VTX_MAX;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//�e�̓����蔻��܂Ƃ�
void WholeBulletCollision(int nCntBullet)
{
	MeshField *Null;	//��������

	if (CollisionWall(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move) == true ||					//�ǂɓ�������
		ColliMeshField(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, &Null) == true ||					//�n�ʂɓ�������
		ColliPlayerToBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size, g_aBullet[nCntBullet].owner) == true ||					//�v���C���[�ɓ�������
		ColliCannonToBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size, g_aBullet[nCntBullet].owner) == true ||					//�v���C���[�ɓ�������
		CollisionModel(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, COLLITYPE_BULLET) == true ||	//���f���ɓ�������
		CollisionSlope(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, &g_aBullet[nCntBullet].rot, COLLITYPE_BULLET) == true ||	//�⓹�ɓ�������
		g_aBullet[nCntBullet].nLife <= ZERO)																								//�������s���Ă���
	{
		g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//------------------------------------------------
//					�e�̕`�揈��
//------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p

	//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//�r���{�[�h�̕`��ݒ�
	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldBullet);

			//�r���[�}�g���b�N�X��Get
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_mtxWorldBullet, NULL, &mtxView);	//�t�s������߂�
			g_mtxWorldBullet._41 = F_ZERO;		//����
			g_mtxWorldBullet._42 = F_ZERO;		//	���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
			g_mtxWorldBullet._43 = F_ZERO;		//����

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldBullet, &g_mtxWorldBullet, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBullet);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffBullet, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * VTX_MAX, SEC);
		}
	}

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//�e�̐ݒ菈��
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size, BULLET_OWNER owner)
{
	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			//�e�̏���ݒ�
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].rot.z = ZERO;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].size = size;
			g_aBullet[nCntBullet].owner = owner;

			g_aBullet[nCntBullet].nLife = BULLET_HP;
			g_aBullet[nCntBullet].bUse = true;			//�g�p���Ă����Ԃɂ���

			//�e�̔��ˉ��Đ�
			PlaySound(SOUND_LABEL_SE_FILING);
			break;
		}
	}
}