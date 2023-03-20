#ifndef _BLOCK_H_
#define _BLOCK_H_

//ブロックの性能構造体
typedef enum
{
	BLOCK_CAPACITY_STAY = ZERO,		//動かない
	BLOCK_CAPACITY_MOVE,			//動く
	BLOCK_CAPACITY_CARRY,			//持ち運べる
	BLOCK_CAPACITY_MAX
}BLOCK_CAPACITY;

//ブロックの構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;			//位置
	float fWidth;				//幅
	float fHeight;				//高さ
	int nType;					//ブロックの種類
	D3DXVECTOR2 Tex;			//テクスチャの座標
	bool bUse;					//使用されているかどうか
}Block;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(void);
bool X_CollisionBlockToPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove,float fWidth, float fHeight, Block **pBlock);
bool Y_CollisionBlockToPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove,float fWidth, float fHeight, Block **pBlock);
bool CollisionBlockToHako(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight, Block **pBlock);
void FixHakoPos(D3DXVECTOR3 FixMove);
#endif