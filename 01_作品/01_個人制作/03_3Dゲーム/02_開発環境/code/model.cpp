/*==========================================================================================================================================================

														モデル処理[model.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "model.h"
#include "shadow.h"
#include "particle.h"
#include "fade.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "meshfield.h"
#include "explosion.h"
#include "score.h"

#define MODEL_TEX			(10)		// モデルのテクスチャの最大数
#define NUM_MODEL			(24)		// モデルの総数
#define MODEL_LIFE			(3)			// モデルの寿命
#define FLASH_COUNTER_MODEL	(3)			// 点滅カウンタ
#define MODEL_GRAVITY		(-2.0f)		// モデルの重力
#define APPEAR_COUNTER		(60)		// 出現カウンタ
#define DAMEGE_COUNTER		(40)		// ダメージカウンタ
#define MODEL_FILE			"data/CSV/model.csv"

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureModel[MODEL_TEX] = {};	// テクスチャへのポインタ
LPD3DXMESH				g_pMeshModel[MODELTYPE_MAX];		// メッシュ(頂点情報)へのポインタ
D3DXMATRIX				g_mtxWorldModel;					// ワールドマトリックス
LPD3DXBUFFER			g_pBuffMatModel[MODELTYPE_MAX];		// マテリアルへのポインタ
DWORD					g_dwNumMatModel[MODELTYPE_MAX];		// マテリアルの数
Model					g_Model[NUM_MODEL];					// モデルの情報
D3DXVECTOR3				g_MaxPosModel[MODELTYPE_MAX],		// モデルの種類別　最大座標値
						g_MinPosModel[MODELTYPE_MAX];		// モデルの種類別　最小座標値

#define ENDGAMECOUNT	(60)
VanishModel				g_EndGameModel;

// Xファイル名
const char *c_apFileNameModel[MODELTYPE_MAX] = 
{
	"data/MODEL/木箱.x",					//木箱のファイル名
	"data/MODEL/木箱群.x",					//木箱群のファイル名
	"data/MODEL/hana_chan.x",				//ハナちゃんのファイル名
};

//----------------------------------------------------
//				モデルの初期化処理
//----------------------------------------------------
void InitModel(void)
{
	//モデルの初期位置・ライフ読み込み
	LoadModel();

	//Xファイルの情報を格納する変数の初期化
	for (int nCntMesh = 0; nCntMesh < MODELTYPE_MAX; nCntMesh++)
	{
		g_pMeshModel[nCntMesh] = NULL;		// メッシュ初期化
		g_pBuffMatModel[nCntMesh] = NULL;	// マテリアルへのポインタ初期化
		g_dwNumMatModel[nCntMesh] = NULL;	// マテリアルの数初期化
	}

	//最小・最大位置設定
	SearchModel_MinAndMax_Pos();

	//モデルの情報初期化
	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{		
		g_Model[nCntModel].rot				= ZERO_SET;						// 位置初期化
		g_Model[nCntModel].PosOld			= g_Model[nCntModel].pos;		// 前回の位置初期化
		g_Model[nCntModel].move				= ZERO_SET;						// 移動量初期化
		g_Model[nCntModel].nIdxShadow		= SetShadow();					// 影の設定
		g_Model[nCntModel].nStateCounter	= ZERO;							// 状態カウンタ初期化
		g_Model[nCntModel].state			= MODELSTATE_NORMAL;			// 状態初期化
		g_Model[nCntModel].bUse				= true;							// 使われている状態に
		g_Model[nCntModel].bDisp			= true;							// 表示するかどうか初期化
		g_Model[nCntModel].rot				= ZERO_SET;						// 向き初期化
		SetModel_Max_Min_Pos(nCntModel);									// モデルの最小・最大位置設定
	}

	g_EndGameModel.nDeadModelCount = ZERO;		// ゲーム終了までのカウント
	g_EndGameModel.nCount = ENDGAMECOUNT;		// ゲーム終了までのカウント
}

//モデルの情報読み込み処理
void LoadModel(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(MODEL_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルタイトル読み込み

		for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			g_Model[nCntModel].pos.x = atof(strtok(&aData[ZERO], ","));	// 開始位置初期化
			g_Model[nCntModel].pos.y = atof(strtok(NULL, ","));			// 開始位置初期化
			g_Model[nCntModel].pos.z = atof(strtok(NULL, ","));			// 開始位置初期化

			g_Model[nCntModel].nLife = atoi(strtok(NULL, ","));			// ライフの初期化
			g_Model[nCntModel].nPoint = atoi(strtok(NULL, ","));		// ポイント初期化
			g_Model[nCntModel].type = (MODELTYPE)atoi(strtok(NULL,","));// 種類の初期化
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//当たり判定に使用する最小・最大値設定処理
void SearchModel_MinAndMax_Pos(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	for (int nCntLoadModel = 0; nCntLoadModel < MODELTYPE_MAX; nCntLoadModel++)
	{
		// Xファイル読み込み
		D3DXLoadMeshFromX(c_apFileNameModel[nCntLoadModel], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatModel[nCntLoadModel], NULL, &g_dwNumMatModel[nCntLoadModel], &g_pMeshModel[nCntLoadModel]);

		//モデルのテクスチャ設定
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatModel[nCntLoadModel]; nCntMat++)
		{
			//マテリアルデータへのポインタを取得
			D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatModel[nCntLoadModel]->GetBufferPointer();

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する
			 //テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureModel[nCntMat]);
			}
		}

		int		nNumVtx;		//頂点数
		DWORD	dwSizeFVF;		//貯点フォーマットのサイズ
		BYTE	*pVtxBuff;		//頂点バッファへのポインタ

		//頂点数を取得
		nNumVtx = g_pMeshModel[nCntLoadModel]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshModel[nCntLoadModel]->GetFVF());

		//頂点バッファをロック
		g_pMeshModel[nCntLoadModel]->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

		D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;		//頂点座標の代入

		g_MinPosModel[nCntLoadModel] = vtx;		//最小の位置をモデルの原点の位置から求める
		g_MaxPosModel[nCntLoadModel] = vtx;		//最大の位置をモデルの原点の位置から求める

		//頂点フォーマットのサイズ分、ポインタを進める
		pVtxBuff += dwSizeFVF;

		vtx = *(D3DXVECTOR3 *)pVtxBuff;		//頂点座標の代入

		//当たり判定に使う頂点の最小・最大位置を取得
		for (int nCntVtx = ONE; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 現在格納されている最小・最大値を頂点座標で塗り替えるかどうか判定
			//------------------------------
			//			Xの判定
			//------------------------------
			// 最小値判定
			if (vtx.x <= g_MinPosModel[nCntLoadModel].x)
			{
				g_MinPosModel[nCntLoadModel].x = vtx.x;		//最小値代入
			}

			// 最大値判定
			else if (g_MaxPosModel[nCntLoadModel].x <= vtx.x)
			{
				g_MaxPosModel[nCntLoadModel].x = vtx.x;		//最大値代入
			}

			//------------------------------
			//			Yの判定
			//------------------------------
			// 最小値判定
			if (vtx.y <= g_MinPosModel[nCntLoadModel].y)
			{
				g_MinPosModel[nCntLoadModel].y = vtx.y;		//最小値代入
			}

			// 最大値判定
			else if (g_MaxPosModel[nCntLoadModel].y <= vtx.y)
			{
				g_MaxPosModel[nCntLoadModel].y = vtx.y;		//最大値代入
			}

			//------------------------------
			//			Zの判定
			//------------------------------
			// 最小値判定
			if (vtx.z <= g_MinPosModel[nCntLoadModel].z)
			{
				g_MinPosModel[nCntLoadModel].z = vtx.z;		//最小値代入
			}

			// 最大値判定
			else if (g_MaxPosModel[nCntLoadModel].z <= vtx.z)
			{
				g_MaxPosModel[nCntLoadModel].z = vtx.z;		//最大値代入
			}

			//頂点フォーマットのサイズ分、ポインタを進める
			pVtxBuff += dwSizeFVF;

			vtx = *(D3DXVECTOR3 *)pVtxBuff;		//頂点座標の代入
		}

		//頂点バッファをアンロック
		g_pMeshModel[nCntLoadModel]->UnlockVertexBuffer();
	}
}

//----------------------------------------------------
//					モデルの終了処理
//----------------------------------------------------
void UninitModel(void)
{
	//メッシュとマテリアルの破棄
	for (int nCntModel = START_CNT; nCntModel < MODELTYPE_MAX; nCntModel++)
	{
		//メッシュ破棄
		if (g_pMeshModel[nCntModel] != NULL)
		{
			g_pMeshModel[nCntModel]->Release();
			g_pMeshModel[nCntModel] = NULL;
		}

		//マテリアル破棄
		if (g_pBuffMatModel[nCntModel] != NULL)
		{
			g_pBuffMatModel[nCntModel]->Release();
			g_pBuffMatModel[nCntModel] = NULL;
		}
	}

	for (int nCntTex = START_CNT; nCntTex < MODEL_TEX; nCntTex++)
	{
		//テクスチャ破棄
		if (g_pTextureModel[nCntTex] != NULL)
		{
			g_pTextureModel[nCntTex]->Release();
			g_pTextureModel[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------------
//					モデルの更新処理
//----------------------------------------------------
void UpdateModel(void)
{
	//全モデルの更新処理
	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//状態分岐処理
		SwitchStateModel(nCntModel);

		//モデルが使われている
		if (g_Model[nCntModel].bUse == true)
		{
			//前回の位置更新
			g_Model[nCntModel].PosOld = g_Model[nCntModel].pos;

			//表示されている
			if (g_Model[nCntModel].bDisp == true)
			{
				g_Model[nCntModel].move.y = MODEL_GRAVITY;		//モデルの重力設定

				//モデルの影の大きさを計算
				float ShadowWidth = g_Model[nCntModel].MinPos.x - g_Model[nCntModel].MaxPos.x;
				float ShadowDepth = g_Model[nCntModel].MinPos.z - g_Model[nCntModel].MaxPos.z;

				//影の描画位置設定
				SetPositionShadow(g_Model[nCntModel].nIdxShadow, g_Model[nCntModel].pos, ShadowWidth, ShadowDepth);
			}

			//表示されていない
			else
			{
				//影を消す
				BreakShadow(g_Model[nCntModel].nIdxShadow);
			}

			//モデルの移動
			g_Model[nCntModel].pos += g_Model[nCntModel].move;

			MeshField *Null;	//引数だけ
			ColliMeshField(&g_Model[nCntModel].pos, &g_Model[nCntModel].PosOld, &g_Model[nCntModel].move, &Null);

			// モデルの最小・最大位置設定
			SetModel_Max_Min_Pos(nCntModel);

			//モデルの落ちたかどうかの処理
			FallModel(nCntModel);

			//モデル同士の当たり判定
			ColliModel_to_Model(nCntModel);
		}
	}

	//消失しているモデルの数計測
	DeadCountModel();
}

//モデルの状態分岐処理
void SwitchStateModel(int nCntModel)
{
	//状態分岐
	switch (g_Model[nCntModel].state)
	{
		//通常状態
	case MODELSTATE_NORMAL:
	{

	}
		break;

		//出現状態
	case MODELSTATE_APPEAR:
	{
		//状態カウンタを減らす
		g_Model[nCntModel].nStateCounter--;

		//カウンターがゼロになった
		if (g_Model[nCntModel].nStateCounter <= ZERO)
		{
			g_Model[nCntModel].state = MODELSTATE_NORMAL;	// 通常状態に
			g_Model[nCntModel].bDisp = true;				// 表示する
		}
		else
		{
			//点滅させる
			if (g_Model[nCntModel].nStateCounter % FLASH_COUNTER_MODEL == ZERO)
			{
				g_Model[nCntModel].bDisp = g_Model[nCntModel].bDisp ? false : true;
			}
		}
	}
		break;

		//ダメージ状態
	case MODELSTATE_DAMAGE:
	{
		//状態カウンタを減らす
		g_Model[nCntModel].nStateCounter--;

		//カウンターがゼロになった
		if (g_Model[nCntModel].nStateCounter <= ZERO)
		{
			g_Model[nCntModel].state = MODELSTATE_NORMAL;	// 通常状態に
			g_Model[nCntModel].bDisp = true;				// 表示する
		}
		else
		{
			//点滅させる
			if (g_Model[nCntModel].nStateCounter % FLASH_COUNTER_MODEL == ZERO)
			{
				g_Model[nCntModel].bDisp = g_Model[nCntModel].bDisp ? false : true;
			}
		}
	}
	break;

		//消えている状態
	case MODELSTATE_NONE:
	{

	}
		break;
	}
}

//落ちているモデルの処理
void FallModel(int nCntModel)
{
	// フィールドの枠を超えた
	if (g_Model[nCntModel].pos.x < -MESHFIELD_SIZE || MESHFIELD_SIZE < g_Model[nCntModel].pos.x ||
		g_Model[nCntModel].pos.z < -MESHFIELD_SIZE || MESHFIELD_SIZE < g_Model[nCntModel].pos.z ||
		g_Model[nCntModel].pos.y < NIL_F)
	{
		//影を消す
		BreakShadow(g_Model[nCntModel].nIdxShadow);
	}

	//モデルの位置が奈落に落ちてしまった
	if (g_Model[nCntModel].pos.y <= THE_END_HELL)
	{
		//モデルを使えなくする
		g_Model[nCntModel].bUse = false;
		g_Model[nCntModel].bDisp = false;
		SetExplosion(g_Model[nCntModel].pos, XCOL_WHITE);	//爆発処理
	}
}

//消えているモデルの数計測処理
void DeadCountModel(void) 
{
	g_EndGameModel.nDeadModelCount = ZERO;

	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//表示されていなくて、使われてもいなくて、消えている状態に設定されている
		if (g_Model[nCntModel].bDisp == false && g_Model[nCntModel].bUse == false && g_Model[nCntModel].state == MODELSTATE_NONE)
		{
			//消失モデルの数を加算する
			g_EndGameModel.nDeadModelCount++;

			if (g_EndGameModel.nDeadModelCount > NUM_MODEL)
			{
				g_EndGameModel.nDeadModelCount = NUM_MODEL;
			}
		}
	}

	//消えているモデルの数が、全体のモデルの数以上になった
	if (g_EndGameModel.nDeadModelCount >= NUM_MODEL)
	{
		//ゲーム終了のカウント設定
		g_EndGameModel.nCount--;

		if (g_EndGameModel.nCount <= ZERO)
		{
			//ゲームクリア
			SetEndGame(EndGAME_CLEAR);

			//リザルト画面に移行
			SetFade(MODE_RESULT);
		}
	}
}

//----------------------------------------------------
//					モデルの描画処理
//----------------------------------------------------
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	D3DXCOLOR pMatStrage;						//マテリアルデータ保存用

	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//使用されている　＆＆　表示されている
		if (g_Model[nCntModel].bUse == true && g_Model[nCntModel].bDisp == true)
		{
			int type = g_Model[nCntModel].type;

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldModel);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model[nCntModel].rot.y, g_Model[nCntModel].rot.x, g_Model[nCntModel].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Model[nCntModel].pos.x, g_Model[nCntModel].pos.y, g_Model[nCntModel].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL *)g_pBuffMatModel[type]->GetBufferPointer();

			for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatModel[type]; nCntMat++)
			{
				//保存用ストレージに代入
				pMatStrage = pMat[nCntMat].MatD3D.Diffuse;

				//ダメージ状態である
				if (g_Model[nCntModel].state == MODELSTATE_DAMAGE)
				{
					//赤色に設定
					pMat[nCntMat].MatD3D.Diffuse = XCOL_RED;
				}

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(ZERO, g_pTextureModel[nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshModel[type]->DrawSubset(nCntMat);

				//色を戻す
				pMat[nCntMat].MatD3D.Diffuse = pMatStrage;
			}

			//マテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//モデル同士の当たり判定
void ColliModel_to_Model(int nCntTarget)
{
	/*------------------  この処理の中身  ------------------

	対象のモデルとその他のモデルの位置を比較し、
	比較されるモデルが、対象のモデルに
		「前回はめり込んでいなくて	現在めり込んでいる」
	時に、比較されるモデルを　対象のモデルのめり込んでいない位置に直す
	処理を行う。
		基本的にＹ座標を戻す　今後モデルが横移動するなら、
	Ｘ・Ｚの当たり判定も行う										*/

	//比較される側の番号		Comparison  =  比較
	for (int nCntComparison = 0; nCntComparison < NUM_MODEL; nCntComparison++)
	{
		if (nCntTarget != nCntComparison)			//対象のモデル番号と比較されるモデル番号が違う
		{
			if(g_Model[nCntTarget].bUse == true && g_Model[nCntComparison].bUse == true)	//両方のモデルが使われている
			{
				D3DXVECTOR3 TargetMinPos = g_Model[nCntTarget].MinPos;	//対象の最小の位置
				D3DXVECTOR3 TargetMaxPos = g_Model[nCntTarget].MaxPos;	//対象の最大の位置

				if ((TargetMinPos.x < g_Model[nCntComparison].pos.x && g_Model[nCntComparison].pos.x < TargetMaxPos.x) &&
					(TargetMinPos.z < g_Model[nCntComparison].pos.z && g_Model[nCntComparison].pos.z < TargetMaxPos.z))
				{
					if (TargetMaxPos.y <= g_Model[nCntComparison].PosOld.y &&	//前回は対象の頂上位置以上の高さにいて
						TargetMaxPos.y > g_Model[nCntComparison].pos.y)			//現在は頂上位置より下にいる
					{
						g_Model[nCntComparison].pos.y = TargetMaxPos.y;			//対象の頂上位置に合わせる
						g_Model[nCntComparison].move.y = NIL_F;					//重力を０に
						break;
					}
				}
			}
		}
	}
}

//モデルの当たり判定
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType)
{
	bool bCollision = false;		//当たったかどうかの判定
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得

	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//モデルが使われている	かつ	通常状態である
		if (g_Model[nCntModel].bUse == true)
		{
			// Xの最小値と最大値の範囲内にいる
			if (g_Model[nCntModel].MinPos.x <= pPos->x && pPos->x <= g_Model[nCntModel].MaxPos.x &&
				g_Model[nCntModel].MinPos.y <= pPos->y && pPos->y <= g_Model[nCntModel].MaxPos.y &&
				g_Model[nCntModel].MinPos.z <= pPos->z && pPos->z <= g_Model[nCntModel].MaxPos.z)
			{
				//当たったかどうか判定
				ColliModel_Max(pPos, pPosold, nCntModel, nType);	// 最大値の当たり判定
				ColliModel_Min(pPos, pPosold, nCntModel, nType);	// 最小値の当たり判定

				//当たったものが弾だった　かつ　モデルの状態が通常状態である
				if (nType == COLLITYPE_BULLET && g_Model[nCntModel].state == MODELSTATE_NORMAL)
				{
					// 体力を減らす
					g_Model[nCntModel].nLife--;

					//体力が尽きた
					if (g_Model[nCntModel].nLife <= ZERO)
					{
						g_Model[nCntModel].bUse = false;											// 使用されていない状態に
						g_Model[nCntModel].bDisp = false;											// 表示されていない状態に
						g_Model[nCntModel].state = MODELSTATE_NONE;									// 消えた状態に
						BreakShadow(g_Model[nCntModel].nIdxShadow);									// 影を消す
						SetParticle(g_Model[nCntModel].pos, SET_PARTICLE_SLOW_EX, MODEL_DEAD_COL);	// パーティクルゆっくり発生
						AddScore(g_Model[nCntModel].nPoint);										// スコア加算
					}

					//体力がまだある
					else
					{
						g_Model[nCntModel].state = MODELSTATE_DAMAGE;		// ダメージ状態に設定
						g_Model[nCntModel].nStateCounter = DAMEGE_COUNTER;	// ダメージカウンタ設定
					}

					//ヒット音再生
					PlaySound(SOUND_LABEL_SE_HIT);
				}

				//当たったものがプレイヤーだった　かつ  対象のモデルがハナちゃんだった			　 かつ　プレイヤーがダメージ状態ではない
				else if (nType == COLLITYPE_PLAYER && g_Model[nCntModel].type == MODELTYPE_HANACHAN && pPlayer->state != PLAYERSTATE_DAMEGE)
				{
					//プレイヤーの体力を減らす
					pPlayer->nLife--;

					//体力が尽きてしまった
					if (pPlayer->nLife <= ZERO)
					{
						//死亡状態に
						pPlayer->state = PLAYERSTATE_DEAD;

						//ゲームオーバー
						SetEndGame(EndGAME_OVER);
					}

					//まだ体力がある
					else
					{
						//ダメージ状態に
						pPlayer->state = PLAYERSTATE_DAMEGE;
					}

					//ヒット音再生
					PlaySound(SOUND_LABEL_SE_HIT);
				}

				//当たった
				bCollision = true;

				//もしあたったらfor文を抜ける
				break;
			}
		}
	}

	//判定を返す
	return bCollision;
}

//最大値を使った当たり判定
bool ColliModel_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType)
{	
	bool bCollision = false;	//当たったかどうかの判定

	if (g_Model[nCntModel].MaxPos.x <= pPosold->x)		//前回の位置が対象のモデルの Xの最小位置のより大きかった
	{
		pPos->x = g_Model[nCntModel].MaxPos.x;			//今回の位置を対象のモデルの Xの位置にする
		bCollision = true;								//当たった
	}

	if (g_Model[nCntModel].MaxPos.y <= pPosold->y)		//前回の位置が対象のモデルの Zの最小位置のより大きかった
	{
		pPos->y = g_Model[nCntModel].MaxPos.y;			//今回の位置を対象のモデルの Zの位置にする
		bCollision = true;								//当たった
	}


	if (g_Model[nCntModel].MaxPos.z <= pPosold->z)		//前回の位置が対象のモデルの Zの最小位置のより大きかった
	{
		pPos->z = g_Model[nCntModel].MaxPos.z;			//今回の位置を対象のモデルの Zの位置にする
		bCollision = true;								//当たった
	}

	//判定を返す
	return bCollision;
}

//最小値を使った当たり判定
bool ColliModel_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType)
{
	bool bCollision = false;	//当たったかどうかの判定

	if (pPosold->x <= g_Model[nCntModel].MinPos.x)		//前回の位置が対象のモデルの Xの最小位置のより小さかった
	{
		pPos->x = g_Model[nCntModel].MinPos.x;			//今回の位置を対象のモデルの Xの位置にする
		bCollision = true;								//当たった
	}

	if (pPosold->y <= g_Model[nCntModel].MinPos.y)		//前回の位置が対象のモデルの Zの最小位置のより小さかった
	{
		pPos->y = g_Model[nCntModel].MinPos.y;			//今回の位置を対象のモデルの Zの位置にする
		bCollision = true;								//当たった
	}


	if (pPosold->z <= g_Model[nCntModel].MinPos.z)		//前回の位置が対象のモデルの Zの最小位置のより小さかった
	{
		pPos->z = g_Model[nCntModel].MinPos.z;			//今回の位置を対象のモデルの Zの位置にする
		bCollision = true;								//当たった
	}

	//判定を返す
	return bCollision;	
}

//モデルの上に影を描画する
void ColliModelShadow(int nIdxShadow, D3DXVECTOR3 pos, float *pDrawPosY)
{
	for (int nCntModel = START_CNT; nCntModel < NUM_MODEL; nCntModel++)
	{
		//モデルが使われている	AND		そのモデルの影の番号と描画する影の番号が違う
		if (g_Model[nCntModel].bUse == true && g_Model[nCntModel].nIdxShadow != nIdxShadow)
		{
			if (g_Model[nCntModel].MinPos.x < pos.x && pos.x < g_Model[nCntModel].MaxPos.x &&		// X座標軸の最小値と最大値の範囲内にいる
				g_Model[nCntModel].MinPos.y < pos.y && pos.y <= g_Model[nCntModel].MaxPos.y &&		// Y座標軸の最小値と最大値の範囲内にいる
				g_Model[nCntModel].MinPos.z < pos.z && pos.z < g_Model[nCntModel].MaxPos.z &&		// Z座標軸の最小値と最大値の範囲内にいる
				*pDrawPosY <= g_Model[nCntModel].MaxPos.y)											// 修正する位置が対象のモデルの最大Y座標より下にいる
			{
				//修正位置を代入
				*pDrawPosY = g_Model[nCntModel].MaxPos.y;
			}
		}
	}

}

//モデルの情報取得
Model *GetModel(void)
{
	//先頭のアドレスを返す
	return &g_Model[MODELTYPE_WOODEN_BLOCK];
}

//消失しているモデルの情報取得
VanishModel *GetEndModel(void)
{
	return &g_EndGameModel;
}

//モデルの最大・最小位置を設定
void SetModel_Max_Min_Pos(int nCntModel)
{
	int type = g_Model[nCntModel].type;		//　文字数削減のため、種類を格納

	g_Model[nCntModel].MaxPos = g_Model[nCntModel].pos + g_MaxPosModel[type];	// 最大位置代入
	g_Model[nCntModel].MinPos = g_Model[nCntModel].pos + g_MinPosModel[type];	// 最小位置代入
	g_Model[nCntModel].MinPos.y = g_Model[nCntModel].pos.y;						// 最小Ｙ座標はずれるので、現在位置を代入
}