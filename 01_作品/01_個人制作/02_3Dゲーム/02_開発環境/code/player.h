/*==========================================================================================================================================================

														プレイヤーの処理[player.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "bullet.h"
#include "meshfield.h"

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_NORMAL = ZERO,	//通常
	PLAYERSTATE_DAMEGE,			//ダメージ
	PLAYERSTATE_APPEAR,			//出現
	PLAYERSTATE_DEAD,			//死亡
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3		pos;					//位置
	D3DXVECTOR3		posOld;					//前回の位置
	D3DXVECTOR3		move;					//移動量
	D3DXVECTOR3		rot;					//向き
	PLAYERSTATE		state;					//プレイヤーの状態
	MeshField		*pField;				//フィールドの情報
	int				nStateCounter;			//状態カウンター
	int				nLife;					//体力
	bool			bLand;					//乗っかっているかどうか
	bool			bJump;					//ジャンプ中かどうか
	bool			bDisp;					//表示するかどうか
}Player;

void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

void SwitchPlayerState(void);
void MovePlayer(void);
void PlayerGravity(void);
void FixPlayerMove(void);
void InertiaPlayerMove(void);	// Inertia は「慣性」という意味
void FixPlayerPos(void);
void RotPlayer(void);
void ResetPlayer(void);
bool ColliPlayerToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner);
void WholePlayerCollision(void);// プレイヤーの当たり判定まとめ Whole は「全体」という意味

#endif 