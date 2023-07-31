#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//ポーズメニューの構造体
typedef enum
{
	PAUSE_MENU_BG = ZERO,		//ポーズ画面の背景
	PAUSE_MENU_CONTINUE,		//コンティニュー(ポーズ画面を終了)する
	PAUSE_MENU_RETRY,			//ゲームをやり直す
	PAUSE_MENU_LOAD,			//ゲームを終了する(ローディング画面に戻る)
	PAUSE_MENU_MAX
}PAUSE_MENU;

//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
PAUSE_MENU *GetPause(void);
void SetPause(PAUSE_MENU SetPause);

#endif