/*==========================================================================================================================================================

															坂道の壁の処理[slopewall.cpp]
																Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"
#include "slopewall.h"

//マクロ定義
#define NUM_SLOPEWALL		(2)									// 坂道の壁の数
#define SLOPEWALL_NOR		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))		// 坂道の壁のnor

#define SLOPEWALL_TEX_NAME	"data/TEXTURE/fault000.png"	// 断面のファイル名
#define SLOPEWALL_ORI_POS	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// 断面の原点座標
#define SLOPEWALL_FILE		"data/CSV/slopewall.csv"			// 断面の情報が入ったファイル名

//坂道の壁(断面)の頂点列挙型
typedef enum
{
	SLOPEWALL_VTX_START = ZERO,		// 斜面のふもとのスタート座標
	SLOPEWALL_VTX_START_TOP,		// 斜面の頂上で、スタート座標からみた次の頂点
	SLOPEWALL_VTX_START_LOW,		// ひとつ前の頂点の真下の頂点
	SLOPEWALL_VTX_START_DIAGONAL,	// スタート座標の対角線の位置にある頂点
	SLOPEWALL_VTX_LOW_DIAGONAL,		// スタート座標の底辺の対角線
	SLOPEWALL_VTX_SAME_DIAGONAL,	// 縮退ポリゴンにするため 4番目( 頂点番号は3番)と同じ
	SLOPEWALL_VTX_LAST,				// 斜面のふもとのラスト座標
	SLOPEWALL_VTX_MAX
}SLOPEWALL_VTX;

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSlopeWall = NULL;					//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureSlopeWall = NULL;	//テクスチャ
SlopeWall				g_SlopeWall[NUM_SLOPEWALL];					//坂道の壁の情報

//------------------------------------------------
//				坂道の断面の初期化処理
//------------------------------------------------
void InitSlopeWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ	

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * SLOPEWALL_VTX_MAX * NUM_SLOPEWALL, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffSlopeWall, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSlopeWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	/*------------------------------------
			坂道の壁の情報読み込み
	------------------------------------*/
	LoadSlopeWall();

	D3DXCreateTextureFromFile(pDevice, SLOPEWALL_TEX_NAME, &g_pTextureSlopeWall);	//テクスチャの読み込み

	//各種情報・頂点座標の初期化
	for (int nCntSlopeWall = START_CNT; nCntSlopeWall < NUM_SLOPEWALL; nCntSlopeWall++, pVtx += SLOPEWALL_VTX_MAX)
	{
		float TexWidth = F_ZERO;	//テクスチャの幅

		//頂点座標の設定
		for (int nCntVtx = START_CNT; nCntVtx < SLOPEWALL_VTX_MAX; nCntVtx++)
		{
			//-------------------------
			//		X座標の設定
			//-------------------------
			//坂道の方向が手前向きの場合
			if (g_SlopeWall[nCntSlopeWall].sloperot == SLOPEROT_FRONT_BACK)
			{
				//最初の三角形を形成する頂点とその他の頂点は原点からの幅が逆
				if (nCntVtx <= SLOPEWALL_VTX_START_LOW)
				{
					pVtx[nCntVtx].pos.x = +g_SlopeWall[nCntSlopeWall].fWidth;
				}
				else
				{
					pVtx[nCntVtx].pos.x = -g_SlopeWall[nCntSlopeWall].fWidth;
				}
			}

			//坂道の方向が左右向きの場合
			else
			{
				//最初と最後の頂点だけ原点から見て奥行きがある
				if (nCntVtx % SLOPEWALL_VTX_LAST == ZERO)
				{
					pVtx[nCntVtx].pos.x = g_SlopeWall[nCntSlopeWall].fWidth;
				}
				else
				{
					pVtx[nCntVtx].pos.x = NIL_F;
				}
			}

			//-------------------------
			//		Y座標の設定
			//-------------------------
			//頂点番号が奇数のものだけ高さがある
			if (nCntVtx % EVENPARITY == ODDPARITY)
			{
				pVtx[nCntVtx].pos.y = g_SlopeWall[nCntSlopeWall].fHeight;
			}
			else
			{
				pVtx[nCntVtx].pos.y = NIL_F;
			}

			//-------------------------
			//		Z座標の設定
			//-------------------------
			//坂道の方向が手前向きの場合
			if (g_SlopeWall[nCntSlopeWall].sloperot == SLOPEROT_FRONT_BACK)
			{
				//最初と最後の頂点だけ原点から見て奥行きがある
				if (nCntVtx % SLOPEWALL_VTX_LAST == ZERO)
				{
					pVtx[nCntVtx].pos.z = g_SlopeWall[nCntSlopeWall].fDepth;
				}
				else
				{
					pVtx[nCntVtx].pos.z = NIL_F;
				}
			}

			//坂道の方向が左右向きの場合
			else
			{
				//最初の三角形を形成する頂点とその他の頂点は原点からの幅が逆
				if (nCntVtx <= SLOPEWALL_VTX_START_LOW)
				{
					pVtx[nCntVtx].pos.z = -g_SlopeWall[nCntSlopeWall].fDepth;
				}
				else
				{
					pVtx[nCntVtx].pos.z = +g_SlopeWall[nCntSlopeWall].fDepth;
				}
			}

			//-------------------------
			//		norの設定
			//-------------------------
			pVtx[nCntVtx].nor = SLOPEWALL_NOR;

			//-------------------------
			//	  頂点カラーの設定
			//-------------------------
			pVtx[nCntVtx].col = RGBA_WHITE;

			//-------------------------
			//	テクスチャ座標の設定
			//-------------------------
			pVtx[nCntVtx].tex.x = TexWidth;
			pVtx[nCntVtx].tex.y = nCntVtx / EVENPARITY;

			//頂点番号が奇数の時に幅を足す
			if (nCntVtx % EVENPARITY == NOPARITY)
			{
				TexWidth++;
			}
		}

		pVtx[4].tex = D3DXVECTOR2(2.0f, 1.0f);
		pVtx[5].tex = D3DXVECTOR2(2.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(2.0f, 0.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffSlopeWall->Unlock();
}

//------------------------------------------------
//				坂道の断面の終了処理
//------------------------------------------------
void UninitSlopeWall(void)
{
	//テクスチャの破棄
	if (g_pTextureSlopeWall != NULL)
	{
		g_pTextureSlopeWall->Release();
		g_pTextureSlopeWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSlopeWall != NULL)
	{
		g_pVtxBuffSlopeWall->Release();
		g_pVtxBuffSlopeWall = NULL;
	}
}

//------------------------------------------------
//				坂道の断面の更新処理
//------------------------------------------------
void UpdateSlopeWall(void)
{

}

//------------------------------------------------
//				坂道の断面の描画処理
//------------------------------------------------
void DrawSlopeWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntSlopeWall = START_CNT; nCntSlopeWall < NUM_SLOPEWALL; nCntSlopeWall++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_SlopeWall[nCntSlopeWall].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_SlopeWall[nCntSlopeWall].rot.y, g_SlopeWall[nCntSlopeWall].rot.x, g_SlopeWall[nCntSlopeWall].rot.z);

		D3DXMatrixMultiply(&g_SlopeWall[nCntSlopeWall].mtxWorld, &g_SlopeWall[nCntSlopeWall].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_SlopeWall[nCntSlopeWall].pos.x, g_SlopeWall[nCntSlopeWall].pos.y, g_SlopeWall[nCntSlopeWall].pos.z);

		D3DXMatrixMultiply(&g_SlopeWall[nCntSlopeWall].mtxWorld, &g_SlopeWall[nCntSlopeWall].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_SlopeWall[nCntSlopeWall].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(ZERO, g_pVtxBuffSlopeWall, ZERO, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureSlopeWall);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSlopeWall * SLOPEWALL_VTX_MAX, 5);
	}
}

//坂道の壁の当たり判定処理
bool CollisionSlopeWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld)
{
	bool bCollision = false;	//当たったかどうかの判定
	D3DXVECTOR3 PeakPos;		//頂上の位置
	D3DXVECTOR3 BasePos;		//ふもとの位置（頂点の位置とは対角線上の位置

	//全坂道の壁の当たり判定
	for (int nCntSlopeWall = START_CNT; nCntSlopeWall < NUM_SLOPEWALL; nCntSlopeWall++)
	{
		//坂道の壁の向き別処理判定
		switch (g_SlopeWall[nCntSlopeWall].sloperot)
		{//向きが正面・後ろ向き
		case SLOPEROT_FRONT_BACK:
		{
			//--------------------------------------------------									------------------------------------------------
			//					頂上の位置設定																		ふもとの位置設定
			//--------------------------------------------------									------------------------------------------------
			PeakPos.x = g_SlopeWall[nCntSlopeWall].pos.x - g_SlopeWall[nCntSlopeWall].fWidth;		BasePos.x = g_SlopeWall[nCntSlopeWall].pos.x + g_SlopeWall[nCntSlopeWall].fWidth;
			PeakPos.y = g_SlopeWall[nCntSlopeWall].pos.y + g_SlopeWall[nCntSlopeWall].fHeight;		BasePos.y = g_SlopeWall[nCntSlopeWall].pos.y;
			PeakPos.z = g_SlopeWall[nCntSlopeWall].pos.z;											BasePos.z = g_SlopeWall[nCntSlopeWall].pos.z - g_SlopeWall[nCntSlopeWall].fDepth;

			//頂上のX座標とふもとのX座標の間にプレイヤーがいる　かつ　頂上のZ座標より手前にいる
			if (PeakPos.x <= pPos->x && pPos->x <= BasePos.x && BasePos.z <= pPos->z && pPos->z <= PeakPos.z)
			{
				//頂上のX座標の判定
				if (pPosOld->x <= PeakPos.x)
				{
					pPos->x = PeakPos.x;		//頂点のX座標に戻す
				}

				//ふもとのX座標の判定
				if (BasePos.x <= pPosOld->x)
				{
					pPos->x = BasePos.x;		//ふもとのX座標に戻す
				}

				//坂道の４角形の壁の判定（サイドは３角形）
				if (PeakPos.z <= pPosOld->z)
				{
					pPos->z = PeakPos.z;		//頂点のZ座標に戻す
				}
			}
		}
			break;

		case SLOPEROT_RIGHT_LEFT:	//向きが左右を向いている
		{
			//--------------------------------------------------									------------------------------------------------
			//					頂上の位置設定																		ふもとの位置設定
			//--------------------------------------------------									------------------------------------------------
			PeakPos.x = g_SlopeWall[nCntSlopeWall].pos.x;											BasePos.x = g_SlopeWall[nCntSlopeWall].pos.x + g_SlopeWall[nCntSlopeWall].fWidth;
			PeakPos.y = g_SlopeWall[nCntSlopeWall].pos.y + g_SlopeWall[nCntSlopeWall].fHeight;		BasePos.y = g_SlopeWall[nCntSlopeWall].pos.y;
			PeakPos.z = g_SlopeWall[nCntSlopeWall].pos.z + g_SlopeWall[nCntSlopeWall].fDepth;		BasePos.z = g_SlopeWall[nCntSlopeWall].pos.z - g_SlopeWall[nCntSlopeWall].fDepth;

			//頂上のZ座標とふもとのZ座標の間にプレイヤーがいる　かつ　
			if (PeakPos.z <= pPos->z && pPos->z <= BasePos.z && BasePos.x <= pPos->x && pPos->x <= PeakPos.x)
			{
				//頂上のX座標の判定
				if (pPosOld->x <= PeakPos.x)
				{
					pPos->x = PeakPos.x;		//頂点のX座標に戻す
				}

				//ふもとのX座標の判定
				if (BasePos.x <= pPosOld->x)
				{
					pPos->x = BasePos.x;		//ふもとのX座標に戻す
				}

				//坂道の４角形の壁の判定（サイドは３角形）
				if (PeakPos.x <= pPosOld->z)
				{
					pPos->z = PeakPos.z;		//頂点のZ座標に戻す
				}
			}
		}
			break;
		}
	}

	//判定を返す
	return bCollision;
}

//坂道の壁の情報
SlopeWall *GetSlopeWall(void)
{
	//坂道の情報の先頭アドレスを返す
	return &g_SlopeWall[0];
}

//坂道の壁の情報読み込み処理
void LoadSlopeWall(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(SLOPEWALL_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルタイトル読み込み

		for (int nCntSlopeWall = START_CNT; nCntSlopeWall < NUM_SLOPEWALL; nCntSlopeWall++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			//配置場所の初期化
			g_SlopeWall[nCntSlopeWall].pos.x = atof(strtok(&aData[ZERO], ","));			//位置初期化
			g_SlopeWall[nCntSlopeWall].pos.y = atof(strtok(NULL, ","));					//位置初期化
			g_SlopeWall[nCntSlopeWall].pos.z = atof(strtok(NULL, ","));					//位置初期化

			//配置方向の初期化
			g_SlopeWall[nCntSlopeWall].rot.x = atof(strtok(NULL, ",")) / FIX_FLOAT;		//向き初期化
			g_SlopeWall[nCntSlopeWall].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;		//向き初期化
			g_SlopeWall[nCntSlopeWall].rot.z = atof(strtok(NULL, ",")) / FIX_FLOAT;		//向き初期化

			g_SlopeWall[nCntSlopeWall].fWidth = atof(strtok(NULL, ","));				//幅の初期化
			g_SlopeWall[nCntSlopeWall].fHeight = atof(strtok(NULL, ","));				//高さの初期化
			g_SlopeWall[nCntSlopeWall].fDepth = atof(strtok(NULL, ","));				//奥行きの初期化

			g_SlopeWall[nCntSlopeWall].sloperot = (SLOPEROT)(atoi(strtok(NULL, ",")));	//坂道の方向初期化
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}