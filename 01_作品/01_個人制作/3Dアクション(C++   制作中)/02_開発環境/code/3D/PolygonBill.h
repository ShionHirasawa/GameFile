//================================================================================================
//
//3Dビルボードポリゴン処理[PolygonBill.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _POLYGON_BILL_H_
#define _POLYGON_BILL_H_

#include "billboard.h"

//3Dビルボードポリゴンクラス
class CPolyBill : public CObjectBillboard
{
public:
	//ポリゴン属性
	//この列挙型は後の更新処理等の条件式にて使用され、メンバ変数内で列挙型の数分のbool型から判定される
	enum PROPERTY
	{
		PROPERTY_GRAVITY = 0,//重力
		PROPERTY_MOVE,		//移動
		PROPERTY_LIFE,		//体力
		PROPERTY_MAX,
	};

	CPolyBill(const int nPriority = PRIORITY);
	~CPolyBill();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPolyBill *Create(char *pPath);

	void SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth = 0.0f, D3DXVECTOR2 fTexPos = VEC2_INIT, float TexWidth = 1.0f, float TexHeight = 1.0f);
	void DispSwitch(bool bDisp) { m_bDisp = bDisp; }

	//=========================================
	//足跡を生成する
	//生成する足跡には「重力・移動・体力」属性を付与する
	//詳しくはヘッダファイルにて
	//-----------------------------------------
	//引数１　pos	：発生位置
	//引数２　fRot	：角度
	//引数３　nRange：飛び散り範囲（Rangeが100の場合、飛び散り範囲は +50 ～ -50 の間 * 0.01  になる
	//引数４　fSize	：ポリゴンサイズ
	//------  属性に必要な要素  ---------------
	//引数５　fGravity		：重力
	//引数６　fGravityCorr	：重力係数
	//引数７　fJump			：浮き上がる量
	//引数８　fSpeed		：移動スピード
	//引数９　nLife			：体力
	//=========================================
	static void CreateFootPrint(D3DXVECTOR3 pos, float fRot, int nRange, float fSize, D3DXCOLOR col, float fGravity, float fGravityCorr, float fJump, float fSpeed, int nLife);

	//=========================================
	//属性設定（関数を呼び出し、引数を設定することで属性を付与する
	//=========================================

	//重力属性を設定する
	//１つ目の引数には落下速度を、２つ目の引数には係数を
	void SetProperty(float fGravity, float fCorrect);

	//移動属性を設定する　引数の値が移動量となる
	void SetProperty(D3DXVECTOR3 move);

	//体力属性を設定する
	void SetProperty(int nLife);

private:
	static const int PRIORITY;

	//属性ごとの更新処理（UpdatePropertyEntrance関数はそれぞれの更新処理のまとめ役
	void UpdatePropertyEntrance(void);
	void UpdateProperty_GRAVITY(void);
	void UpdateProperty_MOVE(void);
	void UpdateProperty_LIFE(void);

	int	m_nTextureID;	// テクスチャ番号
	bool m_bDisp;

	bool m_bProperty[PROPERTY_MAX];

	//属性により使用する変数
	float		m_Property_fGravity;	//重力
	float		m_Property_fGravityCorr;//重力係数
	D3DXVECTOR3 m_Property_move;		//移動量
	int			m_Property_nLife;		//寿命

	bool		m_Property_bDead;	//属性の更新処理により死ぬかどうか
};

#endif