//==========================================
//
//何も効果を持たないオブジェクトのプログラム[object.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "object.h"
#include "model.h"
#include "camera.h"
#include "meshfield.h"

//グローバル
Object g_aObject[MAX_OBJECT];

//========================
//初期化処理
//========================
void InitObject(void)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		g_aObject[nCntObj] = {};
	}
}

//========================
//終了処理
//========================
void UninitObject(void)
{
	
}

//========================
//更新処理
//========================
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_aObject[nCntObject].bUse == true)
		{
			g_aObject[nCntObject].nFallTime++;
			//落とす
			g_aObject[nCntObject].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aObject[nCntObject].move.y = -(ACCELERATION_GRAVITY * g_aObject[nCntObject].nFallTime / MAX_FPS);

			//移動後がy<0であり、現在の位置が、フィールド以上の高さにいるなら移動量消す
			if (g_aObject[nCntObject].pos.y + g_aObject[nCntObject].move.y < 0.0f && g_aObject[nCntObject].pos.y >= 0.0f)
			{
				//原点位置からのプレイヤーの距離を計算
				float fLength = sqrtf(powf((g_aObject[nCntObject].pos.x + g_aObject[nCntObject].move.x), 2) +
					powf((g_aObject[nCntObject].pos.z + g_aObject[nCntObject].move.z), 2));

				//原点位置からの距離が、フィールドの半径以下　　なら、フィールドに乗っている
				if (fLength <= GetMeshField()->fRadius)
				{
					g_aObject[nCntObject].move.y = 0.0f;
					g_aObject[nCntObject].nFallTime = 0;
					g_aObject[nCntObject].pos.y = 0.0f;
				}
			}

			//代入
			g_aObject[nCntObject].pos.y += g_aObject[nCntObject].move.y;
		}
	}
}

//========================
//描画処理
//========================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;	//計算用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	for (int nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (g_aObject[nCount].bUse == true)
		{
			//モデル取得
			Model objectModel = GetXObject(g_aObject[nCount].type);

			//ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_aObject[nCount].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObject[nCount].rot.y, g_aObject[nCount].rot.x, g_aObject[nCount].rot.z);
			D3DXMatrixMultiply(&g_aObject[nCount].mtxWorld, &g_aObject[nCount].mtxWorld, &mtxRot);

			//位置反映
			D3DXMatrixTranslation(&mtxTrans, g_aObject[nCount].pos.x, g_aObject[nCount].pos.y, g_aObject[nCount].pos.z);
			D3DXMatrixMultiply(&g_aObject[nCount].mtxWorld, &g_aObject[nCount].mtxWorld, &mtxTrans);

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aObject[nCount].mtxWorld);

			//マテリアルデータへのポインタ取得
			pMat = (D3DXMATERIAL*)objectModel.aParts[0].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)objectModel.aParts[0].dwNumMatModel; nCntMat++)
			{
				//マテリアル設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャ設定
				pDevice->SetTexture(0, objectModel.aParts[0].apTexture[nCntMat]);

				//モデル描画
				objectModel.aParts[0].pMesh->DrawSubset(nCntMat);
			}
		}
	}
	
	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//設定処理
//========================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE type)
{
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		//対象のフェンスが使われていない
		if (g_aObject[nCntObj].bUse == false)
		{
			//引数の情報を追加			
			g_aObject[nCntObj].posOld = g_aObject[nCntObj].pos = pos;			//初期位置設定
			g_aObject[nCntObj].rot = rot;										//角度を設定
			g_aObject[nCntObj].type = type;										//オブジェクト種類指定

			//使用していることにする
			g_aObject[nCntObj].bUse = true;

			//抜ける
			break;
		}
	}
}

//========================
//オブジェクト情報を返す処理
//GetObjectというマクロ関数がデフォルトで実在したため、後ろにInfoをつけました。
//========================
Object *GetObjectInfo(void)
{
	return &g_aObject[0];
}