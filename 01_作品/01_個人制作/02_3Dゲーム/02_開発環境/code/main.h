/*==========================================================================================================================================================

															���C������[main.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _MAIN_H_						//���̃}�N����`����x������Ă��Ȃ�������
#define _MAIN_H_

#include <Windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dInput.h"						//���͏����ɕK�v
#include "XInput.h"
#include "xaudio2.h"
#include <stdio.h>

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//�}�N����`
#define ORIGIN_POS			(0.0f)		//�E�C���h�E�̃T�C�Y�̌��_
#define MAX_POS				(1.0f)		//�E�C���h�E�̃T�C�Y�̌��E�_
#define SCREEN_WIDTH		(1280)		//�E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)		//�E�C���h�E�̍���
#define HALF_WIDTH			(640.0f)	//�E�C���h�E������
#define HALF_HEIGHT			(360.0f)	//�E�C���h�E��������
#define ZERO_SET			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//���Z�b�g�p

//float�^�̍��W�}�N��
#define NIL_F				(0.0f)		//0.0f���W
#define HAL_F				(0.5f)		//0.5f���W
#define MAX_F				(1.0f)		//1.0f���W

//�e�N�X�`�����W�̃}�N��
#define ORIGIN_TEX			D3DXVECTOR2(NIL_F, NIL_F)	//	0			0
#define LIMIT_TEX			D3DXVECTOR2(MAX_F, MAX_F)	//	1			1

#define HALF_TEX			D3DXVECTOR2(HAL_F, HAL_F)	//	0.5f		0.5f
#define ZE_HA_TEX			D3DXVECTOR2(NIL_F, HAL_F)	//	0			0.5f
#define HA_ZE_TEX			D3DXVECTOR2(HAL_F, NIL_F)	//	0.5f		0

#define ZE_MA_TEX			D3DXVECTOR2(NIL_F, MAX_F)	//	0			1.0f
#define MA_ZE_TEX			D3DXVECTOR2(MAX_F, NIL_F)	//	1.0f		0

#define HA_MA_TEX			D3DXVECTOR2(HAL_F, MAX_F)	//	0.5f		1
#define MA_HA_TEX			D3DXVECTOR2(MAX_F, HAL_F)	//	1			0.5f

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				//2D�p���_�t�H�[�}�b�g
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//3D�p���_�t�H�[�}�b�g
#define RHW_NUM				(1.0f)														//rhw�Ɏg�p
#define SET_NOR				D3DXVECTOR3(0.0f, 1.0f, 0.0f)								//nor�Ɏg�p

#define START_CNT	(0)		//for���̏����ݒ�Ɏg�p

/*=========================================================
						�����̃}�N��
===========================================================*/
#define MINUS_ONE	(-1)	
#define ZERO		(0)
#define ONE			(1)
#define SEC			(2)
#define SAN			(3)
#define FOUR		(4)
#define FIVE		(5)

//�����̃}�N���ʃp�^�[��
#define SINGLE		(1)
#define DOUBLE		(2)
#define TRIPLE		(3)
#define QUATRO		(4)

#define MAX_MOJI	(512)	//�ǂݍ��ޕ����̍ő吔

//�񐔂̃}�N��
#define ADD			(1)
#define ONCE		(1)
#define TWICE		(2)

/*=========================================================
						�����̃}�N��
===========================================================*/
#define F_ZERO			(0.0f)
#define F_ONE			(0.1f)
#define F_SEC			(0.2f)
#define F_SAN			(0.3f)
#define F_FOUR			(0.4f)
#define F_FIVE			(0.5f)
#define F_SIX			(0.6f)
#define F_SEV			(0.7f)
#define F_ATE			(0.8f)
#define F_NINE			(0.9f)
#define F_TEN			(1.0f)

//�����̃}�N���ʃp�^�[��
#define F_SINGLE		(1.0f)
#define F_DOUBLE		(2.0f)
#define F_TRIPLE		(3.0f)
#define F_QUATRO		(4.0f)

//�p�x�̃}�N��
#define UP				(D3DX_PI * +1.0f)
#define DOWN			(D3DX_PI * 00.0f)
#define RIGHT			(D3DX_PI * 00.5f)
#define LEFT			(D3DX_PI * -0.5f)
#define FIX_ROT			(D3DX_PI * +2.0f)
#define LEFT_UP			(D3DX_PI * -0.75f)
#define RIGHT_UP		(D3DX_PI * +0.75f)
#define LEFT_DOWN		(D3DX_PI * -0.25f)
#define RIGHT_DOWN		(D3DX_PI * +0.25f)

//�{���̃}�N��
#define HALF_MAG		(0.5f)		//����
#define NORMAL_MAG		(1.0f)		//���{
#define TWICE_MAG		(2.0f)		//�Q�{

#define FIX_FLOAT		(100.0f)	//�p�x��float�^�ɏC��

//��ʃ��[�h�̎��
typedef enum
{
	MODE_TITLE = ZERO,	//�^�C�g�����
	MODE_TUTORIAL,		//�`���[�g���A�����
	MODE_GAME,			//�Q�[�����
	MODE_RESULT,		//���U���g���
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

//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g���@nor = Normal
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`��
}VERTEX_3D;

//�v���C���[�h�\����
typedef enum
{
	PLAYMODE_MODEL = ZERO,	//���f������\
	PLAYMODE_FIELD,			//�n�ʂ̑���\
	PLAYMODE_WALL,			//�ǂ̑���\
	PLAYMODE_STAGE,			//�n�ʂƕǂ̑���\
	PLAYMODE_CAMERA,		//�J��������\
	PLAYMODE_MAX
}PLAYMODE;

//4�̒��_���W�̗񋓌^
typedef enum
{
	VTX_LE_UP = ZERO,	//����
	VTX_RI_UP,			//�E��
	VTX_LE_DO,			//����
	VTX_RI_DO,			//�E��
	VTX_MAX
}VTX;

//FPS�̎��
typedef enum
{
	FPS_30 = 30,
	FPS_60 = 60,
	FPS_120 = 120,
	FPS_MAX = 3,
}FPS_TIME;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(PLAYMODE mode);
void DrawFPSPlayer(void);
void DrawFPSField(void);
void DrawFPSWall(void);
void DrawFPSStage(void);
void DrawFPSCamera(void);
void DrawFPSALL(void);
void DrawFPSSecond(PLAYMODE mode);
void SetMode(MODE mode);
MODE GetMode(void);
void VectorProduct(float ParentX, float ChildZ, float ParentZ, float ChildX, float *pAnswer, float fMagnification);	// �O�όv�Z
void InnerProduct(float Rot, D3DXVECTOR3 vecMove, D3DXVECTOR3 *pMove);												// ���όv�Z

#endif