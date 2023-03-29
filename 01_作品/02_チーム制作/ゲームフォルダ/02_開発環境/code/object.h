//==========================================
//
//何も効果を持たないオブジェクトプログラムのヘッダ[fence.h]
//Author:石原颯馬
//
//==========================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "model.h"

//マクロ
#define MAX_OBJECT		(128)	//オブジェクト配置最大数

//フェンス用構造体
typedef struct
{
	//位置関係
	D3DXVECTOR3 pos, posOld;	//位置・前回位置
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//向き
	int nFallTime;				//落下時間

	//描画関係
	OBJECTTYPE type;			//使用オブジェクト
	D3DXMATRIX mtxWorld;		//ワールドマトリ

	//使用有無
	bool bUse;					//使用有無

} Object;

//プロトタイプ宣言
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);

void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type);
Object *GetObjectInfo(void);

#endif // !_FENCE_H_