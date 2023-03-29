//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���V����
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_MORUDAW = ZERO,	// BGM0
	SOUND_LABEL_TETRIS,			// BGM1
	SOUND_LABEL_HAKO001,		// �n�R1��
	SOUND_LABEL_HAKO002,		// �n�R2��
	SOUND_LABEL_HAKO003,		// �n�R3��
	SOUND_LABEL_HAKO004,		// �n�R4��
	SOUND_LABEL_HAKO005,		// �n�R5��
	SOUND_LABEL_THROW,			// ������
	SOUND_LABEL_SET,			// �u��
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
