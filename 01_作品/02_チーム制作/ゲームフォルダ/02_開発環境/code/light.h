//--------------------------------------------------------------------------------------------------------
//
//ライト処理[light.h]
//Author:菊地陽人
//
//--------------------------------------------------------------------------------------------------------
#ifndef _LIGHT_H_	//このマクロ定義がされてなかったら
#define _LIGHT_H_	//2重インクルード防止のマクロを定義する

#include"main.h"

//プロトタイプ宣言
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);
void SetLight(int nLightNum, D3DLIGHT9 light);

#endif
