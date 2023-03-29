//==========================================
//
//プレイヤープログラムのヘッダ[hdr_player.h]
//Author:平澤詩苑　石原颯馬
//
//==========================================
#ifndef _RESULTPLAYER_H_
#define _RESULTPLAYER_H_

#include "main.h"
#include "model.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "input.h"

//プレイヤーモデルの構造体
typedef struct
{
	int nIdxModel;			//インデックス番号
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	LPD3DXMESH pMesh;		//メッシュ
	LPD3DXBUFFER pBuffMat;	//マテリアルポインタ
	DWORD dwNumMat;			//マテリアルの数

}PlayModel_RESULT;

//プレイヤー構造体
struct Player_RESULT
{
	//位置類
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 posOld;	// 前回の位置
	D3DXVECTOR3 rot;	// 向き
	float	fFallSpeed;	// 落下速度
	float	fAdvance;	// 前進速度
	bool bDive;			// 飛び込み
	bool bHipDrop;		// ヒップドロップしているかどうか
	bool bHipDropSpin;	// ヒップドロップのスピンしたかどうか
	int nHipDropWait;	// ヒップドロップの開始硬直カウンター
	int nRank;			// 順位を格納

	//パラメータ類		 
	ANIMAL animal;		// 使用している動物

						// 描画類
	Model model;		// 使用モデル
	AnimalInstance animalInst[MAX_PARTS];
	D3DXMATRIX mtxWorld;// ワールドマトリ
	PlayModel_RESULT aPlaymodel[10];

	bool bUsePlayer;	// プレイヤー使用の有無
};

//プロトタイプ宣言
void InitPlayer_RESULT(void);											//基本情報初期化
void InitResultPlayerType_GAMEMODE_PVP(Player_RESULT *pResPlayer);		//ゲームモードが "PvP" のときの初期化処理
void InitResultPlayerType_GAMEMODE_HDR(Player_RESULT *pResPlayer);		//ゲームモードが "HDR" のときの初期化処理
void UninitPlayer_RESULT(void);			//終了処理

void UpdatePlayer_RESULT(void);									//更新処理
void UpdatePosPlayer_RESULT(Player_RESULT *pPlayer);			//重力処理
void StopPlayer_RESULT(Player_RESULT *pPlayer, int nCntPlayer);	//停止処理
void HipSpinResPlayer(Player_RESULT *pPlayer);					//ヒップドロップスピン処理

void DrawPlayer_RESULT(void);		//描画処理

void SetDivePlayer(void);			//飛び込み開始処理

Player_RESULT *GetPlayer_RESULT(void);						//プレイヤーの情報取得処理
Player_RESULT ResetResultPlayerInfo(int nCntResetPlayer);	//プレイヤーの初期情報取得処理

#endif