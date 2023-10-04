//================================================================================================
//
//ポリゴン関連の専属処理[number.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "Polygon2D.h"

//=======================================
//コンストラクタ
//=======================================
CPoly2D::CPoly2D(const int nPriority) : CObject2D(nPriority)
{
	//メンバ変数クリア
	m_bDisp = true;
}

//=======================================
//デストラクタ
//=======================================
CPoly2D::~CPoly2D()
{

}

//=======================================
//生成処理
//=======================================
CPoly2D *CPoly2D::Create(char *pPath)
{
	//ポリゴンインスタンス生成
	CPoly2D *pPoly2D = new CPoly2D;

	//インスタンスを生成できた
	if (pPoly2D != NULL)
	{
		//初期化処理
		pPoly2D->Init();

		//テクスチャ設定
		pPoly2D->SetTexture(pPath);
	}

	//インスタンスを返す
	return pPoly2D;
}

//=======================================
//初期化処理
//=======================================
HRESULT CPoly2D::Init(void)
{
	//頂点座標の初期化
	if (FAILED(CObject2D::Init(VEC3_INIT, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_POLY_2D);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CPoly2D::Uninit(void)
{
	//頂点バッファの破棄など
	CObject2D::Uninit();
}

//=======================================
//更新処理
//=======================================
void CPoly2D::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CPoly2D::Draw(void)
{
	if (!m_bDisp) return;

	//αテストON
	CManager::GetRenderer()->SetAlphaTest(true, 0);

	//頂点バッファの破棄など
	CObject2D::Draw();

	//αテストOFF
	CManager::GetRenderer()->SetAlphaTest(false, 255);
}

//=======================================
//ポリゴン情報の設定処理
//=======================================
void CPoly2D::SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, D3DXVECTOR2 fTexPos, float TexWidth, float TexHeight)
{
	//位置設定
	SetPosition(pos);

	//頂点座標・テクスチャ座標の設定
	SetVertexInfo(fWidth, fHeight, fTexPos, TexWidth, TexHeight);
}

//=======================================
//テクスチャ設定処理
//=======================================
void CPoly2D::SetTexture(char *pPath)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(pPath);

	//テクスチャ割り当て
	BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
}