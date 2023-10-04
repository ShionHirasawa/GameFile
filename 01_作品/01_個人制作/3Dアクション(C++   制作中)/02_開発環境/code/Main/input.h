//==========================================================================================
//
//入力処理[input.h]
//Author:Hirasawa Shion
//
//==========================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//==========================================================================================
//マクロ定義
//==========================================================================================
#define NUM_KEY_MAX			(256)	//キーの最大数
#define REPEATE_TIME		(150)	//リピートの間隔

#define MOUSE_CLICK_LEFT	(0)		//マウス左クリック
#define MOUSE_CLICK_RIGHT	(1)		//マウス右クリック

#define GAMEPAD_BUTTON_NUM	(14)	//ゲームパッドのボタン数
#define STICK_DEADZONE		(655)	//スティックの遊び範囲
#define STICK_MAX			(32767)	//スティック最大値
#define MAX_USE_GAMEPAD		(4)		//ゲームパッド使用可能台数

#define PAD_VIBE_FADE		(200)	//振動の減少量
#define VIBE_POWER_MIN		(7000)	//最低振動量
#define VIBE_POWER_LEVEL_01	(10000)	//弱めの振動
#define VIBE_POWER_LEVEL_02	(23000)	//中くらいの振動
#define VIBE_POWER_LEVEL_03	(37000)	//強めの振動
#define VIBE_POWER_LEVEL_04	(50000)	//かなり強めの振動
#define VIBE_POWER_LEVEL_05	(65535)	//最大振動

//==========================================================================================
//インプットクラス（親クラス）の定義
//==========================================================================================
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//==========================================================================================
//キーボードクラスの定義：インプットクラスを継承
//==========================================================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeate(int nKey);

private:
	BYTE	m_state[NUM_KEY_MAX];		//状態（保存用およびプレス情報）
	BYTE	m_trigger[NUM_KEY_MAX];		//トリガー情報
	BYTE	m_release[NUM_KEY_MAX];		//リリース情報
	BYTE	m_repeate[NUM_KEY_MAX];		//リピート情報
	DWORD	m_currentTime[NUM_KEY_MAX];	//現在の時間
	DWORD	m_execLastTime[NUM_KEY_MAX];//最後にtrueにした時間
};

//==========================================================================================
//マウスクラスの定義：インプットクラスを継承
//==========================================================================================
class CInputMouse : public CInput
{
public:
	//クリック種類列挙
	typedef enum
	{
		CLICK_LEFT = 0,	//左クリック
		CLICK_RIGHT,	//右クリック
		CLICK_WHEEL,	//ホイールクリック
		CLICK_MAX
	}CLICK;

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);

	//========================
	//マウスのカーソル位置を返す処理
	//=======================
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	//========================
	//マウスのカーソル移動量を返す処理
	//=======================
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:
	/*現在位置*/D3DXVECTOR3	 	m_pos;
	/*移動量*/	D3DXVECTOR3	 	m_move;
	/*状態*/	DIMOUSESTATE 	m_state;
	/*トリガー*/BYTE		 	m_trigger[CLICK_MAX];
};

//==========================================================================================
//ゲームパッドクラスの定義：インプットクラスを継承
//==========================================================================================
class CInputGamePad : public CInput
{
public:
	static const int UP;		// 十字「上」
	static const int DOWN;		// 十字「下」
	static const int LEFT;		// 十字「左」
	static const int RIGHT;		// 十字「右」
	static const int START;		// STARTボタン
	static const int BACK;		// BACKボタン
	static const int L_THUMB;	// 左スティックボタン
	static const int R_THUMB;	// 右スティックボタン
	static const int L_SHOULDER;// 通称「LB」ボタン
	static const int R_SHOULDER;// 通称「RB」ボタン
	static const int A;			// Ａボタン
	static const int B;			// Ｂボタン
	static const int X;			// Ⅹボタン
	static const int Y;			// Ｙボタン
	static const int L_TRIGGER;	// 左トリガー
	static const int R_TRIGGER;	// 右トリガー

	//ゲームパッドの振動状態
	enum VibeState
	{
		VibeState_00_STOP,	//停止
		VibeState_01_LOOP,	//ずっと同じ振動
		VibeState_02_FADE,	//徐々に落ち着いていく
		VibeState_MAX
	};

	CInputGamePad();
	~CInputGamePad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetUse(void);

	//=============================
	//振動
	//=============================
	void UpdateVibe(void);
	void StopVibration(void);
	void SetPadVibration(WORD wPower, int nVibeTime, VibeState State);

	//=============================
	//ボタン
	//=============================
	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeate(int nButton);

	//=============================
	//スティック
	//=============================
	float GetLStickRot(void);
	float GetRStickRot(void);
	SHORT GetLStickX(void);
	SHORT GetLStickY(void);
	SHORT GetRStickX(void);
	SHORT GetRStickY(void);

private:
	static const int DEADZONE_TRIGGER_BUTTON;	//左右のトリガーボタンのデッドゾーン
	static const int MAX_TRIGGER_BUTTON;		//左右のトリガーボタンの最大入力値

	void FixTriggerButton(XINPUT_STATE *pState);
	void UpdatePress(const XINPUT_STATE state);
	void UpdateTrigger(const XINPUT_STATE state);
	void UpdateRelease(const XINPUT_STATE state);
	void UpdateRepeate(const XINPUT_STATE state);

	static int	 m_nNumAll;		//使われているコントローラーの総数
	XINPUT_STATE m_state;		//状態（保存用およびプレス情報）
	XINPUT_STATE m_trigger;		//トリガー情報
	XINPUT_STATE m_release;		//リリース情報
	XINPUT_STATE m_repeate;		//リピート情報
	DWORD		 m_currentTime;	//現在の時間
	DWORD		 m_execLastTime;//最後にtrueにした時間
	VibeState	 m_VibeState;	//振動の状態
	WORD		 m_wVibePower;	//振動の強さ
	int			 m_nVibeTime;	//振動させる時間
	bool		 m_bUse;		//使用の有無
	int			 m_nID;			//自分の番号
};

#endif