//==========================================
//
//エフェクトプログラム[charge_effect.h]
//Author:飯田洲暉
//
//==========================================
#ifndef _CHARGEEFFECT_H_
#define _CHARGEEFFECT_H_

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
}ChargeEffect;

//プロトタイプ宣言
void InitChargeEffect(void);								//初期化処理
void UninitChargeEffect(void);							//終了処理
void UpdateChargeEffect(void);							//更新処理
void UpdateChargeEffectSize(int nEffect);					//エフェクトのサイズ更新

void DrawChargeEffect(void);								//描画処理

void SetChargeEffectPos(void);							//プレイヤーの位置に設定する

void SetChargeEffect(D3DXVECTOR3 pos, int nCntEffect);	//エフェクトを設定

#endif
