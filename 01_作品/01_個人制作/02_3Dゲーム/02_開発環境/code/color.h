/*==========================================================================================================================================================

														�F�̃}�N����`[color.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

#define COLOR_MIN	(0)		// RGBACOLOR�̍ŏ���
#define COLOR_MAX	(255)	// RGBACOLOR�̍ő吔

//D3DXCOLOR_RGBA �̃}�N��
#define RGBA_WHITE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX)	// ��
#define RGBA_BLACK		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// ��
#define RGBA_RED		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MIN, COLOR_MIN, COLOR_MAX)	// ��
#define RGBA_GREEN		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MAX, COLOR_MIN, COLOR_MAX)	// ��
#define RGBA_BLUE		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MAX, COLOR_MAX)	// ��

//D3DXCOLOR �̃}�N��
#define XCOL_NONE		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)	// ���F����
#define XCOL_WHITE		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// ��
#define XCOL_BLACK		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	// ��
#define XCOL_YELLOW		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)	// ���F
#define XCOL_RED		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)	// �ԐF

#define PARTICLE_COL	D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f)	// �p�[�e�B�N���̐F

//���f���Ɏg�p����J���[�}�N��
#define MODEL_APER_COL	D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f)	// ���S���̐F
#define MODEL_DEAD_COL	D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f)	// �o�����̐F

//�������ԂɎg�p����}�N��
#define TIMELIMIT_NORMAL_COLOR			(RGBA_BLACK)		// ��������100�b�ȏ�̒ʏ�F
#define TIMELIMIT_SPEED_COLOR			(RGBA_RED)			// ��������100�b�����̔{���F

#endif