//================================================================================================
//
//���[�V�����Ǘ�����[motion.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "model.h"

//�L�[�̍ő吔
#define MAX_KEY	(10)

//���[�V�����N���X
class CMotion
{
public:
	//���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	// �ҋ@
		MOTIONTYPE_SNEAK,		// �X�j�[�N
		MOTIONTYPE_WALK,		// ����
		MOTIONTYPE_ACTION,		// �A�N�V����
		MOTIONTYPE_JUMP,		// �V�����v
		MOTIONTYPE_LANDING,		// ���n
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	//�L�[�̍\����
	typedef struct
	{
		float fPosX;	// �ʒu X
		float fPosY;	// �ʒu Y
		float fPosZ;	// �ʒu Z
		float fRotX;	// ���� X
		float fRotY;	// ���� Y
		float fRotZ;	// ���� Z
	} KEY;

	//�L�[���̍\����
	typedef struct
	{
		int nFrame;				// ���[�V�����̍Đ����ԁi�t���[��
		KEY aKey[MAX_PARTS];	// �e���f���̃L�[�v�f
	}KEY_INFO;

	//���[�V�������̍\����
	typedef struct
	{
		bool bLoop;					// ���[�v���邩�ǂ���
		int nNumKey;				// �L�[�̑���
		KEY_INFO aKeyInfo[MAX_KEY];	// �L�[���
	}MOTION_INFO;

	CMotion();
	~CMotion();

	/*	���[�V�����̎�ނ�ݒ�	*/	void Set(int nType);
	/*	���[�V�����̈ʒu�ݒ�	*/	void SetPos(void);
	/*	���[�V�����̍X�V����	*/	void Update(void);
	/*	���[�V�����̎�ނ��擾	*/	int GetType(void) { return m_nType; }
	/*	���[�V�����̏I������	*/	bool IsFinish(void) { return m_bFinish; }

	/*	���[�V��������ݒ�	*/	void SetInfo(MOTION_INFO info);
	/*���[�V���������郂�f���ݒ�*/	void SetModel(CModel **ppModel, int nNumModel);
	/*�p�[�c�̏����ʒu�E�p�x�ݒ�*/	void SetOffset(D3DXVECTOR3 offSet, D3DXVECTOR3 offRot, int nCntSet);
	/*���[�V�����̃u�����h*/		void SetBlend(int nType);
	/*�A�N�V�������\���ǂ���*/	bool IsAction(void) { return m_bAction; }
	/*	  �p�[�c��񃊃Z�b�g	*/	void ResetParts(KEY *pKEY);
	/*	���[�V������񏉊���	*/	void InitMotionInfo(MOTION_INFO *pInfo);

private:
	void StepKey(void);		//���̃L�[�֐i��
	void SetMoveParts(void);//�e�p�[�c�̈ړ��ʂ�ݒ肷��


	MOTION_INFO m_aInfo[MOTIONTYPE_MAX];
	int m_nNumAll;	// �悭�������
	int m_nType;	// ���݂̃��[�V�����̎�ނ��i�[
	bool m_bLoop;	// ���[�v���邩�ǂ����iMOTION_INFO�\���̂ɂ����� "bLoop"�ϐ��Ɖ����Ⴄ�H)
	int m_nNumKey;	// �e���[�V�����̃L�[�̕����������i�[
	int m_nKey;		// ���݂̕����L�[�ԍ�
	int m_nCounter;	// �Đ��o�߃t���[��
	bool m_bFinish;	// �I���������ǂ���

	int		m_nBlendType;	//�u�����h���������[�V�����̎��	
	bool	m_bBlend;		//�u�����h�����ǂ���

	bool m_bAction;	//�e���[�V�������L�̍s�������Ă��ǂ���

	CModel **m_ppModel;	// ���f���ւ̃|�C���^
	int m_nNumModel;	// ���f���̑���

	D3DXVECTOR3 m_OffSetPos[MAX_PARTS];	//�����ʒu
	D3DXVECTOR3 m_OffSetRot[MAX_PARTS];	//�����p�x

	D3DXVECTOR3 m_Pos[MAX_PARTS], m_TargetPos[MAX_PARTS], m_MovePos[MAX_PARTS];
	D3DXVECTOR3 m_Rot[MAX_PARTS], m_TargetRot[MAX_PARTS], m_MoveRot[MAX_PARTS];

	//�f�o�b�O�p�^�C�v�\��
	char *m_pTypeSentence[CMotion::MOTIONTYPE_MAX] = {
		"�ҋ@",
		"�X�j�[�N",
		"����",
		"�A�N�V����",
		"�V�����v",
		"���n",
	};
};

#endif