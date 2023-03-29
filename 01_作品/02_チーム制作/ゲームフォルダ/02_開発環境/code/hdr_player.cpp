//==========================================
//
//プレイヤープログラム[hdr_player.cpp]
//Author:藤原龍輝  平澤詩苑
//
//==========================================
#include "main.h"
#include "HDRgame.h"
#include "HDR_player.h"
#include "model.h"
#include "conversioninput.h"
#include "debugproc.h"
#include "camera.h"
#include "HDR_camera.h"
#include "collision.h"
#include <assert.h>
#include "color.h"
#include "sound.h"
#include "meshfield.h"
#include "HipDropRankUI.h"
#include "block.h"
#include "rank.h"
#include "ai.h"
#include "TypeFrame.h"

//マクロ定義
#define PLAYER_HIPSPIN_SPEED	(-0.5f)				//ヒップドロップスピンの回転値
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//ヒップドロップスピンしたときの１周判定をとる値
#define PLAYER_HIPSPIN_WAIT		(20)				//ヒップドロップスピンが終わって急降下するまでの時間
#define PLAYER_MOVE_SPEED		(5.0f)				//普通に移動するときの移動量
#define PLAYER_NORMAL_JUMP		(7.0f)				//プレイヤーの通常ジャンプ量
#define PLAYER_GOAL_JUMP		(4.3f)				//ゴール後のジャンプ量
#define PLAYER_BODY_SIZE		(30.0f)				//プレイヤーの胴体サイズ

//向き
#define ROT_WA	(+0.75f * D3DX_PI)	//左上
#define ROT_WD	(-0.75f * D3DX_PI)	//右上
#define ROT_SA	(+0.25f * D3DX_PI)	//左下
#define ROT_SD	(-0.25f * D3DX_PI)	//右下
#define ROT_W	(-1.0f * D3DX_PI)	//上
#define ROT_A	(+0.5f * D3DX_PI)	//左
#define ROT_S	(+0.0f * D3DX_PI)	//下
#define ROT_D	(-0.5f * D3DX_PI)	//右

//グローバル変数
Player_HDR g_aPlayerHDR[MAX_USE_GAMEPAD];
int g_nIdxShadow_HDR = -1;

//プロトタイプ宣言
void ControllPlayer_HDR(int nPlayerNum);
void MovePlayer(int nPadNum);

void JumpPlayer_HDR(int nJumpPlayer);			//ジャンプの設定処理
void HipDropPlayer_HDR(int nHipDropPlayer);		//ヒップドロップの設定処理
void HipSpinPlayer_HDR(int nHipSpinPlayer);		//ヒップスピンの処理

//初期位置向き
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-225.0f,MAX_BLOCK * COLLISION_SIZE_Y ,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(-75.0f,MAX_BLOCK * COLLISION_SIZE_Y ,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(75.0f,MAX_BLOCK * COLLISION_SIZE_Y ,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
	{ D3DXVECTOR3(225.0f,MAX_BLOCK * COLLISION_SIZE_Y ,0.0f) ,D3DXVECTOR3(0.0f,1.57f,0.0f) },
};

//========================
//初期化処理
//========================
void InitPlayer_HDR(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//変数初期化
		g_aPlayerHDR[nCntPlayer].pos = g_aPlayerHDR[nCntPlayer].posOld = c_aPosRot[nCntPlayer][0];
		g_aPlayerHDR[nCntPlayer].move = g_aPlayerHDR[nCntPlayer].moveV0 = g_aPlayerHDR[nCntPlayer].rot = ZERO_SET;
		g_aPlayerHDR[nCntPlayer].jumpTime = g_aPlayerHDR[nCntPlayer].nHipDropWait = 0;
		g_aPlayerHDR[nCntPlayer].bJump = false;
		g_aPlayerHDR[nCntPlayer].bHipDrop = false;
		g_aPlayerHDR[nCntPlayer].bHipDropSpin = false;
		g_aPlayerHDR[nCntPlayer].nRank = RANKING_4TH;
		g_aPlayerHDR[nCntPlayer].bGoal = false;
		g_aPlayerHDR[nCntPlayer].fBodySize = PLAYER_BODY_SIZE;
		g_aPlayerHDR[nCntPlayer].HipDropPower = 0;
		g_aPlayerHDR[nCntPlayer].nDestroyCounter = 0;

		g_aPlayerHDR[nCntPlayer].animal = ANIMAL_WILDBOAR;

		g_aPlayerHDR[nCntPlayer].fAIHeight = 0;
		g_aPlayerHDR[nCntPlayer].aiDiff = GetAIDiff();;
		g_aPlayerHDR[nCntPlayer].nAICT = 20;
		
		g_aPlayerHDR[nCntPlayer].model = GetAnimal(g_aPlayerHDR[nCntPlayer].animal);
	}
}

//========================
//終了処理
//========================
void UninitPlayer_HDR(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//プレイヤーの数だけ処理する
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{//取得した方なのでNULL入れるだけでOK
			g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh = NULL;
			g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat = NULL;
		}
	}
}

//========================
//更新処理
//========================
void UpdatePlayer_HDR(void)
{
	if (*GetHDR_Ready() == HDR_Ready_OK)
	{
		//プレイヤー人数分繰り返す
		for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
		{
			if (g_aPlayerHDR[nCntPlayer].bUsePlayer == true)
			{
				//前回の位置を保存
				g_aPlayerHDR[nCntPlayer].posOld.x = g_aPlayerHDR[nCntPlayer].pos.x;
				g_aPlayerHDR[nCntPlayer].posOld.z = g_aPlayerHDR[nCntPlayer].pos.z;

				//ジャンプ時間を増やす
				g_aPlayerHDR[nCntPlayer].jumpTime++;

				//AIのクールタイムを減らす
				g_aPlayerHDR[nCntPlayer].nAICT--;

				if (g_aPlayerHDR[nCntPlayer].bUsePlayer == true)
				{//使用時のみ行う
				 //プレイヤーであるか確認
				 //接続されているか確認して切断されていたらプレイヤーを消す（例外としてコントローラーがつながっていないときは無視）
					if (g_aPlayerHDR[nCntPlayer].bUseAI == false && GetUseControllerNum_HDR() != 0)
					{
						g_aPlayerHDR[nCntPlayer].bUsePlayer = GetUseController_HDR(nCntPlayer);
					}

					//各プレイヤーの操作
					ControllPlayer_HDR(nCntPlayer);
				}

				//ヒップドロップスピンの硬直中
				if (0 < g_aPlayerHDR[nCntPlayer].nHipDropWait)
				{
					HipSpinPlayer_HDR(nCntPlayer);
				}
				//硬直中で無ければ落下速度を代入
				else
				{
					g_aPlayerHDR[nCntPlayer].move.y = g_aPlayerHDR[nCntPlayer].moveV0.y - (9.8f * g_aPlayerHDR[nCntPlayer].jumpTime / MAX_FPS);
				}

				//移動
				g_aPlayerHDR[nCntPlayer].pos += g_aPlayerHDR[nCntPlayer].move;

				//プレイヤーが地面を突き抜けてしまった
				if (g_aPlayerHDR[nCntPlayer].pos.y < 0.0f)
				{
					g_aPlayerHDR[nCntPlayer].pos.y = 0.0f;
					g_aPlayerHDR[nCntPlayer].move.y = 0.0f;
					g_aPlayerHDR[nCntPlayer].bJump = false;
					g_aPlayerHDR[nCntPlayer].bHipDrop = false;

					//まだゴールしていない
					if (!g_aPlayerHDR[nCntPlayer].bGoal)
					{
						g_aPlayerHDR[nCntPlayer].bGoal = true;	//ゴールした
						g_aPlayerHDR[nCntPlayer].nRank = SetRank(nCntPlayer);//順位設定
					}
				}

				//ブロックの当たり判定
				CollisionBlock(&g_aPlayerHDR[nCntPlayer]);

				//フェンスとの当たり判定
				CollisionFence(&g_aPlayerHDR[nCntPlayer]);

				PrintDebugProc("%dPのヒップドロップパワー：%d\n", nCntPlayer + 1, g_aPlayerHDR[nCntPlayer].HipDropPower);
			}
		}
	}

	PrintDebugProc("バフ倍率：%f\n", HipDropBuff());
}

//========================
//描画処理
//========================
void DrawPlayer_HDR(void)
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
		if (g_aPlayerHDR[nCntPlayer].bUsePlayer == true)
		{
			//"プレイヤーの"ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_aPlayerHDR[nCntPlayer].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerHDR[nCntPlayer].rot.y, g_aPlayerHDR[nCntPlayer].rot.x, g_aPlayerHDR[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].mtxWorld, &g_aPlayerHDR[nCntPlayer].mtxWorld, &mtxRot);

			//位置反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayerHDR[nCntPlayer].pos.x, g_aPlayerHDR[nCntPlayer].pos.y, g_aPlayerHDR[nCntPlayer].pos.z);
			D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].mtxWorld, &g_aPlayerHDR[nCntPlayer].mtxWorld, &mtxTrans);

			//"プレイヤーの"ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerHDR[nCntPlayer].mtxWorld);

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				if (g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].bUse == true)
				{
					g_aPlayerHDR[nCntPlayer].posOld_Coll.y = g_aPlayerHDR[nCntPlayer].pos.y;

					D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
					D3DXMATRIX mtxParent;					//親のマトリ

															//仮でオフセットをそのまま使う（アニメーション使うようになったら消して）
					g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos = g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].posOffset;
					g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot = g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rotOffset;

					//"モデルの"ワールドマトリックス初期化
					D3DXMatrixIdentity(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld);

					//向きを反映
					D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.y, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.x, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].rot.z);
					D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxRotModel);

					//位置反映
					D3DXMatrixTranslation(&mtxTransModel, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.x, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.y, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pos.z);
					D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxTransModel);

					//パーツの親マトリ設定
					if (g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mIdxModelParent != -1)
					{
						mtxParent = g_aPlayerHDR[nCntPlayer].model.aParts[g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mIdxModelParent].mtxWorld;
					}
					else
					{
						mtxParent = g_aPlayerHDR[nCntPlayer].mtxWorld;
					}

					//パーツのマトリと親マトリをかけ合わせる
					D3DXMatrixMultiply(&g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld, &mtxParent);

					//"プレイヤーの"ワールドマトリックス設定
					pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].mtxWorld);

					//マテリアルデータへのポインタ取得
					pMat = (D3DXMATERIAL*)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].dwNumMatModel; nCntMat++)
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
						pDevice->SetTexture(0, g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].apTexture[nCntMat]);

						//モデル描画
						g_aPlayerHDR[nCntPlayer].model.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
					}
				}
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//プレイヤーのゲームパッド操作
//========================
void ControllPlayer_HDR(int nPlayerNum)
{
	//ヒップドロップ中でなければ操作できる
	if (g_aPlayerHDR[nPlayerNum].bHipDrop == false)
	{
		//ジャンプ・ヒップドロップ
		if (g_aPlayerHDR[nPlayerNum].bUseAI == false)
		{//プレイヤー
			if (GetButton(nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A) == true && g_aPlayerHDR[nPlayerNum].bHipDrop == false)
			{
				if (g_aPlayerHDR[nPlayerNum].bJump && g_aPlayerHDR[nPlayerNum].bGoal == false)
				{//ジャンプ中であり、ゴールしていない
					HipDropPlayer_HDR(nPlayerNum);		//プレイヤーのヒップドロップ処理
				}
				else if (g_aPlayerHDR[nPlayerNum].bJump == false)
				{//ジャンプしていない
					JumpPlayer_HDR(nPlayerNum);			//プレイヤーのジャンプ処理
				}
			}
		}
		else if (g_aPlayerHDR[nPlayerNum].nAICT <= 0)
		{
			if (g_aPlayerHDR[nPlayerNum].bJump && g_aPlayerHDR[nPlayerNum].bGoal == false)
			{//ジャンプ中であり、ゴールしていない
				//ジャンプ量が一定に達しているか確認する
				if (g_aPlayerHDR[nPlayerNum].pos.y >= g_aPlayerHDR[nPlayerNum].fHeightLastJump + g_aPlayerHDR[nPlayerNum].fAIHeight)
				{
					HipDropPlayer_HDR(nPlayerNum);		//プレイヤーのヒップドロップ処理
				}
			}
			else if (g_aPlayerHDR[nPlayerNum].bJump == false)
			{//ジャンプしていない
				JumpPlayer_HDR(nPlayerNum);			//プレイヤーのジャンプ処理
				if (g_aPlayerHDR[nPlayerNum].bUseAI == true)
				{//AIのジャンプレベル指定処理
					AIDIFF aiDiff = g_aPlayerHDR[nPlayerNum].aiDiff;
					int nRandAll = c_aAIParamHDR[aiDiff].nHDLevel_1 + c_aAIParamHDR[aiDiff].nHDLevel_2 +
						c_aAIParamHDR[aiDiff].nHDLevel_3 + c_aAIParamHDR[aiDiff].nHDLevel_Max;
					int nJumpRand = rand() % nRandAll;

					//どのレベルか判別する処理
					if (nJumpRand < c_aAIParamHDR[aiDiff].nHDLevel_1)
					{
						g_aPlayerHDR[nPlayerNum].fAIHeight = HIPDROP_HEIGHT_NICE;
					}
					else if (nJumpRand < c_aAIParamHDR[aiDiff].nHDLevel_1 + c_aAIParamHDR[aiDiff].nHDLevel_2)
					{
						g_aPlayerHDR[nPlayerNum].fAIHeight = HIPDROP_HEIGHT_GOOD;
					}
					else if (nJumpRand < c_aAIParamHDR[aiDiff].nHDLevel_1 + c_aAIParamHDR[aiDiff].nHDLevel_2 + c_aAIParamHDR[aiDiff].nHDLevel_3)
					{
						g_aPlayerHDR[nPlayerNum].fAIHeight = HIPDROP_HEIGHT_GREAT;
					}
					else
					{
						g_aPlayerHDR[nPlayerNum].fAIHeight = HIPDROP_HEIGHT_PERFECT;
					}
				}
			}
		}
	}

	//ゴール後の煽り行為
	if (g_aPlayerHDR[nPlayerNum].bGoal == true)
	{
		MovePlayer(nPlayerNum);
	}
}

//========================
//プレイヤーが普通に移動する処理
//========================
void MovePlayer(int nPadNum)
{
	//モデル移動
	//ゲームパッド部
	if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
	{//X方向のスティックが傾いている
		if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y方向のスティックも傾いている
			g_aPlayerHDR[nPadNum].rot.y = FIX_ROT(atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum)) + D3DX_PI);
			g_aPlayerHDR[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
			g_aPlayerHDR[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = FIX_ROT(atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum)) + D3DX_PI);
			g_aPlayerHDR[nPadNum].move.x = (float)GetLStickX(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
		}
	}
	else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
	{//Y方向のスティックだけ傾いている
		g_aPlayerHDR[nPadNum].rot.y = FIX_ROT(atan2f((float)GetLStickX(nPadNum), (float)GetLStickY(nPadNum)) + D3DX_PI);
		g_aPlayerHDR[nPadNum].move.z = (float)GetLStickY(nPadNum) / STICK_MAX * PLAYER_MOVE_SPEED;
	}
	//キーボード部
	else if (GetStick(nPadNum,INPUTTYPE_PRESS).y == CONVSTICK_UP)
	{
		if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_WA;
		}
		else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_WD;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_W;
		}
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).y == CONVSTICK_DOWN)
	{
		if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_SA;
		}
		else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_SD;
		}
		else
		{
			g_aPlayerHDR[nPadNum].rot.y = ROT_S;
		}
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
	{
		g_aPlayerHDR[nPadNum].rot.y = ROT_A;
	}
	else if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
	{
		g_aPlayerHDR[nPadNum].rot.y = ROT_D;
	}

	//キーが押されたら移動
	if (GetStick(nPadNum, INPUTTYPE_PRESS).x != CONVSTICK_NEUTRAL || GetStick(nPadNum, INPUTTYPE_PRESS).y != CONVSTICK_NEUTRAL)
	{
		g_aPlayerHDR[nPadNum].move.x = sinf(FIX_ROT((g_aPlayerHDR[nPadNum].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
		g_aPlayerHDR[nPadNum].move.z = cosf(FIX_ROT((g_aPlayerHDR[nPadNum].rot.y + D3DX_PI))) * PLAYER_MOVE_SPEED;
	}
	else
	{//そうでないので終了
		return;
	}

	//ボタン操作に応じてプレイヤー・カメラ視点・注視点移動
	g_aPlayerHDR[nPadNum].pos.x += g_aPlayerHDR[nPadNum].move.x;
	g_aPlayerHDR[nPadNum].pos.z += g_aPlayerHDR[nPadNum].move.z;

	//移動量消す
	g_aPlayerHDR[nPadNum].move.x = 0.0f;
	g_aPlayerHDR[nPadNum].move.z = 0.0f;
}

//========================
//ジャンプの処理
//========================
void JumpPlayer_HDR(int nJumpPlayer)
{
	PlaySoundSE(SOUND_LABEL_SE_JUMP,nJumpPlayer);

	g_aPlayerHDR[nJumpPlayer].posOld.y = g_aPlayerHDR[nJumpPlayer].pos.y;
	g_aPlayerHDR[nJumpPlayer].jumpTime = 0;	//ジャンプ時間リセット
	g_aPlayerHDR[nJumpPlayer].fHeightLastJump = g_aPlayerHDR[nJumpPlayer].pos.y;
	g_aPlayerHDR[nJumpPlayer].bJump = true;

	//ゴールしていた時のジャンプ量
	if (g_aPlayerHDR[nJumpPlayer].bGoal)
	{
		g_aPlayerHDR[nJumpPlayer].moveV0.y = PLAYER_GOAL_JUMP;
	}
	//まだゴールしていないときのジャンプ量
	else
	{
		g_aPlayerHDR[nJumpPlayer].moveV0.y = PLAYER_NORMAL_JUMP;
	}
}

//========================
//ヒップドロップの処理
//========================
void HipDropPlayer_HDR(int nHipDropPlayer)
{
	//プレイヤーのジャンプ量を保存
	float fJumpHeight = g_aPlayerHDR[nHipDropPlayer].pos.y - g_aPlayerHDR[nHipDropPlayer].fHeightLastJump;

	//ヒップドロップ量最大
	if (fJumpHeight >= HIPDROP_HEIGHT_PERFECT)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_PERFECT;	//最大パワー代入
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_PERFECT);					//UI設定
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_04_PERFECT, nHipDropPlayer);		//ヒップドロップランクのサウンド再生
	}

	//ヒップドロップ量準最大
	else if (fJumpHeight >= HIPDROP_HEIGHT_GREAT)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_GREAT;	//準最大パワー代入
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_GREAT);						//UI設定
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_03_GREAT, nHipDropPlayer);		//ヒップドロップランクのサウンド再生
	}

	//ヒップドロップ量弱め
	else if (fJumpHeight >= HIPDROP_HEIGHT_GOOD)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_GOOD;	//弱めのパワー代入
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_GOOD);					//UI設定
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_02_GOOD, nHipDropPlayer);	//ヒップドロップランクのサウンド再生
	}

	//ヒップドロップ量かなり弱め
	else if	(fJumpHeight >= HIPDROP_HEIGHT_NICE)
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = HIPDROP_POWER_NICE;	//最弱パワー代入
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_NICE);					//UI設定
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_01_NICE, nHipDropPlayer);		//ヒップドロップランクのサウンド再生
	}

	//ヒップドロップ量ナシ
	else
	{
		g_aPlayerHDR[nHipDropPlayer].HipDropPower = 0;	//威力ナシ
		SetRankUI(nHipDropPlayer, HIPDROP_RANK_BAD);	//UI設定
		PlaySoundSE(SOUND_LABEL_SE_HIPRANK_00_BAD, nHipDropPlayer);		//ヒップドロップランクのサウンド再生
	}

	//時間経過のバフ（力を倍に）
	g_aPlayerHDR[nHipDropPlayer].HipDropPower *= HipDropBuff();

	PlaySoundSE(SOUND_LABEL_SE_HIPSPIN, nHipDropPlayer);//ヒップドロップ音再生

	g_aPlayerHDR[nHipDropPlayer].move.y = 0.0f;						//通常の落下速度を０にする
	g_aPlayerHDR[nHipDropPlayer].moveV0.y = -15.0f;					//ヒップドロップの降下速度を代入
	g_aPlayerHDR[nHipDropPlayer].jumpTime = 0;						//ジャンプ時間リセット
	g_aPlayerHDR[nHipDropPlayer].bHipDrop = true;					//ヒップドロップしている
	g_aPlayerHDR[nHipDropPlayer].bHipDropSpin = true;				//スピンしている
	g_aPlayerHDR[nHipDropPlayer].nHipDropWait = PLAYER_HIPSPIN_WAIT;//ヒップドロップスピン後の硬直時間設定
}

//========================
//ヒップスピンの処理
//========================
void HipSpinPlayer_HDR(int nHipSpinPlayer)
{
	//ヒップドロップのスピン中	
	if (g_aPlayerHDR[nHipSpinPlayer].bHipDropSpin)
	{
		//前転させる				
		g_aPlayerHDR[nHipSpinPlayer].rot.x += PLAYER_HIPSPIN_SPEED;

		//１周した				
		if (g_aPlayerHDR[nHipSpinPlayer].rot.x <= PLAYER_HIPSPIN_LAP)
		{
			g_aPlayerHDR[nHipSpinPlayer].rot.x = 0.0f;			//回転値を元に戻す
			g_aPlayerHDR[nHipSpinPlayer].bHipDropSpin = false;	//スピンし終わった		
		}
	}

	//ヒップドロップ硬直時間がある
	else
	{
		g_aPlayerHDR[nHipSpinPlayer].nHipDropWait--;		//硬直時間を減らしていく
	}
}

//========================
//プレイヤー指定処理
//========================
void SetPlayerType_HDR(int nPlayerNum, bool bUse, bool bAIUse)
{
	//プレイヤー使用するか指定
	g_aPlayerHDR[nPlayerNum].bUsePlayer = bUse;

	//プレイヤー使用するならAI指定
	if (g_aPlayerHDR[nPlayerNum].bUsePlayer == true)
	{
		g_aPlayerHDR[nPlayerNum].bUseAI = bAIUse;
		//g_aAIMove_HDR[nPlayerNum] = bAIUse;
	}
	else
	{//使用しないならAIも使用しない
		g_aPlayerHDR[nPlayerNum].bUseAI = false;
		//g_aAIMove_HDR[nPlayerNum] = false;
	}
}

//========================
//取得処理
//========================
Player_HDR *GetPlayer_HDR(void)
{
	return &g_aPlayerHDR[0];
}