//================================================================================================
//
//�S�e�N�X�`���Ǘ�����[texture.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "manager.h"
#include "texture.h"

//�ÓI�����o�ϐ�
/*		�e�N�X�`���̑���	*/ int CTexture::m_nNumAll = 0;
/*	�e�N�X�`���̃p�X��ۑ�	*/ char *CTexture::m_pTexPathStrage[MAX_TEXTURE] = {};

//=======================================
//�R���X�g���N�^
//=======================================
CTexture::CTexture()
{
	m_nNumAll = 0;

	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		m_apTexture[nCntTex] = NULL;
		m_pTexPathStrage[nCntTex] = NULL;
	}
}

//=======================================
//�f�X�g���N�^
//=======================================
CTexture::~CTexture()
{

}

//=======================================
//�e�N�X�`���j������
//=======================================
void CTexture::UnLoad(void)
{
	//�S�e�N�X�`����j��
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		//���������g���Ă�����A�j������
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();//�j������
			m_apTexture[nCntTex] = NULL;	//NULL�|�C���^��
		}

		//���������g���Ă�����A�j������
		if (m_pTexPathStrage[nCntTex] != NULL)
		{
			delete m_pTexPathStrage[nCntTex];
			m_pTexPathStrage[nCntTex] = NULL;
		}
	}

	//�����N���A
	m_nNumAll = 0;
}

//=======================================
//����̃e�N�X�`���ǂݍ���
//=======================================
int CTexture::Regist(char *pFileName)
{
	//�p�X������
	if (pFileName == NULL) return ERROR_ID;

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���ԍ�
	int nIdxTex = 0;

	//���łɓo�^���ꂽ�e�N�X�`���Ɠ������̂��Ȃ����m�F
	for (nIdxTex = 0; nIdxTex < m_nNumAll; nIdxTex++)
	{
		//�w�肳�ꂽ�t�@�C�����Ɗ��ɓo�^����Ă���t�@�C��������v����
		if (strcmp(pFileName, m_pTexPathStrage[nIdxTex]) == 0)
		{
			//���ɓo�^����Ă���e�N�X�`���ԍ���Ԃ�
			return nIdxTex;
		}
	}

	//==============================================
	//�V�K�e�N�X�`���o�^
	//==============================================
	if (/*�������ő吔����*/	m_nNumAll < MAX_TEXTURE &&
		/*�A�h���X�����g�p*/	m_apTexture[nIdxTex] == NULL &&
		/*�ǂݍ��݂ɐ�������*/	D3DXCreateTextureFromFile(pDevice, pFileName, &m_apTexture[nIdxTex]) == D3D_OK)
	{
		//�����𑝂₷
		m_nNumAll++;

		//���������������g���Ă�����A�j������
		if (m_pTexPathStrage[nIdxTex] != NULL)
		{
			delete m_pTexPathStrage[nIdxTex];
			m_pTexPathStrage[nIdxTex] = NULL;
		}

		//�t�@�C���̃p�X�̒��� + �I�[����  ���̃��������m�ۂ���
		m_pTexPathStrage[nIdxTex] = new char[(int)strlen(pFileName) + 1];

		//�e�N�X�`���̃p�X��ۑ�
		strcpy(m_pTexPathStrage[nIdxTex], pFileName);

		// �V�K�e�N�X�`���ԍ���Ԃ�
		return nIdxTex;
	}

	//�V�K�o�^���s�ŃG���[�ԍ���Ԃ�
	return ERROR_ID;
}

//=======================================
//�e�N�X�`�����擾
//=======================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(const int nIdx)
{
	//�z��͈̔͊O���A������葽���ԍ����w�肳�ꂽ
	if (nIdx <= ERROR_ID || nIdx >= m_nNumAll)
	{
		//����Ȃ��̂͂Ȃ��̂ŁANULL��Ԃ�
		return NULL;
	}

	//�������w�肳�ꂽ�e�N�X�`����Ԃ�
	return m_apTexture[nIdx];
}