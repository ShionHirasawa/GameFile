//================================================================================================
//
//シリンダー処理[number.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "Cylinder.h"

//=======================================
//コンストラクタ
//=======================================
CCylinder::CCylinder(const int nPriority) : CObject3D(nPriority)
{
	//メンバ変数クリア
	m_bDisp = true;
}

//=======================================
//デストラクタ
//=======================================
CCylinder::~CCylinder()
{

}

//=======================================
//生成処理
//=======================================
CCylinder *CCylinder::Create(char *pPath)
{
	//ポリゴンインスタンス生成
	CCylinder *pCylinder = new CCylinder;

	//インスタンスを生成できた
	if (pCylinder != NULL)
	{
		//初期化処理
		pCylinder->Init();

		//テクスチャ読み込み
		pCylinder->m_nTextureID = CManager::GetTexture()->Regist(pPath);

		//テクスチャ割り当て
		pCylinder->BindTexture(CManager::GetTexture()->GetAddress(pCylinder->m_nTextureID));
	}

	//インスタンスを返す
	return pCylinder;
}

//=======================================
//初期化処理
//=======================================
HRESULT CCylinder::Init(void)
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
void CCylinder::Uninit(void)
{
	//頂点バッファの破棄など
	CObject3D::Uninit();
}

//=======================================
//更新処理
//=======================================
void CCylinder::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CCylinder::Draw(void)
{
	if (!m_bDisp) return;

	//頂点バッファの破棄など
	CObject3D::Draw();
}

//=======================================
//ポリゴン情報の設定処理
//=======================================
void CCylinder::SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, D3DXVECTOR2 fTexPos, float TexWidth, float TexHeight)
{
	//位置設定
	SetPosition(pos);

	//頂点座標・テクスチャ座標の設定
	SetVertexInfo(fWidth, fHeight, fDepth, fTexPos, TexWidth, TexHeight);
}