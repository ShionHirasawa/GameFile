/*==========================================================================================================================================================

														エフェクト処理[effect.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _EFFECT_H
#define _EFFECT_H

#include "main.h"

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);

#endif