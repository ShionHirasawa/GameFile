//==========================================
//
//エフェクトプログラム[attack_effect.h]
//Author:飯田洲暉
//
//==========================================
#ifndef _ATTACKEFFECT_H_
#define _ATTACKEFFECT_H_

//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int			nType;		//エフェクトのタイプ
	int			nCntLoop;	//ループした回数（今だけ）
	float		fSize;		//半径
	float		fAlpha;		//α値
	float		fResize;	//サイズ変更用
	bool		bUse;		//使われているかどうか
}AttackEffect;

//プロトタイプ宣言
void InitAttackEffect(void);								//初期化処理
void UninitAttackEffect(void);							//終了処理
void UpdateAttackEffect(void);							//更新処理
void UpdateAttackEffectSize(int nEffect);					//エフェクトのサイズ更新

void DrawAttackEffect(void);								//描画処理

void SetAttackEffectPos(void);							//プレイヤーの位置に設定する

void SetAttackEffect(D3DXVECTOR3 pos, int nCntEffect);	//エフェクトを設定

#endif

