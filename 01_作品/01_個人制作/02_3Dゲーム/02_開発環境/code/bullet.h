/*==========================================================================================================================================================

															�e����[bullet.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _BULLET_H
#define _BULLET_H

#include "main.h"

//�e�̎�����\����
typedef enum
{
	BULLET_OWNER_PLAYER = ZERO,	//�v���C���[�̒e
	BULLET_OWNER_CANNON,		//��C�̒e
	BULLET_OWNER_MAX
}BULLET_OWNER;

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		posOld;			// �O��̈ʒu
	D3DXVECTOR3		move;			// �ړ���
	D3DXVECTOR3		rot;			// ����
	D3DXCOLOR		col;			// �F
	int				nLife;			// ����
	float			size;			// �T�C�Y
	int				nIdxShadow;		// �Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
	bool			bUse;			// �g�p���Ă��邩�ǂ���
	BULLET_OWNER	owner;			// �I�[�i�[
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void WholeBulletCollision(int nCntBullet);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float size, BULLET_OWNER owner);

#endif