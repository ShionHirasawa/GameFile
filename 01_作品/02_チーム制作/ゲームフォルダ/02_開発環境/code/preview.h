/*==========================================================================================================================================================

プレイヤーの位置プレビュー処理[preview.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _PREVIEW_H_
#define _PREVIEW_H_

//ランクUIの構造体
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	float		fPlayerUIHeight;	// プレイヤーUIを表示する高さ
	float		fHeightPer;			// プレイヤーUIを表示する高さのパーセンテージ（START時：100％　　GOAL時：0％）
}Preview;

//プロトタイプ宣言
void InitPreview(void);
void UninitPreview(void);
void UpdatePreview(void);
void DrawPreview(void);

void FixPlayerUIHeight(int nNumPreview);

#endif