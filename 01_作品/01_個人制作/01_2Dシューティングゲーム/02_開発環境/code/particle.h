#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//パーティクルの種類構造体
typedef enum
{
	PARTICLETYPE_STAY = 0,		//その場で発生
	PARTICLETYPE_MOVE,			//移動する
	PARTICLETYPE_MAX
}ParticleType;

//プロトタイプ宣言
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, ParticleType nType);

#endif