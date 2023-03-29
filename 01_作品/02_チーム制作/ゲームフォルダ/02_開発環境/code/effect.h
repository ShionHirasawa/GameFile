//==========================================
//
//エフェクトプログラム[effect.h]
//Author:飯田洲暉
//
//==========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//エフェクト種類列挙
typedef enum
{
	EFFECTTYPE_CHARGE = 0,	//チャージエフェクト
	EFFECTTYPE_ATTACK,		//アタックエフェクト
	EFFECTTYPE_MAX
} EFFECTTYPE;

//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int			nType;		//エフェクトのタイプ
	int			nCntLoop;	//ループした回数（今だけ）
	float		fSize;		//半径
	float		fAlpha;		//α値
	bool		bUse;		//使われているかどうか
	bool		bUseCharge;	//チャージが使われているかどうか
}Effect;

//プロトタイプ宣言
void InitEffect(void);								//初期化処理
void UninitEffect(void);							//終了処理
void UpdateEffect(void);							//更新処理
void UpdateEffectSize(int nEffect);					//エフェクトのサイズ更新

void DrawEffect(void);								//描画処理

void SetEffectPos(void);							//プレイヤーの位置に設定する

void SetEffect(D3DXVECTOR3 pos, int nCntEffect, EFFECTTYPE type);	//エフェクトを設定

#endif
