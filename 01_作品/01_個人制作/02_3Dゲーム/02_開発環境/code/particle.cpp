/*==========================================================================================================================================================

														�p�[�e�B�N������[parthicle.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "particle.h"
#include "billboard.h"
#include "wall.h"
#include "color.h"

//�}�N����`
#define MAX_PARTICLE				(5000)		//�p�[�e�B�N���̍ő吔
#define PARTICLE_LIFE				(60)		//�p�[�e�B�N���̎���

#define MAX_RADIUS					(10.0f)		//���a�̏����l
#define MINUS_RADIUS				(0.1f)		//���a�̏k���l

//�p�[�e�B�N���̔����Ɏg���ړ������̃}�N��
#define ANGLE						(629)		//�p�[�e�B�N�����ړ��������
#define FIX_ANGLE					(314)		//ANGLE�̊p�x���C��

#define TIP_UP						(3)			//���ˏグ��ړ���
#define MIN_TIP						(2)			//�Œ���̈ړ���
#define FIX_TIP_UP					(3)			//TIP_UP�̔{���C��
#define NORMAL_MOVE_PARTICLE		(2.0f)		//�p�[�e�B�N���̊�{�ړ���
#define SLOW_MOVE_PARTICLE			(0.5f)		//�p�[�e�B�N���̂������ړ���
#define PARTICLE_NUM_EXPLOSION		(30)		//�����I�ȃp�[�e�B�N���̔�����

//�p�[�e�B�N���̏d�̓}�N��
#define GRAVITY_PARTICLE			(-4.0f)		//�p�[�e�B�N���̊�{�d�͉����x
#define NORMAL_GRAVITY_PART_COR		(0.2f)		//�p�[�e�B�N���̊�{�d�͌W��
#define SLOW_GRAVITY_PART_COR		(0.05f)		//�p�[�e�B�N���̂������d�͌W��
#define BOUND_PARTICLE_COR			(-0.9f)		//�p�[�e�B�N���̃o�E���h�␳�l

#define REFLECT_PARTICEL			(-1.0f)		//���˕Ԃ��W��

#define PARTICLE_LIMIT_WALL			(600.0f)	//�ǂ̈ʒu
#define PARTICLE_LIMIT_FLOOR		(000.0f)	//���̍��W

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldParticle;			// ���[���h�}�g���b�N�X
Particle				g_aParticle[MAX_PARTICLE];	//�p�[�e�B�N���̏��

//--------------------------------------------------------
//				�p�[�e�B�N���̏���������
//--------------------------------------------------------
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle000.jpg", &g_pTextureParticle);	//�����p�[�e�B�N��
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffParticle, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//�p�[�e�B�N���̏��̏�����
		g_aParticle[nCntParticle].pos		= ZERO_SET;			// �ʒu�̏�����
		g_aParticle[nCntParticle].move		= ZERO_SET;			// �ړ��ʂ̏�����
		g_aParticle[nCntParticle].col		= PARTICLE_COL;		// �F�̏�����
		g_aParticle[nCntParticle].nLife		= PARTICLE_LIFE;	// �����̏�����
		g_aParticle[nCntParticle].fRadius	= MAX_RADIUS;		// ���a�̏�����
		g_aParticle[nCntParticle].nLife		= SET_PARTICLE_MAX;	// �^�C�v������
		g_aParticle[nCntParticle].bUse		= false;			// �g�p���Ă��Ȃ���Ԃɂ���

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, +g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aParticle[nCntParticle].fRadius, +g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
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
			pVtx[VTX_LE_UP].col = g_aParticle[nCntParticle].col;
			pVtx[VTX_RI_UP].col = g_aParticle[nCntParticle].col;
			pVtx[VTX_LE_DO].col = g_aParticle[nCntParticle].col;
			pVtx[VTX_RI_DO].col = g_aParticle[nCntParticle].col;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = ORIGIN_TEX;
			pVtx[VTX_RI_UP].tex = MA_ZE_TEX;
			pVtx[VTX_LE_DO].tex = ZE_MA_TEX;
			pVtx[VTX_RI_DO].tex = LIMIT_TEX;
		}

		pVtx += VTX_MAX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffParticle->Unlock();
}

//--------------------------------------------------------
//				�p�[�e�B�N���̏I������
//--------------------------------------------------------
void UninitParticle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}
	
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//--------------------------------------------------------
//				�p�[�e�B�N���̍X�V����
//--------------------------------------------------------
void UpdateParticle(void)
{
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//�p�[�e�B�N�����ړ�
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//������蔚��
			if (g_aParticle[nCntParticle].nType == SET_PARTICLE_SLOW_EX)
			{
				//�p�[�e�B�N���ɒʏ�̏d�͂�������
				g_aParticle[nCntParticle].move.y += (GRAVITY_PARTICLE - g_aParticle[nCntParticle].move.y) * SLOW_GRAVITY_PART_COR;
			}
			else
			{
				//�p�[�e�B�N���ɒʏ�̏d�͂�������
				g_aParticle[nCntParticle].move.y += (GRAVITY_PARTICLE - g_aParticle[nCntParticle].move.y) * NORMAL_GRAVITY_PART_COR;
			}

			//�p�[�e�B�N���� �ǁE���ɓ�������������
			BoundParticle(nCntParticle);

			//���a���k��
			g_aParticle[nCntParticle].fRadius -= MINUS_RADIUS;

			//���a��0�ɂȂ���
			if (g_aParticle[nCntParticle].fRadius <= F_ZERO)
			{
				g_aParticle[nCntParticle].bUse = false;			//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//���a���g���Ē��_���W��ݒ�
			SetParticleRadius(nCntParticle);
		}
	}
}

//--------------------------------------------------------
//				�p�[�e�B�N���̕`�揈��
//--------------------------------------------------------
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p

	//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Z�o�b�t�@�̗L�������ݒ�
	
	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�r���{�[�h�̕`��ݒ�
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true &&				//�p�[�e�B�N�����g�p����Ă���
			WallDrawObject(g_aParticle[nCntParticle].pos) == true)	//�p�[�e�B�N���ƃJ�����̊Ԃɕǂ��Ȃ�
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldParticle);

			//�r���[�}�g���b�N�X��Get
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_mtxWorldParticle, NULL, &mtxView);	//�t�s������߂�
			g_mtxWorldParticle._41 = F_ZERO;		//����
			g_mtxWorldParticle._42 = F_ZERO;		//	���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
			g_mtxWorldParticle._43 = F_ZERO;		//����

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffParticle, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureParticle);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * VTX_MAX, SEC);
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL �� �u<=�v�̂���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Z�o�b�t�@�̗L�������ݒ�

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//�p�[�e�B�N���̐ݒ菈��
void SetParticle(D3DXVECTOR3 pos, SET_PARTICLE nType, D3DXCOLOR col)
{
	//�����p�^�[������
	switch (nType)
	{
	case SET_PARTICLE_NORMAL:		//�p�[�e�B�N���ʏ픭��
		SetParticleNor(pos, col, nType);
		break;

	case SET_PARTICLE_SLOW_EX:
		SetParticleExplosion(pos, col, SLOW_MOVE_PARTICLE, nType);	//�p�[�e�B�N����������
		break;

	case SET_PARTICLE_EXPLOSION:
		SetParticleExplosion(pos, col, NORMAL_MOVE_PARTICLE, nType);	//�p�[�e�B�N����������
		break;
	}
}

//"�ʏ�" �����̃p�[�e�B�N���ݒ�
void SetParticleNor(D3DXVECTOR3 pos, D3DXCOLOR col, SET_PARTICLE nType)
{
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			D3DXVECTOR3 fAngle;		//�p�[�e�B�N�����ړ�����p�x��ݒ�

			fAngle.x = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;	// ���ꂼ��̕����̊p�x������
			fAngle.y = (float)(rand() % TIP_UP + MIN_TIP);				// ���ꂼ��̕����̊p�x������
			fAngle.z = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;	// ���ꂼ��̕����̊p�x������

			float fMove = (float)(rand() % ANGLE) / FIX_FLOAT + NORMAL_MOVE_PARTICLE;	// �p�[�e�B�N���̈ړ���

			//�p�[�e�B�N�����̐ݒ�
			g_aParticle[nCntParticle].pos = pos;							// �ʒu�ݒ�
			g_aParticle[nCntParticle].nType = nType;						// ��ސݒ�
			g_aParticle[nCntParticle].move.x = (fAngle.x) * fMove;			// �ړ��ʐݒ�
			g_aParticle[nCntParticle].move.y = (fAngle.y) * fMove;			// �ړ��ʐݒ�
			g_aParticle[nCntParticle].move.z = (fAngle.z) * fMove;			// �ړ��ʐݒ�
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;				// �����ݒ�
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;					// ���a�ݒ�
			g_aParticle[nCntParticle].bUse = true;							// �g�p���Ă����Ԃɂ���

			//���[�v�𔲂���
			break;
		}
	}
}

//"����" �����̃p�[�e�B�N���ݒ�
void SetParticleExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fSpeed, SET_PARTICLE nType)
{
	//�����ʃJ�E���g
	int nCntBirth = ZERO;

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			D3DXVECTOR3 fAngle;		//�p�[�e�B�N�����ړ�����p�x��ݒ�
				
			fAngle.x = (float)(rand() %  ANGLE  - FIX_ANGLE ) / FIX_FLOAT;	// ���ꂼ��̕����̊p�x������
			fAngle.y = (float)(rand() %  TIP_UP +  MIN_TIP  );				// ���ꂼ��̕����̊p�x������
			fAngle.z = (float)(rand() %  ANGLE  - FIX_ANGLE ) / FIX_FLOAT;	// ���ꂼ��̕����̊p�x������

			float fMove = (float)(rand() % ANGLE) / FIX_FLOAT + fSpeed;		// �p�[�e�B�N���̈ړ���

			//�p�[�e�B�N�����̐ݒ�
			g_aParticle[nCntParticle].pos = pos;							// �ʒu�ݒ�
			g_aParticle[nCntParticle].col = col;							// �F�ݒ�
			g_aParticle[nCntParticle].move.x = (fAngle.x) * fMove;			// �ړ��ʐݒ�
			g_aParticle[nCntParticle].move.y = (fAngle.y) * fMove;			// �ړ��ʐݒ�
			g_aParticle[nCntParticle].move.z = (fAngle.z) * fMove;			// �ړ��ʐݒ�
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;				// �����ݒ�
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;					// ���a�ݒ�
			g_aParticle[nCntParticle].bUse = true;							// �g�p���Ă����Ԃɂ���

			//������蔚���̃^�C�v�Ȃ� Y�̈ړ��ʂ𑝂₷
			if (nType == SET_PARTICLE_SLOW_EX)
			{
				g_aParticle[nCntParticle].move.y += g_aParticle[nCntParticle].move.y;
			}

			//�����ʑ���
			nCntBirth++;

			//�����ʂ��K��ʂ𒴂�����I��
			if (nCntBirth >= PARTICLE_NUM_EXPLOSION)
			{
				break;
			}
		}
	}
}

//�p�[�e�B�N�����ǂɓ����������ɒ��˕Ԃ�
void BoundParticle(int nCntParticle)
{
	//X���W�̔���
	if (g_aParticle[nCntParticle].pos.x >= PARTICLE_LIMIT_WALL || g_aParticle[nCntParticle].pos.x <= -PARTICLE_LIMIT_WALL)
	{
		g_aParticle[nCntParticle].move.x *= REFLECT_PARTICEL;		//�ړ��ʂ��t�]������
	}

	//Y���W�̔���
	if (g_aParticle[nCntParticle].pos.y <= PARTICLE_LIMIT_FLOOR)
	{//�o�E���h������
		g_aParticle[nCntParticle].move.y = (GRAVITY_PARTICLE + g_aParticle[nCntParticle].move.y) * BOUND_PARTICLE_COR;
	}

	//Z���W�̔���
	if (g_aParticle[nCntParticle].pos.z >= PARTICLE_LIMIT_WALL || g_aParticle[nCntParticle].pos.z <= -PARTICLE_LIMIT_WALL)
	{
		g_aParticle[nCntParticle].move.z *= REFLECT_PARTICEL;		//�ړ��ʂ��t�]������
	}
}

//�p�[�e�B�N���̔��a����
void SetParticleRadius(int nCntParticle)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	pVtx += nCntParticle * VTX_MAX;	//���_�o�b�t�@���p�[�e�B�N���̔ԍ��̐����₷

	//���_���W�̐ݒ�
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
	}

	//���_�J���[�̐ݒ�
	{
		pVtx[VTX_LE_UP].col = g_aParticle[nCntParticle].col;
		pVtx[VTX_RI_UP].col = g_aParticle[nCntParticle].col;
		pVtx[VTX_LE_DO].col = g_aParticle[nCntParticle].col;
		pVtx[VTX_RI_DO].col = g_aParticle[nCntParticle].col;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffParticle->Unlock();
}