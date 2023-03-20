/*==========================================================================================================================================================

														色のマクロ定義[color.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

#define COLOR_MIN	(0)		// RGBACOLORの最小数
#define COLOR_MAX	(255)	// RGBACOLORの最大数

//D3DXCOLOR_RGBA のマクロ
#define RGBA_WHITE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX)	// 白
#define RGBA_BLACK		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// 黒
#define RGBA_RED		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// 赤
#define RGBA_GREEN		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MAX, COLOR_MIN, COLOR_MAX)	// 緑
#define RGBA_BLUE		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MAX, COLOR_MAX)	// 青

//D3DXCOLOR のマクロ
#define XCOL_NONE		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)	// 無色透明
#define XCOL_WHITE		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// 白
#define XCOL_BLACK		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	// 黒
#define XCOL_YELLOW		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)	// 黄色
#define XCOL_RED		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)	// 赤色

#define PARTICLE_COL	D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f)	// パーティクルの色

//モデルに使用するカラーマクロ
#define MODEL_APER_COL	D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f)	// 死亡時の色
#define MODEL_DEAD_COL	D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f)	// 出現時の色

//制限時間に使用するマクロ
#define TIMELIMIT_NORMAL_COLOR			(RGBA_BLACK)		// 制限時間100秒以上の通常色
#define TIMELIMIT_SPEED_COLOR			(RGBA_RED)			// 制限時間100秒未満の倍速色

#endif