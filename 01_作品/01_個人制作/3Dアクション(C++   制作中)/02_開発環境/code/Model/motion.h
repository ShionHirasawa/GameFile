//================================================================================================
//
//モーション管理処理[motion.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "model.h"

//キーの最大数
#define MAX_KEY	(10)

//モーションクラス
class CMotion
{
public:
	//モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	// 待機
		MOTIONTYPE_SNEAK,		// スニーク
		MOTIONTYPE_WALK,		// 歩き
		MOTIONTYPE_ACTION,		// アクション
		MOTIONTYPE_JUMP,		// シャンプ
		MOTIONTYPE_LANDING,		// 着地
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	//キーの構造体
	typedef struct
	{
		float fPosX;	// 位置 X
		float fPosY;	// 位置 Y
		float fPosZ;	// 位置 Z
		float fRotX;	// 向き X
		float fRotY;	// 向き Y
		float fRotZ;	// 向き Z
	} KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame;				// モーションの再生時間（フレーム
		KEY aKey[MAX_PARTS];	// 各モデルのキー要素
	}KEY_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;					// ループするかどうか
		int nNumKey;				// キーの総数
		KEY_INFO aKeyInfo[MAX_KEY];	// キー情報
	}MOTION_INFO;

	CMotion();
	~CMotion();

	/*	モーションの種類を設定	*/	void Set(int nType);
	/*	モーションの位置設定	*/	void SetPos(void);
	/*	モーションの更新処理	*/	void Update(void);
	/*	モーションの種類を取得	*/	int GetType(void) { return m_nType; }
	/*	モーションの終了判定	*/	bool IsFinish(void) { return m_bFinish; }

	/*	モーション情報を設定	*/	void SetInfo(MOTION_INFO info);
	/*モーションをするモデル設定*/	void SetModel(CModel **ppModel, int nNumModel);
	/*パーツの初期位置・角度設定*/	void SetOffset(D3DXVECTOR3 offSet, D3DXVECTOR3 offRot, int nCntSet);
	/*モーションのブレンド*/		void SetBlend(int nType);
	/*アクションが可能かどうか*/	bool IsAction(void) { return m_bAction; }
	/*	  パーツ情報リセット	*/	void ResetParts(KEY *pKEY);
	/*	モーション情報初期化	*/	void InitMotionInfo(MOTION_INFO *pInfo);

private:
	void StepKey(void);		//次のキーへ進む
	void SetMoveParts(void);//各パーツの移動量を設定する


	MOTION_INFO m_aInfo[MOTIONTYPE_MAX];
	int m_nNumAll;	// よく分からん
	int m_nType;	// 現在のモーションの種類を格納
	bool m_bLoop;	// ループするかどうか（MOTION_INFO構造体にもある "bLoop"変数と何が違う？)
	int m_nNumKey;	// 各モーションのキーの分割総数を格納
	int m_nKey;		// 現在の分割キー番号
	int m_nCounter;	// 再生経過フレーム
	bool m_bFinish;	// 終了したかどうか

	int		m_nBlendType;	//ブレンドしたいモーションの種類	
	bool	m_bBlend;		//ブレンド中かどうか

	bool m_bAction;	//各モーション特有の行動をしても良いか

	CModel **m_ppModel;	// モデルへのポインタ
	int m_nNumModel;	// モデルの総数

	D3DXVECTOR3 m_OffSetPos[MAX_PARTS];	//初期位置
	D3DXVECTOR3 m_OffSetRot[MAX_PARTS];	//初期角度

	D3DXVECTOR3 m_Pos[MAX_PARTS], m_TargetPos[MAX_PARTS], m_MovePos[MAX_PARTS];
	D3DXVECTOR3 m_Rot[MAX_PARTS], m_TargetRot[MAX_PARTS], m_MoveRot[MAX_PARTS];

	//デバッグ用タイプ表示
	char *m_pTypeSentence[CMotion::MOTIONTYPE_MAX] = {
		"待機",
		"スニーク",
		"歩き",
		"アクション",
		"シャンプ",
		"着地",
	};
};

#endif