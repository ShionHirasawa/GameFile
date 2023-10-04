//================================================================================================
//
//�`���[�g���A����ʏ���[tutorial.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//�����e�N�X�`���̑���
#define NUM_CONTROLL_TEXTURE	(2)

//�I�u�W�F�N�g�̌����������e�N�X�`���̑���
#define NUM_OBJ_IDENTITY_TEXTURE (4)

//�O���錾
class CPlayer;
class CPoly2D;
class CPolyBill;

//�`���[�g���A���N���X
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//��������̍\����
	struct Controll
	{
		CPoly2D		*pPoly2D;	// �|���S�����
		int			nTexID;		// ���ݕ`�悵�Ă���e�N�X�`���ԍ�
		bool		bDispChange;// �\�����@�؂�ւ��@true�F�����ɕ\��  false�F����ɏ������\��
		D3DXVECTOR3 pos;		// �����e�N�X�`���̈ʒu
		float		fWidth;		// �����e�N�X�`���̕�
		float		fHeight;	// �����e�N�X�`���̍���
	};

	//�I�u�W�F�N�g�̌������̍\����
	struct ObjIdentity
	{
		CPolyBill	*pPolyBill;	// �|���S�����
		D3DXVECTOR3 pos;		// �����e�N�X�`���̈ʒu
		float		fWidth;		// �����e�N�X�`���̕�
		float		fHeight;	// �����e�N�X�`���̍���
		float		fTexV;		// �����e�N�X�`���̃e�N�X�`���u���W
	};

	//��������e�N�X�`�����
	/*�����\���̈ʒu*/ static const D3DXVECTOR3 CONTROLL_TEX_CENTER_POS;
	/*�����\���̕�	*/ static const float		CONTROLL_TEX_CENTER_WIDTH;
	/*�����\���̍���*/ static const float		CONTROLL_TEX_CENTER_HEIGHT;
	/*���ハ�C�v�\���̈ʒu*/ static const D3DXVECTOR3 CONTROLL_TEX_WIPE_POS;
	/*���ハ�C�v�\���̕�  */ static const float		  CONTROLL_TEX_WIPE_WIDTH;
	/*���ハ�C�v�\���̍���*/ static const float		  CONTROLL_TEX_WIPE_HEIGHT;

	//�I�u�W�F�N�g�̌������e�N�X�`�����
	/*�߂��Ō��鎞�̕�	*/ static const float OBJ_IDENTITY_TEX_NEAR_WIDTH;
	/*�߂��Ō��鎞�̍���*/ static const float OBJ_IDENTITY_TEX_NEAR_HEIGHT;
	/*�߂��Ō��鎞�̂x���W*/static const float OBJ_IDENTITY_NEAR_SET_POS_Y;

	/*�����Ō��鎞�̕�	*/ static const float OBJ_IDENTITY_TEX_FAR_WIDTH;
	/*�����Ō��鎞�̍���*/ static const float OBJ_IDENTITY_TEX_FAR_HEIGHT;
	/*�����Ō��鎞�̂x���W*/static const float OBJ_IDENTITY_FAR_SET_POS_Y;
	/*�����Ō��鎞�̃e�N�X�`���x���W*/static const float OBJ_IDENTITY_FAR_TEX_Y;

	/*�T�C�Y�ω������鋗��*/ static const float OBJ_IDENTITY_LENGTH;

	/*�z�u���W*/	static const D3DXVECTOR3 OBJ_IDENTITY_SET_POS[NUM_OBJ_IDENTITY_TEXTURE];

	static char *s_pControllTexturePath[NUM_CONTROLL_TEXTURE];	//��������e�N�X�`���̃t�@�C���p�X
	static char *s_pObjectTexturePath[NUM_OBJ_IDENTITY_TEXTURE];//�I�u�W�F�N�g�̌������e�N�X�`���̃p�X

	CPlayer *m_pPlayer;

	void SetStage(void);
	void TextureLoad(void);
	void SwapControllTexture(void);	//��������|���S���؂�ւ�
	void SwitchDispPoly2D(void);	//��������|���S���̕\��ON/OFF
	void UpdateObjIdentity(void);	//�������̍X�V����

	void End(void);					//�`���[�g���A���I��

	ObjIdentity	m_ObjIdentity[NUM_OBJ_IDENTITY_TEXTURE];
	Controll	m_Controll;
};

#endif