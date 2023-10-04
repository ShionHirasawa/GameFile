//================================================================================================
//
//メッシュ処理[mesh.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MESH_H_
#define _MESH_H_

#include "../Main/manager.h"
#include "../Object/object.h"

//*****************************
//メッシュクラス
//*****************************
class CMesh : public CObject
{
public:
	//メッシュの種類
	enum MESHTYPE
	{
		MESHTYPE_FIELD = 0,	// フィールド（平面）
		MESHTYPE_FAN,		// ファン（扇型～平面円）
		MESHTYPE_CYLINDER,	// シリンダー（筒
		MESHTYPE_DOME,		// ドーム（半球
		MESHTYPE_SPHERE,	// スフィア（球体
		MESHTYPE_MAX,
	};

	//メッシュの頂点数・インデックス数を格納
	struct Buffer
	{
		int nNumVtx;	//頂点数
		int nNumIdx;	//インデックス数
		int nNumPoly;	//ポリゴン数
		MESHTYPE type;	//タイプ
	};

	CMesh(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	virtual ~CMesh();
	virtual HRESULT Init(void) { return S_OK; }

	//シリンダー用初期化処理
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, int nDevide, float fRadius, float fHeight, float fTexUSize, float fTexVSize);

	//フィールド用初期化処理
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, int nColumn, int nRow, float fTexUSize, float fTexVSize);

	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//サイズを設定する処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void SetWidth(const float fWidth) { m_fWidth = fWidth; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetDepth(const float fDepth) { m_fDepth = fDepth; }
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
	void SetSize(const D3DXVECTOR4 size) { m_fWidth = size.x; m_fHeight = size.y; m_fDepth = size.z;  m_fRadius = size.w;}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//サイズを取得する処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	float GetRadius(void) { return m_fRadius; }
	D3DXVECTOR4 GetSize4D(void) { return D3DXVECTOR4(m_fWidth, m_fHeight, m_fDepth, m_fRadius); }

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

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//移動にまつわる処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void UpdatePosOld(void) { m_posOld = m_pos; }
	void Move(void) { m_pos += m_move; }
	void MoveAttenuate(float fMagAttenuate) { m_move += (VEC3_INIT - m_move) * fMagAttenuate; }//移動量を減衰（減衰係数は任意で設定可能
	void MoveStop(void) { m_move = VEC3_INIT; }

	//====================================================
	//各種頂点情報を設定する処理
	//====================================================
	/*頂点カラーを変える*/		HRESULT SetVertexInfo(const D3DXCOLOR col);
	/*頂点座標を変える*/		HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth);
	/*頂点座標・カラーを変える*/HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col);
	/*テクスチャ座標を変える*/	HRESULT SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);
	/*頂点・Tex座標を変える*/	HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);
	/*頂点情報３種を変える*/	HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);

	//====================================================
	//各種頂点情報を取得する処理
	//====================================================
	/*座標を取得*/	HRESULT GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID);
	/*描画ON/OFF*/	void DispSwitch(bool bDisp) { m_bDisp = bDisp; }
	/*タイプ取得*/	MESHTYPE GetType(void) { return m_Buffer.type; }

private:
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//シリンダーの初期化設定
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	HRESULT InitVtxInfo_Cylinder(void);
	HRESULT InitIdxInfo_Cylinder(void);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//フィールドの初期化設定
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	HRESULT InitVtxInfo_Field(void);
	HRESULT InitIdxInfo_Field(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff = NULL;	// インデックスバッファ
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_posOld;
	D3DXCOLOR	m_col;

	Buffer m_Buffer;	// バッファー格納

	int	m_nTextureID;	// テクスチャ番号
	float m_fTexUSize;	// テクスチャのUサイズ
	float m_fTexVSize;	// テクスチャのVサイズ

	int m_nDevide;		// 横の分割数（シリンダー用
	float m_fRadius;	// 半径

	float m_fWidth;		// 幅
	float m_fHeight;	// 高さ
	float m_fDepth;		// 奥行き

	bool m_bFieldType;	// このメッシュがフィールドかそれ以外か（true：フィールド   false：それ以外
	bool m_bDisp;		// 描画するかどうか
};

//*****************************
//フィールドメッシュクラス
//*****************************
class CMeshField : public CMesh
{
public:
	CMeshField(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CMeshField();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshField *Create(void);

private:
	int m_nRow;		//列数
	int m_nColumn;	//行数
};

//*****************************
//メッシュシリンダークラス
//*****************************
class CMeshCylinder : public CMesh
{
public:
	CMeshCylinder(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CMeshCylinder();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshCylinder *Create(void);

private:
	int m_nDevide;	//分割数
	float m_fRadius;//半径

};



#endif 