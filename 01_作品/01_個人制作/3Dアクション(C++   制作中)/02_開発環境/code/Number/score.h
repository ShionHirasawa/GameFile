//================================================================================================
//
//スコア処理[score.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "number.h"

//スコアの桁数
#define SCORE_DEJIT	(8)

//前方宣言
class CNumber;

//スコアクラス
class CScore : public CNumber
{
public:
	CScore();
	~CScore();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *Create(void);
	/*	スコア設定	*/static void Set(int nScore = 0);
	/*	スコア加算	*/static void Add(const int nValue) { Set(m_nScore + nValue); }

private:
	static CNumber	*m_apNumber[SCORE_DEJIT];	// 数字オブジェクトのポインタ
	static int		m_nScore;	// スコア
};

#endif