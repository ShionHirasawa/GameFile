/*==========================================================================================================================================================

															壁の処理[wall.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _WALL_H_
#define _WALL_H_

//壁の方向構造体
typedef enum
{
	WallDirection_D = ZERO,	// 奥向き
	WallDirection_R,		// 右向き
	WallDirection_F,		// 前向き
	WallDirection_L,		// 左向き
	WallDirection_MAX,
}WallDirection;

//壁の情報構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	float			fWidth;		// 幅
	float			fHeight;	// 高さ
	float			fDepth;		// 奥行き
	D3DXVECTOR2		fTexPos;	// テクスチャの開始座標
	float			fTexWidth;	// テクスチャの幅
	float			fTexHeight;	// テクスチャの高さ
	int				nTexType;	// テクスチャの種類
	WallDirection	Direction;	// 壁が向いてる方向
	bool			bCollitype;	// 当たり判定を行うかどうか
}Wall;

void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
Wall *GetWall(void);
void LoadWall(void);
bool WallDrawObject(D3DXVECTOR3 pos);

#endif 