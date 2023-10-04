//================================================================================================
//
//リザルト画面処理[result.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//リザルトクラス
class CResult : public CScene
{
public:
	//リザルトステータス
	enum STATE
	{
		STATE_OVER = 0,	//ゲームオーバー
		STATE_CLEAR,	//ゲームクリア!
		STATE_MAX
	};

	CResult();
	~CResult();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetState(STATE state);

private:
	//====================
	//静的メンバ定数
	//====================
	static char	*BG_TEXTURE_PATH[STATE_MAX];	//背景のテクスチャパス
	static STATE s_State;			//リザルト状態
};

#endif