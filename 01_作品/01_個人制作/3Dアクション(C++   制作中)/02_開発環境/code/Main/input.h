//==========================================================================================
//
//���͏���[input.h]
//Author:Hirasawa Shion
//
//==========================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//==========================================================================================
//�}�N����`
//==========================================================================================
#define NUM_KEY_MAX			(256)	//�L�[�̍ő吔
#define REPEATE_TIME		(150)	//���s�[�g�̊Ԋu

#define MOUSE_CLICK_LEFT	(0)		//�}�E�X���N���b�N
#define MOUSE_CLICK_RIGHT	(1)		//�}�E�X�E�N���b�N

#define GAMEPAD_BUTTON_NUM	(14)	//�Q�[���p�b�h�̃{�^����
#define STICK_DEADZONE		(655)	//�X�e�B�b�N�̗V�є͈�
#define STICK_MAX			(32767)	//�X�e�B�b�N�ő�l
#define MAX_USE_GAMEPAD		(4)		//�Q�[���p�b�h�g�p�\�䐔

#define PAD_VIBE_FADE		(200)	//�U���̌�����
#define VIBE_POWER_MIN		(7000)	//�Œ�U����
#define VIBE_POWER_LEVEL_01	(10000)	//��߂̐U��
#define VIBE_POWER_LEVEL_02	(23000)	//�����炢�̐U��
#define VIBE_POWER_LEVEL_03	(37000)	//���߂̐U��
#define VIBE_POWER_LEVEL_04	(50000)	//���Ȃ苭�߂̐U��
#define VIBE_POWER_LEVEL_05	(65535)	//�ő�U��

//==========================================================================================
//�C���v�b�g�N���X�i�e�N���X�j�̒�`
//==========================================================================================
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//==========================================================================================
//�L�[�{�[�h�N���X�̒�`�F�C���v�b�g�N���X���p��
//==========================================================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeate(int nKey);

private:
	BYTE	m_state[NUM_KEY_MAX];		//��ԁi�ۑ��p����уv���X���j
	BYTE	m_trigger[NUM_KEY_MAX];		//�g���K�[���
	BYTE	m_release[NUM_KEY_MAX];		//�����[�X���
	BYTE	m_repeate[NUM_KEY_MAX];		//���s�[�g���
	DWORD	m_currentTime[NUM_KEY_MAX];	//���݂̎���
	DWORD	m_execLastTime[NUM_KEY_MAX];//�Ō��true�ɂ�������
};

//==========================================================================================
//�}�E�X�N���X�̒�`�F�C���v�b�g�N���X���p��
//==========================================================================================
class CInputMouse : public CInput
{
public:
	//�N���b�N��ޗ�
	typedef enum
	{
		CLICK_LEFT = 0,	//���N���b�N
		CLICK_RIGHT,	//�E�N���b�N
		CLICK_WHEEL,	//�z�C�[���N���b�N
		CLICK_MAX
	}CLICK;

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);

	//========================
	//�}�E�X�̃J�[�\���ʒu��Ԃ�����
	//=======================
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	//========================
	//�}�E�X�̃J�[�\���ړ��ʂ�Ԃ�����
	//=======================
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:
	/*���݈ʒu*/D3DXVECTOR3	 	m_pos;
	/*�ړ���*/	D3DXVECTOR3	 	m_move;
	/*���*/	DIMOUSESTATE 	m_state;
	/*�g���K�[*/BYTE		 	m_trigger[CLICK_MAX];
};

//==========================================================================================
//�Q�[���p�b�h�N���X�̒�`�F�C���v�b�g�N���X���p��
//==========================================================================================
class CInputGamePad : public CInput
{
public:
	static const int UP;		// �\���u��v
	static const int DOWN;		// �\���u���v
	static const int LEFT;		// �\���u���v
	static const int RIGHT;		// �\���u�E�v
	static const int START;		// START�{�^��
	static const int BACK;		// BACK�{�^��
	static const int L_THUMB;	// ���X�e�B�b�N�{�^��
	static const int R_THUMB;	// �E�X�e�B�b�N�{�^��
	static const int L_SHOULDER;// �ʏ́uLB�v�{�^��
	static const int R_SHOULDER;// �ʏ́uRB�v�{�^��
	static const int A;			// �`�{�^��
	static const int B;			// �a�{�^��
	static const int X;			// �]�{�^��
	static const int Y;			// �x�{�^��
	static const int L_TRIGGER;	// ���g���K�[
	static const int R_TRIGGER;	// �E�g���K�[

	//�Q�[���p�b�h�̐U�����
	enum VibeState
	{
		VibeState_00_STOP,	//��~
		VibeState_01_LOOP,	//�����Ɠ����U��
		VibeState_02_FADE,	//���X�ɗ��������Ă���
		VibeState_MAX
	};

	CInputGamePad();
	~CInputGamePad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetUse(void);

	//=============================
	//�U��
	//=============================
	void UpdateVibe(void);
	void StopVibration(void);
	void SetPadVibration(WORD wPower, int nVibeTime, VibeState State);

	//=============================
	//�{�^��
	//=============================
	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeate(int nButton);

	//=============================
	//�X�e�B�b�N
	//=============================
	float GetLStickRot(void);
	float GetRStickRot(void);
	SHORT GetLStickX(void);
	SHORT GetLStickY(void);
	SHORT GetRStickX(void);
	SHORT GetRStickY(void);

private:
	static const int DEADZONE_TRIGGER_BUTTON;	//���E�̃g���K�[�{�^���̃f�b�h�]�[��
	static const int MAX_TRIGGER_BUTTON;		//���E�̃g���K�[�{�^���̍ő���͒l

	void FixTriggerButton(XINPUT_STATE *pState);
	void UpdatePress(const XINPUT_STATE state);
	void UpdateTrigger(const XINPUT_STATE state);
	void UpdateRelease(const XINPUT_STATE state);
	void UpdateRepeate(const XINPUT_STATE state);

	static int	 m_nNumAll;		//�g���Ă���R���g���[���[�̑���
	XINPUT_STATE m_state;		//��ԁi�ۑ��p����уv���X���j
	XINPUT_STATE m_trigger;		//�g���K�[���
	XINPUT_STATE m_release;		//�����[�X���
	XINPUT_STATE m_repeate;		//���s�[�g���
	DWORD		 m_currentTime;	//���݂̎���
	DWORD		 m_execLastTime;//�Ō��true�ɂ�������
	VibeState	 m_VibeState;	//�U���̏��
	WORD		 m_wVibePower;	//�U���̋���
	int			 m_nVibeTime;	//�U�������鎞��
	bool		 m_bUse;		//�g�p�̗L��
	int			 m_nID;			//�����̔ԍ�
};

#endif