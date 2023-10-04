//================================================================================================
//
//�}�l�[�W���[����[manager.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "manager.h"
#include "file.h"
#include "../Object/object.h"
#include "../2D/object2D.h"
#include "../3D/object3D.h"
#include "../3D/billboard.h"
#include "../XFile/objectX.h"
#include "../XFile/enemy.h"
#include "../2D/bg.h"
#include "../2D/fade.h"
#include "../3D/bullet.h"
#include "../3D/effect.h"
#include "../3D/particle.h"
#include "../3D/explosion.h"
#include "../3D/item.h"
#include "../Number/number.h"
#include "../Number/score.h"
#include "../Number/timer.h"
#include "../3D/camera.h"
#include "../3D/light.h"
#include "../3D/floor.h"
#include "../3D/treeBillboard.h"
#include "../XFile/jewel.h"
#include "../Player/player.h"
#include "../3D/shadow.h"

#include "../Mode/title.h"
#include "../Mode/tutorial.h"
#include "../Mode/game.h"
#include "../Mode/result.h"

//�ÓI�����o�ϐ��錾
CRenderer		*CManager::s_pRenderer = NULL;
CScene			*CManager::s_pScene = NULL;
CInputKeyboard	*CManager::s_pKeyboard = NULL;
CInputGamePad	*CManager::s_pGamePad = NULL;
CInputMouse		*CManager::s_pMouse = NULL;
CDebugProc		*CManager::s_pDebugProc = NULL;
CSound			*CManager::s_pSound = NULL;
CCamera			*CManager::s_pCamera = NULL;
CLight			*CManager::s_pLight = NULL;
CTexture		*CManager::s_pTexture = NULL;

HINSTANCE		CManager::s_hInstance;
HWND			CManager::s_hWnd;

//=======================================
//�R���X�g���N�^
//=======================================
CManager::CManager()
{
	//�������J�����ă����o�ϐ����̃|�C���^�ނ�S��NULL��
	OpenPointer();

	m_bAutoFade = false;
	m_bSelect = false;
	type = CFade::FADE_OUT;
}

//=======================================
//�f�X�g���N�^
//=======================================
CManager::~CManager()
{
	//�������J��
	OpenPointer();
}

//=======================================
//����������
//=======================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�ϐ����
	s_hInstance = hInstance;
	s_hWnd = hWnd;

	//�����_���֐��̃V�[�h���������ݒ�
	srand((unsigned int)time(NULL));

	//�ÓI�����o�ϐ��𐶐�����
	CreateStatic();

	//�t�F�[�h�𐶐�����
	CFade *pFade = CFade::Create();
	pFade->Init();

	//�S����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CManager::Uninit(void)
{
	//�e�S�폜
	CShadow::Delete();

	//==============================
	//	�S�I�u�W�F�N�g�̏I������
	//==============================
	CObject::ReleaseAll();

	//�������J��
	OpenPointer();
}

//=======================================
//�������J������
//=======================================
void CManager::OpenPointer(void)
{
	//�V�[���N���X����������Ă���
	if (s_pScene != NULL)
	{
		s_pScene->Uninit();	// �I������
		delete s_pScene;	// �������폜
		s_pScene = NULL;	// NULL�|�C���^�ɂ���
	}

	//�L�[�{�[�h����������Ă���
	if (s_pKeyboard != NULL)
	{
		s_pKeyboard->Uninit();	// �I������
		delete s_pKeyboard;		// �������폜
		s_pKeyboard = NULL;		// NULL�|�C���^�ɂ���
	}

	//�Q�[���p�b�h����������Ă���
	if (s_pGamePad != NULL)
	{
		s_pGamePad->Uninit();	// �I������
		delete s_pGamePad;		// �������폜
		s_pGamePad = NULL;		// NULL�|�C���^�ɂ���
	}

	//�}�E�X����������Ă���
	if (s_pMouse != NULL)
	{
		s_pMouse->Uninit();	// �I������
		delete s_pMouse;	// �������폜
		s_pMouse = NULL;	// NULL�|�C���^�ɂ���
	}

	//�J�����N���X����������Ă���
	if (s_pCamera != NULL)
	{
		s_pCamera->Uninit();// �I������
		delete s_pCamera;	// �������폜
		s_pCamera = NULL;	// NULL�|�C���^�ɂ���
	}

	//���C�g�N���X����������Ă���
	if (s_pLight != NULL)
	{
		s_pLight->Uninit();	// �I������
		delete s_pLight;	// �������폜
		s_pLight = NULL;	// NULL�|�C���^�ɂ���
	}

	//�e�N�X�`���N���X����������Ă���
	if (s_pTexture != NULL)
	{
		s_pTexture->UnLoad();	// �I������
		delete s_pTexture;		// �������폜
		s_pTexture = NULL;		// NULL�|�C���^�ɂ���
	}

	//�f�o�b�O�\���N���X����������Ă���
	if (s_pDebugProc != NULL)
	{
		s_pDebugProc->Uninit();	// �I������
		delete s_pDebugProc;	// �������폜
		s_pDebugProc = NULL;	// NULL�|�C���^�ɂ���
	}

	// �T�E���h�N���X����������Ă���
	if (s_pSound != NULL)
	{
		s_pSound->Stop();	// �T�E���h�S��~
		s_pSound->Uninit();	// �I������
		delete s_pSound;	// �������폜
		s_pSound = NULL;	// NULL�|�C���^�ɂ���
	}

	//�����_���[�N���X����������Ă���
	if (s_pRenderer != NULL)
	{
		s_pRenderer->Uninit();	// �I������
		delete s_pRenderer;		// �������폜
		s_pRenderer = NULL;		// NULL�|�C���^�ɂ���
	}
}

//=======================================
//�ÓI�����o�ϐ��𐶐�
//=======================================
HRESULT CManager::CreateStatic(void)
{
	//�����_���[����������Ă��Ȃ�
	if (s_pRenderer == NULL)
	{
		//�����_���[����
		s_pRenderer = new CRenderer;

		//�����_���[����������
		if (FAILED(s_pRenderer->Init(s_hWnd, TRUE))) return E_FAIL;
	}

	//�L�[�{�[�h����������Ă��Ȃ�
	if (s_pKeyboard == NULL)
	{
		//�L�[�{�[�h����
		s_pKeyboard = new CInputKeyboard;

		//�L�[�{�[�h����������
		if (FAILED(s_pKeyboard->Init(s_hInstance, s_hWnd))) return E_FAIL;
	}

	//�Q�[���p�b�h����������Ă��Ȃ�
	if (s_pGamePad == NULL)
	{
		//�Q�[���p�b�h����
		s_pGamePad = new CInputGamePad;

		//�Q�[���p�b�h����������
		if (FAILED(s_pGamePad->Init(s_hInstance, s_hWnd))) return E_FAIL;
	}

	//�}�E�X����������Ă��Ȃ�
	if (s_pMouse == NULL)
	{
		//�}�E�X����
		s_pMouse = new CInputMouse;

		//�}�E�X������
		if (FAILED(s_pMouse->Init(s_hInstance, s_hWnd))) return E_FAIL;
	}

	//�f�o�b�O�\���I�u�W�F�N�g����������Ă��Ȃ�
	if (s_pDebugProc == NULL)
	{
		//�f�o�b�O�\���I�u�W�F�N�g����
		s_pDebugProc = new CDebugProc;

		//�f�o�b�O�\���I�u�W�F�N�g�����o���Ȃ�����
		if (s_pDebugProc == NULL) return E_FAIL;

		//�f�o�b�O�\���I�u�W�F�N�g������
		s_pDebugProc->Init();
	}

	//�T�E���h�N���X����������Ă��Ȃ�
	if (s_pSound == NULL)
	{
		//�T�E���h�̐���
		s_pSound = new CSound;

		//�T�E���h������
		if (FAILED(s_pSound->Init(s_hWnd))) return E_FAIL;
	}

	//�J��������������Ă��Ȃ�
	if (s_pCamera == NULL)
	{
		//�J��������
		s_pCamera = new CCamera;

		//�J�����������ł��Ȃ�����
		if (s_pCamera == NULL) return E_FAIL;

		//�J����������
		s_pCamera->Init();
	}

	//���C�g����������Ă��Ȃ�
	if (s_pLight == NULL)
	{
		//���C�g����
		s_pLight = new CLight;

		//���C�g�������o���Ȃ�����
		if (s_pLight == NULL) return E_FAIL;

		//���C�g������
		s_pLight->Init();
	}

	//�e�N�X�`���N���X����������Ă��Ȃ�
	if (s_pTexture == NULL)
	{
		//�e�N�X�`������
		s_pTexture = new CTexture;

		//�����ł��Ȃ�����
		if (s_pTexture == NULL) return E_FAIL;
	}

	//�V�[���N���X�̓����[�X���̂ݐ�������
	//�f�o�b�O���͎哮�ŋN���V�[����I���ł���
#ifndef _DEBUG
	m_bSelect = true;
	if (s_pScene == NULL)
	{
		//�V�[���N���X�����[�h��ݒ肵�Đ�������
		s_pScene = CScene::Create(CScene::TITLE);

		//�����ł��Ȃ�����
		if (s_pScene == NULL)	return E_FAIL;
	}
#endif 

	//��������
	return S_OK;
}

//=======================================
//�X�V����
//=======================================
void CManager::Update(void)
{
	//�ÓI�����o�ϐ��𐶐�����
	CreateStatic();

	//�V�[���N���X����������Ă��Ȃ�
	if (!SelectStartMode()) return;

	// [�f�o�b�O�p] �I�u�W�F�N�g�̐���\��
	int nNumAll = 0;
	for (int nCntObj = 0; nCntObj < CObject::OBJECT_PRIORIRY_MAX; nCntObj++)
	{
		nNumAll += CObject::GetNumAll(nCntObj);
	}

	s_pKeyboard->Update();	// �L�[�{�[�h�̍X�V
	s_pGamePad->Update();	// �Q�[���p�b�h�̍X�V
	s_pMouse->Update();		// �}�E�X�̍X�V
	s_pScene->Update();		// �V�[���̍X�V
	s_pRenderer->Update();	// �����_���[�X�V
	s_pLight->Update();		// ���C�g�̍X�V����
	s_pCamera->Update();	// �J�����̍X�V����

#ifdef _DEBUG
	if (s_pKeyboard->GetTrigger(DIK_LSHIFT))
	{
		//�����t�F�[�h�؂�ւ�
		m_bAutoFade ^= 1;
	}

	if (s_pKeyboard->GetPress(DIK_RIGHT) || m_bAutoFade)
	{
		//���̃��[�h�ԍ����Z�o
		//���݂̃��[�h�ԍ����擾
		int mode = (s_pScene->Get() + 1) % CScene::MAX;

		//���[�h�J��
		CFade::Set((CScene::MODE)mode, (CFade::FADE)type);
	}

	else if (s_pKeyboard->GetPress(DIK_LEFT))
	{
		//�O�̃��[�h�ԍ����Z�o
		int mode = (s_pScene->Get() + CScene::MAX - 1) % CScene::MAX;

		//���[�h�J��
		CFade::Set((CScene::MODE)mode, (CFade::FADE)type);
	}
#endif
}

//=======================================
//�`�揈��
//=======================================
void CManager::Draw(void)
{
	s_pRenderer->Draw();	// �����_���[�`��
}

//=======================================
//���̃��[�h�����߂�
//=======================================
void CManager::SetMode(CScene::MODE mode)
{
	//�T�E���h���~����
	if (s_pSound != NULL) s_pSound->Stop();

	//���݂̃��[�h��j������
	if (s_pScene != NULL)
	{
		//�V�[���̃I�u�W�F�N�g�Ȃǂ�j������
		s_pScene->Uninit();
		delete s_pScene;
		s_pScene = NULL;
	}

	//�V�������[�h�𐶐�����
	if (s_pScene == NULL)	s_pScene = CScene::Create(mode);
}

//=======================================
//���݂̃��[�h��Ԃ�
//=======================================
CScene::MODE CManager::GetMode(void)
{
	//���݂̃��[�h��Ԃ�
	if (s_pScene != NULL) return s_pScene->Get();

	//�V�[���N���X����������Ă��Ȃ�������Ƃ肠�����^�C�g����Ԃ�
	return CScene::TITLE;
}

//=======================================
//�t�F�[�h�𐶐�����
//=======================================
void CManager::CreateFade(void)
{
	CFade *pFade = CFade::Create();
	pFade->Init();
}

//=======================================
//�ŏ��̃��[�h�����߂�
//=======================================
bool CManager::SelectStartMode(void)
{
	//�V�[���N���X����������Ă��Ȃ�
	if (s_pScene != NULL)
	{
		return m_bSelect;
	}

	s_pGamePad->Update();	// �Q�[���p�b�h�̍X�V
	s_pKeyboard->Update();	// �L�[�{�[�h�̍X�V
	s_pDebugProc->SetInfo(40, GetColor(COL_WHITE), DT_CENTER);

	s_pDebugProc->Print("\n\n\n�ŏ��̃��[�h��I�����Ă�������\n");

	s_pDebugProc->Print("�^�C�g���F%d / �x\n",		CScene::TITLE);
	s_pDebugProc->Print("�`���[�g���A���F%d / �w\n",CScene::TUTORIAL);
	s_pDebugProc->Print("�Q�[���F%d / �`\n",		CScene::GAME);

	int nKeyNumber[3] = { DIK_0, DIK_1, DIK_2 };
	int nPadNumber[3] = { XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_A };

	for (int nCntMode = 0; nCntMode < 3; nCntMode++)
	{
		//�Ώۂ̔ԍ��������ꂽ
		if (IsInputTrigger(nKeyNumber[nCntMode], nPadNumber[nCntMode]))
		{
			//�V�[���N���X�����[�h��ݒ肵�Đ�������
			SetMode((CScene::MODE)nCntMode);

			//�����ł���
			if (s_pScene != NULL)
			{
				//�t�H���g�����ɖ߂�
				s_pDebugProc->SetInfo(CDebugProc::DEFAULT_FONT_SIZE, GetColor(COL_WHITE), DT_LEFT);
				m_bSelect = true;
				break;
			}
		}
	}

	return m_bSelect;
}

//=======================================
//�L�[�{�[�h�E�Q�[���p�b�h�̃v���X���
//=======================================
bool CManager::IsInputPress(int nKey, int nPadButton)
{
	//�v���X����Ԃ�
	return s_pKeyboard->GetPress(nKey) || s_pGamePad->GetPress(nPadButton);
}

//=======================================
//�L�[�{�[�h�E�Q�[���p�b�h�̃g���K�[���
//=======================================
bool CManager::IsInputTrigger(int nKey, int nPadButton)
{
	//�v���X����Ԃ�
	return s_pKeyboard->GetTrigger(nKey) || s_pGamePad->GetTrigger(nPadButton);
}

//==========================================================================================================
//												�V�[��
//==========================================================================================================

//���[�h�N���X���i�[
CScene *CScene::s_pMode = NULL;

//=======================================
//�R���X�g���N�^
//=======================================
CScene::CScene()
{
	//�ÓI�����o�ϐ��N���A
	s_pMode = NULL;
}

//=======================================
//�f�X�g���N�^
//=======================================
CScene::~CScene()
{

}

//=======================================
//��������
//=======================================
CScene *CScene::Create(MODE mode)
{
	//����
	CScene *pScene = new CScene;

	//�����ł��Ȃ�����
	if (pScene == NULL)	return NULL;

	//���[�h�ݒ�
	pScene->m_mode = mode;

	//����������
	if (FAILED(pScene->Init()))return NULL;

	//���[�h���Ƃɐ������A�������N���X�𕪂���
	switch (mode)
	{
		case TITLE:		s_pMode = new CTitle;	break;
		case TUTORIAL:	s_pMode = new CTutorial;break;
		case GAME:		s_pMode = new CGame;	break;
		case RESULT:	s_pMode = new CResult;	break;
	}

	//�C���X�^���X���������ꂽ�珉����
	if (s_pMode != NULL)s_pMode->Init();

	//�����ł��Ȃ�������C���X�^���X�폜
	else
	{
		delete pScene;
		pScene = NULL;
	}

	//�C���X�^���X��Ԃ�
	return pScene;
}

//=======================================
//����������
//=======================================
HRESULT CScene::Init(void)
{
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CScene::Uninit(void)
{
	//���[�h�N���X����������Ă�����
	if (s_pMode != NULL)
	{
		s_pMode->Uninit();	//�I������
		delete s_pMode;		//�������J��
		s_pMode = NULL;
	}
}

//=======================================
//�X�V����
//=======================================
void CScene::Update(void)
{
	if (s_pMode != NULL) s_pMode->Update();
}

//=======================================
//�`�揈��
//=======================================
void CScene::Draw(void)
{

}