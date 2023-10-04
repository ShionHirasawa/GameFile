//================================================================================================
//
//���b�V������[mesh.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MESH_H_
#define _MESH_H_

#include "../Main/manager.h"
#include "../Object/object.h"

//*****************************
//���b�V���N���X
//*****************************
class CMesh : public CObject
{
public:
	//���b�V���̎��
	enum MESHTYPE
	{
		MESHTYPE_FIELD = 0,	// �t�B�[���h�i���ʁj
		MESHTYPE_FAN,		// �t�@���i��^�`���ʉ~�j
		MESHTYPE_CYLINDER,	// �V�����_�[�i��
		MESHTYPE_DOME,		// �h�[���i����
		MESHTYPE_SPHERE,	// �X�t�B�A�i����
		MESHTYPE_MAX,
	};

	//���b�V���̒��_���E�C���f�b�N�X�����i�[
	struct Buffer
	{
		int nNumVtx;	//���_��
		int nNumIdx;	//�C���f�b�N�X��
		int nNumPoly;	//�|���S����
		MESHTYPE type;	//�^�C�v
	};

	CMesh(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	virtual ~CMesh();
	virtual HRESULT Init(void) { return S_OK; }

	//�V�����_�[�p����������
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, int nDevide, float fRadius, float fHeight, float fTexUSize, float fTexVSize);

	//�t�B�[���h�p����������
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, int nColumn, int nRow, float fTexUSize, float fTexVSize);

	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�T�C�Y��ݒ肷�鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void SetWidth(const float fWidth) { m_fWidth = fWidth; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetDepth(const float fDepth) { m_fDepth = fDepth; }
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
	void SetSize(const D3DXVECTOR4 size) { m_fWidth = size.x; m_fHeight = size.y; m_fDepth = size.z;  m_fRadius = size.w;}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�T�C�Y���擾���鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	float GetRadius(void) { return m_fRadius; }
	D3DXVECTOR4 GetSize4D(void) { return D3DXVECTOR4(m_fWidth, m_fHeight, m_fDepth, m_fRadius); }

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

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�ړ��ɂ܂�鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void UpdatePosOld(void) { m_posOld = m_pos; }
	void Move(void) { m_pos += m_move; }
	void MoveAttenuate(float fMagAttenuate) { m_move += (VEC3_INIT - m_move) * fMagAttenuate; }//�ړ��ʂ������i�����W���͔C�ӂŐݒ�\
	void MoveStop(void) { m_move = VEC3_INIT; }

	//====================================================
	//�e�풸�_����ݒ肷�鏈��
	//====================================================
	/*���_�J���[��ς���*/		HRESULT SetVertexInfo(const D3DXCOLOR col);
	/*���_���W��ς���*/		HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth);
	/*���_���W�E�J���[��ς���*/HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col);
	/*�e�N�X�`�����W��ς���*/	HRESULT SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);
	/*���_�ETex���W��ς���*/	HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);
	/*���_���R���ς���*/	HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);

	//====================================================
	//�e�풸�_�����擾���鏈��
	//====================================================
	/*���W���擾*/	HRESULT GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID);
	/*�`��ON/OFF*/	void DispSwitch(bool bDisp) { m_bDisp = bDisp; }
	/*�^�C�v�擾*/	MESHTYPE GetType(void) { return m_Buffer.type; }

private:
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�V�����_�[�̏������ݒ�
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	HRESULT InitVtxInfo_Cylinder(void);
	HRESULT InitIdxInfo_Cylinder(void);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�t�B�[���h�̏������ݒ�
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	HRESULT InitVtxInfo_Field(void);
	HRESULT InitIdxInfo_Field(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff = NULL;	// �C���f�b�N�X�o�b�t�@
	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_posOld;
	D3DXCOLOR	m_col;

	Buffer m_Buffer;	// �o�b�t�@�[�i�[

	int	m_nTextureID;	// �e�N�X�`���ԍ�
	float m_fTexUSize;	// �e�N�X�`����U�T�C�Y
	float m_fTexVSize;	// �e�N�X�`����V�T�C�Y

	int m_nDevide;		// ���̕������i�V�����_�[�p
	float m_fRadius;	// ���a

	float m_fWidth;		// ��
	float m_fHeight;	// ����
	float m_fDepth;		// ���s��

	bool m_bFieldType;	// ���̃��b�V�����t�B�[���h������ȊO���itrue�F�t�B�[���h   false�F����ȊO
	bool m_bDisp;		// �`�悷�邩�ǂ���
};

//*****************************
//�t�B�[���h���b�V���N���X
//*****************************
class CMeshField : public CMesh
{
public:
	CMeshField(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CMeshField();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshField *Create(void);

private:
	int m_nRow;		//��
	int m_nColumn;	//�s��
};

//*****************************
//���b�V���V�����_�[�N���X
//*****************************
class CMeshCylinder : public CMesh
{
public:
	CMeshCylinder(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CMeshCylinder();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshCylinder *Create(void);

private:
	int m_nDevide;	//������
	float m_fRadius;//���a

};



#endif 