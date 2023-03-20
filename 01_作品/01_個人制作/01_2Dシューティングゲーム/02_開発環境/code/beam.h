#ifndef _BEAM_H_
#define _BEAM_H_

#include "main.h"

//プロトタイプ宣言
void InitBeam(void);
void UninitBeam(void);
void UpdateBeam(void);
void DrawBeam(void);
void SetBeam(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col);
void CollisionEnemy_Beam(Beam *pBeam);
void CollisionPlayer_Beam(Beam *pBeam);

#endif