/*==========================================================================================================================================================

															坂道の処理[slope.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _SLOPE_H_
#define _SLOPE_H_

#include "game.h"

//坂道の方向
typedef enum
{
	SLOPEROT_FRONT_BACK = ZERO,		//前後
	SLOPEROT_RIGHT_LEFT,			//左右
	SLOPEROT_MAX
}SLOPEROT;

//坂道の情報構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	float			fWidth;		//幅
	float			fHeight;	//高さ
	float			fDepth;		//奥行き
	SLOPEROT		sloperot;	//坂道の方向
}Slope;

void InitSlope(void);
void UninitSlope(void);
void UpdateSlope(void);
void DrawSlope(void);
bool CollisionSlope(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, COLLITYPE nType);
void ColliSlopeShadow(D3DXVECTOR3 pos, D3DXVECTOR3 *pRot, float *pDrawPosY, float *pHeight);
Slope *GetSlope(void);
void LoadSlope(void);

#endif