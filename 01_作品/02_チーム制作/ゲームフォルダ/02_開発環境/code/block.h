//==========================================
//
//ブロックプログラムのヘッダ[Block.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//プレイヤーの位置設定のため、ヘッダに移動しました
#define MAX_BLOCK			(80)		//最大数
#define COLLISION_SIZE_Y	(80.0f)		//高さの当たり判定サイズ

//ブロック種類列挙型
typedef enum
{
	BLOCKTYPE_DIRT = 0,
	BLOCKTYPE_HARDDIRT,
	BLOCKTYPE_ROCK,
	BLOCKTYPE_MAX
}BLOCKTYPE;

//アイテムの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	BLOCKTYPE	type;			//種類
	int			nLife;			//体力
	bool		buse;			//使用しているかどうか
}Block;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(void);
void CollisionBlock(Player_HDR *pPlayer);//ブロックの当たり判定
void DestroyBlockSound(int nDestroyCounter, int nCntBlock);

#endif