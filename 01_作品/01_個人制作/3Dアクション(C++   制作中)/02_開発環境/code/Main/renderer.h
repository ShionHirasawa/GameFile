//================================================================================================
//
//レンダリング処理[renderer.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//レンダラークラス
class CRenderer
{
public:
	//αブレンドが [ 加算・減算 ] 合成のどちらか
	typedef enum
	{
		TYPE_ADD = D3DBLENDOP_ADD,			//加算合成
		TYPE_SUB = D3DBLENDOP_REVSUBTRACT,	//減算合成
		VALUE_SET = D3DBLEND_ONE,			//αブレンドを設定
		VALUE_RESET = D3DBLEND_INVSRCALPHA,	//αブレンドを戻す
	}ALPHABLEND;

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	void SetAlphaBlend(ALPHABLEND type, ALPHABLEND value);
	void SetAlphaTest(bool bTest, int nAlpha);
	void SetBackGroundColor(D3DXCOLOR col = DEFAULT_BG_COLOR) { m_BGColor = col; }

	//引数には「両面カリング」「通常カリング」「裏面カリング」のいずれかを代入する
	//両面カリング = D3DCULL_NONE
	//通常カリング = D3DCULL_CCW
	//裏面カリング = D3DCULL_CW
	void SetCulling(_D3DCULL value = D3DCULL_CCW);

	bool Culling(void) { return m_bCull; }
	bool IsPause(void) { return m_bPause; }

private:
	static const D3DXCOLOR DEFAULT_BG_COLOR;//デフォルト背景色

	LPDIRECT3D9			m_pD3D = NULL;		// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9	m_pD3DDevice = NULL;// Direct3Dデバイスへのポインタ

	D3DXCOLOR m_BGColor;	//背景色

	bool m_bPause;	// ポーズ
	bool m_bCull;	// 両面カリングON/OFF
};

#endif