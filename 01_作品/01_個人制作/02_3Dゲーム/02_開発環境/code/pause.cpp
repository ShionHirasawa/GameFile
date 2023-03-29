/*==========================================================================================================================================================

														ポーズ画面処理[pause.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "color.h"

//マクロ定義
#define NUM_PAUSE		(4)
#define NO_SELECT_COLOR	D3DCOLOR_RGBA(80, 80, 80, 255)		// 選択されていないときのメニューの色

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTexturePause[NUM_PAUSE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				// 頂点バッファへのポインタ
PAUSE_MENU				g_Pause;							// 選択されているメニュー
bool					g_bDispPause;						// ポーズ画面を描画するかどうか

//ポーズメニューの位置
const D3DXVECTOR3 g_PauseMenuPos[NUM_PAUSE] = {
	D3DXVECTOR3(600.0f, 350.0f, NIL_F),			//ポーズメニューの背景位置
	D3DXVECTOR3(600.0f, 300.0f, NIL_F),			//ポーズメニューの"Continue" 位置
	D3DXVECTOR3(580.0f, 400.0f, NIL_F),			//ポーズメニューの"Retry" 位置
	D3DXVECTOR3(580.0f, 520.0f, NIL_F),			//ポーズメニューの"Quit" 位置
};

//ポーズメニューのUIの幅
const float g_PauseMenuWidth[NUM_PAUSE] = {
	400.0f,										//ポーズメニューの背景の幅
	120.0f,										//ポーズメニュー"Continue" の幅
	80.0f,										//ポーズメニュー"Retry" の幅
	65.0f,										//ポーズメニュー"Quit" の幅
};

//ポーズメニューのUIの高さ
const float g_PauseMenuHeight[NUM_PAUSE] = {
	250.0f,										//ポーズメニューの背景の高さ
	40.0f,										//ポーズメニュー"Continue" の高さ
	45.0f,										//ポーズメニュー"Retry" の高さ
	45.0f,										//ポーズメニュー"Quit" の高さ
};

//ポーズメニューに表示するUIのファイル名
const char *c_apPauseTextureName[NUM_PAUSE] = 
{
	"data/TEXTURE/ポーズ画面背景.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/retry.png",
	"data/TEXTURE/quit.png",
};

//------------------------------------------------
//			ポーズメニューの初期化処理
//------------------------------------------------
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_PAUSE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//ポーズメニューの諸々の初期化
	g_Pause = PAUSE_MENU_CONTINUE;		// 選択されているポーズの状態の初期化
	g_bDispPause = true;				// ポーズ画面を描画する

	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++, pVtx += VTX_MAX)
	{
		//テクスチャ画像の読み込み
		D3DXCreateTextureFromFile(pDevice, c_apPauseTextureName[nCntPause], &g_pTexturePause[nCntPause]);

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_PauseMenuPos[nCntPause].x - g_PauseMenuWidth[nCntPause], g_PauseMenuPos[nCntPause].y - g_PauseMenuHeight[nCntPause], NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_PauseMenuPos[nCntPause].x + g_PauseMenuWidth[nCntPause], g_PauseMenuPos[nCntPause].y - g_PauseMenuHeight[nCntPause], NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_PauseMenuPos[nCntPause].x - g_PauseMenuWidth[nCntPause], g_PauseMenuPos[nCntPause].y + g_PauseMenuHeight[nCntPause], NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_PauseMenuPos[nCntPause].x + g_PauseMenuWidth[nCntPause], g_PauseMenuPos[nCntPause].y + g_PauseMenuHeight[nCntPause], NIL_F);
		}

		//rhwの設定
		{
			pVtx[VTX_LE_UP].rhw = RHW_NUM;
			pVtx[VTX_RI_UP].rhw = RHW_NUM;
			pVtx[VTX_LE_DO].rhw = RHW_NUM;
			pVtx[VTX_RI_DO].rhw = RHW_NUM;
		}

		//選択されているメニューと背景を強調する
		if (g_Pause == nCntPause || nCntPause == PAUSE_MENU_BG)
		{
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}

		//選択されていないものは暗くする
		else
		{
			pVtx[0].col = NO_SELECT_COLOR;
			pVtx[1].col = NO_SELECT_COLOR;
			pVtx[2].col = NO_SELECT_COLOR;
			pVtx[3].col = NO_SELECT_COLOR;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = ORIGIN_TEX;
			pVtx[VTX_RI_UP].tex = MA_ZE_TEX;
			pVtx[VTX_LE_DO].tex = ZE_MA_TEX;
			pVtx[VTX_RI_DO].tex = LIMIT_TEX;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();
}

//------------------------------------------------
//			ポーズメニューの終了処理
//------------------------------------------------
void UninitPause(void)
{
	//テクスチャの破棄
	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//------------------------------------------------
//			ポーズメニューの更新処理
//------------------------------------------------
void UpdatePause(void)
{
	//ポーズメニューが表示されている
	if (g_bDispPause == true)
	{
		//選択されているメニューの更新(下)
		SelectPauseMenu_Down();

		//選択されているメニューの更新(上)
		SelectPauseMenu_UP();

		//項目決定
		SelectPauseMenu_Decide();

		//ポインタを設定
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPause->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

		//メニューのUIの更新
		for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++, pVtx += VTX_MAX)
		{
			//選択されているメニューと背景を強調する
			if (g_Pause == nCntPause || nCntPause == PAUSE_MENU_BG)
			{
				pVtx[VTX_LE_UP].col = XCOL_WHITE;
				pVtx[VTX_RI_UP].col = XCOL_WHITE;
				pVtx[VTX_LE_DO].col = XCOL_WHITE;
				pVtx[VTX_RI_DO].col = XCOL_WHITE;
			}

			//選択されていないものは暗くする
			else
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}
		}

		//頂点バッファをロックする
		g_pVtxBuffPause->Unlock();
	}

	//ポーズ画面表示 ON / OFF
	if (GetKeyboardTrigger(DIK_F9) == true)
	{
		g_bDispPause = g_bDispPause ? false : true;
	}
}

//ポーズメニュー下に移動
void SelectPauseMenu_Down(void)
{
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//コンティニュー → リトライ
			g_Pause = PAUSE_MENU_RETRY;
			break;

		case PAUSE_MENU_RETRY:				//リトライ → ロード
			g_Pause = PAUSE_MENU_TITLE;
			break;

		case PAUSE_MENU_TITLE:				//ロード → コンティニュー 
			g_Pause = PAUSE_MENU_CONTINUE;
			break;

		default:							//状態をコンティニューに指定
			g_Pause = PAUSE_MENU_CONTINUE;
			break;
		}
	}
}

//ポーズメニュー上に移動
void SelectPauseMenu_UP(void)
{
	if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//コンティニュー → タイトル
			g_Pause = PAUSE_MENU_TITLE;
			break;

		case PAUSE_MENU_RETRY:				//リトライ→ コンティニュー 
			g_Pause = PAUSE_MENU_CONTINUE;
			break;

		case PAUSE_MENU_TITLE:				//タイトル → リトライ
			g_Pause = PAUSE_MENU_RETRY;
			break;

		default:							//状態をコンティニューに指定
			g_Pause = PAUSE_MENU_CONTINUE;
			break;
		}
	}
}

//ポーズメニュー決定
void SelectPauseMenu_Decide(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
	{//ENTERキーが押された
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:		//ポーズ画面を閉じる
			break;

		case PAUSE_MENU_RETRY:			//リトライ(最初からやり直す)
			SetFade(MODE_GAME);			//ゲーム画面を最初から
			break;

		case PAUSE_MENU_TITLE:			//タイトル画面に戻る
			SetFade(MODE_TITLE);
			break;
		}
	}
}

//------------------------------------------------
//			ポーズメニューの描画処理
//------------------------------------------------
void DrawPause(void)
{
	//ポーズ画面の描画ON　なら描画する
	if (g_bDispPause == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(ZERO, g_pVtxBuffPause, ZERO, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++)
		{
			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTexturePause[nCntPause]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * VTX_MAX, SEC);
		}
	}
}

//ポーズ状態の取得
PAUSE_MENU *GetPause(void)
{
	return &g_Pause;
}

//ポーズ状態の設定
void SetPause(PAUSE_MENU SetPause)
{
	g_Pause = SetPause;
}

//ポーズ画面を表示するかどうか
bool GetDispPause(void)
{
	return g_bDispPause;	//表示スイッチを返す
}