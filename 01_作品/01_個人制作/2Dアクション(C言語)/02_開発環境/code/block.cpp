/*==========================================================================================================================

ブロックの処理[block.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "block.h"
#include "player.h"
#include "input.h"
#include "move.h"
#include <string.h>
#include "hako.h"

//マクロ定義
#define NUM_BLOCK				(3)		//背景の数
#define MAX_BLOCK				(22)	//ブロックの最大数
#define MOJI_MAX				(300)	//読み込む際の最大文字数
#define LIGHTEN_MAGNIFICATION	(0.02f)	//移動量を減少させるときの倍率
#define REVERSAL_MAGNIFICATION	(0.2f)	//移動量が逆転しだした時の倍率

#define BLOCK_FILE	"data/stage000.csv"	//ブロックの配置情報などが書かれたファイル

//プレイヤーの位置を探す
void SearchPlayer(Hako pHako);

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureBlock[NUM_BLOCK] = { NULL, NULL ,NULL };	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBlock = NULL;								//頂点バッファへのポインタ		
Block g_Block[MAX_BLOCK];

#define SIZEOF_BLOCK			(sizeof g_Block / sizeof(Block))	//ブロックデータのサイズ

//ブロックの初期化処理
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	//ブロックの設定処理呼び出し
	SetBlock();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Wall.png", &g_pTextureBlock[ZERO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Spike.png", &g_pTextureBlock[ONE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.jpg", &g_pTextureBlock[SEC]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BLOCK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBlock, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBlock = START_CNT; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//頂点座標の設定
		{
			pVtx[ZERO].pos = g_Block[nCntBlock].pos;
			pVtx[ONE].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x,  g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, ORIGIN_POS);
		}

		//rhwの設定
		{
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;
		}

		//頂点カラーの設定
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(g_Block[nCntBlock].Tex.x, 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , g_Block[nCntBlock].Tex.y);
			pVtx[SAN].tex = D3DXVECTOR2(g_Block[nCntBlock].Tex.x, g_Block[nCntBlock].Tex.y);
		}

		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffBlock->Unlock();
}

//ブロックの終了処理
void UninitBlock(void)
{
	//テクスチャ(３枚分)の破棄
	for (int nCntBlock = START_CNT; nCntBlock < NUM_BLOCK; nCntBlock++)
	{
		if (g_pTextureBlock[nCntBlock] != NULL)
		{
			g_pTextureBlock[nCntBlock]->Release();
			g_pTextureBlock[nCntBlock] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//ブロックの更新処理
void UpdateBlock(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	Player *pPlayer = GetPlayer();

	for (int nCntBlock = START_CNT; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += NUM_VERTEX)
	{
		g_Block[nCntBlock].posOld = g_Block[nCntBlock].pos;		//前回の位置を記憶
		float ReturnMagnification = F_ZERO;						//折り返し処理の倍率

		//座標を更新
		moveObject(&g_Block[nCntBlock].pos);

		//頂点座標の設定
		{
			pVtx[ZERO].pos = g_Block[nCntBlock].pos;
			pVtx[ONE].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, ORIGIN_POS);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffBlock->Unlock();
}

//ブロックの描画処理
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ
	int nCntBlock;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBlock, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = START_CNT; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_Block[nCntBlock].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureBlock[g_Block[nCntBlock].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * NUM_VERTEX, SEC);
		}
	}
}

//ブロックの設定処理
void SetBlock(void)
{
	FILE *pFile;
	char aData[MOJI_MAX];

	pFile = fopen(BLOCK_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//最初のセルタイトル読み込み

		for (int nCntBlock = START_CNT; nCntBlock < MAX_BLOCK; nCntBlock++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み
			
			//配置場所の初期化
			g_Block[nCntBlock].pos.x = (float)atof(strtok(&aData[ZERO], ","));	//位置初期化
			g_Block[nCntBlock].pos.y = (float)atof(strtok(NULL, ","));			//位置初期化

			g_Block[nCntBlock].nType = atoi(strtok(NULL, ","));			//種類の初期化
			g_Block[nCntBlock].fWidth = (float)atof(strtok(NULL, ","));		//幅の初期化
			g_Block[nCntBlock].fHeight = (float)atof(strtok(NULL, ","));		//高さの初期化

			//テクスチャの座標
			g_Block[nCntBlock].Tex.x = (float)atof(strtok(NULL, ","));			//一つのブロックに対するテクスチャの描画数を読み込み
			g_Block[nCntBlock].Tex.y = (float)atof(strtok(NULL, ","));			//一つのブロックに対するテクスチャの描画数を読み込み

			g_Block[nCntBlock].posOld = g_Block[nCntBlock].pos;			//前回の位置を現在の位置に初期化
			g_Block[nCntBlock].bUse = true;								//使用している状態に
		}
	}
}

/*￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣
			ブロックの当たり判定
______________________________________________*/

//横
bool X_CollisionBlockToPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight, Block **pBlock)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報
	bool bSide = false;				//着地していない状態に

	for (int nCntBlock = START_CNT; nCntBlock < SIZEOF_BLOCK; nCntBlock++)
	{
		float HeadPos     = pPos->y - fHeight,		//現在の頭の位置
			  fWidth_R    = pPos->x + fWidth,		//現在の体(右側)の横幅
			  fWidth_L    = pPos->x - fWidth,		//現在の体(左側)の横幅
			  HeadPosOld  = pPosold->y - fHeight,	//前回の頭の位置
			  fOldWidth_R = pPosold->x + fWidth,	//前回の体(右側)の横幅
			  fOldWidth_L = pPosold->x - fWidth;	//前回の体(左側)の横幅

		if (g_Block[nCntBlock].bUse == true)
		{
			//現在の位置がブロックの当たり判定の範囲内
			if (g_Block[nCntBlock].pos.x < fWidth_R && fWidth_L < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
			{//現在のブロックの位置(上と下)
				//現在のブロックの位置(左と右)
				if (HeadPos < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight && pPos->y > g_Block[nCntBlock].pos.y)
				{//ブロックの底より頭が高く、ブロックの天井より足が低い
				 //ブロックに左から当たる
					if (g_Block[nCntBlock].posOld.x >= fOldWidth_R &&	//前回の位置がブロックよりも左で
						g_Block[nCntBlock].pos.x <= fWidth_R)		//現在の位置がブロックにめり込んでいる
					{
						pPos->x = g_Block[nCntBlock].pos.x - fWidth;//ブロックの位置+横幅分に戻す
						bSide = true;					//ブロックにぶつかった
						pMove->x = 0.0f;				//移動量Xを０にする
					}

					//ブロックに右から当たる
					if (g_Block[nCntBlock].posOld.x + g_Block[nCntBlock].fWidth <= fOldWidth_L	//前回の体(右側)の位置がブロックよりも右で
					 && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth >= fWidth_L)	//現在の体(右側)の位置がブロックにめり込んでいる
					{
						pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + fWidth;	//ブロックの横幅の位置+横幅分戻す
						bSide = true;					//ブロックにぶつかった
						pMove->x = 0.0f;				//移動量Xを０にする
					}
				}
			}
		}
	}

	return bSide;	//着地したかどうかを返す
}

//縦
bool Y_CollisionBlockToPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight, Block **pBlock)
{
	bool bLand = false;		//着地していない状態に

	for (int nCntBlock = START_CNT; nCntBlock < SIZEOF_BLOCK; nCntBlock++)
	{
		float HeadPos = pPos->y - fHeight,		//現在の頭の位置
			fWidth_R = pPos->x + fWidth,		//現在の体(右側)の横幅
			fWidth_L = pPos->x - fWidth,		//現在の体(左側)の横幅
			HeadPosOld = pPosold->y - fHeight,	//前回の頭の位置
			fOldWidth_R = pPosold->x + fWidth,	//前回の体(右側)の横幅
			fOldWidth_L = pPosold->x - fWidth;	//前回の体(左側)の横幅

		if (g_Block[nCntBlock].bUse == true)
		{
			//現在の位置がブロックの当たり判定の範囲内
			if (g_Block[nCntBlock].pos.x < fWidth_R && fWidth_L < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
			{//現在のブロックの位置(上と下)
			 //ブロックに足がめり込んだ(乗っかる)
				if (g_Block[nCntBlock].posOld.y >= pPosold->y &&	//前回の位置がブロックよりも上で
					g_Block[nCntBlock].pos.y <= pPos->y)		//現在の位置がブロックにめり込んでいる
				{

					if (g_Block[nCntBlock].nType == 1)
					{
						HitPlayer(PLAYERSTATE_DAMAGE);
					}
					else
					{
						bLand = true;							//着地した
						pPos->y = g_Block[nCntBlock].pos.y;		//ブロックの位置に戻す
						pMove->y = 0.0f;						//移動量Yを０にする

						if (pBlock != NULL)
						{//ブロックのアドレスを入れる場所がある
							*pBlock = &g_Block[nCntBlock];
						}
					}
				}

				//ブロックに頭がめり込んだ
				else if (g_Block[nCntBlock].posOld.y + g_Block[nCntBlock].fHeight <= HeadPosOld	//前回の頭の位置がブロックよりも下で
					&& g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight >= HeadPos)		//現在の頭の位置がブロックに突き刺さっている
				{
					pPos->y += g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight - HeadPos;	//ブロックの底から頭の位置分引いたところに戻す
					pMove->y = 0.0f;						//移動量Yを０にする
				}
			}
		}
	}

	return bLand;	//着地したかどうかを返す
}

//ブロックとハコとの当たり判定
bool CollisionBlockToHako(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight, Block **pBlock)
{
	D3DXVECTOR3 FixMove = ZERO_SET;	//ハコの位置修正変数
	bool bLand = false;		//着地していない状態に

	for (int nCntBlock = START_CNT; nCntBlock < SIZEOF_BLOCK; nCntBlock++)
	{
		float BottomPos    = pPos->y + fHeight,		//現在の底の位置
			  fWidth_R     = pPos->x + fWidth,		//現在の体(右側)の横幅
			  BottomPosOld = pPosold->y + fHeight,	//前回の底の位置
			  fOldWidth_R  = pPosold->x + fWidth;	//前回の体(右側)の横幅

		if (g_Block[nCntBlock].bUse == true)
		{
			//現在の位置がブロックの当たり判定の範囲内
			if (g_Block[nCntBlock].pos.x < fWidth_R && pPos->x < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
			{//現在のブロックの位置(上と下)
			 //ブロックに底がめり込んだ(乗っかる)
				if (g_Block[nCntBlock].posOld.y >= BottomPosOld &&		//前回の底の位置がブロックよりも上で
					g_Block[nCntBlock].pos.y <= BottomPos)				//現在の底の位置がブロックにめり込んでいる
				{
					bLand = true;										//着地した
					pMove->y = 0.0f;									//移動量Yを０にする

					FixMove.y = g_Block[nCntBlock].pos.y - BottomPos;	//修正量計算

					FixHakoPos(FixMove);		//ハコ全部修正

					if (pBlock != NULL)
					{//ブロックのアドレスを入れる場所がある
						*pBlock = &g_Block[nCntBlock];
					}
				}

				//ブロックにハコの天井がめり込んだ
				else if (g_Block[nCntBlock].posOld.y + g_Block[nCntBlock].fHeight <= pPosold->y	//前回の天井の位置がブロックよりも下で
					  && g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight >= pPos->y)		//現在の天井の位置がブロックにめり込んでいる
				{
					FixMove.y = (g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight) - pPos->y;	//修正量計算

					FixHakoPos(FixMove);		//ハコ全部修正
				}

				//現在のブロックの位置(左と右)
				if (pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight && BottomPos > g_Block[nCntBlock].pos.y)
				{//ブロックの底よりハコの天井が高く、ブロックの天井よりハコの底が低い		/* Visual Studioのマークみたいな感じ */

					 //ブロックに左から当たる
					if (g_Block[nCntBlock].posOld.x >= fOldWidth_R &&	//前回の位置がブロックよりも左で
						g_Block[nCntBlock].pos.x <= fWidth_R)		//現在の位置がブロックにめり込んでいる
					{
						FixMove.x = g_Block[nCntBlock].pos.x - fWidth_R;	//修正量計算

						FixHakoPos(FixMove);		//ハコ全部修正
					}

					//ブロックに右から当たる
					if (g_Block[nCntBlock].posOld.x + g_Block[nCntBlock].fWidth <= pPosold->x	//前回の位置がブロックよりも右で
					 && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth >= pPos->x)	//現在の位置がブロックにめり込んでいる
					{
						FixMove.x = (g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth) - pPos->x;	//修正量計算

						FixHakoPos(FixMove);		//ハコ全部修正
					}
				}
			}
		}
	}

	return bLand;	//着地したかどうかを返す
}

//ハコの位置修正
void FixHakoPos(D3DXVECTOR3 FixMove)
{
	Player *pPlayer = GetPlayer();
	Hako *pHako = GetHako();

	for (int nCntHako = START_CNT; nCntHako < FIVE; nCntHako++, pHako++)
	{
		if (pHako->bUse == true)
		{//ハコが使われている
			 //ハコの位置修正
			pHako->pos += FixMove;
			pHako->move = ZERO_SET;

			//プレイヤーがハコに埋まっているか調べる
			SearchPlayer(*pHako);
		}
		else
		{//使われなければ修正終了
			break;
		}
	}

}

//プレイヤーの位置を探す
void SearchPlayer(Hako pHako)
{
	Player *pPlayer = GetPlayer();

	//プレイヤーの当たり判定
	float HeadPos	  = pPlayer->pos.y	  - pPlayer->fHeight,	//現在の頭の位置
		  Width_R	  = pPlayer->pos.x	  + pPlayer->fWidth,	//現在の体(右側)の横幅
		  Width_L	  = pPlayer->pos.x	  - pPlayer->fWidth,	//現在の体(左側)の横幅
		  OldWidth_R  = pPlayer->posOld.x + pPlayer->fWidth,	//前回の体(右側)の横幅
		  OldWidth_L  = pPlayer->posOld.x - pPlayer->fWidth;	//前回の体(左側)の横幅

	//ハコの当たり判定
	float BottomPos	   = pHako.pos.y	+ pHako.fHeight,	//現在の底の位置
		  Hako_R	   = pHako.pos.x	+ pHako.fWidth,		//現在の体(右側)の横幅
		  BottomPosOld = pHako.posOld.y	+ pHako.fHeight,	//前回の底の位置
		  HakoOld_R   = pHako.posOld.x	+ pHako.fWidth;		//前回の体(右側)の横幅

	//プレイヤーの位置修正
	if (HeadPos < BottomPos && pPlayer->pos.y > pHako.pos.y)
	{
		//ハコに左から当たる
		if (pHako.posOld.x >= OldWidth_R &&	//前回の位置がハコよりも左で
			pHako.pos.x < Width_R)				//現在の位置がハコにめり込んでいる
		{
			//ハコの左の位置に合わせる
			pPlayer->pos.x = pHako.pos.x - pPlayer->fWidth;
		}

		//ハコに右から当たる
		if (HakoOld_R <= OldWidth_L	//前回の体(右側)の位置がハコよりも右で
			&& Hako_R >  Width_L)	//現在の体(右側)の位置がハコにめり込んでいる
		{
			//ハコの右の位置に合わせる
			pPlayer->pos.x = Hako_R + pPlayer->fWidth;
		}
	}
}