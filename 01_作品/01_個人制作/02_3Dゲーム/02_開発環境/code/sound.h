//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 平澤詩苑
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLEBGM = 0,		// タイトルBGM
	SOUND_LABEL_GAMEBGM,			// ゲームBGM
	SOUND_LABEL_SE_TIME_ALARM,		// 制限時間のアラーム
	SOUND_LABEL_GAMEBGM_SPEED,		// ゲームBGM倍速
	SOUND_LABEL_SE_FILING,			// 弾発射音
	SOUND_LABEL_SE_HIT,				// ヒット音
	SOUND_LABEL_SE_EXPLOSION,		// 爆発音
	SOUND_LABEL_SE_DECISION,		// 決定音
	SOUND_LABEL_SE_PAGE_FlipLeft,	// 左にめくる
	SOUND_LABEL_SE_PAGE_FlipRight,	// 右にめくる
	SOUND_LABEL_SE_PAGE_Close,		// 閉じる
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
