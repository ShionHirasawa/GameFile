//==========================================
//
//�f�o�b�O�\���v���O����[debugproc.cpp]
//Author:Hirasawa Shion
//
//==========================================
#include "main.h"
#include "color.h"
#include "debugproc.h"
#include "renderer.h"
#include "manager.h"
#include <stdarg.h>

//�W���̃t�H���g�T�C�Y
const int CDebugProc::DEFAULT_FONT_SIZE = 18;

//========================
//�R���X�g���N�^
//========================
CDebugProc::CDebugProc()
{
	//�����o�ϐ��N���A
	m_Size = DEFAULT_FONT_SIZE;
	m_Place = DT_LEFT;
	m_Color = GetColor(COL_WHITE);
	memset(&m_String, 0, sizeof m_String);
}

//========================
//�f�X�g���N�^
//========================
CDebugProc::~CDebugProc()
{

}

//========================
//����������
//========================
void CDebugProc::Init(void)
{
	//�t�H���g����
	SetFont();

	//�o�b�t�@�N���A
	memset(&m_String, 0, sizeof m_String);
}

//========================
//�I������
//========================
void CDebugProc::Uninit(void)
{
	//�t�H���g�j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//========================
//�X�V����
//========================
void CDebugProc::Update(void)
{

}

//========================
//�`�揈��
//========================
void CDebugProc::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	
	//�e�L�X�g�`��
	m_pFont->DrawText(NULL, &m_String[0], -1, &rect, m_Place, m_Color);

	//�o�b�t�@�N���A
	memset(&m_String, 0, sizeof m_String);
}

//========================
//���͎�荞�ݏ���
//========================
void CDebugProc::Print(const char *fmt, ...)
{
	int nCount;		//�J�E���^
	va_list args;	//�ψ����̒��g

	va_start(args, fmt);//�o�͊J�n

						//���s����������܂Ń��[�v
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

	//�o�͏I��
	va_end(args);
}

//========================
//���͎�荞�ݏ����i�f�o�b�O���[�h����
//========================
void CDebugProc::DebugPrint(const char *fmt, ...)
{
#ifdef _DEBUG
	int nCount;		//�J�E���^
	va_list args;	//�ψ����̒��g

	va_start(args, fmt);//�o�͊J�n

	//���s����������܂Ń��[�v
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

	//�o�͏I��
	va_end(args);
#endif
}

//========================
//���͎�荞�ݏ����i�����[�X���[�h����
//========================
void CDebugProc::ReleasePrint(const char *fmt, ...)
{
#ifdef _DEBUG
#else
	int nCount;		//�J�E���^
	va_list args;	//�ψ����̒��g

	va_start(args, fmt);//�o�͊J�n

						//���s����������܂Ń��[�v
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

	//�o�͏I��
	va_end(args);
#endif // _DEBUG
}

//========================
//�t�H���g���ݒ�
//========================
void CDebugProc::SetInfo(int size, D3DXCOLOR color, int nFormatPlace)
{
	m_Size = size;			//�T�C�Y�ݒ�
	m_Color = color;		//�F�ݒ�
	m_Place = nFormatPlace;	//�o�͈ʒu�ݒ�

	SetFont();//�t�H���g���Đ���
}

//========================
//�t�H���g�ݒ�
//========================
void CDebugProc::SetFont(void)
{
	//�t�H���g��j������
	Uninit();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�f�o�b�O�\���p�t�H���g����
	D3DXCreateFont(pDevice, m_Size, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
}