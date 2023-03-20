/*==========================================================================================================================================================

															壁の処理[wall.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"
#include "wall.h"
#include "camera.h"

//マクロ定義
#define NUM_WALL		(13)							// 壁の数
#define NUM_TEX_WALL	(4)								// テクスチャの枚数
#define WALL_WIDTH		(600.0f)						// 幅サイズ
#define WALL_HEIGHT		(200.0f)						// 高さサイズ
#define WALL_DEPTH		(600.0f)						// 奥行きサイズ
#define TEX_WIDTH		(6.0f)							// テクスチャの横幅を増やす
#define TEX_HEIGHT		(1.0f)							// テクスチャの横幅を増やす
#define WALL_ROT		(0.01f)							// 壁の回転値
#define WALL_NOR		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 壁のnor
#define WALL_FILE		"data/CSV/wall.csv"				// 壁の情報が書かれたファイル

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureWall[NUM_WALL] = {};	//テクスチャ
D3DXMATRIX				g_mtxWorldWall;					//ワールドマトリックス
Wall					g_Wall[NUM_WALL];				//壁の情報

//壁のテクスチャ名
const char *c_apWallTextureName[NUM_TEX_WALL] =
{
	"data/TEXTURE/fault000.png",
	"data/TEXTURE/fault001.png",
	"data/TEXTURE/fault002.png",
	"data/TEXTURE/fault003.png",
};

//------------------------------------------------
//				壁の初期化処理
//------------------------------------------------
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ	

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_WALL, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffWall, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//壁の情報読み込み
	LoadWall();

	//テクスチャ読み込み
	for (int nCntWallTex = START_CNT; nCntWallTex < NUM_TEX_WALL; nCntWallTex++)
	{
		D3DXCreateTextureFromFile(pDevice, c_apWallTextureName[nCntWallTex], &g_pTextureWall[nCntWallTex]);	//テクスチャの読み込み
	}

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++, pVtx += VTX_MAX)
	{
		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth, g_Wall[nCntWall].fHeight,	+g_Wall[nCntWall].fDepth);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth, g_Wall[nCntWall].fHeight,	-g_Wall[nCntWall].fDepth);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth,	NIL_F,						+g_Wall[nCntWall].fDepth);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth,	NIL_F,						-g_Wall[nCntWall].fDepth);
		}

		//norの設定
		{
			pVtx[VTX_LE_UP].nor = WALL_NOR;
			pVtx[VTX_RI_UP].nor = WALL_NOR;
			pVtx[VTX_LE_DO].nor = WALL_NOR;
			pVtx[VTX_RI_DO].nor = WALL_NOR;
		}

		//頂点カラーの設定
		{
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(g_Wall[nCntWall].fTexPos.x,	g_Wall[nCntWall].fTexPos.y);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(g_Wall[nCntWall].fTexWidth,	g_Wall[nCntWall].fTexPos.y);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(g_Wall[nCntWall].fTexPos.x,	g_Wall[nCntWall].fTexHeight);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(g_Wall[nCntWall].fTexWidth,	g_Wall[nCntWall].fTexHeight);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffWall->Unlock();
}

//------------------------------------------------
//				壁の終了処理
//------------------------------------------------
void UninitWall(void)
{
	//テクスチャの破棄
	for (int nCntWall = START_CNT; nCntWall < NUM_TEX_WALL; nCntWall++)
	{
		if (g_pTextureWall[nCntWall] != NULL)
		{
			g_pTextureWall[nCntWall]->Release();
			g_pTextureWall[nCntWall] = NULL;
		}
	}	

	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//------------------------------------------------
//				壁の更新処理
//------------------------------------------------
void UpdateWall(void)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点バッファをロックする
	g_pVtxBuffWall->Unlock();
}

//------------------------------------------------
//				壁の描画処理
//------------------------------------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldWall);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(ZERO, g_pVtxBuffWall, ZERO, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureWall[g_Wall[nCntWall].nTexType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * VTX_MAX, SEC);
	}
}

//壁の当たり判定処理
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bCollision = false;		//当たったかどうかの判定
	bool bInnerProduct = false;		//内積を行うかどうか
	float fAreaS1, fAreaS2;			//頂点座標2つ・前回・現在位置、の４点で形成される四角形を分断した時の左右の面積を格納
	D3DXVECTOR3 vtx0, vtx1;			//頂点座標０・１番目の位置
	D3DXVECTOR3 vecMove = *pMove;	//移動ベクトル
	Wall *pWall = &g_Wall[0];		//壁のポインタ

	//ポインタを設定	//頂点バッファをロックし、頂点情報へのポインタを取得
	VERTEX_3D *pVtx;	g_pVtxBuffWall->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++, pVtx += VTX_MAX, pWall++)
	{
		vtx0 = pVtx[VTX_LE_UP].pos + pWall->pos;	//頂点座標０番目の位置
		vtx1 = pVtx[VTX_RI_UP].pos + pWall->pos;	//頂点座標１番目の位置
		D3DXVECTOR3 vPosTovtx0 = vtx0 - *pPos;		//現在位置から頂点座標０番目までのベクトル
		D3DXVECTOR3 vVtx1ToPos = *pPos - vtx1;		//頂点座標１番目から現在位置までのベクトル
		D3DXVECTOR3 vecLine = vtx1 - vtx0;			//頂点座標０から１番目までのベクトル

		VectorProduct(vPosTovtx0.x, vecMove.z, vPosTovtx0.z, vecMove.x, &fAreaS1, HALF_MAG);	//左側の面積
		VectorProduct(vVtx1ToPos.x, vecMove.z, vVtx1ToPos.z, vecMove.x, &fAreaS2, HALF_MAG);	//右側の面積

		float fRateLeft = fAreaS1 / (fAreaS1 + fAreaS2);	//左側の面積の割合
		float fRateRight = fAreaS2 / (fAreaS1 + fAreaS2);	//右側の面積の割合

		D3DXVECTOR3 CrossPoint = D3DXVECTOR3(
			vtx0.x + (vecLine.x * fRateLeft),	//交点Ⅹ座標を算出
			0.0f,								//Y座標（後ほど座標を代入
			vtx0.z + (vecLine.z * fRateLeft));	//交点Ｚ座標を算出

		float vecMoveLength = D3DXVec3Length(&vecMove);				//前回と今回の位置の距離

		float posOldToCrossLength = sqrtf(powf(pPosOld->x - CrossPoint.x, 2.0f) + powf(pPosOld->z - CrossPoint.z, 2.0f));	//前回位置と交点の距離

		float fPercentLength = posOldToCrossLength / vecMoveLength;		//カメラから交点までの距離の割合を算出
		CrossPoint.y = pPosOld->y - vecMove.y * fPercentLength;			//交点の高さを代入

		if (posOldToCrossLength <= vecMoveLength &&					//交点までの長さが、現在位置までの長さ以下
			pWall->pos.y <= CrossPoint.y && CrossPoint.y < vtx0.y)	//交点Ｙ座標が壁の中にある
		{
			float	fvecLineWidth = powf(vtx0.x - vtx1.x, 2.0f),			//境界線ベクトルの幅
					fvecLineDepth = powf(vtx0.z - vtx1.z, 2.0f),			//境界線ベクトルの奥ゆき
					fvecLineLength = sqrtf(fvecLineWidth + fvecLineDepth);	//境界線ベクトルの長さ

			float	CrossTovtx0_X = powf(vtx0.x - CrossPoint.x, 2.0f),		//頂点座標０番目から交点までのⅩの距離
					CrossTovtx0_Z = powf(vtx0.z - CrossPoint.z, 2.0f),		//				〃			のＺの距離
					fCrossS1Length = sqrtf(CrossTovtx0_X + CrossTovtx0_Z);	//				〃			の距離

			float	CrossTovtx1_X = powf(vtx1.x - CrossPoint.x, 2.0f),		//頂点座標１番目から交点までのⅩの距離
					CrossTovtx1_Z = powf(vtx1.z - CrossPoint.z, 2.0f),		//				〃			のＺの距離
					fCrossS2Length = sqrtf(CrossTovtx1_X + CrossTovtx1_Z);	//				〃			の距離

			if (fCrossS1Length <= fvecLineLength &&	//交点から頂点座標０番目までの距離が、境界線ベクトルの距離内に収まっていて
				fCrossS2Length <= fvecLineLength)	//交点から頂点座標１番目までの距離も、境界線ベクトルの距離内に収まっている
			{
				float	vtx0to1_Diff_X = vtx0.x - vtx1.x, vtx0toNow_Diff_X = vtx0.x - pPos->x, vtx0toOld_Diff_X = vtx0.x - pPosOld->x,
						vtx0to1_Diff_Z = vtx0.z - vtx1.z, vtx0toNow_Diff_Z = vtx0.z - pPos->z, vtx0toOld_Diff_Z = vtx0.z - pPosOld->z;

				float fVtx0to1_Rot = atan2f(vtx0to1_Diff_X, vtx0to1_Diff_Z);				//頂点番号０から１番目への角度
				float fVtx0toNowPos_Rot = atan2f(vtx0toNow_Diff_X, vtx0toNow_Diff_Z);		//頂点番号０から現在位置への角度
				float fVtx0toPosOld_Rot = atan2f(vtx0toOld_Diff_X, vtx0toOld_Diff_Z);		//頂点番号０から前回位置への角度

				//角度の絶対値が 3.14 を超えていた場合
				if (0.0f >= D3DX_PI - fabsf(fVtx0to1_Rot))
				{
					fVtx0toNowPos_Rot = atan2f(-vtx0toNow_Diff_X, vtx0toNow_Diff_Z);	//頂点番号０から現在位置への角度を再算出
					fVtx0toPosOld_Rot = atan2f(-vtx0toOld_Diff_X, vtx0toOld_Diff_Z);	//頂点番号０から前回位置への角度を再算出

					if (fVtx0toNowPos_Rot <= 0.0f &&
						fVtx0to1_Rot >= fVtx0toPosOld_Rot)
					{
						*pPos = CrossPoint;
						*pMove = ZERO_SET;
						//bCollision = true;		//当たった
						bInnerProduct = true;		//内積を行う
					}
				}

				//前回が  頂点座標０番目から頂点座標１番目を見た時に右側にいる
				else if (fVtx0toNowPos_Rot <= fVtx0to1_Rot && 
						fVtx0to1_Rot <= fVtx0toPosOld_Rot)
				{
					*pPos = CrossPoint;
					//*pMove = ZERO_SET;
					//bCollision = true;		//当たった
					bInnerProduct = true;		//内積を行う
				}

				//内積を行う場合
				if (bInnerProduct)
				{	
					//壁の角度を９０度右回転
					fVtx0to1_Rot += RIGHT;

					//内積計算
					InnerProduct(fVtx0to1_Rot, vecMove, pMove);
				}
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffWall->Unlock();

	//判定を返す
	return bCollision;
}

//壁の情報
Wall *GetWall(void)
{
	//壁の情報の先頭アドレスを返す
	return &g_Wall[ZERO];
}

//壁の情報読み込み
void LoadWall(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(WALL_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルタイトル読み込み

		for (int nCntWall = START_CNT; nCntWall < NUM_WALL; nCntWall++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			g_Wall[nCntWall].pos.x = atof(strtok(&aData[ZERO], ","));				// 位置初期化
			g_Wall[nCntWall].pos.y = atof(strtok(NULL, ","));						// 位置初期化
			g_Wall[nCntWall].pos.z = atof(strtok(NULL, ","));						// 位置初期化

			g_Wall[nCntWall].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;			// 向き初期化
			g_Wall[nCntWall].rot.x = NIL_F;											// 向き初期化
			g_Wall[nCntWall].rot.z = NIL_F;											// 向き初期化

			g_Wall[nCntWall].fWidth = atof(strtok(NULL, ","));						// 幅の初期化
			g_Wall[nCntWall].fHeight = atof(strtok(NULL, ","));						// 高さの初期化
			g_Wall[nCntWall].fDepth = atof(strtok(NULL, ","));						// 奥行きの初期化

			g_Wall[nCntWall].fTexPos.x = atof(strtok(NULL, ","));					// テクスチャの位置初期化
			g_Wall[nCntWall].fTexPos.y = atof(strtok(NULL, ","));					// テクスチャの位置初期化
			g_Wall[nCntWall].fTexWidth = atof(strtok(NULL, ","));					// テクスチャの幅初期化
			g_Wall[nCntWall].fTexHeight = atof(strtok(NULL, ","));					// テクスチャの高さ初期化
			g_Wall[nCntWall].nTexType = atoi(strtok(NULL, ","));					// テクスチャの種類

			g_Wall[nCntWall].bCollitype = (bool)(atoi(strtok(NULL, ",")));			// 当たり判定を行うかどうか[ 0:行わない ・ 1:行う ]
			g_Wall[nCntWall].Direction = (WallDirection)(atoi(strtok(NULL, ",")));	// 壁の方向
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//壁越しにオブジェクトを描画するかどうか
bool WallDrawObject(D3DXVECTOR3 pos)
{
	bool bDisp = true;						//描画するかどうか
	float fAreaS1, fAreaS2;					//頂点座標2つ・カメラの位置・オブジェクトの位置、の４点で形成される四角形を分断した時の左右の面積を格納
	D3DXVECTOR3 CamPos = GetCamera()->posV;//カメラの位置
	D3DXVECTOR3 vCamToObj = CamPos - pos;	//いわゆる移動ベクトル（移動はしていないが
	Wall *pWall = &g_Wall[0];				//最初の壁のポインタを取得

	//ポインタを設定		//頂点バッファをロックし、頂点情報へのポインタを取得
	VERTEX_3D *pVtx;		g_pVtxBuffWall->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < NUM_WALL; nCntWall++, pVtx += VTX_MAX)
	{
		D3DXVECTOR3 vtx0 = pVtx[VTX_LE_UP].pos + pWall->pos;	//０番目の頂点座標
		D3DXVECTOR3 vtx1 = pVtx[VTX_RI_UP].pos + pWall->pos;	//１番目の頂点座標

		//オブジェクトの位置が、壁の底から天井までの高さにいる
		D3DXVECTOR3 vCamTovtx0 = vtx0 - pos;	//オブジェクトの位置から頂点座標０番目までのベクトル
		D3DXVECTOR3 vVtx1ToCam = pos - vtx1;	//頂点座標１番目からオブジェクトの位置までのベクトル
		D3DXVECTOR3 vecLine = vtx1 - vtx0;		//頂点座標０から１番目までのベクトル

		VectorProduct(vCamTovtx0.x, vCamToObj.z, vCamTovtx0.z, vCamToObj.x, &fAreaS1, HALF_MAG);	//左側の面積
		VectorProduct(vVtx1ToCam.x, vCamToObj.z, vVtx1ToCam.z, vCamToObj.x, &fAreaS2, HALF_MAG);	//右側の面積

		float fLeftAreaPer = fAreaS1 / (fAreaS1 + fAreaS2);		//左側の面積の割合

		D3DXVECTOR3 CrossPoint = D3DXVECTOR3(
			vtx0.x + (vecLine.x * fLeftAreaPer),	//交点Ⅹ座標を算出
			0.0f,									//Y座標
			vtx0.z + (vecLine.z * fLeftAreaPer));	//交点Ｚ座標を算出

		float CamToObjectLength = sqrtf(powf(CamPos.x - pos.x, 2.0f) + powf(CamPos.z - pos.z, 2.0f));				//カメラとオブジェクトの距離
		float CamToCrossLength = sqrtf(powf(CamPos.x - CrossPoint.x, 2.0f) + powf(CamPos.z - CrossPoint.z, 2.0f));	//カメラと交点の距離

		float fPercentLength = CamToCrossLength / CamToObjectLength;		//カメラから交点までの距離の割合を算出
		float fCrossHeight = CamPos.y - vCamToObj.y * fPercentLength;		//交点の高さを算出

		if (CamToCrossLength <= CamToObjectLength &&				//カメラから交点までの距離が、カメラからオブジェクトの距離の中にある
			pWall->pos.y <= fCrossHeight && fCrossHeight <= vtx0.y)	//交点のＹ座標が、壁の中にある
		{
			float fvecLineWidth = powf(vtx0.x - vtx1.x, 2.0f);				//境界線ベクトルの幅
			float fvecLineDepth = powf(vtx0.z - vtx1.z, 2.0f);				//境界線ベクトルの奥ゆき
			float fvecLineLength = sqrtf(fvecLineWidth + fvecLineDepth);	//境界線ベクトルの長さ

			float CrossTovtx0_X = powf(vtx0.x - CrossPoint.x, 2.0f);		//頂点座標０番目から交点までのⅩの距離
			float CrossTovtx0_Z = powf(vtx0.z - CrossPoint.z, 2.0f);		//				〃			のＺの距離
			float fCrossS1Length = sqrtf(CrossTovtx0_X + CrossTovtx0_Z);	//				〃			の距離

			float CrossTovtx1_X = powf(vtx1.x - CrossPoint.x, 2.0f);		//頂点座標１番目から交点までのⅩの距離
			float CrossTovtx1_Z = powf(vtx1.z - CrossPoint.z, 2.0f);		//				〃			のＺの距離
			float fCrossS2Length = sqrtf(CrossTovtx1_X + CrossTovtx1_Z);	//				〃			の距離

			if (fCrossS1Length <= fvecLineLength &&	//交点から頂点座標０番目までの距離が、境界線ベクトルの距離内に収まっていて
				fCrossS2Length <= fvecLineLength)	//交点から頂点座標１番目までの距離も、境界線ベクトルの距離内に収まっている
			{
				//描画しない
				bDisp = false;
				break;
			}
		}

		//次の壁へポインタを進める
		pWall++;
	}

	//頂点バッファをロックする
	g_pVtxBuffWall->Unlock();

	//判定を返す
	return bDisp;
}