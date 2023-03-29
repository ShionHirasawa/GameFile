/*==========================================================================================================================================================

														�p�[�e�B�N������[parthicle.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//�p�[�e�B�N�������^�C�v
typedef enum
{
	SET_PARTICLE_NORMAL = ZERO,		// ���ʂ̃p�[�e�B�N��(�����ʁ@�P)
	SET_PARTICLE_SLOW_EX,			// �������ړ�����p�[�e�B�N��(������ 30)
	SET_PARTICLE_EXPLOSION,			// �����̂悤�ȃp�[�e�B�N��(�����ʁ@30)
	SET_PARTICLE_MAX
}SET_PARTICLE;

//�p�[�e�B�N���̍\����
typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu(�����ʒu)
	D3DXVECTOR3		move;		// �ړ���
	D3DXCOLOR		col;		// �F
	SET_PARTICLE	nType;		// �����^�C�v
	int				nLife;		// ����(��������)
	float			fRadius;	// �p�[�e�B�N���̔��a
	bool			bUse;		// �g�p���Ă��邩�ǂ���
}Particle;

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticleRadius(int nCntParticle);

void SetParticle(D3DXVECTOR3 pos, SET_PARTICLE nType, D3DXCOLOR col);
void SetParticleNor(D3DXVECTOR3 pos, D3DXCOLOR col, SET_PARTICLE nType);
void SetParticleExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fSpeed, SET_PARTICLE nType);

void BoundParticle(int nCntParticle);

#endif