//================================================================================================
//
//�I�u�W�F�N�g����[object.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/camera.h"
#include "object.h"

#define DEF_OBJ_PRIORITY	(3)		//��̗D�揇��
#define MAX_OBJ_PRIORITY	(8)		//�ō��̗D�揇��

//�ÓI�����o�ϐ��錾
CObject *CObject::m_pTop[MAX_PRIORITY] = {};	//�擪�̃|�C���^
CObject *CObject::m_pCur[MAX_PRIORITY] = {};	//�Ō���̃|�C���^

const int CObject::OBJECT_PRIORITY_DEF = DEF_OBJ_PRIORITY;
const int CObject::OBJECT_PRIORIRY_MAX = MAX_OBJ_PRIORITY;
int		 CObject::m_nNumAll[MAX_PRIORITY] = {};

//=======================================
//�R���X�g���N�^
//=======================================
CObject::CObject(int nPriority)
{
	/*�����̗D�揇�ʂ�ۑ�*/	m_nPriority = nPriority;
	/*     ��ރN���A     */	m_Type = TYPE_NONE;
	/* �����J�E���g�A�b�v */	m_nNumAll[nPriority]++;
	/*�O�̃|�C���^��NULL��*/	m_pPrev = NULL;
	/*���̃|�C���^��NULL��*/	m_pNext = NULL;
	/*  ���S�t���O��OFF�� */	m_bDeath = false;

	//�擪���܂��ݒ肳��Ă��Ȃ�
	if (m_pTop[nPriority] == NULL)
	{
		//������������
		m_pTop[nPriority] = this;
	}

	//�Ō�����ݒ肳��Ă���
	if (m_pCur[nPriority] != NULL)
	{
		//���݂̍Ō����ۑ�
		CObject *pCurLast = m_pCur[nPriority];

		//���݂̍Ō���̎��̃|�C���^�Ɏ�����������
		m_pCur[nPriority]->m_pNext = this;

		//�������܂ł̍Ō�����A�ŐV�̍Ō���̑O�ɐݒ�
		this->m_pPrev = pCurLast;
	}

	//�Ō���������ɂ���
	m_pCur[nPriority] = this;
}

//=======================================
//�f�X�g���N�^
//=======================================
CObject::~CObject()
{

}

//=======================================
//�S�폜
//=======================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//�ŏ��ɍs���I�u�W�F�N�g��������
		CObject *pObj = m_pTop[nCntPriority];

		//�������s���I�u�W�F�N�g�����Ȃ��Ȃ�܂Ŗ������[�v
		while (pObj != NULL)
		{
			//���ɏ������s���I�u�W�F�N�g��ۑ����Ă���
			CObject *pObjNext = pObj->m_pNext;

			//���S�t���O�𗧂Ă�
			pObj->m_bDeath = true;

			//�����̏I������
			pObj->Uninit();

			//���ɏ������s���I�u�W�F�N�g��������
			pObj = pObjNext;
		}
	}
}

//=======================================
//����̎�ނ̃I�u�W�F�N�g�S�폜
//=======================================
void CObject::ReleaseAll(CObject::TYPE type)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//�ŏ��ɍs���I�u�W�F�N�g��������
		CObject *pObj = m_pTop[nCntPriority];

		//�������s���I�u�W�F�N�g�����Ȃ��Ȃ�܂Ŗ������[�v
		while (pObj != NULL)
		{
			//���ɏ������s���I�u�W�F�N�g��ۑ����Ă���
			CObject *pObjNext = pObj->m_pNext;

			//��������������ނ̃I�u�W�F�N�g���ǂ���
			if (pObj->m_Type == type)
			{
				pObj->m_bDeath = true;	//���S�t���O�𗧂Ă�
				pObj->Uninit();			//�����̏I������
			}

			//���ɏ������s���I�u�W�F�N�g��������
			pObj = pObjNext;
		}
	}
}

//=======================================
//����̎�ވȊO�̃I�u�W�F�N�g�S�폜
//=======================================
void CObject::ExceptReleaseAll(CObject::TYPE type)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//�ŏ��ɍs���I�u�W�F�N�g��������
		CObject *pObj = m_pTop[nCntPriority];

		//�������s���I�u�W�F�N�g�����Ȃ��Ȃ�܂Ŗ������[�v
		while (pObj != NULL)
		{
			//���ɏ������s���I�u�W�F�N�g��ۑ����Ă���
			CObject *pObjNext = pObj->m_pNext;

			//�ی삳���I�u�W�F�N�g�ł͂Ȃ�
			if (pObj->m_Type != type)
			{
				pObj->m_bDeath = true;	//���S�t���O�𗧂Ă�
				pObj->Uninit();			//�����̏I������
			}

			//���ɏ������s���I�u�W�F�N�g��������
			pObj = pObjNext;
		}
	}
}

//=======================================
//�S�X�V
//=======================================
void CObject::UpdateAll(void)
{
	//�D�揇�ʂ��Ⴂ���ɍX�V���Ă���
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//�ŏ��ɍs���I�u�W�F�N�g��������
		CObject *pObj = m_pTop[nCntPriority];

		//�������s���I�u�W�F�N�g�����Ȃ��Ȃ�܂Ŗ������[�v
		while (pObj != NULL)
		{
			//���ɏ������s���I�u�W�F�N�g��ۑ����Ă���
			CObject *pObjNext = pObj->m_pNext;

			//�����Ɏ��S�t���O�������Ă��Ȃ�
			if (!pObj->m_bDeath)
			{
				//�����̍X�V����
				pObj->Update();
			}

			//���ɏ������s���I�u�W�F�N�g��������
			pObj = pObjNext;
		}
	}

	//�D�揇�ʂ��Ⴂ���ɔj�����Ă���
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//�ŏ��ɍs���I�u�W�F�N�g��������
		CObject *pObj = m_pTop[nCntPriority];

		//�������s���I�u�W�F�N�g�����Ȃ��Ȃ�܂Ŗ������[�v
		while (pObj != NULL)
		{
			//���ɏ������s���I�u�W�F�N�g��ۑ����Ă���
			CObject *pObjNext = pObj->m_pNext;

			//�����̎��S�t���O�������Ă���
			if (pObj->m_bDeath)
			{
				//�I������
				pObj->Uninit();
			}

			//���ɏ������s���I�u�W�F�N�g��������
			pObj = pObjNext;
		}
	}
}

//=======================================
//�S�`��
//=======================================
void CObject::DrawAll(void)
{
	//�J�����擾
	CCamera *pCamera = CManager::GetCamera();

	//�J������ݒ�
	pCamera->Set();

	//�D�揇�ʂ��Ⴂ���ɕ`�悵�Ă���
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//�ŏ��ɍs���I�u�W�F�N�g��������
		CObject *pObj = m_pTop[nCntPriority];

		//�������s���I�u�W�F�N�g�����Ȃ��Ȃ�܂Ŗ������[�v
		while (pObj != NULL)
		{
			//�����̍X�V����
			pObj->Draw();

			//���ɏ������s���I�u�W�F�N�g��������
			pObj = pObj->m_pNext;
		}
	}
}

//=======================================
//�폜
//=======================================
void CObject::Release(void)
{
	//�܂������̎��S�t���O�������Ă��Ȃ�
	if (m_bDeath == false)
	{
		//���S�t���O�𗧂ĂďI��
		m_bDeath = true;
		return;
	}

	//���S�t���O�������Ă���Δj������
	//�O��̃I�u�W�F�N�g�̃|�C���^��ۑ�
	CObject *pPrev = m_pPrev;
	CObject *pNext = m_pNext;

	//�����̔ԍ���ۑ�
	int nPriority = m_nPriority;

	//���̃I�u�W�F�N�g�����݂���
	if (m_pNext != NULL)
	{
		//���̃I�u�W�F�N�g�̑O�́A�����̑O�̂��
		m_pNext->m_pPrev = pPrev;

		//���̃I�u�W�F�N�g�̑O�����Ȃ�������
		if (m_pNext->m_pPrev == NULL)
		{
			//�擪�͎��̃I�u�W�F�N�g
			m_pTop[nPriority] = m_pNext;
		}
	}
	//���̃I�u�W�F�N�g�����݂��Ȃ��i�������Ō��������
	else
	{
		//�����̑O�̃I�u�W�F�N�g���Ō���ɂ���i��������NULL�ɂ���
		m_pCur[nPriority] = m_pPrev;
	}

	//�O�̃I�u�W�F�N�g�����݂���
	if (m_pPrev != NULL)
	{
		//�O�̃I�u�W�F�N�g�̎��́A�����̎��̂��
		m_pPrev->m_pNext = pNext;

		//�O�̃I�u�W�F�N�g�̎������Ȃ�������
		if (m_pPrev->m_pNext == NULL)
		{
			//�Ō���͑O�̃I�u�W�F�N�g
			m_pCur[nPriority] = m_pPrev;
		}
	}
	//�O�̃I�u�W�F�N�g�����݂��Ȃ��i�������擪������
	else
	{
		//�����̎��̃I�u�W�F�N�g��擪�ɂ���i��������NULL�ɂ���
		m_pTop[nPriority] = m_pNext;
	}

	//�������J��
	CObject *pMy = this;
	delete pMy;
	pMy = NULL;
	m_nNumAll[nPriority]--;//���������炷
}

//=======================================
//�I�u�W�F�N�g���擾
//=======================================
CObject *CObject::GetObject(const int nPriority, const int nIdx)
{
	if (0 <= nPriority && nPriority < MAX_PRIORITY &&	//�D��ԍ����͈͓�
		0 <= nIdx && nIdx < m_nNumAll[nPriority])		//�I�u�W�F�N�g�̔ԍ����͈͓�
	{
		//�擪�ԍ���ۑ�
		CObject *pObj = m_pTop[nPriority];

		//�擾�������ԍ��̃I�u�W�F�N�g�܂Ŏ��̃I�u�W�F�N�g������������
		for (int nCntObj = 0; nCntObj < nIdx; nCntObj++)
		{
			//�I�u�W�F�N�g�̃|�C���^�����݂���
			if (pObj != NULL)
			{
				//���̃I�u�W�F�N�g��������
				pObj = pObj->m_pNext;
			}
		}

		//�I�u�W�F�N�g��Ԃ�
		return pObj;
	}

	//�O������������NULL��Ԃ�
	return NULL;
}