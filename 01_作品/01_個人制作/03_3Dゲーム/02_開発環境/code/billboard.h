/*==========================================================================================================================================================

														ビルボード処理[billboard.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//ビルボードの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
}Billboard;

//ビルボードのαテストなど描画に関わるものの構造体
typedef struct
{
	int AlphaTest;
	bool bAlphaTest;
	bool bZTest;
}BillTest;

void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
BillTest *GetAlphaTest(void);

#endif 