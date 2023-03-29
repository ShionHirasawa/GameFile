//==========================================
//
//色関係プログラム[color.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "color.h"

//==================================
//HSBからRGBに変換
//引数:hsb.h = 0~360  s = 0~255  b = 0~255
//==================================
D3DXCOLOR HSBToRGB(HSBCOLOR hsb)
{
	D3DXCOLOR temp;
	float max = (float)hsb.b / 255;
	float min = max - (((float)hsb.s / 255) * max);

	if (hsb.h <= 60)
	{
		temp.r = max;
		temp.g = ((float)hsb.h / 60) * (max - min) + min;
		temp.b = min;
	}
	else if (hsb.h <= 120)
	{
		temp.r = ((float)(120 - hsb.h) / 60) * (max - min) + min;
		temp.g = max;
		temp.b = min;
	}
	else if (hsb.h <= 180)
	{
		temp.r = min;
		temp.g = max;
		temp.b = ((float)(hsb.h - 120) / 60) * (max - min) + min;
	}
	else if (hsb.h <= 240)
	{
		temp.r = min;
		temp.g = ((float)(240 - hsb.h) / 60) * (max - min) + min;
	}
	else if (hsb.h <= 300)
	{
		temp.r = ((float)(hsb.h - 240) / 60) * (max - min) + min;
		temp.g = min;
		temp.b = max;
	}
	else if (hsb.h <= 360)
	{
		temp.r = max;
		temp.g = min;
		temp.b = ((float)(360 - hsb.h) / 60) * (max - min) + min;
	}

	return temp;
}