//================================================================================================
//
//3Dビルボードポリゴン処理[PolygonBill.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "PolygonBill.h"
#include "../S_H_Lib/S_H_Calculation.h"

//優先順位
const int CPolyBill::PRIORITY = 2;

//=======================================
//コンストラクタ
//=======================================
CPolyBill::CPolyBill(const int nPriority) : CObjectBillboard(nPriority)
{
	//メンバ変数クリア
	m_bDisp = true;

	//属性によるメンバ変数クリア
	m_Property_bDead = false;
	m_Property_fGravity = 0.0f;
	m_Property_fGravityCorr = 0.0f;
	m_Property_move = VEC3_INIT;
	m_Property_nLife = 0;
}

//=======================================
//デストラクタ
//=======================================
CPolyBill::~CPolyBill()
{

}

//=======================================
//生成処理
//=======================================
CPolyBill *CPolyBill::Create(char *pPath)
{
	//ポリゴンインスタンス生成
	CPolyBill *pPoly3D = new CPolyBill;

	//インスタンスを生成できた
	if (pPoly3D != NULL)
	{
		//初期化処理
		pPoly3D->Init();

		//テクスチャ読み込み
		pPoly3D->m_nTextureID = CManager::GetTexture()->Regist(pPath);

		//テクスチャ割り当て
		pPoly3D->BindTexture(CManager::GetTexture()->GetAddress(pPoly3D->m_nTextureID));
	}

	//インスタンスを返す
	return pPoly3D;
}

//=======================================
//初期化処理
//=======================================
HRESULT CPolyBill::Init(void)
{
	//頂点座標の初期化
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_POLY_BILL);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CPolyBill::Uninit(void)
{
	//頂点バッファの破棄など
	CObjectBillboard::Uninit();
}

//=======================================
//更新処理
//=======================================
void CPolyBill::Update(void)
{
	//属性ごとの更新処理
	UpdatePropertyEntrance();

	//死亡フラグが立っていたら破棄
	if (m_Property_bDead)
	{
		CObjectBillboard::Uninit();
	}
}

//=======================================
//描画処理
//=======================================
void CPolyBill::Draw(void)
{
	if (!m_bDisp) return;

	//描画
	CObjectBillboard::Draw();
}

//=======================================
//属性ごとの更新処理の窓口
//=======================================
void CPolyBill::UpdatePropertyEntrance(void)
{
	for (int nCntProperty = 0; nCntProperty < PROPERTY_MAX; nCntProperty++)
	{
		//対象の属性が付いていないなら処理を飛ばす
		if (!m_bProperty[nCntProperty]) continue;

		switch (nCntProperty)
		{
			case PROPERTY_GRAVITY:	UpdateProperty_GRAVITY(); break;
			case PROPERTY_MOVE:		UpdateProperty_MOVE();	break;
			case PROPERTY_LIFE:		UpdateProperty_LIFE(); break;
		}

		//死ぬなら更新処理をしない
		if (m_Property_bDead) break;
	}
}

//=======================================
//「重力」属性の更新処理
//=======================================
void CPolyBill::UpdateProperty_GRAVITY(void)
{
	//重力処理
	m_Property_move.y += (m_Property_fGravity - m_Property_move.y) * m_Property_fGravityCorr;

	//移動量設定
	SetMoving(m_Property_move);
}

//=======================================
//「移動」属性の更新処理
//=======================================
void CPolyBill::UpdateProperty_MOVE(void)
{
	//移動処理
	Move();
}

//=======================================
//「体力」属性の更新処理
//=======================================
void CPolyBill::UpdateProperty_LIFE(void)
{
	//体力減少
	m_Property_nLife--;

	//体力が尽きたら死亡フラグを立てる
	if (m_Property_nLife <= 0)
	{
		m_Property_bDead = true;
	}
}

//=======================================
//ポリゴン情報の設定処理
//=======================================
void CPolyBill::SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, D3DXVECTOR2 fTexPos, float TexWidth, float TexHeight)
{
	//位置設定
	SetPosition(pos);

	//頂点座標・テクスチャ座標の設定
	SetVertexInfo(fWidth, fHeight, fDepth, fTexPos, TexWidth, TexHeight);
}

//=======================================
//重力属性を設定する
//１つ目の引数には落下速度を、２つ目の引数には係数を
//=======================================
void CPolyBill::SetProperty(float fGravity, float fCorrect)
{
	//重力属性有効
	m_bProperty[PROPERTY_GRAVITY] = true;
	m_Property_fGravity = fGravity;
	m_Property_fGravityCorr = fCorrect;
}

//=======================================
//移動属性を設定する　引数の値が移動量となる
//=======================================
void CPolyBill::SetProperty(D3DXVECTOR3 move)
{
	//移動属性有効
	m_bProperty[PROPERTY_MOVE] = true;
	m_Property_move = move;
	SetMoving(move);
}

//=======================================
//体力属性を設定する
//=======================================
void CPolyBill::SetProperty(int nLife)
{
	m_bProperty[PROPERTY_LIFE] = true;
	m_Property_nLife = nLife;
}

//=========================================
//足跡を生成する
//生成する足跡には「重力・移動・体力」属性を付与する
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
void CPolyBill::CreateFootPrint(D3DXVECTOR3 pos, float fRot, int nRange, float fSize, D3DXCOLOR col, float fGravity, float fGravityCorr, float fJump, float fSpeed, int nLife)
{
	//飛び散る向き
	const float fRotRange = (rand() % nRange - (nRange * HALF)) * FIX_FLOAT;

	//足跡の発生方向を求める
	float fSpreadRot = fRot + fRotRange;
	fSpreadRot = FixRotate(&fSpreadRot);

	//飛び散る移動量
	D3DXVECTOR3 move = VEC3_INIT;
	move.x = sinf(fSpreadRot) * fSpeed;
	move.y = fJump;
	move.z = cosf(fSpreadRot) * fSpeed;

	CPolyBill *pBill = CPolyBill::Create(NULL);
	pBill->SetInfo(pos, fSize, fSize);
	pBill->SetVertexInfo(col);
	pBill->SetProperty(fGravity, fGravityCorr);//重力設定
	pBill->SetProperty(nLife);	//体力設定
	pBill->SetProperty(move);	//移動量設定
}