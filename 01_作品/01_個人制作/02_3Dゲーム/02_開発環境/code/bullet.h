/*==========================================================================================================================================================

															弾処理[bullet.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _BULLET_H
#define _BULLET_H

#include "main.h"

//弾の持ち主構造体
typedef enum
{
	BULLET_OWNER_PLAYER = ZERO,	//プレイヤーの弾
	BULLET_OWNER_CANNON,		//大砲の弾
	BULLET_OWNER_MAX
}BULLET_OWNER;

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		posOld;			// 前回の位置
	D3DXVECTOR3		move;			// 移動量
	D3DXVECTOR3		rot;			// 向き
	D3DXCOLOR		col;			// 色
	int				nLife;			// 寿命
	float			size;			// サイズ
	int				nIdxShadow;		// 対象の影のインデックス(番号)
	bool			bUse;			// 使用しているかどうか
	BULLET_OWNER	owner;			// オーナー
}Bullet;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void WholeBulletCollision(int nCntBullet);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size, BULLET_OWNER owner);

#endif