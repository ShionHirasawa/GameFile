//================================================================================================
//
//�v���C���[����[player.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../Model/model.h"
#include "../Model/motion.h"
#include "../XFile/objectX.h"

//���[����]�I�u�W�F�N�g�̐�
#define NUM_TAKE_OBJECTX	(3)

//���C�t�̌���
#define NUM_LIFE_DEJIT		(3)

//�O���錾
class CPoly2D;
class CShadow;

//�v���C���[�N���X
class CPlayer : public CObject
{
public:
	/*�v���C���[�̃|���S�����E�]��*/ static const float WIDTH_MARGIN;
	/*�v���C���[�̃|���S���㉺�]��*/ static const float HEIGHT_MARGIN;
	/*�v���C���[�̓����蔻��͈�*/	 static const float COLLISION_MARGIN;
	/*�v���C���[�̓����蔻��̒���*/ static const float COLLISION_LENGTH;
	/*�v���C���[�̗D�揇��*/static const int PRIORITY;

	//�v���C���[�̏�ԗ�
	enum STATE{
		STATE_NORMAL = 0,	//�ʏ���
		STATE_DAMAGE,		//�_���[�W���
		STATE_DEATH,		//���S���
		STATE_MAX
	};

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(void);
	void SetPos(const D3DXVECTOR3 pos);

	void GiveDamege(int nDamage = 1);
	
	//�擾�n
	STATE GetState(void)			{ return m_State; }
	int GetTransformID(void)		{ return m_nTransformModelID; }
	int GetDamageInterval(void)		{ return m_nDamageInterval; }
	D3DXVECTOR3 GetPosition(void)	{ return m_pos; }
	D3DXVECTOR3 GetMoving(void)		{ return m_move; }
	float GetMoveLength(void)		{ return D3DXVec3Length(&m_move); }

	//�`���[�g���A������̑���ON/OFF
	void IsTutorialControll(bool bControll);

private:
	static const int MAX_LIFE;			//�ő�̗�
	static const int DAMAGE_INTERVAL;	//�H�炢�d��
	static const int DEATH_INTERVAL;	//���S�C���^�[�o��
	static const int SWITCH_DISP;		//�`��ON/OFF�؂�ւ�����
	static const float PUT_OBJX_LENGTH;	//�]�I�u�W�F�N�g�����o������
	static const float NIMBLE_SPEED;	//���u�r�q�v�̈ړ���
	static const float TOUGH_SPEED;		//���u���x�v�̈ړ���

	static const D3DXVECTOR3 LIFE_BG_POS;	//�̗͔w�i�̊�ʒu
	static const float		 LIFE_BG_WIDTH;	//�̗͔w�i�̕�
	static const float		 LIFE_BG_HEIGHT;//�̗͔w�i�̍���
	static const D3DXCOLOR	 LIFE_BG_COLOR;	//�̗͔w�i�̐F

	static char	*LOGO_TEXTURE_PATH;				//�̗̓��S�̃e�N�X�`��
	static const D3DXVECTOR3 LIFE_LOGO_POS;		//�̗̓��S�̊�ʒu
	static const float		 LIFE_LOGO_WIDTH;	//�̗̓��S�̕�
	static const float		 LIFE_LOGO_HEIGHT;	//�̗̓��S�̍���

	static char	*LIFE_TEXTURE_PATH;		//�̗͂�\���e�N�X�`��
	static const D3DXVECTOR3 LIFE_POS;	//�̗̓|���S���̊�ʒu
	static const float LIFE_WIDTH;		//�̗̓|���S���̕�
	static const float LIFE_HEIGHT;		//�̗̓|���S���̍���
	static const float LIFE_MARGIN;		//�̗̓|���S���ׂ̗Ƃ̊�
	static const float LIFE_TEX_WIDTH;	//�̗̓e�N�X�`���̕�
	static const float LIFE_TEX_HEIGHT;	//�̗̓e�N�X�`���̍���

	static const float SCALING_SPEED;	//�T�C�Y�g�k�̑J�ڃX�s�[�h

	static const int	FOOTPRINT_SPAWN_RANGE;	//���Ղ𔭐�����Ԋu
	static const int	FOOTPRINT_SPAWN_MIN;	//���Ք����̍Œ�C���^�[�o��
	static const int	FOOTPRINT_RANGE;		//���Ղ��юU�点��͈�
	static const float	FOOTPRINT_SIZE;			//���ՃT�C�Y
	static const float	FOOTPRINT_GRAVITY;		//���Ղ𗎉�������d�͗�
	static const float	FOOTPRINT_GRAVITY_CURRECT;//�d�͌W��
	static const float	FOOTPRINT_JUMP;			//���������ۂ̕��V��
	static const float	FOOTPRINT_SPEED;		//��юU�鑬�x
	static const int	FOOTPRINT_LIFE;			//����

	static const float SHADOW_SIZE;	//�e�|���S���̃T�C�Y

	void SetLifePoly2D(void);	//�̗̓|���S��2D����

	void TransformUpdateEntrance(void);	//�ϐg�����]���f���̌��ʂ̍X�V����
	void TransformUpdate_Nimble(void);	//���u�r�q�v�̍X�V����
	void TransformUpdate_Tough(void);	//���u���x�v�̍X�V����
	void TransformUpdate_Take(void);	//���u���[�v�̍X�V����
	void TransformUpdate_Bind(void);	//���u�����v�̍X�V����

	void StateUpdateEntrance(void);	//��Ԃ��Ƃ̍X�V�����͂�������
	void StateUpdate_Damage(void);	//�_���[�W��Ԃ̍X�V����
	void StateUpdate_Death(void);	//���S��Ԃ̍X�V����

	void SetBullet(void);		//�e���ˏ���
	void ActionControl(void);	//�s�����䏈��
	void LoadPartsInfo(void);	//�p�[�c���ǂݎ��
	void Transform(void);		//�ϐg����

	void Goal(void);			//�S�[������

	void CollisionWhole(void);					//�����蔻��܂Ƃ�
	void HumanCollisionWhole(int nType);		//�l�ԏ�Ԃ̂Ƃ��̓����蔻��
	void TransformCollisionWhole(int nType);	//�ϐg��Ԃ̂Ƃ��̓����蔻��

	void TransformSetLife(void);	//�ϐg�������̗͕̑ω�

	void FootPrint(void);	//�������O�Ղ𐶐�
	void Shadow(void);		//�e�ݒ�

	D3DXMATRIX	m_mtxWorld;	//���[���h�}�g���b�N�X
	CModel		*m_apModel[MAX_PARTS];
	int			m_nNumModel;

	int m_nTransformModelID;//�ϐg���郂�f���ԍ��i�l�ԏ�Ԃ�-�P�@����ȊO�͂O�ȏ�̎��R��

	CObjectX	*m_pObjX[NUM_TAKE_OBJECTX];	//�]�I�u�W�F�N�g�����[����
	int			m_nNumTakeObjX;				//���[���Ă���]�I�u�W�F�N�g�̐�

	CPoly2D		*m_pLifeBG;		//�c��̗͂̔w�i
	CPoly2D		*m_pLifeLogo;	//�c��̗͂̃��S
	CPoly2D		*m_pLife[NUM_LIFE_DEJIT];	//�c��̗͂�`�悷��̂Ɏg�p

	CMotion *m_pMotion;		//���[�V�������

	STATE m_State;			//���

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_posOld;	//�O��̈ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_rot;		//����

	int		m_nLifeMax;			//�ő�̗�
	int		m_nLife;			//�̗�
	int		m_nDamageInterval;	//�_���[�W���󂯂Ă���̎���
	int		m_nDeathInterval;	//���S���Ă���̃C���^�[�o��
	bool	m_bSneak;			//�X�j�[�N�����ǂ���
	bool	m_bDisp;			//�`�悷�邩�ǂ���
	float	m_fSpecialMaxSpeed;	//���ɂ��ő�ړ��ʂ̑�����

	bool	m_bTransform;		//�ϐg�\�ǂ���	true�F�ϐg�\	false�F�ϐg�ł��Ȃ�
	bool	m_bScaling;			//�ϐg���̊g�k		true�F�g�咆	false�F�k����
	float	m_fScal;			//�g�k�T�C�Y
	int		m_nNextModelID;		//�ϐg���郂�f���ԍ�

	bool	m_bControll;		//�`���[�g���A������ő���ON/OFF��؂�ւ���

	int m_nMoveCounter;	//���������Ԃ��v��
	int m_nNextFootPrint;//���ɑ��Ղ��o������^�C�~���O

	int m_nShadowID;	//�e�ԍ�
};

#endif