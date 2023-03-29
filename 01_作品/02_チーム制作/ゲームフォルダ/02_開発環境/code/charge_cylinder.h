//--------------------------------------------------------------------------------------------------------
//
//メッシュのチャージシリンダー[chargecylinder.h]
//Author:平澤詩苑　飯田洲暉
//
//--------------------------------------------------------------------------------------------------------
#ifndef _CHARGECYLINDER_H_	//このマクロ定義がされてなかったら
#define _CHARGECYLINDER_H_	//2重インクルード防止のマクロを定義する

//チャージシリンダー情報の構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		rot;		// 向き
	float			fRadius;	// 半径の大きさ
	int				nPlayerNo;	// 呼び出したプレイヤー番号を記憶
	bool			bUse;		// 使われているかどうか
}ChargeCylinder;

//プロトタイプ宣言
void InitChargeCylinder(void);					// 初期化処理
void SetChargeCylinderVertex(int nCntCylinder);	// 頂点情報の設定処理
void SetChargeCylinderIndex(int nCntCylinder);	// インデックス番号の設定処理

void UninitChargeCylinder(void);			// 終了処理

void UpdateChargeCylinder(void);						// 更新処理
void SetChargeCylinderPos(ChargeCylinder *pCylinder);	// プレイヤーの位置に設定する
void SpreadChargeCylinder(ChargeCylinder *pCylinder);	// チャージシリンダーの広がる処理

void DrawChargeCylinder(void);		// 描画処理

void SetChargeCylinder(D3DXVECTOR3 pos, int nPlayerNo);		// シリンダーを設定

#endif