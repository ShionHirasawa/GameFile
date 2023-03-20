/*==========================================================================================================================================================

														���f������[model.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "model.h"
#include "shadow.h"
#include "particle.h"
#include "fade.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "meshfield.h"
#include "explosion.h"
#include "score.h"

#define MODEL_TEX			(10)		// ���f���̃e�N�X�`���̍ő吔
#define NUM_MODEL			(24)		// ���f���̑���
#define MODEL_LIFE			(3)			// ���f���̎���
#define FLASH_COUNTER_MODEL	(3)			// �_�ŃJ�E���^
#define MODEL_GRAVITY		(-2.0f)		// ���f���̏d��
#define APPEAR_COUNTER		(60)		// �o���J�E���^
#define DAMEGE_COUNTER		(40)		// �_���[�W�J�E���^
#define MODEL_FILE			"data/CSV/model.csv"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureModel[MODEL_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH				g_pMeshModel[MODELTYPE_MAX];		// ���b�V��(���_���)�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldModel;					// ���[���h�}�g���b�N�X
LPD3DXBUFFER			g_pBuffMatModel[MODELTYPE_MAX];		// �}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatModel[MODELTYPE_MAX];		// �}�e���A���̐�
Model					g_Model[NUM_MODEL];					// ���f���̏��
D3DXVECTOR3				g_MaxPosModel[MODELTYPE_MAX],		// ���f���̎�ޕʁ@�ő���W�l
						g_MinPosModel[MODELTYPE_MAX];		// ���f���̎�ޕʁ@�ŏ����W�l

#define ENDGAMECOUNT	(60)
VanishModel				g_EndGameModel;

// X�t�@�C����
const char *c_apFileNameModel[MODELTYPE_MAX] = 
{
	"data/MODEL/�ؔ�.x",					//�ؔ��̃t�@�C����
	"data/MODEL/�ؔ��Q.x",					//�ؔ��Q�̃t�@�C����
	"data/MODEL/hana_chan.x",				//�n�i�����̃t�@�C����
};

//----------------------------------------------------
//				���f���̏���������
//----------------------------------------------------
void InitModel(void)
{
	//���f���̏����ʒu�E���C�t�ǂݍ���
	LoadModel();

	//X�t�@�C���̏����i�[����ϐ��̏�����
	for (int nCntMesh = 0; nCntMesh < MODELTYPE_MAX; nCntMesh++)
	{
		g_pMeshModel[nCntMesh] = NULL;		// ���b�V��������
		g_pBuffMatModel[nCntMesh] = NULL;	// �}�e���A���ւ̃|�C���^������
		g_dwNumMatModel[nCntMesh] = NULL;	// �}�e���A���̐�������
	}

	//�ŏ��E�ő�ʒu�ݒ�
	SearchModel_MinAndMax_Pos();

	//���f���̏�񏉊���
	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{		
		g_Model[nCntModel].rot				= ZERO_SET;						// �ʒu������
		g_Model[nCntModel].PosOld			= g_Model[nCntModel].pos;		// �O��̈ʒu������
		g_Model[nCntModel].move				= ZERO_SET;						// �ړ��ʏ�����
		g_Model[nCntModel].nIdxShadow		= SetShadow();					// �e�̐ݒ�
		g_Model[nCntModel].nStateCounter	= ZERO;							// ��ԃJ�E���^������
		g_Model[nCntModel].state			= MODELSTATE_NORMAL;			// ��ԏ�����
		g_Model[nCntModel].bUse				= true;							// �g���Ă����Ԃ�
		g_Model[nCntModel].bDisp			= true;							// �\�����邩�ǂ���������
		g_Model[nCntModel].rot				= ZERO_SET;						// ����������
		SetModel_Max_Min_Pos(nCntModel);									// ���f���̍ŏ��E�ő�ʒu�ݒ�
	}

	g_EndGameModel.nDeadModelCount = ZERO;		// �Q�[���I���܂ł̃J�E���g
	g_EndGameModel.nCount = ENDGAMECOUNT;		// �Q�[���I���܂ł̃J�E���g
}

//���f���̏��ǂݍ��ݏ���
void LoadModel(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(MODEL_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			g_Model[nCntModel].pos.x = atof(strtok(&aData[ZERO], ","));	// �J�n�ʒu������
			g_Model[nCntModel].pos.y = atof(strtok(NULL, ","));			// �J�n�ʒu������
			g_Model[nCntModel].pos.z = atof(strtok(NULL, ","));			// �J�n�ʒu������

			g_Model[nCntModel].nLife = atoi(strtok(NULL, ","));			// ���C�t�̏�����
			g_Model[nCntModel].nPoint = atoi(strtok(NULL, ","));		// �|�C���g������
			g_Model[nCntModel].type = (MODELTYPE)atoi(strtok(NULL,","));// ��ނ̏�����
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//�����蔻��Ɏg�p����ŏ��E�ő�l�ݒ菈��
void SearchModel_MinAndMax_Pos(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	for (int nCntLoadModel = 0; nCntLoadModel < MODELTYPE_MAX; nCntLoadModel++)
	{
		// X�t�@�C���ǂݍ���
		D3DXLoadMeshFromX(c_apFileNameModel[nCntLoadModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatModel[nCntLoadModel], NULL, &g_dwNumMatModel[nCntLoadModel], &g_pMeshModel[nCntLoadModel]);

		//���f���̃e�N�X�`���ݒ�
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatModel[nCntLoadModel]; nCntMat++)
		{
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatModel[nCntLoadModel]->GetBufferPointer();

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`���t�@�C�������݂���
			 //�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureModel[nCntMat]);
			}
		}

		int		nNumVtx;		//���_��
		DWORD	dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
		BYTE	*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

		//���_�����擾
		nNumVtx = g_pMeshModel[nCntLoadModel]->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel[nCntLoadModel]->GetFVF());

		//���_�o�b�t�@�����b�N
		g_pMeshModel[nCntLoadModel]->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

		D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;		//���_���W�̑��

		g_MinPosModel[nCntLoadModel] = vtx;		//�ŏ��̈ʒu�����f���̌��_�̈ʒu���狁�߂�
		g_MaxPosModel[nCntLoadModel] = vtx;		//�ő�̈ʒu�����f���̌��_�̈ʒu���狁�߂�

		//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
		pVtxBuff += dwSizeFVF;

		vtx = *(D3DXVECTOR3 *)pVtxBuff;		//���_���W�̑��

		//�����蔻��Ɏg�����_�̍ŏ��E�ő�ʒu���擾
		for (int nCntVtx = ONE; nCntVtx < nNumVtx; nCntVtx++)
		{
			// ���݊i�[����Ă���ŏ��E�ő�l�𒸓_���W�œh��ւ��邩�ǂ�������
			//------------------------------
			//			X�̔���
			//------------------------------
			// �ŏ��l����
			if (vtx.x <= g_MinPosModel[nCntLoadModel].x)
			{
				g_MinPosModel[nCntLoadModel].x = vtx.x;		//�ŏ��l���
			}

			// �ő�l����
			else if (g_MaxPosModel[nCntLoadModel].x <= vtx.x)
			{
				g_MaxPosModel[nCntLoadModel].x = vtx.x;		//�ő�l���
			}

			//------------------------------
			//			Y�̔���
			//------------------------------
			// �ŏ��l����
			if (vtx.y <= g_MinPosModel[nCntLoadModel].y)
			{
				g_MinPosModel[nCntLoadModel].y = vtx.y;		//�ŏ��l���
			}

			// �ő�l����
			else if (g_MaxPosModel[nCntLoadModel].y <= vtx.y)
			{
				g_MaxPosModel[nCntLoadModel].y = vtx.y;		//�ő�l���
			}

			//------------------------------
			//			Z�̔���
			//------------------------------
			// �ŏ��l����
			if (vtx.z <= g_MinPosModel[nCntLoadModel].z)
			{
				g_MinPosModel[nCntLoadModel].z = vtx.z;		//�ŏ��l���
			}

			// �ő�l����
			else if (g_MaxPosModel[nCntLoadModel].z <= vtx.z)
			{
				g_MaxPosModel[nCntLoadModel].z = vtx.z;		//�ő�l���
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
			pVtxBuff += dwSizeFVF;

			vtx = *(D3DXVECTOR3 *)pVtxBuff;		//���_���W�̑��
		}

		//���_�o�b�t�@���A�����b�N
		g_pMeshModel[nCntLoadModel]->UnlockVertexBuffer();
	}
}

//----------------------------------------------------
//					���f���̏I������
//----------------------------------------------------
void UninitModel(void)
{
	//���b�V���ƃ}�e���A���̔j��
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//���b�V���j��
		if (g_pMeshModel[nCntModel] != NULL)
		{
			g_pMeshModel[nCntModel]->Release();
			g_pMeshModel[nCntModel] = NULL;
		}

		//�}�e���A���j��
		if (g_pBuffMatModel[nCntModel] != NULL)
		{
			g_pBuffMatModel[nCntModel]->Release();
			g_pBuffMatModel[nCntModel] = NULL;
		}
	}

	for (int nCntTex = START_CNT; nCntTex < MODEL_TEX; nCntTex++)
	{
		//�e�N�X�`���j��
		if (g_pTextureModel[nCntTex] != NULL)
		{
			g_pTextureModel[nCntTex]->Release();
			g_pTextureModel[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------------
//					���f���̍X�V����
//----------------------------------------------------
void UpdateModel(void)
{
	//�S���f���̍X�V����
	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//��ԕ��򏈗�
		SwitchStateModel(nCntModel);

		//���f�����g���Ă���
		if (g_Model[nCntModel].bUse == true)
		{
			//�O��̈ʒu�X�V
			g_Model[nCntModel].PosOld = g_Model[nCntModel].pos;

			//�\������Ă���
			if (g_Model[nCntModel].bDisp == true)
			{
				g_Model[nCntModel].move.y = MODEL_GRAVITY;		//���f���̏d�͐ݒ�

				//���f���̉e�̑傫�����v�Z
				float ShadowWidth = g_Model[nCntModel].MinPos.x - g_Model[nCntModel].MaxPos.x;
				float ShadowDepth = g_Model[nCntModel].MinPos.z - g_Model[nCntModel].MaxPos.z;

				//�e�̕`��ʒu�ݒ�
				SetPositionShadow(g_Model[nCntModel].nIdxShadow, g_Model[nCntModel].pos, ShadowWidth, ShadowDepth);
			}

			//�\������Ă��Ȃ�
			else
			{
				//�e������
				BreakShadow(g_Model[nCntModel].nIdxShadow);
			}

			//���f���̈ړ�
			g_Model[nCntModel].pos += g_Model[nCntModel].move;

			MeshField *Null;	//��������
			ColliMeshField(&g_Model[nCntModel].pos, &g_Model[nCntModel].PosOld, &g_Model[nCntModel].move, &Null);

			// ���f���̍ŏ��E�ő�ʒu�ݒ�
			SetModel_Max_Min_Pos(nCntModel);

			//���f���̗��������ǂ����̏���
			FallModel(nCntModel);

			//���f�����m�̓����蔻��
			ColliModel_to_Model(nCntModel);
		}
	}

	//�������Ă��郂�f���̐��v��
	DeadCountModel();
}

//���f���̏�ԕ��򏈗�
void SwitchStateModel(int nCntModel)
{
	//��ԕ���
	switch (g_Model[nCntModel].state)
	{
		//�ʏ���
	case MODELSTATE_NORMAL:
	{

	}
		break;

		//�o�����
	case MODELSTATE_APPEAR:
	{
		//��ԃJ�E���^�����炷
		g_Model[nCntModel].nStateCounter--;

		//�J�E���^�[���[���ɂȂ���
		if (g_Model[nCntModel].nStateCounter <= ZERO)
		{
			g_Model[nCntModel].state = MODELSTATE_NORMAL;	// �ʏ��Ԃ�
			g_Model[nCntModel].bDisp = true;				// �\������
		}
		else
		{
			//�_�ł�����
			if (g_Model[nCntModel].nStateCounter % FLASH_COUNTER_MODEL == ZERO)
			{
				g_Model[nCntModel].bDisp = g_Model[nCntModel].bDisp ? false : true;
			}
		}
	}
		break;

		//�_���[�W���
	case MODELSTATE_DAMAGE:
	{
		//��ԃJ�E���^�����炷
		g_Model[nCntModel].nStateCounter--;

		//�J�E���^�[���[���ɂȂ���
		if (g_Model[nCntModel].nStateCounter <= ZERO)
		{
			g_Model[nCntModel].state = MODELSTATE_NORMAL;	// �ʏ��Ԃ�
			g_Model[nCntModel].bDisp = true;				// �\������
		}
		else
		{
			//�_�ł�����
			if (g_Model[nCntModel].nStateCounter % FLASH_COUNTER_MODEL == ZERO)
			{
				g_Model[nCntModel].bDisp = g_Model[nCntModel].bDisp ? false : true;
			}
		}
	}
	break;

		//�����Ă�����
	case MODELSTATE_NONE:
	{

	}
		break;
	}
}

//�����Ă��郂�f���̏���
void FallModel(int nCntModel)
{
	// �t�B�[���h�̘g�𒴂���
	if (g_Model[nCntModel].pos.x < -MESHFIELD_SIZE || MESHFIELD_SIZE < g_Model[nCntModel].pos.x ||
		g_Model[nCntModel].pos.z < -MESHFIELD_SIZE || MESHFIELD_SIZE < g_Model[nCntModel].pos.z ||
		g_Model[nCntModel].pos.y < NIL_F)
	{
		//�e������
		BreakShadow(g_Model[nCntModel].nIdxShadow);
	}

	//���f���̈ʒu���ޗ��ɗ����Ă��܂���
	if (g_Model[nCntModel].pos.y <= THE_END_HELL)
	{
		//���f�����g���Ȃ�����
		g_Model[nCntModel].bUse = false;
		g_Model[nCntModel].bDisp = false;
		SetExplosion(g_Model[nCntModel].pos, XCOL_WHITE);	//��������
	}
}

//�����Ă��郂�f���̐��v������
void DeadCountModel(void) 
{
	g_EndGameModel.nDeadModelCount = ZERO;

	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//�\������Ă��Ȃ��āA�g���Ă����Ȃ��āA�����Ă����Ԃɐݒ肳��Ă���
		if (g_Model[nCntModel].bDisp == false && g_Model[nCntModel].bUse == false && g_Model[nCntModel].state == MODELSTATE_NONE)
		{
			//�������f���̐������Z����
			g_EndGameModel.nDeadModelCount++;

			if (g_EndGameModel.nDeadModelCount > NUM_MODEL)
			{
				g_EndGameModel.nDeadModelCount = NUM_MODEL;
			}
		}
	}

	//�����Ă��郂�f���̐����A�S�̂̃��f���̐��ȏ�ɂȂ���
	if (g_EndGameModel.nDeadModelCount >= NUM_MODEL)
	{
		//�Q�[���I���̃J�E���g�ݒ�
		g_EndGameModel.nCount--;

		if (g_EndGameModel.nCount <= ZERO)
		{
			//�Q�[���N���A
			SetEndGame(EndGAME_CLEAR);

			//���U���g��ʂɈڍs
			SetFade(MODE_RESULT);
		}
	}
}

//----------------------------------------------------
//					���f���̕`�揈��
//----------------------------------------------------
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXCOLOR pMatStrage;						//�}�e���A���f�[�^�ۑ��p

	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//�g�p����Ă���@�����@�\������Ă���
		if (g_Model[nCntModel].bUse == true && g_Model[nCntModel].bDisp == true)
		{
			int type = g_Model[nCntModel].type;

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldModel);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model[nCntModel].rot.y, g_Model[nCntModel].rot.x, g_Model[nCntModel].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Model[nCntModel].pos.x, g_Model[nCntModel].pos.y, g_Model[nCntModel].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_pBuffMatModel[type]->GetBufferPointer();

			for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatModel[type]; nCntMat++)
			{
				//�ۑ��p�X�g���[�W�ɑ��
				pMatStrage = pMat[nCntMat].MatD3D.Diffuse;

				//�_���[�W��Ԃł���
				if (g_Model[nCntModel].state == MODELSTATE_DAMAGE)
				{
					//�ԐF�ɐݒ�
					pMat[nCntMat].MatD3D.Diffuse = XCOL_RED;
				}

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(ZERO, g_pTextureModel[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_pMeshModel[type]->DrawSubset(nCntMat);

				//�F��߂�
				pMat[nCntMat].MatD3D.Diffuse = pMatStrage;
			}

			//�}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//���f�����m�̓����蔻��
void ColliModel_to_Model(int nCntTarget)
{
	/*------------------  ���̏����̒��g  ------------------

	�Ώۂ̃��f���Ƃ��̑��̃��f���̈ʒu���r���A
	��r����郂�f�����A�Ώۂ̃��f����
		�u�O��͂߂荞��ł��Ȃ���	���݂߂荞��ł���v
	���ɁA��r����郂�f�����@�Ώۂ̃��f���̂߂荞��ł��Ȃ��ʒu�ɒ���
	�������s���B
		��{�I�ɂx���W��߂��@���ヂ�f�������ړ�����Ȃ�A
	�w�E�y�̓����蔻����s��										*/

	//��r����鑤�̔ԍ�		Comparison  =  ��r
	for (int nCntComparison = 0; nCntComparison < NUM_MODEL; nCntComparison++)
	{
		if (nCntTarget != nCntComparison)			//�Ώۂ̃��f���ԍ��Ɣ�r����郂�f���ԍ����Ⴄ
		{
			if(g_Model[nCntTarget].bUse == true && g_Model[nCntComparison].bUse == true)	//�����̃��f�����g���Ă���
			{
				D3DXVECTOR3 TargetMinPos = g_Model[nCntTarget].MinPos;	//�Ώۂ̍ŏ��̈ʒu
				D3DXVECTOR3 TargetMaxPos = g_Model[nCntTarget].MaxPos;	//�Ώۂ̍ő�̈ʒu

				if ((TargetMinPos.x < g_Model[nCntComparison].pos.x && g_Model[nCntComparison].pos.x < TargetMaxPos.x) &&
					(TargetMinPos.z < g_Model[nCntComparison].pos.z && g_Model[nCntComparison].pos.z < TargetMaxPos.z))
				{
					if (TargetMaxPos.y <= g_Model[nCntComparison].PosOld.y &&	//�O��͑Ώۂ̒���ʒu�ȏ�̍����ɂ���
						TargetMaxPos.y > g_Model[nCntComparison].pos.y)			//���݂͒���ʒu��艺�ɂ���
					{
						g_Model[nCntComparison].pos.y = TargetMaxPos.y;			//�Ώۂ̒���ʒu�ɍ��킹��
						g_Model[nCntComparison].move.y = NIL_F;					//�d�͂��O��
						break;
					}
				}
			}
		}
	}
}

//���f���̓����蔻��
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType)
{
	bool bCollision = false;		//�����������ǂ����̔���
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾

	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//���f�����g���Ă���	����	�ʏ��Ԃł���
		if (g_Model[nCntModel].bUse == true)
		{
			// X�̍ŏ��l�ƍő�l�͈͓̔��ɂ���
			if (g_Model[nCntModel].MinPos.x <= pPos->x && pPos->x <= g_Model[nCntModel].MaxPos.x &&
				g_Model[nCntModel].MinPos.y <= pPos->y && pPos->y <= g_Model[nCntModel].MaxPos.y &&
				g_Model[nCntModel].MinPos.z <= pPos->z && pPos->z <= g_Model[nCntModel].MaxPos.z)
			{
				//�����������ǂ�������
				ColliModel_Max(pPos, pPosold, nCntModel, nType);	// �ő�l�̓����蔻��
				ColliModel_Min(pPos, pPosold, nCntModel, nType);	// �ŏ��l�̓����蔻��

				//�����������̂��e�������@���@���f���̏�Ԃ��ʏ��Ԃł���
				if (nType == COLLITYPE_BULLET && g_Model[nCntModel].state == MODELSTATE_NORMAL)
				{
					// �̗͂����炷
					g_Model[nCntModel].nLife--;

					//�̗͂��s����
					if (g_Model[nCntModel].nLife <= ZERO)
					{
						g_Model[nCntModel].bUse = false;											// �g�p����Ă��Ȃ���Ԃ�
						g_Model[nCntModel].bDisp = false;											// �\������Ă��Ȃ���Ԃ�
						g_Model[nCntModel].state = MODELSTATE_NONE;									// ��������Ԃ�
						BreakShadow(g_Model[nCntModel].nIdxShadow);									// �e������
						SetParticle(g_Model[nCntModel].pos, SET_PARTICLE_SLOW_EX, MODEL_DEAD_COL);	// �p�[�e�B�N��������蔭��
						AddScore(g_Model[nCntModel].nPoint);										// �X�R�A���Z
					}

					//�̗͂��܂�����
					else
					{
						g_Model[nCntModel].state = MODELSTATE_DAMAGE;		// �_���[�W��Ԃɐݒ�
						g_Model[nCntModel].nStateCounter = DAMEGE_COUNTER;	// �_���[�W�J�E���^�ݒ�
					}

					//�q�b�g���Đ�
					PlaySound(SOUND_LABEL_SE_HIT);
				}

				//�����������̂��v���C���[�������@����  �Ώۂ̃��f�����n�i����񂾂���			�@ ���@�v���C���[���_���[�W��Ԃł͂Ȃ�
				else if (nType == COLLITYPE_PLAYER && g_Model[nCntModel].type == MODELTYPE_HANACHAN && pPlayer->state != PLAYERSTATE_DAMEGE)
				{
					//�v���C���[�̗̑͂����炷
					pPlayer->nLife--;

					//�̗͂��s���Ă��܂���
					if (pPlayer->nLife <= ZERO)
					{
						//���S��Ԃ�
						pPlayer->state = PLAYERSTATE_DEAD;

						//�Q�[���I�[�o�[
						SetEndGame(EndGAME_OVER);
					}

					//�܂��̗͂�����
					else
					{
						//�_���[�W��Ԃ�
						pPlayer->state = PLAYERSTATE_DAMEGE;
					}

					//�q�b�g���Đ�
					PlaySound(SOUND_LABEL_SE_HIT);
				}

				//��������
				bCollision = true;

				//��������������for���𔲂���
				break;
			}
		}
	}

	//�����Ԃ�
	return bCollision;
}

//�ő�l���g���������蔻��
bool ColliModel_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType)
{	
	bool bCollision = false;	//�����������ǂ����̔���

	if (g_Model[nCntModel].MaxPos.x <= pPosold->x)		//�O��̈ʒu���Ώۂ̃��f���� X�̍ŏ��ʒu�̂��傫������
	{
		pPos->x = g_Model[nCntModel].MaxPos.x;			//����̈ʒu��Ώۂ̃��f���� X�̈ʒu�ɂ���
		bCollision = true;								//��������
	}

	if (g_Model[nCntModel].MaxPos.y <= pPosold->y)		//�O��̈ʒu���Ώۂ̃��f���� Z�̍ŏ��ʒu�̂��傫������
	{
		pPos->y = g_Model[nCntModel].MaxPos.y;			//����̈ʒu��Ώۂ̃��f���� Z�̈ʒu�ɂ���
		bCollision = true;								//��������
	}


	if (g_Model[nCntModel].MaxPos.z <= pPosold->z)		//�O��̈ʒu���Ώۂ̃��f���� Z�̍ŏ��ʒu�̂��傫������
	{
		pPos->z = g_Model[nCntModel].MaxPos.z;			//����̈ʒu��Ώۂ̃��f���� Z�̈ʒu�ɂ���
		bCollision = true;								//��������
	}

	//�����Ԃ�
	return bCollision;
}

//�ŏ��l���g���������蔻��
bool ColliModel_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType)
{
	bool bCollision = false;	//�����������ǂ����̔���

	if (pPosold->x <= g_Model[nCntModel].MinPos.x)		//�O��̈ʒu���Ώۂ̃��f���� X�̍ŏ��ʒu�̂�菬��������
	{
		pPos->x = g_Model[nCntModel].MinPos.x;			//����̈ʒu��Ώۂ̃��f���� X�̈ʒu�ɂ���
		bCollision = true;								//��������
	}

	if (pPosold->y <= g_Model[nCntModel].MinPos.y)		//�O��̈ʒu���Ώۂ̃��f���� Z�̍ŏ��ʒu�̂�菬��������
	{
		pPos->y = g_Model[nCntModel].MinPos.y;			//����̈ʒu��Ώۂ̃��f���� Z�̈ʒu�ɂ���
		bCollision = true;								//��������
	}


	if (pPosold->z <= g_Model[nCntModel].MinPos.z)		//�O��̈ʒu���Ώۂ̃��f���� Z�̍ŏ��ʒu�̂�菬��������
	{
		pPos->z = g_Model[nCntModel].MinPos.z;			//����̈ʒu��Ώۂ̃��f���� Z�̈ʒu�ɂ���
		bCollision = true;								//��������
	}

	//�����Ԃ�
	return bCollision;	
}

//���f���̏�ɉe��`�悷��
void ColliModelShadow(int nIdxShadow, D3DXVECTOR3 pos, float *pDrawPosY)
{
	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//���f�����g���Ă���	AND		���̃��f���̉e�̔ԍ��ƕ`�悷��e�̔ԍ����Ⴄ
		if (g_Model[nCntModel].bUse == true && g_Model[nCntModel].nIdxShadow != nIdxShadow)
		{
			if (g_Model[nCntModel].MinPos.x < pos.x && pos.x < g_Model[nCntModel].MaxPos.x &&		// X���W���̍ŏ��l�ƍő�l�͈͓̔��ɂ���
				g_Model[nCntModel].MinPos.y < pos.y && pos.y <= g_Model[nCntModel].MaxPos.y &&		// Y���W���̍ŏ��l�ƍő�l�͈͓̔��ɂ���
				g_Model[nCntModel].MinPos.z < pos.z && pos.z < g_Model[nCntModel].MaxPos.z &&		// Z���W���̍ŏ��l�ƍő�l�͈͓̔��ɂ���
				*pDrawPosY <= g_Model[nCntModel].MaxPos.y)											// �C������ʒu���Ώۂ̃��f���̍ő�Y���W��艺�ɂ���
			{
				//�C���ʒu����
				*pDrawPosY = g_Model[nCntModel].MaxPos.y;
			}
		}
	}

}

//���f���̏��擾
Model *GetModel(void)
{
	//�擪�̃A�h���X��Ԃ�
	return &g_Model[MODELTYPE_WOODEN_BLOCK];
}

//�������Ă��郂�f���̏��擾
VanishModel *GetEndModel(void)
{
	return &g_EndGameModel;
}

//���f���̍ő�E�ŏ��ʒu��ݒ�
void SetModel_Max_Min_Pos(int nCntModel)
{
	int type = g_Model[nCntModel].type;		//�@�������팸�̂��߁A��ނ��i�[

	g_Model[nCntModel].MaxPos = g_Model[nCntModel].pos + g_MaxPosModel[type];	// �ő�ʒu���
	g_Model[nCntModel].MinPos = g_Model[nCntModel].pos + g_MinPosModel[type];	// �ŏ��ʒu���
	g_Model[nCntModel].MinPos.y = g_Model[nCntModel].pos.y;						// �ŏ��x���W�͂����̂ŁA���݈ʒu����
}