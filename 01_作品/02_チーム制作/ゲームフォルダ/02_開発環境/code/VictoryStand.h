/*==========================================================================================================================================================

表彰台処理[VictoryStand.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _VICTORYSTAND_H_
#define _VICTORYSTAND_H_

//表彰台の情報構造体
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		rot;			// 向き
	float			fLandheight;	// 着地する高さ
	int				nRank;			// 何位か（モデル描画でも使用する
	bool			bAppear;		// 出現するかどうか
	bool			bUse;			// 使用するかどうか
}VictoryStand;

void RankingSort(void);
void InitVictoryStand(void);
void InitVicStdModel_Tex(int nCntModex);	//modex = "model" + "tex" の造語
void SearchVictoryStand_Land_Pos(void);
void UninitVictoryStand(void);
void UpdateVictoryStand(void);
void DrawVictoryStand(void);

bool CollisionVictoryStand(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold);
void SetVictoryStand(D3DXVECTOR3 Setpos);
void SetAppearVictoryStand(void);
void SkipVicStd_Player(void);

#endif