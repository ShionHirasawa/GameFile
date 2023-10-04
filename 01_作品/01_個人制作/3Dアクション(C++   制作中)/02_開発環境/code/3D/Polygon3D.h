//================================================================================================
//
//3D�|���S������[Polygon3D.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _POLYGON_3D_H_
#define _POLYGON_3D_H_

#include "../3D/object3D.h"

//3D�|���S���N���X
class CPoly3D : public CObject3D
{
public:
	CPoly3D(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CPoly3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPoly3D *Create(char *pPath);

	void SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, D3DXVECTOR2 fTexPos = VEC2_INIT, float TexWidth = 1.0f, float TexHeight = 1.0f);
	void DispSwitch(bool bDisp) { m_bDisp = bDisp; }

	//���e�X�g���s�����ǂ���
	void SetAlphaTest(bool bTest) { m_bAlphaTest = bTest; }

	//���Z�������s�����ǂ���
	void SetAddBlend(bool bBlend) { m_bAddBlend = bBlend; }

	//���Z�������s�����ǂ���
	void SetSubBlend(bool bBlend) { m_bSubBlend = bBlend; }

private:
	//�����_���[�̐ݒ���s��  true�F�ݒ�@false�F����
	void SetDrawRender(bool bSet);

	int	m_nTextureID;	// �e�N�X�`���ԍ�
	bool m_bDisp;

	bool m_bAlphaTest;	//���e�X�g���s�����ǂ���
	bool m_bAddBlend;	//���Z�������s�����ǂ���
	bool m_bSubBlend;	//���Z�������s�����ǂ���
};

#endif