//==========================================
//
//アイテムプログラム[item.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "item.h"
#include "PvP_player.h"
#include "color.h"

//マクロ定義
#define MAX_ITEM			(3)			//最大数
#define MAX_ITEM_TEX		(10)		//テクスチャの最大数
#define MAX_POS				(5)			//リスポーン位置最大数
#define INIT_POS_Y			(200.0f)	//初期のY位置
#define INIT_POS_XZ			(200.0f)	//初期の外位置
#define RATIO_MOVE			(100.0f)	//移動量の割合
#define COLLISION_SIZE_XZ	(30.0f)		//縦横の当たり判定サイズ
#define COLLISION_SIZE_Y	(30.0f)		//高さの当たり判定サイズ

//アイテム時間（すべて秒単位）
#define ITEMTIME_ATK		(7)			//ATKの時間
#define ITEMTIME_DEF		(7)			//DEFの時間
#define ITEMTIME_GHOST		(10)		//ゴースト化の時間
#define ITEMTIME_INV		(7)			//無敵の時間

const D3DXVECTOR3 g_ItemRespawnPos[MAX_POS] =
{
	D3DXVECTOR3(INIT_POS_XZ,NIL_F,NIL_F),
	D3DXVECTOR3(NIL_F,NIL_F,-INIT_POS_XZ),
	D3DXVECTOR3(-INIT_POS_XZ,NIL_F,NIL_F),
	D3DXVECTOR3(NIL_F,NIL_F,INIT_POS_XZ),
	D3DXVECTOR3(NIL_F,NIL_F,NIL_F),
};

//グローバル変数宣言     
LPDIRECT3DTEXTURE9		g_pTextureItem[MAX_ITEM_TEX] = {};	//テクスチャへにポインタ
LPD3DXMESH				g_pMeshItem = NULL;					//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER			g_pBuffMatItem = NULL;				//マテリアルへのポインタ
DWORD					g_dwNumMatItem = 0;					//マテリアルの数
D3DXMATRIX				g_mtxWorldItem;						//ワールドマトリックス
Item					g_Item[MAX_ITEM];					//アイテムの情報
bool					bposuse[MAX_POS];					//その座標を使用しているかどうか
int						g_nInvincibleItem;					//無敵アイテム取得回数

//========================
//初期化処理
//========================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL	  *pMat;					//マテリアルへのポインタ

												//xファイルの読み込み
	D3DXLoadMeshFromX({ "data\\MODEL\\konpeito\\konpeito.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatItem,
		NULL,
		&g_dwNumMatItem,
		&g_pMeshItem);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する　

		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureItem[nCntMat]);
		}
	}

	//初期設定
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos = ZERO_SET;
		g_Item[nCntItem].rot = ZERO_SET;
		g_Item[nCntItem].move = ZERO_SET;
		g_Item[nCntItem].DespawnLimit = 0;
		g_Item[nCntItem].fWidth = COLLISION_SIZE_XZ;
		g_Item[nCntItem].fAlpha = 1.0f;					//透明度の設定
		g_Item[nCntItem].buse = false;
	}

	for (int nCntPos = 0; nCntPos < MAX_POS; nCntPos++)
	{
		bposuse[nCntPos] = false;
	}

	g_nInvincibleItem = 0;
}

//========================
//終了処理
//========================
void UninitItem(void)
{
	//メッシュの破棄
	if (g_pMeshItem != NULL)
	{
		g_pMeshItem->Release();
		g_pMeshItem = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatItem != NULL)
	{
		g_pBuffMatItem->Release();
		g_pBuffMatItem = NULL;
	}

	//マテリアルの破棄
	for (int nCntTex = 0; nCntTex < MAX_ITEM_TEX; nCntTex++)
	{
		if (g_pTextureItem[nCntTex] != NULL)
		{
			g_pTextureItem[nCntTex]->Release();
			g_pTextureItem[nCntTex] = NULL;
		}
	}
}

//========================
//更新処理
//========================
void UpdateItem(void) 
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].buse == true)
		{
			g_Item[nCntItem].rot.y += 0.01f;
			g_Item[nCntItem].rot.x += 0.001f;
		}
	}
}

//========================
//描画処理
//========================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX		  mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9	  matDef;					//現在のマテリアル保存用
	D3DXMATERIAL	  *pMat;					//マテリアルデータへのポインタ

												//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].buse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldItem);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Item[nCntItem].rot.y, g_Item[nCntItem].rot.x, g_Item[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
			g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldItem, &g_mtxWorldItem, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldItem);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Emissive;

				//マテリアルの色設定
				pMat[nCntMat].MatD3D.Diffuse.a = g_Item[nCntItem].fAlpha;

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureItem[nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshItem->DrawSubset(nCntMat);
			}
		}

		/*------------------------------------------------------------------
		影の描画
		--------------------------------------------------------------------*/
		D3DXMATRIX	mtxShadow;		//シャドウマトリックス
		D3DLIGHT9	light;			//ライト情報
		D3DXVECTOR4	posLight;		//ライトの位置
		D3DXVECTOR3	pos, normal;	//平面上の任意の点、法線ベクトル
		D3DXPLANE	plane;			//平面情報

									//ライトの位置を設定
		pDevice->GetLight(3, &light);
		posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

		//平面情報を生成
		pos = ZERO_SET;
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXPlaneFromPointNormal(&plane, &pos, &normal);

		//シャドウマトリックスの初期化
		D3DXMatrixIdentity(&mtxShadow);

		//シャドウマトリックスの作成
		D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		D3DXMatrixMultiply(&mtxShadow, &g_mtxWorldItem, &mtxShadow);

		//シャドウマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatItem; nCntMat++)
		{
			D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//マテリアルデータ複製

															//黒色に設定						//自己発光を無くす
			MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

			//マテリアル設定
			pDevice->SetMaterial(&MatCopy);

			//テクスチャ設定
			pDevice->SetTexture(0, NULL);

			//モデル描画
			g_pMeshItem->DrawSubset(nCntMat);
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//設定処理
//========================
void SetItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].buse == false)
		{
			g_Item[nCntItem].RespawnDelay -= 1;//リスポーンディレイ減少

			if (g_Item[nCntItem].RespawnDelay <= 0)
			{//パラメーター設定

				g_Item[nCntItem].RespawnPos = (rand() % MAX_POS);

				if (bposuse[g_Item[nCntItem].RespawnPos] == false)
				{
					g_Item[nCntItem].pos = g_ItemRespawnPos[g_Item[nCntItem].RespawnPos]
						+ D3DXVECTOR3(NIL_F, (float)(rand() % 200), NIL_F);
					//アイテムの種類を設定
					g_Item[nCntItem].type = (ITEMTYPE)(rand() % (ITEMTYPE_MAX - g_nInvincibleItem));

					//無敵アイテムならカウント増やす
					(g_Item[nCntItem].type == ITEMTYPE_INVINCIBLE) ? g_nInvincibleItem++ : g_nInvincibleItem;

					g_Item[nCntItem].DespawnLimit = 0;
					bposuse[g_Item[nCntItem].RespawnPos] = true;
					g_Item[nCntItem].buse = true;
				}
				
			}
		}
	}
}

//========================
//アイテムとプレイヤーの当たり判定処理
//========================
void CollisionIP(int nPlayerNum)
{
	D3DXVECTOR3 pos0, pos1;
	D3DXVECTOR3 vecMove;
	D3DXVECTOR3 vecLine;
	D3DXVECTOR3 vecToPos, vecToPosOld;

	Player *pPlayer = GetPlayer();//プレイヤー情報取得

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].buse == true)
		{
			if (pPlayer[nPlayerNum].pos.x >= g_Item[nCntItem].pos.x - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.x <= g_Item[nCntItem].pos.x + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z >= g_Item[nCntItem].pos.z - COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.z <= g_Item[nCntItem].pos.z + COLLISION_SIZE_XZ
				&&pPlayer[nPlayerNum].pos.y >= g_Item[nCntItem].pos.y - COLLISION_SIZE_Y
				&&pPlayer[nPlayerNum].pos.y <= g_Item[nCntItem].pos.y + COLLISION_SIZE_Y)
			{//プレイヤーがの範囲内に入ったとき
				for (int nCntPos = 0; nCntPos < MAX_POS; nCntPos++)
				{
					if (g_Item[nCntItem].pos.x == g_ItemRespawnPos[nCntPos].x)
					{
						if (g_Item[nCntItem].pos.z == g_ItemRespawnPos[nCntPos].z)
						{
							bposuse[nCntPos] = false;

							//アイテム効果リセット
							pPlayer[nPlayerNum].nATKItemTime = 0;
							pPlayer[nPlayerNum].nDEFItemTime = 0;
							pPlayer[nPlayerNum].nGhostItemTime = 0;
						}
					}
				}

				if (pPlayer[nPlayerNum].nInvincibleTime <= 0)
				{
					switch (g_Item[nCntItem].type)
					{
					case ITEMTYPE_ATK:
						pPlayer[nPlayerNum].nATKItemTime = CONVERT_FPS(ITEMTIME_ATK);
						break;

					case ITEMTYPE_DEF:
						pPlayer[nPlayerNum].nDEFItemTime = CONVERT_FPS(ITEMTIME_DEF);
						break;

					case ITEMTYPE_GHOST:
						pPlayer[nPlayerNum].nGhostItemTime = CONVERT_FPS(ITEMTIME_GHOST);
						break;

					case ITEMTYPE_INVINCIBLE:
						pPlayer[nPlayerNum].nInvincibleTime = CONVERT_FPS(ITEMTIME_INV);
						break;
					}
				}
		
				//使われていない状態にする
				g_Item[nCntItem].RespawnDelay = 3;
				g_Item[nCntItem].fAlpha = 1.0f;
				g_Item[nCntItem].buse = false;
			}
		}
	}
}
