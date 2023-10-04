//================================================================================================
//
//メイン処理[main.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "main.h"
#include "manager.h"
#include "renderer.h"

//マクロ定義
#define WINDOW_NAME	"Hide"	//ウインドウの名前（キャプションに表示）

#define HALF_SEC	(500)		//0.5秒
#define ONE_FLAME	(1000 / 60)	//１フレーム

//----------------------------
//フルスクリーン設定
//引数１	bFull：フルスクリーン切り替え（フルスクリーンにする場合、タスクバーを消す）　true:フルスク　false:通常ウィンドウ
//引数２	hWnd ：ウィンドウステータス情報
//----------------------------
void SetFullScreen(bool bFull, HWND hWnd);

//========================================
// ShowTaskBar関数 - タスクバーの表示/非表示を切り替える -
//----------------------------
//引数１	bShow：タスクバー表示ステータス　true:表示する　false:非表示にする
//引数２	hWnd ：ウィンドウステータス情報
// 参考: https://dixq.net/forum/viewtopic.php?t=12505
//----------------------------
void ShowTaskBar(bool bShow, HWND hWnd);

//======================================================================================================================
//														メイン関数
//======================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//メモリリークを起こしているか判定する（exe を閉じるとリークしているメモリ数が分かる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int		nCountFPS = 0;		// FPSカウンタ
	CManager *pManager = NULL;	//マネちゃん生成
	DWORD dwCurrentTime;		//現在時刻
	DWORD dwExecLastTime;		//最後に処理した時刻
	DWORD dwFrameCount;			//フレームカウント
	DWORD dwFPSLastTime;		//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						//ウインドウのスタイル
		WindowProc,						//ウインドウプロシージャ
		NULL,							//０にする（通常は使用しない）
		NULL,							//０にする（通常は使用しない）
		hInstance,						//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	//タスクバーのアイコン
		LoadIcon(NULL,IDC_ARROW),		//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		//クライアント領域の背景色
		NULL,							//メニューバー
		CLASS_NAME,						//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),	//ファイルのアイコン
	};

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//画面サイズの構造体
	RegisterClassEx(&wcex);								//ウインドウクラスの登録

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	const HWND hWnd = CreateWindowEx(0,//拡張ウインドウスタイル
		CLASS_NAME,					//ウインドウクラスの名前
		WINDOW_NAME,				//ウインドウの名前
		WS_OVERLAPPEDWINDOW,		//ウインドウスタイル
		CW_USEDEFAULT,				//ウインドウの左上Ｘ座標
		CW_USEDEFAULT,				//ウインドウの左上Ｙ座標
		(rect.right - rect.left),	//ウインドウの幅
		(rect.bottom - rect.top),	//ウインドウの高さ
		NULL,						//親ウインドウのハンドル
		NULL,						//メニューハンドルまたは子ウインドウID
		hInstance,					//インスタントハンドル
		NULL);						//ウインドウ作成データ

	SetFullScreen(true, hWnd);	//全画面設定
	ShowWindow(hWnd, nCmdShow);	//ウインドウの表示状態を設定
	UpdateWindow(hWnd);			//クライアント領域を更新

	pManager = new CManager;		//マネちゃん生成
	pManager->Init(hInstance, hWnd, TRUE);//マネちゃん初期化

	//分解能を設定
	timeBeginPeriod(1);

	//描画処理関連の変数の初期化
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	//FPS関連の変数の初期化
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//メッセージを格納する変数
	MSG mSg;

	//メッセージループ
	while (true)
	{
		if (PeekMessage(&mSg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (mSg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&mSg);	//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&mSg);	//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= HALF_SEC)
			{//0.5秒経過
			 //FPSを計測
				nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	//FPSを測定した時刻を保存
				dwFrameCount = 0;				//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= ONE_FLAME)
			{//60分の1秒経過
			 //処理開始の時刻[現在時刻]を保存
				dwExecLastTime = dwCurrentTime;

				//更新処理
				pManager->Update();

				//FPSカウンタを渡す
				CManager::GetDebugProc()->DebugPrint("FPS：%d\n", nCountFPS);

				//描画処理
				pManager->Draw();

				//フレームカウントを加算
				dwFrameCount++;
			}
		}
	}

	//マネちゃんが生成されている
	if (pManager != NULL)
	{
		pManager->Uninit();	//終了処理
		delete pManager;	//メモリ削除
		pManager = NULL;	//NULLポインタにする
	}

	//分解能を戻す
	timeEndPeriod(1);

	//タスクバー表示
	ShowTaskBar(true, hWnd);

	//ウインドウクラスの登録の解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)mSg.wParam;
}

//ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//返り値を格納
	int nID;

	switch (uMsg)
	{
		//ウインドウ破棄のメッセージ
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:			//キー押下のメッセージ
		switch (wParam)
		{
			//[ESC]キーが押された
		case VK_ESCAPE:
			//終了するかの確認
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				MessageBox(NULL, "また来てくださいね～", "見送り", MB_OK);
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
				//タスクバー表示
				ShowTaskBar(true, hWnd);
				break;
			}
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//既定の処理を返す
}

//----------------------------
//フルスクリーン設定
//引数１	bFull：フルスクリーン切り替え（フルスクリーンにする場合、タスクバーを消す）　true:フルスク　false:通常ウィンドウ
//引数２	hWnd ：ウィンドウステータス情報
//----------------------------
void SetFullScreen(bool bFull, HWND hWnd)
{
	HDC hDC = GetDC(GetDesktopWindow());			// デスクトップのハンドル
	int nScrSizeX = GetDeviceCaps(hDC, HORZRES);	// デスクトップの画面サイズX
	int nScrSizeY = GetDeviceCaps(hDC, VERTRES);	// デスクトップの画面サイズY

	ReleaseDC(GetDesktopWindow(), hDC);				// デスクトップのハンドルを手放す

	if (bFull)
	{// 全画面フラグが真の時、
	 // ウインドウの位置を設定
		SetWindowPos(
			hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// 全画面フラグが偽の時、
	 // ウインドウの位置を設定
		SetWindowPos(
			hWnd,
			HWND_TOP,
			(nScrSizeX / 2) - (SCREEN_WIDTH / 2),
			(nScrSizeY / 2) - (SCREEN_HEIGHT / 2),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(hWnd);

	// タスクバーを表示/非表示
	ShowTaskBar(!bFull, hWnd);
}

//========================================
// ShowTaskBar関数 - タスクバーの表示/非表示を切り替える -
//----------------------------
//引数１	bShow：タスクバー表示ステータス　true:表示する　false:非表示にする
//引数２	hWnd ：ウィンドウステータス情報
// 参考: https://dixq.net/forum/viewtopic.php?t=12505
//----------------------------
void ShowTaskBar(bool bShow, HWND hWnd)
{
	// タスクバーのハンドルを取得
	HWND hTask = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (bShow)
	{// 表示
	 // タスクバーを表示
		ShowWindow(hTask, SW_SHOW);

		// ウインドウをアクティブにする
		SetFocus(hWnd);
	}
	else
	{// 非表示
	 // タスクバーを非表示
		ShowWindow(hTask, SW_HIDE);
	}
}