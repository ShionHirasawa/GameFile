/*==========================================================================================================================================================

�ۑ��g�Q�̏���[logthorns.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "logthorns.h"
#include "player.h"
#include "meshfield.h"
#include "sound.h"
#include "slope.h"

#define LOGTHORNS_TEX					(10)						// �ۑ��g�Q�̃e�N�X�`���̍ő吔
#define NUM_LOGTHORNS					(64)						// �ۑ��g�Q�̑���
#define LOGTHORNS_XFILE					"data/MODEL/LogThorns.x"	// �ۑ��g�Q�̃t�@�C����
#define LOGTHORNS_INFOFILE				"data/CSV/logthorns.csv"	// �ۑ��g�Q�̏��t�@�C����
#define LOGTHORNS_ROLL_SPEED			(-0.1f)						// �ۑ��g�Q�̉�]���x
#define LOGTHORNS_CHARGE_TIME			(300)						// �ۑ��g�Q�̃`���[�W����
#define LOGTHORNS_SPWAN_INTERVAL		(80)						// ���ݏo���Ԋu
#define LOGTHORNS_GRAVITY				(-3.0f)						// �ۑ��g�Q�̏d��
#define LOGTHORNS_FIX_HEIGHT_POS		(1.0f)						// �ۑ��g�Q�̈ʒu���C�����鍂��

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureLogThorns[LOGTHORNS_TEX] = {};// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH				g_pMeshLogThorns;						// ���b�V��(���_���)�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldLogThorns;					// ���[���h�}�g���b�N�X
LPD3DXBUFFER			g_pBuffMatLogThorns;					// �}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatLogThorns;					// �}�e���A���̐�
LogThorns				g_LogThorns[NUM_LOGTHORNS];				// �ۑ��g�Q�̏��

//----------------------------------------------------
//				�ۑ��g�Q�̏���������
//----------------------------------------------------
void InitLogThorns(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�ۑ��g�Q�̏�񏉊���
	for (int nCntLogThorns = 0; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		g_LogThorns[nCntLogThorns].pos = ZERO_SET;		// �ʒu������
		g_LogThorns[nCntLogThorns].posOld = ZERO_SET;	// �O��ʒu������
		g_LogThorns[nCntLogThorns].move = ZERO_SET;		// �ړ��ʏ�����
		g_LogThorns[nCntLogThorns].rot = ZERO_SET;		// ����������
		g_LogThorns[nCntLogThorns].bUse = false;		// �g���Ă��Ȃ��悤�ɂ���
	}	

	g_pMeshLogThorns = NULL;		// ���b�V��������
	g_pBuffMatLogThorns = NULL;		// �}�e���A���ւ̃|�C���^������
	g_dwNumMatLogThorns = NULL;		// �}�e���A���̐�������

	// X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX(LOGTHORNS_XFILE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatLogThorns, NULL, &g_dwNumMatLogThorns, &g_pMeshLogThorns);

	//�ۑ��g�Q�̃e�N�X�`���ݒ�
	for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatLogThorns; nCntMat++)
	{
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatLogThorns->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureLogThorns[nCntMat]);
		}
	}

	//�ŏ��E�ő�ʒu�ݒ�
	SearchLogThorns_MinAndMax_Pos();
}

//�����蔻��Ɏg�p����ŏ��E�ő�l�ݒ菈��
void SearchLogThorns_MinAndMax_Pos(void)
{
	int		nNumVtx;		//���_��
	DWORD	dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_�����擾
	nNumVtx = g_pMeshLogThorns->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshLogThorns->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshLogThorns->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

	//���_���W�̑��
	D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

	//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
	pVtxBuff += dwSizeFVF;

	D3DXVECTOR3 MinPos = vtx;	//�ŏ��̈ʒu�����f���̌��_�̈ʒu���狁�߂�
	D3DXVECTOR3 MaxPos = vtx;	//�ő�̈ʒu�����f���̌��_�̈ʒu���狁�߂�

	//�����蔻��Ɏg�����_�̍ŏ��E�ő�ʒu���擾
	for (int nCntVtx = ONE; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���݊i�[����Ă���ŏ��E�ő�l��ǂݍ��񂾒��_���W�œh��ւ��邩�ǂ�������
		//------------------------------
		//			X�̔���
		//------------------------------
		// �ŏ��l����
		if (vtx.x <= MinPos.x)
		{
			MinPos.x = vtx.x;		//�ŏ��l���
		}

		// �ő�l����
		else if (MaxPos.x <= vtx.x)
		{
			MaxPos.x = vtx.x;		//�ő�l���
		}

		//------------------------------
		//			Y�̔���
		//------------------------------
		// �ŏ��l����
		if (vtx.y <= MinPos.y)
		{
			MinPos.y = vtx.y;		//�ŏ��l���
		}

		// �ő�l����
		else if (MaxPos.y <= vtx.y)
		{
			MaxPos.y = vtx.y;		//�ő�l���
		}

		//------------------------------
		//			Z�̔���
		//------------------------------
		// �ŏ��l����
		if (vtx.z <= MinPos.z)
		{
			MinPos.z = vtx.z;		//�ŏ��l���
		}

		// �ő�l����
		else if (MaxPos.z <= vtx.z)
		{
			MaxPos.z = vtx.z;		//�ő�l���
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;

		//���_���W�̑��
		vtx = *(D3DXVECTOR3 *)pVtxBuff;
	}

	//�ŏ��ʒu�ƍő�ʒu����
	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		g_LogThorns[nCntLogThorns].MinPos = g_LogThorns[nCntLogThorns].pos + MinPos;		//�ŏ��̈ʒu�����f���̌��_�̈ʒu���狁�߂�
		g_LogThorns[nCntLogThorns].MaxPos = g_LogThorns[nCntLogThorns].pos + MaxPos;		//�ő�̈ʒu�����f���̌��_�̈ʒu���狁�߂�
		g_LogThorns[nCntLogThorns].pos.y = g_LogThorns[nCntLogThorns].MinPos.y;				//���_�ʒu�͍ŏ��ɂ���
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshLogThorns->UnlockVertexBuffer();
}

//----------------------------------------------
//				�ۑ��g�Q�̏I������
//----------------------------------------------
void UninitLogThorns(void)
{
	//���b�V���j��
	if (g_pMeshLogThorns != NULL)
	{
		g_pMeshLogThorns->Release();
		g_pMeshLogThorns = NULL;
	}

	//�}�e���A���j��
	if (g_pBuffMatLogThorns != NULL)
	{
		g_pBuffMatLogThorns->Release();
		g_pBuffMatLogThorns = NULL;
	}

	//�e�N�X�`���j��
	for (int nCntTex = START_CNT; nCntTex < LOGTHORNS_TEX; nCntTex++)
	{
		if (g_pTextureLogThorns[nCntTex] != NULL)
		{
			g_pTextureLogThorns[nCntTex]->Release();
			g_pTextureLogThorns[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------
//				�ۑ��g�Q�̍X�V����
//----------------------------------------------
void UpdateLogThorns(void)
{
	//�ۑ��g�Q�̍X�V����
	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//�g���Ă���
		if (g_LogThorns[nCntLogThorns].bUse)
		{
			//�O��̈ʒu�X�V
			g_LogThorns[nCntLogThorns].posOld = g_LogThorns[nCntLogThorns].pos;

			g_LogThorns[nCntLogThorns].move.y += (LOGTHORNS_GRAVITY - g_LogThorns[nCntLogThorns].move.y) * 0.1f;

			//�ۑ��g�Q�̈ړ�����
			MoveLogthorns(nCntLogThorns);

			//�ۑ��g�Q�̉�]����
			RollLogThorns(nCntLogThorns);

			//�ۑ��g�Q�̓����蔻��܂Ƃ�	�������菈��
			WholeColliLogThorns(nCntLogThorns);
		}
	}
}

//�ۑ��g�Q�̈ړ�����
void MoveLogthorns(int nCntLogThorns)
{
	g_LogThorns[nCntLogThorns].pos += g_LogThorns[nCntLogThorns].move;	//�ړ�
}

//�ۑ��g�Q�̉�]����
void RollLogThorns(int nCntLogThorns)
{
	g_LogThorns[nCntLogThorns].rot.x += LOGTHORNS_ROLL_SPEED;

	//��]�l�w��-3.14���������
	if (g_LogThorns[nCntLogThorns].rot.x <= -D3DX_PI)
	{
		//3.14�ɏC��
		g_LogThorns[nCntLogThorns].rot.x = D3DX_PI;
	}
}

//�ۑ��g�Q�̓����蔻��܂Ƃ�
void WholeColliLogThorns(int nCntLogThorns)
{
	//��������
	MeshField *Null;

	//�t�B�[���h�ƍ⓹�̓����蔻��
	if (ColliMeshField(&g_LogThorns[nCntLogThorns].pos, &g_LogThorns[nCntLogThorns].posOld, &g_LogThorns[nCntLogThorns].move, &Null) == true ||
		CollisionSlope(&g_LogThorns[nCntLogThorns].pos, &g_LogThorns[nCntLogThorns].posOld, &g_LogThorns[nCntLogThorns].move, &g_LogThorns[nCntLogThorns].rot, COLLITYPE_ENEMY) == true)
	{		
		g_LogThorns[nCntLogThorns].pos.y = g_LogThorns[nCntLogThorns].posOld.y;	//�ʒu�C��
		g_LogThorns[nCntLogThorns].move.y = NIL_F;								//�d�̓[��
	}

	//�ޗ��܂ŗ����Ă��܂���
	if (g_LogThorns[nCntLogThorns].pos.y <= THE_END_HELL)
	{
		//�g��Ȃ�����
		g_LogThorns[nCntLogThorns].bUse = false;
	}
}

//----------------------------------------------------
//					�ۑ��g�Q�̕`�揈��
//----------------------------------------------------
void DrawLogThorns(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//�g���Ă���
		if (g_LogThorns[nCntLogThorns].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldLogThorns);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_LogThorns[nCntLogThorns].rot.y, g_LogThorns[nCntLogThorns].rot.x, g_LogThorns[nCntLogThorns].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldLogThorns, &g_mtxWorldLogThorns, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_LogThorns[nCntLogThorns].pos.x, g_LogThorns[nCntLogThorns].pos.y, g_LogThorns[nCntLogThorns].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldLogThorns, &g_mtxWorldLogThorns, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldLogThorns);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_pBuffMatLogThorns->GetBufferPointer();

			//�}�e���A���ݒ�
			for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatLogThorns; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(ZERO, g_pTextureLogThorns[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_pMeshLogThorns->DrawSubset(nCntMat);
			}

			//�}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//�ۑ��g�Q�̓����蔻��
bool CollisionLogThorns(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType, bool *pLand, bool *pJump)
{
	bool bCollision = false;		//�����������ǂ����̔���
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾

	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//�g���Ă���
		if (g_LogThorns[nCntLogThorns].bUse)
		{
			// ���ꂼ��̍��W�̍ŏ��E�ő�ʒu�͈͓̔��ɂ���
			if (g_LogThorns[nCntLogThorns].MinPos.x <= pPos->x && pPos->x <= g_LogThorns[nCntLogThorns].MaxPos.x &&
				g_LogThorns[nCntLogThorns].MinPos.y <= pPos->y && pPos->y < g_LogThorns[nCntLogThorns].MaxPos.y &&
				g_LogThorns[nCntLogThorns].MinPos.z <= pPos->z && pPos->z <= g_LogThorns[nCntLogThorns].MaxPos.z)
			{
				//�����������ǂ�������
				ColliLogThorns_Max(pPos, pPosold, nCntLogThorns, nType, pLand, pJump);	// �ő�l�̓����蔻��
				ColliLogThorns_Min(pPos, pPosold, nCntLogThorns, nType);				// �ŏ��l�̓����蔻��
				bCollision = true;														// ��������
				break;																	// ����������for���𔲂���
			}
		}
	}

	//�����Ԃ�
	return bCollision;
}

//�ő�l���g���������蔻��
void ColliLogThorns_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntLogThorns, COLLITYPE nType, bool *pLand, bool *pJump)
{
	if (g_LogThorns[nCntLogThorns].MaxPos.x <= pPosold->x)	//�O��̈ʒu���Ώۂ̊ۑ��g�Q�� X�̍ŏ��ʒu�̂��傫������
	{
		pPos->x = g_LogThorns[nCntLogThorns].MaxPos.x;		//����̈ʒu��Ώۂ̊ۑ��g�Q�� X�̈ʒu�ɂ���
	}

	if (g_LogThorns[nCntLogThorns].MaxPos.y <= pPosold->y)	//�O��̈ʒu���Ώۂ̊ۑ��g�Q�� Z�̍ŏ��ʒu�̂��傫������
	{
		pPos->y = g_LogThorns[nCntLogThorns].MaxPos.y;		//����̈ʒu��Ώۂ̊ۑ��g�Q�� Z�̈ʒu�ɂ���
		*pLand = true;										//����Ă���
		*pJump = false;										//�W�����v���Ă��Ȃ�
	}

	if (g_LogThorns[nCntLogThorns].MaxPos.z <= pPosold->z)	//�O��̈ʒu���Ώۂ̊ۑ��g�Q�� Z�̍ŏ��ʒu�̂��傫������
	{
		pPos->z = g_LogThorns[nCntLogThorns].MaxPos.z;		//����̈ʒu��Ώۂ̊ۑ��g�Q�� Z�̈ʒu�ɂ���
	}
}

//�ŏ��l���g���������蔻��
void ColliLogThorns_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntLogThorns, COLLITYPE nType)
{
	if (pPosold->x <= g_LogThorns[nCntLogThorns].MinPos.x)	//�O��̈ʒu���Ώۂ̊ۑ��g�Q�� X�̍ŏ��ʒu�̂�菬��������
	{
		pPos->x = g_LogThorns[nCntLogThorns].MinPos.x;		//����̈ʒu��Ώۂ̊ۑ��g�Q�� X�̈ʒu�ɂ���
	}

	if (pPosold->y <= g_LogThorns[nCntLogThorns].MinPos.y)	//�O��̈ʒu���Ώۂ̊ۑ��g�Q�� Z�̍ŏ��ʒu�̂�菬��������
	{
		pPos->y = g_LogThorns[nCntLogThorns].MinPos.y;		//����̈ʒu��Ώۂ̊ۑ��g�Q�� Z�̈ʒu�ɂ���
	}

	if (pPosold->z <= g_LogThorns[nCntLogThorns].MinPos.z)	//�O��̈ʒu���Ώۂ̊ۑ��g�Q�� Z�̍ŏ��ʒu�̂�菬��������
	{
		pPos->z = g_LogThorns[nCntLogThorns].MinPos.z;		//����̈ʒu��Ώۂ̊ۑ��g�Q�� Z�̈ʒu�ɂ���
	}
}

//�ۑ��g�Q�ƒe�̓����蔻��
bool ColliLogThornsToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner)
{
	D3DXVECTOR3	fRange;				//�e�̃T�C�Y�Ɗۑ��g�Q�̑傫���̍��v�l

	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//�g���Ă���
		if (g_LogThorns[nCntLogThorns].bUse)
		{
			D3DXVECTOR3 posDiff = g_LogThorns[nCntLogThorns].pos - pos;	//�ۑ��g�Q�ƒe�̋����̍�

			fRange.x = (g_LogThorns[nCntLogThorns].MaxPos.x - g_LogThorns[nCntLogThorns].pos.x) + size;	//�e�̃T�C�Y �� �ۑ��g�Q�̑傫�� �̍��v���Z�o
			fRange.y = (g_LogThorns[nCntLogThorns].MaxPos.y - g_LogThorns[nCntLogThorns].pos.y) + size;	//�e�̃T�C�Y �� �ۑ��g�Q�̑傫�� �̍��v���Z�o
			fRange.z = (g_LogThorns[nCntLogThorns].MaxPos.z - g_LogThorns[nCntLogThorns].pos.z) + size;	//�e�̃T�C�Y �� �ۑ��g�Q�̑傫�� �̍��v���Z�o

																										//�v���C���[�ƒe�Ƃ̋������e�̃T�C�Y���Ɏ��܂�
			if (-fRange.x <= posDiff.x && posDiff.x <= fRange.x &&
				-fRange.y <= posDiff.y && posDiff.y <= fRange.y &&
				-fRange.z <= posDiff.z && posDiff.z <= fRange.z)
			{
				//�q�b�g���Đ�
				PlaySound(SOUND_LABEL_SE_HIT);

				return true;			//��������
			}
		}
	}

	//������Ȃ�����
	return false;
}

//----------------------------------------------------
//					�ۑ��g�Q�̐ݒ菈��
//----------------------------------------------------
void SetLogThorns(D3DXVECTOR3 Setpos, D3DXVECTOR3 Setmove, D3DXVECTOR3 Setrot)
{
	for (int nCntLogThorns = 0; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//�ۑ��g�Q���g���Ă��Ȃ�
		if (g_LogThorns[nCntLogThorns].bUse == false)
		{
			g_LogThorns[nCntLogThorns].pos = Setpos;	//�ʒu�ݒ�
			g_LogThorns[nCntLogThorns].move = Setmove;	//�ړ��ʐݒ�
			g_LogThorns[nCntLogThorns].rot = Setrot;	//�����ݒ�

			g_LogThorns[nCntLogThorns].bUse = true;		//�g��
			break;//�����𔲂���
		}
	}
}