//================================================================================================
//
//�e����[shadow.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "Polygon3D.h"
#include "shadow.h"

const D3DXCOLOR CShadow::SHADOW_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
char	*CShadow::SHADOW_TEX_PATH = "data\\TEXTURE\\effect.jpg";
int		 CShadow::s_nNumAll = 0;
CShadow *CShadow::s_pTop = NULL;
CShadow *CShadow::s_pCur = NULL;

//=======================================
//�R���X�g���N�^
//=======================================
CShadow::CShadow()
{
	//�����o�ϐ��N���A
	m_pPoly3D = NULL;

	//������ID��ݒ肵�A�����𑝉�
	m_nID = s_nNumAll++;

	//�擪�̃|�C���^��������΁A������擪�Ƃ���
	if (s_pTop == NULL)
	{
		s_pTop = this;
	}

	//�Ō�����ݒ肳��Ă���ꍇ
	if (s_pCur != NULL)
	{
		//�Ō���̎��������Ƃ���
		s_pCur->m_pNextShadow = this;
	}

	//�Ō���������Ƃ���
	s_pCur = this;
}

//=======================================
//�f�X�g���N�^
//=======================================
CShadow::~CShadow()
{
	//��������
	s_nNumAll--;
}

//=======================================
//��������
//=======================================
int CShadow::Create(D3DXVECTOR3 pos, float fSize)
{
	//�e�C���X�^���X����
	CShadow *pShadow = new CShadow;

	//��������
	if (pShadow != NULL)
	{
		//�e�|���S��������
		pShadow->Init(pos, fSize);
	}

	//�C���X�^���X��Ԃ�
	return pShadow->m_nID;
}

//=======================================
//����������
//=======================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, float fSize)
{
	//�e�|���S���ݒ�
	m_pPoly3D = CPoly3D::Create(SHADOW_TEX_PATH);
	m_pPoly3D->SetInfo(pos, fSize, 0.0f, fSize);
	m_pPoly3D->SetVertexInfo(SHADOW_COLOR);
	m_pPoly3D->SetSubBlend(true);
	m_pPoly3D->SetAlphaTest(true);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CShadow::Uninit(void)
{
	//�����𕡐�
	CShadow *pShadow = this;

	//�����̎���ۑ�
	CShadow *pNext = pShadow->m_pNextShadow;

	//�������J��
	delete pShadow;
	pShadow = NULL;

	//�����̎������Ȃ������i=�������Ō���j
	if (pNext == NULL)
	{
		//�Ō����NULL��
		s_pCur = NULL;
	}
}

//=======================================
//�X�V����
//=======================================
void CShadow::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CShadow::Draw(void)
{

}

//=======================================
//�ʒu�ݒ菈��
//=======================================
void CShadow::SetPos(int nID, D3DXVECTOR3 pos)
{
	//�e�����݂��Ă��Ȃ�  or  �����̔ԍ��������ȏ�  or  �擪���ݒ肳��Ă��Ȃ���΁A�����Ԃ�
	if (s_nNumAll <= 0 || s_nNumAll <= nID ||
		s_pTop == NULL) return;

	//�擪���i�[
	CShadow *pShadow = s_pTop;

	for(int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//�e�̔ԍ��ƈ����̔ԍ�����v���Ă���
		if (pShadow->m_nID == nID)
		{
			//�ʒu�ݒ�
			pos.y = 0.0f;
			pShadow->m_pPoly3D->SetPosition(pos);
			pShadow->Update();
			break;
		}

		//���̃|�C���^��ݒ肷��
		pShadow = pShadow->m_pNextShadow;
	}
}

//=======================================
//�e��S�č폜
//=======================================
void CShadow::Delete(void)
{
	//�e�����݂��Ă��Ȃ�  or  �擪���ݒ肳��Ă��Ȃ���΁A�����Ԃ�
	if (s_nNumAll <= 0 || s_pTop == NULL) return;

	//�擪���i�[
	CShadow *pShadow = s_pTop;

	for (int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//�e�̃|�C���^������
		if (pShadow == NULL) break;

		//���̉e��ۑ�
		CShadow *pNext = pShadow->m_pNextShadow;

		//�I������
		pShadow->Uninit();
		pShadow = NULL;

		//���̉e����
		s_pTop = pShadow = pNext;
	}
}

//=======================================
//�e�폜����
//=======================================
void CShadow::Delete(int nID)
{
	//�e�����݂��Ă��Ȃ�  or  �����̔ԍ��������ȏ�  or  �擪���ݒ肳��Ă��Ȃ���΁A�����Ԃ�
	if (s_nNumAll <= 0 || s_pTop == NULL) return;

	//�擪���i�[
	CShadow *pShadow = s_pTop;

	//�O�̉e��ۑ�
	CShadow *pPrev = NULL;

	for (int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//���̉e��ۑ�
		CShadow *pNext = pShadow->m_pNextShadow;

		//�e�̔ԍ��ƈ����̔ԍ�����v���Ă���
		if (pShadow->m_nID == nID)
		{
			//�O��L�������e�����݂���
			if (pPrev != NULL)
			{
				//�����̑O��̉e���q��
				pPrev->m_pNextShadow = pShadow->m_pNextShadow;
			}
			//�O��L�������e�����݂��Ȃ��i=�������擪)�̏ꍇ
			else
			{
				//�����̎��̂��擪�ɂ���
				s_pTop = pShadow->m_pNextShadow;
			}

			//�I������
			pShadow->Uninit();
			pShadow = NULL;
			break;
		}

		//���̉e��ۑ�
		pPrev = pShadow;

		//���̃|�C���^��ݒ肷��
		pShadow = pNext;
	}
}

//=======================================
//�ʒu�ݒ菈��
//=======================================
void CShadow::DispSwitch(int nID, bool bDisp)
{
	//�e�����݂��Ă��Ȃ�  or  �����̔ԍ��������ȏ�  or  �擪���ݒ肳��Ă��Ȃ���΁A�����Ԃ�
	if (s_nNumAll <= 0 || s_nNumAll <= nID ||
		s_pTop == NULL) return;

	//�擪���i�[
	CShadow *pShadow = s_pTop;

	for (int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//�e�̔ԍ��ƈ����̔ԍ�����v���Ă���
		if (pShadow->m_nID == nID)
		{
			//�ʒu�ݒ�
			pShadow->m_pPoly3D->DispSwitch(bDisp);
			break;
		}

		//���̃|�C���^��ݒ肷��
		pShadow = pShadow->m_pNextShadow;
	}
}

//=======================================
//�e�̃T�C�Y�ݒ�
//=======================================
void CShadow::SetSize(int nID, float fSize)
{
	//�e�����݂��Ă��Ȃ�  or  �����̔ԍ��������ȏ�  or  �擪���ݒ肳��Ă��Ȃ���΁A�����Ԃ�
	if (s_nNumAll <= 0 || s_nNumAll <= nID ||
		s_pTop == NULL) return;

	//�擪���i�[
	CShadow *pShadow = s_pTop;

	for (int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//�e�̔ԍ��ƈ����̔ԍ�����v���Ă���
		if (pShadow->m_nID == nID)
		{
			//�T�C�Y�ݒ�
			pShadow->m_pPoly3D->SetVertexInfo(fSize, 0.0f, fSize);
			break;
		}

		//���̃|�C���^��ݒ肷��
		pShadow = pShadow->m_pNextShadow;
	}
}