//--------------------------------------------------------------------------------------------------------
//
//メッシュシリンダーの地面の断面[meshcylinder.cpp]
//Author:菊地陽人
//
//--------------------------------------------------------------------------------------------------------
#ifndef _MESHCYLONDER_H_	//このマクロ定義がされてなかったら
#define _MESHCYLONDER_H_	//2重インクルード防止のマクロを定義する

//マクロ
#define MESHSYLINDER_WIDTH				(2000.0f)	//背景の広さ
#define MESHSYLINDER_HEIGHT				(8400.0f)	//背景の高さ
#define MESHSYLINDER_SPLIT				(32)		//背景の頂点数
#define MESHSYLINDER_TEX_RESOLUTION		(1.0f)		//背景の解像度

//プロトタイプ宣言
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif
