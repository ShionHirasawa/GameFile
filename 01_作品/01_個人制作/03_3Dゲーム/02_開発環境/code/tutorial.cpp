/*==========================================================================================================================================================

チュートリアル処理[tutorial.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "color.h"
#include "sound.h"

#define TUTORIAL_MOVE_SPEED	(20.0f)	//チュートリアル画像の移動スピード

//チュートリアル画像の種類分け
typedef enum
{
	TUTO_TEX_PLAYER = ZERO,		//プレイヤーのチュートリアル
	TUTO_TEX_CAMERA,			//カメラ・その他のチュートリアル
	TUTO_TEX_MAX
}TUTO_TEX;

//チュートリアルページの構造体
typedef enum
{
	TutorialPage_LEFT = ZERO,	//左ページにいる
	TutorialPage_TurnLEFT,		//左めくり
	TutorialPage_RIGHT,			//右ページにいる
	TutorialPage_TurnRIGHT,		//右めくり
	TutorialPage_MAX,
}TutorialPage;

//ページ情報の構造体
typedef struct
{
	D3DXVECTOR3 pos;
}Tutorial;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureTutorial[TUTO_TEX_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				//頂点バッファへのポインタ
TutorialPage			g_TutorialPage;							//ページの情報
Tutorial				g_Tutorial[TUTO_TEX_MAX];				//チュートリアルの情報

//チュートリアル画像のテクスチャファイル名
const char *c_apTutorialTexName[TUTO_TEX_MAX] = {
	"data/TEXTURE/tutorial000.png",
	"data/TEXTURE/tutorial001.png",
};

//------------------------------------------------
//				チュートリアル初期化処理
//------------------------------------------------
void InitTutorial(void)
{
	//ページの初期化
	g_TutorialPage = TutorialPage_LEFT;	//左ページ状態に設定

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * TUTO_TEX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//チュートリアル情報の初期化
	for (int nCntTutorial = START_CNT; nCntTutorial < TUTO_TEX_MAX; nCntTutorial++, pVtx += VTX_MAX)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_apTutorialTexName[nCntTutorial], &g_pTextureTutorial[nCntTutorial]);

		g_Tutorial[nCntTutorial].pos = D3DXVECTOR3(SCREEN_WIDTH * nCntTutorial, NIL_F, NIL_F);

		//頂点座標の設定
		//Ⅹ座標の設定
		pVtx[VTX_LE_UP].pos.x = g_Tutorial[nCntTutorial].pos.x;
		pVtx[VTX_RI_UP].pos.x = g_Tutorial[nCntTutorial].pos.x + SCREEN_WIDTH;
		pVtx[VTX_LE_DO].pos.x = g_Tutorial[nCntTutorial].pos.x;
		pVtx[VTX_RI_DO].pos.x = g_Tutorial[nCntTutorial].pos.x + SCREEN_WIDTH;

		//Ｙ座標の設定
		pVtx[VTX_LE_UP].pos.y = NIL_F;
		pVtx[VTX_RI_UP].pos.y = NIL_F;
		pVtx[VTX_LE_DO].pos.y = SCREEN_HEIGHT;
		pVtx[VTX_RI_DO].pos.y = SCREEN_HEIGHT;

		//Z座標の設定
		pVtx[VTX_LE_UP].pos.z = NIL_F;
		pVtx[VTX_RI_UP].pos.z = NIL_F;
		pVtx[VTX_LE_DO].pos.z = NIL_F;
		pVtx[VTX_RI_DO].pos.z = NIL_F;

		//rhwの設定
		pVtx[VTX_LE_UP].rhw = RHW_NUM;
		pVtx[VTX_RI_UP].rhw = RHW_NUM;
		pVtx[VTX_LE_DO].rhw = RHW_NUM;
		pVtx[VTX_RI_DO].rhw = RHW_NUM;

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//テクスチャの座標
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(F_TEN, F_ZERO);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(F_ZERO, F_TEN);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(F_TEN, F_TEN);
	}
	
	//頂点バッファをロックする
	g_pVtxBuffTutorial->Unlock();
}

//------------------------------------------------
//				チュートリアル終了処理
//------------------------------------------------
void UninitTutorial(void)
{
	//テクスチャの破棄
	for (int nCntTex = START_CNT; nCntTex < TUTO_TEX_MAX; nCntTex++)
	{
		if (g_pTextureTutorial[nCntTex] != NULL)
		{
			g_pTextureTutorial[nCntTex]->Release();
			g_pTextureTutorial[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//タイトルBGM停止
	StopSound(SOUND_LABEL_TITLEBGM);
}

//------------------------------------------------
//				チュートリアル更新処理
//------------------------------------------------
void UpdateTutorial(void)
{
	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true &&
		(g_TutorialPage == TutorialPage_LEFT || g_TutorialPage == TutorialPage_RIGHT))	//ページが左右どちらかが表示されている状態である
	{//ENTERキーが押された
		//本を閉じる音を再生
		PlaySound(SOUND_LABEL_SE_PAGE_Close);

		//モード設定（ゲーム画面に遷移)
		SetFade(MODE_GAME);
	}

	//押されていないときにページをめくる
	else
	{
		//ぺーじをめくる処理
		TurnPageTutorial();
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//画像位置更新
	for (int nCntTutorial = START_CNT; nCntTutorial < TUTO_TEX_MAX; nCntTutorial++, pVtx += VTX_MAX)
	{
		//Ⅹ座標の設定
		pVtx[VTX_LE_UP].pos.x = g_Tutorial[nCntTutorial].pos.x;
		pVtx[VTX_RI_UP].pos.x = g_Tutorial[nCntTutorial].pos.x + SCREEN_WIDTH;
		pVtx[VTX_LE_DO].pos.x = g_Tutorial[nCntTutorial].pos.x;
		pVtx[VTX_RI_DO].pos.x = g_Tutorial[nCntTutorial].pos.x + SCREEN_WIDTH;
	}

	//頂点バッファをロックする
	g_pVtxBuffTutorial->Unlock();
}

//ページをめくる
void TurnPageTutorial(void)
{
	/*-------------------------------------------
				ページをめくる判定
	---------------------------------------------*/
	//現在左ページにおり、右入力が押された
	if (g_TutorialPage == TutorialPage_LEFT && (GetKeyboardTrigger(DIK_RIGHT) || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT)) == true)
	{
		//右ページへめくる  音も再生
		g_TutorialPage = TutorialPage_TurnRIGHT;
		PlaySound(SOUND_LABEL_SE_PAGE_FlipRight);
	}

	//現在右ページにおり、左入力が押された
	else if (g_TutorialPage == TutorialPage_RIGHT && (GetKeyboardTrigger(DIK_LEFT) || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_LEFT)) == true)
	{
		//左ページへめくる  音も再生
		g_TutorialPage = TutorialPage_TurnLEFT;
		PlaySound(SOUND_LABEL_SE_PAGE_FlipLeft);
	}

	/*-------------------------------------------
				ページをめくる処理
	---------------------------------------------*/
	//右ページにめくる
	if (g_TutorialPage == TutorialPage_TurnRIGHT)
	{
		//ページを移動させる
		for (int nCntPage = START_CNT; nCntPage < TUTO_TEX_MAX; nCntPage++)
		{
			g_Tutorial[nCntPage].pos.x -= TUTORIAL_MOVE_SPEED;
		}

		//画像の１枚目が画面の広さ分左に行ったか、２枚目が原点位置より左に行ってしまった
		if (g_Tutorial[TUTO_TEX_PLAYER].pos.x <= -SCREEN_WIDTH || g_Tutorial[TUTO_TEX_CAMERA].pos.x <= ORIGIN_POS)
		{
			//右ページにいる
			g_TutorialPage = TutorialPage_RIGHT;

			//それぞれの画像の位置修正
			g_Tutorial[TUTO_TEX_PLAYER].pos.x = -SCREEN_WIDTH;
			g_Tutorial[TUTO_TEX_CAMERA].pos.x = ORIGIN_POS;
		}
	}

	//左ページにめくる
	else if (g_TutorialPage == TutorialPage_TurnLEFT)
	{
		//ページを移動させる
		for (int nCntPage = START_CNT; nCntPage < TUTO_TEX_MAX; nCntPage++)
		{
			g_Tutorial[nCntPage].pos.x += TUTORIAL_MOVE_SPEED;
		}

		//画像の１枚目が原点位置より右に行ったか、２枚目が画面の右端より右に行ってしまったか
		if (g_Tutorial[TUTO_TEX_PLAYER].pos.x >= ORIGIN_POS || g_Tutorial[TUTO_TEX_CAMERA].pos.x >= SCREEN_WIDTH)
		{
			//右ページにいる
			g_TutorialPage = TutorialPage_LEFT;

			//それぞれの画像の位置修正
			g_Tutorial[TUTO_TEX_PLAYER].pos.x = ORIGIN_POS;
			g_Tutorial[TUTO_TEX_CAMERA].pos.x = SCREEN_WIDTH;
		}
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
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTutorial, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = START_CNT; nCntTex < TUTO_TEX_MAX; nCntTex++)
	{
		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureTutorial[nCntTex]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * VTX_MAX, SEC);

	}
}