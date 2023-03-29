/*==========================================================================================================================================================

													���b�V���t�B�[���h����[meshfield.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _MESHFIELD_H
#define _MESHFIELD_H

#include "main.h"

#define MESHFIELD_SIZE				(600.0f)	//�t�B�[���h�̊�{�T�C�Y

typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ����
	float			fWidth;		// ��
	float			fDepth;		// ���s��
	D3DXVECTOR2		fTexPos;	// �e�N�X�`���̊J�n���W
	float			fTexWidth;	// �e�N�X�`���̕�
	float			fTexHeight;	// �e�N�X�`���̍���
	int				nTexType;	// �e�N�X�`���̎��
}MeshField;

//�v���g�^�C�v�錾
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
MeshField *GetMeshField(void);
bool ColliMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, MeshField **pField);
void ColliFieldShadow(D3DXVECTOR3 Pos, float *pDrawPosY);

void LoadMeshField(void);

#endif