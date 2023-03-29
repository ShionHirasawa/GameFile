/*==========================================================================================================================================================

ボーナス処理[bonus.h]
Author:藤原龍輝

============================================================================================================================================================*/
#ifndef _BONUS_H_
#define _BONUS_H_

typedef enum
{
	DIRECTION_ZERO = 0,
	DIRECTION_ONE,
	DIRECTION_TWO,
	DIRECTION_THREE,
	DIRECTION_MAX
}BONUS;

//****************************//
//	    ボーナスの構造体      //
//****************************//
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 move;			//位置
	D3DXVECTOR3 rot;			//角度
	bool        buse;			//使用しているかどうか
	BONUS		Respawn;		//出現位置
	int			DespawnLimit;	//消えるリミット
	int			nLife;			//体力
	float		a;				//アルファ値
}Bonus;

//****************************//
//	    プロトタイプ宣言      //
//****************************//
void InitBonus(void);
void UninitBonus(void);
void UpdateBonus(void);
void DrawBonus(void);
void SetBonus(void);
void MoveBonus(void);
void AppearandDisAppearBonus(void);
void CollisionBonus(D3DXVECTOR3 nPlayer,int NumPlayer);
Bonus GetBonus(void);

#endif
