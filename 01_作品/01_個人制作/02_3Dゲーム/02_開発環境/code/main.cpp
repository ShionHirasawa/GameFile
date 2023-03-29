/*==========================================================================================================================================================

															メイン処理[main.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "camera.h"
#include "color.h"
#include "model.h"
#include "player.h"
#include "wall.h"
#include "billboard.h"
#include "meshfield.h"
#include "slope.h"
#include "slopewall.h"
#include "title.h"
#include "fade.h"
#include "game.h"
#include "pause.h"
#include "result.h"
#include "sound.h"
#include "tutorial.h"

//マクロ定義
#define CLASS_NAME		"WindowClass"						//ウインドウクラスの名前
#define WINDOW_NAME		"Road To The TOP"					//ウインドウの名前（キャプションに表示）

#define HALF_TIME		(500)								//0.5秒
#define SEN_TIMES		(1000)								//数値を1000倍にするときに使用
#define ONE_FLAME		(1000 / 60)							//１フレーム
#define SEC_FPS_WIDTH	(1000.0f)							//2つ目のFPS表示に使用する開始位置
#define BACKGROUND_COL	(D3DCOLOR_RGBA(170, 250, 255, 255))	//背景の色

//グローバル変数
LPDIRECT3D9			g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;		// Direct3Dデバイスへのポインタ
LPD3DXFONT			g_pFont = NULL;				// フォントへのポインタ
int					g_nCountFPS = ZERO;			// FPSカウンタ

#if _DEBUG
	MODE			g_mode = MODE_GAME;		// 現在のモード
#else
	MODE			g_mode = MODE_TITLE;		// 現在のモード
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
		LoadIcon(NULL,IDC_ARROW),				//マウスカーソル
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

	//描画処理関連の変数の初期化
	dwCurrentTime = ZERO;
	dwExecLastTime = timeGetTime();

	//FPS関連の変数の初期化
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
				dwFrameCount = ZERO;				//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= ONE_FLAME)
			{//60分の1秒経過
				//処理開始の時刻[現在時刻]を保存
				dwExecLastTime = dwCurrentTime;		

				//更新処理
				Update();

				//描画処理
				Draw();

				//フレームカウントを加算
				dwFrameCount++;
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

//--------------------------------------------------------
//						初期化処理
//--------------------------------------------------------
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

	/*===========================================================================
							各種オブジェクトの初期化処理						
	============================================================================*/
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
	D3DXCreateFont(g_pD3DDevice, 18, ZERO, ZERO, ZERO, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
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

//--------------------------------------------------------
//						終了処理
//--------------------------------------------------------
void Uninit(void)
{
	//分解能を戻す
	timeEndPeriod(ONE);

	/*======================================================================
							各種オブジェクトの終了処理
	========================================================================*/
	UninitTitle();		// タイトル画面終了
	UninitTutorial();	// チュートリアル画面終了
	UninitGame();		// ゲーム終了
	UninitResult();		// リザルト画面終了
	UninitKeyboard();	// キーボードの終了処理
	StopSound();		// サウンドの停止
	UninitSound();		// サウンドの終了処理
	UninitFade();		// フェードの終了処理
	XInputEnable(false);// Xinputを閉じる

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

//--------------------------------------------------------
//						更新処理
//--------------------------------------------------------
void Update(void)
{
	FADE fadeState = GetFade();		//フェード状態取得

	UpdateKeyboard();				//キーボードの更新処理

#if 1

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
		}
	}

	//フェードの更新処理
	UpdateFade();
#endif
}

//--------------------------------------------------------
//						描画処理
//--------------------------------------------------------
void Draw(void)
{
	//画面クリア(バックバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(ZERO, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), BACKGROUND_COL, F_TEN, ZERO);

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
			break;

		case MODE_RESULT:		//リザルト画面描画
			DrawResult();
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

//--------------------------------------------------------
//						FPS表示処理
//--------------------------------------------------------
void DrawFPS(PLAYMODE mode)
{
#if /*_DEBUG*/1
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	BillTest *pAlphaTest = GetAlphaTest();		//αテストの値を表示

	wsprintf(&aStr[ZERO], "\n\n--- [ 操作方法 ] ---    FPS:%d\n[ F2 ] : プレイモード変更\n[ F6 ] : プレイモード : 全体", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	//αテストの状態表示
	if (pAlphaTest->bAlphaTest == true)
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nαテストの値:%d\nαテスト:有効", pAlphaTest->AlphaTest);
	}
	else
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nαテストの値:%d\nαテスト:無効", pAlphaTest->AlphaTest);
	}

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	//Zテストの状態表示
	if (pAlphaTest->bZTest == true)
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                Zテスト:有効");
	}
	else
	{
		wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                Zテスト:無効");
	}

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	//プレイモード描画
	switch (mode)
	{
	case PLAYMODE_MODEL:
		DrawFPSPlayer();	//プレイヤーの情報表示
		break;

	case PLAYMODE_FIELD:
		DrawFPSField();		//地面の情報表示
		break;

	case PLAYMODE_WALL:
		DrawFPSWall();		//壁の情報表示
		break;

	case PLAYMODE_STAGE:
		DrawFPSStage();		//壁の情報表示
		break;

	case PLAYMODE_CAMERA:
		DrawFPSCamera();	//カメラの情報表示
		break;

	case PLAYMODE_MAX:
		DrawFPSALL();	//全体情報表示
		break;
	}
#endif
}

//プレイヤーの情報表示
void DrawFPSPlayer(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	Player *pPlayer = GetPlayer();
	int X, Y, Z, x, y, z;

	wsprintf(&aStr[ZERO], "プレイモード：モデル\n\n\n\n\n\n--- [ 移動 ] ---\n [ A ] : 左\n [ D ] : 右\n [ W ] : 奥\n [ S ] : 手前\n [ Y ] : 上昇\n [ N ] : 下降");

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-------------------------------
			プレイヤーの移動量
	--------------------------------*/

	X = pPlayer->move.x;
	Y = pPlayer->move.y;
	Z = pPlayer->move.z;

	x = fabsf(pPlayer->move.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->move.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->move.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ プレイヤーの移動量 ] ---\n    X の移動量 : %d.%d \n    Y の移動量 : %d.%d \n    Z の移動量 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pPlayer->rot.x / 1;
	Y = pPlayer->rot.y / 1;
	Z = pPlayer->rot.z / 1;

	x = fabsf(pPlayer->rot.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->rot.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->rot.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n[ L SHIFT ] : 左回転\n[ R SHIFT ] : 右回転\n[  ENTER  ]  : モデル位置リセット\n\n\n\n\n\n\n--- [ プレイヤーの向き ] ---\n    X の向き : %d.%d \n    Y の向き : %d.%d \n    Z の向き : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pPlayer->pos.x / 1;
	Y = pPlayer->pos.y / 1;
	Z = pPlayer->pos.z / 1;

	x = fabsf(pPlayer->pos.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->pos.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->pos.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ プレイヤーの位置 ] ---\n    X の位置 : %d.%d \n    Y の位置 : %d.%d \n    Z の位置 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

}

//フィールド情報表示
void DrawFPSField(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];

	//壁の情報表示
	wsprintf(&aStr[ZERO], "プレイモード：フィールド\n\n\n\n\n\n現在操作不可");

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
}

//壁情報表示
void DrawFPSWall(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	Wall *pWall = GetWall();	//壁の情報
	int X, Y, Z, x, y, z;

	//壁の情報表示
	wsprintf(&aStr[ZERO], "プレイモード：壁\n\n\n\n\n\n現在操作不可");

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
}

//ステージ情報
void DrawFPSStage(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	int X, Y, Z, x, y, z;
	Wall *pWall			= GetWall();		//壁の情報
	MeshField *pField	= GetMeshField();	//メッシュフィールドの情報

	X = pField->rot.x / 1;		x = fabsf(pField->rot.x * 100.0f) - fabsf(X * 100.0f);
	Y = pField->rot.y / 1;		y = fabsf(pField->rot.y * 100.0f) - fabsf(Y * 100.0f);
	Z = pField->rot.z / 1;		z = fabsf(pField->rot.z * 100.0f) - fabsf(Z * 100.0f);

	//壁の情報表示
	wsprintf(&aStr[ZERO], "プレイモード：ステージ\n\n\n\n\n\n現在操作不可\n左 / 右回転 : ← / →\n--- [ フィールドの向き ] --- \n    X の向き : %d.%d \n    Y の向き : %d.%d \n    Z の向き : %d.%d", X, x, Y, y, Z, z);

	X = pWall->pos.x / 1;		x = fabsf(pWall->pos.x * 100.0f) - fabsf(X * 100.0f);
	Y = pWall->pos.y / 1;		y = fabsf(pWall->pos.y * 100.0f) - fabsf(Y * 100.0f);		//壁のポインタを次の壁へ
	Z = pWall->pos.z / 1;		z = fabsf(pWall->pos.z * 100.0f) - fabsf(Z * 100.0f);		pWall++;

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	//壁の情報表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ 壁 1の座標 ] --- \n    X の位置 : %d.%d \n    Y の位置 : %d.%d \n    Z の位置 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pWall->pos.x / 1;		x = fabsf(pWall->pos.x * 100.0f) - fabsf(X * 100.0f);
	Y = pWall->pos.y / 1;		y = fabsf(pWall->pos.y * 100.0f) - fabsf(Y * 100.0f);		//壁のポインタを次の壁へ
	Z = pWall->pos.z / 1;		z = fabsf(pWall->pos.z * 100.0f) - fabsf(Z * 100.0f);		pWall++;

	//壁の情報表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ 壁 2の座標 ] --- \n    X の位置 : %d.%d \n    Y の位置 : %d.%d \n    Z の位置 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pWall->pos.x / 1;		x = fabsf(pWall->pos.x * 100.0f) - fabsf(X * 100.0f);
	Y = pWall->pos.y / 1;		y = fabsf(pWall->pos.y * 100.0f) - fabsf(Y * 100.0f);		//壁のポインタを次の壁へ
	Z = pWall->pos.z / 1;		z = fabsf(pWall->pos.z * 100.0f) - fabsf(Z * 100.0f);		pWall++;

	//壁の情報表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ 壁 3の座標 ] --- \n    X の位置 : %d.%d \n    Y の位置 : %d.%d \n    Z の位置 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pWall->pos.x / 1;		x = fabsf(pWall->pos.x * 100.0f) - fabsf(X * 100.0f);
	Y = pWall->pos.y / 1;		y = fabsf(pWall->pos.y * 100.0f) - fabsf(Y * 100.0f);
	Z = pWall->pos.z / 1;		z = fabsf(pWall->pos.z * 100.0f) - fabsf(Z * 100.0f);

	//壁の情報表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ 壁 4の座標 ] --- \n    X の位置 : %d.%d \n    Y の位置 : %d.%d \n    Z の位置 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
}

//カメラ情報表示
void DrawFPSCamera(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	Camera *pCamera = GetCamera();
	int X, Y, Z, x, y, z;

	//前後左右移動 上昇 下降 方法表示
	wsprintf(&aStr[ZERO], "プレイモード：カメラ\n\n\n\n\n\n\n Z / C : 視点旋回\n\n  視点上昇 I / SPACE\n 下降 k / L SHIFT\n\nENTER : カメラ位置リセット");

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

#if _DEBUG
	X = pCamera->rot.x / 1;
	Y = pCamera->rot.y / 1;
	Z = pCamera->rot.z / 1;

	x = fabsf(pCamera->rot.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pCamera->rot.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pCamera->rot.z * 100.0f) - fabsf(Z * 100.0f);

	//カメラの向き表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ カメラの向き ] ---\n    X の向き : %d.%d \n    Y の向き : %d.%d \n    Z の向き : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pCamera->posV.x / 1;
	Y = pCamera->posV.y / 1;
	Z = pCamera->posV.z / 1;

	x = fabsf(pCamera->posV.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pCamera->posV.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pCamera->posV.z * 100.0f) - fabsf(Z * 100.0f);

	//視点の座標 表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ 視点の座標 ] --- \n    X の位置 : %d.%d \n    Y の位置 : %d.%d \n    Z の位置 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pCamera->posR.x / 1;
	Y = pCamera->posR.y / 1;
	Z = pCamera->posR.z / 1;

	x = fabsf(pCamera->posR.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pCamera->posR.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pCamera->posR.z * 100.0f) - fabsf(Z * 100.0f);

	//注視点の座標 表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ 注視点の座標 ] --- \n    X の位置 : %d.%d \n    Y の位置 : %d.%d \n    Z の位置 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	X = pCamera->fLength / ONE;
	Y = X * 100.0f;
	Z = pCamera->fLength * 100.0f;
	x = Z - Y;

	//注視点の座標 表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ 視点間の距離 ] ---\n距離:%d.%d", X, x);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
#endif

}

//全体情報表示
void DrawFPSALL(void)
{
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];
	Camera *pCamera = GetCamera();
	Player *pPlayer = GetPlayer();
	int X, Y, Z, x, y, z;

	/*-----------------------------
	プレイヤーの向き
	------------------------------*/
	X = pPlayer->rot.x;
	Y = pPlayer->rot.y;
	Z = pPlayer->rot.z;

	x = fabsf(pPlayer->rot.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->rot.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->rot.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "プレイモード：すべて\n\n\n\n\n\n--- [ プレイヤーの向き ] ---\n    X の向き : %d.%d \n    Y の向き : %d.%d \n    Z の向き : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
		プレイヤーの移動量
	------------------------------*/

	X = pPlayer->move.x;
	Y = pPlayer->move.y;
	Z = pPlayer->move.z;

	x = fabsf(pPlayer->move.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pPlayer->move.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pPlayer->move.z * 100.0f) - fabsf(Z * 100.0f);

	wsprintf(&aStr[ZERO], "プレイモード：すべて\n\n\n\n\n\n\n\n\n\n\n--- [ プレイヤーの移動量 ] ---\n    X の移動量 : %d.%d \n    Y の移動量 : %d.%d \n    Z の移動量 : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
			カメラの向き
	------------------------------*/

	X = pCamera->rot.x / 1,
	Y = pCamera->rot.y / 1,
	Z = pCamera->rot.z / 1;

	x = fabsf(pCamera->rot.x * 100.0f) - fabsf(X * 100.0f);
	y = fabsf(pCamera->rot.y * 100.0f) - fabsf(Y * 100.0f);
	z = fabsf(pCamera->rot.z * 100.0f) - fabsf(Z * 100.0f);

	//カメラの向き表示
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ カメラの向き ] ---\n    X の向き : %d.%d \n    Y の向き : %d.%d \n    Z の向き : %d.%d", X, x, Y, y, Z, z);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
			プレイヤー操作説明
	------------------------------*/
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ プレイヤーの操作説明 ] ---\n[ 移動 ] : A / D / W / S\n[ 上昇 ] : SPACE\n[ 下降 ] : L SHIFT\n[ 発射 ] : J");

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
			カメラ操作説明
	------------------------------*/
	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ カメラの操作説明 ] ---\n[  視点  上昇 / 下降 ] : I / K\n[  視点  旋回 ] : Z / C");

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);

	/*-----------------------------
			共通操作説明
	------------------------------*/

	wsprintf(&aStr[ZERO], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--- [ 共通操作 ] ---\n[ 上昇 ] : SPACE\n[ 下降 ] : TAB\n[ 初期化 ] : ENTER");

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, RGBA_WHITE);
}

//2つ目のFPS表示(右側に描画)
void DrawFPSSecond(PLAYMODE mode)
{

}

//デバイスの取得
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//モードの設定
void SetMode(MODE mode)
{
#if 1
	//現在の画面（モード）の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面終了
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面終了
		UninitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面終了
		UninitGame();
		break;

	case MODE_RESULT:		//リザルト画面終了
		UninitResult();
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

	case MODE_RESULT:		//リザルト画面終了
		InitResult();
		break;
	}

	g_mode = mode;		//現在のモードを切り替える
#endif
}

//モードの取得
MODE GetMode(void)
{
	return g_mode;
}

//外積計算
void VectorProduct(float ParentX, float ChildZ, float ParentZ, float ChildX, float *pAnswer, float fMagnification)
{
	*pAnswer = ((ParentX * ChildZ) - (ParentZ * ChildX)) * fMagnification;
}

//内積計算
void InnerProduct(float Rot, D3DXVECTOR3 vecMove, D3DXVECTOR3 *pMove)
{
	//法線ベクトルを設定
	D3DXVECTOR3 vecNor = D3DXVECTOR3(sinf(Rot), 0.0f, cosf(Rot));
	
	//法線ベクトルを正規化する
	D3DXVec3Normalize(&vecNor, &vecNor);

	//内積の大きさを求める
	float fDot = D3DXVec3Dot(&vecMove, &vecNor);

	//反射ベクトルを求めて、その数値を代入する
	*pMove = vecMove - TWICE * fDot * vecNor;
}