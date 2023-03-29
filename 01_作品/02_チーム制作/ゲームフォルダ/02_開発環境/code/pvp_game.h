/*==========================================================================================================================================================

ÉQÅ[ÉÄâÊñ èàóù[game.cpp]
Author:ïΩ‡Véçâë

============================================================================================================================================================*/
#ifndef _PVP_GAME_H_
#define _PVP_GAME_H_

#include "main.h"
#include "time.h"

//ÉvÉçÉg
void InitPvPGame(void);
void UninitPvPGame(void);
void UpdatePvPGame(void);
void DrawPvPGame(void);
int SetUseController_PvP(void);
bool GetUseController_PvP(int nPadNum);
int GetUseControllerNum_PvP(void);
void SetEnablePause_PvP(bool pause);
bool CheckUseController_PvP(CHECKMODE mode);
void ChangeNumCamera_PvP(void);

#endif