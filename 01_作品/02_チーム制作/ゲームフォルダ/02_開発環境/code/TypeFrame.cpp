//==========================================
//
//プレイヤー選択のタイプ枠プログラム[TypeFrame.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "TypeFrame.h"
#include "input.h"
#include "conversioninput.h"
#include "fade.h"
#include "select_game.h"
#include "PvP_player.h"
#include "hdr_player.h"
#include "color.h"
#include "sound.h"
#include "ai.h"

//マクロ
#define TYPEFRAME_USE_TEXTURE	(3)			//タイプ枠で使うテクスチャ数
#define TYPESTR_USE_TEXTURE		(3)			//タイプ文字で使うテクスチャ数
#define AIFRAME_USE_TEXTURE		(1)			//COM難易度枠で使うテクスチャ数
#define AISTR_USE_TEXTURE		(3)			//COM難易度文字で使うテクスチャ数

#define TYPEFRAME_SIZE_WIDTH	(312.0f)	//タイプ枠のサイズ（幅）
#define TYPEFRAME_SIZE_HEIGHT	(108.0f)	//タイプ枠のサイズ（高さ）
#define TYPESTR_OFFSET_WIDTH	(37.0f)		//タイプ枠と文字の位置ずらし（幅）
#define TYPESTR_OFFSET_HEIGHT	(29.0f)		//タイプ枠と文字の位置ずらし（高さ）
#define TYPESTR_SIZE_WIDTH		(238.0f)	//タイプ文字のサイズ（幅）
#define TYPESTR_SIZE_HEIGHT		(50.0f)		//タイプ文字のサイズ（高さ）
#define AIFRAME_SIZE_WIDTH		(324.0f)	//COM難易度枠のサイズ（幅）
#define AIFRAME_SIZE_HEIGHT		(144.0f)		//COM難易度枠のサイズ（高さ）
#define AISTR_SIZE_WIDTH		(284.0f)	//COM難易度文字のサイズ（幅）
#define AISTR_SIZE_HEIGHT		(72.0f)		//COM難易度文字のサイズ（高さ）
#define AISTR_OFFSET_WIDTH		(20.0f)		//タイプ枠と文字の位置ずらし（幅）
#define AISTR_OFFSET_HEIGHT		(60.0f)		//タイプ枠と文字の位置ずらし（高さ）


#define INPUT_JUDGE			(STICK_MAX / 5)	//スティック入力をしたと判断するスティック移動量
#define MIN_PLAYER			(2)				//プレイヤーの最低人数

//プロトタイプ宣言
bool CheckMinPlayer(void);

//グローバル
PLAYERTYPE g_playerType[MAX_USE_GAMEPAD];

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeFrame;					//タイプ枠の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureTypeFrame[TYPEFRAME_USE_TEXTURE];	//タイプ枠のテクスチャポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTypeStr;						//タイプ文字の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureTypeStr[TYPESTR_USE_TEXTURE];		//タイプ文字のテクスチャポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffAIFrame;						//AI枠の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureAIFrame[AIFRAME_USE_TEXTURE];		//AI枠のテクスチャポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffAIStr;						//AI文字の頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureAIStr[AISTR_USE_TEXTURE];			//AI文字のテクスチャポインタ
int g_nSelectNum = 0;

AIDIFF g_SelAIDiff = AIDIFF_NORMAL;

//タイプ枠のファイルパス
const char *c_apFilePathTypeFrame[TYPEFRAME_USE_TEXTURE] =
{
	"data\\TEXTURE\\SelPlayer_Frame_02.png",
	"data\\TEXTURE\\SelPlayer_Frame_01.png",
	"data\\TEXTURE\\SelPlayer_Frame_03.png"
};

//タイプ文字のファイルパス
const char *c_apFilePathTypeStr[TYPESTR_USE_TEXTURE] =
{
	"data\\TEXTURE\\SelPlayer_Str_01.png",
	"data\\TEXTURE\\SelPlayer_Str_02.png",
	"data\\TEXTURE\\SelPlayer_Str_03.png"
};

//AI枠のファイルパス
const char *c_apFilePathAIFrame[AIFRAME_USE_TEXTURE] =
{
	"data\\TEXTURE\\AIDiff_Frame.png"
};

//AI文字のファイルパス
const char *c_apFilePathAIStr[AISTR_USE_TEXTURE] =
{
	"data\\TEXTURE\\AIDiff_Str_01.png",
	"data\\TEXTURE\\AIDiff_Str_02.png",
	"data\\TEXTURE\\AIDiff_Str_03.png"
};

//タイプ枠の位置
const D3DXVECTOR3 c_aPosTypeFrame[MAX_USE_GAMEPAD] =
{
	D3DXVECTOR3(2.5f,550.0f,0.0f),
	D3DXVECTOR3(323.5f,550.0f,0.0f),
	D3DXVECTOR3(634.5f,550.0f,0.0f),
	D3DXVECTOR3(955.5f,550.0f,0.0f)
};

//AI枠の位置
const D3DXVECTOR3 c_PosAIFrame = D3DXVECTOR3(900.0f, 20.0f, 0.0f);

//タイプ枠背景の色
const D3DXCOLOR c_aColTypeFrameBG[MAX_USE_GAMEPAD] =
{
	D3DXCOLOR(1.0f,0.7f,0.7f,1.0f),
	D3DXCOLOR(0.24f,0.72f,0.92f,1.0f),
	D3DXCOLOR(0.72f,0.9f,0.73f,1.0f),
	D3DXCOLOR(1.0f,0.95f,0.67f,1.0f)
};

//========================
//初期化処理
//========================
void InitTypeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;	//設定用ポインタ

	//===============
	//変数初期化
	//===============
	for (int nCntType = 0; nCntType < MAX_USE_GAMEPAD; nCntType++)
	{
		g_playerType[nCntType] = PLAYERTYPE_PLAYER;
	}
	g_SelAIDiff = AIDIFF_NORMAL;
	g_nSelectNum = 0;

	//コントローラーがつながっていなければAIにする
	for (int nCntAI = 1; nCntAI < MAX_USE_GAMEPAD; nCntAI++)
	{
		if (GetUseGamepad(nCntAI) == false)
		{
			g_playerType[nCntAI] = PLAYERTYPE_COM;
		}
	}

	//===============
	//テクスチャ・バッファ生成
	//===============
	//タイプ枠
	//テクスチャ読み込み
	for (int nCntTypeFrame = 0; nCntTypeFrame < TYPEFRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeFrame[nCntTypeFrame],
			&g_pTextureTypeFrame[nCntTypeFrame]);
	}

	//頂点バッファの生成(+1は選択中の枠用）
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_USE_GAMEPAD * (TYPEFRAME_USE_TEXTURE - 1) + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeFrame,
		NULL);

	//タイプ文字
	//テクスチャ読み込み
	for (int nCntTypeStr = 0; nCntTypeStr < TYPESTR_USE_TEXTURE; nCntTypeStr++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathTypeStr[nCntTypeStr],
			&g_pTextureTypeStr[nCntTypeStr]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_USE_GAMEPAD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTypeStr,
		NULL);

	//AI枠
	//テクスチャ読み込み
	for (int nCntAIFrame = 0; nCntAIFrame < AIFRAME_USE_TEXTURE; nCntAIFrame++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathAIFrame[nCntAIFrame],
			&g_pTextureAIFrame[nCntAIFrame]);
	}

	//頂点バッファの生成(+1は選択中の枠用）
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffAIFrame,
		NULL);

	//AI文字
	//テクスチャ読み込み
	for (int nCntAIStr = 0; nCntAIStr < AISTR_USE_TEXTURE; nCntAIStr++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilePathAIStr[nCntAIStr],
			&g_pTextureAIStr[nCntAIStr]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffAIStr,
		NULL);


	//頂点設定

	//タイプ枠
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTypeFrame->Lock(0, 0, (void **)&pVtx, 0);

	//位置固定枠
	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * (TYPEFRAME_USE_TEXTURE - 1); nCntTypeFrame++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].x;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].y;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].x + TYPEFRAME_SIZE_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].y;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].x;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].y + TYPEFRAME_SIZE_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].x + TYPEFRAME_SIZE_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)].y + TYPEFRAME_SIZE_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		//もし偶数（背景）なら色変える
		if (nCntTypeFrame % 2 == 0)
		{
			//頂点カラー
			if (g_playerType[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)] == PLAYERTYPE_PLAYER)
			{//プレイヤーなら色付ける
				pVtx[0].col = c_aColTypeFrameBG[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)];
				pVtx[1].col = c_aColTypeFrameBG[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)];
				pVtx[2].col = c_aColTypeFrameBG[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)];
				pVtx[3].col = c_aColTypeFrameBG[nCntTypeFrame / (TYPEFRAME_USE_TEXTURE - 1)];
			}
			else
			{//COM・使用しないなら灰色
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//位置可変枠
	//頂点座標の設定
	pVtx[0].pos.x = c_aPosTypeFrame[0].x;
	pVtx[0].pos.y = c_aPosTypeFrame[0].y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_aPosTypeFrame[0].x + TYPEFRAME_SIZE_WIDTH;
	pVtx[1].pos.y = c_aPosTypeFrame[0].y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_aPosTypeFrame[0].x;
	pVtx[2].pos.y = c_aPosTypeFrame[0].y + TYPEFRAME_SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_aPosTypeFrame[0].x + TYPEFRAME_SIZE_WIDTH;
	pVtx[3].pos.y = c_aPosTypeFrame[0].y + TYPEFRAME_SIZE_HEIGHT;
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

	//頂点バッファをアンロック
	g_pVtxbuffTypeFrame->Unlock();


	//文字
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTypeStr->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntGaugeFrame = 0; nCntGaugeFrame < MAX_USE_GAMEPAD; nCntGaugeFrame++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + TYPESTR_OFFSET_WIDTH;
		pVtx[0].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + TYPESTR_OFFSET_HEIGHT;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + TYPEFRAME_SIZE_WIDTH - TYPESTR_OFFSET_WIDTH;
		pVtx[1].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + TYPESTR_OFFSET_HEIGHT;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + TYPESTR_OFFSET_WIDTH;
		pVtx[2].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + TYPEFRAME_SIZE_HEIGHT - TYPESTR_OFFSET_HEIGHT;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = c_aPosTypeFrame[nCntGaugeFrame].x + TYPEFRAME_SIZE_WIDTH - TYPESTR_OFFSET_WIDTH;
		pVtx[3].pos.y = c_aPosTypeFrame[nCntGaugeFrame].y + TYPEFRAME_SIZE_HEIGHT - TYPESTR_OFFSET_HEIGHT;
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
	g_pVtxbuffTypeStr->Unlock();

	//AI枠
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffAIFrame->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = c_PosAIFrame.x;
	pVtx[0].pos.y = c_PosAIFrame.y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_PosAIFrame.x + AIFRAME_SIZE_WIDTH;
	pVtx[1].pos.y = c_PosAIFrame.y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_PosAIFrame.x;
	pVtx[2].pos.y = c_PosAIFrame.y + AIFRAME_SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_PosAIFrame.x + AIFRAME_SIZE_WIDTH;
	pVtx[3].pos.y = c_PosAIFrame.y + AIFRAME_SIZE_HEIGHT;
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

	//頂点バッファをアンロック
	g_pVtxbuffAIFrame->Unlock();


	//AI文字
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffAIStr->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = c_PosAIFrame.x + AISTR_OFFSET_WIDTH;
	pVtx[0].pos.y = c_PosAIFrame.y + AISTR_OFFSET_HEIGHT;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_PosAIFrame.x + AISTR_SIZE_WIDTH + AISTR_OFFSET_WIDTH;
	pVtx[1].pos.y = c_PosAIFrame.y + AISTR_OFFSET_HEIGHT;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_PosAIFrame.x + AISTR_OFFSET_WIDTH;
	pVtx[2].pos.y = c_PosAIFrame.y + AISTR_SIZE_HEIGHT + AISTR_OFFSET_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_PosAIFrame.x + AISTR_SIZE_WIDTH + AISTR_OFFSET_WIDTH;
	pVtx[3].pos.y = c_PosAIFrame.y + AISTR_SIZE_HEIGHT + AISTR_OFFSET_HEIGHT;
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

	//頂点バッファをアンロック
	g_pVtxbuffAIStr->Unlock();
}

//========================
//終了処理
//========================
void UninitTypeFrame(void)
{
	//タイプ枠
	//テクスチャの破棄
	for (int nCntTypeFrame = 0; nCntTypeFrame < TYPEFRAME_USE_TEXTURE; nCntTypeFrame++)
	{
		if (g_pTextureTypeFrame[nCntTypeFrame] != NULL)
		{
			g_pTextureTypeFrame[nCntTypeFrame]->Release();
			g_pTextureTypeFrame[nCntTypeFrame] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffTypeFrame != NULL)
	{
		g_pVtxbuffTypeFrame->Release();
		g_pVtxbuffTypeFrame = NULL;
	}


	//タイプ文字
	//テクスチャの破棄
	for (int nCntTypeStr = 0; nCntTypeStr < TYPEFRAME_USE_TEXTURE; nCntTypeStr++)
	{
		if (g_pTextureTypeStr[nCntTypeStr] != NULL)
		{
			g_pTextureTypeStr[nCntTypeStr]->Release();
			g_pTextureTypeStr[nCntTypeStr] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffTypeStr != NULL)
	{
		g_pVtxbuffTypeStr->Release();
		g_pVtxbuffTypeStr = NULL;
	}

	//AI枠
	//テクスチャの破棄
	for (int nCntAIFrame = 0; nCntAIFrame < AIFRAME_USE_TEXTURE; nCntAIFrame++)
	{
		if (g_pTextureAIFrame[nCntAIFrame] != NULL)
		{
			g_pTextureAIFrame[nCntAIFrame]->Release();
			g_pTextureAIFrame[nCntAIFrame] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffAIFrame != NULL)
	{
		g_pVtxbuffAIFrame->Release();
		g_pVtxbuffAIFrame = NULL;
	}


	//AI文字
	//テクスチャの破棄
	for (int nCntAIStr = 0; nCntAIStr < AISTR_USE_TEXTURE; nCntAIStr++)
	{
		if (g_pTextureAIStr[nCntAIStr] != NULL)
		{
			g_pTextureAIStr[nCntAIStr]->Release();
			g_pTextureAIStr[nCntAIStr] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffAIStr != NULL)
	{
		g_pVtxbuffAIStr->Release();
		g_pVtxbuffAIStr = NULL;
	}
}

//========================
//更新処理
//========================
void UpdateTypeFrame(void)
{
	VERTEX_2D *pVtx;	//設定用ポインタ

	//プレイヤー選択
	if (GetUseGamepad(0) == true)
	{
		if (GetGamepadTrigger(0, XINPUT_GAMEPAD_A) == true || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{//次のプレイヤー
			g_nSelectNum = (g_nSelectNum + 1) % MAX_USE_GAMEPAD;
		}
		else if (GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{//戻る
			g_nSelectNum = (g_nSelectNum + (MAX_USE_GAMEPAD - 1)) % MAX_USE_GAMEPAD;
		}

		//タイプ選択
		if (GetStick(0, INPUTTYPE_TRIGGER).y == CONVSTICK_DOWN || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{//下に倒した
			//いったん減らす
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);

			//最低人数を下回るまたはプレイヤー使用していない（1P除く）ならもう一回
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
			else if (g_nSelectNum != 0 && g_playerType[g_nSelectNum] == PLAYERTYPE_PLAYER && GetUseGamepad(g_nSelectNum) == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
		}
		else if (GetStick(0, INPUTTYPE_TRIGGER).y == CONVSTICK_UP || GetGamepadTrigger(0, XINPUT_GAMEPAD_DPAD_UP) == true)
		{//上に倒した
			//いったん減らす
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);

			//最低人数を下回るまたはプレイヤー使用していない（1P除く）ならもう一回
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
			else if (g_nSelectNum != 0 && g_playerType[g_nSelectNum] == PLAYERTYPE_PLAYER && GetUseGamepad(g_nSelectNum) == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
		}

		//AI設定
		if (GetGamepadTrigger(0, XINPUT_GAMEPAD_LEFT_THUMB) == true)
		{
			g_SelAIDiff = (AIDIFF)((g_SelAIDiff + (AIDIFF_VIEW -1)) % AIDIFF_VIEW);
		}
		else if (GetGamepadTrigger(0, XINPUT_GAMEPAD_RIGHT_THUMB) == true)
		{
			g_SelAIDiff = (AIDIFF)((g_SelAIDiff + 1) % AIDIFF_VIEW);
		}

		//決定
		if (GetGamepadTrigger(0, XINPUT_GAMEPAD_START) == true)
		{
			//タイトル決定音再生
			PlaySoundSE(SOUND_LABEL_SE_TITLE_DECIDE, 0);
			if (GetSelGameMode() == SelGameMode_PVP)
			{
				SetFade(MODE_PvPGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_PvP(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
			else if (GetSelGameMode() == SelGameMode_HDR)
			{
				SetFade(MODE_RaceGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_HDR(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
		}
		//戻る
		else if (GetGamepadTrigger(0, XINPUT_GAMEPAD_B) == true)
		{
			SetFade(MODE_SELECTGAME);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_A) == true)
		{//左に倒した
			g_nSelectNum = (g_nSelectNum + (MAX_USE_GAMEPAD - 1)) % MAX_USE_GAMEPAD;
		}
		else if (GetKeyboardTrigger(DIK_D) == true)
		{//右に倒した
			g_nSelectNum = (g_nSelectNum + 1) % MAX_USE_GAMEPAD;
		}

		//タイプ選択
		if (GetKeyboardTrigger(DIK_UP) == true)
		{//下に倒した
			//いったん減らす
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);

			//最低人数を下回るまたはプレイヤー使用していない（1P除く）ならもう一回
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
			else if (g_nSelectNum != 0 && g_playerType[g_nSelectNum] == PLAYERTYPE_PLAYER && GetUseGamepad(g_nSelectNum) == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + (PLAYERTYPE_MAX - 1)) % PLAYERTYPE_MAX);
			}
		}
		else if (GetKeyboardTrigger(DIK_DOWN) == true)
		{//上に倒した
			 //いったん減らす
			g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);

			//最低人数を下回るまたはプレイヤー使用していない（1P除く）ならもう一回
			if (CheckMinPlayer() == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
			else if (g_nSelectNum != 0 && g_playerType[g_nSelectNum] == PLAYERTYPE_PLAYER && GetUseGamepad(g_nSelectNum) == false)
			{
				g_playerType[g_nSelectNum] = (PLAYERTYPE)((g_playerType[g_nSelectNum] + 1) % PLAYERTYPE_MAX);
			}
		}

		//AI設定
		if (GetKeyboardTrigger(DIK_LEFT) == true)
		{
			g_SelAIDiff = (AIDIFF)((g_SelAIDiff + (AIDIFF_VIEW - 1)) % AIDIFF_VIEW);
		}
		else if (GetKeyboardTrigger(DIK_RIGHT) == true)
		{
			g_SelAIDiff = (AIDIFF)((g_SelAIDiff + 1) % AIDIFF_VIEW);
		}

		//決定
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//タイトル決定音再生
			PlaySoundSE(SOUND_LABEL_SE_TITLE_DECIDE, 0);
			if (GetSelGameMode() == SelGameMode_PVP)
			{
				SetFade(MODE_PvPGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_PvP(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
			else if (GetSelGameMode() == SelGameMode_HDR)
			{
				SetFade(MODE_RaceGAME);
				for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
				{
					SetPlayerType_HDR(nCntPlayer, (g_playerType[nCntPlayer] != PLAYERTYPE_NONE ? true : false), (g_playerType[nCntPlayer] == PLAYERTYPE_COM ? true : false));
				}
			}
		}
		//戻る
		else if (GetKeyboardTrigger(DIK_BACKSPACE) == true)
		{
			SetFade(MODE_SELECTGAME);
		}
	}

	//タイプ枠頂点設定
	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTypeFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD; nCntTypeFrame++, pVtx += 8)	//偶数のみ変更のため8つずらしている
	{
		//頂点カラー
		if (g_playerType[nCntTypeFrame] == PLAYERTYPE_PLAYER)
		{//プレイヤーなら色付ける
			pVtx[0].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[1].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[2].col = c_aColTypeFrameBG[nCntTypeFrame];
			pVtx[3].col = c_aColTypeFrameBG[nCntTypeFrame];
		}
		else
		{//COM・使用しないなら灰色
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}

	//位置可変部
	//頂点座標の設定
	pVtx[0].pos.x = c_aPosTypeFrame[g_nSelectNum].x;
	pVtx[0].pos.y = c_aPosTypeFrame[g_nSelectNum].y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = c_aPosTypeFrame[g_nSelectNum].x + TYPEFRAME_SIZE_WIDTH;
	pVtx[1].pos.y = c_aPosTypeFrame[g_nSelectNum].y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = c_aPosTypeFrame[g_nSelectNum].x;
	pVtx[2].pos.y = c_aPosTypeFrame[g_nSelectNum].y + TYPEFRAME_SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = c_aPosTypeFrame[g_nSelectNum].x + TYPEFRAME_SIZE_WIDTH;
	pVtx[3].pos.y = c_aPosTypeFrame[g_nSelectNum].y + TYPEFRAME_SIZE_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロック
	g_pVtxbuffTypeFrame->Unlock();
}

//========================
//描画処理
//========================
void DrawTypeFrame(void)
{
	int nCntTypeFrame;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//タイプ枠
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTypeFrame, 0, sizeof(VERTEX_2D));

	for (nCntTypeFrame = 0; nCntTypeFrame < MAX_USE_GAMEPAD * (TYPEFRAME_USE_TEXTURE - 1); nCntTypeFrame++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureTypeFrame[nCntTypeFrame % (TYPEFRAME_USE_TEXTURE - 1)]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeFrame, 2);
	}

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureTypeFrame[2]);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeFrame, 2);


	//タイプ文字
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTypeStr, 0, sizeof(VERTEX_2D));

	for (int nCntTypeStr = 0; nCntTypeStr < MAX_USE_GAMEPAD; nCntTypeStr++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureTypeStr[g_playerType[nCntTypeStr]]);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTypeStr, 2);
	}

	//AI枠
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffAIFrame, 0, sizeof(VERTEX_2D));

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureAIFrame[0]);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	//AI文字
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffAIStr, 0, sizeof(VERTEX_2D));

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureAIStr[g_SelAIDiff]);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//プレイヤーが最低人数を下回らないか確認する処理
//========================
bool CheckMinPlayer(void)
{
	int nUsePlayer = 0;

	//現在の使用人数確認
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_playerType[nCntPlayer] != PLAYERTYPE_NONE)
		{
			nUsePlayer++;
		}
	}

	//下回らない場合は減らしてもOK
	return nUsePlayer >= MIN_PLAYER ? true : false;
}

//========================
//プレイヤー使用取得処理
//========================
bool GetUsePlayer(int nPlayerNum)
{
	return g_playerType[nPlayerNum] != PLAYERTYPE_NONE ? true : false;
}

//========================
//AI難易度取得処理
//========================
AIDIFF GetAIDiff(void)
{
	return g_SelAIDiff;
}