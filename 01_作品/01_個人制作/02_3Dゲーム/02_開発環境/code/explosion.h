/*==========================================================================================================================================================

														爆発処理[explosion.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "main.h"

//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif