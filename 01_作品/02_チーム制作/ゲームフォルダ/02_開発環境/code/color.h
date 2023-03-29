/*==========================================================================================================================================================

色のマクロ定義[color.h]
Author:平澤詩苑 石原颯馬

============================================================================================================================================================*/
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"
#include "input.h"

#define COLOR_MIN	(0)		// RGBACOLORの最小数
#define COLOR_HALF	(127)	// RGBACOLORの中間数
#define COLOR_MAX	(255)	// RGBACOLORの最大数

//D3DXCOLOR_RGBA のマクロ
#define RGBA_WHITE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX)	// 白
#define RGBA_BLACK		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// 黒
#define RGBA_RED		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// 赤
#define RGBA_ORANGE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_HALF,COLOR_MIN, COLOR_MAX)	// オレンジ
#define RGBA_GREEN		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MAX, COLOR_MIN, COLOR_MAX)	// 緑
#define RGBA_BLUE		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MAX, COLOR_MAX)	// 青
#define RGBA_YELLOW		D3DCOLOR_RGBA(COLOR_MAX, 246, COLOR_MIN, COLOR_MAX)			// 黄色
#define RGBA_RIGHTBLUE	D3DCOLOR_RGBA(COLOR_HALF,COLOR_HALF,COLOR_MAX, COLOR_MAX)	// ライトブルー

//D3DXCOLOR のマクロ
#define XCOL_NONE			D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)	// 無色透明
#define XCOL_WHITE			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// 白色
#define XCOL_BLACK			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	// 黒色
#define XCOL_BLACKSHADOW	D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.85f)	// 黒影
#define XCOL_YELLOW			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)	// 黄色
#define XCOL_RED			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)	// 赤色
#define XCOL_BLUE			D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)	// 青色
#define XCOL_GREEN			D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)	// 緑色
#define XCOL_NO_SELECT		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f)	// 選択されていない時の色

//自作の色構造体のマクロ
#define SELF_MADE_COLOR_WHITE		{255,255,255,255}	// 白
#define SELF_MADE_COLOR_BLACK		{000,000,000,255}	// 黒
#define SELF_MADE_COLOR_RED			{255,000,000,255}	// 赤
#define SELF_MADE_COLOR_GREEN		{000,255,000,255}	// 緑
#define SELF_MADE_COLOR_BLUE		{000,000,255,255}	// 青
#define SELF_MADE_COLOR_YELLOW		{255,255,000,255}	// 黄
#define SELF_MADE_COLOR_BLUE		{000,000,255,255}	// 緑
#define SELF_MADE_COLOR_SEMI_CLEAR	{255,255,255,100}	// 半透明

//HSB用色構造体
typedef struct
{
	int h;	//0～360
	int s;	//0～255
	int b;	//0～255
} HSBCOLOR;

//*****************************************
// 構造体の定義
//*****************************************
// 色構造体		
typedef struct
{
	int r;
	int g;
	int b;
	int a;
}Color;

//プロト
D3DXCOLOR HSBToRGB(HSBCOLOR hsb);

//プレイヤーのゼッケンの色
const D3DXCOLOR c_aColPlayer[MAX_USE_GAMEPAD] =
{
	XCOL_RED,
	XCOL_BLUE,
	XCOL_GREEN,
	XCOL_YELLOW
};

#endif