/*==========================================================================================================================

ポーズ画面処理[pause.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "pause.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//マクロ定義
#define NUM_PAUSE		(4)
#define SELECT_COLOR	D3DCOLOR_RGBA(255, 255, 255, 255)	//選択されているときのメニューの色
#define NO_SELECT_COLOR	D3DCOLOR_RGBA(80, 80, 80, 255)		//選択されていないときのメニューの色

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTexturePause[NUM_PAUSE] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				//頂点バッファへのポインタ
PAUSE_MENU g_Pause;											//選択されているメニュー

//ポーズの初期化処理
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/PauseBG.png", &g_pTexturePause[PAUSE_MENU_BG]);	//背景画像
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/continue.png", &g_pTexturePause[PAUSE_MENU_CONTINUE]);	//コンティニュー画像
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/retry.png", &g_pTexturePause[PAUSE_MENU_RETRY]);		//リトライ画像
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/quit.png", &g_pTexturePause[PAUSE_MENU_LOAD]);			//ロード画像

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_PAUSE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//ポーズメニューの諸々の初期化
	g_Pause = PAUSE_MENU_CONTINUE;		//選択されているポーズの状態の初期化

	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++, pVtx += NUM_VERTEX)
	{
		switch (nCntPause)
		{
		case PAUSE_MENU_BG:			//ポーズメニュー背景
		{
			//頂点座標の設定
			{
				pVtx[0].pos = D3DXVECTOR3(200.0f, 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(200.0f, 600.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(1000.0f, 600.0f, 0.0f);
			}

			//rhwの設定
			{
				pVtx[0].rhw = RHW_NUM;
				pVtx[1].rhw = RHW_NUM;
				pVtx[2].rhw = RHW_NUM;
				pVtx[3].rhw = RHW_NUM;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = SELECT_COLOR;
				pVtx[1].col = SELECT_COLOR;
				pVtx[2].col = SELECT_COLOR;
				pVtx[3].col = SELECT_COLOR;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_CONTINUE:	//コンティニューボタン
		{
			//頂点座標の設定
			{
				pVtx[0].pos = D3DXVECTOR3(480.0f, 260.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(720.0f, 260.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(480.0f, 340.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(720.0f, 340.0f, 0.0f);
			}

			//rhwの設定
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = SELECT_COLOR;
				pVtx[1].col = SELECT_COLOR;
				pVtx[2].col = SELECT_COLOR;
				pVtx[3].col = SELECT_COLOR;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_RETRY:		//やり直しボタン
		{
			//頂点座標の設定
			{
				pVtx[0].pos = D3DXVECTOR3(500.0f, 355.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(660.0f, 355.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(500.0f, 445.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(660.0f, 445.0f, 0.0f);
			}

			//rhwの設定
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_LOAD:		//終了ボタン
		{
			//頂点座標の設定
			{
				pVtx[0].pos = D3DXVECTOR3(515.0f, 475.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(645.0f, 475.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(515.0f, 565.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(645.0f, 565.0f, 0.0f);
			}

			//rhwの設定
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();
}

//ポーズの終了処理
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

//ポーズの更新処理
void UpdatePause(void)
{
	//選択されているメニューの更新(下)
	if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//コンティニュー → リトライ
			g_Pause = PAUSE_MENU_RETRY;
			break;

		case PAUSE_MENU_RETRY:				//リトライ → ロード
			g_Pause = PAUSE_MENU_LOAD;
			break;

		case PAUSE_MENU_LOAD:				//ロード → コンティニュー 
			g_Pause = PAUSE_MENU_CONTINUE;
			break;

		default:							//状態をコンティニューに指定
			g_Pause = PAUSE_MENU_CONTINUE;
			break;
		}
	}

	//選択されているメニューの更新(上)
	else if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
	{
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//コンティニュー → ロード
			g_Pause = PAUSE_MENU_LOAD;
			break;

		case PAUSE_MENU_RETRY:				//リトライ→ コンティニュー 
			g_Pause = PAUSE_MENU_CONTINUE;
			break;

		case PAUSE_MENU_LOAD:				//ロード → リトライ
			g_Pause = PAUSE_MENU_RETRY;
			break;

		default:							//状態をコンティニューに指定
			g_Pause = PAUSE_MENU_CONTINUE;
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
	{//ENTERキーが押された
		switch (g_Pause)
		{
		case PAUSE_MENU_CONTINUE:			//ポーズ画面を閉じる
			break;

		case PAUSE_MENU_RETRY:				//リトライ(最初からやり直す)
			//ゲーム画面を最初から
			SetFade(MODE_GAME);
			break;

		case PAUSE_MENU_LOAD:				//ローディング画面に戻る
			SetFade(MODE_LOADING);
			break;
		}
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//メニューのフォントの更新
	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++, pVtx += NUM_VERTEX)
	{
		switch (nCntPause)
		{
		case PAUSE_MENU_BG:			//ポーズメニュー背景
		{
			//頂点座標の設定
			{
				pVtx[0].pos = D3DXVECTOR3(200.0f, 100.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(200.0f, 600.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(1000.0f, 600.0f, 0.0f);
			}

			//rhwの設定
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = SELECT_COLOR;
				pVtx[1].col = SELECT_COLOR;
				pVtx[2].col = SELECT_COLOR;
				pVtx[3].col = SELECT_COLOR;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_CONTINUE:	//コンティニューボタン
		{
			//頂点座標の設定
			{
				pVtx[0].pos = D3DXVECTOR3(480.0f, 260.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(720.0f, 260.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(480.0f, 340.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(720.0f, 340.0f, 0.0f);
			}

			//rhwの設定
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_RETRY:		//やり直しボタン
		{
			//頂点座標の設定
			{
				pVtx[0].pos = D3DXVECTOR3(500.0f, 355.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(660.0f, 355.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(500.0f, 445.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(660.0f, 445.0f, 0.0f);
			}

			//rhwの設定
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;

		case PAUSE_MENU_LOAD:		//終了ボタン
		{
			//頂点座標の設定
			{
				pVtx[0].pos = D3DXVECTOR3(515.0f, 475.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(645.0f, 475.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(515.0f, 565.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(645.0f, 565.0f, 0.0f);
			}

			//rhwの設定
			{
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;
			}

			//頂点カラーの設定
			{
				pVtx[0].col = NO_SELECT_COLOR;
				pVtx[1].col = NO_SELECT_COLOR;
				pVtx[2].col = NO_SELECT_COLOR;
				pVtx[3].col = NO_SELECT_COLOR;
			}

			//テクスチャの座標
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
		break;
		}

		//選択されているメニューを強調する
		if (g_Pause == nCntPause)
		{
			pVtx[0].col = SELECT_COLOR;
			pVtx[1].col = SELECT_COLOR;
			pVtx[2].col = SELECT_COLOR;
			pVtx[3].col = SELECT_COLOR;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffPause->Unlock();
}

//ポーズの描画処理
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = START_CNT; nCntPause < NUM_PAUSE; nCntPause++)
	{
		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * NUM_PAUSE, SEC);
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