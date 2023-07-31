#ifndef _PLAYERBLOCK_H_
#define _PLAYERBLOCK_H_

#include "block.h"

//自分が生み出したブロックの構造体
typedef enum
{
	HAKO_NONE = ZERO,	//生み出されていない
	HAKO_SPAWN,			//ブロックを生み出す
	HAKO_CARRY,			//持ち運べる
	HAKO_SET,			//動く	
	HAKO_MAX
}HAKO;

//産み出された方向
typedef enum
{
	DIRECTION_OVER  = ZERO,
	DIRECTION_UNDER ,
	DIRECTION_LEFT  ,
	DIRECTION_RIGHT ,
	DIRECTION_MAX
}DIRECTION;

//ハコの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//位置
	D3DXVECTOR3 move;		//移動量
	float fWidth;			//幅
	float fHeight;			//高さ
	HAKO myBlockState;		//自分が作ったブロックの状態
	DIRECTION nDirect;		//方向
	Block *pBlock;			//対象ブロックへのポインタ
	int nCol;				//カラー
	bool bShade;			//カラーの折り返し
	bool bUse;				//使用されているかどうか
}Hako;

//プロトタイプ宣言
void InitHako(void);
void UninitHako(void);
void UpdateHako(void);
void DrawHako(void);
void SetHako(void);
void X_CollisionHako(void);
bool Y_CollisionHako(void);
Hako *GetHako(void);
void HakoMove(float fHakoMove);
void ConnectSide(D3DXVECTOR3 ParentPos);
void HakoSpawnSound(void);

//更新処理内で行う処理
void FadeHako(void);
void ResetHako(void);
void LiftUpHako(void);
void CarryHako(void);
void GravityHako(void);
void SwitchHako(int nSwitchNum);
void FixHakoCol(void);

#endif