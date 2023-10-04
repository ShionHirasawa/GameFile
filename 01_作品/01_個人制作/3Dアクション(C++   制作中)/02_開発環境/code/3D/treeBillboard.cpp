//================================================================================================
//
//ビルボードの木ポリゴン処理[treeBillboard.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "billboard.h"
#include "treeBillboard.h"

//フィールドの処理優先順位
#define PRIORITY_FIELD	(2)
//テクスチャのパス
#define TEX_PATH		"data\\TEXTURE\\tree.png"

//静的メンバ変数宣言
int CTree::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CTree::CTree() : CObjectBillboard(PRIORITY_FIELD)
{

}

//=======================================
//デストラクタ
//=======================================
CTree::~CTree()
{

}

//=======================================
//生成処理
//=======================================
CTree *CTree::Create(void)
{
	//インスタンス生成
	CTree *pTree = new CTree;

	//インスタンスが生成された
	if (pTree != NULL)
	{
		//初期化処理に失敗
		if (FAILED(pTree->Init()))
		{
			delete pTree;//メモリを解放
			pTree = NULL;//NULLポインタに
			return NULL;	//NULLを返す
		}

		//テクスチャ割り当て
		pTree->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//インスタンスを返す
	return pTree;
}

//=======================================
//初期化処理
//=======================================
HRESULT CTree::Init(void)
{
	//初期化処理
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{
		//初期化失敗
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_BLOCK);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CTree::Uninit(void)
{
	//終了処理
	CObjectBillboard::Uninit();
}

//=======================================
//更新処理
//=======================================
void CTree::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CTree::Draw(void)
{
	//描画処理
	CObjectBillboard::Draw();
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CTree::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}

//=======================================
//設定処理
//=======================================
void CTree::Set(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth)
{
	/* 位置設定 */	SetPosition(pos);
	/*サイズ設定*/	SetSize(D3DXVECTOR3(fWidth, fHeight, fDepth));

	//頂点座標設定
	SetVertexInfo(fWidth, fHeight, fDepth);
}