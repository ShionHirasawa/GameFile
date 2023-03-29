/*==========================================================================================================================================================

//ゲームメニューの選択処理[select_game.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _SELECT_GAME_H_
#define _SELECT_GAME_H_

//選択できるゲームモード
typedef enum
{
	SelGameMode_PVP = 0,	//落とし合うゲーム
	SelGameMode_HDR,		//ヒップドロップレースゲーム
	SelGameMode_MAX
}SelGameMode;

//ゲーム選択情報の構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float		fWidth;		// 幅
	float		fHeight;	// 高さ
	bool		bChoice;	// 選択されているかどうか
}SelectGame;

//プロトタイプ宣言
void InitSelectGame(void);
void LoadSelectGame(void);
void SetVertexSelectGame(void);

void UninitSelectGame(void);
void UpdateSelectGame(void);
void DrawSelectGame(void);

void ChooseGameMode(void);
void DecideGameMode(void);
SelGameMode GetSelGameMode(void);

#endif 