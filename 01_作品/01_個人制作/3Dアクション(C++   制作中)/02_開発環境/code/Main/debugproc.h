//==========================================
//
//�f�o�b�O�\���v���O����[debugproc.h]
//Author:Hirasawa Shion
//
//==========================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

//�f�o�b�O�\���N���X
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

	LPD3DXFONT	m_pFont;	//�t�H���g�ւ̃|�C���^
	char		m_String[2048];	//�\�������o�b�t�@
	
	D3DXCOLOR	m_Color;	//�t�H���g�̐F
	int			m_Size;		//�t�H���g�T�C�Y
	int			m_Place;	//�\���͈͂̂ǂ��ɏo�͂��邩
};

#endif