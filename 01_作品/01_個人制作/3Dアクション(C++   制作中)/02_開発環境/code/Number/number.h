//================================================================================================
//
//�����֘A�̐ꑮ����[number.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "../2D/object2D.h"

//�����N���X
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
	static int	m_nTextureID;	// �e�N�X�`���ԍ�
	int			m_nNumber;		// �����̔ԍ�
};

#endif