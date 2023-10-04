//================================================================================================
//
//�I�u�W�F�N�g����[object.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#define MAX_OBJ		(128)	//�I�u�W�F�N�g�̐�

//�I�u�W�F�N�g�N���X
class CObject
{
public:
	/*  ��̗D�揇��  */	static const int OBJECT_PRIORITY_DEF;
	/*  �ō��̗D�揇��  */	static const int OBJECT_PRIORIRY_MAX;

	//�I�u�W�F�N�g�̎�ޗ񋓌^
	typedef enum
	{
		TYPE_NONE = 0,	// �Ȃɂ��Ȃ�
		TYPE_BG,		// �w�i
		TYPE_PLAYER,	// �v���C���[
		TYPE_ENEMY,		// �G
		TYPE_ROBOT,		// ���{�b�g
		TYPE_BULLET,	// �e
		TYPE_EXPLOSION,	// ����
		TYPE_EFFECT,	// �G�t�F�N�g
		TYPE_PARTICLE,	// �p�[�e�B�N��
		TYPE_NUMBER,	// �����֘A
		TYPE_SCORE,		// �X�R�A
		TYPE_TIMER,		// �^�C�}�[
		TYPE_BLOCK,		// �u���b�N
		TYPE_ITEM,		// �A�C�e��
		TYPE_FLOOR,		// ��
		TYPE_WALL,		// ��
		TYPE_CEILING,	// �V��
		TYPE_XMODEL,	// �]���f��
		TYPE_JEWEL,		// ��΁i�S�[���A�C�e��)
		TYPE_FADE,		// �t�F�[�h
		TYPE_POLY_2D,	// 2D�|���S��
		TYPE_POLY_3D,	// 3D�|���S��
		TYPE_POLY_BILL,	// 3D�r���{�[�h�|���S��
		TYPE_MESH,		// ���b�V���|���S��
		TYPE_MAX
	}TYPE;

	CObject(const int nPriority = OBJECT_PRIORITY_DEF);
	virtual ~CObject();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);			//�S�I�u�W�F�N�g�j��
	static void ReleaseAll(TYPE type);		//1��ނ̃I�u�W�F�N�g���ׂĔj��
	static void ExceptReleaseAll(TYPE type);//�P��ނ����������ׂẴI�u�W�F�N�g�j��
	static void UpdateAll(void);
	static void DrawAll(void);

	//�T�C�Y��ݒ肷�鏈��
	virtual void SetWidth(const float fWidth) { ; }
	virtual void SetHeight(const float fHeight) { ; }
	virtual void SetDepth(const float fDepth) { ; }
	virtual void SetSize(const D3DXVECTOR2 size) { ; }
	virtual void SetSize(const D3DXVECTOR3 size) { ; }

	//�T�C�Y���擾���鏈��
	virtual float GetWidth(void) { return 0.0f; }
	virtual float GetHeight(void) { return 0.0f; }
	virtual float GetDepth(void) { return 0.0f; }
	virtual D3DXVECTOR2 GetSize2D(void) { return VEC2_INIT; }
	virtual D3DXVECTOR3 GetSize3D(void) { return VEC3_INIT; }

	// D3DXVECTOR3 �̕ϐ���ݒ肷�鏈��
	virtual void SetPosition(D3DXVECTOR3 pos) { pos = pos; }
	virtual void SetMoving(D3DXVECTOR3 move) { move = move; }
	virtual void SetRotation(D3DXVECTOR3 rot) { rot = rot; }
	virtual void SetTransrate(const D3DXVECTOR3 pos, const  D3DXVECTOR3 move, const  D3DXVECTOR3 rot) { ; }

	// D3DXVECTOR3 �̕ϐ����擾���鏈��
	virtual D3DXVECTOR3 GetPosition(void) { return VEC3_INIT; }
	virtual D3DXVECTOR3 GetPosOld(void) { return VEC3_INIT; }
	virtual D3DXVECTOR3 GetMoving(void) { return VEC3_INIT; }
	virtual D3DXVECTOR3 GetRotation(void) { return VEC3_INIT; }
	virtual void GetTransrate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot) { *pPos = GetPosition(); *pMove = GetMoving(); *pRot = GetRotation(); }

	// ���_�����擾���鏈��
	/*	   ���W���擾	*/	HRESULT GetVertexPos(D3DXVECTOR3 &rPos, const int nVtxID) { return S_OK; }
	/* ���W�Ɩ@�����擾 */	HRESULT GetVertexNor(D3DXVECTOR3 &rNor, const int nVtxID) { return S_OK; }

	virtual void UpdatePosOld(void) { ; }

	void SetType(const TYPE type) { m_Type = type; }
	TYPE GetType(void) { return m_Type; }
	static CObject *GetObject(const int nPriority, const int nIdx);
	static CObject *GetTop(int nPriority = OBJECT_PRIORITY_DEF) { return m_pTop[nPriority]; }
	static int		GetNumAll(int nPriority) { return m_nNumAll[nPriority]; }

protected:
	void Release(void);

private:
	static CObject *m_pTop[MAX_PRIORITY];	//�擪�̃|�C���^
	static CObject *m_pCur[MAX_PRIORITY];	//�Ō���̃|�C���^
	CObject *m_pPrev;		//�O�̃|�C���^
	CObject *m_pNext;		//���̃|�C���^

	static int	m_nNumAll[MAX_PRIORITY];//����
	TYPE		m_Type;			//�����̎��
	int			m_nPriority;	//�����̗D�揇��
	bool		m_bDeath;		//���S�t���O
};

#endif