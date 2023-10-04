//================================================================================================
//
//�^�C�g����ʏ���[title.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Object/object.h"
#include "title.h"
#include "../2D/bg.h"
#include "../2D/fade.h"

//���S�̐ÓI���
char *CTitle::LOGO_TEXTURE_PATH = "data\\TEXTURE\\LOGO.png";
const D3DXVECTOR3 CTitle::LOGO_POS = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
const float CTitle::LOGO_WIDTH = 200.0f;	
const float CTitle::LOGO_HEIGHT = 100.0f;

//�v���X�\���̐ÓI���
char *CTitle::PRESS_TEXTURE_PATH = "data\\TEXTURE\\PressButton.png";
const D3DXVECTOR3 CTitle::PRESS_POS = D3DXVECTOR3(950.0f, 600.0f, 0.0f);
const float CTitle::PRESS_WIDTH = 300.0f;
const float CTitle::PRESS_HEIGHT = 50.0f;

//�_�ł̐ÓI���
const int CTitle::FLASH_TIME = 100;
const float CTitle::FLASH_SPEED = 1.0f / CTitle::FLASH_TIME;

//=======================================
//�R���X�g���N�^
//=======================================
CTitle::CTitle()
{
	//�����o�ϐ��N���A
	m_pLogo = NULL;
	m_pPress = NULL;
	m_nFlashCounter = 0;
	m_PressColor = GetColor(COL_WHITE);
	m_PressColor.a = 0.0f;
}

//=======================================
//�f�X�g���N�^
//=======================================
CTitle::~CTitle()
{

}

//=======================================
//����������
//=======================================
HRESULT CTitle::Init(void)
{
	//�w�i�F��ύX
	CManager::GetRenderer()->SetBackGroundColor(COL_BLACK);

	//���S�I�u�W�F�N�g����
	m_pLogo = CPoly2D::Create(LOGO_TEXTURE_PATH);
	if (m_pLogo != NULL)	m_pLogo->SetInfo(LOGO_POS, LOGO_WIDTH, LOGO_HEIGHT);

	//�v���X�\���I�u�W�F�N�g����
	m_pPress = CPoly2D::Create(PRESS_TEXTURE_PATH);
	if (m_pPress != NULL)	m_pPress->SetInfo(PRESS_POS, PRESS_WIDTH, PRESS_HEIGHT);

	//BGM�Đ�
	CManager::GetSound()->Play(CSound::BGM_TITLE);

	return S_OK;
}

//=======================================
//�I������
//=======================================
void CTitle::Uninit(void)
{
	//�t�F�[�h�ȊO�̃I�u�W�F�N�g�S�j��
	CObject::ExceptReleaseAll(CObject::TYPE_FADE);

	//�w�i�F��߂�
	CManager::GetRenderer()->SetBackGroundColor();
}

//=======================================
//�X�V����
//=======================================
void CTitle::Update(void)
{
	//��ʑJ��
	if (CManager::IsInputTrigger(DIK_RETURN, CInputGamePad::A))
	{
		CFade::Set(CScene::TUTORIAL, CFade::FADE_WIPE_OUT);
	}

	//�_�ŃJ�E���^�[����
	m_nFlashCounter = (m_nFlashCounter + 1) % (FLASH_TIME * 2);

	//�J�E���^�[��_�ł̊Ԋu�Ŋ����āA�����̏ꍇ
	//�������̓t�F�[�h���Ȃ�����Ă���
	if ((m_nFlashCounter / FLASH_TIME) % EVENPARITY == 0 ||
		CFade::Get() != CFade::FADE_NONE)
	{
		//�����ɂ��Ă���
		m_PressColor.a -= FLASH_SPEED;

		//�����x���O�����������O�ɖ߂�
		if (m_PressColor.a < 0.0f) m_PressColor.a = 0.0f;
	}
	//��Ȃ猻���
	else
	{
		//�\�����Ă���
		m_PressColor.a += FLASH_SPEED;

		//�����x���P����������P�ɖ߂�
		if (m_PressColor.a > 1.0f) m_PressColor.a = 1.0f;
	}

	//�v���X�\��UI�̐F��ݒ�
	m_pPress->SetVertexInfo(m_PressColor);
}

//=======================================
//�`�揈��
//=======================================
void CTitle::Draw(void)
{

}