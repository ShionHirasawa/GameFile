/*==========================================================================================================================================================

ヒップドロップのランクUI処理[HipDropRankUI.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _HIP_DROP_RANK_UI_H_
#define _HIP_DROP_RANK_UI_H_

//ヒップドロップレベルに必要な高さ（その値以上次のレベルの値以下）
#define HIPDROP_HEIGHT_NICE		(10.0f)			//レベル1
#define HIPDROP_HEIGHT_GOOD		(60.0f)			//レベル2
#define HIPDROP_HEIGHT_GREAT	(120.0f)		//レベル3
#define HIPDROP_HEIGHT_PERFECT	(145.0f)		//レベルMAX

//ヒップドロップレベルに応じた強さ
#define HIPDROP_POWER_NICE		( 100)			//レベル1  （ 100ダメージ）
#define HIPDROP_POWER_GOOD		( 200)			//レベル2  （ 300ダメージ）
#define HIPDROP_POWER_GREAT		( 500)			//レベル3  （ 600ダメージ）
#define HIPDROP_POWER_PERFECT	(1000)			//レベルMAX（1000ダメージ）

//ヒップドロップレベル列挙型
typedef enum
{
	HIPDROP_RANK_BAD = 0,
	HIPDROP_RANK_NICE,
	HIPDROP_RANK_GOOD,
	HIPDROP_RANK_GREAT,
	HIPDROP_RANK_PERFECT,
	HIPDROP_RANK_MAX
}HIPDROP_RANK;

//ランクUIの構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	float		fWidth;	// 幅
	float		fHeight;// 高さ
	int			nLife;	// 寿命
	bool		bDisp;	// 表示するか否か
}HDRankUI;

//プロトタイプ宣言
void InitHipDropRankUI(void);
void InitHipDropRankUIVertex(void);
void UninitHipDropRankUI(void);
void UpdateHipDropRankUI(void);
void DrawHipDropRankUI(void);
void SetRankUI(int nNumUI, int nTexHeight);

#endif