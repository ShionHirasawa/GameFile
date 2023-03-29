/*==========================================================================================================================================================

タイトル画面処理[title.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _TITLE_H_
#define _TITLE_H_

//フェードの状態
typedef enum
{
	TITLE_NONE = 0,
	TITLE_START,	
	TITLE_EXIT,		
	TITLE_MAX
}TITLE;

void InitTitle(void);
void LoadTitle(void);

void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

void DirecUpTitle(int NumPad);
void DirecDownTitle(int NumPad);

#endif