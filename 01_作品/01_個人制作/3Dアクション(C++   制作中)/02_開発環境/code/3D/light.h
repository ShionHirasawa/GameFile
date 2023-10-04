//================================================================================================
//
//���C�h����[light.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//���C�g�̐�
#define NUM_LIGHT	(3)

//���C�g�N���X
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void LoadVec(void);
	void Uninit(void);
	void Update(void);

private:
	D3DXVECTOR3	m_LightVec[NUM_LIGHT];	//���C�g�̃x�N�g��
	D3DLIGHT9	m_light[NUM_LIGHT];		//���C�g�̏��
};

#endif 