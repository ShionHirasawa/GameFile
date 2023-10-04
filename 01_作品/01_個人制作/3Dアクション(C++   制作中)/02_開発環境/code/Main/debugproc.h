//==========================================
//
//デバッグ表示プログラム[debugproc.h]
//Author:Hirasawa Shion
//
//==========================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

//デバッグ表示クラス
class CDebugProc
{
public:
	static const int DEFAULT_FONT_SIZE;

	CDebugProc();
	~CDebugProc();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Print(const char *fmt, ...);
	void DebugPrint(const char *fmt, ...);
	void ReleasePrint(const char *fmt, ...);

	void SetInfo(int size, D3DXCOLOR color, int nFormatPlace);

private:
	static const int DEBPRO_MAX_STR;

	void SetFont(void);

	LPD3DXFONT	m_pFont;	//フォントへのポインタ
	char		m_String[2048];	//表示文字バッファ
	
	D3DXCOLOR	m_Color;	//フォントの色
	int			m_Size;		//フォントサイズ
	int			m_Place;	//表示範囲のどこに出力するか
};

#endif