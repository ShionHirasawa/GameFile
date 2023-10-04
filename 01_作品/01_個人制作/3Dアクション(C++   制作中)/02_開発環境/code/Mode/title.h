//================================================================================================
//
//�^�C�g����ʏ���[title.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "../2D/object2D.h"
#include "../2D/Polygon2D.h"

//�^�C�g���N���X
class CTitle : public CScene
{
public:
	CTitle();
	virtual ~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//====================
	//�ÓI�����o�萔
	//====================
	static char *LOGO_TEXTURE_PATH;		//���S�̃e�N�X�`���p�X
	static const D3DXVECTOR3 LOGO_POS;	//���S�̊�ʒu
	static const float LOGO_WIDTH;		//���S�̕�
	static const float LOGO_HEIGHT;		//���S�̍���

	static char *PRESS_TEXTURE_PATH;	//�v���X�\���̃e�N�X�`���p�X
	static const D3DXVECTOR3 PRESS_POS;	//�v���X�\���̊�ʒu
	static const float PRESS_WIDTH;		//�v���X�\���̕�
	static const float PRESS_HEIGHT;	//�v���X�\���̍���

	CPoly2D	*m_pLogo;	//���S�̃|�C���^
	CPoly2D *m_pPress;	//�v���X�\����UI

	static const int FLASH_TIME;	//�_�ŊԊu
	static const float FLASH_SPEED;	//�_�ŃX�s�[�h
	D3DXCOLOR m_PressColor;			//�v���X�\��UI�̐F�i�����x����
	int m_nFlashCounter;			//�v���X�\��UI�̓_�ŃJ�E���^�[
};

#endif