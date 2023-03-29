/*==========================================================================================================================================================

ゲージの処理[gauge.h]
Author:大宮愛羅

============================================================================================================================================================*/
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

//ゲージの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	bool bUse;				//使用しているかどうか
}Gauge;

//ゲージの列挙
typedef enum
{
	GAUGETYPE_BG = 0,
	GAUGETYPE_NORMAL,
	GAUGETYPE_FRAME,
	GAUGETYPE_MAX
}GAUGETYPE;

void InitGauge(void);		//初期化処理
void UninitGauge(void);		//終了処理
void UpdateGauge(void);		//更新処理
void DrawGauge(void);		//描画処理

#endif