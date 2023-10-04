//================================================================================================
//
//�F�Ɋւ��鏈��[color.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "color.h"

/***	�F�A��	***/
#define HAVE	(1.0f)

/***	�F�����܂Ƃ߂�����	***/
const D3DXCOLOR g_ColorInfo[COL_MAX] = {
	D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),	/* ����	*/
	D3DXCOLOR(HAVE, 0.0f, 0.0f, HAVE),	/*	��	*/
	D3DXCOLOR(0.0f, HAVE, 0.0f, HAVE),	/*	��	*/
	D3DXCOLOR(0.0f, 0.0f, HAVE, HAVE),	/*	��	*/
	D3DXCOLOR(HAVE, HAVE, 0.0f, HAVE),	/*	��	*/
	D3DXCOLOR(HAVE, 0.0f, HAVE, HAVE),	/*	��	*/
	D3DXCOLOR(0.0f, HAVE, HAVE, HAVE),	/* ���F	*/
	D3DXCOLOR(HAVE, HAVE, HAVE, HAVE),	/*	��	*/
	D3DXCOLOR(0.0f, 0.0f, 0.0f, HAVE),	/*	��	*/
	D3DXCOLOR(0.5f, 0.5f, 0.5f, HAVE),	/* �D�F */
	D3DXCOLOR(HAVE, HAVE, 0.0f, 0.8f),	/*	�G�t�F�N�g	*/
};

//=============================
//�F��Ԃ�
//=============================
D3DXCOLOR GetColor(COL col)
{
	//�w�肳�ꂽ�F��Ԃ�
	return g_ColorInfo[col];
}