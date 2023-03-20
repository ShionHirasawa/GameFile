#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//�p�[�e�B�N���̎�ލ\����
typedef enum
{
	PARTICLETYPE_STAY = 0,		//���̏�Ŕ���
	PARTICLETYPE_MOVE,			//�ړ�����
	PARTICLETYPE_MAX
}ParticleType;

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, ParticleType nType);

#endif