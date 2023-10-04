//================================================================================================
//
//2Dポリゴン処理[Polygon2D.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _POLYGON_2D_H_
#define _POLYGON_2D_H_

#include "../2D/object2D.h"

//2Dポリゴンクラス
class CPoly2D : public CObject2D
{
public:
	CPoly2D(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CPoly2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPoly2D *Create(char *pPath);
	void SetTexture(char *pPath);

	void SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, D3DXVECTOR2 fTexPos = VEC2_INIT, float TexWidth = 1.0f, float TexHeight = 1.0f);
	void DispSwitch(bool bDisp) { m_bDisp = bDisp; }
	bool GetDisp(void) { return m_bDisp; }

private:
	int		m_nTextureID;	// テクスチャ番号
	bool m_bDisp;
};

#endif