/*==========================================================================================================================================================

���ˑ�̏���[launchpad.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "launchpad.h"
#include "logthorns.h"
#include "player.h"
#include "meshfield.h"
#include "sound.h"
#include "slope.h"

#define LAUNCHPAD_TEX			(10)						// ���ˑ�̃e�N�X�`���̍ő吔
#define LAUNCHPAD_NUM			(4)							// ���ˑ�̑���
#define LAUNCHPAD_XFILE			"data/MODEL/LaunchPad.x"	// ���ˑ�̃t�@�C����
#define LAUNCHPAD_INFOFILE		"data/CSV/launchpad.csv"	// ���ˑ�̏��t�@�C����
#define LAUNCHPAD_WAIT_TIME		(300)						// ���ˑ�̃`���[�W����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureLaunchPad[LAUNCHPAD_TEX] = {};// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH				g_pMeshLaunchPad;						// ���b�V��(���_���)�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldLaunchPad;					// ���[���h�}�g���b�N�X
LPD3DXBUFFER			g_pBuffMatLaunchPad;					// �}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatLaunchPad;					// �}�e���A���̐�
LaunchPad				g_LaunchPad[LAUNCHPAD_TEX];				// ���ˑ�̏��

//----------------------------------------------------
//				���ˑ�̏���������
//----------------------------------------------------
void InitLaunchPad(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//��񏉊���
	LoadLaunchPad();

	g_pMeshLaunchPad = NULL;		// ���b�V��������
	g_pBuffMatLaunchPad = NULL;		// �}�e���A���ւ̃|�C���^������
	g_dwNumMatLaunchPad = NULL;		// �}�e���A���̐�������

									// X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX(LAUNCHPAD_XFILE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatLaunchPad, NULL, &g_dwNumMatLaunchPad, &g_pMeshLaunchPad);

	//���ˑ�̃e�N�X�`���ݒ�
	for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatLaunchPad; nCntMat++)
	{
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatLaunchPad->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureLaunchPad[nCntMat]);
		}
	}
}

//���ǂݍ��ݏ���
void LoadLaunchPad(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(LAUNCHPAD_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntLaunchPad = START_CNT; nCntLaunchPad < LAUNCHPAD_NUM; nCntLaunchPad++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			//****************
			//�ʒu������
			//****************
			g_LaunchPad[nCntLaunchPad].pos.x = atof(strtok(&aData[ZERO], ","));
			g_LaunchPad[nCntLaunchPad].pos.y = atof(strtok(NULL, ","));
			g_LaunchPad[nCntLaunchPad].pos.z = atof(strtok(NULL, ","));

			//****************
			//����������
			//****************
			g_LaunchPad[nCntLaunchPad].rot.x = NIL_F;
			g_LaunchPad[nCntLaunchPad].rot.z = NIL_F;
			g_LaunchPad[nCntLaunchPad].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;
		
			//----------------
			//��Ԋ֘A������
			//----------------
			g_LaunchPad[nCntLaunchPad].state = LAUNCHPADSTATE_WAIT;
			g_LaunchPad[nCntLaunchPad].nCntWait = atoi(strtok(NULL, ","));
			g_LaunchPad[nCntLaunchPad].nCntDelay = atoi(strtok(NULL, ","));

			//****************
			//�ݒ肷��ړ��ʏ�����
			//****************
			float fMove = atof(strtok(NULL, ","));

			g_LaunchPad[nCntLaunchPad].Setmove = D3DXVECTOR3(
				sinf(D3DX_PI - g_LaunchPad[nCntLaunchPad].rot.y) * fMove,		// �ړ��ʏ�����
				atof(strtok(NULL, ",")),										// ���ˎ��̏㏸�ʏ�����
				cosf(D3DX_PI - g_LaunchPad[nCntLaunchPad].rot.y) * fMove);		// �ړ��ʏ�����
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//----------------------------------------------
//				���ˑ�̏I������
//----------------------------------------------
void UninitLaunchPad(void)
{
	//���b�V���j��
	if (g_pMeshLaunchPad != NULL)
	{
		g_pMeshLaunchPad->Release();
		g_pMeshLaunchPad = NULL;
	}

	//�}�e���A���j��
	if (g_pBuffMatLaunchPad != NULL)
	{
		g_pBuffMatLaunchPad->Release();
		g_pBuffMatLaunchPad = NULL;
	}

	//�e�N�X�`���j��
	for (int nCntTex = START_CNT; nCntTex < LAUNCHPAD_TEX; nCntTex++)
	{
		if (g_pTextureLaunchPad[nCntTex] != NULL)
		{
			g_pTextureLaunchPad[nCntTex]->Release();
			g_pTextureLaunchPad[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------
//				���ˑ�̍X�V����
//----------------------------------------------
void UpdateLaunchPad(void)
{
	//�X�V����
	for (int nCntLaunchPad = START_CNT; nCntLaunchPad < LAUNCHPAD_NUM; nCntLaunchPad++)
	{
		switch (g_LaunchPad[nCntLaunchPad].state)
		{
		//----------------------
		//		�ҋ@���
		//----------------------
		case LAUNCHPADSTATE_WAIT:	
			LaunchPad_Wait(nCntLaunchPad);
			break;

		//----------------------
		//		���ˏ��
		//----------------------
		case LAUNCHPADSTATE_FIRE:
			LaunchPad_Fire(nCntLaunchPad);
			break;
		}
	}
}

//�ҋ@����
void LaunchPad_Wait(int nCntLaunchPad)
{
	//�x��������
	if (g_LaunchPad[nCntLaunchPad].nCntDelay > 0)
	{
		//�x�����Ԍ���
		g_LaunchPad[nCntLaunchPad].nCntDelay--;
	}

	//�x�����Ԃ������Ȃ���
	else
	{
		//�ҋ@���Ԍ���
		g_LaunchPad[nCntLaunchPad].nCntWait--;

		//�ҋ@���Ԃ��O�ɂȂ���
		if (g_LaunchPad[nCntLaunchPad].nCntWait <= 0)
		{
			g_LaunchPad[nCntLaunchPad].state = LAUNCHPADSTATE_FIRE;	//���ˏ�Ԃɂ���
		}
	}
}

//���ˏ���
void LaunchPad_Fire(int nCntLaunchPad)
{
	//���˂���
	SetLogThorns(g_LaunchPad[nCntLaunchPad].pos, g_LaunchPad[nCntLaunchPad].Setmove, g_LaunchPad[nCntLaunchPad].rot);

	//�ҋ@��Ԃ�
	g_LaunchPad[nCntLaunchPad].state = LAUNCHPADSTATE_WAIT;		//�ҋ@��Ԃɂ���
	g_LaunchPad[nCntLaunchPad].nCntWait = LAUNCHPAD_WAIT_TIME;	//�ҋ@���Ԑݒ�
}

//----------------------------------------------
//				���ˑ�̕`�揈��
//----------------------------------------------
void DrawLaunchPad(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntLaunchPad = START_CNT; nCntLaunchPad < LAUNCHPAD_NUM; nCntLaunchPad++)
	{
		//�g���Ă���
			//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldLaunchPad);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_LaunchPad[nCntLaunchPad].rot.y, g_LaunchPad[nCntLaunchPad].rot.x, g_LaunchPad[nCntLaunchPad].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldLaunchPad, &g_mtxWorldLaunchPad, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_LaunchPad[nCntLaunchPad].pos.x, g_LaunchPad[nCntLaunchPad].pos.y, g_LaunchPad[nCntLaunchPad].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldLaunchPad, &g_mtxWorldLaunchPad, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldLaunchPad);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)g_pBuffMatLaunchPad->GetBufferPointer();

		//�}�e���A���ݒ�
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatLaunchPad; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureLaunchPad[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_pMeshLaunchPad->DrawSubset(nCntMat);
		}

		//�}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}