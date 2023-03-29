//--------------------------------------------------------------------------------------------------------
//
//メッシュの断面図[meshfault.h]
//Author:菊地陽人	平澤詩苑
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHFAULT_H_	//このマクロ定義がされてなかったら
#define _MESHFAULT_H_	//2重インクルード防止のマクロを定義する

//マクロ
#define MESHFAULT_BOTTOM			(-600.0f)	//背景の足元
#define MESHFAULT_SPLIT				(32)		//背景の頂点数
#define MESHFAULT_TEX_RESOLUTION	(4.0f)		//背景の解像度

//断面情報の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	float			fRadius;	//半径の大きさ
}Fault;

//プロトタイプ宣言
void InitMeshFault(void);
void SetMeshFaultVertex(void);
void SetMeshFaultIndex(void);
void UninitMeshFault(void);
void UpdateMeshFault(void);
void DrawMeshFault(void);

#endif