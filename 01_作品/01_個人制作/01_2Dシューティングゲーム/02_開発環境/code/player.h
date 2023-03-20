#ifndef _PLAYER_H_
#define _PLAYER_H_

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);
int ResultChoice(void);

#endif