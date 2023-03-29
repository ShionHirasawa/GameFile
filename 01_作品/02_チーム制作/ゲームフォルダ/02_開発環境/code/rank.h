//==========================================
//
//ブロックプログラムのヘッダ[Block.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _RANK_H_
#define _RANK_H_

#include "main.h"

//プレイヤーの位置設定のため、ヘッダに移動しました
#define MAX_RANK			(4)		//最大数

//アイテム種類列挙型
typedef enum
{
	RANKING_1ST = 0,
	RANKING_2ND,
	RANKING_3RD,
	RANKING_4TH,
	RANKING_MAX
}RANKING;

//アイテムの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	bool		bDisp;	//表示するかどうか
}Rank;

//プロトタイプ宣言
void InitRank(void);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);

void UpperRank(void);
int JoinPlayer(void);
int SetRank(int nCntPlayer);

#endif
