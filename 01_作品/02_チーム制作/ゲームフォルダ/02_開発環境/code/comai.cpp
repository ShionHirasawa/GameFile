//==========================================
//
//コンピューターのAIプログラム[comai.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "comai.h"
#include "PvP_player.h"
#include "input.h"
#include "meshfield.h"
#include "conversioninput.h"
#include <time.h>

//マクロ
#define CHARGE_LENGTH_AVG	(600.0f)	//チャージの移動距離の基準値
#define CHARGE_DIGIT		(100)		//チャージの小数点の位置指定
#define HIPDROP_RADIUS		(100.0f)	//落下回避ヒップドロップまでの距離
#define HIPDROP_RAND_DIGIT	(100)		//ヒップドロップ回避確率桁数
#define CHARGE_WAIT			(75)		//チャージする前の待機時間
#define ONE_MORE_PUSH		(0.31f)		//プレイヤーを落とすためにするもう一押し

//プロト
void RotateAI(Player *pCom);
void ChargeAI(Player *pCom);

//グローバル
ComAIBrain g_aAIBrain[MAX_USE_GAMEPAD];
bool g_bUseAIPointer[MAX_USE_GAMEPAD];

const AIParam c_aAIParam[AIDIFF_MAX] = 
{
	{ 0.50f, 0.25f, 30, 20 },	//EASY
	{ 0.25f, 0.17f, 60, 15 },	//NORMAL
	{ 0.15f, 0.10f, 90, 10 },	//HARD
	{ 0.00f, 0.50f, 0, 0 }		//VIEW
};

//========================
//初期化処理
//========================
void InitComAI(void)
{
	srand((unsigned int)time(NULL));	//乱数の初期化

	for (int nCntAI = 0; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{//脳みそを空っぽにする
		g_aAIBrain[nCntAI] = {};
		g_aAIBrain[nCntAI].nCounterWaitTime = 0;
		g_aAIBrain[nCntAI].difficulty = AIDIFF_NORMAL;
		g_aAIBrain[nCntAI].bDoHipdrop = false;
		g_aAIBrain[nCntAI].bHipdropped = false;
		g_bUseAIPointer[nCntAI] = false;
	}
}

//========================
//終了処理
//========================
void UninitComAI(void)
{
	for (int nCntAI = 0; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{//AIを使っていないことにする
		g_bUseAIPointer[nCntAI] = false;
	}
}

//========================
//選択処理
//========================
void SelectAIMove(Player *pCom)
{
	if (pCom->stat != PLAYERSTAT_FALL)
	{//落ちていない
		RotateAI(pCom);

		//あらかじめAIの中心からの距離求める
		float fAILength = fabsf(D3DXVec3Length(&pCom->pos));

		//スティック操作をしていないならチャージ
		if (pCom->pAI->nCounterWaitTime >= CHARGE_WAIT)
		{
			float fMoveLength = sqrtf(powf(fAILength, 2) + powf(GetMeshField()->fRadius, 2) - 2 * fAILength * GetMeshField()->fRadius * cosf(pCom->rot.y)) / CHARGE_LENGTH_AVG;	//ジャストなチャージ量を求める

			//↓でアバウトにしていく
			int nChargeWidth = (int)((((fMoveLength + c_aAIParam[pCom->pAI->difficulty].fChargeAboutPlus) - (fMoveLength - c_aAIParam[pCom->pAI->difficulty].fChargeAboutMinus)) * CHARGE_DIGIT) + ONE_MORE_PUSH);
			pCom->pAI->fChargePower = (float)(rand() % (nChargeWidth + 1)) / CHARGE_DIGIT + (fMoveLength - c_aAIParam[pCom->pAI->difficulty].fChargeAboutMinus);

			//最低値以下なら最低値にする
			if (pCom->pAI->fChargePower < 1.0f / CHARGE_DIGIT)
			{
				pCom->pAI->fChargePower = 1.0f / CHARGE_DIGIT;
			}
			else if (pCom->pAI->fChargePower > PLAYER_POWER_MAX)
			{
				pCom->pAI->fChargePower = PLAYER_POWER_MAX;
			}

			ChargeAI(pCom);

		}
		else if (GetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X) == true)
		{
			ChargeAI(pCom);
		}

		//落ちそうならヒップドロップ回避
		if (GetMeshField()->fRadius - fAILength <= HIPDROP_RADIUS)
		{
			if (pCom->pAI->bHipdropped == false)
			{
				pCom->pAI->bDoHipdrop = rand() % (HIPDROP_RAND_DIGIT + 1) <= c_aAIParam[pCom->pAI->difficulty].nHipdropRandom ? true : false;
				if (pCom->pAI->bDoHipdrop == true || pCom->stat == PLAYERSTAT_JUMP)
				{
					SetButton(pCom->nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A, true);
					if (pCom->stat == PLAYERSTAT_JUMP)
					{
						if (pCom->jumpTime > c_aAIParam[pCom->pAI->difficulty].nHipdropTime)
						{
							pCom->pAI->bHipdropped = true;
						}
						else
						{
							SetButton(pCom->nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A, false);
						}
					}
				}
				else
				{
					pCom->pAI->bHipdropped = true;
				}
			}
			else
			{
				SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_A, false);
			}
		}
		else
		{
			pCom->pAI->bHipdropped = false;
			SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_A, false);
		}
	}
	else
	{//落ちたのでAIがコントローラーから手を放してOMGしている
		SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_A, false);
		SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X, false);
		SetButton(pCom->nPlayerNum, INPUTTYPE_RELEASE, BUTTON_X, false);
		SetStick(pCom->nPlayerNum, CONVSTICK_NEUTRAL);
	}
}

//========================
//回転処理
//========================
void RotateAI(Player *pCom)
{
	Player *pPlayer = GetPlayer();
	Player *pNearPlayer = NULL;
	float fMinLength = -1.0f;

	//一番近いプレイヤーの方を向くようにする
	//近いプレイヤーを探す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if ((pPlayer + nCntPlayer) != pCom)
		{//自分でなければ
		 //距離計算
			float fLength = PYTHAGORAS(((pPlayer + nCntPlayer)->pos.x - pCom->pos.x), ((pPlayer + nCntPlayer)->pos.z - pCom->pos.z));

			if (pNearPlayer == NULL || fMinLength >= fLength)
			{//何も入っていない または もっと近いプレイヤー見つけた
			 //距離と近いプレイヤーのポインタ代入
				fMinLength = fLength;
				pNearPlayer = (pPlayer + nCntPlayer);
			}
		}
	}

	//角度を変える（2DSTGのホーミング技術使用）
	float fRotMove = pCom->rot.y;
	float fRotDest = atan2f(pNearPlayer->pos.x - pCom->pos.x, pNearPlayer->pos.z - pCom->pos.z);
	float fRotDiff = FIX_ROT(fRotDest - fRotMove + D3DX_PI);

	if ((int)(fabsf(fRotDiff) * 10) == 0)//10にしないと厳しすぎてプルプルする
	{
		SetStick(pCom->nPlayerNum, CONVSTICK_NEUTRAL);
		pCom->pAI->nCounterWaitTime++;
	}
	else
	{
		//判断
		if ((int)copysign(1, fRotDiff) < 0)
		{
			SetStick(pCom->nPlayerNum, CONVSTICK_LEFT);
		}
		else
		{
			SetStick(pCom->nPlayerNum, CONVSTICK_RIGHT);
		}
	}
}

//========================
//チャージボタンを押す処理
//========================
void ChargeAI(Player *pCom)
{
	if (pCom->moveGauge >= pCom->pAI->fChargePower)
	{
		SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X, false);
		SetButton(pCom->nPlayerNum, INPUTTYPE_RELEASE, BUTTON_X, true);
		pCom->pAI->nCounterWaitTime = 0;
	}
	else
	{
		SetButton(pCom->nPlayerNum, INPUTTYPE_PRESS, BUTTON_X, true);
	}
}

//========================
//選択処理
//========================
ComAIBrain *GetAI(AIDIFF diff)
{
	for (int nCntAI = 0; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{
		if (g_bUseAIPointer[nCntAI] == false)
		{
			g_bUseAIPointer[nCntAI] = true;
			g_aAIBrain[nCntAI].difficulty = diff;
			return &g_aAIBrain[nCntAI];
		}
	}
	return NULL;
}