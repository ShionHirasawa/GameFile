//================================================================================================
//
//色に関する処理[color.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

//カラー列挙型
enum COL
{
	COL_CLEAR = 0,	/* 透明	*/
	COL_RED,		/*	赤	*/
	COL_GREEN,		/*	緑	*/
	COL_BLUE,		/*	青	*/
	COL_YELLOW,		/*	黄	*/
	COL_PURPLE,		/*	紫	*/
	COL_LightBLUE,	/* 水色	*/
	COL_WHITE,		/*	白	*/
	COL_BLACK,		/*	黒	*/
	COL_GRAY,		/* 灰色 */
	COL_EFFECT,		/*エフェクト用*/
	COL_MAX
};

//色を返す
D3DXCOLOR GetColor(COL col);

#endif