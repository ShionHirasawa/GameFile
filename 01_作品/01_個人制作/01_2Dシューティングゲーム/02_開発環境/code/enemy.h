#ifndef _ENEMY_H
#define _ENEMY_H

#include "main.h"

//マクロ定義
#define MAX_ENEMY		(40)		//敵の最大数
#define ENEMY_VERTEX	(20.0f)		//敵の中心からの長さ

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy();
Enemy *GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage, int nType, REFLECTBULLET type);
int GetNumEnemy(void);

#endif