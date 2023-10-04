//==========================================
//
//デバッグ表示プログラム[debugproc.cpp]
//Author:Hirasawa Shion
//
//==========================================
#include "main.h"
#include "color.h"
#include "debugproc.h"
#include "renderer.h"
#include "manager.h"
#include <stdarg.h>

//標準のフォントサイズ
const int CDebugProc::DEFAULT_FONT_SIZE = 18;

//========================
//コンストラクタ
//========================
CDebugProc::CDebugProc()
{
	//メンバ変数クリア
	m_Size = DEFAULT_FONT_SIZE;
	m_Place = DT_LEFT;
	m_Color = GetColor(COL_WHITE);
	memset(&m_String, 0, sizeof m_String);
}

//========================
//デストラクタ
//========================
CDebugProc::~CDebugProc()
{

}

//========================
//初期化処理
//========================
void CDebugProc::Init(void)
{
	//フォント生成
	SetFont();

	//バッファクリア
	memset(&m_String, 0, sizeof m_String);
}

//========================
//終了処理
//========================
void CDebugProc::Uninit(void)
{
	//フォント破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//========================
//更新処理
//========================
void CDebugProc::Update(void)
{

}

//========================
//描画処理
//========================
void CDebugProc::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	
	//テキスト描画
	m_pFont->DrawText(NULL, &m_String[0], -1, &rect, m_Place, m_Color);

	//バッファクリア
	memset(&m_String, 0, sizeof m_String);
}

//========================
//入力取り込み処理
//========================
void CDebugProc::Print(const char *fmt, ...)
{
	int nCount;		//カウンタ
	va_list args;	//可変引数の中身

	va_start(args, fmt);//出力開始

						//改行文字が来るまでループ
	for (nCount = 0; *fmt != '\0'; fmt++)
	{
		if (*fmt == '%')
		{
			char aStr[10] = {};
			switch (*(fmt + 1))
			{
				case 'd':
					sprintf(&aStr[0], "%d", va_arg(args, int));
					snprintf(&m_String[0], sizeof m_String, "%s%s", &m_String[0], &aStr[0]);
					fmt++;
					break;
				case 'f':
					sprintf(&aStr[0], "%.2f", va_arg(args, double));
					snprintf(&m_String[0], sizeof m_String, "%s%s", &m_String[0], &aStr[0]);
					fmt++;
					break;
				case 'c':
					strcat(&m_String[0], &va_arg(args, char));
					fmt++;
					break;
				case 's':
					strcat(&m_String[0], va_arg(args, const char*));
					fmt++;
					break;
				default:
					strcat(&m_String[0], "< ERROR >");
					break;
			}
		}
		else
		{
			char cData = *fmt;
			snprintf(&m_String[0], sizeof m_String, "%s%c", &m_String[0], cData);
		}
	}

	//出力終了
	va_end(args);
}

//========================
//入力取り込み処理（デバッグモード限定
//========================
void CDebugProc::DebugPrint(const char *fmt, ...)
{
#ifdef _DEBUG
	int nCount;		//カウンタ
	va_list args;	//可変引数の中身

	va_start(args, fmt);//出力開始

	//改行文字が来るまでループ
	for (nCount = 0; *fmt != '\0'; fmt++)
	{
		if (*fmt == '%')
		{
			char aStr[10] = {};
			switch (*(fmt + 1))
			{
			case 'd':
				sprintf(&aStr[0], "%d", va_arg(args, int));
				snprintf(&m_String[0], sizeof m_String, "%s%s", &m_String[0], &aStr[0]);
				fmt++;
				break;
			case 'f':
				sprintf(&aStr[0], "%.2f", va_arg(args, double));
				snprintf(&m_String[0], sizeof m_String, "%s%s", &m_String[0], &aStr[0]);
				fmt++;
				break;
			case 'c':
				strcat(&m_String[0], &va_arg(args, char));
				fmt++;
				break;
			case 's':
				strcat(&m_String[0], va_arg(args, const char*));
				fmt++;
				break;
			default:
				strcat(&m_String[0], "< ERROR >");
				break;
			}
		}
		else
		{
			char cData = *fmt;
			snprintf(&m_String[0], sizeof m_String, "%s%c", &m_String[0], cData);
		}
	}

	//出力終了
	va_end(args);
#endif
}

//========================
//入力取り込み処理（リリースモード限定
//========================
void CDebugProc::ReleasePrint(const char *fmt, ...)
{
#ifdef _DEBUG
#else
	int nCount;		//カウンタ
	va_list args;	//可変引数の中身

	va_start(args, fmt);//出力開始

						//改行文字が来るまでループ
	for (nCount = 0; *fmt != '\0'; fmt++)
	{
		if (*fmt == '%')
		{
			char aStr[10] = {};
			switch (*(fmt + 1))
			{
				case 'd':
					sprintf(&aStr[0], "%d", va_arg(args, int));
					snprintf(&m_String[0], sizeof m_String, "%s%s", &m_String[0], &aStr[0]);
					fmt++;
					break;
				case 'f':
					sprintf(&aStr[0], "%.2f", va_arg(args, double));
					snprintf(&m_String[0], sizeof m_String, "%s%s", &m_String[0], &aStr[0]);
					fmt++;
					break;
				case 'c':
					strcat(&m_String[0], &va_arg(args, char));
					fmt++;
					break;
				case 's':
					strcat(&m_String[0], va_arg(args, const char*));
					fmt++;
					break;
				default:
					strcat(&m_String[0], "< ERROR >");
					break;
			}
		}
		else
		{
			char cData = *fmt;
			snprintf(&m_String[0], sizeof m_String, "%s%c", &m_String[0], cData);
		}
	}

	//出力終了
	va_end(args);
#endif // _DEBUG
}

//========================
//フォント情報設定
//========================
void CDebugProc::SetInfo(int size, D3DXCOLOR color, int nFormatPlace)
{
	m_Size = size;			//サイズ設定
	m_Color = color;		//色設定
	m_Place = nFormatPlace;	//出力位置設定

	SetFont();//フォントを再生成
}

//========================
//フォント設定
//========================
void CDebugProc::SetFont(void)
{
	//フォントを破棄する
	Uninit();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//デバッグ表示用フォント生成
	D3DXCreateFont(pDevice, m_Size, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
}