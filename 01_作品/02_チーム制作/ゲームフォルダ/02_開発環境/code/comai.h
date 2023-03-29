//==========================================
//
//コンピューターのAIプログラムのヘッダ[comai.h]
//Author:石原颯馬
//
//==========================================
#ifndef _COMAI_H_
#define _COMAI_H_

#include "main.h"
#include "PvP_player.h"
#include "ai.h"

//プロトタイプ宣言
void InitComAI(void);
void UninitComAI(void);
void SelectAIMove(Player *pCom);
ComAIBrain *GetAI(AIDIFF diff);

#endif // !_COMAI_H_
