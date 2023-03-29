/*==========================================================================================================================================================

丸太トゲの処理[logthorns.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "logthorns.h"
#include "player.h"
#include "meshfield.h"
#include "sound.h"
#include "slope.h"

#define LOGTHORNS_TEX					(10)						// 丸太トゲのテクスチャの最大数
#define NUM_LOGTHORNS					(64)						// 丸太トゲの総数
#define LOGTHORNS_XFILE					"data/MODEL/LogThorns.x"	// 丸太トゲのファイル名
#define LOGTHORNS_INFOFILE				"data/CSV/logthorns.csv"	// 丸太トゲの情報ファイル名
#define LOGTHORNS_ROLL_SPEED			(-0.1f)						// 丸太トゲの回転速度
#define LOGTHORNS_CHARGE_TIME			(300)						// 丸太トゲのチャージ時間
#define LOGTHORNS_SPWAN_INTERVAL		(80)						// 生み出し間隔
#define LOGTHORNS_GRAVITY				(-3.0f)						// 丸太トゲの重力
#define LOGTHORNS_FIX_HEIGHT_POS		(1.0f)						// 丸太トゲの位置を修正する高さ

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureLogThorns[LOGTHORNS_TEX] = {};// テクスチャへのポインタ
LPD3DXMESH				g_pMeshLogThorns;						// メッシュ(頂点情報)へのポインタ
D3DXMATRIX				g_mtxWorldLogThorns;					// ワールドマトリックス
LPD3DXBUFFER			g_pBuffMatLogThorns;					// マテリアルへのポインタ
DWORD					g_dwNumMatLogThorns;					// マテリアルの数
LogThorns				g_LogThorns[NUM_LOGTHORNS];				// 丸太トゲの情報

//----------------------------------------------------
//				丸太トゲの初期化処理
//----------------------------------------------------
void InitLogThorns(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//丸太トゲの情報初期化
	for (int nCntLogThorns = 0; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		g_LogThorns[nCntLogThorns].pos = ZERO_SET;		// 位置初期化
		g_LogThorns[nCntLogThorns].posOld = ZERO_SET;	// 前回位置初期化
		g_LogThorns[nCntLogThorns].move = ZERO_SET;		// 移動量初期化
		g_LogThorns[nCntLogThorns].rot = ZERO_SET;		// 向き初期化
		g_LogThorns[nCntLogThorns].bUse = false;		// 使われていないようにする
	}	

	g_pMeshLogThorns = NULL;		// メッシュ初期化
	g_pBuffMatLogThorns = NULL;		// マテリアルへのポインタ初期化
	g_dwNumMatLogThorns = NULL;		// マテリアルの数初期化

	// Xファイル読み込み
	D3DXLoadMeshFromX(LOGTHORNS_XFILE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatLogThorns, NULL, &g_dwNumMatLogThorns, &g_pMeshLogThorns);

	//丸太トゲのテクスチャ設定
	for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatLogThorns; nCntMat++)
	{
		//マテリアルデータへのポインタを取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatLogThorns->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureLogThorns[nCntMat]);
		}
	}

	//最小・最大位置設定
	SearchLogThorns_MinAndMax_Pos();
}

//当たり判定に使用する最小・最大値設定処理
void SearchLogThorns_MinAndMax_Pos(void)
{
	int		nNumVtx;		//頂点数
	DWORD	dwSizeFVF;		//貯点フォーマットのサイズ
	BYTE	*pVtxBuff;		//頂点バッファへのポインタ

	//頂点数を取得
	nNumVtx = g_pMeshLogThorns->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshLogThorns->GetFVF());

	//頂点バッファをロック
	g_pMeshLogThorns->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

	//頂点座標の代入
	D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

	//頂点フォーマットのサイズ分、ポインタを進める
	pVtxBuff += dwSizeFVF;

	D3DXVECTOR3 MinPos = vtx;	//最小の位置をモデルの原点の位置から求める
	D3DXVECTOR3 MaxPos = vtx;	//最大の位置をモデルの原点の位置から求める

	//当たり判定に使う頂点の最小・最大位置を取得
	for (int nCntVtx = ONE; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 現在格納されている最小・最大値を読み込んだ頂点座標で塗り替えるかどうか判定
		//------------------------------
		//			Xの判定
		//------------------------------
		// 最小値判定
		if (vtx.x <= MinPos.x)
		{
			MinPos.x = vtx.x;		//最小値代入
		}

		// 最大値判定
		else if (MaxPos.x <= vtx.x)
		{
			MaxPos.x = vtx.x;		//最大値代入
		}

		//------------------------------
		//			Yの判定
		//------------------------------
		// 最小値判定
		if (vtx.y <= MinPos.y)
		{
			MinPos.y = vtx.y;		//最小値代入
		}

		// 最大値判定
		else if (MaxPos.y <= vtx.y)
		{
			MaxPos.y = vtx.y;		//最大値代入
		}

		//------------------------------
		//			Zの判定
		//------------------------------
		// 最小値判定
		if (vtx.z <= MinPos.z)
		{
			MinPos.z = vtx.z;		//最小値代入
		}

		// 最大値判定
		else if (MaxPos.z <= vtx.z)
		{
			MaxPos.z = vtx.z;		//最大値代入
		}

		//頂点フォーマットのサイズ分、ポインタを進める
		pVtxBuff += dwSizeFVF;

		//頂点座標の代入
		vtx = *(D3DXVECTOR3 *)pVtxBuff;
	}

	//最小位置と最大位置を代入
	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		g_LogThorns[nCntLogThorns].MinPos = g_LogThorns[nCntLogThorns].pos + MinPos;		//最小の位置をモデルの原点の位置から求める
		g_LogThorns[nCntLogThorns].MaxPos = g_LogThorns[nCntLogThorns].pos + MaxPos;		//最大の位置をモデルの原点の位置から求める
		g_LogThorns[nCntLogThorns].pos.y = g_LogThorns[nCntLogThorns].MinPos.y;				//原点位置は最小にする
	}

	//頂点バッファをアンロック
	g_pMeshLogThorns->UnlockVertexBuffer();
}

//----------------------------------------------
//				丸太トゲの終了処理
//----------------------------------------------
void UninitLogThorns(void)
{
	//メッシュ破棄
	if (g_pMeshLogThorns != NULL)
	{
		g_pMeshLogThorns->Release();
		g_pMeshLogThorns = NULL;
	}

	//マテリアル破棄
	if (g_pBuffMatLogThorns != NULL)
	{
		g_pBuffMatLogThorns->Release();
		g_pBuffMatLogThorns = NULL;
	}

	//テクスチャ破棄
	for (int nCntTex = START_CNT; nCntTex < LOGTHORNS_TEX; nCntTex++)
	{
		if (g_pTextureLogThorns[nCntTex] != NULL)
		{
			g_pTextureLogThorns[nCntTex]->Release();
			g_pTextureLogThorns[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------
//				丸太トゲの更新処理
//----------------------------------------------
void UpdateLogThorns(void)
{
	//丸太トゲの更新処理
	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//使われている
		if (g_LogThorns[nCntLogThorns].bUse)
		{
			//前回の位置更新
			g_LogThorns[nCntLogThorns].posOld = g_LogThorns[nCntLogThorns].pos;

			g_LogThorns[nCntLogThorns].move.y += (LOGTHORNS_GRAVITY - g_LogThorns[nCntLogThorns].move.y) * 0.1f;

			//丸太トゲの移動処理
			MoveLogthorns(nCntLogThorns);

			//丸太トゲの回転処理
			RollLogThorns(nCntLogThorns);

			//丸太トゲの当たり判定まとめ	落下判定処理
			WholeColliLogThorns(nCntLogThorns);
		}
	}
}

//丸太トゲの移動処理
void MoveLogthorns(int nCntLogThorns)
{
	g_LogThorns[nCntLogThorns].pos += g_LogThorns[nCntLogThorns].move;	//移動
}

//丸太トゲの回転処理
void RollLogThorns(int nCntLogThorns)
{
	g_LogThorns[nCntLogThorns].rot.x += LOGTHORNS_ROLL_SPEED;

	//回転値Ｘが-3.14を下回った
	if (g_LogThorns[nCntLogThorns].rot.x <= -D3DX_PI)
	{
		//3.14に修正
		g_LogThorns[nCntLogThorns].rot.x = D3DX_PI;
	}
}

//丸太トゲの当たり判定まとめ
void WholeColliLogThorns(int nCntLogThorns)
{
	//引数だけ
	MeshField *Null;

	//フィールドと坂道の当たり判定
	if (ColliMeshField(&g_LogThorns[nCntLogThorns].pos, &g_LogThorns[nCntLogThorns].posOld, &g_LogThorns[nCntLogThorns].move, &Null) == true ||
		CollisionSlope(&g_LogThorns[nCntLogThorns].pos, &g_LogThorns[nCntLogThorns].posOld, &g_LogThorns[nCntLogThorns].move, &g_LogThorns[nCntLogThorns].rot, COLLITYPE_ENEMY) == true)
	{		
		g_LogThorns[nCntLogThorns].pos.y = g_LogThorns[nCntLogThorns].posOld.y;	//位置修正
		g_LogThorns[nCntLogThorns].move.y = NIL_F;								//重力ゼロ
	}

	//奈落まで落ちてしまった
	if (g_LogThorns[nCntLogThorns].pos.y <= THE_END_HELL)
	{
		//使わなくする
		g_LogThorns[nCntLogThorns].bUse = false;
	}
}

//----------------------------------------------------
//					丸太トゲの描画処理
//----------------------------------------------------
void DrawLogThorns(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//使われている
		if (g_LogThorns[nCntLogThorns].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldLogThorns);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_LogThorns[nCntLogThorns].rot.y, g_LogThorns[nCntLogThorns].rot.x, g_LogThorns[nCntLogThorns].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldLogThorns, &g_mtxWorldLogThorns, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_LogThorns[nCntLogThorns].pos.x, g_LogThorns[nCntLogThorns].pos.y, g_LogThorns[nCntLogThorns].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldLogThorns, &g_mtxWorldLogThorns, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldLogThorns);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL *)g_pBuffMatLogThorns->GetBufferPointer();

			//マテリアル設定
			for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatLogThorns; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(ZERO, g_pTextureLogThorns[nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshLogThorns->DrawSubset(nCntMat);
			}

			//マテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//丸太トゲの当たり判定
bool CollisionLogThorns(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType, bool *pLand, bool *pJump)
{
	bool bCollision = false;		//当たったかどうかの判定
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得

	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//使われている
		if (g_LogThorns[nCntLogThorns].bUse)
		{
			// それぞれの座標の最小・最大位置の範囲内にいる
			if (g_LogThorns[nCntLogThorns].MinPos.x <= pPos->x && pPos->x <= g_LogThorns[nCntLogThorns].MaxPos.x &&
				g_LogThorns[nCntLogThorns].MinPos.y <= pPos->y && pPos->y < g_LogThorns[nCntLogThorns].MaxPos.y &&
				g_LogThorns[nCntLogThorns].MinPos.z <= pPos->z && pPos->z <= g_LogThorns[nCntLogThorns].MaxPos.z)
			{
				//当たったかどうか判定
				ColliLogThorns_Max(pPos, pPosold, nCntLogThorns, nType, pLand, pJump);	// 最大値の当たり判定
				ColliLogThorns_Min(pPos, pPosold, nCntLogThorns, nType);				// 最小値の当たり判定
				bCollision = true;														// 当たった
				break;																	// あたったらfor文を抜ける
			}
		}
	}

	//判定を返す
	return bCollision;
}

//最大値を使った当たり判定
void ColliLogThorns_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntLogThorns, COLLITYPE nType, bool *pLand, bool *pJump)
{
	if (g_LogThorns[nCntLogThorns].MaxPos.x <= pPosold->x)	//前回の位置が対象の丸太トゲの Xの最小位置のより大きかった
	{
		pPos->x = g_LogThorns[nCntLogThorns].MaxPos.x;		//今回の位置を対象の丸太トゲの Xの位置にする
	}

	if (g_LogThorns[nCntLogThorns].MaxPos.y <= pPosold->y)	//前回の位置が対象の丸太トゲの Zの最小位置のより大きかった
	{
		pPos->y = g_LogThorns[nCntLogThorns].MaxPos.y;		//今回の位置を対象の丸太トゲの Zの位置にする
		*pLand = true;										//乗っている
		*pJump = false;										//ジャンプしていない
	}

	if (g_LogThorns[nCntLogThorns].MaxPos.z <= pPosold->z)	//前回の位置が対象の丸太トゲの Zの最小位置のより大きかった
	{
		pPos->z = g_LogThorns[nCntLogThorns].MaxPos.z;		//今回の位置を対象の丸太トゲの Zの位置にする
	}
}

//最小値を使った当たり判定
void ColliLogThorns_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntLogThorns, COLLITYPE nType)
{
	if (pPosold->x <= g_LogThorns[nCntLogThorns].MinPos.x)	//前回の位置が対象の丸太トゲの Xの最小位置のより小さかった
	{
		pPos->x = g_LogThorns[nCntLogThorns].MinPos.x;		//今回の位置を対象の丸太トゲの Xの位置にする
	}

	if (pPosold->y <= g_LogThorns[nCntLogThorns].MinPos.y)	//前回の位置が対象の丸太トゲの Zの最小位置のより小さかった
	{
		pPos->y = g_LogThorns[nCntLogThorns].MinPos.y;		//今回の位置を対象の丸太トゲの Zの位置にする
	}

	if (pPosold->z <= g_LogThorns[nCntLogThorns].MinPos.z)	//前回の位置が対象の丸太トゲの Zの最小位置のより小さかった
	{
		pPos->z = g_LogThorns[nCntLogThorns].MinPos.z;		//今回の位置を対象の丸太トゲの Zの位置にする
	}
}

//丸太トゲと弾の当たり判定
bool ColliLogThornsToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner)
{
	D3DXVECTOR3	fRange;				//弾のサイズと丸太トゲの大きさの合計値

	for (int nCntLogThorns = START_CNT; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//使われている
		if (g_LogThorns[nCntLogThorns].bUse)
		{
			D3DXVECTOR3 posDiff = g_LogThorns[nCntLogThorns].pos - pos;	//丸太トゲと弾の距離の差

			fRange.x = (g_LogThorns[nCntLogThorns].MaxPos.x - g_LogThorns[nCntLogThorns].pos.x) + size;	//弾のサイズ と 丸太トゲの大きさ の合計を算出
			fRange.y = (g_LogThorns[nCntLogThorns].MaxPos.y - g_LogThorns[nCntLogThorns].pos.y) + size;	//弾のサイズ と 丸太トゲの大きさ の合計を算出
			fRange.z = (g_LogThorns[nCntLogThorns].MaxPos.z - g_LogThorns[nCntLogThorns].pos.z) + size;	//弾のサイズ と 丸太トゲの大きさ の合計を算出

																										//プレイヤーと弾との距離が弾のサイズ内に収まる
			if (-fRange.x <= posDiff.x && posDiff.x <= fRange.x &&
				-fRange.y <= posDiff.y && posDiff.y <= fRange.y &&
				-fRange.z <= posDiff.z && posDiff.z <= fRange.z)
			{
				//ヒット音再生
				PlaySound(SOUND_LABEL_SE_HIT);

				return true;			//当たった
			}
		}
	}

	//当たらなかった
	return false;
}

//----------------------------------------------------
//					丸太トゲの設定処理
//----------------------------------------------------
void SetLogThorns(D3DXVECTOR3 Setpos, D3DXVECTOR3 Setmove, D3DXVECTOR3 Setrot)
{
	for (int nCntLogThorns = 0; nCntLogThorns < NUM_LOGTHORNS; nCntLogThorns++)
	{
		//丸太トゲが使われていない
		if (g_LogThorns[nCntLogThorns].bUse == false)
		{
			g_LogThorns[nCntLogThorns].pos = Setpos;	//位置設定
			g_LogThorns[nCntLogThorns].move = Setmove;	//移動量設定
			g_LogThorns[nCntLogThorns].rot = Setrot;	//向き設定

			g_LogThorns[nCntLogThorns].bUse = true;		//使う
			break;//処理を抜ける
		}
	}
}