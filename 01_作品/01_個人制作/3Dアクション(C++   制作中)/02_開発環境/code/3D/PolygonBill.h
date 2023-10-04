//================================================================================================
//
//3D�r���{�[�h�|���S������[PolygonBill.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _POLYGON_BILL_H_
#define _POLYGON_BILL_H_

#include "billboard.h"

//3D�r���{�[�h�|���S���N���X
class CPolyBill : public CObjectBillboard
{
public:
	//�|���S������
	//���̗񋓌^�͌�̍X�V�������̏������ɂĎg�p����A�����o�ϐ����ŗ񋓌^�̐�����bool�^���画�肳���
	enum PROPERTY
	{
		PROPERTY_GRAVITY = 0,//�d��
		PROPERTY_MOVE,		//�ړ�
		PROPERTY_LIFE,		//�̗�
		PROPERTY_MAX,
	};

	CPolyBill(const int nPriority = PRIORITY);
	~CPolyBill();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPolyBill *Create(char *pPath);

	void SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth = 0.0f, D3DXVECTOR2 fTexPos = VEC2_INIT, float TexWidth = 1.0f, float TexHeight = 1.0f);
	void DispSwitch(bool bDisp) { m_bDisp = bDisp; }

	//=========================================
	//���Ղ𐶐�����
	//�������鑫�Ղɂ́u�d�́E�ړ��E�̗́v������t�^����
	//�ڂ����̓w�b�_�t�@�C���ɂ�
	//-----------------------------------------
	//�����P�@pos	�F�����ʒu
	//�����Q�@fRot	�F�p�x
	//�����R�@nRange�F��юU��͈́iRange��100�̏ꍇ�A��юU��͈͂� +50 �` -50 �̊� * 0.01  �ɂȂ�
	//�����S�@fSize	�F�|���S���T�C�Y
	//------  �����ɕK�v�ȗv�f  ---------------
	//�����T�@fGravity		�F�d��
	//�����U�@fGravityCorr	�F�d�͌W��
	//�����V�@fJump			�F�����オ���
	//�����W�@fSpeed		�F�ړ��X�s�[�h
	//�����X�@nLife			�F�̗�
	//=========================================
	static void CreateFootPrint(D3DXVECTOR3 pos, float fRot, int nRange, float fSize, D3DXCOLOR col, float fGravity, float fGravityCorr, float fJump, float fSpeed, int nLife);

	//=========================================
	//�����ݒ�i�֐����Ăяo���A������ݒ肷�邱�Ƃő�����t�^����
	//=========================================

	//�d�͑�����ݒ肷��
	//�P�ڂ̈����ɂ͗������x���A�Q�ڂ̈����ɂ͌W����
	void SetProperty(float fGravity, float fCorrect);

	//�ړ�������ݒ肷��@�����̒l���ړ��ʂƂȂ�
	void SetProperty(D3DXVECTOR3 move);

	//�̗͑�����ݒ肷��
	void SetProperty(int nLife);

private:
	static const int PRIORITY;

	//�������Ƃ̍X�V�����iUpdatePropertyEntrance�֐��͂��ꂼ��̍X�V�����̂܂Ƃߖ�
	void UpdatePropertyEntrance(void);
	void UpdateProperty_GRAVITY(void);
	void UpdateProperty_MOVE(void);
	void UpdateProperty_LIFE(void);

	int	m_nTextureID;	// �e�N�X�`���ԍ�
	bool m_bDisp;

	bool m_bProperty[PROPERTY_MAX];

	//�����ɂ��g�p����ϐ�
	float		m_Property_fGravity;	//�d��
	float		m_Property_fGravityCorr;//�d�͌W��
	D3DXVECTOR3 m_Property_move;		//�ړ���
	int			m_Property_nLife;		//����

	bool		m_Property_bDead;	//�����̍X�V�����ɂ�莀�ʂ��ǂ���
};

#endif