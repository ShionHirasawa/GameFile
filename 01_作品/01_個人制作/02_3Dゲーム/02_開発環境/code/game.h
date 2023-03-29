/*==========================================================================================================================================================

														ゲーム画面処理[game.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "time.h"

//マクロ定義
#define END_FADE		(60)		// 終了余韻
#define THE_END_HELL	(-400.0f)	// 奈落ポイント

//ゲーム画面を閉じるときの列挙型
typedef enum
{
	EndGAME_CLEAR = ZERO,	//ゲームクリア
	EndGAME_OVER,			//ゲームオーバー
	EndGAME_MAX,
}EndGAME;

//当たり判定の種類
typedef enum
{
	COLLITYPE_PLAYER = ZERO,	// プレイヤー
	COLLITYPE_BULLET,			// 弾
	COLLITYPE_ENEMY,			// 敵
	COLLITYPE_MAX
}COLLITYPE;

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void PlayMode(void);
void SetEndGame(EndGAME end);
int EndGameResult(void);

#endif