//==========================================
//
//ポーズのヘッダ[pause.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//****************************//
//		ポーズの状態		  //
//****************************//
typedef enum
{
	PAUSE_CONTINUE = 1,
	PAUSE_RETRY,
	PAUSE_MODESELECT,
	PAUSE_QUIT,
	PAUSE_MAX
}PAUSE;

//****************************//
//		ポーズの構造体		  //
//****************************//
typedef struct
{
	D3DXVECTOR3 pos;			//位置
}Pause;

//****************************//
//		プロトタイプ宣言	  //
//****************************//
void InitPause(void);
void UninitPause(void);
void UpdatePause(MODE mode);
void DrawPause(void);
void SetPause(PAUSE Pause);
void SetPadPause(bool bDisconect, int GamePad = -1);
void SelectDownPause(int nPadNum);
void SelectUpPause(int nPadNum);
void UIStatePause(void);
PAUSE *GetPause(void);

#endif 
