/*==========================================================================================================================================================

												キーボード・コントローラー処理[main.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX		(256)		//キーの最大数
#define NUM_PAD_MAX		(14)		//ゲームパッドの最大ボタン数

//グローバル変数宣言
LPDIRECTINPUT8		 g_pInput = NULL;			//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//入力デバイス(キーボード)へのポインタ

//キーボード
BYTE g_aKeyState[NUM_KEY_MAX];					//キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			//キーボードのトリガー情報
BYTE g_aKeyStateRelease[NUM_KEY_MAX];			//キーボードのリリース情報

//コントローラー
WORD g_wGamePadStateTrigger;					//ゲームパッドのトリガー情報
WORD g_wGamePadStateRelease;					//ゲームパッドのリリース情報
XINPUT_STATE g_xInput;							//コントローラーの情報
bool g_bUseGamepad;								//ゲームパッドが使われているかどうか

//--------------------------------------------------------
//				キーボードの初期化処理
//--------------------------------------------------------
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void* *)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の作成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を取得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//--------------------------------------------------------
//				キーボードの終了処理
//--------------------------------------------------------
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();		//キーボードへのアクセス権を破棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//--------------------------------------------------------
//				キーボードの更新処理
//--------------------------------------------------------
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	XINPUT_STATE xInput;			//ゲームパッドの入力情報

									//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = START_CNT; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRelease[nCntKey] = (aKeyState[nCntKey] ^ g_aKeyState[nCntKey]) & g_aKeyState[nCntKey];//キーボードのリリース情報を保存する

			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//キーボードのトリガー情報を保存する

			g_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保存する
		}
	}

	else
	{
		//キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}

	//ゲームパッドから情報取得
	if (XInputGetState(0, &xInput) == ERROR_SUCCESS)
	{
		g_bUseGamepad = true;
	}
	else
	{
		g_bUseGamepad = false;
	}

	//ゲームパッドのリリース情報を保存する
	g_wGamePadStateRelease = (xInput.Gamepad.wButtons ^ g_xInput.Gamepad.wButtons) & g_xInput.Gamepad.wButtons;

	//ゲームパッドのトリガー情報を保存する
	g_wGamePadStateTrigger = (g_xInput.Gamepad.wButtons ^ xInput.Gamepad.wButtons) & xInput.Gamepad.wButtons;

	//ゲームパッドのプレス情報を保存する
	g_xInput.Gamepad.wButtons = xInput.Gamepad.wButtons;
}

//--------------------------------------------------------
//			キーボードの入力情報返答処理
//--------------------------------------------------------
//キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;		//３項演算子
}

//キーボードのトリガー情報を取得
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//キーボードのリリース情報を取得
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//--------------------------------------------------------
//			コントローラーの入力情報返答処理
//--------------------------------------------------------
XINPUT_STATE *GetXInputState(void)
{
	return &g_xInput;
}

//ゲームパッドのプレス情報
bool GetGamePadPress(int nPad)
{
	if (g_bUseGamepad == true)
	{
		return g_xInput.Gamepad.wButtons & nPad ? true : false;
	}
	else
	{
		return false;
	}
}

//ゲームパッドのトリガー情報
bool GetGamePadTrigger(int nPad)
{
	if (g_bUseGamepad == true)
	{
		return g_wGamePadStateTrigger & nPad ? true : false;
	}
	else
	{
		return false;
	}
}

//ゲームパッドのリリース情報
bool GetGamePadRelease(int nPad)
{
	if (g_bUseGamepad == true)
	{
		return g_wGamePadStateRelease & nPad ? true : false;
	}
	else
	{
		return false;
	}
}