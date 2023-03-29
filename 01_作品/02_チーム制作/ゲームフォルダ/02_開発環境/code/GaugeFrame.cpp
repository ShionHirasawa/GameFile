//==========================================
//
//ゲージ類の枠関係プログラム[GaugeFrame.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "GaugeFrame.h"
#include "pvp_game.h"
#include "input.h"
#include "model.h"
#include "item.h"
#include "PvP_player.h"

//マクロ
#define GAUGE_FRAME_SIZE_WIDTH			(232)	//プレイヤーのポリゴンサイズ（幅）
#define GAUGE_FRAME_SIZE_HEIGHT			(140)	//プレイヤーのポリゴンサイズ（高さ）
#define ANIMALICON_SIZE_WIDTH			(64)	//アイコンサイズ（幅）
#define ANIMALICON_SIZE_HEIGHT			(64)	//アイコンサイズ（高さ）
#define BUFFICON_SIZE_WIDTH				(48)
#define BUFFICON_SIZE_HEIGHT			(48)
#define ANIMALICON_POS_OFFSET			(4)		//アイコンの位置ずらし
#define BUFFICON_POS_OFFSET_WIDTH		(ANIMALICON_POS_OFFSET + ANIMALICON_SIZE_WIDTH + 8)
#define BUFFICON_POS_OFFSET_HEIGHT		(ANIMALICON_POS_OFFSET + 24)

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffGaugeFrame;					//ゲージ枠の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureGaugeFrame[MAX_USE_GAMEPAD];		//ゲージ枠のテクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffAnimalIcon;					//動物アイコンの頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureAnimalIcon[ANIMAL_MAX];			//動物アイコンのテクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBuffIcon;						//バフアイコンの頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureBuffIcon[ITEMTYPE_MAX];			//バフアイコンのテクスチャポインタ

//ファイルパス
const char* c_apFilePathGaugeFrame[MAX_USE_GAMEPAD] =
{
	"data\\TEXTURE\\PlayerFrame_01.png",
	"data\\TEXTURE\\PlayerFrame_02.png",
	"data\\TEXTURE\\PlayerFrame_03.png",
	"data\\TEXTURE\\PlayerFrame_04.png"
};

const char* c_apFilePathAnimalIcon[ANIMAL_MAX] =
{
	"data\\TEXTURE\\WildBoar_PlayerIcon.png"
};

const char* c_apFilePathBuffIcon[ITEMTYPE_MAX] = 
{
	"data\\TEXTURE\\Power.png",
	"data\\TEXTURE\\Shield.png",
	"data\\TEXTURE\\Ghost.png",
	"data\\TEXTURE\\Invincible.png"
};

const D3DXVECTOR3		c_aPosGaugeFrame[MAX_USE_GAMEPAD] =				//ゲージの位置（およびアイコン位置）
{//参考:X = gaugeのX - 17.0f, Y = gaugeのY - 80.0f
	D3DXVECTOR3(10.0f,210.0f,0.0f),
	D3DXVECTOR3(SCREEN_WIDTH - GAUGE_FRAME_SIZE_WIDTH - 10.0f,210.0f,0.0f),
	D3DXVECTOR3(10.0f,SCREEN_HEIGHT - GAUGE_FRAME_SIZE_HEIGHT - 10.0f,0.0f),
	D3DXVECTOR3(SCREEN_WIDTH - GAUGE_FRAME_SIZE_WIDTH - 10.0f,SCREEN_HEIGHT - GAUGE_FRAME_SIZE_HEIGHT - 10.0f,0.0f),
};

//========================
//プレイヤー初期化処理
//========================
void InitGaugeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//ゲージ枠
	//テクスチャ読み込み
	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathGaugeFrame[nCntGaugeFrame],
			&g_pTextureGaugeFrame[nCntGaugeFrame]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffGaugeFrame,
		NULL);


	//動物アイコン
	//テクスチャ読み込み
	for (int nCntIcon = 0; nCntIcon < ANIMAL_MAX; nCntIcon++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathAnimalIcon[nCntIcon],
			&g_pTextureAnimalIcon[nCntIcon]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffAnimalIcon,
		NULL);


	//バフアイコン
	//テクスチャ読み込み
	for (int nCntIcon = 0; nCntIcon < ITEMTYPE_MAX; nCntIcon++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathBuffIcon[nCntIcon],
			&g_pTextureBuffIcon[nCntIcon]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBuffIcon,
		NULL);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//ゲージ枠
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffGaugeFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = c_aPosGaugeFrame[nCntGaugeFrame].x;
		pVtx[0].pos.y = c_aPosGaugeFrame[nCntGaugeFrame].y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosGaugeFrame[nCntGaugeFrame].x + GAUGE_FRAME_SIZE_WIDTH;
		pVtx[1].pos.y = c_aPosGaugeFrame[nCntGaugeFrame].y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosGaugeFrame[nCntGaugeFrame].x;
		pVtx[2].pos.y = c_aPosGaugeFrame[nCntGaugeFrame].y + GAUGE_FRAME_SIZE_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosGaugeFrame[nCntGaugeFrame].x + GAUGE_FRAME_SIZE_WIDTH;
		pVtx[3].pos.y = c_aPosGaugeFrame[nCntGaugeFrame].y + GAUGE_FRAME_SIZE_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxbuffGaugeFrame->Unlock();


	//動物アイコン
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffAnimalIcon->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = c_aPosGaugeFrame[nCntIcon].x + ANIMALICON_POS_OFFSET;
		pVtx[0].pos.y = c_aPosGaugeFrame[nCntIcon].y + ANIMALICON_POS_OFFSET;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosGaugeFrame[nCntIcon].x + ANIMALICON_SIZE_WIDTH + ANIMALICON_POS_OFFSET;
		pVtx[1].pos.y = c_aPosGaugeFrame[nCntIcon].y + ANIMALICON_POS_OFFSET;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosGaugeFrame[nCntIcon].x + ANIMALICON_POS_OFFSET;
		pVtx[2].pos.y = c_aPosGaugeFrame[nCntIcon].y + ANIMALICON_SIZE_HEIGHT + ANIMALICON_POS_OFFSET;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosGaugeFrame[nCntIcon].x + ANIMALICON_SIZE_WIDTH + ANIMALICON_POS_OFFSET;
		pVtx[3].pos.y = c_aPosGaugeFrame[nCntIcon].y + ANIMALICON_SIZE_HEIGHT + ANIMALICON_POS_OFFSET;
		pVtx[3].pos.z = 0.0f;

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxbuffAnimalIcon->Unlock();


	//バフアイコン
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBuffIcon->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = c_aPosGaugeFrame[nCntIcon].x + BUFFICON_POS_OFFSET_WIDTH;
		pVtx[0].pos.y = c_aPosGaugeFrame[nCntIcon].y + BUFFICON_POS_OFFSET_HEIGHT;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosGaugeFrame[nCntIcon].x + BUFFICON_SIZE_WIDTH + BUFFICON_POS_OFFSET_WIDTH;
		pVtx[1].pos.y = c_aPosGaugeFrame[nCntIcon].y + BUFFICON_POS_OFFSET_HEIGHT;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosGaugeFrame[nCntIcon].x + BUFFICON_POS_OFFSET_WIDTH;
		pVtx[2].pos.y = c_aPosGaugeFrame[nCntIcon].y + BUFFICON_SIZE_HEIGHT + BUFFICON_POS_OFFSET_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosGaugeFrame[nCntIcon].x + BUFFICON_SIZE_WIDTH + BUFFICON_POS_OFFSET_WIDTH;
		pVtx[3].pos.y = c_aPosGaugeFrame[nCntIcon].y + BUFFICON_SIZE_HEIGHT + BUFFICON_POS_OFFSET_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxbuffBuffIcon->Unlock();
}

//========================
//プレイヤー終了処理
//========================
void UninitGaugeFrame(void)
{
	//ゲージ枠
	//テクスチャの破棄
	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		if (g_pTextureGaugeFrame[nCntGaugeFrame] != NULL)
		{
			g_pTextureGaugeFrame[nCntGaugeFrame]->Release();
			g_pTextureGaugeFrame[nCntGaugeFrame] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffGaugeFrame != NULL)
	{
		g_pVtxbuffGaugeFrame->Release();
		g_pVtxbuffGaugeFrame = NULL;
	}


	//動物アイコン
	//テクスチャの破棄
	for (int nCntIcon = 0; nCntIcon < ANIMAL_MAX; nCntIcon++)
	{
		if (g_pTextureAnimalIcon[nCntIcon] != NULL)
		{
			g_pTextureAnimalIcon[nCntIcon]->Release();
			g_pTextureAnimalIcon[nCntIcon] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffAnimalIcon != NULL)
	{
		g_pVtxbuffAnimalIcon->Release();
		g_pVtxbuffAnimalIcon = NULL;
	}


	//バフアイコン
	//テクスチャの破棄
	for (int nCntIcon = 0; nCntIcon < ITEMTYPE_MAX; nCntIcon++)
	{
		if (g_pTextureBuffIcon[nCntIcon] != NULL)
		{
			g_pTextureBuffIcon[nCntIcon]->Release();
			g_pTextureBuffIcon[nCntIcon] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffBuffIcon != NULL)
	{
		g_pVtxbuffBuffIcon->Release();
		g_pVtxbuffBuffIcon = NULL;
	}
}

//========================
//プレイヤー更新処理
//========================
void UpdateGaugeFrame(void)
{
	//無
}

//========================
//プレイヤー描画処理
//========================
void DrawGaugeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	Player *pPlayer = GetPlayer();				//プレイヤー取得

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ゲージ枠
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffGaugeFrame, 0, sizeof(VERTEX_2D));

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureGaugeFrame[nCntGaugeFrame]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGaugeFrame, 2);
	}


	//動物アイコン
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffAnimalIcon, 0, sizeof(VERTEX_2D));

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureAnimalIcon[0]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntIcon, 2);
	}


	//バフアイコン
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffBuffIcon, 0, sizeof(VERTEX_2D));

	for (int nCntIcon = 0; nCntIcon < MAX_USE_GAMEPAD; nCntIcon++, pPlayer++)
	{
		//テクスチャ設定
		if (pPlayer->nATKItemTime > 0)
		{
			pDevice->SetTexture(0, g_pTextureBuffIcon[0]);
		}
		else if (pPlayer->nDEFItemTime > 0)
		{
			pDevice->SetTexture(0, g_pTextureBuffIcon[1]);
		}
		else if (pPlayer->nGhostItemTime > 0)
		{
			pDevice->SetTexture(0, g_pTextureBuffIcon[2]);
		}
		else if (pPlayer->nInvincibleTime > 0)
		{
			pDevice->SetTexture(0, g_pTextureBuffIcon[3]);
		}
		else
		{
			continue;
		}

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntIcon, 2);
	}
}