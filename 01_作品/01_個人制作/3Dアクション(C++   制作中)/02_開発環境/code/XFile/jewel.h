//================================================================================================
//
//宝石処理[jewel.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _JEWEL_X_
#define _JEWEL_X_

//宝石の配置数
#define NUM_JEWEL	(1)

class CMesh;

//宝石クラス
class CJewel : public CObjectX
{
public:
	CJewel();
	~CJewel();

	HRESULT Init(char *pXFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 GoalPoint, float fLength);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void Collision(D3DXVECTOR3 pos);
	static bool GetDisp(void) { return m_bDisp; }
	static void Goal(D3DXVECTOR3 pos);

private:
	static D3DXVECTOR3	m_pos;			//出現位置
	static D3DXVECTOR3	m_GoalPoint;	//ゴール地点
	static float		m_fGoalLength;	//ゴールの半径
	static bool			m_bDisp;		//描画するかしないか

	static CMesh		*m_pMesh;		//ゴール表示
};

#endif