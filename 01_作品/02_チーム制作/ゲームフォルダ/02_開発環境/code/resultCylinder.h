//--------------------------------------------------------------------------------------------------------
//
//リザルト用背景[resultCylinder.h]
//Author:平澤詩苑
//
//--------------------------------------------------------------------------------------------------------
#ifndef _RESULTCYLINDER_H_	//このマクロ定義がされてなかったら
#define _RESULTCYLINDER_H_	//2重インクルード防止のマクロを定義する

//プロトタイプ宣言
void InitResultCylinder(void);					// 初期化処理
void SetResultCylinderVertex(int nCntCylinder);	// 頂点情報の設定処理
void SetResultCylinderIndex(int nCntCylinder);	// インデックス番号の設定処理

void UninitResultCylinder(void);	// 終了処理
void UpdateResultCylinder(void);	// 更新処理
void DrawResultCylinder(void);		// 描画処理

#endif