//================================================================================================
//
//�G����[enemy.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//�G�̍ő吔
#define MAX_ENEMY		(10)

//�G�N���X
class CEnemy : public CObjectX
{
public:
	CEnemy(const int nPriority = OBJECT_PRIORITY_DEF);
	virtual ~CEnemy();
	virtual HRESULT Init(char *pFilePath);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Draw(D3DXCOLOR *pDiffuse);

	static HRESULT Load(void);

	static CEnemy *Create(char *pFilePath);
	void SetPos(const D3DXVECTOR3 pos);

private:
	void	MoveControl(void);
	static int	m_nTextureID;	// �e�N�X�`���ԍ�
};

//�h�ƃJ�����N���X
class CEnemySecurityCamera : public CEnemy
{
public:
	CEnemySecurityCamera(const int nPriority = OBJECT_PRIORITY_DEF);
	~CEnemySecurityCamera();
	HRESULT Init(char *pFilePath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemySecurityCamera *Create(char *pFilePath);

	//============================================
	//�h�ƃJ�����̐ݒ菈��
	//�����P�@pos	�F�ʒu
	//�����Q�@rot	�F��������
	//�����R�@fPatRot:���G���Ɍ���͈�
	//============================================
	void SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fPatRot);

private:
	static const D3DXCOLOR SEARCH_LIGHT_COLOR;//�T�[�`���C�g�F
	static const float SEARCH_LENGTH;	//���G�����̒���
	static const float SEARCH_RADIUS;	//���G���a

	static const int NUM_DEVIDE;	//�~�����b�V���̕�����
	static const int ALL_VERTEX;	//�~�����b�V���̑S�̒��_�o�b�t�@��
	static const int ALL_INDEX;		//�~�����b�V���̑S�̃C���f�b�N�X�o�b�t�@��

	static const float ROTAION_SPEED;	//��]���x
	static const int ROTAION_INTERVAL;	//��]���~�̃C���^�[�o��

	void Patrol(void);	//���񏈗�
	void SearchLight(void);	//�v���C���[���T�[�`���C�g���ɋ��邩���肷�鏈��
	void Chaice(void);	//�v���C���[��ǂ������鏈��

	HRESULT SetVtxInfo(void);
	HRESULT SetIdxInfo(void);
	void DrawMesh(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@
	D3DXMATRIX				m_mtxWorld;	// ���[���h�}�g���b�N�X

	D3DXVECTOR3 m_CirclePos;	//�T�[�`���C�g�̒��S�ʒu
	bool	m_bFind;			//�v���C���[���T�[�`���C�g�̒��ɂ��邩�ǂ���
	float	m_fPatrolStartRot;	//�p�g���[������J�n�p�x
	float	m_fPatrolEndRot;	//�p�g���[������I���p�x
	float	m_fRotationSpeed;	//�I���p�x�։�]����ۂ̉�]��
	bool	m_bRotation;	//���n���ۂ̉�]�����@�@true�F�I���p�x�ց@�@false�F�J�n�p�x��
	int		m_nRotationInterval;//�p�g���[���̌��E�p�x�ɒB�������́A�ꎞ�I�ɉ�]���~����C���^�[�o��
};

//���{�b�g�N���X
class CEnemyRobot : public CEnemy
{
public:
	//�`�F�b�N�|�C���g�̏��
	struct CheckPoint
	{
		float fMove;		//�ړ���
		int nNumAll;		//�`�F�b�N�|�C���g�̑���
		int nIdx;			//�`�F�b�N�|�C���g�̔ԍ�
		D3DXVECTOR3 *pPoint;//�`�F�b�N�|�C���g�̏ꏊ
	};

	CEnemyRobot(const int nPriority = 1);
	~CEnemyRobot();
	HRESULT Init(char *pFilePath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void GiveDamege(int nDamage = 1);

	static CEnemyRobot *Create(char *pFilePath);

	void SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CheckPoint CheckPoint, int nAttackPower);

private:
	static const float SEARCH_RANGE;	//���G�͈͂̊p�x
	static const float SEARCH_LENGTH;	//���G�͈͂̒���
	static const float CHACE_SPEED;		//�ǐՃX�s�[�h
	static const float ROTATE_SPEED;	//��]��
	static const int MISS_INTERVAL;		//�������Ă�����G���鎞��
	static const int RESTART_INTERVAL;	//�ċN���܂łɂ����鎞��
	static const int PATROL_INTERVAL;	//���񒆂ɗ����~�܂鎞��
	static const int ATTACK_INTERVAL;	//�U���d��

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

	static bool s_bSiren;	//�x������炷���ǂ���

	void PlaySiren(bool bPlay);	//�x������炷����

	void UpdateRestart(void);
	bool SearchPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot);
	bool CollisionPlayer(const D3DXVECTOR3 PlayerPos, const float fPlayerMargin, D3DXVECTOR3 *pPos, const float fMargin);
	void Patrol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot);		//�`�F�b�N�|�C���g�����񂷂�
	bool IsPassCheckPoint(D3DXVECTOR3 TargetPos, D3DXVECTOR3 Pos);				//�`�F�b�N�|�C���g��ʉ߂�����
	void CountInterval(void);	//�C���^�[�o��������������
	bool IsAnyInterval(void);	//�e��C���^�[�o�����c���Ă��邩����
	void CollisionWhole(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot);

	void FootPrint(D3DXVECTOR3 move, D3DXVECTOR3 pos, float rotY);	//�������O�Ղ𐶐�

	CheckPoint	m_CheckPoint;	//�`�F�b�N�|�C���g�̃|�C���^
	int m_nPatrolInterval;		//���񒆂̃`�F�b�N�|�C���g�ɓ��B�������̗����~�܂�C���^�[�o��
	int m_nMissInterval;		//�������Ă�����G����C���^�[�o��
	int m_nReStartInterval;		//�ċN���܂ł̃C���^�[�o��
	int m_nAttackInterval;		//�U���d���C���^�[�o��
	int m_nAttackPower;			//�U����

	int m_nLife;	//�̗�

	int m_nMoveCounter;	//���������Ԃ��v��
	int m_nNextFootPrint;//���ɑ��Ղ��o������^�C�~���O

	int m_nShadowID;	//�e�ԍ�
};

#endif