//==========================================
//
//メッシュフィールドのヘッダ[meshfield.h]
//Author:飯田洲暉
//
//==========================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//メッシュフィールドの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;			//フィールドの位置
	D3DXVECTOR3 rot;			//フィールドの向き
	float		fRadius;		//半径
}MESHFIELD;

//プロトタイプ宣言
void InitMeshfield(void);
void SetFieldVertex(void);
void SetFieldIndex(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);
void SetMeshField(int nTexNum, MESHFIELD mf);
MESHFIELD *GetMeshField(void);
bool LandMeshfield(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif 