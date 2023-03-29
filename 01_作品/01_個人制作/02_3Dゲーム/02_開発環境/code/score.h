/*==========================================================================================================================================================

スコア表示処理[score.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

#define DEJIT_DIVISION			(10)								//桁の位を下げる

//ライフの情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int			nScore;		//スコア
	int			Dejit;		//桁
}Score;

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void FixDejit(void);
Score *GetScore(void);

#endif