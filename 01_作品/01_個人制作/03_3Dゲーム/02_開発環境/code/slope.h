/*==========================================================================================================================================================

															�⓹�̏���[slope.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _SLOPE_H_
#define _SLOPE_H_

#include "game.h"

//�⓹�̕���
typedef enum
{
	SLOPEROT_FRONT_BACK = ZERO,		//�O��
	SLOPEROT_RIGHT_LEFT,			//���E
	SLOPEROT_MAX
}SLOPEROT;

//�⓹�̏��\����
typedef struct
{
	D3DXVECTOR3		pos;		//�ʒu
	D3DXVECTOR3		rot;		//����
	float			fWidth;		//��
	float			fHeight;	//����
	float			fDepth;		//���s��
	SLOPEROT		sloperot;	//�⓹�̕���
}Slope;

void InitSlope(void);
void UninitSlope(void);
void UpdateSlope(void);
void DrawSlope(void);
bool CollisionSlope(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, COLLITYPE nType);
void ColliSlopeShadow(D3DXVECTOR3 pos, D3DXVECTOR3 *pRot, float *pDrawPosY, float *pHeight);
Slope *GetSlope(void);
void LoadSlope(void);

#endif