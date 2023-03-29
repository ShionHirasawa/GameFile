//==========================================
//
//落下防止フェンスプログラム[fence.cpp]
//Author:石原颯馬  平澤詩苑
//
//==========================================
#include "main.h"
#include "fence.h"
#include "model.h"
#include "camera.h"
#include "meshfield.h"

//マクロ
#define FENCE_MARGIN_SPACE	(10.0f)		//フェンスが倒れる際の後ろの余裕スペース
#define FENCE_DESTROY_POS	(-800.0f)	//フェンスを消す位置
#define FENCE_DROP_SPEED	(1.3f)		//フェンスの落下加速度
#define FENCE_FALL_SPEED	(0.005f)	//フェンスの倒れる加速度
#define FENCE_LIMIT_FALL	(3.1f)		//フェンスの倒れる限界回転値
#define FENCE_SHIFT_LENGTH	(1.0f)		//フェンスが倒れる時の奥にずらす移動量

//グローバル
Fence g_aFence[MAX_USE_FENCE];

//========================
//初期化処理
//========================
void InitFence(void)
{
	//基本の半径の大きさを格納
	float fRadius = FENCE_RADIUS;

	//変数初期化
	for (int nCntfence = 0; nCntfence < MAX_USE_FENCE; nCntfence++)
	{
		//情報初期化
		g_aFence[nCntfence] = {};
		g_aFence[nCntfence].fRadius = fRadius;	//半径設定

		//フェンスを設置
		SetFence(fRadius, FIX_ROT(((float)nCntfence / MAX_USE_FENCE) * 2 * D3DX_PI));
	}
}

//========================
//終了処理
//========================
void UninitFence(void)
{
	
}

//========================
//更新処理
//========================
void UpdateFence(void)
{
	//フェンスのポインタを取得
	Fence *pFence = &g_aFence[0];

	//フィールドの半径取得
	float FieldRadius = GetMeshField()->fRadius;

	for (int nCntFence = 0; nCntFence < MAX_USE_FENCE; nCntFence++, pFence++)
	{
		//対象のフェンスが使われていなかったら、次のフェンスまで処理を飛ばす
		if (pFence->bUse == false)
		{
			continue;
		}

		//フェンスの倒れる処理
		FallFence(pFence, FieldRadius);

		//フェンスの落下処理
		DropFence(pFence, FieldRadius);
	}
}

//フェンスの落下処理
void DropFence(Fence *pFence, float FieldRadius)
{
	//フェンスの位置が、フィールドの半径以下
	if (FieldRadius <= pFence->fRadius)
	{
		pFence->bCollision = true;				//後の当たり判定を行わない
		pFence->fDropSpeed += FENCE_DROP_SPEED;	//落下速度をどんどん上げる
		pFence->pos.y -= pFence->fDropSpeed;	//落下させる

		//消滅ポイントまで落ちた
		if (pFence->pos.y <= FENCE_DESTROY_POS)
		{
			//使用しない
			pFence->bUse = false;
		}
	}
}

//フェンスの倒れる処理
void FallFence(Fence *pFence, float FieldRadius)
{
	//フェンスの当たり判定が残っていたら、処理を飛ばす
	if (pFence->bCollision)
	{
		return;
	}

	pFence->fFallSpeed += FENCE_FALL_SPEED;		//倒れる速度をどんどん上げる
	pFence->rot.x += pFence->fFallSpeed;		//回転値を加算
	
	//フェンスの置かれている位置+余裕を持たせたスペース　より　フィールドの半径がでかい
	if (pFence->fRadius + FENCE_MARGIN_SPACE <= FieldRadius)
	{
		//真横以上にまで倒れた
		if (D3DX_PI * RIGHT < pFence->rot.x)
		{
			pFence->rot.x = D3DX_PI * RIGHT;	//真横に戻す
			pFence->fFallSpeed = 0.0f;			//倒れるスピードを０にする
		}
	}

	//余裕を持たせたスペースよりフィールドの半径が小さかった場合、最大限まで倒れさせる
	//フェンスが逆さまになりかけるくらい、倒れた
	else
	{
		//少しずつ奥にずらす
		pFence->fRadius += FENCE_SHIFT_LENGTH;
		
		//最大限まで倒れた
		if (FENCE_LIMIT_FALL < pFence->rot.x)
		{
			pFence->bUse = false;	//フェンスを使わない
		}

		//位置設定
		SetFencePos(&pFence->pos, pFence->fRadius, pFence->rot.y);
	}
}

//========================
//描画処理
//========================
void DrawFence(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxScall, mtxRot, mtxTrans, mtxTexture;	//計算用
	D3DXMATRIX mtxInvNor;			//法線用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//フェンスモデル取得
	Model fenceModel = GetXObject(OBJECTTYPE_FENCE);

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	for (int nCount = 0; nCount < MAX_USE_FENCE; nCount++)
	{
		if (g_aFence[nCount].bUse == true)
		{
			//ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_aFence[nCount].mtxWorld);

			//拡縮を反映
			D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFence[nCount].rot.y, g_aFence[nCount].rot.x, g_aFence[nCount].rot.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxRot);

			//位置反映
			D3DXMatrixTranslation(&mtxTrans, g_aFence[nCount].pos.x, g_aFence[nCount].pos.y, g_aFence[nCount].pos.z);
			D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aFence[nCount].mtxWorld);

			//テクスチャ拡縮
			D3DXMatrixScaling(&mtxTexture, FENCE_SCALE, FENCE_SCALE, 1.0f);
			pDevice->SetTransform(D3DTS_TEXTURE0, &mtxTexture);

			//マテリアルデータへのポインタ取得
			pMat = (D3DXMATERIAL*)fenceModel.aParts[0].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)fenceModel.aParts[0].dwNumMatModel; nCntMat++)
			{
				//マテリアル設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャ設定
				pDevice->SetTexture(0, fenceModel.aParts[0].apTexture[nCntMat]);

				//モデル描画
				fenceModel.aParts[0].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	
	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//設定処理
//========================
void SetFence(float fRadius, float rotY)
{
	//対象のフェンスのポインタを取得
	Fence *pFence = &g_aFence[0];

	for (int nCntObj = 0; nCntObj < MAX_USE_FENCE; nCntObj++, pFence++)
	{
		//対象のフェンスが使われていない
		if (pFence->bUse == false)
		{
			//引数の情報を追加			
			SetFencePos(&pFence->pos, fRadius, rotY);		//位置設定
			pFence->pos.y = 0.0f;							//初期位置設定
			pFence->posOld = pFence->pos;					//初期位置設定
			pFence->fRadius = fRadius;						//半径設定
			pFence->rot = D3DXVECTOR3(0.0f, rotY, 0.0f);	//角度を設定
			pFence->fDropSpeed = pFence->fFallSpeed = 0.0f;	//落下・倒れる速度を初期化

			//使用していることにする
			pFence->bUse = true;
			pFence->bCollision = true;
			break;			  //抜ける
		}
	}
}

//位置設定
void SetFencePos(D3DXVECTOR3 *pPos, float fRadius, float rotY) 
{
	pPos->x = fRadius * sinf(rotY);	//Ⅹ位置設定
	pPos->z = fRadius * cosf(rotY);	//Ｚ位置設定
}

//========================
//取得処理
//========================
Fence *GetFence(void)
{
	return &g_aFence[0];
}