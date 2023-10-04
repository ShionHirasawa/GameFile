//================================================================================================
//
//���C�h����[light.cpp]
//Author:���V����
//
//================================================================================================
#include "../Main/manager.h"
#include "object3D.h"
#include "light.h"

//�}�N����`
//���C�g�̐ݒ���t�@�C��
#define LIGHT_INFOFILE	"data\\CSV\\light.csv"

//=======================================
//�R���X�g���N�^
//=======================================
CLight::CLight()
{
	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		m_LightVec[nCntLight] = VEC3_INIT;
	}
}

//=======================================
//�f�X�g���N�^
//=======================================
CLight::~CLight()
{

}

//=======================================
//���C�g�̏���������
//=======================================
HRESULT CLight::Init(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���C�g�̃x�N�g���ǂݍ���
	LoadVec();

	//���C�g�̏����N���A����
	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//�������[������
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		m_light[nCntLight].Diffuse = GetColor(COL_WHITE);

		//���C�g�̕�����ݒ�
		D3DXVec3Normalize(&m_LightVec[nCntLight], &m_LightVec[nCntLight]);		//�x�N�g���𐳋K������
		m_light[nCntLight].Direction = m_LightVec[nCntLight];

		//���C�g��ݒ�
		if (FAILED(pDevice->SetLight(nCntLight, &m_light[nCntLight])))
		{
			//�ݒ莸�s
			return E_FAIL;
		}

		//���C�g��L���ɂ���
		if (FAILED(pDevice->LightEnable(nCntLight, TRUE)))
		{
			//���s
			return E_FAIL;
		}
	}

	//����������
	return S_OK;
}

//=======================================
//���C�g�̌����ǂݍ���
//=======================================
void CLight::LoadVec(void)
{
	FILE *pFile;
	char aData[MAX_STR];		//�ǂݍ��񂾕������i�[

	pFile = fopen(LIGHT_INFOFILE, "r");

	//�t�@�C�����J����
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);		//�P�s�ڂɏ����ꂽ�A�e��̃^�C�g����ǂݎ��

		for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
		{
			fscanf(pFile, "%s", &aData[0]);	//��s�ǂݍ���

			m_LightVec[nCntLight].x = (float)atof(strtok(&aData[0], CSV_DELIMITER));// �x�N�g���ǂݍ���
			m_LightVec[nCntLight].y = (float)atof(strtok(NULL, CSV_DELIMITER));		// �x�N�g���ǂݍ���
			m_LightVec[nCntLight].z = (float)atof(strtok(NULL, CSV_DELIMITER));		// �x�N�g���ǂݍ���
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//=======================================
//���C�g�̏I������
//=======================================
void CLight::Uninit(void)
{

}

//=======================================
//���C�g�̍X�V����
//=======================================
void CLight::Update(void)
{

}