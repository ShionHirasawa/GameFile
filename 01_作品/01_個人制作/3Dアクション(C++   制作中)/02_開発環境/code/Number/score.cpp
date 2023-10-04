//================================================================================================
//
//スコア処理[score.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "number.h"
#include "score.h"

/***		マクロ定義		***/
/*スコアの最大数*/		#define MAX_SCORE	((int)pow(10, SCORE_DEJIT + 1) - 1)
/*スコアの基準Ⅹ座標*/	#define DEFAULT_POS_X	(1100.0f)
/*数字の横幅*/			#define NUMBER_WIDTH	(20.0f)
/*数字の高さ*/			#define NUMBER_HEIGHT	(40.0f)
/*数字の余白*/			#define NUMBER_MARGIN	(10.0f)

//静的メンバ変数
/*オブジェクト2Dのポインタ*/CNumber	*CScore::m_apNumber[SCORE_DEJIT] = {};
/*スコア*/					int		CScore::m_nScore = 0;

//=======================================
//コンストラクタ
//=======================================
CScore::CScore()
{
	//数字オブジェクトをすべてNULLに
	for (int nCntScore = 0; nCntScore < SCORE_DEJIT; nCntScore++)
	{
		m_apNumber[nCntScore] = NULL;
	}

	/*スコア初期化*/m_nScore = 0;
}

//=======================================
//デストラクタ
//=======================================
CScore::~CScore()
{

}

//=======================================
//生成処理
//=======================================
CScore *CScore::Create(void)
{
	//数字インスタンス生成
	CScore *pScore = new CScore;

	//インスタンスを生成できた
	if (pScore != NULL)
	{
		//初期化処理
		pScore->Init();
	}

	//インスタンスを返す
	return pScore;
}

//=======================================
//初期化処理
//=======================================
HRESULT CScore::Init(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DEJIT; nCntScore++)
	{
		//数字インスタンス生成
		CNumber *pNumber = CNumber::Create();

		//インスタンスが生成できなかった
		if (pNumber == NULL)
		{//初期化失敗
			return E_FAIL;
		}

		SetType(CObject::TYPE_SCORE);	//種類設定
		m_apNumber[nCntScore] = pNumber;//インスタンス割り当て
	}

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DEJIT; nCntScore++)
	{
		//数字オブジェクト削除
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
			m_apNumber[nCntScore] = NULL;
		}
	}

	//自分自身を破棄
	this->Release();
}

//=======================================
//更新処理
//=======================================
void CScore::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CScore::Draw(void)
{

}

//=======================================
//スコア設定処理
//=======================================
void CScore::Set(int nScore)
{
	//設定するスコア値が最大値を超えた
	if (nScore > MAX_SCORE)
	{
		//設定する値を最大値に戻す
		nScore = MAX_SCORE;
	}

	//スコア設定
	m_nScore = nScore;

	//全桁の頂点情報設定
	for (int nCntScore = 0; nCntScore < SCORE_DEJIT; nCntScore++)
	{
		//数字設定（一の位から設定
		m_apNumber[nCntScore]->SetInfo
		(nScore % 10, D3DXVECTOR3(DEFAULT_POS_X - ((float)nCntScore * NUMBER_WIDTH + (float)nCntScore * NUMBER_MARGIN), NUMBER_HEIGHT, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT);

		//一桁下げる
		nScore /= 10;
	}
}