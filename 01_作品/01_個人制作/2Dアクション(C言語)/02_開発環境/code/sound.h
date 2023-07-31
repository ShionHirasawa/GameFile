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
	SOUND_LABEL_MORUDAW = ZERO,	// BGM0
	SOUND_LABEL_TETRIS,			// BGM1
	SOUND_LABEL_HAKO001,		// ハコ1つ目
	SOUND_LABEL_HAKO002,		// ハコ2つ目
	SOUND_LABEL_HAKO003,		// ハコ3つ目
	SOUND_LABEL_HAKO004,		// ハコ4つ目
	SOUND_LABEL_HAKO005,		// ハコ5つ目
	SOUND_LABEL_THROW,			// 投げる
	SOUND_LABEL_SET,			// 置く
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
