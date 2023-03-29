//==========================================
//
//ゲーム全般を管理するプログラム[main.cpp]
//Author:石原颯馬
//
//==========================================
#define _CRTDBG_MAP_ALLOC

#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "fade.h"
#include "select_game.h"
#include "pvp_game.h"
#include "HDRgame.h"
#include "title.h"
#include "color.h"
#include "tutorial.h"
#include "light.h"
#include "sound.h"
#include "conversioninput.h"
#include <crtdbg.h>
#include "meshfield.h"
#include "meshdome.h"
#include "result.h"
#include "SelPlayer.h"

//マクロ定義
#define WINDOW_NAME				"Compete Animal"		//ウィンドウに表示される名前
#define PROC_SPEED				(1000 / MAX_FPS)		//実行速度
#define FPS_SPEED				(500)					//FPS計測速度（単位:ミリ秒）
#define SHOWCURSOR_COUNTER		(2)						//カーソル表示非表示が正常にされるカウンタ

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void SetFullScreen(bool bFullScreen);
void ShowTaskBar(bool bShow);
void Uninit(void);
void Update(void);
void Draw(void);
void UpdateShowCursor(void);


//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;	//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//Direct3Dデバイスへのポインタ
int g_nCountFPS;			//FPSカウンタ

#ifdef _DEBUG
MODE			g_mode = MODE_SELECTGAME;		// 現在のモード
#else
MODE			g_mode = MODE_TITLE;		// 現在のモード
#endif

HWND g_hWnd;
bool g_bShowCursor = true;
bool g_bDebug = true;
bool g_bFullScreen = true;
bool g_bShowTaskbar = true;

//========================
//メイン関数
//========================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int CmdShow)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//ウィンドウの領域（書き換え不可の定数）

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//メモリサイズ
		CS_CLASSDC,							//ウィンドウのスタイル
		WindowProc,							//ウィンドウプロシージャ
		0,									//使用しない
		0,									//使用しない
		hInstance,							//インスタンスハンドル
		(HICON)LoadImage(NULL,"data/WildBoar_exeIcon.ico",IMAGE_ICON,0,0,LR_SHARED | LR_LOADFROMFILE),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//クラスの名前
		(HICON)LoadImage(NULL,"data/WildBoar_exeIcon.ico",IMAGE_ICON,0,0,LR_SHARED | LR_LOADFROMFILE)		//アプリケーションのアイコン
	};

	HWND hWnd;	//ウィンドウハンドル
	MSG msg;	//メッセージ格納

	//時刻関係
	DWORD dwCurrentTime;		//現在時刻
	DWORD dwExecLastTime;		//最後に処理した時刻

	//デバッグ（FPS関係）
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(
		0,						//拡張ウィンドウスタイル
		CLASS_NAME,				//ウィンドウクラスの名前
		WINDOW_NAME,			//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,						//ウィンドウの左上X座標
		CW_USEDEFAULT,						//ウィンドウの左上Y座標
		(rect.right - rect.left),			//ウィンドウの幅
		(rect.bottom - rect.top),			//ウィンドウの高さ
		NULL,								//親ウィンドウのハンドル
		NULL,					//メニューハンドルまたは子ウィンドウID
		hInstance,				//インスタンスハンドル
		NULL					//ウィンドウ作成データ
	);

	//初期化
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//全画面表示
	SetFullScreen(g_bFullScreen);

	//分解能設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;		//初期化
	dwExecLastTime = timeGetTime();

	//デバッグ（FPS計測）
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウィンドウの表示
	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);

	//カーソルを消す
	SetShowCursor(true);
	
	//メッセージループ
	while (1)
	{
#if 0	//カーソル非表示に使用していたもの。
		if (GetMousePos().x > 0.0f && GetMousePos().x < SCREEN_WIDTH && GetMousePos().y > 0.0f && GetMousePos().y < SCREEN_HEIGHT)
		{//カーソルがウィンドウの中にある場合はカーソル非表示
			SetShowCursor(false);
		}
		else
		{
			SetShowCursor(true);
		}
#endif

		//カーソル表示非表示更新
		UpdateShowCursor();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= FPS_SPEED)
			{//0.5秒経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= PROC_SPEED)
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;

				//更新処理
				Update();

				//描画処理
				Draw();

				//フレームカウント加算
				dwFrameCount++;
			}
		}
	}

	//カーソル表示
	while (ShowCursor(TRUE) < 0);
	while (ShowCursor(FALSE) > 0);

	Uninit();

	//クラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================
//ウィンドウプロシージャ
//========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			SetShowCursor(true);
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				EndProject();
			}
			SetShowCursor(false);
			break;
		}
		break;
	case WM_CLOSE:
		SetShowCursor(true);
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	//Destroyメッセージを送る
		}
		else
		{
			SetShowCursor(false);
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//========================
//初期化処理
//========================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のスクリーンモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;			//画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			//画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;			//バックバッファの形式
	d3dpp.BackBufferCount = 1;						//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//ダブルバッファの切り替え（同期）
	d3dpp.EnableAutoDepthStencil = TRUE;			//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		//デプスバッファとして16ビット使用
	d3dpp.Windowed = bWindow;						//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

	//Direr3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//ウインドウの情報を格納
	g_hWnd = hWnd;

	//オブジェクトの初期化処理
	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステート設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//サウンド
	InitSound(hWnd);

	//入力系
	//キーボードの初期化
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//マウスの初期化
	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ゲームパッドの初期化
	InitGamePad();
	UpdateGamePad();

	//入力変換の初期化
	InitConvertionInput();

	//デバッグ表示の初期化
	InitDebugProc();

	//オブジェクト初期化
	//フェードの設定
	InitFade(g_mode);

	return S_OK;
}

//========================
//全画面表示処理
//========================
void SetFullScreen(bool bFullScreen)
{
	HDC hDC = GetDC(GetDesktopWindow());			// デスクトップのハンドル
	int nScrSizeX = GetDeviceCaps(hDC, HORZRES);	// デスクトップの画面サイズX
	int nScrSizeY = GetDeviceCaps(hDC, VERTRES);	// デスクトップの画面サイズY

	ReleaseDC(GetDesktopWindow(), hDC);				// デスクトップのハンドルを手放す

	if (bFullScreen)
	{// 全画面フラグが真の時、
	 // ウインドウの位置を設定
		SetWindowPos(
			g_hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// 全画面フラグが偽の時、
	 // ウインドウの位置を設定
		SetWindowPos(
			g_hWnd,
			HWND_TOP,
			(nScrSizeX / 2) - (SCREEN_WIDTH /2),
			(nScrSizeY / 2) - (SCREEN_HEIGHT /2),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(g_hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(g_hWnd);

	// タスクバーを表示/非表示
	ShowTaskBar(!bFullScreen);
}

//========================================
// ShowTaskBar関数 - タスクバーの表示/非表示を切り替える -
//Author：RIKU NISHIMURA
// 参考: https://dixq.net/forum/viewtopic.php?t=12505
//========================================
void ShowTaskBar(bool bShow)
{
	// タスクバーのハンドルを取得
	HWND hTask = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (bShow)
	{// 表示
	 // タスクバーを表示
		ShowWindow(hTask, SW_SHOW);

		// ウインドウをアクティブにする
		SetFocus(g_hWnd);
	}
	else
	{// 非表示
	 // タスクバーを非表示
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================
//終了処理
//========================
void Uninit(void)
{
	//タスクバーを表示
	ShowTaskBar(true);

	//終了処理（自分が作ったものを捨てる）
	UninitTitle();		// タイトルの終了処理
	UninitTutorial();	// チュートリアルの終了処理
	UninitSelectGame();	// ゲーム選択の終了処理
	UninitPvPGame();	// PvPゲームの終了処理
	UninitHDRGame();	// HDRゲームの終了処理
	UninitResult();		// リザルト画面の終了処理

	//フェード終了
	UninitFade();	

	//デバッグ表示の終了
	UninitDebugProc();

	//サウンド
	UninitSound();

	//入力系
	//ゲームパッドの終了
	UninitGamePad();

	//マウスの終了
	UninitMouse();

	//キーボードの終了
	UninitKeyboard();

	//デバイス破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//オブジェクト破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//========================
//更新処理
//========================
void Update(void)
{
	FADE fadeState = GetFade();		//フェード状態取得

	//入力系
	//キーボードの更新
	UpdateKeyboard();
	//マウスの更新
	UpdateMouse();
	//ゲームパッドの更新
	UpdateGamePad();	

	//入力変換の更新
	UpdateConvertionInput();

	//デバッグ表示
	UpdateDebugProc();

	if (fadeState == FADE_NONE)
	{
		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル画面の更新
			UpdateTitle();
			break;

		case MODE_SELECTGAME:	//ゲーム選択の更新処理
			UpdateSelectGame();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面の更新
			UpdateTutorial();
			break;

		case MODE_SELECTPLAYER:	//プレイヤー選択画面の更新
			UpdateSelPlayer();
			break;

		case MODE_PvPGAME:		//PvPゲーム画面の更新
			UpdatePvPGame();
			break;
		case MODE_RaceGAME:		//HDRゲーム画面の更新
			UpdateHDRGame();
			break;

		case MODE_RESULT:
			UpdateResult();		//リザルト画面の更新
			break;
		}
	}

	//フェードの更新処理
	UpdateFade();

	//タスクバーの表示 / 非表示
	ShowTaskBar(g_bShowTaskbar);
}

//========================
//描画処理
//========================
void Draw(void)
{
	D3DVIEWPORT9 viewportDef;

	//画面クリア（バックバッファとZバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), RGBA_WHITE, 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//成功した場合
		//現在のビューポート取得
		g_pD3DDevice->GetViewport(&viewportDef);

		//FPSを文字にして送る
		PrintDebugProc("FPS:%d\n\n", g_nCountFPS);

		/*======================================================================
		各種オブジェクトの描画処理
		========================================================================*/
		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル画面描画
			DrawTitle();
			break;

		case MODE_SELECTGAME:	//ゲーム選択画面描画処理
			DrawSelectGame();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面描画
			DrawTutorial();
			break;

		case MODE_SELECTPLAYER:	//プレイヤー選択画面描画
			DrawSelPlayer();
			break;

		case MODE_PvPGAME:		//PvPゲーム画面描画
			DrawPvPGame();
			break;
		case MODE_RaceGAME:		//HDRゲーム画面の描画
			DrawHDRGame();
			break;

		case MODE_RESULT:		//リザルト画面の描画処理
			DrawResult();
			break;
		}

		//ビューポートを元に戻す
		g_pD3DDevice->SetViewport(&viewportDef);

		//フェードの描画処理
		DrawFade();

		//デバッグ表示
		DrawDebugProc();

		//描画終了処理
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================
//デバイス取得
//========================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//========================
//画面のモード設定
//========================
void SetMode(MODE mode)
{
	//現在の画面（モード）の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面終了
		UninitTitle();
		break;

	case MODE_SELECTGAME:	//ゲーム選択画面終了
		UninitSelectGame();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面終了
		UninitTutorial();
		break;

	case MODE_SELECTPLAYER:	//プレイヤー選択画面終了
		UninitSelPlayer();
		break;

	case MODE_PvPGAME:		//PvPゲーム画面終了
		UninitPvPGame();
		break;

	case MODE_RaceGAME:		//HDRゲーム画面終了
		UninitHDRGame();
		break;

	case MODE_RESULT:
		UninitResult();		//リザルト画面終了
		break;
	}

	//**************************
	//		全サウンド停止
	//**************************
	StopAllSound();

	//新しい画面（モード）の初期化処理
	switch (mode)
	{
	case MODE_TITLE:		//タイトル画面初期化
		InitTitle();
		break;

	case MODE_SELECTGAME:	//ゲーム選択画面初期化
		InitSelectGame();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面初期化
		InitTutorial();
		break;

	case MODE_SELECTPLAYER:	//プレイヤー選択画面初期化
		InitSelPlayer();
		break;

	case MODE_PvPGAME:		//PvPゲーム画面初期化
		InitPvPGame();
		break;

	case MODE_RaceGAME:		//HDRゲーム画面初期化
		InitHDRGame();
		break;

	case MODE_RESULT:		//リザルト画面初期化
		InitResult();
		break;
	}

	g_mode = mode;		//現在のモードを切り替える
}

//========================
//カーソル表示設定
//========================
void SetShowCursor(bool bDisp)
{
	//カーソル表示有無の設定
	g_bShowCursor = bDisp;

	if (g_bShowCursor)
	{
		ShowCursor(TRUE);
		while (ShowCursor(TRUE) <= SHOWCURSOR_COUNTER);
	}
	else
	{
		ShowCursor(FALSE);
		while (ShowCursor(FALSE) > SHOWCURSOR_COUNTER);
	}
}

//========================
//カーソル更新処理（内部カウンタを小さくする）
//========================
void UpdateShowCursor(void)
{
	//小さくする処理
	if (g_bShowCursor)
	{
		while (ShowCursor(FALSE) > SHOWCURSOR_COUNTER);
		ShowCursor(TRUE);
	}
	else
	{
		while (ShowCursor(TRUE) <= -SHOWCURSOR_COUNTER);
		ShowCursor(FALSE);
	}	
}
//========================
//画面モードの取得
//========================
MODE GetMode(void)
{
	return g_mode;
}
//
//
//
void EndProject(void)
{
	DestroyWindow(g_hWnd);	//Destroyメッセージを送る
}