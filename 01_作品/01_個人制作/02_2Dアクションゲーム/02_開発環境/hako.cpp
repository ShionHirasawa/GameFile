/*==========================================================================================================================

ブロックの処理[block.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "hako.h"
#include "block.h"
#include "player.h"
#include "input.h"
#include "gravity.h"
#include "move.h"
#include "sound.h"
#include <string.h>
#include "particle.h"

//マクロ定義
#define MAX_MyBLOCK			(5)							//ハコの最大数
#define BIRTH_PARTICLE_HAKO	(5)							//パーティクルの発生量
#define MOJI_MAX			(300)						//読み込む際の最大文字数
#define MyHAKO_SIZE			(60.0f)						//ハコのサイズ
#define HAKO_SPAWN_RIGHT	(60.0f)						//ハコが右に生まれる距離調整
#define HAKO_SPAWN_LEFT		(55.0f)						//ハコが左に生まれる距離調整
#define HAKO_SPAWN_OVER		(30.0f)						//ハコが上に生まれる距離調整
#define THROW_MOVE			(8.0f)						//投げるスピード
#define LIFT_UP				(10.0f)						//もち上げる高さ
#define UNLOAD				(10.0f)						//ハコを降ろす高さ
#define FIX_MARGIN			(D3DXVECTOR2(0.0f, 1.0f))	//プレイヤーの余白調整
#define SHADE_HAKO			(8)							//ハコのフェード変化量

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureHako = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffHako = NULL;	//頂点バッファへのポインタ
Hako g_Hako[MAX_MyBLOCK];							//プレイヤーのブロック構造体
int g_nDeliveryCnt;									//出産カウンタ
bool g_bGravity;									//ハコに重力を加えるかどうか  false:重力を加える   true:重力を加えない

#define SIZEOF_HAKO			(sizeof g_Hako / sizeof(Hako))	//ハコのデータサイズ

//ハコの初期化処理
void InitHako(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/playerHAKO.png", &g_pTextureHako);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * SIZEOF_HAKO, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHako, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHako->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		//プレイヤーが生み出すハコの情報初期化
		{
			g_Hako[nCntHako].pos = ZERO_SET;				//位置初期化
			g_Hako[nCntHako].posOld = ZERO_SET;				//前回の位置も初期化
			g_Hako[nCntHako].move = ZERO_SET;				//移動量初期化
			g_Hako[nCntHako].fWidth = MyHAKO_SIZE;			//幅初期化
			g_Hako[nCntHako].fHeight = MyHAKO_SIZE;			//高さ初期化
			g_Hako[nCntHako].myBlockState = HAKO_NONE;		//生まれていない状態に初期化
			g_Hako[nCntHako].nDirect = DIRECTION_MAX;		//方向は決まっていないことに
			g_Hako[nCntHako].pBlock = NULL;					//ブロックの情報無し
			g_Hako[nCntHako].nCol = COLOR_MAX;				//色の初期化
			g_Hako[nCntHako].bShade = false;				//使用されていない状態に初期化
			g_Hako[nCntHako].bUse = false;					//使用されていない状態に初期化
		}

		//頂点座標の設定
		{
			pVtx[ZERO].pos = g_Hako[nCntHako].pos;
			pVtx[ONE].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth, g_Hako[nCntHako].pos.y, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x,  g_Hako[nCntHako].pos.y + g_Hako[nCntHako].fHeight, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth, g_Hako[nCntHako].pos.y + g_Hako[nCntHako].fHeight, ORIGIN_POS);
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
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN,       ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS,  F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN,       F_TEN);
		}

		pVtx += NUM_VERTEX;
	}

	g_nDeliveryCnt = ZERO;		//出産カウンタ初期化
	g_bGravity = false;			//重力処理判定初期化

	//頂点バッファをロックする
	g_pVtxBuffHako->Unlock();
}

//ハコの終了処理
void UninitHako(void)
{
	//テクスチャの破棄
	if (g_pTextureHako != NULL)
	{
		g_pTextureHako->Release();
		g_pTextureHako = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHako != NULL)
	{
		g_pVtxBuffHako->Release();
		g_pVtxBuffHako = NULL;
	}
}

//ハコの更新処理
void UpdateHako(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHako->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//すべてのハコを削除 & 色修正
	if (GetKeyboardTrigger(DIK_DELETE) == true || GetGamePadTrigger(XINPUT_GAMEPAD_Y) == true || pPlayer->state == PLAYERSTATE_GOAL)
	{
		ResetHako();
	}
	
	//すべてのハコを持ち上げ移動させる
	LiftUpHako();

	//ハコ運び中
	CarryHako();

	//ハコの重力処理(ブロックの当たり判定もここで)
	GravityHako();

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++, pVtx += NUM_VERTEX)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			g_Hako[nCntHako].posOld = g_Hako[nCntHako].pos;		//前回の位置を記憶

			//ハコの状態による処理分岐
			SwitchHako(nCntHako);

			//座標更新
			moveObject(&g_Hako[nCntHako].pos);

			//頂点座標の設定
			{
				pVtx[ZERO].pos = g_Hako[nCntHako].pos;
				pVtx[ONE].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + MyHAKO_SIZE,  g_Hako[nCntHako].pos.y,  ORIGIN_POS);
				pVtx[SEC].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x,  MyHAKO_SIZE + g_Hako[nCntHako].pos.y , ORIGIN_POS);
				pVtx[SAN].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + MyHAKO_SIZE,  g_Hako[nCntHako].pos.y + MyHAKO_SIZE, ORIGIN_POS);
			}

			//頂点カラーの設定
			{
				pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
				pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
				pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
				pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Hako[nCntHako].nCol);
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffHako->Unlock();
}

//ハコの描画処理
void DrawHako(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffHako, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureHako);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHako * NUM_VERTEX, SEC);
		}
	}
}

//ハコの産み出し処理
void SetHako(void)
{	
	Player *pPlayer = GetPlayer();	//プレイヤーの情報

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHako->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//一つ目生成
	if (g_nDeliveryCnt == ZERO)
	{	//右に出す
		if (GetKeyboardTrigger(DIK_RIGHT) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{
			//ハコをすべて消去する
			ResetHako();
			
			//ハコの生み出し音楽
			HakoSpawnSound();

			g_Hako[g_nDeliveryCnt].pos.x = pPlayer->pos.x + HAKO_SPAWN_RIGHT;
			g_Hako[g_nDeliveryCnt].pos.y = pPlayer->pos.y - MyHAKO_SIZE;
			g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_RIGHT;	//右方向に生まれた
			g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//生まれた状態に
			g_Hako[g_nDeliveryCnt].bUse = true;
			g_nDeliveryCnt++;
		}

		//左に出す
		else if (GetKeyboardTrigger(DIK_LEFT) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{
			//ハコをすべて消去する
			ResetHako();

			//ハコの生み出し音楽
			HakoSpawnSound();

			g_Hako[g_nDeliveryCnt].pos.x = pPlayer->pos.x - (pPlayer->fWidth + HAKO_SPAWN_LEFT);
			g_Hako[g_nDeliveryCnt].pos.y = pPlayer->pos.y - MyHAKO_SIZE;
			g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_LEFT;	//左方向に生まれた
			g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//生まれた状態に
			g_Hako[g_nDeliveryCnt].bUse = true;
			g_nDeliveryCnt++;
		}

		//上に出す
		else if (GetKeyboardTrigger(DIK_UP) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
		{
			//ハコをすべて消去する
			ResetHako();

			//ハコの生み出し音楽
			HakoSpawnSound();

			g_Hako[g_nDeliveryCnt].pos.x = pPlayer->pos.x - HAKO_SPAWN_OVER;
			g_Hako[g_nDeliveryCnt].pos.y = pPlayer->pos.y - (MyHAKO_SIZE + MyHAKO_SIZE);
			g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_OVER;		//上方向に生まれた
			g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;		//生まれた状態に
			g_Hako[g_nDeliveryCnt].bUse = true;
			g_nDeliveryCnt++;
		}

		//何も操作が行われていない
		else
		{
			FadeHako();								//ハコを生み出すまで既存のハコをフェードさせる
			pPlayer->state = PLAYERSTATE_NORMAL;	//通常モード
		}
	}

	//二つ目以降生成
	else if(g_nDeliveryCnt <= MAX_MyBLOCK)
	{//２つ目以降はひとつ前のハコを元に生成
		//右に生成
		if (GetKeyboardTrigger(DIK_RIGHT) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{
			//ひとつ前のハコの方向が逆方向に生まれている
			if (g_Hako[g_nDeliveryCnt - ONE].nDirect == DIRECTION_LEFT)
			{//ひとつ前のハコを消す
				g_Hako[g_nDeliveryCnt - ONE].bUse = false;
				g_nDeliveryCnt--;
			}
			else if(g_nDeliveryCnt < MAX_MyBLOCK)
			{
				//ハコの生み出し音楽
				HakoSpawnSound();

				g_Hako[g_nDeliveryCnt].pos.x = g_Hako[g_nDeliveryCnt - ONE].pos.x + MyHAKO_SIZE;
				g_Hako[g_nDeliveryCnt].pos.y = g_Hako[g_nDeliveryCnt - ONE].pos.y;
				g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_RIGHT;	//右方向に生まれた
				g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//生まれた状態に
				g_Hako[g_nDeliveryCnt].bUse = true;
				g_nDeliveryCnt++;
			}
		}

		//左に生成
		else if (GetKeyboardTrigger(DIK_LEFT) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{
			//ひとつ前のハコの方向が逆方向に生まれている
			if (g_Hako[g_nDeliveryCnt - ONE].nDirect == DIRECTION_RIGHT)
			{//ひとつ前のハコを消す
				g_Hako[g_nDeliveryCnt - ONE].bUse = false;
				g_nDeliveryCnt--;
			}
			else if (g_nDeliveryCnt < MAX_MyBLOCK)
			{
				//ハコの生み出し音楽
				HakoSpawnSound();

				g_Hako[g_nDeliveryCnt].pos.x = g_Hako[g_nDeliveryCnt - ONE].pos.x - MyHAKO_SIZE;
				g_Hako[g_nDeliveryCnt].pos.y = g_Hako[g_nDeliveryCnt - ONE].pos.y;
				g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_LEFT;	//左方向に生まれた
				g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//生まれた状態に
				g_Hako[g_nDeliveryCnt].bUse = true;
				g_nDeliveryCnt++;
			}
		}

		//上に生成
		else if (GetKeyboardTrigger(DIK_UP) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
		{
			//ひとつ前のハコの方向が逆方向に生まれている
			if (g_Hako[g_nDeliveryCnt - ONE].nDirect == DIRECTION_UNDER)
			{//ひとつ前のハコを消す
				g_Hako[g_nDeliveryCnt - ONE].bUse = false;
				g_nDeliveryCnt--;
			}
			else if (g_nDeliveryCnt < MAX_MyBLOCK)
			{
				//ハコの生み出し音楽
				HakoSpawnSound();

				g_Hako[g_nDeliveryCnt].pos.x = g_Hako[g_nDeliveryCnt - ONE].pos.x;
				g_Hako[g_nDeliveryCnt].pos.y = g_Hako[g_nDeliveryCnt - ONE].pos.y - MyHAKO_SIZE;
				g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_OVER;	//上方向に生まれた
				g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//生まれた状態に
				g_Hako[g_nDeliveryCnt].bUse = true;
				g_nDeliveryCnt++;
			}
		}

		//下に生成
		else if (GetKeyboardTrigger(DIK_DOWN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{
			//ひとつ前のハコの方向が逆方向に生まれている
			if (g_Hako[g_nDeliveryCnt - ONE].nDirect == DIRECTION_OVER)
			{//ひとつ前のハコを消す
				g_Hako[g_nDeliveryCnt - ONE].bUse = false;
				g_nDeliveryCnt--;
			}
			else if (g_nDeliveryCnt < MAX_MyBLOCK)
			{
				//ハコの生み出し音楽
				HakoSpawnSound();

				g_Hako[g_nDeliveryCnt].pos.x = g_Hako[g_nDeliveryCnt - ONE].pos.x;
				g_Hako[g_nDeliveryCnt].pos.y = g_Hako[g_nDeliveryCnt - ONE].pos.y + MyHAKO_SIZE;
				g_Hako[g_nDeliveryCnt].nDirect = DIRECTION_UNDER;	//右方向に生まれた
				g_Hako[g_nDeliveryCnt].myBlockState = HAKO_SPAWN;	//生まれた状態に
				g_Hako[g_nDeliveryCnt].bUse = true;
				g_nDeliveryCnt++;
			}			
		}
	}

	//ハコの設定
	for (int nCntHako = START_CNT; nCntHako < g_nDeliveryCnt; nCntHako++, pVtx += NUM_VERTEX)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			g_Hako[nCntHako].posOld = g_Hako[nCntHako].pos;

			//頂点座標の設定
			{
				pVtx[ZERO].pos = g_Hako[nCntHako].pos;
				pVtx[ONE].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth, g_Hako[nCntHako].pos.y, ORIGIN_POS);
				pVtx[SEC].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x,  g_Hako[nCntHako].pos.y + g_Hako[nCntHako].fHeight, ORIGIN_POS);
				pVtx[SAN].pos = D3DXVECTOR3(g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth, g_Hako[nCntHako].pos.y + g_Hako[nCntHako].fHeight, ORIGIN_POS);
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
				pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
				pVtx[ONE].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
				pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
				pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffHako->Unlock();
}

/*￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣
				ハコの当たり判定
_______________________________________________*/
//横
void X_CollisionHako(void)
{
	Player *pPlayer = GetPlayer();					//プレイヤーの情報

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		float HeadPos     = pPlayer->pos.y - pPlayer->fHeight,		//現在の頭の位置
			  fWidth_R    = pPlayer->pos.x + pPlayer->fWidth,		//現在の体(右側)の横幅
			  fWidth_L    = pPlayer->pos.x - pPlayer->fWidth,		//現在の体(左側)の横幅
			  fOldWidth_R = pPlayer->posOld.x + pPlayer->fWidth,	//前回の体(右側)の横幅
			  fOldWidth_L = pPlayer->posOld.x - pPlayer->fWidth;	//前回の体(左側)の横幅

		if (g_Hako[nCntHako].bUse == true)
		{
			//現在の位置がハコの当たり判定の範囲内
			if (g_Hako[nCntHako].pos.x < fWidth_R && fWidth_L < g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth)
			{
				//ハコの底より頭が高く、ハコの天井より足が低い
				if (HeadPos < g_Hako[nCntHako].pos.y + MyHAKO_SIZE && pPlayer->pos.y > g_Hako[nCntHako].pos.y)
				{
					//ハコに左から当たる
					if (g_Hako[nCntHako].posOld.x >= fOldWidth_R &&	//前回の位置がハコよりも左で
						g_Hako[nCntHako].pos.x < fWidth_R)			//現在の位置がハコにめり込んでいる
					{
						//プレイヤーがブロックに乗っている
						if (pPlayer->bOnBlock == true)
						{
							if (pPlayer->bOnHako == false)
							{
								//ハコを動かす
								HakoMove((pPlayer->pos.x + pPlayer->fWidth) - g_Hako[nCntHako].pos.x);
							}
						}
						//プレイヤーがブロックに乗っていない
						else
						{
							pPlayer->pos.x = g_Hako[nCntHako].pos.x - pPlayer->fWidth;	//ハコの位置に戻す
							pPlayer->bSideBlock = true;									//ハコに当たった
						}
					}

					//ハコに右から当たる
					if (g_Hako[nCntHako].posOld.x + MyHAKO_SIZE <= fOldWidth_L	//前回の体(右側)の位置がハコよりも右で
						&& g_Hako[nCntHako].pos.x + MyHAKO_SIZE > fWidth_L)	//現在の体(右側)の位置がハコにめり込んでいる
					{
						//プレイヤーがブロックに乗っている
						if (pPlayer->bOnBlock == true)
						{
							if (pPlayer->bOnHako == false)
							{
								//ハコを動かす
								HakoMove((pPlayer->pos.x - pPlayer->fWidth) - (g_Hako[nCntHako].pos.x + MyHAKO_SIZE));
							}
						}
						//プレイヤーがブロックに乗っていない
						else
						{
							pPlayer->pos.x = g_Hako[nCntHako].pos.x + MyHAKO_SIZE + pPlayer->fWidth;	//ハコの位置に戻す
							pPlayer->bSideBlock = true;													//ハコに当たった
						}
					}
				}
			}
		}
	}
}

//縦
bool Y_CollisionHako(void)
{
	bool bLand = false;								//着地していない状態に
	Player *pPlayer = GetPlayer();					//プレイヤーの情報

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		float HeadPos    = pPlayer->pos.y - pPlayer->fHeight,		//現在の頭の位置
			  fWidth_R   = pPlayer->pos.x + pPlayer->fWidth,		//現在の体(右側)の横幅
			  fWidth_L   = pPlayer->pos.x - pPlayer->fWidth,		//現在の体(左側)の横幅
			  HeadPosOld = pPlayer->posOld.y - pPlayer->fHeight;	//前回の頭の位置

		if (g_Hako[nCntHako].bUse == true)
		{
			//現在の位置がハコの当たり判定の範囲内
			if (g_Hako[nCntHako].pos.x < fWidth_R && fWidth_L < g_Hako[nCntHako].pos.x + g_Hako[nCntHako].fWidth)
			{//現在のハコの位置(上と下)
			 //ハコに足がめり込んだ(乗っかる)
				if (g_Hako[nCntHako].posOld.y >= pPlayer->posOld.y	//前回の位置がハコよりも上で
					&& g_Hako[nCntHako].pos.y < pPlayer->pos.y)	//現在の位置がハコにめり込んでいる
				{
					bLand = true;								//着地した
					pPlayer->pos.y = g_Hako[nCntHako].pos.y;	//ハコの位置に戻す
					pPlayer->move.y = 0.0f;						//移動量Yを０にする
				}

				//ハコに頭がめり込んだ
				else if (g_Hako[nCntHako].posOld.y + MyHAKO_SIZE <= HeadPosOld	//前回の頭の位置がハコよりも下で
					  && g_Hako[nCntHako].pos.y + MyHAKO_SIZE > HeadPos)		//現在の頭の位置がハコに突き刺さっている
				{
					g_Hako[nCntHako].pos.y = HeadPos - MyHAKO_SIZE;	//頭にのせる
				}
			}
		}
	}

	return bLand;	//着地したかどうかを返す
}

//ハコを動かす
void HakoMove(float fHakoMove)
{
	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		g_Hako[nCntHako].pos.x += fHakoMove;		//すべてのハコを押す
	}
}

//￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣
//			更新処理の中で行う関数
//__________________________________________
//ハコの接線処理
void ConnectSide(D3DXVECTOR3 ParentPos)
{
	for (int ConnectCnt = START_CNT; ConnectCnt < SIZEOF_HAKO; ConnectCnt++)
	{
		//親の位置と最初のハコの位置が違う
		if (ParentPos != g_Hako[ConnectCnt].pos)
		{
			//生まれた方向による位置調整
			switch (g_Hako[ConnectCnt].nDirect)
			{
			case DIRECTION_LEFT:
				g_Hako[ConnectCnt].pos.x = ParentPos.x - MyHAKO_SIZE;	//親の左側に位置調整
				g_Hako[ConnectCnt].pos.y = ParentPos.y;					//Y軸の位置は同じ
				break;

			case DIRECTION_RIGHT:
				g_Hako[ConnectCnt].pos.x = ParentPos.x + MyHAKO_SIZE;	//親の右側に位置調整
				g_Hako[ConnectCnt].pos.y = ParentPos.y;					//Y軸の位置は同じ
				break;

			case DIRECTION_OVER:
				g_Hako[ConnectCnt].pos.x = ParentPos.x;					//X軸の位置は同じ
				g_Hako[ConnectCnt].pos.y = ParentPos.y - MyHAKO_SIZE;	//親の上側に位置調整
				break;

			case DIRECTION_UNDER:
				g_Hako[ConnectCnt].pos.x = ParentPos.x;					//X軸の位置は同じ
				g_Hako[ConnectCnt].pos.y = ParentPos.y + MyHAKO_SIZE;	//親の下側に位置調整
				break;
			}
		}

		//親を更新
		ParentPos = g_Hako[ConnectCnt].pos;
	}
}

//ハコのフェード処理
void FadeHako(void)
{
	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			if (g_Hako[nCntHako].bShade == false)
			{
				//不透明に減少
				g_Hako[nCntHako].nCol -= SHADE_HAKO;

				if (g_Hako[nCntHako].nCol < COLOR_MIN)
				{
					g_Hako[nCntHako].nCol = COLOR_MIN;
					g_Hako[nCntHako].bShade = true;
				}
			}
			else
			{
				//不透明に減少
				g_Hako[nCntHako].nCol += SHADE_HAKO;

				if (g_Hako[nCntHako].nCol > COLOR_MAX)
				{
					g_Hako[nCntHako].nCol = COLOR_MAX;
					g_Hako[nCntHako].bShade = false;
				}
			}
		}
	}
}

//ハコの色修正
void FixHakoCol(void)
{
	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		g_Hako[nCntHako].nCol = COLOR_MAX;			//色の初期化
		g_Hako[nCntHako].bShade = false;			//使用されていない状態に初期化
	}
}

//ハコの初期化
void ResetHako(void)
{
	Player *pPlayer = GetPlayer();		//プレイヤーの状態をノーマルに

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			SetParticle(g_Hako[nCntHako].pos, BIRTH_PARTICLE_HAKO);

			g_Hako[nCntHako].pos = ZERO_SET;			//位置初期化
			g_Hako[nCntHako].posOld = ZERO_SET;			//前回位置初期化
			g_Hako[nCntHako].move = ZERO_SET;			//移動量初期化
			g_Hako[nCntHako].myBlockState = HAKO_NONE;	//何もない状態に
			g_Hako[nCntHako].bUse = false;				//使用されていない状態に
		}
	}
	FixHakoCol();							//ハコの色修正
	if (pPlayer->state != PLAYERSTATE_GOAL)
	{
		pPlayer->state = PLAYERSTATE_NORMAL;	//プレイヤー状態を通常に
	}

	g_nDeliveryCnt = ZERO;					//出産カウンタをゼロにする
	g_bGravity = false;						//重力処理判定初期化
}

//ハコの持ち上げ処理
void LiftUpHako(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得

	if ((GetKeyboardRelease(DIK_RETURN) == true || GetGamePadRelease(XINPUT_GAMEPAD_X) == true) &&
		pPlayer->state == PLAYERSTATE_DELIVERY && g_Hako[ZERO].myBlockState == HAKO_SPAWN)
	{
		for (int nCntHako = START_CNT; nCntHako < MAX_MyBLOCK; nCntHako++)
		{
			if (g_Hako[nCntHako].bUse == true)
			{
				g_Hako[nCntHako].pos.y = g_Hako[nCntHako].pos.y - LIFT_UP;	//ハコの持ち上げ
				g_Hako[nCntHako].myBlockState = HAKO_CARRY;					//ハコ運ばれ中
				pPlayer->state = PLAYERSTATE_CARRY;							//ハコ運び中
			}
		}

		pPlayer->Rot = pPlayer->OldRot;	//前回の向きに修正
		g_nDeliveryCnt = ZERO;			//出産カウンタをゼロにする
	}
}

//ハコの運び処理
void CarryHako(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得
	int nCntHako;

	if (pPlayer->state == PLAYERSTATE_CARRY)
	{
		float ThrowMoveRot = F_ZERO;

		//ハコを投げる
		if (GetKeyboardTrigger(DIK_RETURN) == true ||		//エンター押された
			GetGamePadTrigger(XINPUT_GAMEPAD_X) == true)	//Xボタン押された
		{
			switch (pPlayer->Rot)
			{
			case PLAYER_ROT_LEFT:		//左に投げる
				ThrowMoveRot = -THROW_MOVE;
				break;

			case PLAYER_ROT_RIGHT:		//右に投げる
				ThrowMoveRot = THROW_MOVE;
				break;

			default:
				ThrowMoveRot = F_ZERO;
				break;
			}

			for (nCntHako = START_CNT; nCntHako < MAX_MyBLOCK; nCntHako++)
			{
				if (g_Hako[nCntHako].bUse == true)
				{
					g_Hako[nCntHako].move.x += ThrowMoveRot;	//横に投げる
					g_Hako[nCntHako].move.y += -15.0f;			//上に投げる
					g_Hako[nCntHako].myBlockState = HAKO_SET;	//ハコの所有権を手放す
				}
			}
			g_nDeliveryCnt = ZERO;
			pPlayer->state = PLAYERSTATE_THROW;	//投げるモード
			PlaySound(SOUND_LABEL_THROW);		//投げる音再生
		}

		//ハコを置く
		if (GetKeyboardTrigger(DIK_S) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{
			float fUnload = UNLOAD;		//ハコを落とす高さ	

			//最初のハコを真上に産んだ場合、従来通りに置くと体にめり込んでしまうため、高さを変えずそのまま手放す
			if (g_Hako[ZERO].nDirect == DIRECTION_OVER)
			{
				fUnload = -1.0f;
			}

			for (nCntHako = START_CNT; nCntHako < MAX_MyBLOCK; nCntHako++)
			{
				if (g_Hako[nCntHako].bUse == true)
				{
					g_Hako[nCntHako].pos.y = g_Hako[nCntHako].pos.y + fUnload;	//ハコを足元と同じ高さに置く
					g_Hako[nCntHako].myBlockState = HAKO_SET;					//ハコの所有権を手放す
				}
			}

			g_nDeliveryCnt = ZERO;					//出産カウンタをゼロにする
			pPlayer->state = PLAYERSTATE_NORMAL;	//通常モード
			PlaySound(SOUND_LABEL_SET);				//投げる音再生
		}
	}
}

//ハコの重力処理
void GravityHako(void)
{
	int LandCounter = ZERO;	//ブロックに乗っている数カウンタ

	for (int nCntHako = START_CNT; nCntHako < SIZEOF_HAKO; nCntHako++)
	{
		if (g_Hako[nCntHako].bUse == true)
		{
			//ブロックに乗っている
			if ((CollisionBlockToHako(&g_Hako[nCntHako].pos, &g_Hako[nCntHako].posOld, &g_Hako[nCntHako].move,
				MyHAKO_SIZE, MyHAKO_SIZE, &g_Hako[nCntHako].pBlock) == true))	//別のハコにも乗っていない
			{
				LandCounter++;	//カウンタを増やす
			}
		}
	}

	//一つ以上のハコがブロックに乗っている
	if (LandCounter > ZERO)
	{
		//すべてのハコの移動量をゼロに
		for (int zeroMoveCnt = START_CNT; zeroMoveCnt < SIZEOF_HAKO; zeroMoveCnt++)
		{
			g_Hako[zeroMoveCnt].move = ZERO_SET;
		}

		//重力を加えられなくする
		g_bGravity = true;
	}
	else
	{//重力を加えられるようにする
		g_bGravity = false;
	}
}

//ハコの状態分岐処理
void SwitchHako(int nSwitchNum)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得
	float fCorrection = F_ZERO;
	
	if (g_Hako[ZERO].nDirect == DIRECTION_RIGHT)
	{
		fCorrection = 10.0f;		//右に出すときの出現位置調整
	}

	switch (g_Hako[nSwitchNum].myBlockState)
	{
	case HAKO_NONE:
		break;
	case HAKO_SPAWN:
		//ハコの接線を合わせる
		ConnectSide(D3DXVECTOR3((pPlayer->pos.x - pPlayer->fWidth) + FIX_MARGIN.x + fCorrection,	//原点を左上に合わせる
								 pPlayer->pos.y - MyHAKO_SIZE,	//原点を左上に合わせる
								 pPlayer->pos.z));
		break;
	case HAKO_CARRY:
		//ハコの接線を合わせる
		ConnectSide(D3DXVECTOR3((pPlayer->pos.x - pPlayer->fWidth)  + FIX_MARGIN.x + fCorrection,	//原点を左上に合わせる
								(pPlayer->pos.y - pPlayer->fHeight) + FIX_MARGIN.y,	//原点を左上に合わせる
								 pPlayer->pos.z));
		break;
	case HAKO_SET:
		//重力を加える
		if (g_bGravity == false)
		{
			CalculationGravity(&g_Hako[nSwitchNum].move);
		}

		//ハコの接線を合わせる
		if (nSwitchNum == ZERO)
		{
			ConnectSide(g_Hako[ZERO].pos);
		}

		//位置更新
		g_Hako[nSwitchNum].pos += g_Hako[nSwitchNum].move;

		//横の移動量減少(理想では１ブロック分先に落ちるイメージ)
		g_Hako[nSwitchNum].move.x += (ZERO - g_Hako[nSwitchNum].move.x) * 0.1f;

		//移動量が大分少なくなった
		if (fabsf(g_Hako[nSwitchNum].move.x) < F_ONE)
		{
			g_Hako[nSwitchNum].move.x = F_ZERO;	//移動量をゼロにする
		}
		break;
	}
}

//ハコの情報取得
Hako *GetHako(void)
{
	return &g_Hako[0];	//最初のハコを返す
}

//ハコの産み出し音処理
void HakoSpawnSound(void)
{
	switch (g_nDeliveryCnt)
	{
	case ZERO:
		PlaySound(SOUND_LABEL_HAKO001);
		break;

	case ONE:
		PlaySound(SOUND_LABEL_HAKO002);
		break;

	case SEC:
		PlaySound(SOUND_LABEL_HAKO003);
		break;

	case SAN:
		PlaySound(SOUND_LABEL_HAKO004);
		break;

	case FOUR:
		PlaySound(SOUND_LABEL_HAKO005);
		break;

	}

}