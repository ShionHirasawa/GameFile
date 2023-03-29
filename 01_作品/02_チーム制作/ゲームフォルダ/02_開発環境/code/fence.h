//==========================================
//
//落下防止フェンスプログラムのヘッダ[fence.h]
//Author:石原颯馬  平澤詩苑
//
//==========================================
#ifndef _FENCE_H_
#define _FENCE_H_

#include "main.h"

//マクロ
#define MAX_USE_FENCE		(48)		//フェンス最大使用数
#define FENCE_SCALE			(4.5f)		//フェンスの拡大倍率
#define FENCE_WIDTH			(12.0f)		//フェンスの幅
#define FENCE_DEPTH			(1.0f)		//フェンスの高さ
#define FENCE_RADIUS		(650.0f)	//フェンスの位置

//フェンス用構造体
typedef struct
{
	//位置関係
	D3DXVECTOR3 pos, posOld;	//位置・前回位置
	D3DXVECTOR3 rot;			//向き
	float		fRadius;		//原点位置からの距離
	float		fDropSpeed;		//落下速度
	float		fFallSpeed;		//倒れる速度

	//描画関係
	D3DXMATRIX mtxWorld;		//ワールドマトリ

	//使用有無
	bool bUse;					//使用有無
	bool bCollision;			//当たり判定有無
} Fence;

//プロトタイプ宣言
void InitFence(void);
void UninitFence(void);
void UpdateFence(void);
void DrawFence(void);

void SetFencePos(D3DXVECTOR3 *pPos, float fRadius, float rotY);	//フェンスの位置設定
void DropFence(Fence *pFence, float FieldRadius);				//フェンスの落下処理
void FallFence(Fence *pFence, float FieldRadius);				//フェンスの倒れる処理
void SetFence(float fRadius, float rotY);
Fence *GetFence(void);

#endif // !_FENCE_H_
