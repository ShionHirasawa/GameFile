/*==========================================================================================================================================================

															���C�h����[light.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "light.h"
#include "color.h"

//�}�N����`
#define NUM_LIGHT		(3)		//���C�g�̐�
#define LIGHT_INFOFILE	"data/CSV/light.csv"

//�O���[�o���ϐ�
D3DXVECTOR3	g_LightVec[NUM_LIGHT];	//���C�g�̃x�N�g��
D3DLIGHT9	g_light[NUM_LIGHT];		//���C�g�̏��

//------------------------------------------------
//				���C�g�̏���������
//------------------------------------------------
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXVECTOR3 vecDir[NUM_LIGHT];				//�ݒ�p�����x�N�g��

	//���C�g�̃x�N�g���ǂݍ���
	LoadLightVec();

	//���C�g�̏����N���A����
	for (int nCntLight = START_CNT; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//���C�g�̕������
		vecDir[nCntLight] = g_LightVec[nCntLight];

		//�������[������
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		g_light[nCntLight].Diffuse = XCOL_WHITE;

		//���C�g�̕�����ݒ�
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		//�x�N�g���𐳋K������
		g_light[nCntLight].Direction = vecDir[nCntLight];

		//���C�g��ݒ�
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//���C�g�̌����ǂݍ���
void LoadLightVec(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(LIGHT_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);		//�ŏ��̃Z���^�C�g���ǂݍ���

		for (int nCntLight = START_CNT; nCntLight < NUM_LIGHT; nCntLight++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			g_LightVec[nCntLight].x = atof(strtok(&aData[ZERO], ","));		// �x�N�g��������
			g_LightVec[nCntLight].y = atof(strtok(NULL, ","));				// �x�N�g��������
			g_LightVec[nCntLight].z = atof(strtok(NULL, ","));				// �x�N�g��������
		}

		//�t�@�C�������
		fclose(pFile);
	}

}

//------------------------------------------------
//				���C�g�̏I������
//------------------------------------------------
void UninitLight(void)
{

}

//------------------------------------------------
//				���C�g�̍X�V����
//------------------------------------------------
void UpdateLight(void)
{

}