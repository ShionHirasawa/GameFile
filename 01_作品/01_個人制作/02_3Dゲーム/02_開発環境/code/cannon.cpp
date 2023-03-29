/*==========================================================================================================================================================

														大砲の処理[cannon.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "cannon.h"
#include "shadow.h"
#include "particle.h"
#include "fade.h"
#include "player.h"
#include "sound.h"

#define CANNON_TEX						(10)								// 大砲のテクスチャの最大数
#define NUM_CANNON						(5)									// 大砲の総数
#define CANNON_XFILE					"data/MODEL/cannon.x"				// 大砲のXファイル名
#define CANNON_INFOFILE					"data/CSV/cannon.csv"				// 大砲の情報ファイル名
#define CANNON_BULLET_COL				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// 大砲の弾の色
#define CANNON_BULLET_SIZE				(18.0f)								// 大砲の弾のサイズ
#define CANNON_BULLET_SPEED				(-4.0f)								// 大砲の弾の速度
#define CANNON_BULLET_CORRECT_HEIGHT	(18.0f)								// 弾の発射位置調整
#define CANNON_SEARCH_RANGE				(700.0f)							// 弾を発射する範囲
#define CANNON_CHARGE_TIME				(300)								// 大砲のチャージ時間
#define CANNON_SHOOT_INTERVAL			(80)								// 発射してから回転するまでのインターバル
#define CANNON_ROTATE_MOVE				(0.02f)								// 大砲の旋回速度

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureCannon[CANNON_TEX] = {};	// テクスチャへのポインタ
LPD3DXMESH				g_pMeshCannon;						// メッシュ(頂点情報)へのポインタ
D3DXMATRIX				g_mtxWorldCannon;					// ワールドマトリックス
LPD3DXBUFFER			g_pBuffMatCannon;					// マテリアルへのポインタ
DWORD					g_dwNumMatCannon;					// マテリアルの数
Cannon					g_Cannon[NUM_CANNON];				// 大砲の情報

//----------------------------------------------------
//					大砲の初期化処理
//----------------------------------------------------
void InitCannon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//大砲の情報初期化
	LoadCannon();

	g_pMeshCannon = NULL;			// メッシュ初期化
	g_pBuffMatCannon = NULL;		// マテリアルへのポインタ初期化
	g_dwNumMatCannon = ZERO;		// マテリアルの数初期化

	// Xファイル読み込み
	D3DXLoadMeshFromX(CANNON_XFILE, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCannon, NULL, &g_dwNumMatCannon, &g_pMeshCannon);

	//大砲のテクスチャ設定
	for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatCannon; nCntMat++)
	{
		//マテリアルデータへのポインタを取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatCannon->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureCannon[nCntMat]);
		}
	}

	//最小・最大位置設定
	SearchCannon_MinAndMax_Pos();
}

//大砲の情報読み込み処理
void LoadCannon(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(CANNON_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルタイトル読み込み

		for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			g_Cannon[nCntCannon].pos.x = atof(strtok(&aData[ZERO], ","));		// 開始位置初期化
			g_Cannon[nCntCannon].pos.y = atof(strtok(NULL, ","));				// 開始位置初期化
			g_Cannon[nCntCannon].pos.z = atof(strtok(NULL, ","));				// 開始位置初期化

			g_Cannon[nCntCannon].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;	// 向き初期化

			g_Cannon[nCntCannon].bRotate = (bool)atoi(strtok(NULL, ","));		// 大砲の開始向きを向ける
			g_Cannon[nCntCannon].nStateCounter = ZERO;							// 状態カウンター初期化
			g_Cannon[nCntCannon].state = CANNONSTATE_NORMAL;					// 状態初期化
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//当たり判定に使用する最小・最大値設定処理
void SearchCannon_MinAndMax_Pos(void)
{
	int		nNumVtx;		//頂点数
	DWORD	dwSizeFVF;		//貯点フォーマットのサイズ
	BYTE	*pVtxBuff;		//頂点バッファへのポインタ

	//頂点数を取得
	nNumVtx = g_pMeshCannon->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshCannon->GetFVF());

	//頂点バッファをロック
	g_pMeshCannon->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

	D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;		//頂点座標の代入

	//頂点フォーマットのサイズ分、ポインタを進める
	pVtxBuff += dwSizeFVF;

	D3DXVECTOR3 MinPos = vtx;						//最小の位置をモデルの原点の位置から求める
	D3DXVECTOR3 MaxPos = vtx;						//最大の位置をモデルの原点の位置から求める

	//当たり判定に使う頂点の最小・最大位置を取得
	for (int nCntVtx = ONE; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 現在格納されている最小・最大値を頂点座標で塗り替えるかどうか判定
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
	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		g_Cannon[nCntCannon].MinPos = g_Cannon[nCntCannon].pos + MinPos;		//最小の位置をモデルの原点の位置から求める
		g_Cannon[nCntCannon].MaxPos = g_Cannon[nCntCannon].pos + MaxPos;		//最大の位置をモデルの原点の位置から求める
	}

	//頂点バッファをアンロック
	g_pMeshCannon->UnlockVertexBuffer();
}

//----------------------------------------------------
//					大砲の終了処理
//----------------------------------------------------
void UninitCannon(void)
{
	//メッシュ破棄
	if (g_pMeshCannon != NULL)
	{
		g_pMeshCannon->Release();
		g_pMeshCannon = NULL;
	}

	//マテリアル破棄
	if (g_pBuffMatCannon != NULL)
	{
		g_pBuffMatCannon->Release();
		g_pBuffMatCannon = NULL;
	}

	//テクスチャ破棄
	for (int nCntTex = START_CNT; nCntTex < CANNON_TEX; nCntTex++)
	{
		if (g_pTextureCannon[nCntTex] != NULL)
		{
			g_pTextureCannon[nCntTex]->Release();
			g_pTextureCannon[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------------
//					大砲の更新処理
//----------------------------------------------------
void UpdateCannon(void)
{
	//全大砲の更新処理
	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		//状態分岐処理
		SwitchStateCannon(nCntCannon);
	}
}

//大砲の状態分岐処理
void SwitchStateCannon(int nCntCannon)
{
	//状態分岐
	switch (g_Cannon[nCntCannon].state)
	{	
	case CANNONSTATE_NORMAL:		//通常状態
	{
		D3DXVECTOR3 PlayerPos = GetPlayer()->pos;	//プレイヤーの位置
		float PosDiffX, PosDiffZ;					//プレイヤーと大砲の距離の差を格納

		//プレイヤーとの距離を計算
		PosDiffX = pow(g_Cannon[nCntCannon].pos.x - PlayerPos.x, 2.0f);	//2乗
		PosDiffZ = pow(g_Cannon[nCntCannon].pos.z - PlayerPos.z, 2.0f);	//2乗

		//距離が範囲内に収まった
		if (sqrtf(PosDiffX + PosDiffZ) <= CANNON_SEARCH_RANGE)
		{
			//チャージ状態に設定
			g_Cannon[nCntCannon].state = CANNONSTATE_RELOADING;
		}
	}
	break;

	case CANNONSTATE_RELOADING:		// 発射準備中
	{
		g_Cannon[nCntCannon].nStateCounter++;

		//発射からのインターバルを終えた
		if (g_Cannon[nCntCannon].nStateCounter >= CANNON_SHOOT_INTERVAL)
		{
			//大砲回転
			RotCannon(nCntCannon);
		}

		//カウンターがチャージ時間を超えた
		if (g_Cannon[nCntCannon].nStateCounter >= CANNON_CHARGE_TIME)
		{			
			g_Cannon[nCntCannon].state = CANNONSTATE_SHOOTING;	// 弾発射状態にする
			g_Cannon[nCntCannon].nStateCounter = ZERO;			// 状態カウンターをゼロにする
		}
	}
	break;

	case CANNONSTATE_SHOOTING:		// 弾発射
	{
		//弾の移動向き・量
		D3DXVECTOR3 BulletMove;

		BulletMove.x = sinf(g_Cannon[nCntCannon].rot.y) * CANNON_BULLET_SPEED;	//移動量・向き計算
		BulletMove.z = cosf(g_Cannon[nCntCannon].rot.y) * CANNON_BULLET_SPEED;	//移動量・向き計算
		BulletMove.y = F_ZERO;													//Yは変化なし

		//二方向に弾発射
		SetBullet(D3DXVECTOR3(g_Cannon[nCntCannon].pos.x, g_Cannon[nCntCannon].pos.y + CANNON_BULLET_CORRECT_HEIGHT, g_Cannon[nCntCannon].pos.z), BulletMove, CANNON_BULLET_COL, CANNON_BULLET_SIZE, BULLET_OWNER_CANNON);
		SetBullet(D3DXVECTOR3(g_Cannon[nCntCannon].pos.x, g_Cannon[nCntCannon].pos.y + CANNON_BULLET_CORRECT_HEIGHT, g_Cannon[nCntCannon].pos.z), -BulletMove, CANNON_BULLET_COL, CANNON_BULLET_SIZE, BULLET_OWNER_CANNON);
		
		//正面・横向きを変える
		g_Cannon[nCntCannon].bRotate = g_Cannon[nCntCannon].bRotate ? false : true;

		//状態を戻す
		g_Cannon[nCntCannon].state = CANNONSTATE_NORMAL;
	}
	break;

	}
}

//----------------------------------------------------
//					大砲の描画処理
//----------------------------------------------------
void DrawCannon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldCannon);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Cannon[nCntCannon].rot.y, g_Cannon[nCntCannon].rot.x, g_Cannon[nCntCannon].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldCannon, &g_mtxWorldCannon, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Cannon[nCntCannon].pos.x, g_Cannon[nCntCannon].pos.y, g_Cannon[nCntCannon].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldCannon, &g_mtxWorldCannon, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCannon);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)g_pBuffMatCannon->GetBufferPointer();

		//マテリアル設定
		for (int nCntMat = START_CNT; nCntMat < (int)g_dwNumMatCannon; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureCannon[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshCannon->DrawSubset(nCntMat);
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//大砲の回転処理
void RotCannon(int nCntCannon)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//大砲が正面を向いている
	if (g_Cannon[nCntCannon].bRotate == false)
	{
		//大砲を横方向に回転
		g_Cannon[nCntCannon].rot.y += CANNON_ROTATE_MOVE;

		//回転が横向きを超えてしまった
		if (g_Cannon[nCntCannon].rot.y >= RIGHT)
		{//横に戻す
			g_Cannon[nCntCannon].rot.y = RIGHT;
		}

		else
		{
			//プレイヤーが上に載っている
			if (g_Cannon[nCntCannon].MaxPos.y == pPlayer->pos.y)
			{//プレイヤーも回転
				pPlayer->rot.y += CANNON_ROTATE_MOVE;
			}
		}
	}

	//大砲が横を向いている
	else	
	{
		//大砲を正面方向に回転
		g_Cannon[nCntCannon].rot.y -= CANNON_ROTATE_MOVE;

		//回転が正面を超えてしまった
		if (g_Cannon[nCntCannon].rot.y <= F_ZERO)
		{//正面に戻す
			g_Cannon[nCntCannon].rot.y = F_ZERO;
		}

		else
		{
			//プレイヤーが上に載っている
			if (g_Cannon[nCntCannon].MaxPos.y == pPlayer->pos.y)
			{//プレイヤーも回転
				pPlayer->rot.y -= CANNON_ROTATE_MOVE;
			}
		}
	}
}

//大砲の当たり判定
bool CollisionCannon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType, bool *pLand, bool *pJump)
{
	bool bCollision = false;		//当たったかどうかの判定
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得

	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		// Xの最小値と最大値の範囲内にいる
		if (g_Cannon[nCntCannon].MinPos.x <= pPos->x && pPos->x <= g_Cannon[nCntCannon].MaxPos.x &&
			g_Cannon[nCntCannon].MinPos.y <= pPos->y && pPos->y <  g_Cannon[nCntCannon].MaxPos.y &&
			g_Cannon[nCntCannon].MinPos.z <= pPos->z && pPos->z <= g_Cannon[nCntCannon].MaxPos.z )
		{
			//当たったかどうか判定
			ColliCannon_Max(pPos, pPosold, nCntCannon, nType, pLand, pJump);	// 最大値の当たり判定
			ColliCannon_Min(pPos, pPosold, nCntCannon, nType);					// 最小値の当たり判定

			//当たった
			bCollision = true;

			//あたったらfor文を抜ける
			break;
		}
	}

	//判定を返す
	return bCollision;
}

//最大値を使った当たり判定
void ColliCannon_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntCannon, COLLITYPE nType, bool *pLand, bool *pJump)
{
	if (g_Cannon[nCntCannon].MaxPos.x <= pPosold->x)	//前回の位置が対象の大砲の Xの最小位置のより大きかった
	{
		pPos->x = g_Cannon[nCntCannon].MaxPos.x;		//今回の位置を対象の大砲の Xの位置にする
	}

	if (g_Cannon[nCntCannon].MaxPos.y <= pPosold->y)	//前回の位置が対象の大砲の Zの最小位置のより大きかった
	{
		pPos->y = g_Cannon[nCntCannon].MaxPos.y;		//今回の位置を対象の大砲の Zの位置にする
		*pLand = true;									//乗っている
		*pJump = false;									//ジャンプしていない
	}

	if (g_Cannon[nCntCannon].MaxPos.z <= pPosold->z)	//前回の位置が対象の大砲の Zの最小位置のより大きかった
	{
		pPos->z = g_Cannon[nCntCannon].MaxPos.z;		//今回の位置を対象の大砲の Zの位置にする
	}
}

//最小値を使った当たり判定
void ColliCannon_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntCannon, COLLITYPE nType)
{
	if (pPosold->x <= g_Cannon[nCntCannon].MinPos.x)	//前回の位置が対象の大砲の Xの最小位置のより小さかった
	{
		pPos->x = g_Cannon[nCntCannon].MinPos.x;		//今回の位置を対象の大砲の Xの位置にする
	}

	if (pPosold->y <= g_Cannon[nCntCannon].MinPos.y)	//前回の位置が対象の大砲の Zの最小位置のより小さかった
	{
		pPos->y = g_Cannon[nCntCannon].MinPos.y;		//今回の位置を対象の大砲の Zの位置にする
	}

	if (pPosold->z <= g_Cannon[nCntCannon].MinPos.z)	//前回の位置が対象の大砲の Zの最小位置のより小さかった
	{
		pPos->z = g_Cannon[nCntCannon].MinPos.z;		//今回の位置を対象の大砲の Zの位置にする
	}
}

//大砲の上に影を描画する
void ColliCannonShadow(int nIdxShadow, D3DXVECTOR3 pos, float *pDrawPosY)
{
	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		if (g_Cannon[nCntCannon].MinPos.x < pos.x && pos.x <  g_Cannon[nCntCannon].MaxPos.x &&		// X座標軸の最小値と最大値の範囲内にいる
			g_Cannon[nCntCannon].MinPos.y < pos.y && pos.y <= g_Cannon[nCntCannon].MaxPos.y &&		// Y座標軸の最小値と最大値の範囲内にいる
			g_Cannon[nCntCannon].MinPos.z < pos.z && pos.z <  g_Cannon[nCntCannon].MaxPos.z &&		// Z座標軸の最小値と最大値の範囲内にいる
			*pDrawPosY <= g_Cannon[nCntCannon].MaxPos.y)											// 修正する位置が対象の大砲の最大Y座標より下にいる
		{
			//修正位置を代入
			*pDrawPosY = g_Cannon[nCntCannon].MaxPos.y;
		}
	}
}

//大砲と弾の当たり判定
bool ColliCannonToBullet(D3DXVECTOR3 pos, float size, BULLET_OWNER owner)
{
	bool bCollision = false;		//あったかどうかの判定
	D3DXVECTOR3	fRange;				//弾のサイズと大砲の大きさの合計値

	for (int nCntCannon = START_CNT; nCntCannon < NUM_CANNON; nCntCannon++)
	{
		D3DXVECTOR3 posDiff = g_Cannon[nCntCannon].pos - pos;	//大砲と弾の距離の差
		fRange.x = (g_Cannon[nCntCannon].MaxPos.x - g_Cannon[nCntCannon].pos.x) + size;	//弾のサイズ と 大砲の大きさ の合計を算出
		fRange.y = (g_Cannon[nCntCannon].MaxPos.y - g_Cannon[nCntCannon].pos.y) + size;	//弾のサイズ と 大砲の大きさ の合計を算出
		fRange.z = (g_Cannon[nCntCannon].MaxPos.z - g_Cannon[nCntCannon].pos.z) + size;	//弾のサイズ と 大砲の大きさ の合計を算出

		//プレイヤーと弾との距離が弾のサイズ内に収まる
		if (-fRange.x <= posDiff.x && posDiff.x <= fRange.x &&
			-fRange.y <= posDiff.y && posDiff.y <= fRange.y &&
			-fRange.z <= posDiff.z && posDiff.z <= fRange.z &&
			owner != BULLET_OWNER_CANNON)						//プレイヤーがダメージ状態ではない
		{
			//ヒット音再生
			PlaySound(SOUND_LABEL_SE_HIT);

			bCollision = true;			//当たった
		}
	}

	//当たったかどうかの判定を返す
	return bCollision;
}