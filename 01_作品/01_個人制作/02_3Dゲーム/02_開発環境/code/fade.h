/*==========================================================================================================================================================

														フェード画面処理[fade.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

//フェードの状態
typedef enum
{
	FADE_NONE = ZERO,	//何もしていない状態
	FADE_IN,		//フェードイン
	FADE_OUT,		//フェードアウト
	FADE_MAX
}FADE;

//プロトタイプ宣言
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);
FADE GetFade(void);

#endif