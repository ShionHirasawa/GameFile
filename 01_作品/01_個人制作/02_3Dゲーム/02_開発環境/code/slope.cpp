/*==========================================================================================================================================================

															坂道の処理[slope.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"
#include "slope.h"

//マクロ定義
#define NUM_SLOPE				(2)								// 坂道の数
#define SLOPE_NOR				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// 坂道のnor
#define SLOPE_FILE				"data/CSV/slope.csv"			// 坂道の情報が入ったファイル名
#define SLOPE_CORRECT_SIZE		(1.0f)							// 坂道のサイズの調整用
#define GO_UP_a_SLOPE_MAG		(0.9f)							// 坂道を上るときの移動速度の倍率
#define GO_DOWN_a_SLOPE_MAG		(0.4f)							// 坂道を下るときの移動速度の倍率
#define SLOPE_FRONT_BACK_ROT	(0.25f)							// 前後の坂道を通るときのプレイヤーの角度
#define SLOPE_RIGHT_LEFT_ROT	(0.50f)							// 左右坂道を通るときのプレイヤーの角度

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSlope = NULL;		//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureSlope = NULL;		//テクスチャ
D3DXMATRIX				g_mtxWorldSlope;			//ワールドマトリックス
Slope					g_Slope[NUM_SLOPE];			//坂道の情報


//------------------------------------------------
//				坂道の初期化処理
//------------------------------------------------
void InitSlope(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ	

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_SLOPE, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffSlope, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSlope->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	/*-------------------------------
			坂道の情報読み込み
	---------------------------------*/
	LoadSlope();

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field.png", &g_pTextureSlope);	//テクスチャの読み込み

	//各種情報・頂点座標の初期化
	for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++, pVtx += VTX_MAX)
	{
		//方向による頂点座標の分岐処理
		switch (g_Slope[nCntSlope].sloperot)
		{
		case SLOPEROT_FRONT_BACK:
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Slope[nCntSlope].fWidth, g_Slope[nCntSlope].fHeight, +g_Slope[nCntSlope].fDepth);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Slope[nCntSlope].fWidth, g_Slope[nCntSlope].fHeight, +g_Slope[nCntSlope].fDepth);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Slope[nCntSlope].fWidth, NIL_F, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Slope[nCntSlope].fWidth, NIL_F, NIL_F);
			break;
			
		case SLOPEROT_RIGHT_LEFT:
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(+g_Slope[nCntSlope].fWidth, +g_Slope[nCntSlope].fHeight, +g_Slope[nCntSlope].fDepth);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Slope[nCntSlope].fWidth, +g_Slope[nCntSlope].fHeight, -g_Slope[nCntSlope].fDepth);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(NIL_F, NIL_F, +g_Slope[nCntSlope].fDepth);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(NIL_F, NIL_F, -g_Slope[nCntSlope].fDepth);
			break;
		}

		//norの設定
		{
			pVtx[VTX_LE_UP].nor = SLOPE_NOR;
			pVtx[VTX_RI_UP].nor = SLOPE_NOR;
			pVtx[VTX_LE_DO].nor = SLOPE_NOR;
			pVtx[VTX_RI_DO].nor = SLOPE_NOR;
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
			pVtx[VTX_LE_UP].tex = ORIGIN_TEX;
			pVtx[VTX_RI_UP].tex = MA_ZE_TEX;
			pVtx[VTX_LE_DO].tex = ZE_MA_TEX;
			pVtx[VTX_RI_DO].tex = LIMIT_TEX;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffSlope->Unlock();
}

//------------------------------------------------
//				坂道の終了処理
//------------------------------------------------
void UninitSlope(void)
{
	//テクスチャの破棄
	if (g_pTextureSlope != NULL)
	{
		g_pTextureSlope->Release();
		g_pTextureSlope = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSlope != NULL)
	{
		g_pVtxBuffSlope->Release();
		g_pVtxBuffSlope = NULL;
	}
}

//------------------------------------------------
//				坂道の更新処理
//------------------------------------------------
void UpdateSlope(void)
{

}

//------------------------------------------------
//				坂道の描画処理
//------------------------------------------------
void DrawSlope(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldSlope);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Slope[nCntSlope].rot.y, g_Slope[nCntSlope].rot.x, g_Slope[nCntSlope].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldSlope, &g_mtxWorldSlope, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Slope[nCntSlope].pos.x, g_Slope[nCntSlope].pos.y, g_Slope[nCntSlope].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldSlope, &g_mtxWorldSlope, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSlope);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(ZERO, g_pVtxBuffSlope, ZERO, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureSlope);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSlope * VTX_MAX, SEC);
	}
}

//坂道の当たり判定処理
bool CollisionSlope(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, COLLITYPE nType)
{
	bool bLand = false;				//乗ったかどうか判定
	bool bCollison = false;			//坂道と当たったかどうか（弾限定）
	float fOldPercent = F_ZERO;		//ふもとから前回の距離の割合
	float fPercent = F_ZERO;		//ふもとから今回の距離の割合
	float FixHeight = F_ZERO;		//今回修正する高さを格納

	float fSlopeRot = F_ZERO;		//坂道の角度
	float FixStandardRot = F_ZERO;	//修正する角度の基準
	float MinDownRot = F_ZERO;		//下ってる判定をとる最小角度
	float MaxDownRot = F_ZERO;		//下ってる判定をとる最大角度

	D3DXVECTOR3 BasePos;			//ふもとの座標
	D3DXVECTOR3 PeakPos;			//頂上の座標

	for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++)
	{
		fOldPercent = F_ZERO;				//ふもとから前回の距離の割合
		fPercent = F_ZERO;					//ふもとから今回の距離の割合
		FixHeight = F_ZERO;					//今回修正する高さを格納

		BasePos = g_Slope[nCntSlope].pos;	//ふもとの座標
		PeakPos = g_Slope[nCntSlope].pos;	//頂上の座標

		//処理分岐
		switch (g_Slope[nCntSlope].sloperot)
		{
		case SLOPEROT_FRONT_BACK:	//前後に伸びる坂
		{
			//ふもとの位置を設定
			BasePos.x += g_Slope[nCntSlope].fWidth;		//ふもとのX座標の位置を設定

			//頂上の位置を設定
			PeakPos.x -= g_Slope[nCntSlope].fWidth;		//頂上のX座標の位置を設定
			PeakPos.y += g_Slope[nCntSlope].fHeight;	//頂上のY座標の位置を設定
			PeakPos.z += g_Slope[nCntSlope].fDepth;		//頂上のZ座標の位置を設定

			//頂点とふもとの間にいる
			if (PeakPos.x <= pPosOld->x && BasePos.x >= pPos->x &&
				PeakPos.z >= pPosOld->z && BasePos.z <= pPos->z &&
				PeakPos.y >= pPosOld->y && BasePos.y <= pPos->y)
			{
				//前回の位置割合算出
				fOldPercent = fabsf(pPosOld->z - BasePos.z) / g_Slope[nCntSlope].fDepth;

				//今回の位置割合算出
				fPercent = fabsf(pPos->z - BasePos.z) / g_Slope[nCntSlope].fDepth;

				//坂道の角度を代入
				fSlopeRot = SLOPE_FRONT_BACK_ROT;

				//修正する角度の基準値を設定
				FixStandardRot = FIX_ROT;

				MinDownRot = LEFT;		//下っている判定をとる最小値を設定
				MaxDownRot = RIGHT;		//下っている判定をとる最大値を設定
			}

			//居なければループ分を一回分飛ばす
			else
			{
				continue;
			}
		}
		break;

		case SLOPEROT_RIGHT_LEFT:	//左右に広がる坂
		{
			//ふもとの位置を設定
			BasePos.z += g_Slope[nCntSlope].fDepth;		//ふもとのZ座標の位置を設定

			//頂上の位置を設定
			PeakPos.x += g_Slope[nCntSlope].fWidth;		//頂上のX座標の位置を設定
			PeakPos.y += g_Slope[nCntSlope].fHeight;	//頂上のY座標の位置を設定
			PeakPos.z -= g_Slope[nCntSlope].fDepth;		//頂上のZ座標の位置を設定

			//頂点とふもとの間にいる
			if (PeakPos.x >= pPosOld->x && BasePos.x <= pPos->x &&
				PeakPos.z <= pPosOld->z && BasePos.z >= pPos->z &&
				PeakPos.y >= pPosOld->y && BasePos.y <= pPos->y)
			{
				//前回の位置割合算出
				fOldPercent = fabsf(pPosOld->x - BasePos.x) / g_Slope[nCntSlope].fWidth;

				//今回の位置割合算出
				fPercent = fabsf(pPos->x - BasePos.x) / g_Slope[nCntSlope].fWidth;

				//坂道の角度を代入
				fSlopeRot = SLOPE_RIGHT_LEFT_ROT;

				//修正する角度の基準値を設定
				FixStandardRot = D3DX_PI;

				MinDownRot = -D3DX_PI;		//下っている判定をとる最小値を設定
				MaxDownRot = F_ZERO;		//下っている判定をとる最大値を設定
			}

			//居なければループ分を一回分飛ばす
			else
			{
				continue;
			}
		}
		break;
		}

		//割合を基に高さを算出
		FixHeight = BasePos.y + (g_Slope[nCntSlope].fHeight * fPercent);

		//当たったものによって判定を変える
		switch (nType)
		{
		case COLLITYPE_BULLET:		/*弾の場合*/
		{
			//対象の位置が坂道の高さ以下
			if (pPos->y <= FixHeight)
			{
				return bCollison = true;
			}
		}
			break;

		case COLLITYPE_PLAYER:		/*プレイヤーの場合*/
		{
			//前回と今回のふもとからの距離の割合が違う （移動している）
			if (fOldPercent != fPercent)
			{
				//上っている
				if (fOldPercent < fPercent)
				{
					//上っているので移動量を減少
					*pMove *= GO_UP_a_SLOPE_MAG;
				}

				//下っている == 移動キーが押されている
				else if ((GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D)) == true)
				{
					//下っているので移動量の一部をつかって位置を更新
					pPos->x += pMove->x * GO_DOWN_a_SLOPE_MAG;
					pPos->z += pMove->z * GO_DOWN_a_SLOPE_MAG;
					pPos->y = FixHeight;						//Y座標は割合から求めた位置に代入
				}
			}

			//対象の位置が坂道の高さ以下
			if (pPos->y <= FixHeight)
			{
				//位置を修正する
				pPos->y = FixHeight;

				//Y軸の移動量をゼロにする
				pMove->y = F_ZERO;

				//向きを変える倍率
				float fAngle = 0.3f;

				//処理分岐
				switch (g_Slope[nCntSlope].sloperot)
				{
				case SLOPEROT_FRONT_BACK:	//前後に伸びる坂
				{
					//角度的に下っている
					if (MinDownRot <= pRot->y && pRot->y <= MaxDownRot)
					{
						//向きを変える
						pRot->x += (-(fSlopeRot - fabsf(pRot->y) * 0.25f) - pRot->x) * fAngle;
					}

					//上っている
					else
					{
						//Yの角度の割合で向きを変える
						pRot->x += ((fSlopeRot * (fabsf(pRot->y) / D3DX_PI)) - pRot->x) * fAngle;
					}

					//Xの方向を計算
					pRot->x = (fabsf(pRot->y) / FixStandardRot) - fSlopeRot;

					//Yの角度がマイナス方向の場合
					if (pRot->y <= ZERO)
					{
						pRot->z = fSlopeRot - fabsf(pRot->x);
					}

					//Yの角度がプラス方向の場合
					else
					{
						pRot->z = -fSlopeRot + fabsf(pRot->x);
					}
				}
				break;

				case SLOPEROT_RIGHT_LEFT:	//左右に広がる坂
				{
					D3DXVECTOR3 Rot = *pRot;	//角度を格納

					//Xの方向を計算
					//pRot->z = (fabsf(Rot.y) / FixStandardRot) + fSlopeRot;

					/*----------------------------------------------------------------------------------
												角度がきれいな境目と同じ
					------------------------------------------------------------------------------------*/
					//角度の絶対値が3.14
					if (fabsf(Rot.y) == UP)
					{
						pRot->x = NIL_F;
						pRot->z = fSlopeRot;
					}

					//角度の絶対値が1.57
					else if (fabsf(Rot.y) == RIGHT)
					{
						pRot->x = fSlopeRot * (RIGHT / Rot.y);
						pRot->z = NIL_F;
					}

					//角度が0.0
					else if (Rot.y == NIL_F)
					{
						pRot->x = NIL_F;
						pRot->z = -fSlopeRot;
					}

					/*----------------------------------------------------------------------------------
												角度がきれいな境目とは違う
					------------------------------------------------------------------------------------*/
					else
					{
						//角度が原点向きより大きい
						if (Rot.y < NIL_F)
						{
							pRot->z = sinf(fabsf(RIGHT - fabsf(Rot.y)) / D3DX_PI);
						}
						else
						{
							pRot->z = sinf(fabsf(RIGHT - Rot.y) / -D3DX_PI);
						}

						//絶対値が1.57f以上
						if (fabsf(Rot.y) > RIGHT)
						{
							//pRot->z = (RIGHT - fabsf(Rot.y)) + fSlopeRot;
						}
						else
						{
							//pRot->x = (RIGHT - fabsf(pRot->y)) + fSlopeRot;
						}
					}
				}
				break;
				}

				//判定を返す
				return bLand = true;
			}

			//修正位置より上に居たら角度を徐々に戻す
			else
			{
				pRot->x += (F_ZERO - pRot->x) * F_ONE;
				pRot->z += (F_ZERO - pRot->z) * F_ONE;
			}
		}

		case COLLITYPE_ENEMY:		/*敵の場合*/
		{
			//前回と今回のふもとからの距離の割合が違う （移動している）
			//上っている
			if (fOldPercent < fPercent)
			{
				//上っているので移動量を減少
				*pMove *= GO_UP_a_SLOPE_MAG;
			}

			//下っている
			else
			{
				//下っているので移動量の一部をつかって位置を更新（通常の位置更新に加えて位置を更新するので、速く移動する
				pPos->x += pMove->x * GO_DOWN_a_SLOPE_MAG;
				pPos->z += pMove->z * GO_DOWN_a_SLOPE_MAG;
				pPos->y = FixHeight;						//Y座標は割合から求めた位置に代入
			}
		}
			break;
		}

		//ループ分を抜ける
		break;
	}

	//プレイヤーがこの関数を呼び出して、坂道の当たり判定が通らなかった場合
	if (nType == COLLITYPE_PLAYER)
	{
		//角度を徐々に戻す
		pRot->x += (F_ZERO - pRot->x) * F_ONE;
		pRot->z += (F_ZERO - pRot->z) * F_ONE;
	}

	//判定を返す
	return bLand;
}

//坂道に影を描画する
void ColliSlopeShadow(D3DXVECTOR3 pos, D3DXVECTOR3 *pRot, float *pDrawPosY, float *pHeight)
{
	for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++)
	{
		float FixHeight = F_ZERO;	//修正する位置
		float fPercent = F_ZERO;	//割合
		float fHeight = F_ZERO;		//かげに高さを付ける
		float fRot = F_ZERO;		//修正する向き

		D3DXVECTOR3 BasePos = g_Slope[nCntSlope].pos;			//ふもとの座標

		//坂道の方向による処理分岐
		switch (g_Slope[nCntSlope].sloperot)
		{
		case SLOPEROT_FRONT_BACK:	//前後方向
			if (g_Slope[nCntSlope].pos.x - g_Slope[nCntSlope].fWidth <= pos.x &&	//左端より右にいる
				g_Slope[nCntSlope].pos.x + g_Slope[nCntSlope].fWidth >= pos.x &&	//右端より左にいる
				g_Slope[nCntSlope].pos.z + g_Slope[nCntSlope].fDepth >= pos.z &&	//手前より奥にいる
				g_Slope[nCntSlope].pos.z <= pos.z)
			{
				fRot = F_ZERO;		//向きを設定

				//今回の位置割合算出
				fPercent = fabsf(pos.z - BasePos.z) / g_Slope[nCntSlope].fDepth;

				//影の傾きの高さは、高さと奥行きの割合　×10 で割り出す
				fHeight = (g_Slope[nCntSlope].fHeight / g_Slope[nCntSlope].fDepth) * 10.0f;
			}

			//居なければループ分を一回分飛ばす
			else
			{
				continue;
			}

			break;

		case SLOPEROT_RIGHT_LEFT:	//左右方向
			if (g_Slope[nCntSlope].pos.x <= pos.x &&	//左端より右にいる
				g_Slope[nCntSlope].pos.x + g_Slope[nCntSlope].fWidth >= pos.x &&	//右端より左にいる
				g_Slope[nCntSlope].pos.z + g_Slope[nCntSlope].fDepth >= pos.z &&	//手前より奥にいる
				g_Slope[nCntSlope].pos.z - g_Slope[nCntSlope].fDepth <= pos.z)		//奥より手前にいる
			{
				fRot = RIGHT;		//向きを設定
				
				//今回の位置割合算出
				fPercent = fabsf(pos.x - BasePos.x) / g_Slope[nCntSlope].fWidth;

				//影の傾きの高さは、高さと幅の割合　×10 で割り出す
				fHeight = (g_Slope[nCntSlope].fHeight / g_Slope[nCntSlope].fWidth) * 10.0f;
			}

			//居なければループ分を一回分飛ばす
			else
			{
				continue;
			}

			break;
		}

		//割合を基に高さを算出
		FixHeight = BasePos.y + (g_Slope[nCntSlope].fHeight * fPercent);

		//修正する位置が影の位置より低い　かつ　処理を行う
		if (*pDrawPosY <= FixHeight)
		{
			pRot->y = fRot;			//角度を付ける
			*pHeight = fHeight;		//高さを付けて傾ける
			*pDrawPosY = FixHeight;	//そのまま影の位置に描画する
		}

		//処理を終える
		break;
	}
}

//坂道の情報
Slope *GetSlope(void)
{
	//坂道の情報の先頭アドレスを返す
	return &g_Slope[0];
}

//坂道の情報読み込み処理
void LoadSlope(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(SLOPE_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルに書いた項目名読み込み

		for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			//配置場所の初期化
			g_Slope[nCntSlope].pos.x = atof(strtok(&aData[ZERO], ","));			//位置初期化
			g_Slope[nCntSlope].pos.y = atof(strtok(NULL, ","));					//位置初期化
			g_Slope[nCntSlope].pos.z = atof(strtok(NULL, ","));					//位置初期化

			//配置方向の初期化
			g_Slope[nCntSlope].rot.x = atof(strtok(NULL, ",")) / FIX_FLOAT;		//向き初期化
			g_Slope[nCntSlope].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;		//向き初期化
			g_Slope[nCntSlope].rot.z = atof(strtok(NULL, ",")) / FIX_FLOAT;		//向き初期化

			g_Slope[nCntSlope].fWidth = atof(strtok(NULL, ","));				//幅の初期化
			g_Slope[nCntSlope].fHeight = atof(strtok(NULL, ","));				//高さの初期化
			g_Slope[nCntSlope].fDepth = atof(strtok(NULL, ","));				//奥行きの初期化

			g_Slope[nCntSlope].sloperot = (SLOPEROT)(atoi(strtok(NULL, ",")));	//坂道の方向初期化
		}
	
		//ファイルを閉じる
		fclose(pFile);
	}
}