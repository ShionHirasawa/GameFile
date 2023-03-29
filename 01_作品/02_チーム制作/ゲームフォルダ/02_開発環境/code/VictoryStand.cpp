/*==========================================================================================================================================================

表彰台処理[VictoryStand.h]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "rank.h"
#include "debugproc.h"
#include "select_game.h"
#include "conversioninput.h"
#include "VictoryStand.h"
#include "resultPlayer.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "sound.h"

#define VICTORYSTAND_TEX		(10)			// 表彰台のテクスチャの最大数
#define NUM_VICTORYSTAND		(4)				// 表彰台の総数
#define VICSTD_FILL_HEIGHT		(2.0f)			// 初期埋め立て量補正値
#define VICSTD_STAIRS_HEIGHT	(8.0f)			// 表彰台を階段のように埋め立てる

#define VICSTD_APPEAR_TIME	(CONVERT_FPS(5))	// 表彰台のあらわれる時間
#define VICSTD_APPEAR_SPEED		(0.8f)			// 表彰台のあらわれるスピード

//グローバル変数
LPDIRECT3DTEXTURE9			g_pTextureVictoryStand[VICTORYSTAND_TEX] = {};	// テクスチャへのポインタ
LPD3DXMESH					g_pMeshVictoryStand[NUM_VICTORYSTAND];			// メッシュ(頂点情報)へのポインタ
D3DXMATRIX					g_mtxWorldVictoryStand;							// ワールドマトリックス
LPD3DXBUFFER				g_pBuffMatVictoryStand[NUM_VICTORYSTAND];		// マテリアルへのポインタ
DWORD						g_dwNumMatVictoryStand[NUM_VICTORYSTAND];		// マテリアルの数
VictoryStand				g_VictoryStand[NUM_VICTORYSTAND];				// 表彰台の情報
float						g_fLandPoint[NUM_VICTORYSTAND];					// 表彰台ごとの着地点を代入（同率を考慮して）
int							g_RankStrage[RANKING_MAX] = {};

//表彰台のⅩファイル名
const char *c_apVicStdFilePath[NUM_VICTORYSTAND] = {
	"data\\MODEL\\victorystand\\Stand_No1.x",
	"data\\MODEL\\victorystand\\Stand_No2.x",
	"data\\MODEL\\victorystand\\Stand_No3.x",
	"data\\MODEL\\victorystand\\Stand_No4.x",
};

//----------------------------------------------------
//				表彰台の初期化処理
//----------------------------------------------------
void InitVictoryStand(void)
{	
	//着地点設定
	SearchVictoryStand_Land_Pos();

	//対象の表彰台のポインタ取得
	VictoryStand *pVicStand = &g_VictoryStand[0];

	//リザルト用プレイヤーのポインタ取得
	Player_RESULT *pResPlayer = GetPlayer_RESULT();

	//表彰台の情報初期化
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStand++, pResPlayer++)
	{
		pVicStand->pos.x = pResPlayer->pos.x;		// Ｘ位置初期化
		pVicStand->pos.z = pVicStand->pos.y = 0.0f;	// Ｚ位置と Ｙ座標を０にする

		pVicStand->rot = ZERO_SET;					// 向き初期化
		pVicStand->bUse = true;						// 使われているようにする
		pVicStand->bAppear = false;					// 現れないようにする
		pVicStand->nRank = g_RankStrage[nCntVicStd] = 
			pResPlayer->nRank;	// 順位を初期化
	}

	//順位をソートする
	RankingSort();
}

//同率も考慮してランキングを再ソート
void RankingSort(void)
{
	//順位判定用
	int nSortRanking[RANKING_MAX] = { 0,0,0,0 };

	//ランキングをソートする
	//比較される方の配列番号を回す
	for (int nCntComp = 0; nCntComp < RANKING_MAX; nCntComp++)
	{
		//比較する方の番号を回す
		for (int nCntJudge = 0; nCntJudge < nCntComp; nCntJudge++)
		{
			switch (GetSelGameMode())
			{
				//=========================================
				//昇順にソートする
				//=========================================
			case SelGameMode_PVP:
			{
				//比較「される」方のポイントのほうが大きい
				if (g_VictoryStand[nCntJudge].nRank < g_VictoryStand[nCntComp].nRank)
				{
					nSortRanking[nCntJudge]++;//比較するほうの順位を下げる
				}

				//比較「する」方のポイントのほうが大きい
				else if (g_VictoryStand[nCntJudge].nRank > g_VictoryStand[nCntComp].nRank)
				{
					nSortRanking[nCntComp]++;//比較されるほうの順位を下げる
				}
			}
				break;

				//=========================================
				//降順にソートする
				//=========================================
			case SelGameMode_HDR:
			{
				//比較「される」方のポイントのほうが大きい
				if (g_VictoryStand[nCntJudge].nRank < g_VictoryStand[nCntComp].nRank)
				{
					nSortRanking[nCntComp]++;//比較されるほうの順位を下げる
				}

				//比較「する」方のポイントのほうが大きい
				else if (g_VictoryStand[nCntJudge].nRank > g_VictoryStand[nCntComp].nRank)
				{
					nSortRanking[nCntJudge]++;//比較するほうの順位を下げる
				}
			}
				break;
			}

			//*****************************
			//同点の時は何もしない
			//*****************************
		}
	}

	//ソートしたランキングを代入する
	for (int nCntPay = 0; nCntPay < RANKING_MAX; nCntPay++)
	{
		//順位　＆　文章削減用変数　に代入
		int nRank = g_VictoryStand[nCntPay].nRank = nSortRanking[nCntPay];
		
		// 着地点を初期化
		g_VictoryStand[nCntPay].fLandheight = g_fLandPoint[nRank];	

		//最初は地中に埋める（左から順番に埋め立てる深さを増やしていく
		g_VictoryStand[nCntPay].pos.y -= g_VictoryStand[nCntPay].fLandheight + VICSTD_FILL_HEIGHT * (nCntPay * VICSTD_STAIRS_HEIGHT);
	}
}

//当たり判定に使用する最小・最大値設定処理
void SearchVictoryStand_Land_Pos(void)
{
	for (int nCntLandPos = 0; nCntLandPos < NUM_VICTORYSTAND; nCntLandPos++)
	{
		g_pMeshVictoryStand[nCntLandPos] = NULL;	// メッシュ初期化
		g_pBuffMatVictoryStand[nCntLandPos] = NULL;	// マテリアルへのポインタ初期化
		g_dwNumMatVictoryStand[nCntLandPos] = NULL;	// マテリアルの数初期化

		/***********************************************
			Xファイルの読み込み and テクスチャ設定
		*************************************************/
		InitVicStdModel_Tex(nCntLandPos);

		int		nNumVtx;		//頂点数
		DWORD	dwSizeFVF;		//貯点フォーマットのサイズ
		BYTE	*pVtxBuff;		//頂点バッファへのポインタ

		//頂点数を取得
		nNumVtx = g_pMeshVictoryStand[nCntLandPos]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshVictoryStand[nCntLandPos]->GetFVF());

		//頂点バッファをロック
		g_pMeshVictoryStand[nCntLandPos]->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

		//当たり判定に使う頂点の最小・最大位置を取得
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

			//頂点フォーマットのサイズ分、ポインタを進める
			pVtxBuff += dwSizeFVF;

			// 現在表彰台の取得した頂点座標のＹ座標が、変数に代入されている着地位置より高ければ、上書きする
			g_fLandPoint[nCntLandPos] = g_fLandPoint[nCntLandPos] < vtx.y ? vtx.y : g_fLandPoint[nCntLandPos];
		}

		//頂点バッファをアンロック
		g_pMeshVictoryStand[nCntLandPos]->UnlockVertexBuffer();
	}
}

//メッシュやマテリアルの初期化・設定処理
void InitVicStdModel_Tex(int nCntModex) 
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイル読み込み
	D3DXLoadMeshFromX(c_apVicStdFilePath[nCntModex], D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatVictoryStand[nCntModex], NULL, &g_dwNumMatVictoryStand[nCntModex], &g_pMeshVictoryStand[nCntModex]);

	//表彰台のテクスチャ設定
	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nCntModex]; nCntMat++)
	{
		//マテリアルデータへのポインタを取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nCntModex]->GetBufferPointer();

		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_pTextureVictoryStand[nCntModex]);
			break;
		}
	}
}

//----------------------------------------------
//				表彰台の終了処理
//----------------------------------------------
void UninitVictoryStand(void)
{
	//メッシュとマテリアルの破棄
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//メッシュ破棄
		if (g_pMeshVictoryStand[nCntVicStd] != NULL)
		{
			g_pMeshVictoryStand[nCntVicStd]->Release();
			g_pMeshVictoryStand[nCntVicStd] = NULL;
		}

		//マテリアル破棄
		if (g_pBuffMatVictoryStand[nCntVicStd] != NULL)
		{
			g_pBuffMatVictoryStand[nCntVicStd]->Release();
			g_pBuffMatVictoryStand[nCntVicStd] = NULL;
		}
	}

	//テクスチャ破棄
	for (int nCntTex = 0; nCntTex < VICTORYSTAND_TEX; nCntTex++)
	{
		if (g_pTextureVictoryStand[nCntTex] != NULL)
		{
			g_pTextureVictoryStand[nCntTex]->Release();
			g_pTextureVictoryStand[nCntTex] = NULL;
		}
	}
}

//----------------------------------------------
//				表彰台の更新処理
//----------------------------------------------
void UpdateVictoryStand(void)
{
	//表彰台のポインタを取得
	VictoryStand *pVicStd = &g_VictoryStand[0];

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStd++)
	{
		//地面から現れてもよい
		if (pVicStd->bAppear)
		{
			//地面から現れる
			pVicStd->pos.y += VICSTD_APPEAR_SPEED;

			//表彰台が地面から浮いてしまったら
			if (0.0f < pVicStd->pos.y)
			{
				//地面に戻す
				pVicStd->pos.y = 0.0f;
			}
		}
	}
}

//----------------------------------------------------
//					表彰台の描画処理
//----------------------------------------------------
void DrawVictoryStand(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	VictoryStand *pVicStd = &g_VictoryStand[0];	//表彰台のポインタを取得

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStd++)
	{
		//使われている
		if (pVicStd->bUse)
		{
			//順位格納
			int nRank = pVicStd->nRank;

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldVictoryStand);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pVicStd->rot.y, pVicStd->rot.x, pVicStd->rot.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pVicStd->pos.x, pVicStd->pos.y, pVicStd->pos.z);

			D3DXMatrixMultiply(&g_mtxWorldVictoryStand, &g_mtxWorldVictoryStand, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldVictoryStand);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL *)g_pBuffMatVictoryStand[nRank]->GetBufferPointer();

			//マテリアル設定
			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatVictoryStand[nRank]; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureVictoryStand[nRank]);

				//与えられた順位別のモデル(パーツ)の描画
				g_pMeshVictoryStand[nRank]->DrawSubset(nCntMat);
			}

			//マテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//表彰台の当たり判定
bool CollisionVictoryStand(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold)
{
	bool bCollision = false;		//当たったかどうかの判定

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//文字数削減のために、対象の表彰台の情報を格納
		VictoryStand VicStd = g_VictoryStand[nCntVicStd];

		//最初は地中に埋まっているので、着地点を都度、計算する
		float LandPoint = VicStd.pos.y + VicStd.fLandheight;

		//対象のプレイヤーと表彰台のⅩ位置が一致している（表彰台の位置は、プレイヤーの順位ごとに真下に配置するので、Ⅹ座標以外の判定は無し
		if (pPos->x == VicStd.pos.x)
		{
			if (pPos->y <= LandPoint)		//現在は着地点以下の高さにいる
			{
				//プレイヤーの位置を着地点に修正する
				pPos->y = LandPoint;
			}
		}
	}

	//判定を返す
	return bCollision;
}

//----------------------------------------------------
//					表彰台の設定処理
//----------------------------------------------------
void SetVictoryStand(D3DXVECTOR3 Setpos)
{
	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++)
	{
		//表彰台が使われていない
		if (g_VictoryStand[nCntVicStd].bUse == false)
		{
			g_VictoryStand[nCntVicStd].pos = Setpos;	//位置設定
			g_VictoryStand[nCntVicStd].rot = ZERO_SET;	//向き設定

			g_VictoryStand[nCntVicStd].bUse = true;		//使う
			break;//処理を抜ける
		}
	}
}

//----------------------------------------------------
//					表彰台の出現処理
//----------------------------------------------------
void SetAppearVictoryStand(void)
{
	//表彰台のポインタを取得
	VictoryStand *pVicStd = &g_VictoryStand[0];

	for (int nCntVicStd = 0; nCntVicStd < NUM_VICTORYSTAND; nCntVicStd++, pVicStd++)
	{
		//出現させる
		pVicStd->bAppear = true;
	}
}

//****************************************************
//	表彰台、プレイヤーのリザルト演出スキップ時の処理
//****************************************************
void SkipVicStd_Player(void)
{
	VictoryStand *pVicStd = &g_VictoryStand[0];		//表彰台のポインタを取得
	Player_RESULT *pPlayer = GetPlayer_RESULT();	//プレイヤーのポインタも取得

	for (int nCntResSkip = 0; nCntResSkip < NUM_VICTORYSTAND; nCntResSkip++, pVicStd++, pPlayer++)
	{
		//-------------------
		//表彰台の処理
		//-------------------
		pVicStd->pos.y = 0.0f;	//表彰台を地表に出す
		pVicStd->bAppear = true;//出現させる

		//-------------------
		//プレイヤーの処理
		//-------------------
		pPlayer->pos.y = pVicStd->fLandheight;
	}
}