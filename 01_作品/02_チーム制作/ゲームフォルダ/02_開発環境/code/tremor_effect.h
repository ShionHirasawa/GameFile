//==========================================
//
//エフェクトプログラム[tremor_effect.h]
//Author:石原颯馬（ベース：飯田洲暉）
//
//==========================================
#ifndef _TREMOR_EFFECT_H_
#define _TREMOR_EFFECT_H_

#include "main.h"

//マクロ
#define TREMOR_EFFECT_TIME		(30)		//エフェクト発生時間（フレーム単位）

//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int			nType;		//エフェクトのタイプ
	int			nCounter;	//エフェクト発生開始から経過した時間
	float		fSize;		//半径
	float		fAlpha;		//α値
	bool		bUse;		//使われているかどうか
}TremorEffect;

//プロトタイプ宣言
void InitTremorEffect(void);							//初期化処理
void UninitTremorEffect(void);							//終了処理
void UpdateTremorEffect(void);							//更新処理
void UpdateTremorEffectSize(int nEffect);				//エフェクトのサイズ更新
void DrawTremorEffect(void);							//描画処理
void SetTremorEffect(D3DXVECTOR3 pos);	//エフェクトを設定

#endif
