//================================================================================================
//
//シリンダー処理[Polygon3D.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _POLYGON_3D_H_
#define _POLYGON_3D_H_

#include "../3D/object3D.h"

//シリンダークラス
class CCylinder : public CObject3D
{
public:
	CCylinder(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CCylinder();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCylinder *Create(char *pPath);

	void SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, D3DXVECTOR2 fTexPos = VEC2_INIT, float TexWidth = 1.0f, float TexHeight = 1.0f);
	void DispSwitch(bool bDisp) { m_bDisp = bDisp; }

private:
	int	m_nTextureID;	// テクスチャ番号
	bool m_bDisp;
};

#endif