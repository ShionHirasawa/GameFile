//================================================================================================
//
//�w�i����[bg.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _BG_H_
#define _BG_H_

//�w�i�N���X
class CBg : public CObject2D
{
public:
	CBg();
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(char *pPath);

	static CBg *Create(void);

private:
	static int m_nTextureID;	// �e�N�X�`���ԍ�
};

#endif