//================================================================================================
//
//マネージャー処理[manager.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "manager.h"
#include "file.h"
#include "../Object/object.h"
#include "../2D/object2D.h"
#include "../3D/object3D.h"
#include "../3D/billboard.h"
#include "../XFile/objectX.h"
#include "../XFile/enemy.h"
#include "../2D/bg.h"
#include "../2D/fade.h"
#include "../3D/bullet.h"
#include "../3D/effect.h"
#include "../3D/particle.h"
#include "../3D/explosion.h"
#include "../3D/item.h"
#include "../Number/number.h"
#include "../Number/score.h"
#include "../Number/timer.h"
#include "../3D/camera.h"
#include "../3D/light.h"
#include "../3D/floor.h"
#include "../3D/treeBillboard.h"
#include "../XFile/jewel.h"
#include "../Player/player.h"
#include "../3D/shadow.h"

#include "../Mode/title.h"
#include "../Mode/tutorial.h"
#include "../Mode/game.h"
#include "../Mode/result.h"

//静的メンバ変数宣言
CRenderer		*CManager::s_pRenderer = NULL;
CScene			*CManager::s_pScene = NULL;
CInputKeyboard	*CManager::s_pKeyboard = NULL;
CInputGamePad	*CManager::s_pGamePad = NULL;
CInputMouse		*CManager::s_pMouse = NULL;
CDebugProc		*CManager::s_pDebugProc = NULL;
CSound			*CManager::s_pSound = NULL;
CCamera			*CManager::s_pCamera = NULL;
CLight			*CManager::s_pLight = NULL;
CTexture		*CManager::s_pTexture = NULL;

HINSTANCE		CManager::s_hInstance;
HWND			CManager::s_hWnd;

//=======================================
//コンストラクタ
//=======================================
CManager::CManager()
{
	//メモリ開放してメンバ変数内のポインタ類を全てNULLに
	OpenPointer();

	m_bAutoFade = false;
	m_bSelect = false;
	type = CFade::FADE_OUT;
}

//=======================================
//デストラクタ
//=======================================
CManager::~CManager()
{
	//メモリ開放
	OpenPointer();
}

//=======================================
//初期化処理
//=======================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//変数代入
	s_hInstance = hInstance;
	s_hWnd = hWnd;

	//ランダム関数のシードを時刻より設定
	srand((unsigned int)time(NULL));

	//静的メンバ変数を生成する
	CreateStatic();

	//フェードを生成する
	CFade *pFade = CFade::Create();
	pFade->Init();

	//全初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CManager::Uninit(void)
{
	//影全削除
	CShadow::Delete();

	//==============================
	//	全オブジェクトの終了処理
	//==============================
	CObject::ReleaseAll();

	//メモリ開放
	OpenPointer();
}

//=======================================
//メモリ開放処理
//=======================================
void CManager::OpenPointer(void)
{
	//シーンクラスが生成されている
	if (s_pScene != NULL)
	{
		s_pScene->Uninit();	// 終了処理
		delete s_pScene;	// メモリ削除
		s_pScene = NULL;	// NULLポインタにする
	}

	//キーボードが生成されている
	if (s_pKeyboard != NULL)
	{
		s_pKeyboard->Uninit();	// 終了処理
		delete s_pKeyboard;		// メモリ削除
		s_pKeyboard = NULL;		// NULLポインタにする
	}

	//ゲームパッドが生成されている
	if (s_pGamePad != NULL)
	{
		s_pGamePad->Uninit();	// 終了処理
		delete s_pGamePad;		// メモリ削除
		s_pGamePad = NULL;		// NULLポインタにする
	}

	//マウスが生成されている
	if (s_pMouse != NULL)
	{
		s_pMouse->Uninit();	// 終了処理
		delete s_pMouse;	// メモリ削除
		s_pMouse = NULL;	// NULLポインタにする
	}

	//カメラクラスが生成されている
	if (s_pCamera != NULL)
	{
		s_pCamera->Uninit();// 終了処理
		delete s_pCamera;	// メモリ削除
		s_pCamera = NULL;	// NULLポインタにする
	}

	//ライトクラスが生成されている
	if (s_pLight != NULL)
	{
		s_pLight->Uninit();	// 終了処理
		delete s_pLight;	// メモリ削除
		s_pLight = NULL;	// NULLポインタにする
	}

	//テクスチャクラスが生成されている
	if (s_pTexture != NULL)
	{
		s_pTexture->UnLoad();	// 終了処理
		delete s_pTexture;		// メモリ削除
		s_pTexture = NULL;		// NULLポインタにする
	}

	//デバッグ表示クラスが生成されている
	if (s_pDebugProc != NULL)
	{
		s_pDebugProc->Uninit();	// 終了処理
		delete s_pDebugProc;	// メモリ削除
		s_pDebugProc = NULL;	// NULLポインタにする
	}

	// サウンドクラスが生成されている
	if (s_pSound != NULL)
	{
		s_pSound->Stop();	// サウンド全停止
		s_pSound->Uninit();	// 終了処理
		delete s_pSound;	// メモリ削除
		s_pSound = NULL;	// NULLポインタにする
	}

	//レンダラークラスが生成されている
	if (s_pRenderer != NULL)
	{
		s_pRenderer->Uninit();	// 終了処理
		delete s_pRenderer;		// メモリ削除
		s_pRenderer = NULL;		// NULLポインタにする
	}
}

//=======================================
//静的メンバ変数を生成
//=======================================
HRESULT CManager::CreateStatic(void)
{
	//レンダラーが生成されていない
	if (s_pRenderer == NULL)
	{
		//レンダラー生成
		s_pRenderer = new CRenderer;

		//レンダラー初期化処理
		if (FAILED(s_pRenderer->Init(s_hWnd, TRUE))) return E_FAIL;
	}

	//キーボードが生成されていない
	if (s_pKeyboard == NULL)
	{
		//キーボード生成
		s_pKeyboard = new CInputKeyboard;

		//キーボード初期化処理
		if (FAILED(s_pKeyboard->Init(s_hInstance, s_hWnd))) return E_FAIL;
	}

	//ゲームパッドが生成されていない
	if (s_pGamePad == NULL)
	{
		//ゲームパッド生成
		s_pGamePad = new CInputGamePad;

		//ゲームパッド初期化処理
		if (FAILED(s_pGamePad->Init(s_hInstance, s_hWnd))) return E_FAIL;
	}

	//マウスが生成されていない
	if (s_pMouse == NULL)
	{
		//マウス生成
		s_pMouse = new CInputMouse;

		//マウス初期化
		if (FAILED(s_pMouse->Init(s_hInstance, s_hWnd))) return E_FAIL;
	}

	//デバッグ表示オブジェクトが生成されていない
	if (s_pDebugProc == NULL)
	{
		//デバッグ表示オブジェクト生成
		s_pDebugProc = new CDebugProc;

		//デバッグ表示オブジェクト生成出来なかった
		if (s_pDebugProc == NULL) return E_FAIL;

		//デバッグ表示オブジェクト初期化
		s_pDebugProc->Init();
	}

	//サウンドクラスが生成されていない
	if (s_pSound == NULL)
	{
		//サウンドの生成
		s_pSound = new CSound;

		//サウンド初期化
		if (FAILED(s_pSound->Init(s_hWnd))) return E_FAIL;
	}

	//カメラが生成されていない
	if (s_pCamera == NULL)
	{
		//カメラ生成
		s_pCamera = new CCamera;

		//カメラが生成できなかった
		if (s_pCamera == NULL) return E_FAIL;

		//カメラ初期化
		s_pCamera->Init();
	}

	//ライトが生成されていない
	if (s_pLight == NULL)
	{
		//ライト生成
		s_pLight = new CLight;

		//ライトが生成出来なかった
		if (s_pLight == NULL) return E_FAIL;

		//ライト初期化
		s_pLight->Init();
	}

	//テクスチャクラスが生成されていない
	if (s_pTexture == NULL)
	{
		//テクスチャ生成
		s_pTexture = new CTexture;

		//生成できなかった
		if (s_pTexture == NULL) return E_FAIL;
	}

	//シーンクラスはリリース時のみ生成する
	//デバッグ時は主動で起動シーンを選択できる
#ifndef _DEBUG
	m_bSelect = true;
	if (s_pScene == NULL)
	{
		//シーンクラスをモードを設定して生成する
		s_pScene = CScene::Create(CScene::TITLE);

		//生成できなかった
		if (s_pScene == NULL)	return E_FAIL;
	}
#endif 

	//生成成功
	return S_OK;
}

//=======================================
//更新処理
//=======================================
void CManager::Update(void)
{
	//静的メンバ変数を生成する
	CreateStatic();

	//シーンクラスが生成されていない
	if (!SelectStartMode()) return;

	// [デバッグ用] オブジェクトの数を表示
	int nNumAll = 0;
	for (int nCntObj = 0; nCntObj < CObject::OBJECT_PRIORIRY_MAX; nCntObj++)
	{
		nNumAll += CObject::GetNumAll(nCntObj);
	}

	s_pKeyboard->Update();	// キーボードの更新
	s_pGamePad->Update();	// ゲームパッドの更新
	s_pMouse->Update();		// マウスの更新
	s_pScene->Update();		// シーンの更新
	s_pRenderer->Update();	// レンダラー更新
	s_pLight->Update();		// ライトの更新処理
	s_pCamera->Update();	// カメラの更新処理

#ifdef _DEBUG
	if (s_pKeyboard->GetTrigger(DIK_LSHIFT))
	{
		//自動フェード切り替え
		m_bAutoFade ^= 1;
	}

	if (s_pKeyboard->GetPress(DIK_RIGHT) || m_bAutoFade)
	{
		//次のモード番号を算出
		//現在のモード番号を取得
		int mode = (s_pScene->Get() + 1) % CScene::MAX;

		//モード遷移
		CFade::Set((CScene::MODE)mode, (CFade::FADE)type);
	}

	else if (s_pKeyboard->GetPress(DIK_LEFT))
	{
		//前のモード番号を算出
		int mode = (s_pScene->Get() + CScene::MAX - 1) % CScene::MAX;

		//モード遷移
		CFade::Set((CScene::MODE)mode, (CFade::FADE)type);
	}
#endif
}

//=======================================
//描画処理
//=======================================
void CManager::Draw(void)
{
	s_pRenderer->Draw();	// レンダラー描画
}

//=======================================
//次のモードを決める
//=======================================
void CManager::SetMode(CScene::MODE mode)
{
	//サウンドを停止する
	if (s_pSound != NULL) s_pSound->Stop();

	//現在のモードを破棄する
	if (s_pScene != NULL)
	{
		//シーンのオブジェクトなどを破棄する
		s_pScene->Uninit();
		delete s_pScene;
		s_pScene = NULL;
	}

	//新しいモードを生成する
	if (s_pScene == NULL)	s_pScene = CScene::Create(mode);
}

//=======================================
//現在のモードを返す
//=======================================
CScene::MODE CManager::GetMode(void)
{
	//現在のモードを返す
	if (s_pScene != NULL) return s_pScene->Get();

	//シーンクラスが生成されていなかったらとりあえずタイトルを返す
	return CScene::TITLE;
}

//=======================================
//フェードを生成する
//=======================================
void CManager::CreateFade(void)
{
	CFade *pFade = CFade::Create();
	pFade->Init();
}

//=======================================
//最初のモードを決める
//=======================================
bool CManager::SelectStartMode(void)
{
	//シーンクラスが生成されていない
	if (s_pScene != NULL)
	{
		return m_bSelect;
	}

	s_pGamePad->Update();	// ゲームパッドの更新
	s_pKeyboard->Update();	// キーボードの更新
	s_pDebugProc->SetInfo(40, GetColor(COL_WHITE), DT_CENTER);

	s_pDebugProc->Print("\n\n\n最初のモードを選択してください\n");

	s_pDebugProc->Print("タイトル：%d / Ｙ\n",		CScene::TITLE);
	s_pDebugProc->Print("チュートリアル：%d / Ｘ\n",CScene::TUTORIAL);
	s_pDebugProc->Print("ゲーム：%d / Ａ\n",		CScene::GAME);

	int nKeyNumber[3] = { DIK_0, DIK_1, DIK_2 };
	int nPadNumber[3] = { XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_A };

	for (int nCntMode = 0; nCntMode < 3; nCntMode++)
	{
		//対象の番号が押された
		if (IsInputTrigger(nKeyNumber[nCntMode], nPadNumber[nCntMode]))
		{
			//シーンクラスをモードを設定して生成する
			SetMode((CScene::MODE)nCntMode);

			//生成できた
			if (s_pScene != NULL)
			{
				//フォントを元に戻す
				s_pDebugProc->SetInfo(CDebugProc::DEFAULT_FONT_SIZE, GetColor(COL_WHITE), DT_LEFT);
				m_bSelect = true;
				break;
			}
		}
	}

	return m_bSelect;
}

//=======================================
//キーボード・ゲームパッドのプレス情報
//=======================================
bool CManager::IsInputPress(int nKey, int nPadButton)
{
	//プレス情報を返す
	return s_pKeyboard->GetPress(nKey) || s_pGamePad->GetPress(nPadButton);
}

//=======================================
//キーボード・ゲームパッドのトリガー情報
//=======================================
bool CManager::IsInputTrigger(int nKey, int nPadButton)
{
	//プレス情報を返す
	return s_pKeyboard->GetTrigger(nKey) || s_pGamePad->GetTrigger(nPadButton);
}

//==========================================================================================================
//												シーン
//==========================================================================================================

//モードクラスを格納
CScene *CScene::s_pMode = NULL;

//=======================================
//コンストラクタ
//=======================================
CScene::CScene()
{
	//静的メンバ変数クリア
	s_pMode = NULL;
}

//=======================================
//デストラクタ
//=======================================
CScene::~CScene()
{

}

//=======================================
//生成処理
//=======================================
CScene *CScene::Create(MODE mode)
{
	//生成
	CScene *pScene = new CScene;

	//生成できなかった
	if (pScene == NULL)	return NULL;

	//モード設定
	pScene->m_mode = mode;

	//初期化処理
	if (FAILED(pScene->Init()))return NULL;

	//モードごとに生成し、代入するクラスを分ける
	switch (mode)
	{
		case TITLE:		s_pMode = new CTitle;	break;
		case TUTORIAL:	s_pMode = new CTutorial;break;
		case GAME:		s_pMode = new CGame;	break;
		case RESULT:	s_pMode = new CResult;	break;
	}

	//インスタンスが生成されたら初期化
	if (s_pMode != NULL)s_pMode->Init();

	//生成できなかったらインスタンス削除
	else
	{
		delete pScene;
		pScene = NULL;
	}

	//インスタンスを返す
	return pScene;
}

//=======================================
//初期化処理
//=======================================
HRESULT CScene::Init(void)
{
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CScene::Uninit(void)
{
	//モードクラスが生成されていたら
	if (s_pMode != NULL)
	{
		s_pMode->Uninit();	//終了処理
		delete s_pMode;		//メモリ開放
		s_pMode = NULL;
	}
}

//=======================================
//更新処理
//=======================================
void CScene::Update(void)
{
	if (s_pMode != NULL) s_pMode->Update();
}

//=======================================
//描画処理
//=======================================
void CScene::Draw(void)
{

}