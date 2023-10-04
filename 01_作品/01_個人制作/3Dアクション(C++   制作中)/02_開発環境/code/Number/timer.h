//================================================================================================
//
//タイマー処理[timer.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

//タイマーの桁数
#define TIMER_DEJIT	(3)

//タイマークラス
class CTimer : public CNumber
{
public:
	CTimer();
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer *Create(void);
	/*	タイマー設定	*/void Set(int nTimer = 0);

private:
	static D3DXCOLOR s_TimerColor;				//タイマーの色
	static CNumber	*m_apNumber[TIMER_DEJIT];	// 数字オブジェクトのポインタ
	int		m_nTimer;		// タイマー（残り時間
	int		m_nCountFrame;	// フレームカウンター
};

#endif