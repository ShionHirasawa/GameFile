//================================================================================================
//
//X�t�@�C�����g�p�������f���Ǘ�����[objectX.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _OBJECT_X_
#define _OBJECT_X_

#include "../Object/object.h"

//�]���f���̐�
#define MAX_X_MODEL	(16)

//�O���錾
class CPoly3D;
class CPolyBill;

//�I�u�W�F�N�g�]�N���X
class CObjectX : public CObject
{
public:

	//�]���f���̌�
	enum XMODELTYPE
	{
		XMODELTYPE_NONE = 0,//������
		XMODELTYPE_NIMBLE,	//���u�r�q�v
		XMODELTYPE_TOUGH,	//���u���x�v
		XMODELTYPE_TAKE,	//���u���[�v
		XMODELTYPE_BIND,	//���u�����v
		XMODELTYPE_MAX,
	};

	//�]���f���̍\����
	struct XModel
	{
		LPD3DXMESH		pMesh;		//���b�V���̃|�C���^
		LPD3DXBUFFER	pBuffMat;	//�}�e���A���|�C���^
		DWORD			dwNumMat;	//�}�e���A���̑���
		int				*pTextureID;//�e�N�X�`���ԍ�
		D3DXVECTOR3		MinPos;		//�ŏ��ʒu
		D3DXVECTOR3		MaxPos;		//�ő�ʒu
		float			fMargin;	//�����蔻��͈̔́i�����͍l�����Ȃ�
		XMODELTYPE		Type;		//��
		int				nLife;		//�̗�
	};

	//�擾������W�̃^�C�v��
	enum TYPE
	{
		TYPE_X = 0,	//�]���W
		TYPE_Y,		//�x���W
		TYPE_Z,		//�y���W
		TYPE_MIN,	//�ŏ����W
		TYPE_MAX,	//�ō����W
	};

	CObjectX(const int nPriority = OBJECT_PRIORITY_DEF);
	~CObjectX();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(char *pXFilePath, XMODELTYPE type, int nLife);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void) { CObjectX::Draw(NULL); }
	void	Draw(D3DXCOLOR *pDiffuse = NULL);

	//====================================================
	// D3DXVECTOR3 �̕ϐ���ݒ肷�鏈��
	//====================================================
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	void SetMoving(D3DXVECTOR3 move) { m_move = move; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetTransrate(const D3DXVECTOR3 pos, const  D3DXVECTOR3 move, const  D3DXVECTOR3 rot) { SetPosition(pos); SetMoving(move); SetRotation(rot); }

	//====================================================
	// D3DXVECTOR3 �̕ϐ����擾���鏈��
	//====================================================
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	D3DXVECTOR3 GetMoving(void) { return m_move; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void GetTransrate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot) { *pPos = GetPosition(); *pMove = GetMoving(); *pRot = GetRotation(); }
	static D3DXVECTOR3	GetPosition(int nXModelID, TYPE typeH_L);						//�R���_�̍ŏ����ō����W��Ԃ�
	static D3DXVECTOR3	GetPosition(int nXModelID, TYPE Xtype, TYPE Ytype, TYPE Ztype);	//�R���_�̍ŏ��E�ō����W��Ԃ�
	static float		GetPosition(int nXModelID, TYPE posType, TYPE typeH_L);			//�w�x�y�̂ǂꂩ�̍ŏ��E�ō����W��Ԃ�

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�ړ��ɂ܂�鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void UpdatePosOld(void) { m_posOld = m_pos; }
	void Move(void) { m_pos += m_move; }
	void MoveAttenuate(void);				//�ړ��ʂ������i�����W���̓f�t�H���g�l
	void MoveAttenuate(float fMagAttenuate) { m_move += (VEC3_INIT - m_move) * fMagAttenuate; }//�ړ��ʂ������i�����W���͔C�ӂŐݒ�\
	void MoveStop(void) { m_move = VEC3_INIT; }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�d�͂Ɋւ���ړ�����
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	/*�d�͂�p�����ړ��ʌ�������*/void MoveAttenuateGravity(const float fGravityPower, const float fGravityMag);

	//�`��X�C�b�`ON/OFF
	void DispSwitch(bool bDisp);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�]���f���Ɋ֘A���鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	/* �����̔ԍ����擾*/	int GetMyXModelID(void) { return m_nXmodelID; }
	/* �]���f����o�^ */	int Regist(char *pFileName, XMODELTYPE type, int nLife);
	/* �]���f�����擾 */	static XModel *GetXModel(int nXModelID);
	/* �]���f���̑��� */	static int GetNumXModel(void) { return m_nNumAll; }
	/* �]���f�������[�ł��邩�ǂ���*/ static CObjectX *TakeXModel(D3DXVECTOR3 pos, float rot, float fRangeAngle, float fLength);
	/* �]���f���ɕϐg�ł��邩�ǂ���*/ static int TransformXModel(D3DXVECTOR3 pos, float rot, float fRangeAngle, float fLength, int nTransformModelID);
	/* �����蔻��͈͎̔擾�i�����͍l�����ĂȂ�*/ static float GetXModelMargin(int nXModelID);
	/* �]���f���̌����擾 */ static XMODELTYPE GetXModelType(int nXModelID);
	/* �]���f���̗̑͂��擾 */ static int GetXModelLife(int nXModelID);

	/* �]���f���Ƃ̓����蔻��i�l�ԏ�ԁj*/ static void CollisionXModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, TYPE nType, float fRange, float fMarginSize);
	/* �]���f���Ƃ̓����蔻��i�ϐg��ԁj*/ static void CollisionXModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, TYPE nType, float fMargin, int nXModelID);

private:
	void InitTargetPolygon(void);
	static void SetTargetPolygon(bool bDisp, D3DXVECTOR3 TargetPos = VEC3_INIT, float MaxPosY = 0.0f);

	/*�]���f���̃|�C���^*/	static XModel	*m_apXModel[MAX_X_MODEL];
	/*�t�@�C���̃p�X*/		static char		*m_pFilePath[MAX_X_MODEL];
	/*�]���f���̑���*/		static int		m_nNumAll;
	/*�����̇]���f���̔ԍ�*/int m_nXmodelID;

	D3DXMATRIX	m_mtxWorld;	//���[���h�}�g���b�N�X

	D3DXVECTOR3	m_pos;		//�ʒu
	D3DXVECTOR3	m_posOld;	//�O��ʒu
	D3DXVECTOR3	m_move;		//�ړ���
	D3DXVECTOR3	m_rot;		//����

	bool m_bDisp;	//�`�悷�邩�ǂ���

	static const float SHADOW_SIZE_CORRECT;//�e�|���S���̕␳�l
	int m_nShadowID;	//�e�ԍ�

	//3D�^�[�Q�b�g�̐ÓI���
	static char *TARGET_3D_TEX_PATH;	//�e�N�X�`���p�X
	static const float TARGET_3D_SIZE;	//���E���s���T�C�Y

	//�r���{�[�h�^�[�Q�b�g�̐ÓI���
	static char *TARGET_BILL_TEX_PATH;	//�e�N�X�`���p�X
	static const float TARGET_BILL_SIZE;	//���E�����T�C�Y
	static const float TARGET_BILL_FLOAT;	//�\������ʒu���^�[�Q�b�g�̍ő�x���W���畂�������

	//�ϐg�ł���I�u�W�F�N�g�𕪂���₷���\�����邽�߂̃|���S��
	static CPoly3D		*m_pTarget3D;	//�ϐg�ł���I�u�W�F�N�g�̑����ɕ\��
	static CPolyBill	*m_pTargetBill;	//�ϐg�ł���I�u�W�F�N�g�̓���ɕ\��
};

#endif