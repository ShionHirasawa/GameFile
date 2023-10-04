//================================================================================================
//
//���U���g��ʏ���[result.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "result.h"
#include "../Object/object.h"
#include "../2D/object2D.h"
#include "../2D/bg.h"
#include "../2D/fade.h"

//�ÓI�����o�ϐ��錾
CResult::STATE CResult::s_State;

//�e�N�X�`���p�X�ݒ�
char *CResult::BG_TEXTURE_PATH[STATE_MAX] = {
	"data\\TEXTURE\\GameOver.png",
	"data\\TEXTURE\\GameClear.png",
};

//=======================================
//�R���X�g���N�^
//=======================================
CResult::CResult()
{

}

//=======================================
//�f�X�g���N�^
//=======================================
CResult::~CResult()
{
	s_State = STATE_OVER;
}

//=======================================
//����������
//=======================================
HRESULT CResult::Init(void)
{
	//�w�i�̃e�N�X�`���ǂݍ���
	CBg::Load(BG_TEXTURE_PATH[s_State]);

	//�w�i�I�u�W�F�N�g����
	CBg *pBg = CBg::Create();
	pBg->SetVertexInfo(GetColor(COL_WHITE));

	//BGM�Đ�
	CManager::GetSound()->Play(CSound::BGM_RESULT);

	return S_OK;
}

//=======================================
//�I������
//=======================================
void CResult::Uninit(void)
{
	//�t�F�[�h�ȊO�̃I�u�W�F�N�g�S�j��
	CObject::ExceptReleaseAll(CObject::TYPE_FADE);
}

//=======================================
//�X�V����
//=======================================
void CResult::Update(void)
{
	//��ʑJ��
	if (CManager::IsInputTrigger(DIK_RETURN, CInputGamePad::A))
	{
		CFade::Set(CScene::TITLE, CFade::FADE_WIPE_OUT);
	}
}

//=======================================
//�`�揈��
//=======================================
void CResult::Draw(void)
{

}

//=======================================
//���U���g��Ԑݒ菈��
//=======================================
void CResult::SetState(STATE state)
{
	//��Ԑݒ�
	s_State = state;
}