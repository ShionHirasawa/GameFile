/*==========================================================================================================================================================

												キーボード・コントローラー処理[main.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
XINPUT_STATE *GetXInputState(void);
bool GetGamePadPress(int nKey);
bool GetGamePadTrigger(int nKey);
bool GetGamePadRelease(int nKey);

#endif