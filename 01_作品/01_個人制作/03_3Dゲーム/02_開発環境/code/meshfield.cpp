/*==========================================================================================================================================================

													���b�V���t�B�[���h����[meshfield.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "meshfield.h"

#define NUM_MESHFIELD				(4)						//���b�V���t�B�[���h�̐�
#define NUM_MESHFIELD_TEX			(3)						//���b�V���t�B�[���h�̃e�N�X�`����
#define MESHFIELD_DIVISION_WIDTH	(3)						//���̕�����
#define MESHFIELD_DIVISION_DEPTH	(3)						//���s���̕�����
#define MESHFIELD_CAVEIN_HEIGHT		(1.0f)					//�����蔻��Ɏg���߂荞�ސ[��
#define MESHFIELD_FILE				"data/CSV/meshfield.csv"//���b�V���t�B�[���h�̏�񂪓����Ă���t�@�C����

#define MESHFIELD_VERTEX			(9)						//���b�V���t�B�[���h�̒��_��
#define MESHFIELD_INDEX_VTX			(14)					//�C���f�b�N�X�o�b�t�@�̐�
#define SEPALATE_MESHFIELD			(12)					//���b�V���t�B�[���h�̕�����

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;						//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureMeshField[NUM_MESHFIELD_TEX] = {};	//�e�N�X�`��
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;						//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldMeshField;							//���[���h�}�g���b�N�X
MeshField				g_MeshField[NUM_MESHFIELD];						//���b�V���t�B�[���h�̏��

//�n�ʂ̃e�N�X�`�����܂Ƃ�
const char *c_apMeshFieldTexName[NUM_MESHFIELD_TEX] = {
	"data/TEXTURE/cross_section.png",			//�n�ʂ̃e�N�X�`����
	"data/TEXTURE/grassland000.png",			//�����F�����������̃e�N�X�`����
	"data/TEXTURE/grassland001.png",			//�����̃e�N�X�`����
};

//------------------------------------------------
//		���b�V���t�B�[���h�̏���������
//------------------------------------------------
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//���b�V���t�B�[���h�̏��ǂݍ���
	LoadMeshField();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX * NUM_MESHFIELD, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshField, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = START_CNT; nCntTex < NUM_MESHFIELD_TEX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apMeshFieldTexName[nCntTex], &g_pTextureMeshField[nCntTex]);
	}

	//���ꂼ��̃��b�V���t�B�[���h�̒��_���̐ݒ�
	for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++, pVtx += MESHFIELD_VERTEX)
	{
		//���_���̐ݒ�
		for (int nCntVtx = START_CNT; nCntVtx < MESHFIELD_VERTEX; nCntVtx++)
		{
			//���_���W�̐ݒ�
			pVtx[nCntVtx].pos.x = -g_MeshField[nCntMeshField].fWidth * (ONE - (nCntVtx % MESHFIELD_DIVISION_WIDTH));	//�J�E���^�[�� "���̕������Ŋ������]��" ���g���ĉ��̍��W������o��
			pVtx[nCntVtx].pos.z = +g_MeshField[nCntMeshField].fDepth * (ONE - (nCntVtx / MESHFIELD_DIVISION_WIDTH));	//�J�E���^�[�� "���̕������Ŋ���������" ���g���ĉ��̍��W������o��
			pVtx[nCntVtx].pos.y = NIL_F;																				//���ʂȂ̂ŁAY���W�͕ς�炸

			//nor �̐ݒ�
			pVtx[nCntVtx].nor = SET_NOR;

			//col �̐ݒ�
			pVtx[nCntVtx].col = RGBA_WHITE;

			//�e�N�X�`�����W�̍��W
			pVtx[nCntVtx].tex.x = g_MeshField[nCntMeshField].fTexPos.x + g_MeshField[nCntMeshField].fTexWidth * (nCntVtx % MESHFIELD_DIVISION_WIDTH);		//�J�E���^�[�� "���̕������Ŋ������]��" ���g���� ��ʒu���牡�̍��W������o��
			pVtx[nCntVtx].tex.y = g_MeshField[nCntMeshField].fTexPos.y + g_MeshField[nCntMeshField].fTexHeight * (nCntVtx / MESHFIELD_DIVISION_DEPTH);		//�J�E���^�[�� "���̕������Ŋ���������" ���g���� ��ʒu���牜�̍��W������o��
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffMeshField->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_VTX * NUM_MESHFIELD, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffMeshField, NULL);

	//�C���f�b�N�X���̃o�b�t�@
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(ZERO, ZERO, (void* *)&pIdx, ZERO);

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	for (int nCntIdx = START_CNT; nCntIdx < NUM_MESHFIELD; nCntIdx++, pIdx += MESHFIELD_INDEX_VTX)
	{
		int nNumVtx = ZERO;		//���_�ԍ�

		for (int mCntIdx = START_CNT; mCntIdx < MESHFIELD_INDEX_VTX; mCntIdx++)
		{
			//�k�ރ|���S���ɂȂ�Ȃ��A�ʏ�̒��_
			if (mCntIdx != 6 && mCntIdx != 7)
			{
				//�J�E���^�[�������̏ꍇ
				if (mCntIdx % EVENPARITY == NOPARITY)
				{
					//�C���f�b�N�X�ԍ��ݒ�
					pIdx[mCntIdx] = nNumVtx + MESHFIELD_DIVISION_WIDTH;		//�C���f�b�N�X�ԍ� = ��̒��_�ԍ� + ���̕�����
				}

				//�J�E���^�[����̏ꍇ
				else if (mCntIdx % EVENPARITY == ODDPARITY)
				{
					//�C���f�b�N�X�ԍ��ݒ�
					pIdx[mCntIdx] = nNumVtx;								//�C���f�b�N�X�ԍ� = ��̒��_�ԍ�

					//��̒��_�ԍ����Z
					nNumVtx++;
				}
			}

			//�k�ރ|���S���̍ŏ��̏d�����_�͂ЂƂO�̃C���f�b�N�X�ԍ��Ɠ���
			else if (mCntIdx == 6)
			{
				pIdx[mCntIdx] = pIdx[nCntIdx - ONE];
			}

			//�k�ރ|���S���̍Ō�̏d�����_�͎��̃C���f�b�N�X�ԍ��Ɠ���
			else if (mCntIdx == 7)
			{
				pIdx[mCntIdx] = pIdx[nCntIdx + ONE];
			}
		}
	}

	//�C���f�b�N�X�o�b�t�@�����b�N����
	g_pIdxBuffMeshField->Unlock();
}

//���b�V���t�B�[���h�̏��ǂݍ���
void LoadMeshField(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(MESHFIELD_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			//�z�u�ꏊ�̏�����
			g_MeshField[nCntMeshField].pos.x = atof(strtok(&aData[ZERO], ","));	// �ʒu������
			g_MeshField[nCntMeshField].pos.y = atof(strtok(NULL, ","));			// �ʒu������
			g_MeshField[nCntMeshField].pos.z = atof(strtok(NULL, ","));			// �ʒu������

			g_MeshField[nCntMeshField].rot = ZERO_SET;							// ����������

			g_MeshField[nCntMeshField].fWidth = atof(strtok(NULL, ","));		// ���̏�����
			g_MeshField[nCntMeshField].fDepth = atof(strtok(NULL, ","));		// ���s���̏�����

			g_MeshField[nCntMeshField].fTexPos.x = atof(strtok(NULL, ","));		// �e�N�X�`���̈ʒu������
			g_MeshField[nCntMeshField].fTexPos.y = atof(strtok(NULL, ","));		// �e�N�X�`���̈ʒu������
			g_MeshField[nCntMeshField].fTexWidth = atof(strtok(NULL, ","));		// �e�N�X�`���̕�������
			g_MeshField[nCntMeshField].fTexHeight = atof(strtok(NULL, ","));	// �e�N�X�`���̍���������
			g_MeshField[nCntMeshField].nTexType = atoi(strtok(NULL, ","));		// �e�N�X�`���̃^�C�v
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//------------------------------------------------
//		���b�V���t�B�[���h�̏I������
//------------------------------------------------
void UninitMeshField(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = START_CNT; nCntTex < NUM_MESHFIELD_TEX; nCntTex++)
	{
		if (g_pTextureMeshField[nCntTex] != NULL)
		{
			g_pTextureMeshField[nCntTex]->Release();
			g_pTextureMeshField[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//------------------------------------------------
//		���b�V���t�B�[���h�̍X�V����
//------------------------------------------------
void UpdateMeshField(void)
{

}

//------------------------------------------------
//		���b�V���t�B�[���h�̕`�揈��
//------------------------------------------------
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldMeshField);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField[nCntMeshField].rot.y, g_MeshField[nCntMeshField].rot.x, g_MeshField[nCntMeshField].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_MeshField[nCntMeshField].pos.x, g_MeshField[nCntMeshField].pos.y, g_MeshField[nCntMeshField].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(ZERO, g_pVtxBuffMeshField, ZERO, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffMeshField);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureMeshField[g_MeshField[nCntMeshField].nTexType]);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, nCntMeshField * MESHFIELD_VERTEX, ZERO, MESHFIELD_VERTEX * NUM_MESHFIELD, ZERO, SEPALATE_MESHFIELD);
	}
}

//���b�V���t�B�[���h�̏��擾
MeshField *GetMeshField(void)
{
	return &g_MeshField[0];
}

//���b�V���t�B�[���h�Ƃ̓����蔻��
bool ColliMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, MeshField **pField)
{
	for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++)
	{
		//�O��͑Ώۂ̃��b�V���t�B�[���h����ɋ��āA���͉��ɂ߂荞��ł��܂��Ă���
		if (g_MeshField[nCntMeshField].pos.x - g_MeshField[nCntMeshField].fWidth <= pPos->x &&	//���[���E�ɂ���
			g_MeshField[nCntMeshField].pos.x + g_MeshField[nCntMeshField].fWidth >= pPos->x &&	//�E�[��荶�ɂ���
			g_MeshField[nCntMeshField].pos.z - g_MeshField[nCntMeshField].fDepth <= pPos->z &&	//��O��艜�ɂ���
			g_MeshField[nCntMeshField].pos.z + g_MeshField[nCntMeshField].fDepth >= pPos->z &&	//�������O�ɂ���
			g_MeshField[nCntMeshField].pos.y - MESHFIELD_CAVEIN_HEIGHT <= pPosOld->y &&			//�߂荞�݈ʒu�ȏ�ł�����
			g_MeshField[nCntMeshField].pos.y >= pPos->y)				//�t�B�[���h�ʒu�ȏ�ł�����
		{
			//���b�V���t�B�[���h��Y���W�̈ʒu�ɖ߂�
			pPos->y = g_MeshField[nCntMeshField].pos.y;

			//Y���̈ړ��ʂ��[���ɂ���
			pMove->y = F_ZERO;

			//���b�V���t�B�[���h�̏���n��
			*pField = &g_MeshField[nCntMeshField];

			//���n����
			return true;
		}
	}

	//�ǂ̃t�B�[���h�ɂ�����Ă��Ȃ��̂Ń|�C���^����������
	pField = NULL;

	//���n�����Ԃ�
	return false;
}

//���b�V���t�B�[���h�ɉe��`�悷��
void ColliFieldShadow(D3DXVECTOR3 Pos, float *pDrawPosY)
{
	//���ׂẴt�B�[���h�Ŕ���
	for (int nCntMeshField = START_CNT; nCntMeshField < NUM_MESHFIELD; nCntMeshField++)
	{
		if (g_MeshField[nCntMeshField].pos.x - g_MeshField[nCntMeshField].fWidth <= Pos.x &&	//���[���E�ɂ���
			g_MeshField[nCntMeshField].pos.x + g_MeshField[nCntMeshField].fWidth >= Pos.x &&	//�E�[��荶�ɂ���
			g_MeshField[nCntMeshField].pos.z - g_MeshField[nCntMeshField].fDepth <= Pos.z &&	//��O��艜�ɂ���
			g_MeshField[nCntMeshField].pos.z + g_MeshField[nCntMeshField].fDepth >= Pos.z &&	//�������O�ɂ���
			g_MeshField[nCntMeshField].pos.y <= Pos.y)											//�O��͏�ɋ��āA���܂͉��ɂ߂荞��ł�
		{
			//�C������ʒu���Ώۂ̃��b�V���t�B�[���h��艺�ɂ��鎞�A���̃��b�V���t�B�[���h�̈ʒu���C���ʒu�Ƃ���
			if (*pDrawPosY <= g_MeshField[nCntMeshField].pos.y)
			{
				//�C���ʒu���
				*pDrawPosY = g_MeshField[nCntMeshField].pos.y;
			}
		}
	}
}