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
	SOUND_LABEL_TITLEBGM = 0,		// �^�C�g��BGM
	SOUND_LABEL_GAMEBGM,			// �Q�[��BGM
	SOUND_LABEL_SE_TIME_ALARM,		// �������Ԃ̃A���[��
	SOUND_LABEL_GAMEBGM_SPEED,		// �Q�[��BGM�{��
	SOUND_LABEL_SE_FILING,			// �e���ˉ�
	SOUND_LABEL_SE_HIT,				// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,		// ������
	SOUND_LABEL_SE_DECISION,		// ���艹
	SOUND_LABEL_SE_PAGE_FlipLeft,	// ���ɂ߂���
	SOUND_LABEL_SE_PAGE_FlipRight,	// �E�ɂ߂���
	SOUND_LABEL_SE_PAGE_Close,		// ����
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
