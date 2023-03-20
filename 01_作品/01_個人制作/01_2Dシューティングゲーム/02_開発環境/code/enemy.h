#ifndef _ENEMY_H
#define _ENEMY_H

#include "main.h"

//�}�N����`
#define MAX_ENEMY		(40)		//�G�̍ő吔
#define ENEMY_VERTEX	(20.0f)		//�G�̒��S����̒���

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy();
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage, int nType, REFLECTBULLET type);
int GetNumEnemy(void);

#endif