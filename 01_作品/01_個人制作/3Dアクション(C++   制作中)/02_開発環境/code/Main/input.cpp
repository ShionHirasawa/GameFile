//==========================================================================================
//
//入力処理[input.cpp]
//Author:Hirasawa Shion
//
//==========================================================================================
#include "main.h"
#include "input.h"
#include "debugproc.h"

//静的メンバ変数変数
LPDIRECTINPUT8 CInput::m_pInput = NULL;


//==========================================================================================================
//												入力情報の基礎
//==========================================================================================================
//=======================================
//コンストラクタ
//=======================================
CInput::CInput()
{
	//終了処理を呼んで、ポインタを破棄
	Uninit();
}

//=======================================
//デストラクタ
//=======================================
CInput::~CInput()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//まだDirectInputオブジェクトが生成されていない
	if (m_pInput == NULL)
	{
		//DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==========================================================================================================
//													キーボード
//==========================================================================================================
//=======================================
//コンストラクタ
//=======================================
CInputKeyboard::CInputKeyboard()
{
	//キー入力情報全てクリア
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_state[nCntKey] = 0;		m_trigger[nCntKey] = 0;
		m_release[nCntKey] = 0;		m_repeate[nCntKey] = 0;
		m_currentTime[nCntKey] = 0;	m_execLastTime[nCntKey] = 0;
	}
}

//=======================================
//デストラクタ
//=======================================
CInputKeyboard::~CInputKeyboard()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//初期化処理
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//デバイス取得
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマット設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モード設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	if (FAILED(m_pDevice->Acquire()))
	{
		return E_FAIL;
	}

	//時間初期化
	for (int nCntInit = 0; nCntInit < NUM_KEY_MAX; nCntInit++)
	{
		m_currentTime[nCntInit] = 0;
		m_execLastTime[nCntInit] = timeGetTime() - REPEATE_TIME;
	}

	//初期化無事成功
	return S_OK;
}

//=======================================
//キーボード終了処理
//=======================================
void CInputKeyboard::Uninit(void)
{
	//親クラスの終了処理
	CInput::Uninit();
}

//=======================================
//キーボード更新処理
//=======================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//入力デバイスからデバイスを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//トリガー情報
			m_trigger[nCntKey] = (m_state[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//リリース情報
			m_release[nCntKey] = (m_state[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];

			//リピート情報
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

			//プレス情報
			m_state[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{//アクセス権を再取得
		m_pDevice->Acquire();
	}
}

//=======================================
//キーボードのプレス情報を返す処理
//=======================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_state[nKey] & 0x80) ? true : false;
}

//========================
//キーボードのトリガー情報を返す処理
//========================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_trigger[nKey] & 0x80) ? true : false;
}

//=======================================
//キーボードのリリース情報を返す処理
//=======================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_release[nKey] & 0x80) ? true : false;
}

//=======================================
//キーボードのリピート情報を返す処理
//=======================================
bool CInputKeyboard::GetRepeate(int nKey)
{
	return (m_repeate[nKey] & 0x80) ? true : false;
}

//==========================================================================================================
//												マウス
//==========================================================================================================
//=======================================
//コンストラクタ
//=======================================
CInputMouse::CInputMouse()
{
	//キー入力情報全てクリア
	for (int nCntKey = 0; nCntKey < CLICK_MAX; nCntKey++)
	{
		m_state.rgbButtons[nCntKey] = 0;	m_trigger[nCntKey] = 0;
	}
}

//=======================================
//デストラクタ
//=======================================
CInputMouse::~CInputMouse()
{

}

//========================
//マウス初期化処理
//========================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DireceInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	//デバイス取得
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマット設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//協調モード設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//マウスへのアクセス権を獲得
	m_pDevice->Acquire();

	//初期化成功
	return S_OK;
}

//========================
//マウス終了処理
//========================
void CInputMouse::Uninit(void)
{
	//親の終了処理
	CInput::Uninit();
}

//========================
//マウス更新処理
//========================
void CInputMouse::Update(void)
{
	DIMOUSESTATE State;
	POINT point;

	//マウス取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &State)))
	{
		//トリガー情報格納
		for (int nCntKey = 0; nCntKey < CLICK_MAX; nCntKey++)
		{
			m_trigger[nCntKey] = (m_state.rgbButtons[nCntKey] ^ State.rgbButtons[nCntKey]) & State.rgbButtons[nCntKey];
		}

		//プレス
		m_state = State;	//キーボードのプレス情報を保存

							//移動量
		m_move = D3DXVECTOR3((float)m_state.lX, (float)m_state.lY, (float)m_state.lZ);
	}
	else
	{
		m_pDevice->Acquire();
	}

	//マウス座標取得
	GetCursorPos(&point);
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &point);
	m_pos.x = (float)point.x;
	m_pos.y = (float)point.y;
}

//========================
//マウスのプレス情報を返す処理
//=======================
bool CInputMouse::GetPress(int nButton)
{
	return m_state.rgbButtons[nButton] & 0x80 ? true : false;
}

//========================
//マウスのトリガー情報を返す処理
//=======================
bool CInputMouse::GetTrigger(int nButton)
{
	return m_trigger[nButton] & 0x80 ? true : false;
}

//==========================================================================================================
//													ゲームパッド
//==========================================================================================================
int		  CInputGamePad::m_nNumAll = 0;								// 使われているコントローラーの総数
const int CInputGamePad::DEADZONE_TRIGGER_BUTTON = 200;				// 左右のトリガーボタンのデッドゾーン
const int CInputGamePad::MAX_TRIGGER_BUTTON = 255;					// 左右のトリガーボタンの最大入力値
const int CInputGamePad::UP			= XINPUT_GAMEPAD_DPAD_UP;		// 十字「上」
const int CInputGamePad::DOWN		= XINPUT_GAMEPAD_DPAD_DOWN;		// 十字「下」
const int CInputGamePad::LEFT		= XINPUT_GAMEPAD_DPAD_LEFT;		// 十字「左」
const int CInputGamePad::RIGHT		= XINPUT_GAMEPAD_DPAD_RIGHT;	// 十字「右」
const int CInputGamePad::START		= XINPUT_GAMEPAD_START;			// STARTボタン
const int CInputGamePad::BACK		= XINPUT_GAMEPAD_BACK;			// BACKボタン
const int CInputGamePad::L_THUMB	= XINPUT_GAMEPAD_LEFT_THUMB;	// 左スティックボタン
const int CInputGamePad::R_THUMB	= XINPUT_GAMEPAD_RIGHT_THUMB;	// 右スティックボタン
const int CInputGamePad::L_SHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER;	// 通称「LB」ボタン
const int CInputGamePad::R_SHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER;// 通称「RB」ボタン
const int CInputGamePad::A			= XINPUT_GAMEPAD_A;				// Ａボタン
const int CInputGamePad::B			= XINPUT_GAMEPAD_B;				// Ｂボタン
const int CInputGamePad::X			= XINPUT_GAMEPAD_X;				// Ⅹボタン
const int CInputGamePad::Y			= XINPUT_GAMEPAD_Y;				// Ｙボタン
const int CInputGamePad::L_TRIGGER	= 147;							// 左トリガー（数字に意味なし
const int CInputGamePad::R_TRIGGER	= 369;							// 右トリガー（数字に意味なし

//=======================================
//コンストラクタ
//=======================================
CInputGamePad::CInputGamePad()
{
	//ゲームパッド類
	m_nID = m_nNumAll++;
	m_nVibeTime = 0;
	m_wVibePower = 0;
	m_VibeState = VibeState_00_STOP;
	m_bUse = false;
	m_currentTime = 0;
	m_execLastTime = timeGetTime();
}

//=======================================
//デストラクタ
//=======================================
CInputGamePad::~CInputGamePad()
{
	//総数を減らす
	m_nNumAll--;
}

//=======================================
//ゲームパッド初期化処理
//=======================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//親クラスの初期化処理
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	m_bUse = true;

	//初期化成功
	return S_OK;
}

//=======================================
//ゲームパッド終了処理
//=======================================
void CInputGamePad::Uninit(void)
{
	//振動停止
	StopVibration();

	//XInput終了
	XInputEnable(false);

	//親クラスの終了処理
	CInput::Uninit();
}

//=======================================
//ゲームパッド更新処理
//=======================================
void CInputGamePad::Update(void)
{
	//最新のパッド情報を格納
	XINPUT_STATE xInputState;

	//ゲームパッドから情報取得
	if (XInputGetState(0, &xInputState) != ERROR_SUCCESS)
	{
		//使ってないよ
		m_bUse = false;
		return;
	}

	//使ってるよ
	m_bUse = true;

	//トリガーボタン情報修正
	FixTriggerButton(&xInputState);

	//リリース情報更新
	UpdateRelease(xInputState);

	//トリガー情報更新
	UpdateTrigger(xInputState);

	//リピート情報更新
	UpdateRepeate(xInputState);

	//プレス情報更新
	UpdatePress(xInputState);

	//コントローラーの振動状態更新
	UpdateVibe();
}

//=======================================
//左右のトリガーボタン情報を修正
//=======================================
void CInputGamePad::FixTriggerButton(XINPUT_STATE *pState)
{
	//左トリガーボタンがデッドゾーンより大きい
	if (pState->Gamepad.bLeftTrigger > DEADZONE_TRIGGER_BUTTON)
		 pState->Gamepad.bLeftTrigger = MAX_TRIGGER_BUTTON;

	//デッドゾーン以下なら押されていない
	else pState->Gamepad.bLeftTrigger = 0;

	//右トリガーボタンがデッドゾーンより大きい
	if (pState->Gamepad.bRightTrigger > DEADZONE_TRIGGER_BUTTON)
		pState->Gamepad.bRightTrigger = MAX_TRIGGER_BUTTON;

	//デッドゾーン以下なら押されていない
	else pState->Gamepad.bRightTrigger = 0;
}

//=======================================
//プレス情報更新
//=======================================
void CInputGamePad::UpdatePress(const XINPUT_STATE state)
{
	//情報更新
	m_state = state;

	//トリガーボタン情報修正
	FixTriggerButton(&m_state);
}

//=======================================
//トリガー情報更新
//=======================================
void CInputGamePad::UpdateTrigger(const XINPUT_STATE state)
{
	m_trigger.Gamepad.wButtons = (m_state.Gamepad.wButtons ^ state.Gamepad.wButtons) & state.Gamepad.wButtons;
	m_trigger.Gamepad.bLeftTrigger = (m_state.Gamepad.bLeftTrigger ^ state.Gamepad.bLeftTrigger) & state.Gamepad.bLeftTrigger;
	m_trigger.Gamepad.bRightTrigger = (m_state.Gamepad.bRightTrigger ^ state.Gamepad.bRightTrigger) & state.Gamepad.bRightTrigger;
}

//=======================================
//リリース情報更新
//=======================================
void CInputGamePad::UpdateRelease(const XINPUT_STATE state)
{
	m_release.Gamepad.wButtons = (state.Gamepad.wButtons ^ m_state.Gamepad.wButtons) & m_state.Gamepad.wButtons;
	m_release.Gamepad.bLeftTrigger = (state.Gamepad.bLeftTrigger ^ m_state.Gamepad.bLeftTrigger) & m_state.Gamepad.bLeftTrigger;
	m_release.Gamepad.bRightTrigger = (state.Gamepad.bRightTrigger ^ m_state.Gamepad.bRightTrigger) & m_state.Gamepad.bRightTrigger;
}

//=======================================
//リピート情報更新
//=======================================
void CInputGamePad::UpdateRepeate(const XINPUT_STATE state)
{
	//現在時間取得
	m_currentTime = timeGetTime();

	//基本ボタンのリピート情報更新
	for (int nCntKey = 0; nCntKey < GAMEPAD_BUTTON_NUM; nCntKey++)
	{
		if (m_state.Gamepad.wButtons & 1 << nCntKey && (m_currentTime - m_execLastTime) > REPEATE_TIME)
		{
			m_execLastTime = m_currentTime;	//最終実行時間更新
			m_repeate.Gamepad.wButtons += 1 << nCntKey;
		}
	}
	//左右のトリガーボタン情報更新
	if (m_state.Gamepad.bLeftTrigger > DEADZONE_TRIGGER_BUTTON && (m_currentTime - m_execLastTime) > REPEATE_TIME)
	{
		m_execLastTime = m_currentTime;	//最終実行時間更新
		m_repeate.Gamepad.bLeftTrigger = (BYTE)L_TRIGGER;
	}
	if (m_state.Gamepad.bRightTrigger > DEADZONE_TRIGGER_BUTTON && (m_currentTime - m_execLastTime) > REPEATE_TIME)
	{
		m_execLastTime = m_currentTime;	//最終実行時間更新
		m_repeate.Gamepad.bRightTrigger = (BYTE)R_TRIGGER;
	}
}

//=======================================
//ゲームパッド振動状態の更新
//=======================================
void CInputGamePad::UpdateVibe(void)
{
	//振動停止中
	if (m_VibeState == VibeState_00_STOP) return;

	//継続時間減少
	m_nVibeTime--;

	if (m_nVibeTime <= 0)
	{
		StopVibration();	//振動停止
	}
	//バイブの状態が、徐々に弱まっていくものである
	else if (m_VibeState == VibeState_02_FADE)
	{
		//振動量が最低値を下回ったら
		if (m_wVibePower <= VIBE_POWER_MIN)
		{
			StopVibration();	//振動停止
		}
		else
		{
			//徐々に減少させる
			m_wVibePower -= PAD_VIBE_FADE;
		}
	}

	//左右のモーターに振動力を設定
	XINPUT_VIBRATION Vibration = { m_wVibePower , m_wVibePower };
	XInputSetState(m_nID, &Vibration);			//コントローラーに振動を設定
}

//=======================================
//全ゲームパッドの振動停止
//=======================================
void CInputGamePad::StopVibration(void)
{
	//振動を止める
	m_wVibePower = 0;
	m_nVibeTime = 0;
	m_VibeState = VibeState_00_STOP;

	//左右のモーターの回転速度を０に設定
	XINPUT_VIBRATION Vibration = { 0,0 };

	//振動をOFF
	XInputSetState(m_nID, &Vibration);
}

//=======================================
//ゲームパッド振動設定
//=======================================
void CInputGamePad::SetPadVibration(WORD wPower, int nVibeTime, VibeState State)
{
	//左右の振動力を設定
	XINPUT_VIBRATION Vibration = { wPower , wPower };

	//強さ・時間・状態設定
	m_wVibePower = wPower;
	m_nVibeTime = nVibeTime;
	m_VibeState = State;

	//対象のコントローラーに振動を設定
	XInputSetState(m_nID, &Vibration);
}

//=======================================
//ゲームパッドのプレス情報を返す処理
//=======================================
bool CInputGamePad::GetPress(int nButton)
{
	//使われていない
	if (!m_bUse)	return false;

	if (nButton == L_TRIGGER) return m_state.Gamepad.bLeftTrigger & nButton ? true : false;

	if (nButton == R_TRIGGER) return m_state.Gamepad.bRightTrigger & nButton ? true : false;

	return m_state.Gamepad.wButtons & nButton ? true : false;
}

//=======================================
//ゲームパッドのトリガー情報を返す処理
//=======================================
bool CInputGamePad::GetTrigger(int nButton)
{
	//使われていない
	if (!m_bUse)	return false;

	if (nButton == L_TRIGGER) return m_trigger.Gamepad.bLeftTrigger & nButton ? true : false;

	if (nButton == R_TRIGGER) return m_trigger.Gamepad.bRightTrigger & nButton ? true : false;

	return m_trigger.Gamepad.wButtons & nButton ? true : false;
}

//=======================================
//ゲームパッドのリリース情報を返す処理
//=======================================
bool CInputGamePad::GetRelease(int nButton)
{
	//使われていない
	if (!m_bUse)	return false;

	if (nButton == L_TRIGGER) return m_release.Gamepad.bLeftTrigger & nButton ? true : false;

	if (nButton == R_TRIGGER) return m_release.Gamepad.bRightTrigger & nButton ? true : false;

	return m_release.Gamepad.wButtons & nButton ? true : false;
}

//=======================================
//ゲームパッドのリピート情報を返す処理
//=======================================
bool CInputGamePad::GetRepeate(int nButton)
{
	//使われていない
	if (!m_bUse)	return false;

	if (nButton == L_TRIGGER) return m_repeate.Gamepad.bLeftTrigger & nButton ? true : false;

	if (nButton == R_TRIGGER) return m_repeate.Gamepad.bRightTrigger & nButton ? true : false;

	return m_repeate.Gamepad.wButtons & nButton ? true : false;
}

//=======================================
//左スティックの横軸を返す処理
//=======================================
SHORT CInputGamePad::GetLStickX(void)
{
	//使われていない
	if (!m_bUse)		return 0;

	//-値最大なら+値最大に合わせる
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
//左スティックの縦軸を返す処理
//=======================================
SHORT CInputGamePad::GetLStickY(void)
{
	//使われていない
	if (!m_bUse)		return 0;

	//-値最大なら+値最大に合わせる
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
//右スティックの横軸を返す処理
//=======================================
SHORT CInputGamePad::GetRStickX(void)
{
	//使われていない
	if (!m_bUse)		return 0;

	//-値最大なら+値最大に合わせる
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
//右スティックの縦軸を返す処理
//=======================================
SHORT CInputGamePad::GetRStickY(void)
{
	//使われていない
	if (!m_bUse)		return 0;

	//-値最大なら+値最大に合わせる
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
//ゲームパッド使用しているか返す処理
//=======================================
bool CInputGamePad::GetUse(void)
{
	return m_bUse;
}

//=======================================
//左スティックの角度を取得
//=======================================
float CInputGamePad::GetLStickRot(void)
{
	//使われていない
	if (!m_bUse) return 0.0f;

	//左スティックの角度を返す
	return atan2f((float)m_state.Gamepad.sThumbLX, (float)m_state.Gamepad.sThumbLY);
}

//=======================================
//右スティックの角度を取得
//=======================================
float CInputGamePad::GetRStickRot(void)
{
	//使われていない
	if (!m_bUse) return 0.0f;

	//右スティックの角度を返す
	return atan2f((float)m_state.Gamepad.sThumbRX, (float)m_state.Gamepad.sThumbRY);
}