/*==========================================================================================================================================================

制限時間処理[timelimit.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _TIMELIMIT_H_
#define _TIMELIMIT_H_

#include "main.h"

//ライフの情報構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	int			nTime;			//残り時間
	int			nDejit;			//桁数
	bool		bSoundAlarm;	//アラーム音を再生したか
	int			nFromAlarm;		//アラーム音を再生してから経過した時間
	bool		bSoundSpeed;	//サウンドスピード判定
}TimeLimit;

//プロトタイプ宣言
void InitTimeLimit(void);		void SetTimeLimit_Vertex(void);
void UninitTimeLimit(void);
void UpdateTimeLimit(void);		void TimeLimit_Sound_Change(void);		void Timelimit_TheEnd(void);
void DrawTimeLimit(void);
TimeLimit *GetTimeLimit(void);

#endif