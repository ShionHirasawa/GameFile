//================================================================================================
//
//オブジェクト処理[object.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#define MAX_OBJ		(128)	//オブジェクトの数

//オブジェクトクラス
class CObject
{
public:
	/*  基準の優先順位  */	static const int OBJECT_PRIORITY_DEF;
	/*  最高の優先順位  */	static const int OBJECT_PRIORIRY_MAX;

	//オブジェクトの種類列挙型
	typedef enum
	{
		TYPE_NONE = 0,	// なにもなし
		TYPE_BG,		// 背景
		TYPE_PLAYER,	// プレイヤー
		TYPE_ENEMY,		// 敵
		TYPE_ROBOT,		// ロボット
		TYPE_BULLET,	// 弾
		TYPE_EXPLOSION,	// 爆発
		TYPE_EFFECT,	// エフェクト
		TYPE_PARTICLE,	// パーティクル
		TYPE_NUMBER,	// 数字関連
		TYPE_SCORE,		// スコア
		TYPE_TIMER,		// タイマー
		TYPE_BLOCK,		// ブロック
		TYPE_ITEM,		// アイテム
		TYPE_FLOOR,		// 床
		TYPE_WALL,		// 壁
		TYPE_CEILING,	// 天井
		TYPE_XMODEL,	// Ⅹモデル
		TYPE_JEWEL,		// 宝石（ゴールアイテム)
		TYPE_FADE,		// フェード
		TYPE_POLY_2D,	// 2Dポリゴン
		TYPE_POLY_3D,	// 3Dポリゴン
		TYPE_POLY_BILL,	// 3Dビルボードポリゴン
		TYPE_MESH,		// メッシュポリゴン
		TYPE_MAX
	}TYPE;

	CObject(const int nPriority = OBJECT_PRIORITY_DEF);
	virtual ~CObject();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);			//全オブジェクト破棄
	static void ReleaseAll(TYPE type);		//1種類のオブジェクトすべて破棄
	static void ExceptReleaseAll(TYPE type);//１種類を除いたすべてのオブジェクト破棄
	static void UpdateAll(void);
	static void DrawAll(void);

	//サイズを設定する処理
	virtual void SetWidth(const float fWidth) { ; }
	virtual void SetHeight(const float fHeight) { ; }
	virtual void SetDepth(const float fDepth) { ; }
	virtual void SetSize(const D3DXVECTOR2 size) { ; }
	virtual void SetSize(const D3DXVECTOR3 size) { ; }

	//サイズを取得する処理
	virtual float GetWidth(void) { return 0.0f; }
	virtual float GetHeight(void) { return 0.0f; }
	virtual float GetDepth(void) { return 0.0f; }
	virtual D3DXVECTOR2 GetSize2D(void) { return VEC2_INIT; }
	virtual D3DXVECTOR3 GetSize3D(void) { return VEC3_INIT; }

	// D3DXVECTOR3 の変数を設定する処理
	virtual void SetPosition(D3DXVECTOR3 pos) { pos = pos; }
	virtual void SetMoving(D3DXVECTOR3 move) { move = move; }
	virtual void SetRotation(D3DXVECTOR3 rot) { rot = rot; }
	virtual void SetTransrate(const D3DXVECTOR3 pos, const  D3DXVECTOR3 move, const  D3DXVECTOR3 rot) { ; }

	// D3DXVECTOR3 の変数を取得する処理
	virtual D3DXVECTOR3 GetPosition(void) { return VEC3_INIT; }
	virtual D3DXVECTOR3 GetPosOld(void) { return VEC3_INIT; }
	virtual D3DXVECTOR3 GetMoving(void) { return VEC3_INIT; }
	virtual D3DXVECTOR3 GetRotation(void) { return VEC3_INIT; }
	virtual void GetTransrate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot) { *pPos = GetPosition(); *pMove = GetMoving(); *pRot = GetRotation(); }

	// 頂点情報を取得する処理
	/*	   座標を取得	*/	HRESULT GetVertexPos(D3DXVECTOR3 &rPos, const int nVtxID) { return S_OK; }
	/* 座標と法線を取得 */	HRESULT GetVertexNor(D3DXVECTOR3 &rNor, const int nVtxID) { return S_OK; }

	virtual void UpdatePosOld(void) { ; }

	void SetType(const TYPE type) { m_Type = type; }
	TYPE GetType(void) { return m_Type; }
	static CObject *GetObject(const int nPriority, const int nIdx);
	static CObject *GetTop(int nPriority = OBJECT_PRIORITY_DEF) { return m_pTop[nPriority]; }
	static int		GetNumAll(int nPriority) { return m_nNumAll[nPriority]; }

protected:
	void Release(void);

private:
	static CObject *m_pTop[MAX_PRIORITY];	//先頭のポインタ
	static CObject *m_pCur[MAX_PRIORITY];	//最後尾のポインタ
	CObject *m_pPrev;		//前のポインタ
	CObject *m_pNext;		//次のポインタ

	static int	m_nNumAll[MAX_PRIORITY];//総数
	TYPE		m_Type;			//自分の種類
	int			m_nPriority;	//自分の優先順位
	bool		m_bDeath;		//死亡フラグ
};

#endif