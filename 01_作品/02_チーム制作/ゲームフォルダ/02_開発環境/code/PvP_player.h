//==========================================
//
//プレイヤープログラムのヘッダ[player.h]
//Author:石原颯馬
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "model.h"
#include "item.h"
#include "input.h"
#include "ai.h"

//マクロ
#define PLAYER_POWER_MAX	(1.5f)		//移動の強さの最大値

//プレイヤー状態列挙
typedef enum
{
	PLAYERSTAT_WAIT = 0,
	PLAYERSTAT_CHARGE,
	PLAYERSTAT_DASH,
	PLAYERSTAT_PUSHED,
	PLAYERSTAT_JUMP,
	PLAYERSTAT_HIPDROP,
	PLAYERSTAT_LAND,
	PLAYERSTAT_FALL,
	PLAYERSTAT_MAX
} PLAYERSTAT;

//プレイヤーモデルの構造体
typedef struct
{
	int nIdxModel;			//インデックス番号
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	LPD3DXMESH pMesh;		//メッシュ
	LPD3DXBUFFER pBuffMat;	//マテリアルポインタ
	DWORD dwNumMat;			//マテリアルの数

}PlayModel;

//プレイヤー個人で使うモデル類構造体
typedef struct
{
	D3DXVECTOR3 pos;		//モーション設定した位置
	D3DXVECTOR3 rot;		//モーション設定した向き
	D3DXMATRIX mtxWorld;	//ワールドマトリ
} AnimalInstance;

//プレイヤー構造体
struct Player
{
	//位置類
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 moveV0;		//初期移動量
	D3DXVECTOR3 rot;		//向き
	float moveGauge;		//移動量(ダッシュ)
	float fOldMoveGauge;	//前の移動量（アクション硬直に使用）
	int jumpTime;			//ジャンプ開始からの時間[フレーム単位]
	bool bJump;				//ジャンプしているかどうか
	bool bHipDrop;			//ヒップドロップしているかどうか
	bool bHipDropSpin;		//ヒップドロップのスピンしたかどうか
	int nHipDropWait;		//ヒップドロップの「開始・着地」硬直カウンター
	bool bNotMove;			//移動していない
	int nRespawnPosNum;		//復活した位置番号

	//モーション類
	Motion motion;				//モーション情報
	D3DXVECTOR3 motionPos, motionPosOffset;	//位置
	D3DXVECTOR3 motionRot, motionRotOffset;	//向き
	bool bAnimed;

	//衝突関係
	D3DXVECTOR3 faceCollider[2];	//当たり判定
	int nFrameAtkPlayer;			//このフレームで触れたプレイヤー

	//パラメータ類
	ANIMAL animal;			//使用している動物
	int nScore;				//得点
	int nLastHitPlayer;		//最後に衝突したプレイヤー（初期値-1）
	PLAYERSTAT stat;		//状態
	int nActionRigor;		//アクション硬直時間
	int nPlayerNum;			//プレイヤー番号

	//アイテム類
	int nATKItemTime;		//はじき強化アイテムの持続時間
	int nDEFItemTime;		//押し合い強化アイテムの持続時間
	int nGhostItemTime;		//ゴースト化アイテムの持続時間
	int nInvincibleTime;	//無敵の持続時間

	//AIの操作（AIVerにて追加）
	ComAIBrain *pAI;		//AIポインタ

	//描画類
	AnimalInstance animalInst[MAX_PARTS];
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	bool bUseShadow;		//影使用の有無
	bool bUsePlayer;		//プレイヤー使用の有無

	PlayModel aPlaymodel[10];
};

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void SetPlayerType_PvP(int nPlayerNum, bool bUse, bool bAIUse);	//プレイヤー使用時AI使用するか指定しない場合AI使用

#endif // !_PLAYER_H_
