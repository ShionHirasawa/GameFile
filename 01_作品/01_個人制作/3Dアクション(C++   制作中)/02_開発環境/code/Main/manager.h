//================================================================================================
//
//マネージャー処理[manager.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "color.h"
#include "texture.h"

//前方宣言
class CRenderer;
class CDebugProc;
class CInputKeyboard;
class CInputMouse;
class CInputGamePad;
class CSound;
class CCamera;
class CLight;
class CTexture;

//シーンクラス
class CScene
{
public:
	//モードの列挙型
	enum MODE
	{
		TITLE = 0,	// タイトル
		TUTORIAL,	// チュートリアル
		GAME,		// ゲーム
		RESULT,	// リザルト
		MAX
	};

	CScene();
	virtual ~CScene();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CScene *Create(MODE mode);	//生成処理
	MODE Get(void) { return m_mode; }	//取得処理

private:
	static CScene *s_pMode;	//モードごとのクラスを格納

	MODE m_mode;	//現在のモードを格納
};

//マネちゃんクラス
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void);

	static bool IsInputPress(int nKey, int nPadButton);
	static bool IsInputTrigger(int nKey, int nPadButton);

	//各種オブジェクトのポインタ取得処理
	static CRenderer		*GetRenderer(void)	{ return s_pRenderer;}
	static CScene			*GetScene(void)		{ return s_pScene; }
	static CDebugProc		*GetDebugProc(void)	{ return s_pDebugProc;}
	static CInputKeyboard	*GetKeyboard(void)	{ return s_pKeyboard;}
	static CInputGamePad	*GetGamePad(void)	{ return s_pGamePad; }
	static CInputMouse		*GetMouse(void)		{ return s_pMouse; }
	static CSound			*GetSound(void)		{ return s_pSound; }
	static CCamera			*GetCamera(void)	{ return s_pCamera;}
	static CLight			*GetLight(void)		{ return s_pLight;	}
	static CTexture			*GetTexture(void)	{ return s_pTexture; }

private:
	void OpenPointer(void);			//メモリ開放
	HRESULT CreateStatic(void);		//静的メンバ変数を生成
	static void CreateFade(void);	//フェードを生成する

	bool SelectStartMode(void);		//開始するモードを選択
	bool m_bSelect;

	static CRenderer		*s_pRenderer;
	static CScene			*s_pScene;
	static CDebugProc		*s_pDebugProc;
	static CInputKeyboard	*s_pKeyboard;
	static CInputGamePad	*s_pGamePad;
	static CInputMouse		*s_pMouse;
	static CSound			*s_pSound;
	static CCamera			*s_pCamera;
	static CLight			*s_pLight;
	static CTexture			*s_pTexture;

	//静的メンバ変数生成用に保持
	static HINSTANCE		s_hInstance;
	static HWND				s_hWnd;

	bool m_bAutoFade;
	int type;//FADE_OUT　か　FADE_WIPE_OUTのどちらか
};

#endif