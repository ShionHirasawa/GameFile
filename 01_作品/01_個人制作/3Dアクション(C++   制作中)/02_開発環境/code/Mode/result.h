//================================================================================================
//
//���U���g��ʏ���[result.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//���U���g�N���X
class CResult : public CScene
{
public:
	//���U���g�X�e�[�^�X
	enum STATE
	{
		STATE_OVER = 0,	//�Q�[���I�[�o�[
		STATE_CLEAR,	//�Q�[���N���A!
		STATE_MAX
	};

	CResult();
	~CResult();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetState(STATE state);

private:
	//====================
	//�ÓI�����o�萔
	//====================
	static char	*BG_TEXTURE_PATH[STATE_MAX];	//�w�i�̃e�N�X�`���p�X
	static STATE s_State;			//���U���g���
};

#endif