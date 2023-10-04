//================================================================================================
//
//�S�e�N�X�`���Ǘ�����[texture.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//�e�N�X�`���̍ő吔
#define MAX_TEXTURE	(64)

//�e�N�X�`���N���X
class CTexture
{
public:
	CTexture();
	~CTexture();

	/* �e�N�X�`����j������ */	void UnLoad(void);

	/* ����̃e�N�X�`���o�^ */	int Regist(char *pFileName);
	/* �e�N�X�`�����擾���� */	LPDIRECT3DTEXTURE9 GetAddress(const int nIdx);

private:
	/*		�e�N�X�`����ۑ�	*/ LPDIRECT3DTEXTURE9	m_apTexture[MAX_TEXTURE];
	/*		�e�N�X�`���̑���	*/ static int			m_nNumAll;
	/*	�e�N�X�`���̃p�X��ۑ�	*/ static char			*m_pTexPathStrage[MAX_TEXTURE];
};

#endif