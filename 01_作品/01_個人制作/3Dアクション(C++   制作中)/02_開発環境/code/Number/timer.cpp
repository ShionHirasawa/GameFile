//================================================================================================
//
//タイマー処理[timer.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "number.h"
#include "timer.h"
#include "../2D/fade.h"

/***		マクロ定義		***/
/*タイマーの最大数*/	#define MAX_TIMER	((int)pow(10, TIMER_DEJIT + 1) - 1)
/*タイマーの基準Ⅹ座標*/#define DEFAULT_POS_X	(660.0f)
/*数字の横幅*/			#define NUMBER_WIDTH	(20.0f)
/*数字の高さ*/			#define NUMBER_HEIGHT	(40.0f)
/*数字の余白*/			#define NUMBER_MARGIN	(10.0f)

//静的メンバ変数
/*オブジェクト2Dのポインタ*/CNumber	*CTimer::m_apNumber[TIMER_DEJIT] = {};
/*タイマーの色*/			D3DXCOLOR CTimer::s_TimerColor = GetColor(COL_WHITE);

//=======================================
//コンストラクタ
//=======================================
CTimer::CTimer()
{
	//数字オブジェクトをすべてNULLに
	for (int nCntTimer = 0; nCntTimer < TIMER_DEJIT; nCntTimer++)
	{
		m_apNumber[nCntTimer] = NULL;
	}

	/*タイマー初期化*/m_nTimer = 0;
	/*タイマーの色*/s_TimerColor = GetColor(COL_WHITE);
}

//=======================================
//デストラクタ
//=======================================
CTimer::~CTimer()
{

}

//=======================================
//生成処理
//=======================================
CTimer *CTimer::Create(void)
{
	//インスタンス生成
	CTimer *pTimer = new CTimer;

	//インスタンスを生成できた
	if (pTimer != NULL)
	{
		//初期化処理
		pTimer->Init();
	}

	//インスタンスを返す
	return pTimer;
}

//=======================================
//初期化処理
//=======================================
HRESULT CTimer::Init(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DEJIT; nCntTimer++)
	{
		//数字インスタンス生成
		CNumber *pNumber = CNumber::Create();

		//インスタンスが生成できなかった
		if (pNumber == NULL)
		{//初期化失敗
			return E_FAIL;
		}

		SetType(CObject::TYPE_SCORE);	//種類設定
		m_apNumber[nCntTimer] = pNumber;//インスタンス割り当て
	}

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CTimer::Uninit(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DEJIT; nCntTimer++)
	{
		//数字オブジェクト削除
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->Uninit();
			m_apNumber[nCntTimer] = NULL;
		}
	}

	//自分自身を破棄
	this->Release();
}

//=======================================
//更新処理
//=======================================
void CTimer::Update(void)
{
	//フレームカウンター増加
	m_nCountFrame++;

	//フレームが1秒経過した
	if (m_nCountFrame % MAX_FPS == 0)
	{
		/*	カウンター初期化 */	m_nCountFrame = 0;
		/*	  タイマー減少	 */	m_nTimer--;

		//タイムリミット予告
		if (m_nTimer == 40)
		{
			CManager::GetSound()->Play(CSound::SE_TIME_LIMIT);
			s_TimerColor = GetColor(COL_RED);
		}

		//タイマーが０になった
		if (m_nTimer < 0)
		{
			//フェード処理
			CFade::Set(CScene::RESULT, CFade::FADE_WIPE_OUT);
			m_nTimer = 0;
		}

		//タイマーセット
		Set(m_nTimer);
	}
}

//=======================================
//描画処理
//=======================================
void CTimer::Draw(void)
{

}

//=======================================
//タイマー設定処理
//=======================================
void CTimer::Set(int nTimer)
{
	//設定するタイマー値が最大値を超えた
	if (nTimer > MAX_TIMER)
	{
		//設定する値を最大値に戻す
		nTimer = MAX_TIMER;
	}

	//タイマー設定
	m_nTimer = nTimer;

	//全桁の頂点情報設定
	for (int nCntTimer = 0; nCntTimer < TIMER_DEJIT; nCntTimer++)
	{
		//数字設定（一の位から設定
		m_apNumber[nCntTimer]->SetInfo
		(nTimer % 10, D3DXVECTOR3(DEFAULT_POS_X - ((float)nCntTimer * NUMBER_WIDTH + (float)nCntTimer * NUMBER_MARGIN), NUMBER_HEIGHT, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT);

		//タイマー色設定
		m_apNumber[nCntTimer]->SetVertexInfo(s_TimerColor);

		//一桁下げる
		nTimer /= 10;
	}
}