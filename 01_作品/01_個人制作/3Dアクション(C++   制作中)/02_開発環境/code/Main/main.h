//================================================================================================
//
//���C������[main.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MAIN_H_	//���̃}�N����`����x������Ă��Ȃ�������
#define _MAIN_H_

#include <Windows.h>
#include <d3d9.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dInput.h"						//���͏����ɕK�v
#include "XInput.h"
#include "xaudio2.h"
#include <stdio.h>
#include <time.h>

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//�}�N����`
//****************************************************************
//�E�B���h�E�n
//****************************************************************
#define CLASS_NAME	"WindowClass"	//�E�C���h�E�N���X�̖��O
#define ORIGIN_POS		(0.0f)		//�E�C���h�E�̃T�C�Y�̌��_
#define MAX_POS			(1.0f)		//�E�C���h�E�̃T�C�Y�̌��E�_
#define SCREEN_WIDTH	(1280)		//�E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)		//�E�C���h�E�̍���
#define HALF_WIDTH		(640.0f)	//�E�C���h�E������
#define HALF_HEIGHT		(360.0f)	//�E�C���h�E��������
#define SCREEN_WIDTH_F	((float)SCREEN_WIDTH)		//�E�C���h�E�̕�
#define SCREEN_HEIGHT_F	((float)SCREEN_HEIGHT)		//�E�C���h�E�̍���
#define SCREEN_CENTER_POS	(D3DXVECTOR3(HALF_WIDTH, HALF_HEIGHT, 0.0f))

//****************************************************************
//VECTOR�n
//****************************************************************
#define VEC2_INIT	(D3DXVECTOR2(0.0f, 0.0f))		//�������p
#define VEC3_INIT	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//�������p
#define NOR_INIT	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	//nor�Ɏg�p

//****************************************************************
//float�n
//****************************************************************
#define NIL_F		(0.0f)
#define HALF		(0.5f)
#define TWICE		(2.0f)

#define FULL_ANGLE	(628)	//�S�̂̊p�x
#define FIX_ANGLE	(314)	//�p�x���C������
#define FIX_FLOAT	(0.01f)	//�p�x��float�^�ɏC��

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�p�x�n
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PI_UP			(D3DX_PI * +1.0f)	/*  ��  */
#define PI_DOWN			(D3DX_PI * 00.0f)	/*  ��  */
#define PI_RIGHT		(D3DX_PI * 00.5f)	/*  �E  */
#define PI_LEFT			(D3DX_PI * -0.5f)	/*  ��  */
#define PI_LEFT_UP		(D3DX_PI * -0.75f)	/* ���� */
#define PI_RIGHT_UP		(D3DX_PI * +0.75f)	/* �E�� */
#define PI_LEFT_DOWN	(D3DX_PI * -0.25f)	/* ���� */
#define PI_RIGHT_DOWN	(D3DX_PI * +0.25f)	/* �E�� */
#define FIX_ROT			(D3DX_PI * 2.0f)	/*�p�x�C��*/
#define HALF_PI			(D3DX_PI * 0.5f)	/*�����̊p�x*/

/***	�ő�FPS	***/
#define MAX_FPS	(60)

/***	2D�p���_�t�H�[�}�b�g	***/
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
/***	3D�p���_�t�H�[�}�b�g	***/
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
/***	rhw�Ɏg�p	***/
#define RHW		(1.0f)

/***	�ǂݍ��ޕ����̍ő吔	***/
#define MAX_STR	(512)
/***	CSV�̋�؂蕶��	***/
#define CSV_DELIMITER	","
/***	���_��	***/
#define	VTX_MAX	(4)
/***	�G���[ID	***/
#define ERROR_ID	(-1)
//�ʏ��atof����double�^�ŕԂ��Ă���̂�float�L���X�g���s���悤�ɂ�������
#define fatof(x)			(float)atof(x)

//���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float		rhw;	//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR	col;	//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g���@nor = Normal
	D3DCOLOR	col;	//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`��
}VERTEX_3D;

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif