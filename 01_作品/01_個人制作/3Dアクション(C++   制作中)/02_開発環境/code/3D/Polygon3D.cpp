//================================================================================================
//
//3Dポリゴン処理[Polygon3D.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "Polygon3D.h"

//=======================================
//コンストラクタ
//=======================================
CPoly3D::CPoly3D(const int nPriority) : CObject3D(nPriority)
{
	//メンバ変数クリア
	m_bDisp = true;
	m_bAlphaTest = false;	//αテストを行うかどうか
	m_bAddBlend = false;	//加算合成を行うかどうか
	m_bSubBlend = false;	//減算合成を行うかどうか
}

//=======================================
//デストラクタ
//=======================================
CPoly3D::~CPoly3D()
{

}

//=======================================
//生成処理
//=======================================
CPoly3D *CPoly3D::Create(char *pPath)
{
	//ポリゴンインスタンス生成
	CPoly3D *pPoly3D = new CPoly3D;

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
HRESULT CPoly3D::Init(void)
{
	//頂点座標の初期化
	if (FAILED(CObject3D::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_POLY_3D);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CPoly3D::Uninit(void)
{
	//頂点バッファの破棄など
	CObject3D::Uninit();
}

//=======================================
//更新処理
//=======================================
void CPoly3D::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CPoly3D::Draw(void)
{
	if (!m_bDisp) return;

	//レンダー設定
	SetDrawRender(true);

	//頂点バッファの破棄など
	CObject3D::Draw();

	//レンダー設定解除
	SetDrawRender(false);
}

//=======================================
//レンダラーの設定を行う
//=======================================
void CPoly3D::SetDrawRender(bool bSet)
{
	//レンダラー取得
	CRenderer *pRender = CManager::GetRenderer();

	//レンダラー設定
	if (bSet)
	{
		//加算合成を行う
		if (m_bAddBlend)	pRender->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_SET);

		//減算合成を行う
		else if (m_bSubBlend)pRender->SetAlphaBlend(CRenderer::TYPE_SUB, CRenderer::VALUE_SET);

		//αテスト有効化
		if (m_bAlphaTest) pRender->SetAlphaTest(true, 0);
	}
	//設定解除
	else
	{
		//αテスト無効化
		if (m_bAlphaTest) pRender->SetAlphaTest(false, 0);

		//加算合成を解除
		if (m_bAddBlend)	pRender->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_RESET);

		//減算合成を解除
		else if (m_bSubBlend)	pRender->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_RESET);
	}
}

//=======================================
//ポリゴン情報の設定処理
//=======================================
void CPoly3D::SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, D3DXVECTOR2 fTexPos, float TexWidth, float TexHeight)
{
	//位置設定
	SetPosition(pos);

	//頂点座標・テクスチャ座標の設定
	SetVertexInfo(fWidth, fHeight, fDepth, fTexPos, TexWidth, TexHeight);
}