//================================================================================================
//
//�t�F�[�h����[fade.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "object2D.h"

//�t�F�[�h�N���X
class CFade : public CObject2D
{
public:
	//�t�F�[�h�̏��
	enum FADE
	{
		FADE_NONE = 0,	//�������Ă��Ȃ����
		FADE_IN,		//�t�F�[�h�C��
		FADE_OUT,		//�t�F�[�h�A�E�g
		FADE_WIPE_IN,	//���ォ��E���ɂ����ăt�F�[�h�C��
		FADE_WIPE_OUT,	//���ォ��E���ɂ����ăt�F�[�h�A�E�g
		FADE_MAX
	};

	CFade();
	~CFade();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFade *Create(void);
	static void Set(CScene::MODE modeNext, FADE FadeOutType);
	static FADE Get(void) { return s_state; }
	int GetCounter(void) { return s_nCounter; }

private:
	//====================
	//�v���C�x�[�g�ÓI�����o�萔
	//====================
	static const float FADE_SPEED;	//�t�F�[�h����X�s�[�h
	static const int FADE_PRIORITY;	//�t�F�[�h�̗D�揇��

	static void Update_In(void);	//�t�F�[�h�C�����̍X�V
	static void Update_Out(void);	//�t�F�[�h�A�E�g���̍X�V
	static void Update_WipeIn(void);//���C�v�C�����̍X�V
	static void Update_WipeOut(void);//���C�v�A�E�g���̍X�V

	static CScene::MODE s_NextMode;//���̃��[�h
	static FADE			s_state;	//�t�F�[�h���
	static D3DXCOLOR	s_Color;	//�����x
	static D3DXCOLOR	s_WipeColor[VTX_MAX];//���C�v�^�C�v�̃t�F�[�h�J���[

	static const int c_WipeFadeTiming[VTX_MAX];
	static int s_WipeCounter;
	static int s_nCounter;
};

#endif