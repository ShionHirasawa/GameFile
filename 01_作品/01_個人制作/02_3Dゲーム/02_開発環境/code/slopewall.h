/*==========================================================================================================================================================

															坂道の壁の処理[slopewall.h]
																Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _SLOPEWALL_H_
#define _SLOPEWALL_H_

#include "slope.h"

//坂道の壁の情報構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	float			fWidth;		//幅
	float			fHeight;	//高さ
	float			fDepth;		//奥行き
	SLOPEROT		sloperot;	//坂道の向き
}SlopeWall;

void InitSlopeWall(void);
void UninitSlopeWall(void);
void UpdateSlopeWall(void);
void DrawSlopeWall(void);
bool CollisionSlopeWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
SlopeWall *GetSlopeWall(void);
void LoadSlopeWall(void);

#endif