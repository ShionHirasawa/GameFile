//================================================================================================
//
//��Ώ���[jewel.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _JEWEL_X_
#define _JEWEL_X_

//��΂̔z�u��
#define NUM_JEWEL	(1)

class CMesh;

//��΃N���X
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
	static D3DXVECTOR3	m_pos;			//�o���ʒu
	static D3DXVECTOR3	m_GoalPoint;	//�S�[���n�_
	static float		m_fGoalLength;	//�S�[���̔��a
	static bool			m_bDisp;		//�`�悷�邩���Ȃ���

	static CMesh		*m_pMesh;		//�S�[���\��
};

#endif