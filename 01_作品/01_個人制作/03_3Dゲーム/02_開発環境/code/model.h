/*==========================================================================================================================================================

															モデル処理[model.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _MODEL_H_
#define _MODEL_H_

#include "game.h"

//モデルの種類
typedef enum
{
	MODELTYPE_WOODEN_BLOCK = ZERO,	// 木箱
	MODELTYPE_WOODEN_BLOCKs,		// 木箱群
	MODELTYPE_HANACHAN,				// ハナちゃん
	MODELTYPE_MAX
}MODELTYPE;

//モデルの状態構造体
typedef enum
{
	MODELSTATE_NORMAL = ZERO,	// 通常
	MODELSTATE_APPEAR,			// 出現
	MODELSTATE_DAMAGE,			// ダメージ状態
	MODELSTATE_NONE,			// 消えている(居ない)
	MODELSTATE_MAX
}MODELSTATE;

//モデルの構造体
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		PosOld;			// 前回の位置
	D3DXVECTOR3		move;			// 移動量
	D3DXVECTOR3		MinPos;			// 最小の位置
	D3DXVECTOR3		MaxPos;			// 最大の位置
	D3DXVECTOR3		rot;			// 向き
	D3DXCOLOR		col;			// 色
	int				nIdxShadow;		// 対象の影のインデックス(番号)
	int				nLife;			// 寿命
	int				nPoint;			// 保有するポイント
	int				nStateCounter;	// 状態のカウンタ
	MODELSTATE		state;			// 状態
	MODELTYPE		type;			// 種類
	bool			bDisp;			// 表示するかどうか
	bool			bUse;			// 使用しているかどうか
}Model;

//消失したモデルの構造体
typedef struct
{
	int			nDeadModelCount;	// 消失しているモデルの数
	int			nCount;				// ゲーム終了までのカウント
}VanishModel;

void InitModel(void);
void LoadModel(void);
void SearchModel_MinAndMax_Pos(void);
void SetModel_Max_Min_Pos(int nCntModel);
void UninitModel(void);
void UpdateModel(void);
void FallModel(int nCntModel);
void DrawModel(void);

void SwitchStateModel(int nCntModel);
void DeadCountModel(void);

void ColliModel_to_Model(int nCntTarget);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType);
bool ColliModel_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType);
bool ColliModel_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType);
void ColliModelShadow(int nIdxShadow, D3DXVECTOR3 pos, float *pDrawPosY);
Model *GetModel(void);
VanishModel *GetEndModel(void);

#endif