#ifndef _BULLET_H
#define _BULLET_H

#include "main.h"

//エフェクトのα値に関するマクロ定義
#define BULLETEFFECT_PLAYER			(0.5f)
#define BULLETEFFECT_ENEMY			(0.6f)
#define BULLETEFFECT_FAST_ENEMY		(0.7f)

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE type, HOMINGTYPE homing, float Alpha);
void CollisionEnemy(Bullet *pBullet);
void CollisionPlayer(Bullet *pBullet);

#endif