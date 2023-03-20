#ifndef _MAIN_H_						//���̃}�N����`����x������Ă��Ȃ�������
#define _MAIN_H_

#include <Windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dInput.h"						//���͏����ɕK�v
#include "xaudio2.h"
#include <stdio.h>

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

//�}�N����`
#define ORIGIN_POS			(0.0f)		//�E�C���h�E�̃T�C�Y�̌��_
#define MAX_POS				(1.0f)		//�E�C���h�E�̃T�C�Y�̌��E�_
#define SCREEN_WIDTH		(1280)		//�E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)		//�E�C���h�E�̍���
#define HALF_WIDTH			(640.0f)	//�E�C���h�E������
#define HALF_HEIGHT			(360.0f)	//�E�C���h�E��������

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���_�t�H�[�}�b�g
#define RHW_NUM				(1.0f)	//rhw�Ɏg�p

#define COLOR_MAX	(255)	//RGBACOLOR�̍ő吔

#define START_CNT	(0)		//for���̏����ݒ�Ɏg�p

/*
ZERO
ZERO
ONE
ONE
SEC
SEC
SAN
SAN
*/

//�����̃}�N��
#define MINUS_ONE	(-1)	
#define ZERO		(0)
#define ONE			(1)
#define SEC			(2)
#define SAN			(3)
#define FOUR		(4)
#define FIVE		(5)

#define NUM_VERTEX	(4)		//�|�C���^�Ⓒ�_���W���S�i�߂�̂Ɏg�p

//�����̃}�N��
#define F_ZERO		(0.0f)
#define F_ONE		(0.1f)
#define F_SEC		(0.2f)
#define F_SAN		(0.3f)
#define F_FOUR		(0.4f)
#define F_FIVE		(0.5f)
#define F_SIX		(0.6f)
#define F_SEV		(0.7f)
#define F_ATE		(0.8f)
#define F_NINE		(0.9f)
#define F_TEN		(1.0f)

//POS.Z�Ɏg�p
#define POS_Z		(0.0f)

//�p�x�̃}�N��
#define UP			(1.0f)
#define DOWN		(0.0f)
#define RIGHT		(0.5f)
#define LEFT		(-0.5f)
#define UP_RIGHT	(0.75f)
#define UP_LEFT		(-0.75f)
#define DOWN_RIGHT	(0.25f)
#define DOWN_LEFT	(-0.25f)

//��ʃ��[�h�̎��
typedef enum
{
	MODE_TITLE = 0,	//�^�C�g�����
	MODE_TUTORIAL,	//�`���[�g���A�����
	MODE_GAME,		//�Q�[�����
	MODE_RESULT,	//���U���g���
	MODE_RANKING,	//�����L���O���
	MODE_MAX
}MODE;

//���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

//�e�̎��
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//�v���C���[�̒e
	BULLETTYPE_ENEMY,		//�G�̒e
	BULLETTYPE_BOSS,		//�{�X�̒e
	BULLETTYPE_MAX,
}BULLETTYPE;

//�z�[�~���O���\�̍\����
typedef enum
{
	HOMING_POSSIBLE = 0,	//�z�[�~���O�\
	HOMING_IMPOSSIBLE,		//�z�[�~���O�s�\
	HOMING_MAX
}HOMINGTYPE;

//�e�̔��ˊԊu
typedef enum
{
	BULLETINTERVAL_NORMAL = 180,	//�ʏ�e
	BULLETINTERVAL_SPEED  = 90,		//�X�s�[�h�e
	BULLETINTERVAL_RUSH1  = 90,		//3�A�e(���e)
	BULLETINTERVAL_RUSH2  = 95,		//3�A�e(���e)
	BULLETINTERVAL_RUSH3  = 100,	//3�A�e(�I�e)
	BULLETINTERVAL_BEAM   = 220,	//�r�[���e
	BULLETINTERVAL_MAX
}BULLETINTERVAL;

//�e�̃X�s�����
typedef enum
{
	REFLECTBULLET_NO = 0,		//���˕Ԃ���Ă��Ȃ�
	REFLECTBULLET_YES,			//���˕Ԃ��ꂽ
	REFLECTBULLET_MAX
}REFLECTBULLET;

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXCOLOR col;			//�F
	int nLife;				//����
	BULLETTYPE type;		//�e�̎��
	float EffectAlphaColor;	//�e�̋O�Ղ̃��l���w�肷��ϐ�
	HOMINGTYPE homingtype;	//�z�[�~���O���\�̗L��
	REFLECTBULLET Reflect;	//���˕Ԃ��ꂽ���ǂ���
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Bullet;

//�r�[���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXCOLOR col;			//�F
	BULLETTYPE type;		//�e�̎��
	REFLECTBULLET Reflect;	//���˕Ԃ��ꂽ���ǂ���
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Beam;

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		//�o����ԁi�_�Łj
	PLAYERSTATE_NORMAL,			//�ʏ���
	PLAYERSTATE_SPIN,			//�X�s�����
	PLAYERSTATE_DAMAGE,			//�_���[�W���
	PLAYERSTATE_WAIT,			//�o���҂���ԁi�\���Ȃ��j
	PLAYERSTATE_DEATH,			//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 rot;			//����
	int nLife;					//�̗�
	PLAYERSTATE state;			//�v���C���[�̏��
	int LimitSpin;				//�X�s���ɂ�钵�˕Ԃ��̏��
	int SpinInterval;			//���ɃX�s���ł���܂ł̃C���^�[�o��
	int PlayerSwitch;			//�_�ŕ\��ON/OFF
	int AppearCnt;				//�_�ŕ\�����Ԃ̃J�E���g
	int nCounterState;			//��ԊǗ��J�E���g
	float fLength;				//�Ίp���̒���
	float fAngle;				//�Ίp���̊p�x
	BULLETTYPE myBullet;		//�e�̎��
	HOMINGTYPE homingtype;		//�z�[�~���O���\�̗L��
	bool bDisp;					//�v���C���[��\��ON/OFF
}Player;

//BOSS�̏��
typedef enum
{
	BOSSSTATE_APPEAR = 0,	//�o����ԁi�_�Łj
	BOSSSTATE_NORMAL,		//�ʏ���
	BOSSSTATE_SPIN,			//�X�s�����
	BOSSSTATE_DAMAGE,		//�_���[�W���
	BOSSSTATE_WAIT,			//�o���҂���ԁi�\���Ȃ��j
	BOSSSTATE_DEATH,		//���S���
	BOSSSTATE_MAX
}BOSSSTATE;

//�{�X�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 rot;			//����
	int nLife;					//�̗�
	BOSSSTATE state;			//�v���C���[�̏��
	int SpinInterval;			//���ɃX�s���ł���܂ł̃C���^�[�o��
	int BossAttackCnt;			//�{�X�̍U���Ԋu
	int nCounterState;			//��ԊǗ��J�E���g
	float fLength;				//�Ίp���̒���
	float fAngle;				//�Ίp���̊p�x
	BULLETTYPE BulletType;		//�e�̎��
	HOMINGTYPE homingtype;		//�z�[�~���O���\�̗L��
	bool bDisp;					//�v���C���[��\��ON/OFF
}BOSS;

//�G�̃_���[�W���
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//�ʏ�
	ENEMYSTATE_DAMAGE,			//�_���[�W���
	ENEMYSTATE_MAX,
}ENEMYSTATE;

//�G�\����
typedef struct
{
	D3DXVECTOR3 pos;						//�ʒu
	float Standardpos;						//�~�����Ă����~����܂ł̏ꏊ
	D3DXVECTOR3 move;						//�ړ���
	float fLength;							//�Ίp���̒���
	float fAngle;							//�Ίp���̊p�x
	int nType;								//���
	int nLife;								//�̗�
	int nLifeMax;							//�̗͂̍ő�l
	float Return;							//�ړ��܂�Ԃ�
	ENEMYSTATE state = ENEMYSTATE_NORMAL;	//���
	int nCounterState;						//��ԊǗ��J�E���^�[
	int firstInterval;						//�o������e���ˉ\��Ԃ܂ł̃C���^�[�o��
	BULLETTYPE enBullet;					//�e�̎��
	HOMINGTYPE homingtype;					//�z�[�~���O���\�̗L��
	int enemyAtackCounter;					//�G�̍U���^�[��
	bool bUse;								//�g�p�̗L��
}Enemy;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif