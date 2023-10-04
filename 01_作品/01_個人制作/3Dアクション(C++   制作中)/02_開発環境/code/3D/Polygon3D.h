//================================================================================================
//
//3Dポリゴン処理[Polygon3D.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _POLYGON_3D_H_
#define _POLYGON_3D_H_

#include "../3D/object3D.h"

//3Dポリゴンクラス
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

	//αテストを行うかどうか
	void SetAlphaTest(bool bTest) { m_bAlphaTest = bTest; }

	//加算合成を行うかどうか
	void SetAddBlend(bool bBlend) { m_bAddBlend = bBlend; }

	//減算合成を行うかどうか
	void SetSubBlend(bool bBlend) { m_bSubBlend = bBlend; }

private:
	//レンダラーの設定を行う  true：設定　false：解除
	void SetDrawRender(bool bSet);

	int	m_nTextureID;	// テクスチャ番号
	bool m_bDisp;

	bool m_bAlphaTest;	//αテストを行うかどうか
	bool m_bAddBlend;	//加算合成を行うかどうか
	bool m_bSubBlend;	//減算合成を行うかどうか
};

#endif