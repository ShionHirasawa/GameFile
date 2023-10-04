//==========================================================================================
//
//���͏���[input.cpp]
//Author:Hirasawa Shion
//
//==========================================================================================
#include "main.h"
#include "input.h"
#include "debugproc.h"

//�ÓI�����o�ϐ��ϐ�
LPDIRECTINPUT8 CInput::m_pInput = NULL;


//==========================================================================================================
//												���͏��̊�b
//==========================================================================================================
//=======================================
//�R���X�g���N�^
//=======================================
CInput::CInput()
{
	//�I���������Ă�ŁA�|�C���^��j��
	Uninit();
}

//=======================================
//�f�X�g���N�^
//=======================================
CInput::~CInput()
{

}

//=======================================
//����������
//=======================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�܂�DirectInput�I�u�W�F�N�g����������Ă��Ȃ�
	if (m_pInput == NULL)
	{
		//DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CInput::Uninit(void)
{
	//���̓f�o�C�X�̔j��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==========================================================================================================
//													�L�[�{�[�h
//==========================================================================================================
//=======================================
//�R���X�g���N�^
//=======================================
CInputKeyboard::CInputKeyboard()
{
	//�L�[���͏��S�ăN���A
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_state[nCntKey] = 0;		m_trigger[nCntKey] = 0;
		m_release[nCntKey] = 0;		m_repeate[nCntKey] = 0;
		m_currentTime[nCntKey] = 0;	m_execLastTime[nCntKey] = 0;
	}
}

//=======================================
//�f�X�g���N�^
//=======================================
CInputKeyboard::~CInputKeyboard()
{

}

//=======================================
//����������
//=======================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//����������
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�擾
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h�ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	if (FAILED(m_pDevice->Acquire()))
	{
		return E_FAIL;
	}

	//���ԏ�����
	for (int nCntInit = 0; nCntInit < NUM_KEY_MAX; nCntInit++)
	{
		m_currentTime[nCntInit] = 0;
		m_execLastTime[nCntInit] = timeGetTime() - REPEATE_TIME;
	}

	//��������������
	return S_OK;
}

//=======================================
//�L�[�{�[�h�I������
//=======================================
void CInputKeyboard::Uninit(void)
{
	//�e�N���X�̏I������
	CInput::Uninit();
}

//=======================================
//�L�[�{�[�h�X�V����
//=======================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//���̓f�o�C�X����f�o�C�X���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//�g���K�[���
			m_trigger[nCntKey] = (m_state[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//�����[�X���
			m_release[nCntKey] = (m_state[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];

			//���s�[�g���
			m_currentTime[nCntKey] = timeGetTime();
			if (aKeyState[nCntKey] && (m_currentTime[nCntKey] - m_execLastTime[nCntKey]) > REPEATE_TIME)
			{
				m_execLastTime[nCntKey] = m_currentTime[nCntKey];
				m_repeate[nCntKey] = aKeyState[nCntKey];
			}
			else
			{
				m_repeate[nCntKey] = 0;
			}

			//�v���X���
			m_state[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{//�A�N�Z�X�����Ď擾
		m_pDevice->Acquire();
	}
}

//=======================================
//�L�[�{�[�h�̃v���X����Ԃ�����
//=======================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_state[nKey] & 0x80) ? true : false;
}

//========================
//�L�[�{�[�h�̃g���K�[����Ԃ�����
//========================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_trigger[nKey] & 0x80) ? true : false;
}

//=======================================
//�L�[�{�[�h�̃����[�X����Ԃ�����
//=======================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_release[nKey] & 0x80) ? true : false;
}

//=======================================
//�L�[�{�[�h�̃��s�[�g����Ԃ�����
//=======================================
bool CInputKeyboard::GetRepeate(int nKey)
{
	return (m_repeate[nKey] & 0x80) ? true : false;
}

//==========================================================================================================
//												�}�E�X
//==========================================================================================================
//=======================================
//�R���X�g���N�^
//=======================================
CInputMouse::CInputMouse()
{
	//�L�[���͏��S�ăN���A
	for (int nCntKey = 0; nCntKey < CLICK_MAX; nCntKey++)
	{
		m_state.rgbButtons[nCntKey] = 0;	m_trigger[nCntKey] = 0;
	}
}

//=======================================
//�f�X�g���N�^
//=======================================
CInputMouse::~CInputMouse()
{

}

//========================
//�}�E�X����������
//========================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DireceInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�擾
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g�ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//�������[�h�ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�}�E�X�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	//����������
	return S_OK;
}

//========================
//�}�E�X�I������
//========================
void CInputMouse::Uninit(void)
{
	//�e�̏I������
	CInput::Uninit();
}

//========================
//�}�E�X�X�V����
//========================
void CInputMouse::Update(void)
{
	DIMOUSESTATE State;
	POINT point;

	//�}�E�X�擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &State)))
	{
		//�g���K�[���i�[
		for (int nCntKey = 0; nCntKey < CLICK_MAX; nCntKey++)
		{
			m_trigger[nCntKey] = (m_state.rgbButtons[nCntKey] ^ State.rgbButtons[nCntKey]) & State.rgbButtons[nCntKey];
		}

		//�v���X
		m_state = State;	//�L�[�{�[�h�̃v���X����ۑ�

							//�ړ���
		m_move = D3DXVECTOR3((float)m_state.lX, (float)m_state.lY, (float)m_state.lZ);
	}
	else
	{
		m_pDevice->Acquire();
	}

	//�}�E�X���W�擾
	GetCursorPos(&point);
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &point);
	m_pos.x = (float)point.x;
	m_pos.y = (float)point.y;
}

//========================
//�}�E�X�̃v���X����Ԃ�����
//=======================
bool CInputMouse::GetPress(int nButton)
{
	return m_state.rgbButtons[nButton] & 0x80 ? true : false;
}

//========================
//�}�E�X�̃g���K�[����Ԃ�����
//=======================
bool CInputMouse::GetTrigger(int nButton)
{
	return m_trigger[nButton] & 0x80 ? true : false;
}

//==========================================================================================================
//													�Q�[���p�b�h
//==========================================================================================================
int		  CInputGamePad::m_nNumAll = 0;								// �g���Ă���R���g���[���[�̑���
const int CInputGamePad::DEADZONE_TRIGGER_BUTTON = 200;				// ���E�̃g���K�[�{�^���̃f�b�h�]�[��
const int CInputGamePad::MAX_TRIGGER_BUTTON = 255;					// ���E�̃g���K�[�{�^���̍ő���͒l
const int CInputGamePad::UP			= XINPUT_GAMEPAD_DPAD_UP;		// �\���u��v
const int CInputGamePad::DOWN		= XINPUT_GAMEPAD_DPAD_DOWN;		// �\���u���v
const int CInputGamePad::LEFT		= XINPUT_GAMEPAD_DPAD_LEFT;		// �\���u���v
const int CInputGamePad::RIGHT		= XINPUT_GAMEPAD_DPAD_RIGHT;	// �\���u�E�v
const int CInputGamePad::START		= XINPUT_GAMEPAD_START;			// START�{�^��
const int CInputGamePad::BACK		= XINPUT_GAMEPAD_BACK;			// BACK�{�^��
const int CInputGamePad::L_THUMB	= XINPUT_GAMEPAD_LEFT_THUMB;	// ���X�e�B�b�N�{�^��
const int CInputGamePad::R_THUMB	= XINPUT_GAMEPAD_RIGHT_THUMB;	// �E�X�e�B�b�N�{�^��
const int CInputGamePad::L_SHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER;	// �ʏ́uLB�v�{�^��
const int CInputGamePad::R_SHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER;// �ʏ́uRB�v�{�^��
const int CInputGamePad::A			= XINPUT_GAMEPAD_A;				// �`�{�^��
const int CInputGamePad::B			= XINPUT_GAMEPAD_B;				// �a�{�^��
const int CInputGamePad::X			= XINPUT_GAMEPAD_X;				// �]�{�^��
const int CInputGamePad::Y			= XINPUT_GAMEPAD_Y;				// �x�{�^��
const int CInputGamePad::L_TRIGGER	= 147;							// ���g���K�[�i�����ɈӖ��Ȃ�
const int CInputGamePad::R_TRIGGER	= 369;							// �E�g���K�[�i�����ɈӖ��Ȃ�

//=======================================
//�R���X�g���N�^
//=======================================
CInputGamePad::CInputGamePad()
{
	//�Q�[���p�b�h��
	m_nID = m_nNumAll++;
	m_nVibeTime = 0;
	m_wVibePower = 0;
	m_VibeState = VibeState_00_STOP;
	m_bUse = false;
	m_currentTime = 0;
	m_execLastTime = timeGetTime();
}

//=======================================
//�f�X�g���N�^
//=======================================
CInputGamePad::~CInputGamePad()
{
	//���������炷
	m_nNumAll--;
}

//=======================================
//�Q�[���p�b�h����������
//=======================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�e�N���X�̏���������
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	m_bUse = true;

	//����������
	return S_OK;
}

//=======================================
//�Q�[���p�b�h�I������
//=======================================
void CInputGamePad::Uninit(void)
{
	//�U����~
	StopVibration();

	//XInput�I��
	XInputEnable(false);

	//�e�N���X�̏I������
	CInput::Uninit();
}

//=======================================
//�Q�[���p�b�h�X�V����
//=======================================
void CInputGamePad::Update(void)
{
	//�ŐV�̃p�b�h�����i�[
	XINPUT_STATE xInputState;

	//�Q�[���p�b�h������擾
	if (XInputGetState(0, &xInputState) != ERROR_SUCCESS)
	{
		//�g���ĂȂ���
		m_bUse = false;
		return;
	}

	//�g���Ă��
	m_bUse = true;

	//�g���K�[�{�^�����C��
	FixTriggerButton(&xInputState);

	//�����[�X���X�V
	UpdateRelease(xInputState);

	//�g���K�[���X�V
	UpdateTrigger(xInputState);

	//���s�[�g���X�V
	UpdateRepeate(xInputState);

	//�v���X���X�V
	UpdatePress(xInputState);

	//�R���g���[���[�̐U����ԍX�V
	UpdateVibe();
}

//=======================================
//���E�̃g���K�[�{�^�������C��
//=======================================
void CInputGamePad::FixTriggerButton(XINPUT_STATE *pState)
{
	//���g���K�[�{�^�����f�b�h�]�[�����傫��
	if (pState->Gamepad.bLeftTrigger > DEADZONE_TRIGGER_BUTTON)
		 pState->Gamepad.bLeftTrigger = MAX_TRIGGER_BUTTON;

	//�f�b�h�]�[���ȉ��Ȃ牟����Ă��Ȃ�
	else pState->Gamepad.bLeftTrigger = 0;

	//�E�g���K�[�{�^�����f�b�h�]�[�����傫��
	if (pState->Gamepad.bRightTrigger > DEADZONE_TRIGGER_BUTTON)
		pState->Gamepad.bRightTrigger = MAX_TRIGGER_BUTTON;

	//�f�b�h�]�[���ȉ��Ȃ牟����Ă��Ȃ�
	else pState->Gamepad.bRightTrigger = 0;
}

//=======================================
//�v���X���X�V
//=======================================
void CInputGamePad::UpdatePress(const XINPUT_STATE state)
{
	//���X�V
	m_state = state;

	//�g���K�[�{�^�����C��
	FixTriggerButton(&m_state);
}

//=======================================
//�g���K�[���X�V
//=======================================
void CInputGamePad::UpdateTrigger(const XINPUT_STATE state)
{
	m_trigger.Gamepad.wButtons = (m_state.Gamepad.wButtons ^ state.Gamepad.wButtons) & state.Gamepad.wButtons;
	m_trigger.Gamepad.bLeftTrigger = (m_state.Gamepad.bLeftTrigger ^ state.Gamepad.bLeftTrigger) & state.Gamepad.bLeftTrigger;
	m_trigger.Gamepad.bRightTrigger = (m_state.Gamepad.bRightTrigger ^ state.Gamepad.bRightTrigger) & state.Gamepad.bRightTrigger;
}

//=======================================
//�����[�X���X�V
//=======================================
void CInputGamePad::UpdateRelease(const XINPUT_STATE state)
{
	m_release.Gamepad.wButtons = (state.Gamepad.wButtons ^ m_state.Gamepad.wButtons) & m_state.Gamepad.wButtons;
	m_release.Gamepad.bLeftTrigger = (state.Gamepad.bLeftTrigger ^ m_state.Gamepad.bLeftTrigger) & m_state.Gamepad.bLeftTrigger;
	m_release.Gamepad.bRightTrigger = (state.Gamepad.bRightTrigger ^ m_state.Gamepad.bRightTrigger) & m_state.Gamepad.bRightTrigger;
}

//=======================================
//���s�[�g���X�V
//=======================================
void CInputGamePad::UpdateRepeate(const XINPUT_STATE state)
{
	//���ݎ��Ԏ擾
	m_currentTime = timeGetTime();

	//��{�{�^���̃��s�[�g���X�V
	for (int nCntKey = 0; nCntKey < GAMEPAD_BUTTON_NUM; nCntKey++)
	{
		if (m_state.Gamepad.wButtons & 1 << nCntKey && (m_currentTime - m_execLastTime) > REPEATE_TIME)
		{
			m_execLastTime = m_currentTime;	//�ŏI���s���ԍX�V
			m_repeate.Gamepad.wButtons += 1 << nCntKey;
		}
	}
	//���E�̃g���K�[�{�^�����X�V
	if (m_state.Gamepad.bLeftTrigger > DEADZONE_TRIGGER_BUTTON && (m_currentTime - m_execLastTime) > REPEATE_TIME)
	{
		m_execLastTime = m_currentTime;	//�ŏI���s���ԍX�V
		m_repeate.Gamepad.bLeftTrigger = (BYTE)L_TRIGGER;
	}
	if (m_state.Gamepad.bRightTrigger > DEADZONE_TRIGGER_BUTTON && (m_currentTime - m_execLastTime) > REPEATE_TIME)
	{
		m_execLastTime = m_currentTime;	//�ŏI���s���ԍX�V
		m_repeate.Gamepad.bRightTrigger = (BYTE)R_TRIGGER;
	}
}

//=======================================
//�Q�[���p�b�h�U����Ԃ̍X�V
//=======================================
void CInputGamePad::UpdateVibe(void)
{
	//�U����~��
	if (m_VibeState == VibeState_00_STOP) return;

	//�p�����Ԍ���
	m_nVibeTime--;

	if (m_nVibeTime <= 0)
	{
		StopVibration();	//�U����~
	}
	//�o�C�u�̏�Ԃ��A���X�Ɏ�܂��Ă������̂ł���
	else if (m_VibeState == VibeState_02_FADE)
	{
		//�U���ʂ��Œ�l�����������
		if (m_wVibePower <= VIBE_POWER_MIN)
		{
			StopVibration();	//�U����~
		}
		else
		{
			//���X�Ɍ���������
			m_wVibePower -= PAD_VIBE_FADE;
		}
	}

	//���E�̃��[�^�[�ɐU���͂�ݒ�
	XINPUT_VIBRATION Vibration = { m_wVibePower , m_wVibePower };
	XInputSetState(m_nID, &Vibration);			//�R���g���[���[�ɐU����ݒ�
}

//=======================================
//�S�Q�[���p�b�h�̐U����~
//=======================================
void CInputGamePad::StopVibration(void)
{
	//�U�����~�߂�
	m_wVibePower = 0;
	m_nVibeTime = 0;
	m_VibeState = VibeState_00_STOP;

	//���E�̃��[�^�[�̉�]���x���O�ɐݒ�
	XINPUT_VIBRATION Vibration = { 0,0 };

	//�U����OFF
	XInputSetState(m_nID, &Vibration);
}

//=======================================
//�Q�[���p�b�h�U���ݒ�
//=======================================
void CInputGamePad::SetPadVibration(WORD wPower, int nVibeTime, VibeState State)
{
	//���E�̐U���͂�ݒ�
	XINPUT_VIBRATION Vibration = { wPower , wPower };

	//�����E���ԁE��Ԑݒ�
	m_wVibePower = wPower;
	m_nVibeTime = nVibeTime;
	m_VibeState = State;

	//�Ώۂ̃R���g���[���[�ɐU����ݒ�
	XInputSetState(m_nID, &Vibration);
}

//=======================================
//�Q�[���p�b�h�̃v���X����Ԃ�����
//=======================================
bool CInputGamePad::GetPress(int nButton)
{
	//�g���Ă��Ȃ�
	if (!m_bUse)	return false;

	if (nButton == L_TRIGGER) return m_state.Gamepad.bLeftTrigger & nButton ? true : false;

	if (nButton == R_TRIGGER) return m_state.Gamepad.bRightTrigger & nButton ? true : false;

	return m_state.Gamepad.wButtons & nButton ? true : false;
}

//=======================================
//�Q�[���p�b�h�̃g���K�[����Ԃ�����
//=======================================
bool CInputGamePad::GetTrigger(int nButton)
{
	//�g���Ă��Ȃ�
	if (!m_bUse)	return false;

	if (nButton == L_TRIGGER) return m_trigger.Gamepad.bLeftTrigger & nButton ? true : false;

	if (nButton == R_TRIGGER) return m_trigger.Gamepad.bRightTrigger & nButton ? true : false;

	return m_trigger.Gamepad.wButtons & nButton ? true : false;
}

//=======================================
//�Q�[���p�b�h�̃����[�X����Ԃ�����
//=======================================
bool CInputGamePad::GetRelease(int nButton)
{
	//�g���Ă��Ȃ�
	if (!m_bUse)	return false;

	if (nButton == L_TRIGGER) return m_release.Gamepad.bLeftTrigger & nButton ? true : false;

	if (nButton == R_TRIGGER) return m_release.Gamepad.bRightTrigger & nButton ? true : false;

	return m_release.Gamepad.wButtons & nButton ? true : false;
}

//=======================================
//�Q�[���p�b�h�̃��s�[�g����Ԃ�����
//=======================================
bool CInputGamePad::GetRepeate(int nButton)
{
	//�g���Ă��Ȃ�
	if (!m_bUse)	return false;

	if (nButton == L_TRIGGER) return m_repeate.Gamepad.bLeftTrigger & nButton ? true : false;

	if (nButton == R_TRIGGER) return m_repeate.Gamepad.bRightTrigger & nButton ? true : false;

	return m_repeate.Gamepad.wButtons & nButton ? true : false;
}

//=======================================
//���X�e�B�b�N�̉�����Ԃ�����
//=======================================
SHORT CInputGamePad::GetLStickX(void)
{
	//�g���Ă��Ȃ�
	if (!m_bUse)		return 0;

	//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
	if (m_state.Gamepad.sThumbLX < -STICK_MAX)
	{
		m_state.Gamepad.sThumbLX = -STICK_MAX;
	}

	if (abs(m_state.Gamepad.sThumbLX) > STICK_DEADZONE)
	{
		return m_state.Gamepad.sThumbLX;
	}
	else
	{
		return 0;
	}
}

//=======================================
//���X�e�B�b�N�̏c����Ԃ�����
//=======================================
SHORT CInputGamePad::GetLStickY(void)
{
	//�g���Ă��Ȃ�
	if (!m_bUse)		return 0;

	//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
	if (m_state.Gamepad.sThumbLY < -STICK_MAX)
	{
		m_state.Gamepad.sThumbLY = -STICK_MAX;
	}

	if (abs(m_state.Gamepad.sThumbLY) > STICK_DEADZONE)
	{
		return m_state.Gamepad.sThumbLY;
	}
	else
	{
		return 0;
	}
}

//=======================================
//�E�X�e�B�b�N�̉�����Ԃ�����
//=======================================
SHORT CInputGamePad::GetRStickX(void)
{
	//�g���Ă��Ȃ�
	if (!m_bUse)		return 0;

	//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
	if (m_state.Gamepad.sThumbRX < -STICK_MAX)
	{
		m_state.Gamepad.sThumbRX = -STICK_MAX;
	}

	if (abs(m_state.Gamepad.sThumbRX) > STICK_DEADZONE)
	{
		return m_state.Gamepad.sThumbRX;
	}
	else
	{
		return 0;
	}
}

//=======================================
//�E�X�e�B�b�N�̏c����Ԃ�����
//=======================================
SHORT CInputGamePad::GetRStickY(void)
{
	//�g���Ă��Ȃ�
	if (!m_bUse)		return 0;

	//-�l�ő�Ȃ�+�l�ő�ɍ��킹��
	if (m_state.Gamepad.sThumbRY < -STICK_MAX)
	{
		m_state.Gamepad.sThumbRY = -STICK_MAX;
	}

	if (abs(m_state.Gamepad.sThumbRY) > STICK_DEADZONE)
	{
		return m_state.Gamepad.sThumbRY;
	}
	else
	{
		return 0;
	}
}

//=======================================
//�Q�[���p�b�h�g�p���Ă��邩�Ԃ�����
//=======================================
bool CInputGamePad::GetUse(void)
{
	return m_bUse;
}

//=======================================
//���X�e�B�b�N�̊p�x���擾
//=======================================
float CInputGamePad::GetLStickRot(void)
{
	//�g���Ă��Ȃ�
	if (!m_bUse) return 0.0f;

	//���X�e�B�b�N�̊p�x��Ԃ�
	return atan2f((float)m_state.Gamepad.sThumbLX, (float)m_state.Gamepad.sThumbLY);
}

//=======================================
//�E�X�e�B�b�N�̊p�x���擾
//=======================================
float CInputGamePad::GetRStickRot(void)
{
	//�g���Ă��Ȃ�
	if (!m_bUse) return 0.0f;

	//�E�X�e�B�b�N�̊p�x��Ԃ�
	return atan2f((float)m_state.Gamepad.sThumbRX, (float)m_state.Gamepad.sThumbRY);
}