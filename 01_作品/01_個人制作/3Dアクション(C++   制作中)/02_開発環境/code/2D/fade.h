//================================================================================================
//
//フェード処理[fade.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "object2D.h"

//フェードクラス
class CFade : public CObject2D
{
public:
	//フェードの状態
	enum FADE
	{
		FADE_NONE = 0,	//何もしていない状態
		FADE_IN,		//フェードイン
		FADE_OUT,		//フェードアウト
		FADE_WIPE_IN,	//左上から右下にかけてフェードイン
		FADE_WIPE_OUT,	//左上から右下にかけてフェードアウト
		FADE_MAX
	};

	CFade();
	~CFade();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFade *Create(void);
	static void Set(CScene::MODE modeNext, FADE FadeOutType);
	static FADE Get(void) { return s_state; }
	int GetCounter(void) { return s_nCounter; }

private:
	//====================
	//プライベート静的メンバ定数
	//====================
	static const float FADE_SPEED;	//フェードするスピード
	static const int FADE_PRIORITY;	//フェードの優先順位

	static void Update_In(void);	//フェードイン中の更新
	static void Update_Out(void);	//フェードアウト中の更新
	static void Update_WipeIn(void);//ワイプイン中の更新
	static void Update_WipeOut(void);//ワイプアウト中の更新

	static CScene::MODE s_NextMode;//次のモード
	static FADE			s_state;	//フェード状態
	static D3DXCOLOR	s_Color;	//透明度
	static D3DXCOLOR	s_WipeColor[VTX_MAX];//ワイプタイプのフェードカラー

	static const int c_WipeFadeTiming[VTX_MAX];
	static int s_WipeCounter;
	static int s_nCounter;
};

#endif