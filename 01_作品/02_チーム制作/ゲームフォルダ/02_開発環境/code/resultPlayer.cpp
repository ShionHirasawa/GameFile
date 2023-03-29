//==========================================
//
//リザルトプレイヤープログラムの処理[resultPlayer.cpp]
//Author:平澤詩苑　石原颯馬
//
//==========================================
#include "resultPlayer.h"
#include "select_game.h"
#include "VictoryStand.h"
#include "debugproc.h"
#include "color.h"
#include "sound.h"

//マクロ定義
#define JUMP_RES_PLAYER			(6.0f)		//リザルトプレイヤーのジャンプ量
#define ADVANCE_RES_PLAYER		(-2.4f)		//リザルトプレイヤーの前進速度
#define FALL_RES_PLAYER			(-4.0f)		//リザルトプレイヤーの落下速度
#define GRAVITY_MAG				(0.03f)		//重力計算に用いる倍率
#define PLAYER_HIPDROPWAIT		(10)		//ヒップドロップ開始の硬直フレーム
#define PLAYER_HIPDROP_SPEED	(-8.0f)		//ヒップドロップ中の落下速度
#define PLAYER_HIPSPIN_SPEED	(-0.5f)		//ヒップドロップスピンの回転値
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//ヒップドロップスピンしたときの１周判定をとる値

//グローバル変数
Player_RESULT g_ResultPlayer[MAX_USE_GAMEPAD];			/*リザルト用プレイヤーの情報*/
const D3DXVECTOR3 c_ResultPlayerPos[MAX_USE_GAMEPAD] = {	/*プレイヤーの初期位置設定*/
	D3DXVECTOR3(-75.0f, 700.0f, 80.0f),
	D3DXVECTOR3(-25.0f, 700.0f, 80.0f),
	D3DXVECTOR3(+25.0f, 700.0f, 80.0f),
	D3DXVECTOR3(+75.0f, 700.0f, 80.0f),
};

//========================
//初期化処理
//========================
void InitPlayer_RESULT(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//リザルト用プレイヤーのポインタを取得
	Player_RESULT *pResPlayer = &g_ResultPlayer[0];

	//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++, pResPlayer++)
	{
		//変数初期化
		*pResPlayer = ResetResultPlayerInfo(nCntPlayer);
	}

	//ポインタを再取得
	pResPlayer = &g_ResultPlayer[0];

	//ゲームモードに応じて取得するものを変更
	switch (GetSelGameMode())
	{
	case SelGameMode_PVP:	//ゲームモード「PVP」の場合の初期化処理
		InitResultPlayerType_GAMEMODE_PVP(pResPlayer);
		break;

	case SelGameMode_HDR:	//ゲームモード「HDR」の場合の初期化処理
		InitResultPlayerType_GAMEMODE_HDR(pResPlayer);
		break;
	}
}

//========================
//ゲームモードが PVP のときの初期化処理
//========================
void InitResultPlayerType_GAMEMODE_PVP(Player_RESULT *pResPlayer)
{
	//PVP プレイヤーのポインタ取得
	Player *pPVPPlayer = GetPlayer();

	for (int nCntPVP = 0; nCntPVP < MAX_USE_GAMEPAD; nCntPVP++, pPVPPlayer++, pResPlayer++)
	{
		//プレイヤーのスコアを取得
		pResPlayer->nRank = pPVPPlayer->nScore;
	}
}

//========================
//ゲームモードが HDR のときの初期化処理
//========================
void InitResultPlayerType_GAMEMODE_HDR(Player_RESULT *pResPlayer)
{
	//HDR のプレイヤー情報取得
	Player_HDR *pHDRPlayer = GetPlayer_HDR();

	for (int nCntHDR = 0; nCntHDR < MAX_USE_GAMEPAD; nCntHDR++, pResPlayer++, pHDRPlayer++)
	{
		//プレイヤーの順位を取得
		pResPlayer->nRank = pHDRPlayer->nRank;
	}
}

//========================
//終了処理
//========================
void UninitPlayer_RESULT(void)
{

}

//========================
//更新処理
//========================
void UpdatePlayer_RESULT(void)
{
	//対象のプレイヤーのポインタを取得
	Player_RESULT *pPlayer = &g_ResultPlayer[0];

	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++, pPlayer++)
	{
		//前回の位置を記憶
		pPlayer->posOld = pPlayer->pos;

		//プレイヤーの重力処理
		UpdatePosPlayer_RESULT(pPlayer);

		//停止処理
		StopPlayer_RESULT(pPlayer, nCntPlayer);

		//ヒップドロップスピン処理
		HipSpinResPlayer(pPlayer);

		//表彰台への当たり判定
		CollisionVictoryStand(&pPlayer->pos, &pPlayer->posOld);
	}
}

//重力処理
void UpdatePosPlayer_RESULT(Player_RESULT *pPlayer)
{
	//プレイヤーがヒップドロップスピンをしていなければ
	if (!pPlayer->bHipDropSpin)
	{
		//プレイヤーが飛び込んでいたら
		if (pPlayer->bDive)
		{
			//落下速度上昇
			pPlayer->fFallSpeed += (FALL_RES_PLAYER - pPlayer->fFallSpeed) * GRAVITY_MAG;
		}

		//ヒップドロップ硬直がある
		if (0 < pPlayer->nHipDropWait)
		{
			//硬直フレーム減算
			pPlayer->nHipDropWait--;
		}

		//ヒップドロップ硬直がない
		else
		{
			//Ｙ座標更新								//Ｚ座標更新
			pPlayer->pos.y += pPlayer->fFallSpeed;		pPlayer->pos.z += pPlayer->fAdvance;
		}
	}
}

//停止処理
void StopPlayer_RESULT(Player_RESULT *pPlayer, int nCntPlayer)
{
	//プレイヤーがＺの原点位置を超えて手前に来た and ヒップドロップしていない
	if (pPlayer->pos.z <= 0.0f && !pPlayer->bHipDrop)
	{
		pPlayer->pos.z =				//原点位置に戻す
		pPlayer->fAdvance = 0.0f;		//前進速度を０にする
		pPlayer->bHipDrop =				//ヒップドロップと
		pPlayer->bHipDropSpin = true;	//ヒップドロップスピンを開始する
		pPlayer->nHipDropWait = PLAYER_HIPDROPWAIT; //ヒップドロップの開始硬直を設定
		pPlayer->fFallSpeed = PLAYER_HIPDROP_SPEED;	//ヒップドロップの初速を代入

		PlaySoundSE(SOUND_LABEL_SE_HIPSPIN, nCntPlayer);//ヒップドロップ音再生
	}
}

//ヒップドロップスピン処理
void HipSpinResPlayer(Player_RESULT *pPlayer)
{
	//もしもプレイヤーがヒップドロップスピン中だったら
	if (pPlayer->bHipDropSpin)
	{
		//回転値を加算
		pPlayer->rot.x += PLAYER_HIPSPIN_SPEED;

		//１周した
		if (pPlayer->rot.x <= PLAYER_HIPSPIN_LAP)
		{
			pPlayer->rot.x = 0.0f;			//回転値を元に戻す
			pPlayer->bHipDropSpin = false;	//スピンし終わった
		}
	}
}

//========================
//描画処理
//========================
void DrawPlayer_RESULT(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

									//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//プレイヤーの数だけ繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//プレイヤーのモデルを取得
		Model useAnimal = GetAnimal(g_ResultPlayer[nCntPlayer].animal);

		//"プレイヤーの"ワールドマトリックス初期化
		D3DXMatrixIdentity(&g_ResultPlayer[nCntPlayer].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ResultPlayer[nCntPlayer].rot.y, g_ResultPlayer[nCntPlayer].rot.x, g_ResultPlayer[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].mtxWorld, &g_ResultPlayer[nCntPlayer].mtxWorld, &mtxRot);

		//位置反映
		D3DXMatrixTranslation(&mtxTrans, g_ResultPlayer[nCntPlayer].pos.x, g_ResultPlayer[nCntPlayer].pos.y, g_ResultPlayer[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].mtxWorld, &g_ResultPlayer[nCntPlayer].mtxWorld, &mtxTrans);

		//"プレイヤーの"ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ResultPlayer[nCntPlayer].mtxWorld);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
			D3DXMATRIX mtxParent;					//親のマトリ

			//ここ新仕様
			//仮でオフセットをそのまま使う（アニメーション使うようになったら消して）
			g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos = useAnimal.aParts[nCntParts].posOffset;
			g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot = useAnimal.aParts[nCntParts].rotOffset;

			//"モデルの"ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.y, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.x, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].rot.z);
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxRotModel);

			//位置反映
			D3DXMatrixTranslation(&mtxTransModel, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.x, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.y, g_ResultPlayer[nCntPlayer].animalInst[nCntParts].pos.z);
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxTransModel);

			//パーツの親マトリ設定
			if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
			{
				mtxParent = g_ResultPlayer[nCntPlayer].animalInst[useAnimal.aParts[nCntParts].mIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_ResultPlayer[nCntPlayer].mtxWorld;
			}

			//パーツのマトリと親マトリをかけ合わせる
			D3DXMatrixMultiply(&g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxParent);

			//"モデルの"ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_ResultPlayer[nCntPlayer].animalInst[nCntParts].mtxWorld);

			//マテリアルデータへのポインタ取得
			pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
			{
				//ゴースト用
				D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

				if (nCntParts == MAX_PARTS - 1)
				{//ゼッケンの時は色変更
					matChange.Diffuse = c_aColPlayer[nCntPlayer];
				}

				//マテリアルの設定
				pDevice->SetMaterial(&matChange);

				//テクスチャ設定
				pDevice->SetTexture(0, useAnimal.aParts[nCntParts].apTexture[nCntMat]);

				//モデル描画
				useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
			}
		}
		
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//プレイヤーの飛び込み開始
void SetDivePlayer(void)
{
	//プレイヤーのポインタ取得
	Player_RESULT *pPlayer = &g_ResultPlayer[0];

	for (int nCntDive = 0; nCntDive < MAX_USE_GAMEPAD; nCntDive++, pPlayer++)
	{
		//プレイヤーがダイブしていない
		if (!pPlayer->bDive)
		{
			pPlayer->fFallSpeed = JUMP_RES_PLAYER;	// 初期落下速度（飛び込みなので、代入値はジャンプ量と同じ）を代入
			pPlayer->fAdvance = ADVANCE_RES_PLAYER;	// 前進速度代入
			pPlayer->bDive = true;					// 飛び込む
			pPlayer->bHipDropSpin = false;			// ヒップドロップスピンをしていない

			break;	// 処理を終える
		}
	}
}

//リザルト用プレイヤーの情報を取得
Player_RESULT *GetPlayer_RESULT(void)
{
	return &g_ResultPlayer[0];
}

//リザルト用プレイヤーの構造体初期化
Player_RESULT ResetResultPlayerInfo(int nCntResetPlayer)
{
	return {
		c_ResultPlayerPos[nCntResetPlayer],	// 位置初期化
		c_ResultPlayerPos[nCntResetPlayer],	// 前回位置初期化
		ZERO_SET,	// 向き初期化
		0.0f,		// 落下速度初期化
		0.0f,		// 前進速度初期化
		false,		// 飛び込み初期化
		false,		// ヒップドロップ初期化
		false,		// ヒップドロップスピン初期化
		0,			// ヒップドロップ開始硬直初期化
		-1,			// 順位初期化

		ANIMAL_WILDBOAR,	// 使用している動物情報初期化

		{},		// モデル情報初期化
		{},		// 体パーツの情報初期化
		{},		// ワールドマトリックス初期化
		{},		// プレイヤーモデルの構造体初期化

		false,	// プレイヤー使用・不使用初期化
	};
}