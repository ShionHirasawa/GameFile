//==========================================
//
//プレイヤー選択のタイプ枠プログラムのヘッダ[TypeFrame.h]
//Author:石原颯馬
//
//==========================================
#ifndef _TYPEFRAME_H_
#define _TYPEFRAME_H_

#include "main.h"
#include "ai.h"

//プレイヤー種類列挙
typedef enum
{
	PLAYERTYPE_PLAYER = 0,
	PLAYERTYPE_COM,
	PLAYERTYPE_NONE,
	PLAYERTYPE_MAX
} PLAYERTYPE;

//プロトタイプ宣言
void InitTypeFrame(void);
void UninitTypeFrame(void);
void UpdateTypeFrame(void);
void DrawTypeFrame(void);
bool GetUsePlayer(int nPlayerNum);
AIDIFF GetAIDiff(void);

#endif // !_TYPEFRAME_H_