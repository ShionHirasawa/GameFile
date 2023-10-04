//================================================================================================
//
//�r���{�[�h����[billboard.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "../Main/manager.h"
#include "../Object/object.h"

//�r���{�[�h�N���X
class CObjectBillboard : public CObject
{
public:
	CObjectBillboard(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CObjectBillboard();

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�T�C�Y��ݒ肷�鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void SetWidth(const float fWidth)	 { m_fWidth = fWidth; }
	void SetHeight(const float fHeight)  { m_fHeight = fHeight; }
	void SetDepth(const float fDepth)	 { m_fDepth = fDepth; }
	void SetSize(const D3DXVECTOR3 size) { m_fWidth = size.x; m_fHeight = size.y; m_fDepth = size.z; }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�T�C�Y���擾���鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	D3DXVECTOR3 GetSize3D(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth); }

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
	void Move(void)			{ m_pos += m_move; }
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
	/*���W���擾*/	 HRESULT GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID);

	/*�e�N�X�`�����蓖��*/	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	/*�y�e�X�g�����ݒ�*/	virtual void ZTestSwitch(bool bZTest) { m_bZTest = bZTest; }

private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;	// �e�N�X�`���̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@
	D3DXMATRIX				m_mtxWorld;			// ���[���h�}�g���b�N�X

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_posOld;
	D3DXCOLOR	m_col;

	float m_fWidth;		//��
	float m_fHeight;	//����
	float m_fDepth;		//���s��

	static int	m_nTextureID;	// �e�N�X�`���ԍ�
	bool m_bZTest;	//�`�掞��Z�e�X�g�𖳌��ɂ��邩�ǂ����@�@true�F�����ɂ���  false�F�����ɂ��Ȃ�
};

#endif 