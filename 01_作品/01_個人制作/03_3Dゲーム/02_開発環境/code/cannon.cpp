/*==========================================================================================================================================================

														��C�̏���[cannon.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "cannon.h"
#include "shadow.h"
#include "particle.h"
#include "fade.h"
#include "player.h"
#include "sound.h"

#define CANNON_TEX						(10)								// ��C�̃e�N�X�`���̍ő吔
#define NUM_CANNON						(5)									// ��C�̑���
#define CANNON_XFILE					"data/MODEL/cannon.x"				// ��C��X�t�@�C����
#define CANNON_INFOFILE					"data/CSV/cannon.csv"				// ��C�̏��t�@�C����
#define CANNON_BULLET_COL				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// ��C�̒e�̐F
#define CANNON_BULLET_SIZE				(18.0f)								// ��C�̒e�̃T�C�Y
#define CANNON_BULLET_SPEED				(-4.0f)								// ��C�̒e�̑��x
#define CANNON_BULLET_CORRECT_HEIGHT	(18.0f)								// �e�̔��ˈʒu����
#define CANNON_SEARCH_RANGE				(700.0f)							// �e�𔭎˂���͈�
#define CANNON_CHARGE_TIME				(300)								// ��C�̃`���[�W����
#define CANNON_SHOOT_INTERVAL			(80)								// ���˂��Ă����]����܂ł̃C���^�[�o��
#define CANNON_ROTATE_MOVE				(0.02f)								// ��C�̐��񑬓x

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureCannon[CANNON_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH				g_pMeshCannon;						// ���b�V��(���_���)�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldCannon;					// ���[���h�}�g���b�N�X
LPD3DXBUFFER			g_pBuffMatCannon;					// �}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatCannon;					// �}�e���A���̐�
Cannon					g_Cannon[NUM_CANNON];				// ��C�̏��

//----------------------------------------------------
//					��C�̏���������
//----------------------------------------------------
void InitCannon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//��C�̏�񏉊���
	LoadCannon();

	g_pMeshCannon = NULL;			// ���b�V��������
	g_pBuffMatCannon = NULL;		// �}�e���A���ւ̃|�C���^������
	g_dwNumMatCannon = ZERO;		// �}�e���A���̐�������

	// X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX(CANNON_XFILE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCannon, NULL, &g_dwNumMatCannon, &g_pMeshCannon);

	//��C�̃e�N�X�`���ݒ�
	for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatCannon; nCntMat++)
	{
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatCannon->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureCannon[nCntMat]);
		}
	}

	//�ŏ��E�ő�ʒu�ݒ�
	SearchCannon_MinAndMax_Pos();
}

//��C�̏��ǂݍ��ݏ���
void LoadCannon(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(CANNON_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			g_Cannon[nCntCannon].pos.x = atof(strtok(&aData[ZERO], ","));		// �J�n�ʒu������
			g_Cannon[nCntCannon].pos.y = atof(strtok(NULL, ","));				// �J�n�ʒu������
			g_Cannon[nCntCannon].pos.z = atof(strtok(NULL, ","));				// �J�n�ʒu������

			g_Cannon[nCntCannon].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;	// ����������

			g_Cannon[nCntCannon].bRotate = (bool)atoi(strtok(NULL, ","));		// ��C�̊J�n������������
			g_Cannon[nCntCannon].nStateCounter = ZERO;							// ��ԃJ�E���^�[������
			g_Cannon[nCntCannon].state = CANNONSTATE_NORMAL;					// ��ԏ�����
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//�����蔻��Ɏg�p����ŏ��E�ő�l�ݒ菈��
void SearchCannon_MinAndMax_Pos(void)
{
	int		nNumVtx;		//���_��
	DWORD	dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_�����擾
	nNumVtx = g_pMeshCannon->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshCannon->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshCannon->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

	D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;		//���_���W�̑��

	//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
	pVtxBuff += dwSizeFVF;

	D3DXVECTOR3 MinPos = vtx;						//�ŏ��̈ʒu�����f���̌��_�̈ʒu���狁�߂�
	D3DXVECTOR3 MaxPos = vtx;						//�ő�̈ʒu�����f���̌��_�̈ʒu���狁�߂�

	//�����蔻��Ɏg�����_�̍ŏ��E�ő�ʒu���擾
	for (int nCntVtx = ONE; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ���݊i�[����Ă���ŏ��E�ő�l�𒸓_���W�œh��ւ��邩�ǂ�������
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
	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		g_Cannon[nCntCannon].MinPos = g_Cannon[nCntCannon].pos + MinPos;		//�ŏ��̈ʒu�����f���̌��_�̈ʒu���狁�߂�
		g_Cannon[nCntCannon].MaxPos = g_Cannon[nCntCannon].pos + MaxPos;		//�ő�̈ʒu�����f���̌��_�̈ʒu���狁�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshCannon->UnlockVertexBuffer();
}

//----------------------------------------------------
//					��C�̏I������
//----------------------------------------------------
void UninitCannon(void)
{
	//���b�V���j��
	if (g_pMeshCannon != NULL)
	{
		g_pMeshCannon->Release();
		g_pMeshCannon = NULL;
	}

	//�}�e���A���j��
	if (g_pBuffMatCannon != NULL)
	{
		g_pBuffMatCannon->Release();
		g_pBuffMatCannon = NULL;
	}

	//�e�N�X�`���j��
	for (int nCntTex = START_CNT; nCntTex < CANNON_TEX; nCntTex++)
	{
		if (g_pTextureCannon[nCntTex] != NULL)
		{
			g_pTextureCannon[nCntTex]->Release();
			g_pTextureCannon[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------------
//					��C�̍X�V����
//----------------------------------------------------
void UpdateCannon(void)
{
	//�S��C�̍X�V����
	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		//��ԕ��򏈗�
		SwitchStateCannon(nCntCannon);
	}
}

//��C�̏�ԕ��򏈗�
void SwitchStateCannon(int nCntCannon)
{
	//��ԕ���
	switch (g_Cannon[nCntCannon].state)
	{	
	case CANNONSTATE_NORMAL:		//�ʏ���
	{
		D3DXVECTOR3 PlayerPos = GetPlayer()->pos;	//�v���C���[�̈ʒu
		float PosDiffX, PosDiffZ;					//�v���C���[�Ƒ�C�̋����̍����i�[

		//�v���C���[�Ƃ̋������v�Z
		PosDiffX = pow(g_Cannon[nCntCannon].pos.x - PlayerPos.x, 2.0f);	//2��
		PosDiffZ = pow(g_Cannon[nCntCannon].pos.z - PlayerPos.z, 2.0f);	//2��

		//�������͈͓��Ɏ��܂���
		if (sqrtf(PosDiffX + PosDiffZ) <= CANNON_SEARCH_RANGE)
		{
			//�`���[�W��Ԃɐݒ�
			g_Cannon[nCntCannon].state = CANNONSTATE_RELOADING;
		}
	}
	break;

	case CANNONSTATE_RELOADING:		// ���ˏ�����
	{
		g_Cannon[nCntCannon].nStateCounter++;

		//���˂���̃C���^�[�o�����I����
		if (g_Cannon[nCntCannon].nStateCounter >= CANNON_SHOOT_INTERVAL)
		{
			//��C��]
			RotCannon(nCntCannon);
		}

		//�J�E���^�[���`���[�W���Ԃ𒴂���
		if (g_Cannon[nCntCannon].nStateCounter >= CANNON_CHARGE_TIME)
		{			
			g_Cannon[nCntCannon].state = CANNONSTATE_SHOOTING;	// �e���ˏ�Ԃɂ���
			g_Cannon[nCntCannon].nStateCounter = ZERO;			// ��ԃJ�E���^�[���[���ɂ���
		}
	}
	break;

	case CANNONSTATE_SHOOTING:		// �e����
	{
		//�e�̈ړ������E��
		D3DXVECTOR3 BulletMove;

		BulletMove.x = sinf(g_Cannon[nCntCannon].rot.y) * CANNON_BULLET_SPEED;	//�ړ��ʁE�����v�Z
		BulletMove.z = cosf(g_Cannon[nCntCannon].rot.y) * CANNON_BULLET_SPEED;	//�ړ��ʁE�����v�Z
		BulletMove.y = F_ZERO;													//Y�͕ω��Ȃ�

		//������ɒe����
		SetBullet(D3DXVECTOR3(g_Cannon[nCntCannon].pos.x, g_Cannon[nCntCannon].pos.y + CANNON_BULLET_CORRECT_HEIGHT, g_Cannon[nCntCannon].pos.z), BulletMove, CANNON_BULLET_COL, CANNON_BULLET_SIZE, BULLET_OWNER_CANNON);
		SetBullet(D3DXVECTOR3(g_Cannon[nCntCannon].pos.x, g_Cannon[nCntCannon].pos.y + CANNON_BULLET_CORRECT_HEIGHT, g_Cannon[nCntCannon].pos.z), -BulletMove, CANNON_BULLET_COL, CANNON_BULLET_SIZE, BULLET_OWNER_CANNON);
		
		//���ʁE��������ς���
		g_Cannon[nCntCannon].bRotate = g_Cannon[nCntCannon].bRotate ? false : true;

		//��Ԃ�߂�
		g_Cannon[nCntCannon].state = CANNONSTATE_NORMAL;
	}
	break;

	}
}

//----------------------------------------------------
//					��C�̕`�揈��
//----------------------------------------------------
void DrawCannon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldCannon);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Cannon[nCntCannon].rot.y, g_Cannon[nCntCannon].rot.x, g_Cannon[nCntCannon].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldCannon, &g_mtxWorldCannon, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Cannon[nCntCannon].pos.x, g_Cannon[nCntCannon].pos.y, g_Cannon[nCntCannon].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldCannon, &g_mtxWorldCannon, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCannon);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)g_pBuffMatCannon->GetBufferPointer();

		//�}�e���A���ݒ�
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatCannon; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureCannon[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshCannon->DrawSubset(nCntMat);
		}

		//�}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//��C�̉�]����
void RotCannon(int nCntCannon)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//��C�����ʂ������Ă���
	if (g_Cannon[nCntCannon].bRotate == false)
	{
		//��C���������ɉ�]
		g_Cannon[nCntCannon].rot.y += CANNON_ROTATE_MOVE;

		//��]���������𒴂��Ă��܂���
		if (g_Cannon[nCntCannon].rot.y >= RIGHT)
		{//���ɖ߂�
			g_Cannon[nCntCannon].rot.y = RIGHT;
		}

		else
		{
			//�v���C���[����ɍڂ��Ă���
			if (g_Cannon[nCntCannon].MaxPos.y == pPlayer->pos.y)
			{//�v���C���[����]
				pPlayer->rot.y += CANNON_ROTATE_MOVE;
			}
		}
	}

	//��C�����������Ă���
	else	
	{
		//��C�𐳖ʕ����ɉ�]
		g_Cannon[nCntCannon].rot.y -= CANNON_ROTATE_MOVE;

		//��]�����ʂ𒴂��Ă��܂���
		if (g_Cannon[nCntCannon].rot.y <= F_ZERO)
		{//���ʂɖ߂�
			g_Cannon[nCntCannon].rot.y = F_ZERO;
		}

		else
		{
			//�v���C���[����ɍڂ��Ă���
			if (g_Cannon[nCntCannon].MaxPos.y == pPlayer->pos.y)
			{//�v���C���[����]
				pPlayer->rot.y -= CANNON_ROTATE_MOVE;
			}
		}
	}
}

//��C�̓����蔻��
bool CollisionCannon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType, bool *pLand, bool *pJump)
{
	bool bCollision = false;		//�����������ǂ����̔���
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾

	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		// X�̍ŏ��l�ƍő�l�͈͓̔��ɂ���
		if (g_Cannon[nCntCannon].MinPos.x <= pPos->x && pPos->x <= g_Cannon[nCntCannon].MaxPos.x &&
			g_Cannon[nCntCannon].MinPos.y <= pPos->y && pPos->y <  g_Cannon[nCntCannon].MaxPos.y &&
			g_Cannon[nCntCannon].MinPos.z <= pPos->z && pPos->z <= g_Cannon[nCntCannon].MaxPos.z )
		{
			//�����������ǂ�������
			ColliCannon_Max(pPos, pPosold, nCntCannon, nType, pLand, pJump);	// �ő�l�̓����蔻��
			ColliCannon_Min(pPos, pPosold, nCntCannon, nType);					// �ŏ��l�̓����蔻��

			//��������
			bCollision = true;

			//����������for���𔲂���
			break;
		}
	}

	//�����Ԃ�
	return bCollision;
}

//�ő�l���g���������蔻��
void ColliCannon_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntCannon, COLLITYPE nType, bool *pLand, bool *pJump)
{
	if (g_Cannon[nCntCannon].MaxPos.x <= pPosold->x)	//�O��̈ʒu���Ώۂ̑�C�� X�̍ŏ��ʒu�̂��傫������
	{
		pPos->x = g_Cannon[nCntCannon].MaxPos.x;		//����̈ʒu��Ώۂ̑�C�� X�̈ʒu�ɂ���
	}

	if (g_Cannon[nCntCannon].MaxPos.y <= pPosold->y)	//�O��̈ʒu���Ώۂ̑�C�� Z�̍ŏ��ʒu�̂��傫������
	{
		pPos->y = g_Cannon[nCntCannon].MaxPos.y;		//����̈ʒu��Ώۂ̑�C�� Z�̈ʒu�ɂ���
		*pLand = true;									//����Ă���
		*pJump = false;									//�W�����v���Ă��Ȃ�
	}

	if (g_Cannon[nCntCannon].MaxPos.z <= pPosold->z)	//�O��̈ʒu���Ώۂ̑�C�� Z�̍ŏ��ʒu�̂��傫������
	{
		pPos->z = g_Cannon[nCntCannon].MaxPos.z;		//����̈ʒu��Ώۂ̑�C�� Z�̈ʒu�ɂ���
	}
}

//�ŏ��l���g���������蔻��
void ColliCannon_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntCannon, COLLITYPE nType)
{
	if (pPosold->x <= g_Cannon[nCntCannon].MinPos.x)	//�O��̈ʒu���Ώۂ̑�C�� X�̍ŏ��ʒu�̂�菬��������
	{
		pPos->x = g_Cannon[nCntCannon].MinPos.x;		//����̈ʒu��Ώۂ̑�C�� X�̈ʒu�ɂ���
	}

	if (pPosold->y <= g_Cannon[nCntCannon].MinPos.y)	//�O��̈ʒu���Ώۂ̑�C�� Z�̍ŏ��ʒu�̂�菬��������
	{
		pPos->y = g_Cannon[nCntCannon].MinPos.y;		//����̈ʒu��Ώۂ̑�C�� Z�̈ʒu�ɂ���
	}

	if (pPosold->z <= g_Cannon[nCntCannon].MinPos.z)	//�O��̈ʒu���Ώۂ̑�C�� Z�̍ŏ��ʒu�̂�菬��������
	{
		pPos->z = g_Cannon[nCntCannon].MinPos.z;		//����̈ʒu��Ώۂ̑�C�� Z�̈ʒu�ɂ���
	}
}

//��C�̏�ɉe��`�悷��
void ColliCannonShadow(int nIdxShadow, D3DXVECTOR3 pos, float *pDrawPosY)
{
	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		if (g_Cannon[nCntCannon].MinPos.x < pos.x && pos.x <  g_Cannon[nCntCannon].MaxPos.x &&		// X���W���̍ŏ��l�ƍő�l�͈͓̔��ɂ���
			g_Cannon[nCntCannon].MinPos.y < pos.y && pos.y <= g_Cannon[nCntCannon].MaxPos.y &&		// Y���W���̍ŏ��l�ƍő�l�͈͓̔��ɂ���
			g_Cannon[nCntCannon].MinPos.z < pos.z && pos.z <  g_Cannon[nCntCannon].MaxPos.z &&		// Z���W���̍ŏ��l�ƍő�l�͈͓̔��ɂ���
			*pDrawPosY <= g_Cannon[nCntCannon].MaxPos.y)											// �C������ʒu���Ώۂ̑�C�̍ő�Y���W��艺�ɂ���
		{
			//�C���ʒu����
			*pDrawPosY = g_Cannon[nCntCannon].MaxPos.y;
		}
	}
}

//��C�ƒe�̓����蔻��
bool ColliCannonToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner)
{
	bool bCollision = false;		//���������ǂ����̔���
	D3DXVECTOR3	fRange;				//�e�̃T�C�Y�Ƒ�C�̑傫���̍��v�l

	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		D3DXVECTOR3 posDiff = g_Cannon[nCntCannon].pos - pos;	//��C�ƒe�̋����̍�
		fRange.x = (g_Cannon[nCntCannon].MaxPos.x - g_Cannon[nCntCannon].pos.x) + size;	//�e�̃T�C�Y �� ��C�̑傫�� �̍��v���Z�o
		fRange.y = (g_Cannon[nCntCannon].MaxPos.y - g_Cannon[nCntCannon].pos.y) + size;	//�e�̃T�C�Y �� ��C�̑傫�� �̍��v���Z�o
		fRange.z = (g_Cannon[nCntCannon].MaxPos.z - g_Cannon[nCntCannon].pos.z) + size;	//�e�̃T�C�Y �� ��C�̑傫�� �̍��v���Z�o

		//�v���C���[�ƒe�Ƃ̋������e�̃T�C�Y���Ɏ��܂�
		if (-fRange.x <= posDiff.x && posDiff.x <= fRange.x &&
			-fRange.y <= posDiff.y && posDiff.y <= fRange.y &&
			-fRange.z <= posDiff.z && posDiff.z <= fRange.z &&
			owner != BULLET_OWNER_CANNON)						//�v���C���[���_���[�W��Ԃł͂Ȃ�
		{
			//�q�b�g���Đ�
			PlaySound(SOUND_LABEL_SE_HIT);

			bCollision = true;			//��������
		}
	}

	//�����������ǂ����̔����Ԃ�
	return bCollision;
}