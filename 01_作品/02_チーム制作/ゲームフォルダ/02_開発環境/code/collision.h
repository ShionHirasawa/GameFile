//==========================================
//
//当たり判定プログラムのヘッダ[collision.h]
//Author:石原颯馬
//
//==========================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"
#include "PvP_player.h"
#include "hdr_player.h"

//プロトタイプ宣言
bool CollisionPP(Player *pATKPlayer, float fWidth, float fHeight, float fDepth);										//プレイヤー同士の衝突判定
bool CollisionHipDropPP(Player *pATKPlayer, float fWidth, float fHeight, float fDepth, float fRadius, float fPower);	//ヒップドロップ時の衝突判定
bool CollisionFence(Player *pPlayer, float fFenceWidth, float fPlayerHeight, float fFenceDepth, float fenceSize);		//フェンスの衝突処理
void CollisionFence(Player_HDR *pPlayer);
void CollisionObject_Tree(Player *pPlayer);

#endif // !_COLLISION_H_
