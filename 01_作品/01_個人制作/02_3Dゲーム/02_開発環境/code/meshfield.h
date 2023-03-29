/*==========================================================================================================================================================

													メッシュフィールド処理[meshfield.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _MESHFIELD_H
#define _MESHFIELD_H

#include "main.h"

#define MESHFIELD_SIZE				(600.0f)	//フィールドの基本サイズ

typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	float			fWidth;		// 幅
	float			fDepth;		// 奥行き
	D3DXVECTOR2		fTexPos;	// テクスチャの開始座標
	float			fTexWidth;	// テクスチャの幅
	float			fTexHeight;	// テクスチャの高さ
	int				nTexType;	// テクスチャの種類
}MeshField;

//プロトタイプ宣言
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
MeshField *GetMeshField(void);
bool ColliMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, MeshField **pField);
void ColliFieldShadow(D3DXVECTOR3 Pos, float *pDrawPosY);

void LoadMeshField(void);

#endif