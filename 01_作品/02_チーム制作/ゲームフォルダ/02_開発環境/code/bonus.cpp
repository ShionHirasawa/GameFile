//============================================================================================
//
//ボーナスの処理[bonus.cpp]
//Author:藤原龍輝
//
//============================================================================================
#include"main.h"
#include"color.h"
#include"input.h"
#include"score.h"
#include"bonus.h"
#include"time.h"
#include"particle.h"

//****************************//
//		　 マクロ定義		  //
//****************************//
#define INIT_POS_Y			(200.0f)	//初期のY位置
#define INIT_POS_XZ			(200.0f)	//初期の外位置
#define RATIO_MOVE			(100.0f)	//移動量の割合
#define COLLISION_SIZE_XZ	(60.0f)		//縦横の当たり判定サイズ
#define COLLISION_SIZE_Y	(15.0f)		//高さの当たり判定サイズ

#define DESPAWN_LIMIT_BONUS (1200)		//ボーナスが消えるまでのリミット
#define PARTICLE_LIMIT_BONUS (6)		//ボーナスパーティクルのリミット
#define LIFE_CT				(60)		//ボーナス当たり判定のクールタイム

//****************************//
//		　　出現情報		  //
//****************************//
//位置
const D3DXVECTOR3 g_RespawnPos[] =
{
	D3DXVECTOR3(INIT_POS_XZ,INIT_POS_Y,NIL_F),
	D3DXVECTOR3(NIL_F,INIT_POS_Y,-INIT_POS_XZ),
	D3DXVECTOR3(-INIT_POS_XZ,INIT_POS_Y,NIL_F),
	D3DXVECTOR3(NIL_F,INIT_POS_Y,INIT_POS_XZ),
};

//角度
const D3DXVECTOR3 g_RespawnRot[] =
{
	ZERO_SET,
	D3DXVECTOR3(NIL_F, 1.57f,NIL_F),
	D3DXVECTOR3(NIL_F, 3.14f,NIL_F),
	D3DXVECTOR3(NIL_F,-1.57f,NIL_F),
};

//****************************//
//	   グローバル変数宣言     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTextureBonus[10] = {};	//テクスチャへにポインタ
LPD3DXMESH				g_pMeshBonus	    = NULL;	//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER			g_pBuffMatBonus		= NULL;	//マテリアルへのポインタ
DWORD					g_dwNumMatBonus		= 0;	//マテリアルの数
D3DXMATRIX				g_mtxWorldBonus;			//ワールドマトリックス
Bonus					g_Bonus;					//ボーナスの情報
int						g_ParticleCounter;
int						g_LifeCT;					//体力のクールタイム
int						g_PlayerNumTemp;				//プレイヤー番号の保存用
//===================================================
//ボーナスの初期化処理
//===================================================
void InitBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL	  *pMat;					//マテリアルへのポインタ

	//xファイルの読み込み
	D3DXLoadMeshFromX({ "data/MODEL/bird/bonus_bird.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBonus,
		NULL,
		&g_dwNumMatBonus,
		&g_pMeshBonus);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatBonus->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBonus; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する　
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureBonus[nCntMat]);
		}
	}

	//初期設定
	g_Bonus.Respawn = DIRECTION_ZERO;			//リスポーンの位置番号
	g_Bonus.pos = ZERO_SET;						//位置
	g_Bonus.rot = ZERO_SET;						//角度
	g_Bonus.move = ZERO_SET;					//移動量
	g_Bonus.DespawnLimit = 0;					//消える時間
	g_Bonus.a = 0.0f;							//透明度の設定
	g_Bonus.buse = false;						//使用しているかどうか
	g_ParticleCounter = PARTICLE_LIMIT_BONUS;	//パーティクルのでる間隔
	g_Bonus.nLife = 3;							//体力		
	g_LifeCT = 0;
	g_PlayerNumTemp = -1;
}
//===================================================
//ボーナスの終了処理
//===================================================
void UninitBonus(void)
{   
	//メッシュの破棄
	if (g_pMeshBonus != NULL)
	{
		g_pMeshBonus->Release();
		g_pMeshBonus = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatBonus != NULL)
	{
		g_pBuffMatBonus->Release();
		g_pBuffMatBonus = NULL;
	}
}
//===================================================
//ボーナスの更新処理
//===================================================
void UpdateBonus(void)
{
	if (g_Bonus.buse == true)
	{//使用されているとき

		//パーティクルセットまでの制限時間
		g_ParticleCounter--;

		if (g_ParticleCounter <= 0)
		{//0になったとき
		
			//パーティクルのセット
			SetParticle(g_Bonus.pos, 12.0f, 15, PARTICLE_NORMAL,OBJECT_BONUS);
			SetParticle(g_Bonus.pos, 7.0f, 15, PARTICLE_ACSORPTION, OBJECT_BONUS);

			//リミットの再設定
			g_ParticleCounter = PARTICLE_LIMIT_BONUS;
		}

		//移動処理
		MoveBonus();

		//消出現処理
		AppearandDisAppearBonus();

		//位置の更新
		g_Bonus.pos.x += g_Bonus.move.x;
		g_Bonus.pos.z += g_Bonus.move.z;

		//慣性の設定
		g_Bonus.move.x += (NIL_F - g_Bonus.move.x) * 1.0f;
		g_Bonus.move.z += (NIL_F - g_Bonus.move.z) * 1.0f;
	}
}
//===================================================
//ボーナスの描画処理
//===================================================
void DrawBonus(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX		  mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9	  matDef;					//現在のマテリアル保存用
	D3DXMATERIAL	  *pMat;					//マテリアルデータへのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (g_Bonus.buse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldBonus);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Bonus.rot.y, g_Bonus.rot.x, g_Bonus.rot.z);
		D3DXMatrixMultiply(&g_mtxWorldBonus, &g_mtxWorldBonus, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_Bonus.pos.x, g_Bonus.pos.y, g_Bonus.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldBonus, &g_mtxWorldBonus, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBonus);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatBonus->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBonus; nCntMat++)
		{
			//マテリアルの色設定
			pMat[nCntMat].MatD3D.Diffuse.a = g_Bonus.a;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBonus[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshBonus->DrawSubset(nCntMat);
		}

		//************************//
		//************************//
		//		  影の描画		  //
		//************************//
		//************************//

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
		D3DXMatrixMultiply(&mtxShadow, &g_mtxWorldBonus, &mtxShadow);

		//シャドウマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)g_pBuffMatBonus->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBonus; nCntMat++)
		{
			D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//マテリアルデータ複製

			//黒色に設定						//自己発光を無くす
			MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

			//マテリアル設定
			pDevice->SetMaterial(&MatCopy);

			//テクスチャ設定
			pDevice->SetTexture(0, NULL);

			//モデル描画
			g_pMeshBonus->DrawSubset(nCntMat);
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//===================================================
//ボーナスの設定処理
//===================================================
void SetBonus(void)
{
	if (g_Bonus.buse == false)
	{
		//現在時間の取得
		srand((unsigned int)time(0));

		//初期設定
		g_Bonus.Respawn = (BONUS)(rand() % 4);
		g_Bonus.pos = g_RespawnPos[g_Bonus.Respawn];
		g_Bonus.rot = g_RespawnRot[g_Bonus.Respawn];
		g_Bonus.DespawnLimit = DESPAWN_LIMIT_BONUS;
		g_Bonus.buse = true;
	}
}
//===================================================
//ボーナスの移動処理
//===================================================
void MoveBonus(void)
{
	//角度加算
	g_Bonus.rot.y += 0.01f;

	//移動方向に応じた処理
	switch (g_Bonus.Respawn)
	{
	case DIRECTION_ZERO:

		//移動量設定
		g_Bonus.move.x += sinf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_ZERO].x / RATIO_MOVE;
		g_Bonus.move.z += cosf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_ZERO].x / RATIO_MOVE;

		break;

	case DIRECTION_ONE:

		//移動量設定
		g_Bonus.move.x -= sinf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_ONE].z / RATIO_MOVE;
		g_Bonus.move.z -= cosf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_ONE].z / RATIO_MOVE;

		break;

	case DIRECTION_TWO:

		//移動量設定
		g_Bonus.move.x -= sinf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_TWO].x / RATIO_MOVE;
		g_Bonus.move.z -= cosf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_TWO].x / RATIO_MOVE;

		break;

	case DIRECTION_THREE:

		//移動量設定
		g_Bonus.move.x += sinf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_THREE].z / RATIO_MOVE;
		g_Bonus.move.z += cosf((D3DX_PI * 1.0f) + g_Bonus.rot.y) * g_RespawnPos[DIRECTION_THREE].z / RATIO_MOVE;

		break;
	}
}
//===================================================
//ボーナスの消出現処理
//===================================================
void AppearandDisAppearBonus(void)
{
	//消えるまでのカウントダウン
	g_Bonus.DespawnLimit--;

	if (g_Bonus.DespawnLimit <= 0)
	{
		if (g_Bonus.a > 0.0f)
		{
			//透明にしていく
			g_Bonus.a -= 0.01f;
		}
		else
		{
			//使われていない状態にする
			g_Bonus.buse = false;
		}
	}
	else if (g_Bonus.a < 1.0f)
	{
		//不透明にしていく
		g_Bonus.a += 0.01f;
	}
}
//===================================================
//ボーナスの当たり判定処理
//===================================================
void CollisionBonus(D3DXVECTOR3 nPlayer , int NumPlayer)
{
	if (g_LifeCT > 0)
	{
		g_LifeCT--;
	}
	else
	{
		g_PlayerNumTemp = -1;
	}

	if (g_Bonus.buse == true && g_PlayerNumTemp != NumPlayer)
	{//使用されているとき
		if (nPlayer.x >= g_Bonus.pos.x - COLLISION_SIZE_XZ
			&&nPlayer.x <= g_Bonus.pos.x + COLLISION_SIZE_XZ
			&&nPlayer.z >= g_Bonus.pos.z - COLLISION_SIZE_XZ
			&&nPlayer.z <= g_Bonus.pos.z + COLLISION_SIZE_XZ
			&&nPlayer.y >= g_Bonus.pos.y - COLLISION_SIZE_Y
			&&nPlayer.y <= g_Bonus.pos.y + COLLISION_SIZE_Y)
		{//プレイヤーがボーナスの範囲内に入ったとき

			if (g_Bonus.nLife > 0)
			{
				//体力の減少

				g_Bonus.nLife--;

				//クールタイムの設定
				g_LifeCT = LIFE_CT;

				//触れたプレイヤーの保存
				g_PlayerNumTemp = NumPlayer;
			}

			if (g_Bonus.nLife <= 0)
			{
				//使われていない状態にする
				g_Bonus.buse = false;

				//スコアを加算
				AddScore(3, NumPlayer);
			}
		}
	}
}
//====================================================
//ボーナスの取得処理
//====================================================
Bonus GetBonus(void)
{
	return g_Bonus;
}