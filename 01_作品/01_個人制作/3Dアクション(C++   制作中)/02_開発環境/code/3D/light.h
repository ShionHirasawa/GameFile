//================================================================================================
//
//ライド処理[light.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//ライトの数
#define NUM_LIGHT	(3)

//ライトクラス
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
	D3DXVECTOR3	m_LightVec[NUM_LIGHT];	//ライトのベクトル
	D3DLIGHT9	m_light[NUM_LIGHT];		//ライトの情報
};

#endif 