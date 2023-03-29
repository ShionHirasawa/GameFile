//==========================================
//
//AIのヘッダ[ai.h]
//Author:石原颯馬
//
//==========================================
#ifndef _AI_H_
#define _AI_H_

#include "main.h"
#include "input.h"

//AIのパラメータ種類（もとい難易度）
typedef enum
{
	AIDIFF_EASY = 0,
	AIDIFF_NORMAL,
	AIDIFF_HARD,
	AIDIFF_VIEW,
	AIDIFF_MAX
} AIDIFF;

//AIのパラメータ
typedef struct
{
	float fChargeAboutPlus;	//チャージのアバウト具合（プラス）
	float fChargeAboutMinus;//チャージのアバウト具合（マイナス）
	int nHipdropRandom;		//ヒップドロップ回避する確率
	int nHipdropTime;		//ジャンプ→ヒップドロップまでの時間
} AIParam;

//AI（HDR）のパラメーター
typedef struct
{
	int nHDLevel_1;
	int nHDLevel_2;
	int nHDLevel_3;
	int nHDLevel_Max;
	int nAICT;
} AIParam_HDR;

//コンピューターAIのパラメータ構造体
typedef struct
{
	//脳みそ部
	AIDIFF difficulty;		//AIの難易度

	//チャージ系
	float fChargePower;		//チャージ量
	int nCounterWaitTime;	//チャージ待ち時間

	//ヒップドロップ回避系
	bool bDoHipdrop;		//ヒップドロップ回避するかどうか
	bool bHipdropped;		//ヒップドロップ回避したか

} ComAIBrain;

#endif // !_AI_H_
