//================================================================================================
//
//Xファイルを使用したモデル管理処理[objectX.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _OBJECT_X_
#define _OBJECT_X_

#include "../Object/object.h"

//Ⅹモデルの数
#define MAX_X_MODEL	(16)

//前方宣言
class CPoly3D;
class CPolyBill;

//オブジェクトⅩクラス
class CObjectX : public CObject
{
public:

	//Ⅹモデルの個性
	enum XMODELTYPE
	{
		XMODELTYPE_NONE = 0,//個性無し
		XMODELTYPE_NIMBLE,	//個性「俊敏」
		XMODELTYPE_TOUGH,	//個性「強靭」
		XMODELTYPE_TAKE,	//個性「収納」
		XMODELTYPE_BIND,	//個性「束縛」
		XMODELTYPE_MAX,
	};

	//Ⅹモデルの構造体
	struct XModel
	{
		LPD3DXMESH		pMesh;		//メッシュのポインタ
		LPD3DXBUFFER	pBuffMat;	//マテリアルポインタ
		DWORD			dwNumMat;	//マテリアルの総数
		int				*pTextureID;//テクスチャ番号
		D3DXVECTOR3		MinPos;		//最小位置
		D3DXVECTOR3		MaxPos;		//最大位置
		float			fMargin;	//当たり判定の範囲（高さは考慮しない
		XMODELTYPE		Type;		//個性
		int				nLife;		//体力
	};

	//取得する座標のタイプ列挙
	enum TYPE
	{
		TYPE_X = 0,	//Ⅹ座標
		TYPE_Y,		//Ｙ座標
		TYPE_Z,		//Ｚ座標
		TYPE_MIN,	//最小座標
		TYPE_MAX,	//最高座標
	};

	CObjectX(const int nPriority = OBJECT_PRIORITY_DEF);
	~CObjectX();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(char *pXFilePath, XMODELTYPE type, int nLife);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void) { CObjectX::Draw(NULL); }
	void	Draw(D3DXCOLOR *pDiffuse = NULL);

	//====================================================
	// D3DXVECTOR3 の変数を設定する処理
	//====================================================
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	void SetMoving(D3DXVECTOR3 move) { m_move = move; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetTransrate(const D3DXVECTOR3 pos, const  D3DXVECTOR3 move, const  D3DXVECTOR3 rot) { SetPosition(pos); SetMoving(move); SetRotation(rot); }

	//====================================================
	// D3DXVECTOR3 の変数を取得する処理
	//====================================================
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	D3DXVECTOR3 GetMoving(void) { return m_move; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void GetTransrate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot) { *pPos = GetPosition(); *pMove = GetMoving(); *pRot = GetRotation(); }
	static D3DXVECTOR3	GetPosition(int nXModelID, TYPE typeH_L);						//３頂点の最小か最高座標を返す
	static D3DXVECTOR3	GetPosition(int nXModelID, TYPE Xtype, TYPE Ytype, TYPE Ztype);	//３頂点の最小・最高座標を返す
	static float		GetPosition(int nXModelID, TYPE posType, TYPE typeH_L);			//ＸＹＺのどれかの最小・最高座標を返す

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//移動にまつわる処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void UpdatePosOld(void) { m_posOld = m_pos; }
	void Move(void) { m_pos += m_move; }
	void MoveAttenuate(void);				//移動量を減衰（減衰係数はデフォルト値
	void MoveAttenuate(float fMagAttenuate) { m_move += (VEC3_INIT - m_move) * fMagAttenuate; }//移動量を減衰（減衰係数は任意で設定可能
	void MoveStop(void) { m_move = VEC3_INIT; }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//重力に関する移動処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	/*重力を用いた移動量減衰処理*/void MoveAttenuateGravity(const float fGravityPower, const float fGravityMag);

	//描画スイッチON/OFF
	void DispSwitch(bool bDisp);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Ⅹモデルに関連する処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	/* 自分の番号を取得*/	int GetMyXModelID(void) { return m_nXmodelID; }
	/* Ⅹモデルを登録 */	int Regist(char *pFileName, XMODELTYPE type, int nLife);
	/* Ⅹモデルを取得 */	static XModel *GetXModel(int nXModelID);
	/* Ⅹモデルの総数 */	static int GetNumXModel(void) { return m_nNumAll; }
	/* Ⅹモデルを収納できるかどうか*/ static CObjectX *TakeXModel(D3DXVECTOR3 pos, float rot, float fRangeAngle, float fLength);
	/* Ⅹモデルに変身できるかどうか*/ static int TransformXModel(D3DXVECTOR3 pos, float rot, float fRangeAngle, float fLength, int nTransformModelID);
	/* 当たり判定の範囲取得（高さは考慮してない*/ static float GetXModelMargin(int nXModelID);
	/* Ⅹモデルの個性を取得 */ static XMODELTYPE GetXModelType(int nXModelID);
	/* Ⅹモデルの体力を取得 */ static int GetXModelLife(int nXModelID);

	/* Ⅹモデルとの当たり判定（人間状態）*/ static void CollisionXModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, TYPE nType, float fRange, float fMarginSize);
	/* Ⅹモデルとの当たり判定（変身状態）*/ static void CollisionXModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, TYPE nType, float fMargin, int nXModelID);

private:
	void InitTargetPolygon(void);
	static void SetTargetPolygon(bool bDisp, D3DXVECTOR3 TargetPos = VEC3_INIT, float MaxPosY = 0.0f);

	/*Ⅹモデルのポインタ*/	static XModel	*m_apXModel[MAX_X_MODEL];
	/*ファイルのパス*/		static char		*m_pFilePath[MAX_X_MODEL];
	/*Ⅹモデルの総数*/		static int		m_nNumAll;
	/*自分のⅩモデルの番号*/int m_nXmodelID;

	D3DXMATRIX	m_mtxWorld;	//ワールドマトリックス

	D3DXVECTOR3	m_pos;		//位置
	D3DXVECTOR3	m_posOld;	//前回位置
	D3DXVECTOR3	m_move;		//移動量
	D3DXVECTOR3	m_rot;		//向き

	bool m_bDisp;	//描画するかどうか

	static const float SHADOW_SIZE_CORRECT;//影ポリゴンの補正値
	int m_nShadowID;	//影番号

	//3Dターゲットの静的情報
	static char *TARGET_3D_TEX_PATH;	//テクスチャパス
	static const float TARGET_3D_SIZE;	//幅・奥行きサイズ

	//ビルボードターゲットの静的情報
	static char *TARGET_BILL_TEX_PATH;	//テクスチャパス
	static const float TARGET_BILL_SIZE;	//幅・高さサイズ
	static const float TARGET_BILL_FLOAT;	//表示する位置をターゲットの最大Ｙ座標から浮かせる量

	//変身できるオブジェクトを分かりやすく表示するためのポリゴン
	static CPoly3D		*m_pTarget3D;	//変身できるオブジェクトの足元に表示
	static CPolyBill	*m_pTargetBill;	//変身できるオブジェクトの頭上に表示
};

#endif