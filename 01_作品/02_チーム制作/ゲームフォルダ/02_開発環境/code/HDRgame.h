/*==========================================================================================================================================================

ヒップドロップゲーム画面処理[HDRgame.cpp]
Author:藤原龍輝

============================================================================================================================================================*/
#ifndef _HDR_GAME_H_
#define _HDR_GAME_H_

#include "main.h"
#include "time.h"

//プロト
void InitHDRGame(void);
void UninitHDRGame(void);
void UpdateHDRGame(void);
void DrawHDRGame(void);
int SetUseController_HDR(void);
bool GetUseController_HDR(int nPadNum);
int GetUseControllerNum_HDR(void);
void SetEnablePause_HDR(bool pause);
bool CheckUseController_HDR(CHECKMODE mode);
void ChangeNumCamera_HDR(void);
float HipDropBuff(void);

#endif
