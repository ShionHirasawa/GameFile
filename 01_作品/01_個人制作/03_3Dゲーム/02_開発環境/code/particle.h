/*==========================================================================================================================================================

														パーティクル処理[parthicle.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//パーティクル発生タイプ
typedef enum
{
	SET_PARTICLE_NORMAL = ZERO,		// 普通のパーティクル(発生量　１)
	SET_PARTICLE_SLOW_EX,			// ゆっくり移動するパーティクル(発生量 30)
	SET_PARTICLE_EXPLOSION,			// 爆発のようなパーティクル(発生量　30)
	SET_PARTICLE_MAX
}SET_PARTICLE;

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 位置(発生位置)
	D3DXVECTOR3		move;		// 移動量
	D3DXCOLOR		col;		// 色
	SET_PARTICLE	nType;		// 発生タイプ
	int				nLife;		// 寿命(発生時間)
	float			fRadius;	// パーティクルの半径
	bool			bUse;		// 使用しているかどうか
}Particle;

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticleRadius(int nCntParticle);

void SetParticle(D3DXVECTOR3 pos, SET_PARTICLE nType, D3DXCOLOR col);
void SetParticleNor(D3DXVECTOR3 pos, D3DXCOLOR col, SET_PARTICLE nType);
void SetParticleExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fSpeed, SET_PARTICLE nType);

void BoundParticle(int nCntParticle);

#endif