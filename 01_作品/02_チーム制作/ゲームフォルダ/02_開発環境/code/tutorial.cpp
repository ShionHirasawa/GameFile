/*==========================================================================================================================================================

チュートリアル処理[tutorial.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "select_game.h"
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "color.h"
#include "sound.h"

#define TEX_SIZE		(1.0f)	//テクスチャのサイズ
#define TURN_PAGE_SPEED	(4.0f)	//めくるスピード

//チュートリアルのテクスチャの種類　列挙型
typedef enum
{
	TutTexType_HowToPlay = 0,	//操作説明
	TutTexType_PlayRulu,		//ルール説明
	TutTexType_MAX
}TutTexType;

//チュートリアルの構造体
typedef struct
{
	float	fWidth;		//中心位置から右端までの幅
	float	fTurnSpeed;	//めくるスピード
	bool	bTurn;		//めくるどうか（最初はめくっていない
}Tutorial;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureTutorial[TutTexType_MAX] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				//頂点バッファへのポインタ
Tutorial				g_Tutorial[TutTexType_MAX];				//チュートリアルの情報

//ファイルパス
const char* c_aFilePathTutorial[SelGameMode_MAX][TutTexType_MAX] =
{
	{ "data/TEXTURE/PVP_Tutorial000.png", "data/TEXTURE/PVP_Tutorial001.png" },
	{ "data/TEXTURE/HDR_Tutorial000.png", "data/TEXTURE/HDR_Tutorial001.png" },
};

//--------------------------------------------
//			チュートリアル初期化処理
//--------------------------------------------
void InitTutorial(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * TutTexType_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL);

	//テクスチャの読み込み
	for (int nCntTut = 0; nCntTut < TutTexType_MAX; nCntTut++)
	{
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, c_aFilePathTutorial[GetSelGameMode()][nCntTut], &g_pTextureTutorial[nCntTut]);

		g_Tutorial[nCntTut].fWidth = (float)SCREEN_WIDTH;//チュートリアルの頂点座標の幅を初期化
		g_Tutorial[nCntTut].fTurnSpeed = 0.0f;			 //めくるスピード初期化
		g_Tutorial[nCntTut].bTurn = true;				 //めくっていない

		//========================
		//		頂点情報設定
		//========================
		SetVertexInfoTut(nCntTut);
	}

	//チュートリアルBGM再生
	PlaySoundBGM(SOUND_LABEL_BGM_TUTORIAL);
}

//------------------------------------------------
//				チュートリアル終了処理
//------------------------------------------------
void UninitTutorial(void)
{
	//テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < TutTexType_MAX; nCntTexture++)
	{
		if (g_pTextureTutorial[nCntTexture] != NULL)
		{
			g_pTextureTutorial[nCntTexture]->Release();
			g_pTextureTutorial[nCntTexture] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//タイトルBGM停止
	StopSoundBGM(SOUND_LABEL_BGM_TUTORIAL);
}

//------------------------------------------------
//				チュートリアル更新処理
//------------------------------------------------
void UpdateTutorial(void)
{
	for (int nCntPad = 0; nCntPad < MAX_USE_GAMEPAD; nCntPad++)
	{
		//左のページに戻る
		if (GetGamepadTrigger(nCntPad, XINPUT_GAMEPAD_DPAD_LEFT) || GetKeyboardTrigger(DIK_LEFT))
		{
			g_Tutorial[0].bTurn = true;
			break;
		}

		//右のページに進む
		else if(GetGamepadTrigger(nCntPad, XINPUT_GAMEPAD_DPAD_RIGHT) || GetKeyboardTrigger(DIK_RIGHT))
		{
			g_Tutorial[0].bTurn = false;
			break;
		}
	}

	//１ページ目のみをめくる処理
	TurnPageTutorial();

	//頂点情報（１枚目のみ）設定
	SetVertexInfoTut(0);

	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(0, XINPUT_GAMEPAD_A) || GetGamepadTrigger(0, XINPUT_GAMEPAD_START)))
	{
		//モード設定（ゲーム画面に遷移)
		SetFade(MODE_SELECTPLAYER);
	}
}

//１ページ目のみをめくる処理
void TurnPageTutorial(void)
{
	//一枚目のポインタを取得
	Tutorial *pTut = &g_Tutorial[0];

	//右にめくる
	if (pTut->bTurn)
	{
		//めくるスピードを上げる
		pTut->fTurnSpeed += TURN_PAGE_SPEED;
	}

	//左にめくる
	else
	{
		//めくるスピードを上げる
		pTut->fTurnSpeed -= TURN_PAGE_SPEED;
	}

	//めくる
	pTut->fWidth += pTut->fTurnSpeed;

	//幅が、画面の幅を超えた
	if (pTut->fWidth >= SCREEN_WIDTH)
	{
		pTut->fWidth = SCREEN_WIDTH;//幅を画面の幅に戻す
		pTut->fTurnSpeed = 0.0f;	//めくるスピードを０に
	}

	//幅が、画面の幅を超えた
	if (pTut->fWidth <= 0.0f)
	{
		pTut->fWidth = 0.0f;//幅を画面の幅に戻す
		pTut->fTurnSpeed = 0.0f;	//めくるスピードを０に
	}
}

//------------------------------------------------
//				チュートリアル描画処理
//------------------------------------------------
void DrawTutorial(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//一枚目が上になるように後ろから表示
	for (int nCntTex = TutTexType_MAX - 1; 0 <= nCntTex; nCntTex--)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[nCntTex]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * VTX_MAX, 2);
	}
}

//------------------------------------------------
//				頂点情報設定処理
//------------------------------------------------
void SetVertexInfoTut(int nCntTut)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void* *)&pVtx, 0);

	//ポインタを設定
	pVtx += nCntTut * VTX_MAX;

	//========================
	//		頂点情報設定
	//========================
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		pVtx[nCntVtx].pos.x = g_Tutorial[nCntTut].fWidth * (nCntVtx % EVENPARITY);
		pVtx[nCntVtx].pos.y = SCREEN_HEIGHT * (nCntVtx / EVENPARITY);
		pVtx[nCntVtx].pos.z = 0.0f;

		pVtx[nCntVtx].tex.x = TEX_SIZE * (nCntVtx % EVENPARITY);
		pVtx[nCntVtx].tex.y = TEX_SIZE * (nCntVtx / EVENPARITY);

		D3DXVECTOR2 pos = D3DXVECTOR2(pVtx[nCntVtx].pos.x, pVtx[nCntVtx].pos.y);
		D3DXVECTOR2 tex = pVtx[nCntVtx].tex;

		pVtx[nCntVtx].rhw = RHW;
		pVtx[nCntVtx].col = RGBA_WHITE;
	}

	//頂点バッファをロックする
	g_pVtxBuffTutorial->Unlock();
}