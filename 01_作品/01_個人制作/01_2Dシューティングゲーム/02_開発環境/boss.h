#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"

void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(void);
BOSS *GetBoss(void);
void HitBoss(int nDamage, REFLECTBULLET type);

#endif