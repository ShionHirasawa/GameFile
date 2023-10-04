//================================================================================================
//
//色に関する処理[color.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "color.h"

/***	色アリ	***/
#define HAVE	(1.0f)

/***	色情報をまとめたもの	***/
const D3DXCOLOR g_ColorInfo[COL_MAX] = {
	D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),	/* 透明	*/
	D3DXCOLOR(HAVE, 0.0f, 0.0f, HAVE),	/*	赤	*/
	D3DXCOLOR(0.0f, HAVE, 0.0f, HAVE),	/*	緑	*/
	D3DXCOLOR(0.0f, 0.0f, HAVE, HAVE),	/*	青	*/
	D3DXCOLOR(HAVE, HAVE, 0.0f, HAVE),	/*	黄	*/
	D3DXCOLOR(HAVE, 0.0f, HAVE, HAVE),	/*	紫	*/
	D3DXCOLOR(0.0f, HAVE, HAVE, HAVE),	/* 水色	*/
	D3DXCOLOR(HAVE, HAVE, HAVE, HAVE),	/*	白	*/
	D3DXCOLOR(0.0f, 0.0f, 0.0f, HAVE),	/*	黒	*/
	D3DXCOLOR(0.5f, 0.5f, 0.5f, HAVE),	/* 灰色 */
	D3DXCOLOR(HAVE, HAVE, 0.0f, 0.8f),	/*	エフェクト	*/
};

//=============================
//色を返す
//=============================
D3DXCOLOR GetColor(COL col)
{
	//指定された色を返す
	return g_ColorInfo[col];
}