//================================================================================================
//
//�t�F�[�h����[fade.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "fade.h"

//�t�F�[�h�̗D�揇��
const int CFade::FADE_PRIORITY = 7;

//�t�F�[�h����X�s�[�h
const float CFade::FADE_SPEED = 0.015f;

CScene::MODE	CFade::s_NextMode;			//���̃��[�h
CFade::FADE		CFade::s_state;				//�t�F�[�h���
D3DXCOLOR		CFade::s_Color;				//�t�F�[�h�J���[�i��Ƀ��l���g�p����
D3DXCOLOR		CFade::s_WipeColor[VTX_MAX];//���C�v�t�F�[�h�J���[�i��Ƀ��l���g�p���邪�A�e�v�f���Ƃɕω�����^�C�~���O�͈قȂ�
int				CFade::s_WipeCounter = 0;	//���C�v�t�F�[�h�̃J�E���^�[
int				CFade::s_nCounter = 0;

//���C�v�t�F�[�h�̃^�C�~���O
const int CFade::c_WipeFadeTiming[VTX_MAX] = {
	0,		//����̒��_�͍ŏ�����
	20, 20,	//�E��A�����̒��_�́A�����^�C�~���O
	40		//�E���̒��_�́A��Ԓx���
};

//=======================================
//�R���X�g���N�^
//=======================================
CFade::CFade() : CObject2D(FADE_PRIORITY)
{
	//�F��ݒ�
	s_Color = GetColor(COL_BLACK);
	s_WipeColor[0] = s_WipeColor[1] = s_WipeColor[2] = s_WipeColor[3] = s_Color;

	//�t�F�[�h�C�����
	s_state = FADE_IN;
	
	s_WipeCounter = 0;
	s_nCounter = 0;
}

//=======================================
//�f�X�g���N�^
//=======================================
CFade::~CFade()
{

}

//=======================================
//��������
//=======================================
CFade *CFade::Create(void)
{
	CFade *pFade = new CFade;

	if (pFade == NULL) return NULL;

	pFade->Init();

	return pFade;
}

//=======================================
//����������
//=======================================
HRESULT CFade::Init(void)
{
	if (FAILED(CObject2D::Init(D3DXVECTOR3(HALF_WIDTH, HALF_HEIGHT, 0.0f), HALF_WIDTH, HALF_HEIGHT, s_Color, VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_FADE);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CFade::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObject2D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CFade::Update(void)
{
	//�t�F�[�h�̏�ԂōX�V�����𕪂���
	switch (s_state)
	{
		case FADE_IN: Update_In();	SetVertexInfo(s_Color); break;
		case FADE_OUT:Update_Out();	SetVertexInfo(s_Color); break;
		case FADE_WIPE_IN: 
			Update_WipeIn();
			for (int nCntWipe = 0; nCntWipe < VTX_MAX; nCntWipe++)
			{
				SetVertexInfo(s_WipeColor[nCntWipe], nCntWipe);
			}
			break;

		case FADE_WIPE_OUT:
			Update_WipeOut();
			for (int nCntWipe = 0; nCntWipe < VTX_MAX; nCntWipe++)
			{
				SetVertexInfo(s_WipeColor[nCntWipe], nCntWipe);
			}
			break;
	}
}

//=======================================
//�`�揈��
//=======================================
void CFade::Draw(void)
{
	if (s_state == FADE_NONE) return;

	//������`��
	CObject2D::Draw();
}

//=======================================
//�ݒ菈��
//=======================================
void CFade::Set(CScene::MODE modeNext, FADE FadeOutType)
{
	//�t�F�[�h���Ȃ�ݒ肵�Ȃ�
	if (s_state != FADE_NONE || FadeOutType != FADE_OUT && FadeOutType != FADE_WIPE_OUT) return;

	if (FadeOutType == FADE_OUT || FadeOutType == FADE_WIPE_OUT)
	{
		s_state = FadeOutType;	//�t�F�[�h�A�E�g��Ԃ�
		s_NextMode = modeNext;	//���̉��(���[�h)��ݒ�

		if(FadeOutType == FADE_OUT)	s_Color = GetColor(COL_CLEAR);	//�����|���S��(����)�ɂ��Ă���
		else
		{
			//�����|���S��(����)�ɂ��Ă���
			s_WipeColor[0] = s_WipeColor[1] = s_WipeColor[2] = s_WipeColor[3] = GetColor(COL_CLEAR);
			s_WipeCounter = 0;
		}

		s_nCounter++;
	}
}

//=======================================
//�t�F�[�h�C�����̍X�V
//=======================================
void CFade::Update_In(void)
{
	//�|���S���𓧖��ɂ��Ă���
	s_Color.a -= FADE_SPEED;

	//�����ɂȂ���
	if (s_Color.a <= 0.0f)
	{
		s_Color.a = 0.0f;
		s_state = FADE_NONE;	//�������Ă��Ȃ���Ԃ�
	}
}

//=======================================
//�t�F�[�h�A�E�g���̍X�V
//=======================================
void CFade::Update_Out(void)
{
	//�|���S����s�����ɂ��Ă���
	s_Color.a += FADE_SPEED;

	//���S�Ƀt�F�[�h����
	if (s_Color.a >= 1.0f)
	{
		s_Color.a = 1.0f;
		s_state = FADE_IN;	//�t�F�[�h�C����Ԃ�

		CManager::SetMode(s_NextMode);//���[�h�ݒ�(���̉�ʂɈڍs)
	}
}

//=======================================
//���C�v�t�F�[�h�C�����̍X�V
//=======================================
void CFade::Update_WipeIn(void)
{
	//���C�v�J�E���^�[�𑝂₷
	s_WipeCounter++;

	for (int nCntFade = 0; nCntFade < VTX_MAX; nCntFade++)
	{
		//�܂��t�F�[�h����^�C�~���O�Ŗ����̂Ȃ�A���̃t�F�[�h��
		if (s_WipeCounter < c_WipeFadeTiming[nCntFade]) continue;

		//�����ɂ��Ă���
		s_WipeColor[nCntFade].a -= FADE_SPEED;

		//���l���O�����������O�ɖ߂�
		if (s_WipeColor[nCntFade].a < 0.0f) s_WipeColor[nCntFade].a = 0.0f;
	}

	//�܂��S���������ɂȂ��Ă��Ȃ�
	if (s_WipeColor[0].a > 0.0f || s_WipeColor[1].a > 0.0f || s_WipeColor[2].a > 0.0f || s_WipeColor[3].a > 0.0f) return;

	s_state = FADE_NONE;//�������Ă��Ȃ���Ԃ�
	s_WipeCounter = 0;	//�J�E���^�[������
}

//=======================================
//���C�v�t�F�[�h�A�E�g���̍X�V
//=======================================
void CFade::Update_WipeOut(void)
{
	//���C�v�J�E���^�[�𑝂₷
	s_WipeCounter++;

	for (int nCntFade = 0; nCntFade < VTX_MAX; nCntFade++)
	{
		//�܂��t�F�[�h����^�C�~���O�Ŗ����̂Ȃ�A���̃t�F�[�h��
		if (s_WipeCounter < c_WipeFadeTiming[nCntFade]) continue;

		//�����ɂ��Ă���
		s_WipeColor[nCntFade].a += FADE_SPEED;

		//���l���P����������P�ɖ߂�
		if (s_WipeColor[nCntFade].a > 1.0f) s_WipeColor[nCntFade].a = 1.0f;
	}

	//�܂��S�����s�����ɂȂ��Ă��Ȃ�
	if (s_WipeColor[0].a < 1.0f || s_WipeColor[1].a < 1.0f || s_WipeColor[2].a < 1.0f || s_WipeColor[3].a < 1.0f) return;

	s_state = FADE_WIPE_IN;			//�t�F�[�h�C����Ԃ�
	s_WipeCounter = 0;				//�J�E���^�[������
	CManager::SetMode(s_NextMode);	//���[�h�ݒ�(���̉�ʂɈڍs)
}