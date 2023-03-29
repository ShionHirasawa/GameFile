//==========================================================================================
//
//入力プログラム[input.cpp]
//Author:石原颯馬
//
//==========================================================================================
//Ver.2.2.2（プロジェクト制作中のもの）
//使用可能な入力:キーボード・マウス（カーソル位置・移動量）・ゲームパッド（1~4台）
//==========================================================================================
#include "input.h"
#include "sound.h"
#include "debugproc.h"

//マクロ定義
#define NUM_KEY_MAX			(256)	//キーの最大数
#define REPEATE_TIME		(150)	//リピートの間隔
#define GAMEPAD_BUTTON_NUM	(14)	//ゲームパッドのボタン数
#define STICK_DEADZONE		(655)	//遊び
#define PAD_VIBE_FADE		(200)	//振動の減少量

//グローバル変数
//キーボード部
LPDIRECTINPUT8 g_pInputKeyboard = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
Keyboard g_keyboard[NUM_KEY_MAX];	//キーボード構造体

//ゲームパッド（XInput使用）部
GamePad g_gamePad[MAX_USE_GAMEPAD];	//ゲームパッド情報


//マウス部
LPDIRECTINPUT8 g_pInputMouse = NULL;
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;
Mouse g_mouse;	//マウス情報

//==================================================
//						キーボード
//==================================================
//========================
//キーボード初期化処理
//========================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DireceInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pInputKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//デバイス取得
	if (FAILED(g_pInputKeyboard->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	
	//データフォーマット設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モード設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	//時間初期化
	for (int nCntInit = 0; nCntInit < NUM_KEY_MAX; nCntInit++)
	{
		g_keyboard[nCntInit].currentTime = 0;
		g_keyboard[nCntInit].execLastTime = timeGetTime() - REPEATE_TIME;
	}

	return S_OK;
}

//========================
//キーボード終了処理
//========================
void UninitKeyboard(void)
{
	//入力デバイスの破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInputKeyboard != NULL)
	{
		g_pInputKeyboard->Release();
		g_pInputKeyboard = NULL;
	}
}

//========================
//キーボード更新処理
//========================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	//入力デバイスからデバイスを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_keyboard[nCntKey].trigger = (g_keyboard[nCntKey].state ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_keyboard[nCntKey].release = (g_keyboard[nCntKey].state ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];

			//リピート
			g_keyboard[nCntKey].currentTime = timeGetTime();
			if (aKeyState[nCntKey] && (g_keyboard[nCntKey].currentTime - g_keyboard[nCntKey].execLastTime) > REPEATE_TIME)
			{
				g_keyboard[nCntKey].execLastTime = g_keyboard[nCntKey].currentTime;
				g_keyboard[nCntKey].repeate = aKeyState[nCntKey];
			}
			else
			{
				g_keyboard[nCntKey].repeate = 0;
			}
			//プレス
			g_keyboard[nCntKey].state = aKeyState[nCntKey];	//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//========================
//キーボードのプレス情報を返す処理
//========================
bool GetKeyboardPress(int nKey)
{
	return (g_keyboard[nKey].state & 0x80) ? true : false;	//?が条件式作ることになってるみたいよ
}

//========================
//キーボードのトリガー情報を返す処理
//========================
bool GetKeyboardTrigger(int nKey)
{
	return (g_keyboard[nKey].trigger & 0x80) ? true : false;
}

//========================
//キーボードのリリース情報を返す処理
//========================
bool GetKeyboardRelease(int nKey)
{
	return (g_keyboard[nKey].release & 0x80) ? true : false;
}

//========================
//キーボードのリピート情報を返す処理
//========================
bool GetKeyboardRepeate(int nKey)
{
	return (g_keyboard[nKey].repeate & 0x80) ? true : false;
}


//==================================================
//						マウス
//==================================================
//========================
//マウス初期化処理
//========================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	//DireceInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&g_pInputMouse, NULL)))
	{
		return E_FAIL;
	}

	//デバイス取得
	if (FAILED(g_pInputMouse->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマット設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	//協調モード設定
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//マウスへのアクセス権を獲得
	g_pDevMouse->Acquire();

	//時間初期化
	for (int nCntInit = 0; nCntInit < MOUSE_BUTTON_MAX; nCntInit++)
	{
		g_mouse.mb[nCntInit].currentTime = 0;
		g_mouse.mb[nCntInit].execLastTime = timeGetTime() - REPEATE_TIME;
	}

	return S_OK;
}

//========================
//マウス終了処理
//========================
void UninitMouse(void)
{
	//入力デバイスの破棄
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInputMouse != NULL)
	{
		g_pInputMouse->Release();
		g_pInputMouse = NULL;
	}
}

//========================
//マウス更新処理
//========================
void UpdateMouse(void)
{
	DIMOUSESTATE MouseState;
	POINT point;

	//マウス取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState)))
	{
		for (int nCntKey = 0; nCntKey < MOUSE_BUTTON_MAX; nCntKey++)
		{
			//トリガーとリリース
			g_mouse.mb[nCntKey].trigger = (g_mouse.state.rgbButtons[nCntKey] ^ MouseState.rgbButtons[nCntKey]) & MouseState.rgbButtons[nCntKey];
			g_mouse.mb[nCntKey].release = (g_mouse.state.rgbButtons[nCntKey] ^ MouseState.rgbButtons[nCntKey]) & ~MouseState.rgbButtons[nCntKey];

			//リピート
			g_mouse.mb[nCntKey].currentTime = timeGetTime();
			if (MouseState.rgbButtons[nCntKey] && (g_mouse.mb[nCntKey].currentTime - g_mouse.mb[nCntKey].execLastTime) > REPEATE_TIME)
			{
				g_mouse.mb[nCntKey].execLastTime = g_mouse.mb[nCntKey].currentTime;
				g_mouse.mb[nCntKey].repeate = MouseState.rgbButtons[nCntKey];
			}
			else
			{
				g_mouse.mb[nCntKey].repeate = 0;
			}
		}

		//プレス
		g_mouse.state = MouseState;	//キーボードのプレス情報を保存

		//移動量
		g_mouse.move = D3DXVECTOR3((float)g_mouse.state.lX, (float)g_mouse.state.lY, 0.0f);
	}
	else
	{
		g_pDevMouse->Acquire();
	}

	//マウス座標取得
	GetCursorPos(&point);
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &point);
	g_mouse.pos.x = (float)point.x;
	g_mouse.pos.y = (float)point.y;
}

//========================
//マウスのプレス情報を返す処理
//=======================
bool GetMouseClickPress(int nButton)
{
	return g_mouse.state.rgbButtons[nButton] & 0x80 ? true : false;
}

//========================
//マウスのトリガー情報を返す処理
//=======================
bool GetMouseClickTrigger(int nButton)
{
	return g_mouse.mb[nButton].trigger & 0x80 ? true : false;
}

//========================
//マウスのリピート情報を返す処理
//=======================
bool GetMouseClickRepeate(int nButton)
{
	return g_mouse.mb[nButton].repeate & 0x80 ? true : false;
}

//========================
//マウスのカーソル位置を返す処理
//=======================
D3DXVECTOR3 GetMousePos(void)
{
	return g_mouse.pos;
}

//========================
//マウスのカーソル移動量を返す処理
//=======================
D3DXVECTOR3 GetMouseMove(void)
{
	return g_mouse.move;
}


//==================================================
//						ゲームパッド
//==================================================
//========================
//ゲームパッド初期化処理
//========================
void InitGamePad(void)
{
	//ゲームパッド類
	for (int nCntGPad = 0; nCntGPad < MAX_USE_GAMEPAD; nCntGPad++)
	{
		g_gamePad[nCntGPad].currentTime = 0;
		g_gamePad[nCntGPad].execLastTime = timeGetTime();
		g_gamePad[nCntGPad].bUse = false;
		g_gamePad[nCntGPad].Vibe_State = VIBE_STATE_00_STOP;
		g_gamePad[nCntGPad].wVibePower = 0;
		g_gamePad[nCntGPad].nVibeTime = 0;
	}
}

//========================
//ゲームパッド終了処理
//========================
void UninitGamePad(void)
{
	//全コントローラーの振動停止
	StopVibration();

	//XInput終了
	XInputEnable(false);
}

//========================
//ゲームパッド更新処理
//========================
void UpdateGamePad(void)
{
	XINPUT_STATE xInputState;

	//ゲームパッドから情報取得
	for (int nCntGPad = 0; nCntGPad < MAX_USE_GAMEPAD; nCntGPad++)
	{
		bool bOldUseGPad = g_gamePad[nCntGPad].bUse;
		if (XInputGetState(nCntGPad, &xInputState) == ERROR_SUCCESS)
		{
			//使ってるよ
			g_gamePad[nCntGPad].bUse = true;

			//ボタントリガー情報取得
			g_gamePad[nCntGPad].trigger = (g_gamePad[nCntGPad].state.Gamepad.wButtons ^ xInputState.Gamepad.wButtons) & xInputState.Gamepad.wButtons;

			//ボタンリリース情報取得
			g_gamePad[nCntGPad].release = (g_gamePad[nCntGPad].state.Gamepad.wButtons ^ xInputState.Gamepad.wButtons) & ~xInputState.Gamepad.wButtons;

			//リピート情報生成
			g_gamePad[nCntGPad].currentTime = timeGetTime();
			for (int nCntKey = 0; nCntKey < GAMEPAD_BUTTON_NUM; nCntKey++)
			{
				if (g_gamePad[nCntGPad].state.Gamepad.wButtons & 1 << nCntKey && (g_gamePad[nCntGPad].currentTime - g_gamePad[nCntGPad].execLastTime) > REPEATE_TIME)
				{
					g_gamePad[nCntGPad].execLastTime = g_gamePad[nCntGPad].currentTime;
					g_gamePad[nCntGPad].repeate += 1 << nCntKey;
				}
			}

			//プレス情報その他もろもろ設定
			g_gamePad[nCntGPad].state = xInputState;

			//コントローラーの振動状態更新
			UpdateVibeGamePad(nCntGPad);
		}
		else
		{
			//使ってないよ
			g_gamePad[nCntGPad].bUse = false;
		}

		//コントローラー接続が変化したら音鳴らす
		if (g_gamePad[nCntGPad].bUse != bOldUseGPad)
		{
			PlaySoundSE(SOUND_LABEL_SE_CONNECT, nCntGPad);
		}
	}
}

//========================
//ゲームパッド振動状態の更新
//========================
void UpdateVibeGamePad(int nPadNum)
{
	//振動停止中
	if (g_gamePad[nPadNum].nVibeTime <= 0)
	{
		g_gamePad[nPadNum].wVibePower = 0;					// 振動量    ０   に設定
		g_gamePad[nPadNum].nVibeTime = 0;					//継続時間   ０   に設定
		g_gamePad[nPadNum].Vibe_State = VIBE_STATE_00_STOP;	//  状態  「停止」に設定

		StopVibration(nPadNum);	//対象のコントローラーの振動停止
	}

	//振動中
	else
	{
		//継続時間減少
		g_gamePad[nPadNum].nVibeTime--;

		//バイブの状態が、徐々に弱まっていくものである
		if (g_gamePad[nPadNum].Vibe_State == VIBE_STATE_02_FADE)
		{
			//振動量が最低値を下回ったら
			if (g_gamePad[nPadNum].wVibePower <= VIBE_POWER_MIN)
			{
				//振動を止める
				g_gamePad[nPadNum].wVibePower = 0;
				g_gamePad[nPadNum].nVibeTime = 0;
				g_gamePad[nPadNum].Vibe_State = VIBE_STATE_00_STOP;

				StopVibration(nPadNum);	//対象のコントローラーの振動停止
			}
			else
			{
				//徐々に減少させる
				g_gamePad[nPadNum].wVibePower -= PAD_VIBE_FADE;
			}
		}

		XINPUT_VIBRATION Vibration;

		Vibration.wLeftMotorSpeed = g_gamePad[nPadNum].wVibePower;	//左のモーターの回転速度を設定
		Vibration.wRightMotorSpeed = g_gamePad[nPadNum].wVibePower;	//右のモーターの回転速度を設定

		XInputSetState(nPadNum, &Vibration);	//対象のコントローラーに振動を設定

		PrintDebugProc("振動量：%d    残り時間：%d\n", g_gamePad[nPadNum].wVibePower, g_gamePad[nPadNum].nVibeTime / MAX_FPS);
	}
}

//========================
//全ゲームパッドの振動停止
//========================
void StopVibration(void)
{
	XINPUT_VIBRATION Vibration;

	//左右のモーターの回転速度を０に設定
	Vibration.wLeftMotorSpeed = Vibration.wRightMotorSpeed = 0;

	//全コントローラーの振動をOFFに
	for (int nCntPad = 0; nCntPad < MAX_USE_GAMEPAD; nCntPad++)
	{
		//振動をOFF
		XInputSetState(nCntPad, &Vibration);
	}
}

//========================
//対象のコントローラーの振動停止
//========================
void StopVibration(int nPadNum)
{
	XINPUT_VIBRATION Vibration = {0,0};

	//対象のコントローラーの振動をOFF
	XInputSetState(nPadNum, &Vibration);
}

//========================
//ゲームパッドのプレス情報を返す処理
//=======================
bool GetGamepadPress(int nPadNum, int nButton)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].state.Gamepad.wButtons & nButton ? true : false;
	}
	else
	{
		return false;
	}
}

//========================
//ゲームパッドのトリガー情報を返す処理
//=======================
bool GetGamepadTrigger(int nPadNum, int nButton)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].trigger & nButton ? true : false;
	}
	else
	{
		return false;
	}
}

//========================
//ゲームパッドのリリース情報を返す処理
//=======================
bool GetGamepadRelease(int nPadNum, int nButton)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].release & nButton ? true : false;
	}
	else
	{
		return false;
	}
}

//========================
//ゲームパッドのリピート情報を返す処理
//=======================
bool GetGamepadRepeate(int nPadNum, int nButton)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].repeate & nButton ? true : false;
	}
	else
	{
		return false;
	}
}

//========================
//左スティックの横軸を返す処理
//=======================
SHORT GetLStickX(int nPadNum)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		//-値最大なら+値最大に合わせる
		if (g_gamePad[nPadNum].state.Gamepad.sThumbLX < -STICK_MAX)
		{
			g_gamePad[nPadNum].state.Gamepad.sThumbLX = -STICK_MAX;
		}

		if (abs(g_gamePad[nPadNum].state.Gamepad.sThumbLX) > STICK_DEADZONE)
		{
			return g_gamePad[nPadNum].state.Gamepad.sThumbLX;
		}
		else
		{
			return 0;
		}
	}
	else
	{//使っていなかったら0返す
		return 0;
	}
}

//========================
//左スティックの縦軸を返す処理
//=======================
SHORT GetLStickY(int nPadNum)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		//-値最大なら+値最大に合わせる
		if (g_gamePad[nPadNum].state.Gamepad.sThumbLY < -STICK_MAX)
		{
			g_gamePad[nPadNum].state.Gamepad.sThumbLY = -STICK_MAX;
		}

		if (abs(g_gamePad[nPadNum].state.Gamepad.sThumbLY) > STICK_DEADZONE)
		{
			return g_gamePad[nPadNum].state.Gamepad.sThumbLY;
		}
		else
		{
			return 0;
		}
	}
	else
	{//使っていなかったら0返す
		return 0;
	}
}

//========================
//右スティックの横軸を返す処理
//=======================
SHORT GetRStickX(int nPadNum)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		//-値最大なら+値最大に合わせる
		if (g_gamePad[nPadNum].state.Gamepad.sThumbRX < -STICK_MAX)
		{
			g_gamePad[nPadNum].state.Gamepad.sThumbRX = -STICK_MAX;
		}

		if (abs(g_gamePad[nPadNum].state.Gamepad.sThumbRX) > STICK_DEADZONE)
		{
			return g_gamePad[nPadNum].state.Gamepad.sThumbRX;
		}
		else
		{
			return 0;
		}
	}
	else
	{//使っていなかったら0返す
		return 0;
	}
}

//========================
//右スティックの縦軸を返す処理
//=======================
SHORT GetRStickY(int nPadNum)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		//-値最大なら+値最大に合わせる
		if (g_gamePad[nPadNum].state.Gamepad.sThumbRY < -STICK_MAX)
		{
			g_gamePad[nPadNum].state.Gamepad.sThumbRY = -STICK_MAX;
		}

		if (abs(g_gamePad[nPadNum].state.Gamepad.sThumbRY) > STICK_DEADZONE)
		{
			return g_gamePad[nPadNum].state.Gamepad.sThumbRY;
		}
		else
		{
			return 0;
		}
	}
	else
	{//使っていなかったら0返す
		return 0;
	}
}

//========================
//ゲームパッド使用しているか返す処理
//=======================
bool GetUseGamepad(int nPadNum)
{
	if (g_gamePad[nPadNum].bUse == true)
	{
		return g_gamePad[nPadNum].bUse;
	}
	else
	{
		return false;
	}
}

//========================
//ゲームパッド振動設定
//========================
void SetPadVibration(int nPadNum, WORD wPower, int nVibeTime, VIBE_STATE State)
{
	g_gamePad[nPadNum].wVibePower = wPower;		//強さ設定
	g_gamePad[nPadNum].nVibeTime = nVibeTime;	//時間設定
	g_gamePad[nPadNum].Vibe_State = State;		//状態設定

	XINPUT_VIBRATION Vibration;

	Vibration.wLeftMotorSpeed = wPower;		//左のモーターの回転速度を設定
	Vibration.wRightMotorSpeed = wPower;	//右のモーターの回転速度を設定

	XInputSetState(nPadNum, &Vibration);	//対象のコントローラーに振動を設定
}