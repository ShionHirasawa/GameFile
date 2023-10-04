//================================================================================================
//
//数字関連の専属処理[number.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "../2D/object2D.h"

//数字クラス
class CNumber : public CObject2D
{
public:
	CNumber(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CNumber();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	static CNumber *Create(void);

	void SetInfo(const int nNumber, const D3DXVECTOR3 pos, const float fWidth, const float fHeight);
	void SetTexturePos(const int nNumber);

private:
	static int	m_nTextureID;	// テクスチャ番号
	int			m_nNumber;		// 自分の番号
};

#endif