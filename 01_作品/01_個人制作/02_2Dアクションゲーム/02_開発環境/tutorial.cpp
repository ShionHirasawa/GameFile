/*==========================================================================================================================

チュートリアル処理[tutorial.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "tutorial.h"
#include "player.h"
#include "input.h"
#include "move.h"

//マクロ定義
#define NUM_TUTORIAL		(3)					//チュートリアルの画像数
#define FADE_TUTORIAL		(30)				//αブレンドの変化値
#define UTEX_WIDTH			(0.1f)				//テクスチャのU座標の幅
#define APPEAR_LENGTH		(200.0f)			//画像を表示する距離

//ジャンプチュートリアルのマクロ
#define TUTORIAL_WIDTH		(200.0f)							//チュートリアルの表示幅
#define TUTORIAL_HEIGHT		(200.0f)							//チュートリアルの表示高さ
#define JUMP_POS			(D3DXVECTOR3(900.0f, 400.0f, 0.0f))	//ジャンプチュートリアルの表示場所
#define JUMP_ANIM_CNT		(8)									//ジャンプアニメーション画像の変化量

//ハコ操作チュートリアルのマクロ
#define SPAWN_POS			(D3DXVECTOR3(1700.0f, 200.0f, 0.0f))	//産み出しチュートリアルの表示場所
#define HAKO_ANIM_CNT		(50)									//ジャンプアニメーション画像の変化量

//操作説明チュートリアルのマクロ
#define HOWTO_POS			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//操作説明の表示場所
#define HOWTO_WIDTH			(180.0f)							//操作説明の表示幅
#define HOWTO_HEIGHT		(100.0f)							//操作説明の表示高さ

//チュートリアル画像の種類の構造体
typedef enum
{
	TUTORIAL_JUMP = ZERO,	//ジャンプの説明
	TUTORIAL_HAKO,			//ハコ出しの説明
	TUTORIAL_HOWTO,			//操作説明(不動)
	TUTORIAL_MAX
}TUTORIAL_TEX;

//チュートリアル画像の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int aCol;				//テクスチャのα値
	float TexU;				//テクスチャのU座標
	float UWidth;			//テクスチャ画像の幅
	int nCounterAnim;		//アニメーションカウンタ
	bool bUse;				//使用されているかどうか
}Tutorial;

//ハコ操作アニメーションの構造体
typedef enum
{
	ANIM_HAKO_THROW = ZERO,		//投げる
	ANIM_HAKO_SET,				//置く
	ANIM_HAKO_DELETE,			//消す
	ANIM_HAKO_MAX,
}ANIM_HAKO;

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureTutorial[NUM_TUTORIAL] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTutorial = NULL;				//頂点バッファへのポインタ
Tutorial					g_Tutorial[NUM_TUTORIAL];				//チュートリアルの構造体
int							g_AnimHako;								//ハコ操作アニメーション

//チュートリアルの初期化処理
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ + 取得

	//チュートリアル画像の場所初期化				//チュートリアル画像の透明度初期化				//アニメーションカウンタ初期化
	g_Tutorial[TUTORIAL_JUMP].pos = JUMP_POS;		g_Tutorial[TUTORIAL_JUMP].aCol = COLOR_MIN;		g_Tutorial[TUTORIAL_JUMP].nCounterAnim = ZERO;
	g_Tutorial[TUTORIAL_HAKO].pos = SPAWN_POS;		g_Tutorial[TUTORIAL_HAKO].aCol = COLOR_MIN;		g_Tutorial[TUTORIAL_HAKO].nCounterAnim = ZERO;
	g_Tutorial[TUTORIAL_HOWTO].pos = HOWTO_POS;		g_Tutorial[TUTORIAL_HOWTO].aCol = COLOR_MAX;	g_Tutorial[TUTORIAL_HOWTO].nCounterAnim = ZERO;

	//テクスチャU座標の初期化						//テクスチャ画像の幅初期化						//チュートリアル画像の使用の可否
	g_Tutorial[TUTORIAL_JUMP].TexU = F_ZERO;		g_Tutorial[TUTORIAL_JUMP].UWidth = UTEX_WIDTH;	 g_Tutorial[TUTORIAL_JUMP].bUse = false;
	g_Tutorial[TUTORIAL_HAKO].TexU = F_ZERO; 		g_Tutorial[TUTORIAL_HAKO].UWidth = UTEX_WIDTH;	 g_Tutorial[TUTORIAL_HAKO].bUse = false;
	g_Tutorial[TUTORIAL_HOWTO].TexU = F_ZERO; 		g_Tutorial[TUTORIAL_HOWTO].UWidth = F_TEN;		 g_Tutorial[TUTORIAL_HOWTO].bUse = true;

	//ハコ操作アニメーションの初期化
	g_AnimHako = ANIM_HAKO_THROW;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ジャンプチュートリアル.png", &g_pTextureTutorial[TUTORIAL_JUMP]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ハコチュートリアル.png", &g_pTextureTutorial[TUTORIAL_HAKO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/操作説明.png", &g_pTextureTutorial[TUTORIAL_HOWTO]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_TUTORIAL, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL);

	//頂点座標の設定
	Vertextutorial();
}

//チュートリアルの終了処理
void UninitTutorial(void)
{
	//テクスチャの破棄
	for (int nCntTutorial = START_CNT; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		if (g_pTextureTutorial[nCntTutorial] != NULL)
		{
			g_pTextureTutorial[nCntTutorial]->Release();
			g_pTextureTutorial[nCntTutorial] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//チュートリアルの更新処理
void UpdateTutorial(void)
{
	//チュートリアル画像のフェード処理
	FadeTutorial();

	//ジャンプチュートリアルのアニメーション処理
	AnimJumpTutorial();

	//ハコ操作のチュートリアルのアニメーション処理
	AnimHakoTutorial();

	//頂点座標の設定
	Vertextutorial();
}

//チュートリアルの描画処理
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ + 取得

											//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTutorial, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorial = START_CNT; nCntTutorial < NUM_TUTORIAL; nCntTutorial++)
	{
		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureTutorial[nCntTutorial]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorial * NUM_VERTEX, SEC);
	}
}

//チュートリアル画像のフェード設定
void FadeTutorial(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報

	for (int nCntTutorial = START_CNT; nCntTutorial < TUTORIAL_HOWTO; nCntTutorial++)
	{
		//プレイヤーがチュートリアル画像の表示圏内にいる
		if (fabsf(pPlayer->pos.x - g_Tutorial[nCntTutorial].pos.x) <= 400.0f)
		{			
			g_Tutorial[nCntTutorial].aCol += FADE_TUTORIAL;		//画像を出現させる
			g_Tutorial[nCntTutorial].bUse = true;				//使用されている状態にする

			if (g_Tutorial[nCntTutorial].aCol > COLOR_MAX)
			{
				g_Tutorial[nCntTutorial].aCol = COLOR_MAX;
			}
		}

		else
		{
			//フェードアウトさせていく
			g_Tutorial[nCntTutorial].aCol -= FADE_TUTORIAL;

			if (g_Tutorial[nCntTutorial].aCol < COLOR_MIN)
			{
				g_Tutorial[nCntTutorial].bUse = false;				//使用されていない状態にする
				g_Tutorial[nCntTutorial].aCol = COLOR_MIN;
			}
		}
	}
}

//--------------------------------------------------------------------
//				チュートリアル画像のアニメーション処理
//--------------------------------------------------------------------
//ジャンプチュートリアルのアニメーション処理
void AnimJumpTutorial(void)
{
	//画像が使われている
	if (g_Tutorial[TUTORIAL_JUMP].bUse == true)
	{
		//カウンターを更新
		g_Tutorial[TUTORIAL_JUMP].nCounterAnim++;

		if (g_Tutorial[TUTORIAL_JUMP].nCounterAnim == JUMP_ANIM_CNT)
		{
			//カウンターをゼロにする
			g_Tutorial[TUTORIAL_JUMP].nCounterAnim = ZERO;

			//テクスチャU座標更新
			g_Tutorial[TUTORIAL_JUMP].TexU += F_ONE;

			if (g_Tutorial[TUTORIAL_JUMP].TexU >= F_TEN)
			{
				//テクスチャU座標初期化
				g_Tutorial[TUTORIAL_JUMP].TexU = F_ZERO;
			}
		}
	}
}

//ハコ操作のチュートリアルのアニメーション処理
void AnimHakoTutorial(void)
{
	//画像が使われている
	if (g_Tutorial[TUTORIAL_HAKO].bUse == true)
	{
		//カウンターを更新
		g_Tutorial[TUTORIAL_HAKO].nCounterAnim++;

		if (g_Tutorial[TUTORIAL_HAKO].nCounterAnim == HAKO_ANIM_CNT)
		{
			//カウンターをゼロにする
			g_Tutorial[TUTORIAL_HAKO].nCounterAnim = ZERO;

			//テクスチャU座標更新
			g_Tutorial[TUTORIAL_HAKO].TexU += F_ONE;

			switch (g_AnimHako)
			{
			case ANIM_HAKO_THROW:
				if (g_Tutorial[TUTORIAL_HAKO].TexU > F_FOUR)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_ZERO;	//最初に戻す

					g_AnimHako = ANIM_HAKO_SET;					//ハコ置きアニメーションに変更
				}
				break;

			case ANIM_HAKO_SET:
				//ハコ投げアニメーションを飛ばす
				if (g_Tutorial[TUTORIAL_HAKO].TexU == F_FOUR)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_FIVE;
				}

				if (g_Tutorial[TUTORIAL_HAKO].TexU > F_SIX)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_ZERO;	//最初に戻す

					g_AnimHako = ANIM_HAKO_DELETE;				//ハコ消しアニメーションに変更
				}
				break;

			case ANIM_HAKO_DELETE:
				//ハコ投げ・ハコ置きアニメーションを飛ばす
				if (g_Tutorial[TUTORIAL_HAKO].TexU == F_FOUR)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_SEV;

					//最初のハコ消しアニメーションの表示時間を伸ばす
					g_Tutorial[TUTORIAL_HAKO].nCounterAnim = -HAKO_ANIM_CNT;
				}

				if (g_Tutorial[TUTORIAL_HAKO].TexU > F_TEN)
				{
					g_Tutorial[TUTORIAL_HAKO].TexU = F_ZERO;	//最初に戻す

					g_AnimHako = ANIM_HAKO_THROW;				//ハコ投げアニメーションに変更
				}

				break;
			}
		}
	}
}

//チュートリアルの頂点設定
void Vertextutorial(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntTutorial = START_CNT; nCntTutorial < NUM_TUTORIAL; nCntTutorial++, pVtx += NUM_VERTEX)
	{
		float fWidth, fHeight;		//テクスチャ画像の幅と高さ

		//座標を更新
		if (nCntTutorial < TUTORIAL_HOWTO)
		{
			fWidth = TUTORIAL_WIDTH;
			fHeight = TUTORIAL_HEIGHT;
			moveObject(&g_Tutorial[nCntTutorial].pos);
		}
		else
		{
			fWidth = HOWTO_WIDTH;
			fHeight = HOWTO_HEIGHT;
		}

		//頂点座標の設定
		{
			//X座標の設定
			pVtx[ZERO].pos.x = g_Tutorial[nCntTutorial].pos.x;
			pVtx[ONE].pos.x = g_Tutorial[nCntTutorial].pos.x + fWidth;
			pVtx[SEC].pos.x = g_Tutorial[nCntTutorial].pos.x;
			pVtx[SAN].pos.x = g_Tutorial[nCntTutorial].pos.x + fWidth;

			//Y座標の設定
			pVtx[ZERO].pos.y = g_Tutorial[nCntTutorial].pos.y;
			pVtx[ONE].pos.y = g_Tutorial[nCntTutorial].pos.y;
			pVtx[SEC].pos.y = g_Tutorial[nCntTutorial].pos.y + fHeight;
			pVtx[SAN].pos.y = g_Tutorial[nCntTutorial].pos.y + fHeight;

			//Z座標の設定(変化なし)
			pVtx[ZERO].pos.z = g_Tutorial[nCntTutorial].pos.z;
			pVtx[ONE].pos.z = g_Tutorial[nCntTutorial].pos.z;
			pVtx[SEC].pos.z = g_Tutorial[nCntTutorial].pos.z;
			pVtx[SAN].pos.z = g_Tutorial[nCntTutorial].pos.z;
		}

		//rhwの設定
		{
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;
		}

		//頂点カラーの設定   COLOR_MIN
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Tutorial[nCntTutorial].aCol);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Tutorial[nCntTutorial].aCol);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Tutorial[nCntTutorial].aCol);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Tutorial[nCntTutorial].aCol);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(g_Tutorial[nCntTutorial].TexU,										ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(g_Tutorial[nCntTutorial].TexU + g_Tutorial[nCntTutorial].UWidth,	ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(g_Tutorial[nCntTutorial].TexU,										F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(g_Tutorial[nCntTutorial].TexU + g_Tutorial[nCntTutorial].UWidth,	F_TEN);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffTutorial->Unlock();
}