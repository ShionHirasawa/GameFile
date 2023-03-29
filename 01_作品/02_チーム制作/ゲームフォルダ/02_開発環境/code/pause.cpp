//==========================================
//
//ポーズプログラム[pause.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "PvP_player.h"
#include "HDRgame.h"
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "color.h"
#include "sound.h"
#include "conversioninput.h"

//****************************//
//		　 マクロ定義		  //
//****************************//

//テクスチャの情報
#define NUM_TEXTURE				(7)									//テクスチャの最大数

//各座標
#define CENTER_POS_MENU			(D3DXVECTOR3(640.0f,360.0f,0.0f))	//メニューの中心の位置
#define CENTER_POS_TEXT			(D3DXVECTOR3(640.0f,440.0f,0.0f))	//テキストの中心の位置
#define CENTER_POS_DIRE_UP		(D3DXVECTOR3(640.0f,340.0f,0.0f))	//ボタンの中心の位置
#define CENTER_POS_DIRE_DOWN	(D3DXVECTOR3(640.0f,540.0f,0.0f))	//ボタンの中心の位置

//画面の情報
#define HALF_WIDTH				(640.0f)							//半分の横幅
#define HALF_HEIGHT				(360.0f)							//半分の高さ

//テキストの情報
#define TEXT_WIDTH				(200.0f)							//テキストの横幅
#define TEXT_HEIGHT				(140.0f)								//テキストの高さ

//ボタンの情報
#define NUM_BUTTON				(2)									//ボタンの数
#define COUNT_BUTTON			(15)								//ボタンのカウント
#define DIRE_WIDTH_UP			(40.0f)								//上ボタンの幅
#define DIRE_HEIGHT_UP			(40.0f)								//上ボタンの高さ
#define DIRE_WIDTH_DOWN			(-40.0f)							//下ボタンの幅
#define DIRE_HEIGHT_DOWN		(-40.0f)							//下ボタンの高さ

//****************************//
//		テクスチャのパス	  //
//****************************//
const char *c_pFileNamePause[] =
{
	"data/TEXTURE/pause.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/restart.png",
	"data/TEXTURE/modeselect.png",
	"data/TEXTURE/quit.png",
	"data/TEXTURE/sankaku.png",
	"data/TEXTURE/sankaku.png"
};

//****************************//
//		  各UIの情報	      //
//****************************//

//各UIの座標
const D3DXVECTOR3 g_CenterPos[] = {
	CENTER_POS_MENU,								//ポーズ画面の位置
	CENTER_POS_TEXT,								//コンテニューの位置
	CENTER_POS_TEXT,								//リトライの位置
	CENTER_POS_TEXT,								//リトライの位置
	CENTER_POS_TEXT,								//終了の位置
	CENTER_POS_DIRE_UP,								//上ボタンの位置
	CENTER_POS_DIRE_DOWN							//下ボタンの位置
};

//各UIの幅
const float g_Width[] = {
	HALF_WIDTH,										//ポーズ画面の幅
	TEXT_WIDTH,										//コンテニューの幅
	TEXT_WIDTH,										//リトライの幅
	TEXT_WIDTH,										//リトライの幅
	TEXT_WIDTH,										//終了の幅
	DIRE_WIDTH_UP,									//上ボタンの幅
	DIRE_WIDTH_DOWN									//下ボタンの幅
};

//各UIの高さ
const float g_Height[] = {
	HALF_HEIGHT,									//ポーズ画面の高さ
	TEXT_HEIGHT,									//コンテニューの高さ
	TEXT_HEIGHT,									//リトライの高さ
	TEXT_HEIGHT,									//リトライの高さ
	TEXT_HEIGHT,									//終了の高さ
	DIRE_HEIGHT_UP,									//上ボタンの高さ
	DIRE_HEIGHT_DOWN								//下ボタンの高さ
};

//****************************//
//	    プロトタイプ宣言      //
//****************************//
void SwitchPause(void);

//****************************//
//	   グローバル変数宣言     //
//****************************//
LPDIRECT3DTEXTURE9		g_pTexturePause[NUM_TEXTURE] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//頂点バッファへのポインタ
PAUSE g_Pause;												//ポーズの状態
Pause g_aPause[NUM_TEXTURE];								//ポーズの情報
int g_nCnt[NUM_BUTTON];										//カウント用変数
bool g_ButtonPush[NUM_BUTTON];								//ボタンが押されたかどうか
int g_GamePad;
bool g_bDisconnectPause = false;
MODE g_gamemode;

//=================================
//ポーズの初期化処理
//=================================
void InitPause(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_TEXTURE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_pFileNamePause[nCnt], &g_pTexturePause[nCnt]);
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//コンテニューに設定
	g_Pause = PAUSE_CONTINUE;

	//変数初期化
	g_bDisconnectPause = false;

	//初期ボタン設定
	for (int nCnt = 0; nCnt < NUM_BUTTON; nCnt++)
	{
		//ボタンを押してない状態にする
		g_ButtonPush[nCnt] = false;

		//ボタンのカウントを設定
		g_nCnt[nCnt] = COUNT_BUTTON;
	}

	//各情報読み込み
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++,pVtx += VTX_MAX)
	{
		//位置の設定
		g_aPause[nCnt].pos = g_CenterPos[nCnt];

		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x - g_Width[nCnt], g_aPause[nCnt].pos.y - g_Height[nCnt], NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x + g_Width[nCnt], g_aPause[nCnt].pos.y - g_Height[nCnt], NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x - g_Width[nCnt], g_aPause[nCnt].pos.y + g_Height[nCnt], NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_aPause[nCnt].pos.x + g_Width[nCnt], g_aPause[nCnt].pos.y + g_Height[nCnt], NIL_F);

		//rhwの設定
		pVtx[VTX_LE_UP].rhw = RHW;
		pVtx[VTX_RI_UP].rhw = RHW;
		pVtx[VTX_LE_DO].rhw = RHW;
		pVtx[VTX_RI_DO].rhw = RHW;

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//テクスチャの座標
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();
}
//=================================
//ポーズの終了処理
//=================================
void UninitPause(void) 
{
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//=================================
//ポーズの更新処理
//=================================
void UpdatePause(MODE mode)
{
	g_gamemode = mode;

	//ポーズの要因が切断されたものである（全員に操作権）
	if (g_bDisconnectPause == true)
	{
		for (int nCntPad = 0; nCntPad < MAX_USE_GAMEPAD; nCntPad++)
		{
			//ポーズの上方向選択処理
			SelectUpPause(nCntPad);

			//ポーズの下方向選択処理
			SelectDownPause(nCntPad);

			//ポーズのUIの状態変化処理
			UIStatePause();

			if (GetGamepadTrigger(nCntPad, XINPUT_GAMEPAD_START) == true)
			{//STARTボタン（ポーズ解除）
				g_Pause = PAUSE_CONTINUE;
				SwitchPause();
			}
			if (GetGamepadTrigger(nCntPad, XINPUT_GAMEPAD_A) == true)
			{//Aボタン（選択）
				SwitchPause();
			}
		}
	}
	else
	{//意図してポーズした（操作権は押した人）
	 //ポーズの上方向選択処理
		SelectUpPause(g_GamePad);

		//ポーズの下方向選択処理
		SelectDownPause(g_GamePad);

		//ポーズのUIの状態変化処理
		UIStatePause();

		if (GetGamepadTrigger(g_GamePad, XINPUT_GAMEPAD_START) == true)
		{//STARTボタン（ポーズ解除）
			g_Pause = PAUSE_CONTINUE;
			SwitchPause();
		}
		if (GetGamepadTrigger(g_GamePad, XINPUT_GAMEPAD_A) == true)
		{//Aボタン（選択）
			SwitchPause();
		}
	}

	//キーボードの操作は受け付ける
	if (GetKeyboardTrigger(DIK_P) == true)
	{//Pキー（ポーズ解除）
		g_Pause = PAUSE_CONTINUE;
		SwitchPause();
	}
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//ENTERキー（選択）
		SwitchPause();
	}
}
//=================================
//ポーズの描画処理
//=================================
void DrawPause(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * VTX_MAX, 2);
	}
}
//=================================
//ポーズの上方向選択処理
//=================================
void SelectUpPause(int nPadNum)
{
	//キーボードの上方向キー　か　ゲームパッドの　十字上ボタン　が押された
	if (GetKeyboardTrigger(DIK_UP) == true || GetGamepadTrigger(nPadNum,XINPUT_GAMEPAD_DPAD_UP) == true || GetStick(nPadNum,INPUTTYPE_TRIGGER).y == CONVSTICK_UP)
	{
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_SELECTION, nPadNum);

		//ボタンを使用している状態にする
		g_ButtonPush[0] = true;

		//カウントの再設定
		g_nCnt[0] = COUNT_BUTTON;

		//ポインタを設定
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += VTX_MAX * 5;

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = XCOL_YELLOW;
		pVtx[VTX_RI_UP].col = XCOL_YELLOW;
		pVtx[VTX_LE_DO].col = XCOL_YELLOW;
		pVtx[VTX_RI_DO].col = XCOL_YELLOW;

		//頂点バッファをロックする
		g_pVtxBuffPause->Unlock();

		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			//終了ボタンに設定
			g_Pause = PAUSE_QUIT;

			break;

		case PAUSE_RETRY:

			//続けるボタンに設定
			g_Pause = PAUSE_CONTINUE;

			break;

		case PAUSE_MODESELECT:

			//リトライボタンに設定
			g_Pause = PAUSE_RETRY;

			break;

		case PAUSE_QUIT:

			//モード選択ボタンに設定
			g_Pause = PAUSE_MODESELECT;

			break;
		}
	}
}
//=================================
//ポーズの下方向選択処理
//=================================
void SelectDownPause(int nPadNum)
{
	//キーボードの下方向キー　か　ゲームパッドの　十字下ボタン　が押された
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetGamepadTrigger(nPadNum, XINPUT_GAMEPAD_DPAD_DOWN) == true || GetStick(nPadNum, INPUTTYPE_TRIGGER).y == CONVSTICK_DOWN)
	{
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_SELECTION, nPadNum);

		//ボタンを使用している状態にする
		g_ButtonPush[1] = true;

		//カウントの再設定
		g_nCnt[1] = COUNT_BUTTON;

		//ポインタを設定
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += VTX_MAX * 6;

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = XCOL_YELLOW;
		pVtx[VTX_RI_UP].col = XCOL_YELLOW;
		pVtx[VTX_LE_DO].col = XCOL_YELLOW;
		pVtx[VTX_RI_DO].col = XCOL_YELLOW;

		//頂点バッファをロックする
		g_pVtxBuffPause->Unlock();

		switch (g_Pause)
		{
		case PAUSE_CONTINUE:

			//リトライボタンに設定
			g_Pause = PAUSE_RETRY;

			break;

		case PAUSE_RETRY:

			//モード選択ボタンに設定
			g_Pause = PAUSE_MODESELECT;

			break;

		case PAUSE_MODESELECT:

			//終了ボタンに設定
			g_Pause = PAUSE_QUIT;

			break;

		case PAUSE_QUIT:

			//続けるボタンに設定
			g_Pause = PAUSE_CONTINUE;

			break;
		}
	}
}
//=================================
//ポーズのUIの状態変化処理
//=================================
void UIStatePause(void)
{
	//色の変更のカウントダウン
	for (int nCnt = 0; nCnt < NUM_BUTTON; nCnt++)
	{
		if (g_ButtonPush[nCnt] == true)
		{
			g_nCnt[nCnt]--;
		}
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void* *)&pVtx, 0);

	//選択項目の表示と矢印の色変更
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++, pVtx += VTX_MAX)
	{
		if (g_Pause == nCnt)
		{
			//頂点カラーの設定
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}
		else if (nCnt != 0 && nCnt <= 4)
		{
			//頂点カラーの設定
			pVtx[VTX_LE_UP].col = XCOL_NONE;
			pVtx[VTX_RI_UP].col = XCOL_NONE;
			pVtx[VTX_LE_DO].col = XCOL_NONE;
			pVtx[VTX_RI_DO].col = XCOL_NONE;
		}
		else if (nCnt >= 5)
		{
			if (g_nCnt[0] <= 0 && nCnt == 5)
			{
				//白に設定
				pVtx[VTX_LE_UP].col = RGBA_WHITE;
				pVtx[VTX_RI_UP].col = RGBA_WHITE;
				pVtx[VTX_LE_DO].col = RGBA_WHITE;
				pVtx[VTX_RI_DO].col = RGBA_WHITE;

				//ボタンを使用していない状態にする
				g_ButtonPush[0] = false;

				//カウントの再設定
				g_nCnt[0] = COUNT_BUTTON;
			}
			else if (g_nCnt[1] <= 0 && nCnt == 6)
			{
				//白に設定
				pVtx[VTX_LE_UP].col = RGBA_WHITE;
				pVtx[VTX_RI_UP].col = RGBA_WHITE;
				pVtx[VTX_LE_DO].col = RGBA_WHITE;
				pVtx[VTX_RI_DO].col = RGBA_WHITE;

				//ボタンを使用していない状態にする
				g_ButtonPush[1] = false;

				//カウントの再設定
				g_nCnt[1] = COUNT_BUTTON;
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();
}
//=================================
//ポーズの設定処理
//=================================
void SetPause(PAUSE Pause)
{
	g_Pause = Pause;
}
//=================================
//ポーズのゲームパッド設定処理
//=================================
void SetPadPause(bool bDisconect, int GamePad)
{
	g_GamePad = GamePad;
	g_bDisconnectPause = bDisconect;
}

//=================================
//ポーズの取得
//=================================
PAUSE *GetPause(void)
{
	return &g_Pause;
}

//=================================
//ポーズ選択の分岐
//Author:石原颯馬
//=================================
void SwitchPause(void)
{
	switch (g_Pause)
	{
	case PAUSE_CONTINUE:
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_DECISION, 0);

		//再確認したけどコントローラーの数合ってない
		if (CheckUseController_PvP(CHECKMODE_DISCONNOPAUSE) == true)
		{
			//警告メッセージ

			if (/*じゃいいですぅ*/false)
			{
				break;
			}
		}
		RestartSound(SOUND_LABEL_BGM_GAME_PVP);
		CheckUseController_PvP(CHECKMODE_REMOVE);
		SetEnablePause_PvP(false);
		SetEnablePause_HDR(false);
		break;

	case PAUSE_RETRY:
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_TRANSITION, 0);

		switch (g_gamemode)
		{
		case MODE_PvPGAME:

				//モード設定（ゲーム画面に遷移)
				SetFade(MODE_PvPGAME);

				break;

		case MODE_RaceGAME:

			//モード設定（ゲーム画面に遷移)
			SetFade(MODE_RaceGAME);

			break;
		}

		break;

	case PAUSE_MODESELECT:

		//ゲーム選択画面に遷移
		SetFade(MODE_SELECTGAME);

		break;

	case PAUSE_QUIT:
		PlaySoundSE(SOUND_LABEL_SE_PAUSE_TRANSITION, 0);

		//タイトル画面に遷移
		SetFade(MODE_TITLE);
		break;
	}
}