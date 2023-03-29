//==========================================
//
//アイテムプログラム[Block.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "HDR_player.h"
#include "Block.h"
#include "sound.h"

//マクロ定義
#define MAX_PLAYER			(4)			//最大数
#define MAX_BLOCK_TEX		(10)		//テクスチャの最大数
#define MAX_POS				(5)			//リスポーン位置最大数
#define INIT_POS_Y			(200.0f)	//初期のY位置
#define INIT_POS_XZ			(200.0f)	//初期の外位置
#define COLLISION_SIZE_XZ	(30.0f)		//縦横の当たり判定サイズ

//プロト
BLOCKTYPE SelectBlock(void);

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureBlock[MAX_BLOCK_TEX] = {};	//テクスチャへにポインタ
LPD3DXMESH				g_pMeshBlock = NULL;					//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER			g_pBuffMatBlock = NULL;					//マテリアルへのポインタ
DWORD					g_dwNumMatBlock = 0;					//マテリアルの数
D3DXMATRIX				g_mtxWorldBlock;						//ワールドマトリックス
Block					g_Block[MAX_BLOCK * MAX_PLAYER];		//ブロックの情報
int						g_aUseBlockNum[BLOCKTYPE_MAX];			//ブロック個数

//ブロック体力
const int				c_aBlockLife[BLOCKTYPE_MAX] = { 100,200,400 };

//配置数
const int				c_aBlockNum[BLOCKTYPE_MAX] = { 25,30,25 };

//========================
//初期化処理
//========================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATERIAL	  *pMat;					//マテリアルへのポインタ

	//xファイルの読み込み
	D3DXLoadMeshFromX({ "data/MODEL/block/block.x" },
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBlock,
		NULL,
		&g_dwNumMatBlock,
		&g_pMeshBlock);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatBlock->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBlock; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する　

		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureBlock[nCntMat]);
		}
	}

	//初期設定
	//ブロック情報設定
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		int nSetCount = 0;			//配置した数
		float PlayerPos_X = GetPlayer_HDR()[nCntPlayer].pos.x;	//プレイヤーのⅩ座標を格納
		//初期化
		for (int nCntBlockType = 0; nCntBlockType < BLOCKTYPE_MAX; nCntBlockType++)
		{
			g_aUseBlockNum[nCntBlockType] = c_aBlockNum[nCntBlockType];
		}

		for (int nCntBlock = MAX_BLOCK * nCntPlayer; nCntBlock < MAX_BLOCK * MAX_PLAYER; nCntBlock++)
		{
			g_Block[nCntBlock].pos = D3DXVECTOR3(PlayerPos_X, (float)nSetCount * COLLISION_SIZE_Y, 0.0f);	//位置を設定
			g_Block[nCntBlock].type = SelectBlock();							//固さ設定
			g_Block[nCntBlock].nLife = c_aBlockLife[g_Block[nCntBlock].type];	//体力設定
			g_Block[nCntBlock].buse = true;										//使う
			nSetCount++;	//設定した数加算

			//設定した数が一人分の上限に達した
			if (nSetCount == MAX_BLOCK)
			{//処理を止める
				break;
			}
		}
	}
}

//========================
//終了処理
//========================
void UninitBlock(void)
{
	//メッシュの破棄
	if (g_pMeshBlock != NULL)
	{
		g_pMeshBlock->Release();
		g_pMeshBlock = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatBlock != NULL)
	{
		g_pBuffMatBlock->Release();
		g_pBuffMatBlock = NULL;
	}

	//マテリアルの破棄
	for (int nCntTex = 0; nCntTex < MAX_BLOCK_TEX; nCntTex++)
	{
		if (g_pTextureBlock[nCntTex] != NULL)
		{
			g_pTextureBlock[nCntTex]->Release();
			g_pTextureBlock[nCntTex] = NULL;
		}
	}
}

//========================
//更新処理
//========================
void UpdateBlock(void)
{

}

//========================
//描画処理
//========================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX		  mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9	  matDef;					//現在のマテリアル保存用
	D3DXMATERIAL	  *pMat;					//マテリアルデータへのポインタ

												//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++)
	{
		if (g_Block[nCntBlock].buse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldBlock);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldBlock, &g_mtxWorldBlock, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBlock);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatBlock->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatBlock; nCntMat++)
			{

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshBlock->DrawSubset(nCntMat);
			}
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//ブロック選択処理
//========================
BLOCKTYPE SelectBlock(void)
{
	int nBlockType = rand() % BLOCKTYPE_MAX;

	if (g_aUseBlockNum[nBlockType] == 0)
	{//ブロックが配置上限に達している
		//2つ前（ループする）にする
		nBlockType = (nBlockType + (BLOCKTYPE_MAX - 1)) % BLOCKTYPE_MAX;

		if (g_aUseBlockNum[nBlockType] == 0)
		{//それも配置上限に達している
			//2つ前（ループする）にする
			nBlockType = (nBlockType + (BLOCKTYPE_MAX - 1)) % BLOCKTYPE_MAX;
			//絶対に置けるので終わり
		}
	}

	//置けるので設定
	g_aUseBlockNum[nBlockType]--;
	return (BLOCKTYPE)nBlockType;
}

//========================
//設定処理
//========================
void SetBlock(void)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++)
	{
		if (g_Block[nCntBlock].buse == false)
		{
			g_Block[nCntBlock].nLife = c_aBlockLife[g_Block[nCntBlock].type];
			g_Block[nCntBlock].buse = true;
		}
	}
}

//========================
//ブロックとプレイヤーの当たり判定処理
//========================
void CollisionBlock(Player_HDR *pPlayer)
{
	Block *pBlock = &g_Block[0];						//ブロックの情報取得

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK* MAX_PLAYER; nCntBlock++, pBlock++)
	{
		float
		Block_L = pBlock->pos.x - COLLISION_SIZE_XZ,	//左端
		Block_R = pBlock->pos.x + COLLISION_SIZE_XZ,	//右端
		Block_F = pBlock->pos.z - COLLISION_SIZE_XZ,	//手前
		Block_D = pBlock->pos.z + COLLISION_SIZE_XZ,	// 奥 
		Block_U = pBlock->pos.y - COLLISION_SIZE_Y,		// 底 
		Block_B = pBlock->pos.y + COLLISION_SIZE_Y;		//天面

		//ブロックが使われている
		if (pBlock->buse == true)
		{
			if (pPlayer->pos.x + pPlayer->fBodySize >= Block_L	&&	//左端より 右 にいる
				pPlayer->pos.x - pPlayer->fBodySize <= Block_R	&&	//右端より 左 にいる
				pPlayer->pos.z + pPlayer->fBodySize >= Block_F	&&	//手前より 奥 にいる
				pPlayer->pos.z - pPlayer->fBodySize <= Block_D	&&	// 奥 より手前にいる
				pPlayer->pos.y >= Block_U	&&	// 底 より 上 にいる
				pPlayer->pos.y <= Block_B)		//天面より 下 にいる
			{
				//プレイヤーがヒップドロップ中
				if (pPlayer->bHipDrop == true)
				{
					//ブロックの体力よりパワー（ダメージ量）が大きいければブロックを消す
					if (pPlayer->HipDropPower >= pBlock->nLife)
					{
						pBlock->buse = false;					//対象のブロックを使用しない
						pPlayer->HipDropPower -= pBlock->nLife;	//使用した分のダメージを減らす
						PlaySoundSE(SOUND_LABEL_SE_HIPDROP, nCntBlock);	//ヒップドロップ音再生
						DestroyBlockSound(pPlayer->nDestroyCounter++, nCntBlock);	//破壊した数別サウンド再生（そして同時に加算
					}
					else
					{
						g_Block[nCntBlock].nLife -= pPlayer->HipDropPower;
						pPlayer->HipDropPower = 0;
						pPlayer->nDestroyCounter = 0;
						pPlayer->move.y = 0;
						pPlayer->bHipDrop = false;
						pPlayer->nAICT = c_aAIParamHDR[pPlayer->aiDiff].nAICT;
					}
				}
				else
				{
					//プレイヤーがまだゴールしていない
					if (!pPlayer->bGoal)
					{
						pPlayer->pos.y = g_Block[nCntBlock].pos.y + COLLISION_SIZE_Y;
						pPlayer->move.y = 0;
						pPlayer->moveV0.y = 0;
						pPlayer->jumpTime = 0;
						pPlayer->bJump = false;
						pPlayer->bHipDrop = false;
					}
				}

				//====================================
				//			Xの当たり判定
				//====================================
				//左から当たった場合
				if (pPlayer->posOld.x + pPlayer->fBodySize <= Block_L &&	//前回ではプレイヤーが左に居て、
					pPlayer->pos.x + pPlayer->fBodySize >= Block_L)			//現在ではブロックが左にいる
				{
					pPlayer->pos.x = Block_L - pPlayer->fBodySize;	//ブロックの左端に戻す
					pPlayer->move.x = 0.0f;		//Ⅹの移動量を消す
					pPlayer->moveV0.x = 0.0f;	//Ⅹの移動量を消す
				}

				//右から当たった場合
				if (Block_R <= pPlayer->posOld.x - pPlayer->fBodySize &&	//前回ではプレイヤーが右に居て、
					Block_R >= pPlayer->pos.x - pPlayer->fBodySize)			//現在ではブロックが右にいる
				{
					pPlayer->pos.x = Block_R + pPlayer->fBodySize;	//ブロックの右端に戻す
					pPlayer->move.x = 0.0f;		//Ⅹの移動量を消す
					pPlayer->moveV0.x = 0.0f;	//Ⅹの移動量を消す
				}

				//====================================
				//			Zの当たり判定
				//====================================
				//手前から当たった場合
				if (pPlayer->posOld.z + pPlayer->fBodySize <= Block_F &&	//前回ではプレイヤーが手前に居て、
					pPlayer->pos.z + pPlayer->fBodySize >= Block_F)			//現在ではブロックが手前にいる
				{
					pPlayer->pos.z = Block_F - pPlayer->fBodySize;	//ブロックの手前に戻す
					pPlayer->move.z = 0.0f;		//Ｚの移動量を消す
					pPlayer->moveV0.z = 0.0f;	//Ｚの移動量を消す
				}

				//奥から当たった場合
				if (Block_D <= pPlayer->posOld.z - pPlayer->fBodySize &&	//前回ではプレイヤーが奥に居て、
					Block_D >= pPlayer->pos.z - pPlayer->fBodySize)			//現在ではブロックが奥にいる
				{
					pPlayer->pos.z = Block_D + pPlayer->fBodySize;	//ブロックの奥に戻す
					pPlayer->move.z = 0.0f;		//Ｚの移動量を消す
					pPlayer->moveV0.z = 0.0f;	//Ｚの移動量を消す
				}
			}
		}
	}
}

//========================
//ブロックの破壊個数によるサウンド処理
//========================
void DestroyBlockSound(int nDestroyCounter, int nCntBlock)
{
	//破壊した数によるサウンド再生判定
	switch (nDestroyCounter)
	{
	case 0:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_00, nCntBlock); break;	//破壊した数１個
	case 1:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_01, nCntBlock); break;	//破壊した数２個
	case 2:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_02, nCntBlock); break;	//破壊した数３個
	case 3:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_03, nCntBlock); break;	//破壊した数４個
	case 4:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_04, nCntBlock); break;	//破壊した数５個
	case 5:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_05, nCntBlock); break;	//破壊した数６個
	case 6:	PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_06, nCntBlock); break;	//破壊した数７個
	default:PlaySoundSE(SOUND_LABEL_SE_DESTROY_BLOCK_07, nCntBlock); break;	//破壊した数８個以上
	}
}