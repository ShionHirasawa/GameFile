//==========================================
//
//プレイヤープログラムのヘッダ[hdr_player.h]
//Author:藤原龍輝
//
//==========================================
#ifndef _HDR_PLAYER_H_
#define _HDR_PLAYER_H_

#include "main.h"
#include "model.h"
#include "input.h"
#include "ai.h"

//プレイヤーモデルの構造体
typedef struct
{
	int nIdxModel;			//インデックス番号
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	LPD3DXMESH pMesh;		//メッシュ
	LPD3DXBUFFER pBuffMat;	//マテリアルポインタ
	DWORD dwNumMat;			//マテリアルの数

}PlayModel_HDR;

//プレイヤー構造体
struct Player_HDR
{
	//位置類
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	float		fHeightLastJump;//最後にジャンプした時の高さ
	D3DXVECTOR3 posOld_Coll;//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 moveV0;		//初期移動量
	D3DXVECTOR3 rot;		//向き
	int jumpTime;			//ジャンプ開始からの時間[フレーム単位]
	int nHipDropWait;		//ヒップドロップの「開始・着地」硬直カウンター
	int HipDropPower;
	bool bJump;				//ジャンプしているかどうか
	bool bHipDrop;			//ヒップドロップしているかどうか
	bool bHipDropSpin;		//ヒップドロップのスピンしたかどうか
	bool bGoal;				//ゴールしたか
	int nRank;				//順位
	float fBodySize;		//体の大きさ（当たり判定に使う
	int nDestroyCounter;	//ブロック破壊個数

	//パラメータ類
	ANIMAL animal;			//使用している動物

	//AI
	AIDIFF aiDiff;			//AI難易度
	bool bUseAI;			//AI使用するかどうか
	float fAIHeight;			//ジャンプ量
	int nAICT;				//AIのクールタイム

	//描画類
	Model model;			//使用モデル
	D3DXMATRIX mtxWorld;	//ワールドマトリ
	bool bUseShadow;		//影使用の有無
	bool bUsePlayer;		//プレイヤー使用の有無

	PlayModel_HDR aPlaymodel[10];
};

//プロトタイプ宣言
void InitPlayer_HDR(void);
void UninitPlayer_HDR(void);
void UpdatePlayer_HDR(void);
void DrawPlayer_HDR(void);
Player_HDR *GetPlayer_HDR(void);
void SetPlayerType_HDR(int nPlayerNum, bool bUse, bool bAIUse);	//プレイヤー使用時AI使用するか指定しない場合AI使用

//ヒップドロップ割合
const AIParam_HDR c_aAIParamHDR[AIDIFF_MAX] =
{
	{ 10,25,45,30,40 },	//EASY
	{ 5,15,40,40,30 },	//NORMAL
	{ 1,4,20,75,20 },	//HARD
	{ 1,1,1,1,20 }	//VIEW
};

#endif