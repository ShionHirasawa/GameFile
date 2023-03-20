/*==========================================================================================================================

シューティングゲーム[main.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"
#include "tutorial.h"
#include "pause.h"
#include "ranking.h"

//マクロ定義
#define CLASS_NAME		"WindowClass"			//ウインドウクラスの名前
#define WINDOW_NAME		"SPINING STAR"			//ウインドウの名前（キャプションに表示）

#define HALF_TIME		(500)					//0.5秒
#define SEN_TIMES		(1000)					//数値を1000倍にするときに使用
#define ONE_FLAME		(1000 / 60)				//１フレーム
#define MAX_MOJI		(256)					//デバッグ用に使用する一行の文字の限界数

//グローバル変数
LPDIRECT3D9			g_pD3D = NULL;				//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;		//Direct3Dデバイスへのポインタ
LPD3DXFONT			g_pFont = NULL;				//フォントへのポインタ
int					g_nCountFPS = ZERO;			//FPSカウンタ
int					g_FPS_IO_CNT = ONE;			//FPSを表示するかどうかのカウンタ

#if _DEBUG
MODE g_mode = MODE_GAME;
#else
MODE g_mode = MODE_TITLE;						//現在のモード
#endif

//==========================================================================================================================
//                                                     メイン関数
//==========================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;						//現在時刻
	DWORD dwExecLastTime;						//最後に処理した時刻
	DWORD dwFrameCount;							//フレームカウント
	DWORD dwFPSLastTime;						//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								//ウインドウのスタイル
		WindowProc,								//ウインドウプロシージャ
		NULL,									//０にする（通常は使用しない）
		NULL,									//０にする（通常は使用しない）
		hInstance,								//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),			//タスクバーのアイコン
		LoadIcon(NULL,IDI_APPLICATION),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + ONE),			//クライアント領域の背景色
		NULL,									//メニューバー
		CLASS_NAME,								//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),			//ファイルのアイコン
	};

	HWND hWnd;									//ウインドウハンドル（識別子）
	MSG  mSg;									//メッセージを格納する変数

	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };	//画面サイズの構造体

																//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(ZERO,						//拡張ウインドウスタイル
		CLASS_NAME,									//ウインドウクラスの名前
		WINDOW_NAME,								//ウインドウの名前
		WS_OVERLAPPEDWINDOW,						//ウインドウスタイル
		CW_USEDEFAULT,								//ウインドウの左上Ｘ座標
		CW_USEDEFAULT,								//ウインドウの左上Ｙ座標
		(rect.right - rect.left),					//ウインドウの幅
		(rect.bottom - rect.top),					//ウインドウの高さ
		NULL,										//親ウインドウのハンドル
		NULL,										//メニューハンドルまたは子ウインドウID
		hInstance,									//インスタントハンドル
		NULL);										//ウインドウ作成データ

													//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);						//ウインドウの表示状態を設定

	UpdateWindow(hWnd);								//クライアント領域を更新

													//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗したら
		return MINUS_ONE;
	}

	//分解能を設定
	timeBeginPeriod(ONE);

	//描画処理関連の関数の初期化
	dwCurrentTime = ZERO;
	dwExecLastTime = timeGetTime();

	//FPS関連の関数の初期化
	dwFrameCount = ZERO;
	dwFPSLastTime = timeGetTime();

	//メッセージループ
	while (ONE)
	{
		if (PeekMessage(&mSg, NULL, ZERO, ZERO, PM_REMOVE) != ZERO)
		{//windowsの処理
			if (mSg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&mSg);					//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&mSg);					//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= HALF_TIME)
			{//0.5秒経過
			 //FPSを計測
				g_nCountFPS = (dwFrameCount * SEN_TIMES) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPSを測定した時刻を保存
				dwFrameCount = ZERO;					//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= ONE_FLAME)
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存

													//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;						//フレームカウントを加算
			}
		}
	}

	//終了処理
	Uninit();

	//ウインドウクラスの登録の解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)mSg.wParam;

	return ZERO;
}

//ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;												//返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY:										//ウインドウ破棄のメッセージ
															//WM_QUITメッセージを送る
		PostQuitMessage(ZERO);
		break;

	case WM_KEYDOWN:										//キー押下のメッセージ
		switch (wParam)
		{
		case VK_F3:
			g_FPS_IO_CNT = g_FPS_IO_CNT ^ ONE;
			break;

		case VK_ESCAPE:										//[ESC]キーが押された
															//終了するかの確認
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				MessageBox(NULL, "また来てくださいね～", "見送り", MB_OK);
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
				break;
			}
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//既定の処理を返す
}

//初期化処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;				//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;		//プレゼンテーションパラメータ

										//DirectX3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現存のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面のサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面のサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

																	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	/*======================================================================
	各種オブジェクトの初期化処理
	========================================================================*/
	//レンダーステート
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//テクスチャの繰り返しの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//サウンドの初期化処理
	InitSound(hWnd);

	//フェードの設定
	InitFade(g_mode);

	return S_OK;
}

//終了処理
void Uninit(void)
{
	//分解能を戻す
	timeEndPeriod(ONE);

	/*======================================================================
	各種オブジェクトの終了処理
	========================================================================*/
	UninitTitle();		//タイトル終了
	UninitTutorial();	//チュートリアル終了
	UninitGame();		//ゲーム終了
	UninitResult();		//リザルト終了
	UninitRanking();	//ランキングの終了処理
	UninitKeyboard();	//キーボードの終了処理
	StopSound();		//サウンドの停止
	UninitSound();		//サウンドの終了処理
	UninitFade();		//フェードの終了処理

						//表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//更新処理
void Update(void)
{
	UpdateKeyboard();
	FADE fadeState = GetFade();

	if (fadeState == FADE_NONE)
	{
		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル画面の更新
			UpdateTitle();
			break;
		case MODE_TUTORIAL:		//チュートリアル画面の更新
			UpdateTutorial();
			break;
		case MODE_GAME:			//ゲーム画面の更新
			UpdateGame();
			break;
		case MODE_RESULT:		//リザルト画面の更新
			UpdateResult();
			break;
		case MODE_RANKING:		//ランキング画面の更新
			UpdateRanking();
			break;
		}
	}

	//フェードの更新処理
	UpdateFade();
}

//描画処理
void Draw(void)
{
	//画面クリア(バックバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(ZERO, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(ZERO, ZERO, ZERO, ZERO), F_TEN, ZERO);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		/*======================================================================
		各種オブジェクトの描画処理
		========================================================================*/
		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル画面描画
			DrawTitle();
			break;
		case MODE_TUTORIAL:		//チュートリアル画面描画
			DrawTutorial();
			break;
		case MODE_GAME:			//ゲーム画面描画
			DrawGame();
			DrawFPS();
			break;
		case MODE_RESULT:		//リザルト画面描画
			DrawResult();
			break;
		case MODE_RANKING:		//ランキング画面描画
			DrawRanking();
			break;
		}

		//フェードの描画処理
		DrawFade();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//FPSの表示
void DrawFPS(void)
{
#if _DEBUG
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];

	wsprintf(&aStr[ZERO], "---操作方法---");

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX));

	wsprintf(&aStr[ZERO], "\n上方向 : [Ｗ]\n下方向 : [Ｓ]\n左方向 : [Ａ]\n右方向 : [Ｄ]\n弾発射 : [SPACE]", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX));

	if (g_FPS_IO_CNT == ONE)		//FPSの表示
	{
		//基本情報表示
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\nFPS:%d    [F3]:表示ON/OFF\n[F5]:ホーミングON/OFF\n通常弾：水色\n倍速度：赤色\n３連弾：白色\nビーム：緑", g_nCountFPS);

		//テキストの描画
		g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX));

		Player *pPlayer = GetPlayer();		//プレイヤーの状態(ホーミング性能)の取得
		PAUSE_MENU *pMenu = GetPause();		//ポーズ状態の取得[pause.cppより

											//プレイヤーの弾のホーミング性能について
		switch (pPlayer->homingtype)
		{
		case HOMING_POSSIBLE:
			wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nホーミング性能:アリ");
			break;

		case HOMING_IMPOSSIBLE:
			wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nホーミング性能:ナシ");
			break;
		}

		//テキストの描画
		g_pFont->DrawText(NULL, &aStr[ZERO], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX));

		//ポーズメニューの表示
		switch (*pMenu)
		{
		case PAUSE_MENU_CONTINUE:
			wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nポーズ:コンティニュー");
			break;

		case PAUSE_MENU_RETRY:
			wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nポーズ:リトライ");
			break;

		case PAUSE_MENU_QUIT:
			wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nポーズ:終了");
			break;
		}

		//テキストの描画
		g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX));
	}
#endif
}

//デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//モードの設定
void SetMode(MODE mode)
{
	//現在の画面（モード）の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面終了
		UninitTitle();
		break;
	case MODE_TUTORIAL:		//チュートリアル画面終了
		UninitTutorial();
		StopSound(SOUND_LABEL_BGM000);	//タイトル画面のBGMを停止
		break;
	case MODE_GAME:			//ゲーム画面終了
		UninitGame();
		break;
	case MODE_RESULT:		//リザルト画面終了
		UninitResult();
		break;
	case MODE_RANKING:		//ランキング画面終了
		UninitRanking();
		StopSound(SOUND_LABEL_BGM001);	//ゲーム画面のBGMを停止
		break;
	}

	//新しい画面（モード）の初期化処理
	switch (mode)
	{
	case MODE_TITLE:		//タイトル画面初期化
		InitTitle();
		break;
	case MODE_TUTORIAL:		//チュートリアル画面初期化
		InitTutorial();
		break;
	case MODE_GAME:			//ゲーム画面初期化
		InitGame();
		break;
	case MODE_RESULT:		//リザルト画面初期化
		InitResult();
		break;
	case MODE_RANKING:		//ランキング画面初期化
		InitRanking();
		break;
	}

	g_mode = mode;		//現在のモードを切り替える
}

//モードの取得
MODE GetMode(void)
{
	return g_mode;
}