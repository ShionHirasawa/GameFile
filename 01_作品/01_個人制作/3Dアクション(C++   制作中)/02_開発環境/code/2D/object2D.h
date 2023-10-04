//================================================================================================
//
//�I�u�W�F�N�g2D����[object2D.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "../Object/object.h"

//�I�u�W�F�N�g2D�N���X
class CObject2D : public CObject
{
public:
	CObject2D(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	virtual ~CObject2D();

	virtual HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax);
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�T�C�Y��ݒ肷�鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void SetWidth(const float fWidth )	 { m_fWidth  = fWidth;}
	void SetHeight(const float fHeight)  { m_fHeight = fHeight; }
	void SetDepth(const float fDepth)	 { m_fDepth  = fDepth; }
	void SetSize(const D3DXVECTOR2 size) { m_fWidth = size.x; m_fHeight = size.y; }
	void SetSize(const D3DXVECTOR3 size) { m_fWidth = size.x; m_fHeight = size.y; m_fDepth = size.z; }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�T�C�Y���擾���鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	float GetWidth(void)	{ return m_fWidth;	}
	float GetHeight(void)	{ return m_fHeight; }
	float GetDepth(void)	{ return m_fDepth; }
	D3DXVECTOR2 GetSize2D(void) { return D3DXVECTOR2(m_fWidth, m_fHeight); }
	D3DXVECTOR3 GetSize3D(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth); }

	//====================================================
	// D3DXVECTOR3 �̕ϐ���ݒ肷�鏈��
	//====================================================
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	void SetMoving(D3DXVECTOR3 move) { m_move = move; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetTransrate(const D3DXVECTOR3 pos, const  D3DXVECTOR3 move, const  D3DXVECTOR3 rot)	{SetPosition(pos); SetMoving(move); SetRotation(rot);}

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
	void MoveAttenuate(void);				//�ړ��ʂ������i�����W���̓f�t�H���g�l
	void MoveAttenuate(float fMagAttenuate)	{ m_move += (VEC3_INIT - m_move) * fMagAttenuate; }//�ړ��ʂ������i�����W���͔C�ӂŐݒ�\
	void MoveStop(void) { m_move = VEC3_INIT; }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�d�͂Ɋւ���ړ�����
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	/*�d�͂�p�����ړ��ʌ�������*/void MoveAttenuateGravity(const float fGravityPower, const float fGravityMag);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//��ʓ��ɂƂǂ߂�֐�
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	bool KeepOnWindow(void);
	bool KeepOnWindow(const float fWidth, const float fHeight);

	//====================================================
	//�e�풸�_����ݒ肷�鏈��
	//====================================================
	/*���_�J���[��ς���*/		HRESULT SetVertexInfo(const D3DXCOLOR col);
	/*��̒��_�J���[��ς���*/HRESULT SetVertexInfo(const D3DXCOLOR col, const int nVtxID);
	/*���_���W��ς���*/		HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight);
	/*���_���W�E�J���[��ς���*/HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const D3DXCOLOR col);
	/*�e�N�X�`�����W��ς���*/	HRESULT SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);
	/*���_�ETex���W��ς���*/	HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);
	/*���_���R���ς���*/	HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);

	//====================================================
	//�e�풸�_�����擾���鏈��
	//====================================================
	/*���W���擾*/	 HRESULT GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID);

	/*�e�N�X�`�����蓖��*/void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	//�J���[�n
	void		SetMyColor(const D3DXCOLOR col) { m_col = col; }
	D3DXCOLOR	GetMyColor(void) { return m_col; }

	//���̑�
	virtual void Damage(const int nDamage) { ; }
	virtual void SetJump(const bool bJump) { ; }

private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;	// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�̃|�C���^

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_posOld;	//�O��̈ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_rot;		//����
	D3DXCOLOR	m_col;		//�J���[

	float m_fWidth ;//��
	float m_fHeight;//����
	float m_fDepth ;//���s��
};

#endif