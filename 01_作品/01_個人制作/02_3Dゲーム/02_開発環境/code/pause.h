/*==========================================================================================================================================================

														ポーズ画面処理[pause.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//ポーズメニューの構造体
typedef enum
{
	PAUSE_MENU_BG = ZERO,		//ポーズ画面の背景
	PAUSE_MENU_CONTINUE,		//コンティニュー(ポーズ画面を終了)する
	PAUSE_MENU_RETRY,			//ゲームをやり直す
	PAUSE_MENU_TITLE,			//ゲームを終了する(ローディング画面に戻る)
	PAUSE_MENU_MAX
}PAUSE_MENU;

//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SelectPauseMenu_Down(void);	// ポーズメニューの下の項目にカーソルを合わせる
void SelectPauseMenu_UP(void);		// ポーズメニューの上の項目にカーソルを合わせる
void SelectPauseMenu_Decide(void);	// ポーズメニューの項目を決定
PAUSE_MENU *GetPause(void);
void SetPause(PAUSE_MENU SetPause);
bool GetDispPause(void);

#endif