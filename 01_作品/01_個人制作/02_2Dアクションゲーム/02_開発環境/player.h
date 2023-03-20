#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "block.h"

#define JUMP_MOVE			(-15.0f)	//ジャンプの高さ

//プレイヤーの状態構造体
typedef enum
{
	PLAYERSTATE_APPEAR = ZERO,	//出現状態（点滅）
	PLAYERSTATE_NORMAL,			//通常状態
	PLAYERSTATE_DELIVERY,		//ハコ産み出し中
	PLAYERSTATE_CARRY,			//ハコを運び中
	PLAYERSTATE_THROW,			//ハコ投げ中
	PLAYERSTATE_GOAL,			//ゴールした
	PLAYERSTATE_DAMAGE,			//ダメージ死
	PLAYERSTATE_FALL,			//落下死
	PLAYERSTATE_DEATH,			//死亡
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーの移動向き構造体
typedef enum
{
	PLAYER_ROT_FORWARD = ZERO,	//正面(待機モーション)
	PLAYER_ROT_RIGHT,			//移動方向　右
	PLAYER_ROT_LEFT,			//移動方向　左
	PLAYER_ROT_MAX
}PLAYER_ROT;

//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;			//現在の位置
	D3DXVECTOR3 posOld;			//前回の位置
	D3DXVECTOR3 move;			//移動量
	PLAYERSTATE state;			//プレイヤーの状態
	int AppearCnt;				//点滅表示時間のカウント
	PLAYER_ROT Rot;				//プレイヤーの向き
	PLAYER_ROT OldRot;			//プレイヤーの前回の向き
	int nCounterAnim;			//アニメーションカウンター
	int nPatternAnim;			//アニメーションパターンNo.
	int nCounterState;			//状態管理カウント
	int DeathCounter;			//死亡カウント
	float fWidth;				//幅の長さ(中心から左右に分かれるので端から端の半分に）
	float fHeight;				//高さの長さ
	bool bJump;					//ジャンプ中かどうか
	bool bOnBlock;				//ブロックに乗ったかどうか
	bool bOnHako;				//ハコに乗ったかどうか
	bool bSideBlock;			//ハコに横から当たったか
	Block *pBlock;				//対象ブロックへのポインタ
	bool bDisp;					//プレイヤーを表示ON/OFF
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void FirstUpdatePlayer(void);
void MovePlayer(void);
void CollisionAndPos_x(void);
void CollisionAndPos_y(void);
void HitPlayer(PLAYERSTATE state);

#endif