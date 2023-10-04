//==========================================
//
//�t�@�C���ǂݍ��ݏ���[file.h]
//Author:Hirasawa Shion
//
//==========================================
#ifndef _FILE_H_
#define _FILE_H_

//�R�[�h��
/*�X�N���v�g�J�n*/	#define CODE_SCRIPT		"SCRIPT"
/*�X�N���v�g�I��*/	#define CODE_END_SCRIPT	"END_SCRIPT"

//���f�����֘A
/*�e�N�X�`���̑���*/#define CODE_NUM_TEXTURE	"NUM_TEXTURE"
/*�e�N�X�`���̃p�X*/#define CODE_TEX_FILENAME	"TEXTURE_FILENAME"
/*�p�[�c�̑���*/	#define CODE_NUM_MODEL		"NUM_MODEL"
/*�t�@�C���p�X*/	#define CODE_MODEL_FILENAME	"MODEL_FILENAME"
/*�]���f���̌�*/	#define CODE_XMODEL_TYPE	"XMODELTYPE"
/*���f���z�u�J�n*/	#define CODE_MODELSET		"MODELSET"
/*���f���z�u�I��*/	#define CODE_END_MODELSET	"END_MODELSET"
/*�v���C���z�u�J�n*/#define CODE_PLAYERSET		"PLAYERSET"
/*�v���C���z�u�I��*/#define CODE_END_PLAYERSET	"END_PLAYERSET"

//���[�V����
/*������ԓǂݎ��J�n*/#define CODE_CHARACTERSET		"CHARACTERSET"
/*������ԓǂݎ��I��*/#define CODE_END_CHARACTERSET	"END_CHARACTERSET"

/* �ړ����x */	#define CODE_MOVE		"MOVE"
/*�W�����v��*/	#define CODE_JUMP		"JUMP"
/* �p�[�c�� */	#define CODE_NUM_PARTS	"NUM_PARTS"
/*�����̔ԍ�*/	#define CODE_INDEX		"INDEX"
/* �e�̔ԍ� */	#define CODE_PARENT		"PARENT"
/*���[�v�ݒ�*/	#define CODE_LOOP		"LOOP"
/*�L�[�̑���*/	#define CODE_NUM_KEY	"NUM_KEY"
/*�t���[����*/	#define CODE_FRAME		"FRAME"

//�p�[�c�ݒ�
/*�ݒ�J�n*/#define CODE_PARTSSET		"PARTSSET"
/*�ݒ�I��*/#define CODE_END_PARTSSET	"END_PARTSSET"

//�L�[�Z�b�g�ݒ�
/*�ݒ�J�n*/#define CODE_KEYSET		"KEYSET"
/*�ݒ�I��*/#define CODE_END_KEYSET	"END_KEYSET"

//�L�[�ݒ�
/*�ݒ�J�n*/#define CODE_KEY		"KEY"
/*�ݒ�I��*/#define CODE_END_KEY	"END_KEY"

//���[�V�����ݒ�
/*�ݒ�J�n*/#define CODE_MOTIONSET		"MOTIONSET"
/*�ݒ�I��*/#define CODE_END_MOTIONSET	"END_MOTIONSET"

//�p�����[�^��
/*�ʒu���*/#define CODE_POS	"POS"
/*�������*/#define CODE_ROT	"ROT"
/*�T�C�Y���*/#define CODE_SIZE	"SIZE"
/*�e�N�X�`���T�C�Y���*/#define CODE_TEX_SIZE	"TEX_SIZE"

//�]�t�@�C��
/*X�t�@�C���̑���*/	#define CODE_NUM_XFILE			"NUM_XFILE"
/*X�t�@�C���p�X*/	#define CODE_OBJECT_FILENAME	"OBJECT_FILENAME"

//�I�u�W�F�N�g
/*�ݒ�J�n*/#define CODE_OBJECTSET		"OBJECTSET"
/*�ݒ�I��*/#define CODE_END_OBJECTSET	"END_OBJECTSET"
/*�I�u�W�F�N�g�̐�*/#define CODE_NUM_OBJECT	"NUM_OBJECT"

//���
/* �ݒ�J�n */#define CODE_JEWELSET		"JEWELSET"
/*�S�[���n�_*/#define CODE_GOAL_POINT	"GOALPOINT"
/*�S�[�����a*/#define CODE_GOAL_LENGTH	"GOALLENGTH"
/* �ݒ�I�� */#define CODE_END_JEWELSET	"END_JEWELSET"

//����
/*�����̐�*/#define CODE_NUM_INTERIOR	"NUM_INTERIOR"

//��
/*�ݒ�J�n*/#define CODE_FLOORSET		"FLOORSET"
/*�ݒ�I��*/#define CODE_END_FLOORSET	"END_FLOORSET"

//��
/*�ݒ�J�n*/#define CODE_WALLSET		"WALLSET"
/*�ݒ�I��*/#define CODE_END_WALLSET	"END_WALLSET"

//�V��
/*�ݒ�J�n*/#define CODE_CEILINGSET		"CEILINGSET"
/*�ݒ�I��*/#define CODE_END_CEILINGSET	"END_CEILINGSET"

//�����Y��
/*�R�����g����*/#define CHR_COMMENT	'#'
/*  �^�u����  */#define CHR_TAB		'\t'
/* ��؂蕶�� */#define CHR_PAUSE	" =\n"
/*  �I�[����  */#define CHR_END		'\0'
/*�J���}��؂�*/#define CHR_COMMA	","
/*  ���s����  */#define CHR_NEWLINE	'\n'

/*���f���̍ő吔*/#define MAX_MODEL	(10)

//�O���錾
class CModel;
class CMotion;
class CObjectX;

//�t�@�C���N���X
class CFile
{
public:
	//���[�V�����ǂݎ���ԗ�
	typedef enum
	{
		MOTION_READSTATE_NONE = 0,		//���̏���ǂݎ��
		MOTION_READSTATE_CHARACTERSET,	//�L�����N�^�[�ݒ�
		MOTION_READSTATE_PARTSSET,		//�p�[�c���ݒ�
		MOTION_READSTATE_MOTIONSET,		//���[�V�����ݒ�
		MOTION_READSTATE_KEYSET,		//���[�V�����̃L�[�ݒ�
		MOTION_READSTATE_KEY,			//�e�p�[�c���Ƃ̃L�[�ݒ�
		MOTION_READSTATE_MAX
	} MOTION_READSTATE;

	//�I�u�W�F�N�g�z�u���ǂݎ���ԗ�
	typedef enum
	{
		OBJECT_READSTATE_NONE = 0,	//���̏�񂪉�����ǂݎ��
		OBJECT_READSTATE_SET,
		OBJECT_READSTATE_MAX
	} OBJECT_READSTATE;

	CFile();
	~CFile();

	void Init(void);
	void Uninit(void);
	void LoadMotion(const char *pPath, CModel **ppModel, CMotion *pMotion, int *pNumParts, int *pParentNumber);
	void LoadStageInterior(const char *pPath);
	void LoadStageObject(const char *pPath);
	void LoadStageEnemy(const char *pPath);

private:
	int LoadInt(char *pString, char *pPunc) { return atoi(strtok(pString, pPunc)); }
	float LoadFloat(char *pString, char *pPunc) { return fatof(strtok(pString, pPunc)); }
	D3DXVECTOR2 LoadVec2(char *pString, char *pPunc);
	D3DXVECTOR3 LoadVec3(char *pString, char *pPunc);
	void LoadCheckPoint(FILE *pFile, float *pMoveSpeed, int *pNumAll, D3DXVECTOR3 *pCheckPoint);

	void DeleteString(char *pString, char cDeleteStr);
	void SetFilePath(char *pStrage, char *pString, char *pPunc);

	bool SetEnemyCamera(char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fPatRot);
	bool SetEnemyRobot(char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fMoveSpeed, int nAttackPower, int *pNumAll, D3DXVECTOR3 *pCheckPoint);
	int m_nNumObjectX;
	static CObjectX *m_pObjectX;
};

#endif