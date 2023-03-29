/*==========================================================================================================================================================

														�����\������[grassland.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "color.h"
#include "grassland.h"

#define NUM_GRASSLAND			(3)
#define NUM_GRASSLAND_TEX		(20)
#define NUM_GRASSLAND_XFILE		(3)
#define GRASSLAND_INFOFILE		"data/CSV/grassland.csv"

//�����̏��\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//����
	int			nIdxGrassLand;	//�g�������̎�ނ̔ԍ�
}GrassLand;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureGrassLand[NUM_GRASSLAND_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH				g_pMeshGrassLand[NUM_GRASSLAND_XFILE];			// ���b�V��(���_���)�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldGrassLand;							// ���[���h�}�g���b�N�X
LPD3DXBUFFER			g_pBuffMatGrassLand[NUM_GRASSLAND_XFILE];		// �}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatGrassLand[NUM_GRASSLAND_XFILE];		// �}�e���A���̐�
GrassLand				g_GrassLand[NUM_GRASSLAND];						// �����̏��

//������X�t�@�C����
const char *c_apGrassLandTexName[NUM_GRASSLAND_XFILE] = {
	"data/MODEL/GrassLands000.x",
	"data/MODEL/GrassLands001.x",
	"data/MODEL/GrassLands002.x",
};

//----------------------------------------------------
//					�����̏���������
//----------------------------------------------------
void InitGrassLand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//�����̏�񏉊���
	LoadGrassLand();

	for (int nCntGrassLand = START_CNT; nCntGrassLand < NUM_GRASSLAND_XFILE; nCntGrassLand++)
	{
		g_pMeshGrassLand[nCntGrassLand] = NULL;				// ���b�V��������
		g_pBuffMatGrassLand[nCntGrassLand] = NULL;			// �}�e���A���ւ̃|�C���^������
		g_dwNumMatGrassLand[nCntGrassLand] = ZERO;			// �}�e���A���̐�������

		// X�t�@�C���ǂݍ���
		D3DXLoadMeshFromX(c_apGrassLandTexName[nCntGrassLand], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatGrassLand[nCntGrassLand], NULL, &g_dwNumMatGrassLand[nCntGrassLand], &g_pMeshGrassLand[nCntGrassLand]);

		//���f���̃e�N�X�`���ݒ�
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatGrassLand[nCntGrassLand]; nCntMat++)
		{
			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatGrassLand[nCntGrassLand]->GetBufferPointer();

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`���t�@�C�������݂���
			 //�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureGrassLand[nCntMat]);
			}
		}
	}
}

//�����̏��ǂݍ��ݏ���
void LoadGrassLand(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(GRASSLAND_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntGrassLand = START_CNT; nCntGrassLand < NUM_GRASSLAND; nCntGrassLand++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			g_GrassLand[nCntGrassLand].pos.x = atof(strtok(&aData[ZERO], ","));		// �J�n�ʒu������
			g_GrassLand[nCntGrassLand].pos.y = atof(strtok(NULL, ","));				// �J�n�ʒu������
			g_GrassLand[nCntGrassLand].pos.z = atof(strtok(NULL, ","));				// �J�n�ʒu������

			g_GrassLand[nCntGrassLand].rot.x = atof(strtok(NULL, ",")) / FIX_FLOAT;	// �J�n����������
			g_GrassLand[nCntGrassLand].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;	// �J�n����������
			g_GrassLand[nCntGrassLand].rot.z = atof(strtok(NULL, ",")) / FIX_FLOAT;	// �J�n����������

			g_GrassLand[nCntGrassLand].nIdxGrassLand = atoi(strtok(NULL, ","));		// �g�p���郂�f���ԍ��ݒ�
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//----------------------------------------------------
//					�����̏I������
//----------------------------------------------------
void UninitGrassLand(void)
{
	for (int nCntGrassLand = START_CNT; nCntGrassLand < NUM_GRASSLAND_XFILE; nCntGrassLand++)
	{
		//���b�V���j��
		if (g_pMeshGrassLand[nCntGrassLand] != NULL)
		{
			g_pMeshGrassLand[nCntGrassLand]->Release();
			g_pMeshGrassLand[nCntGrassLand] = NULL;
		}

		//�}�e���A���j��
		if (g_pBuffMatGrassLand[nCntGrassLand] != NULL)
		{
			g_pBuffMatGrassLand[nCntGrassLand]->Release();
			g_pBuffMatGrassLand[nCntGrassLand] = NULL;
		}
	}

	//�e�N�X�`���j��
	for (int nCntTex = START_CNT; nCntTex < NUM_GRASSLAND_TEX; nCntTex++)
	{
		if (g_pTextureGrassLand[nCntTex] != NULL)
		{
			g_pTextureGrassLand[nCntTex]->Release();
			g_pTextureGrassLand[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------------
//					�����̕`�揈��
//----------------------------------------------------
void DrawGrassLand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntGrassLand = START_CNT; nCntGrassLand < NUM_GRASSLAND; nCntGrassLand++)
	{
		//�����̃��f���ԍ�
		int Idx = g_GrassLand[nCntGrassLand].nIdxGrassLand;

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldGrassLand);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_GrassLand[nCntGrassLand].rot.y, g_GrassLand[nCntGrassLand].rot.x, g_GrassLand[nCntGrassLand].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldGrassLand, &g_mtxWorldGrassLand, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_GrassLand[nCntGrassLand].pos.x, g_GrassLand[nCntGrassLand].pos.y, g_GrassLand[nCntGrassLand].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldGrassLand, &g_mtxWorldGrassLand, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldGrassLand);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)g_pBuffMatGrassLand[Idx]->GetBufferPointer();

		//�}�e���A���ݒ�
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatGrassLand[Idx]; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureGrassLand[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshGrassLand[Idx]->DrawSubset(nCntMat);
		}

		//�}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}