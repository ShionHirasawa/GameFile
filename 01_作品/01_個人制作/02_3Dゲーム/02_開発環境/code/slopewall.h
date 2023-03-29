/*==========================================================================================================================================================

															�⓹�̕ǂ̏���[slopewall.h]
																Author:���V����

============================================================================================================================================================*/
#ifndef _SLOPEWALL_H_
#define _SLOPEWALL_H_

#include "slope.h"

//�⓹�̕ǂ̏��\����
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		rot;		//����
	D3DXMATRIX		mtxWorld;	//���[���h�}�g���b�N�X
	float			fWidth;		//��
	float			fHeight;	//����
	float			fDepth;		//���s��
	SLOPEROT		sloperot;	//�⓹�̌���
}SlopeWall;

void InitSlopeWall(void);
void UninitSlopeWall(void);
void UpdateSlopeWall(void);
void DrawSlopeWall(void);
bool CollisionSlopeWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
SlopeWall *GetSlopeWall(void);
void LoadSlopeWall(void);

#endif