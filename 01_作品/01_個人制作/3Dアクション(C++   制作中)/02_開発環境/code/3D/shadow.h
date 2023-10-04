//================================================================================================
//
//�e����[shadow.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//�O���錾
class CPoly3D;

//�e�N���X
class CShadow
{
public:
	CShadow();
	~CShadow();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(D3DXVECTOR3 pos, float fSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static int Create(D3DXVECTOR3 pos, float fSize);
	static void SetPos(int nID, D3DXVECTOR3 pos);
	static void SetSize(int nID, float fSize);

	//-------------------------
	//����̉e�폜
	//��������ݒ肵�Ȃ��ꍇ�A�e�S�폜
	//-------------------------
	//�����@�����̉e�̔ԍ�
	//-------------------------
	static void Delete(int nID);

	//-------------------------
	//�e��S�č폜
	//-------------------------
	static void Delete(void);

	//-------------------------
	//�e�̕`��؂�ւ�
	//-------------------------
	static void DispSwitch(int nID, bool bDisp);

private:
	static const D3DXCOLOR SHADOW_COLOR;
	static int s_nNumAll;
	static char *SHADOW_TEX_PATH;
	static CShadow *s_pTop;
	static CShadow *s_pCur;

	CShadow *m_pNextShadow;//���̉e�I�u�W�F�N�g�̃|�C���^
	CPoly3D *m_pPoly3D;	//�����̃|�C���^
	int		m_nID;		//�����̔ԍ�
};

#endif