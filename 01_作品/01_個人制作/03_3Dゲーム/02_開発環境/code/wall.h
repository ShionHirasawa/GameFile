/*==========================================================================================================================================================

															�ǂ̏���[wall.cpp]
															Author:���V����

============================================================================================================================================================*/
#ifndef _WALL_H_
#define _WALL_H_

//�ǂ̕����\����
typedef enum
{
	WallDirection_D = ZERO,	// ������
	WallDirection_R,		// �E����
	WallDirection_F,		// �O����
	WallDirection_L,		// ������
	WallDirection_MAX,
}WallDirection;

//�ǂ̏��\����
typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ����
	float			fWidth;		// ��
	float			fHeight;	// ����
	float			fDepth;		// ���s��
	D3DXVECTOR2		fTexPos;	// �e�N�X�`���̊J�n���W
	float			fTexWidth;	// �e�N�X�`���̕�
	float			fTexHeight;	// �e�N�X�`���̍���
	int				nTexType;	// �e�N�X�`���̎��
	WallDirection	Direction;	// �ǂ������Ă����
	bool			bCollitype;	// �����蔻����s�����ǂ���
}Wall;

void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
Wall *GetWall(void);
void LoadWall(void);
bool WallDrawObject(D3DXVECTOR3 pos);

#endif 