/*==========================================================================================================================================================

															�ǂ̏���[wall.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"
#include "wall.h"
#include "camera.h"

//�}�N����`
#define NUM_WALL		(13)							// �ǂ̐�
#define NUM_TEX_WALL	(4)								// �e�N�X�`���̖���
#define WALL_WIDTH		(600.0f)						// ���T�C�Y
#define WALL_HEIGHT		(200.0f)						// �����T�C�Y
#define WALL_DEPTH		(600.0f)						// ���s���T�C�Y
#define TEX_WIDTH		(6.0f)							// �e�N�X�`���̉����𑝂₷
#define TEX_HEIGHT		(1.0f)							// �e�N�X�`���̉����𑝂₷
#define WALL_ROT		(0.01f)							// �ǂ̉�]�l
#define WALL_NOR		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// �ǂ�nor
#define WALL_FILE		"data/CSV/wall.csv"				// �ǂ̏�񂪏����ꂽ�t�@�C��

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureWall[NUM_WALL] = {};	//�e�N�X�`��
D3DXMATRIX				g_mtxWorldWall;					//���[���h�}�g���b�N�X
Wall					g_Wall[NUM_WALL];				//�ǂ̏��

//�ǂ̃e�N�X�`����
const char *c_apWallTextureName[NUM_TEX_WALL] =
{
	"data/TEXTURE/fault000.png",
	"data/TEXTURE/fault001.png",
	"data/TEXTURE/fault002.png",
	"data/TEXTURE/fault003.png",
};

//------------------------------------------------
//				�ǂ̏���������
//------------------------------------------------
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^	

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_WALL, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffWall, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�ǂ̏��ǂݍ���
	LoadWall();

	//�e�N�X�`���ǂݍ���
	for (int nCntWallTex = START_CNT; nCntWallTex < NUM_TEX_WALL; nCntWallTex++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apWallTextureName[nCntWallTex], &g_pTextureWall[nCntWallTex]);	//�e�N�X�`���̓ǂݍ���
	}

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++, pVtx += VTX_MAX)
	{
		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth, g_Wall[nCntWall].fHeight,	+g_Wall[nCntWall].fDepth);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth, g_Wall[nCntWall].fHeight,	-g_Wall[nCntWall].fDepth);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth,	NIL_F,						+g_Wall[nCntWall].fDepth);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth,	NIL_F,						-g_Wall[nCntWall].fDepth);
		}

		//nor�̐ݒ�
		{
			pVtx[VTX_LE_UP].nor = WALL_NOR;
			pVtx[VTX_RI_UP].nor = WALL_NOR;
			pVtx[VTX_LE_DO].nor = WALL_NOR;
			pVtx[VTX_RI_DO].nor = WALL_NOR;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(g_Wall[nCntWall].fTexPos.x,	g_Wall[nCntWall].fTexPos.y);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(g_Wall[nCntWall].fTexWidth,	g_Wall[nCntWall].fTexPos.y);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(g_Wall[nCntWall].fTexPos.x,	g_Wall[nCntWall].fTexHeight);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(g_Wall[nCntWall].fTexWidth,	g_Wall[nCntWall].fTexHeight);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffWall->Unlock();
}

//------------------------------------------------
//				�ǂ̏I������
//------------------------------------------------
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	for (int nCntWall = START_CNT; nCntWall < NUM_TEX_WALL; nCntWall++)
	{
		if (g_pTextureWall[nCntWall] != NULL)
		{
			g_pTextureWall[nCntWall]->Release();
			g_pTextureWall[nCntWall] = NULL;
		}
	}	

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//------------------------------------------------
//				�ǂ̍X�V����
//------------------------------------------------
void UpdateWall(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffWall->Unlock();
}

//------------------------------------------------
//				�ǂ̕`�揈��
//------------------------------------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldWall);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(ZERO, g_pVtxBuffWall, ZERO, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureWall[g_Wall[nCntWall].nTexType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * VTX_MAX, SEC);
	}
}

//�ǂ̓����蔻�菈��
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bCollision = false;		//�����������ǂ����̔���
	bool bInnerProduct = false;		//���ς��s�����ǂ���
	float fAreaS1, fAreaS2;			//���_���W2�E�O��E���݈ʒu�A�̂S�_�Ō`�������l�p�`�𕪒f�������̍��E�̖ʐς��i�[
	D3DXVECTOR3 vtx0, vtx1;			//���_���W�O�E�P�Ԗڂ̈ʒu
	D3DXVECTOR3 vecMove = *pMove;	//�ړ��x�N�g��
	Wall *pWall = &g_Wall[0];		//�ǂ̃|�C���^

	//�|�C���^��ݒ�	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	VERTEX_3D *pVtx;	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++, pVtx += VTX_MAX, pWall++)
	{
		vtx0 = pVtx[VTX_LE_UP].pos + pWall->pos;	//���_���W�O�Ԗڂ̈ʒu
		vtx1 = pVtx[VTX_RI_UP].pos + pWall->pos;	//���_���W�P�Ԗڂ̈ʒu
		D3DXVECTOR3 vPosTovtx0 = vtx0 - *pPos;		//���݈ʒu���璸�_���W�O�Ԗڂ܂ł̃x�N�g��
		D3DXVECTOR3 vVtx1ToPos = *pPos - vtx1;		//���_���W�P�Ԗڂ��猻�݈ʒu�܂ł̃x�N�g��
		D3DXVECTOR3 vecLine = vtx1 - vtx0;			//���_���W�O����P�Ԗڂ܂ł̃x�N�g��

		VectorProduct(vPosTovtx0.x, vecMove.z, vPosTovtx0.z, vecMove.x, &fAreaS1, HALF_MAG);	//�����̖ʐ�
		VectorProduct(vVtx1ToPos.x, vecMove.z, vVtx1ToPos.z, vecMove.x, &fAreaS2, HALF_MAG);	//�E���̖ʐ�

		float fRateLeft = fAreaS1 / (fAreaS1 + fAreaS2);	//�����̖ʐς̊���
		float fRateRight = fAreaS2 / (fAreaS1 + fAreaS2);	//�E���̖ʐς̊���

		D3DXVECTOR3 CrossPoint = D3DXVECTOR3(
			vtx0.x + (vecLine.x * fRateLeft),	//��_�]���W���Z�o
			0.0f,								//Y���W�i��قǍ��W����
			vtx0.z + (vecLine.z * fRateLeft));	//��_�y���W���Z�o

		float vecMoveLength = D3DXVec3Length(&vecMove);				//�O��ƍ���̈ʒu�̋���

		float posOldToCrossLength = sqrtf(powf(pPosOld->x - CrossPoint.x, 2.0f) + powf(pPosOld->z - CrossPoint.z, 2.0f));	//�O��ʒu�ƌ�_�̋���

		float fPercentLength = posOldToCrossLength / vecMoveLength;		//�J���������_�܂ł̋����̊������Z�o
		CrossPoint.y = pPosOld->y - vecMove.y * fPercentLength;			//��_�̍�������

		if (posOldToCrossLength <= vecMoveLength &&					//��_�܂ł̒������A���݈ʒu�܂ł̒����ȉ�
			pWall->pos.y <= CrossPoint.y && CrossPoint.y < vtx0.y)	//��_�x���W���ǂ̒��ɂ���
		{
			float	fvecLineWidth = powf(vtx0.x - vtx1.x, 2.0f),			//���E���x�N�g���̕�
					fvecLineDepth = powf(vtx0.z - vtx1.z, 2.0f),			//���E���x�N�g���̉��䂫
					fvecLineLength = sqrtf(fvecLineWidth + fvecLineDepth);	//���E���x�N�g���̒���

			float	CrossTovtx0_X = powf(vtx0.x - CrossPoint.x, 2.0f),		//���_���W�O�Ԗڂ����_�܂ł̇]�̋���
					CrossTovtx0_Z = powf(vtx0.z - CrossPoint.z, 2.0f),		//				�V			�̂y�̋���
					fCrossS1Length = sqrtf(CrossTovtx0_X + CrossTovtx0_Z);	//				�V			�̋���

			float	CrossTovtx1_X = powf(vtx1.x - CrossPoint.x, 2.0f),		//���_���W�P�Ԗڂ����_�܂ł̇]�̋���
					CrossTovtx1_Z = powf(vtx1.z - CrossPoint.z, 2.0f),		//				�V			�̂y�̋���
					fCrossS2Length = sqrtf(CrossTovtx1_X + CrossTovtx1_Z);	//				�V			�̋���

			if (fCrossS1Length <= fvecLineLength &&	//��_���璸�_���W�O�Ԗڂ܂ł̋������A���E���x�N�g���̋������Ɏ��܂��Ă���
				fCrossS2Length <= fvecLineLength)	//��_���璸�_���W�P�Ԗڂ܂ł̋������A���E���x�N�g���̋������Ɏ��܂��Ă���
			{
				float	vtx0to1_Diff_X = vtx0.x - vtx1.x, vtx0toNow_Diff_X = vtx0.x - pPos->x, vtx0toOld_Diff_X = vtx0.x - pPosOld->x,
						vtx0to1_Diff_Z = vtx0.z - vtx1.z, vtx0toNow_Diff_Z = vtx0.z - pPos->z, vtx0toOld_Diff_Z = vtx0.z - pPosOld->z;

				float fVtx0to1_Rot = atan2f(vtx0to1_Diff_X, vtx0to1_Diff_Z);				//���_�ԍ��O����P�Ԗڂւ̊p�x
				float fVtx0toNowPos_Rot = atan2f(vtx0toNow_Diff_X, vtx0toNow_Diff_Z);		//���_�ԍ��O���猻�݈ʒu�ւ̊p�x
				float fVtx0toPosOld_Rot = atan2f(vtx0toOld_Diff_X, vtx0toOld_Diff_Z);		//���_�ԍ��O����O��ʒu�ւ̊p�x

				//�p�x�̐�Βl�� 3.14 �𒴂��Ă����ꍇ
				if (0.0f >= D3DX_PI - fabsf(fVtx0to1_Rot))
				{
					fVtx0toNowPos_Rot = atan2f(-vtx0toNow_Diff_X, vtx0toNow_Diff_Z);	//���_�ԍ��O���猻�݈ʒu�ւ̊p�x���ĎZ�o
					fVtx0toPosOld_Rot = atan2f(-vtx0toOld_Diff_X, vtx0toOld_Diff_Z);	//���_�ԍ��O����O��ʒu�ւ̊p�x���ĎZ�o

					if (fVtx0toNowPos_Rot <= 0.0f &&
						fVtx0to1_Rot >= fVtx0toPosOld_Rot)
					{
						*pPos = CrossPoint;
						*pMove = ZERO_SET;
						//bCollision = true;		//��������
						bInnerProduct = true;		//���ς��s��
					}
				}

				//�O��  ���_���W�O�Ԗڂ��璸�_���W�P�Ԗڂ��������ɉE���ɂ���
				else if (fVtx0toNowPos_Rot <= fVtx0to1_Rot && 
						fVtx0to1_Rot <= fVtx0toPosOld_Rot)
				{
					*pPos = CrossPoint;
					//*pMove = ZERO_SET;
					//bCollision = true;		//��������
					bInnerProduct = true;		//���ς��s��
				}

				//���ς��s���ꍇ
				if (bInnerProduct)
				{	
					//�ǂ̊p�x���X�O�x�E��]
					fVtx0to1_Rot += RIGHT;

					//���όv�Z
					InnerProduct(fVtx0to1_Rot, vecMove, pMove);
				}
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffWall->Unlock();

	//�����Ԃ�
	return bCollision;
}

//�ǂ̏��
Wall *GetWall(void)
{
	//�ǂ̏��̐擪�A�h���X��Ԃ�
	return &g_Wall[ZERO];
}

//�ǂ̏��ǂݍ���
void LoadWall(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(WALL_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			g_Wall[nCntWall].pos.x = atof(strtok(&aData[ZERO], ","));				// �ʒu������
			g_Wall[nCntWall].pos.y = atof(strtok(NULL, ","));						// �ʒu������
			g_Wall[nCntWall].pos.z = atof(strtok(NULL, ","));						// �ʒu������

			g_Wall[nCntWall].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;			// ����������
			g_Wall[nCntWall].rot.x = NIL_F;											// ����������
			g_Wall[nCntWall].rot.z = NIL_F;											// ����������

			g_Wall[nCntWall].fWidth = atof(strtok(NULL, ","));						// ���̏�����
			g_Wall[nCntWall].fHeight = atof(strtok(NULL, ","));						// �����̏�����
			g_Wall[nCntWall].fDepth = atof(strtok(NULL, ","));						// ���s���̏�����

			g_Wall[nCntWall].fTexPos.x = atof(strtok(NULL, ","));					// �e�N�X�`���̈ʒu������
			g_Wall[nCntWall].fTexPos.y = atof(strtok(NULL, ","));					// �e�N�X�`���̈ʒu������
			g_Wall[nCntWall].fTexWidth = atof(strtok(NULL, ","));					// �e�N�X�`���̕�������
			g_Wall[nCntWall].fTexHeight = atof(strtok(NULL, ","));					// �e�N�X�`���̍���������
			g_Wall[nCntWall].nTexType = atoi(strtok(NULL, ","));					// �e�N�X�`���̎��

			g_Wall[nCntWall].bCollitype = (bool)(atoi(strtok(NULL, ",")));			// �����蔻����s�����ǂ���[ 0:�s��Ȃ� �E 1:�s�� ]
			g_Wall[nCntWall].Direction = (WallDirection)(atoi(strtok(NULL, ",")));	// �ǂ̕���
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//�ǉz���ɃI�u�W�F�N�g��`�悷�邩�ǂ���
bool WallDrawObject(D3DXVECTOR3 pos)
{
	bool bDisp = true;						//�`�悷�邩�ǂ���
	float fAreaS1, fAreaS2;					//���_���W2�E�J�����̈ʒu�E�I�u�W�F�N�g�̈ʒu�A�̂S�_�Ō`�������l�p�`�𕪒f�������̍��E�̖ʐς��i�[
	D3DXVECTOR3 CamPos = GetCamera()->posV;//�J�����̈ʒu
	D3DXVECTOR3 vCamToObj = CamPos - pos;	//������ړ��x�N�g���i�ړ��͂��Ă��Ȃ���
	Wall *pWall = &g_Wall[0];				//�ŏ��̕ǂ̃|�C���^���擾

	//�|�C���^��ݒ�		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	VERTEX_3D *pVtx;		g_pVtxBuffWall->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++, pVtx += VTX_MAX)
	{
		D3DXVECTOR3 vtx0 = pVtx[VTX_LE_UP].pos + pWall->pos;	//�O�Ԗڂ̒��_���W
		D3DXVECTOR3 vtx1 = pVtx[VTX_RI_UP].pos + pWall->pos;	//�P�Ԗڂ̒��_���W

		//�I�u�W�F�N�g�̈ʒu���A�ǂ̒ꂩ��V��܂ł̍����ɂ���
		D3DXVECTOR3 vCamTovtx0 = vtx0 - pos;	//�I�u�W�F�N�g�̈ʒu���璸�_���W�O�Ԗڂ܂ł̃x�N�g��
		D3DXVECTOR3 vVtx1ToCam = pos - vtx1;	//���_���W�P�Ԗڂ���I�u�W�F�N�g�̈ʒu�܂ł̃x�N�g��
		D3DXVECTOR3 vecLine = vtx1 - vtx0;		//���_���W�O����P�Ԗڂ܂ł̃x�N�g��

		VectorProduct(vCamTovtx0.x, vCamToObj.z, vCamTovtx0.z, vCamToObj.x, &fAreaS1, HALF_MAG);	//�����̖ʐ�
		VectorProduct(vVtx1ToCam.x, vCamToObj.z, vVtx1ToCam.z, vCamToObj.x, &fAreaS2, HALF_MAG);	//�E���̖ʐ�

		float fLeftAreaPer = fAreaS1 / (fAreaS1 + fAreaS2);		//�����̖ʐς̊���

		D3DXVECTOR3 CrossPoint = D3DXVECTOR3(
			vtx0.x + (vecLine.x * fLeftAreaPer),	//��_�]���W���Z�o
			0.0f,									//Y���W
			vtx0.z + (vecLine.z * fLeftAreaPer));	//��_�y���W���Z�o

		float CamToObjectLength = sqrtf(powf(CamPos.x - pos.x, 2.0f) + powf(CamPos.z - pos.z, 2.0f));				//�J�����ƃI�u�W�F�N�g�̋���
		float CamToCrossLength = sqrtf(powf(CamPos.x - CrossPoint.x, 2.0f) + powf(CamPos.z - CrossPoint.z, 2.0f));	//�J�����ƌ�_�̋���

		float fPercentLength = CamToCrossLength / CamToObjectLength;		//�J���������_�܂ł̋����̊������Z�o
		float fCrossHeight = CamPos.y - vCamToObj.y * fPercentLength;		//��_�̍������Z�o

		if (CamToCrossLength <= CamToObjectLength &&				//�J���������_�܂ł̋������A�J��������I�u�W�F�N�g�̋����̒��ɂ���
			pWall->pos.y <= fCrossHeight && fCrossHeight <= vtx0.y)	//��_�̂x���W���A�ǂ̒��ɂ���
		{
			float fvecLineWidth = powf(vtx0.x - vtx1.x, 2.0f);				//���E���x�N�g���̕�
			float fvecLineDepth = powf(vtx0.z - vtx1.z, 2.0f);				//���E���x�N�g���̉��䂫
			float fvecLineLength = sqrtf(fvecLineWidth + fvecLineDepth);	//���E���x�N�g���̒���

			float CrossTovtx0_X = powf(vtx0.x - CrossPoint.x, 2.0f);		//���_���W�O�Ԗڂ����_�܂ł̇]�̋���
			float CrossTovtx0_Z = powf(vtx0.z - CrossPoint.z, 2.0f);		//				�V			�̂y�̋���
			float fCrossS1Length = sqrtf(CrossTovtx0_X + CrossTovtx0_Z);	//				�V			�̋���

			float CrossTovtx1_X = powf(vtx1.x - CrossPoint.x, 2.0f);		//���_���W�P�Ԗڂ����_�܂ł̇]�̋���
			float CrossTovtx1_Z = powf(vtx1.z - CrossPoint.z, 2.0f);		//				�V			�̂y�̋���
			float fCrossS2Length = sqrtf(CrossTovtx1_X + CrossTovtx1_Z);	//				�V			�̋���

			if (fCrossS1Length <= fvecLineLength &&	//��_���璸�_���W�O�Ԗڂ܂ł̋������A���E���x�N�g���̋������Ɏ��܂��Ă���
				fCrossS2Length <= fvecLineLength)	//��_���璸�_���W�P�Ԗڂ܂ł̋������A���E���x�N�g���̋������Ɏ��܂��Ă���
			{
				//�`�悵�Ȃ�
				bDisp = false;
				break;
			}
		}

		//���̕ǂփ|�C���^��i�߂�
		pWall++;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffWall->Unlock();

	//�����Ԃ�
	return bDisp;
}