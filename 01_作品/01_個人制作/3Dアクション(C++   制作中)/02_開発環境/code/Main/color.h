//================================================================================================
//
//�F�Ɋւ��鏈��[color.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

//�J���[�񋓌^
enum COL
{
	COL_CLEAR = 0,	/* ����	*/
	COL_RED,		/*	��	*/
	COL_GREEN,		/*	��	*/
	COL_BLUE,		/*	��	*/
	COL_YELLOW,		/*	��	*/
	COL_PURPLE,		/*	��	*/
	COL_LightBLUE,	/* ���F	*/
	COL_WHITE,		/*	��	*/
	COL_BLACK,		/*	��	*/
	COL_GRAY,		/* �D�F */
	COL_EFFECT,		/*�G�t�F�N�g�p*/
	COL_MAX
};

//�F��Ԃ�
D3DXCOLOR GetColor(COL col);

#endif