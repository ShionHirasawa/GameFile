//================================================================================================
//
//数字関連の専属処理[number.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "number.h"

//テクスチャのパス
#define TEX_PATH	"data\\TEXTURE\\number.png"
//テクスチャの分割数
#define TEX_WIDTH		(1.0f / 10.0f)

//静的メンバ変数
//テクスチャ
int CNumber::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CNumber::CNumber(const int nPriority)
{
	//メンバ変数クリア
	m_nNumber = 0;
}

//=======================================
//デストラクタ
//=======================================
CNumber::~CNumber()
{

}

//=======================================
//生成処理
//=======================================
CNumber *CNumber::Create(void)
{
	//数字インスタンス生成
	CNumber *pNumber = new CNumber;

	//インスタンスを生成できた
	if (pNumber != NULL)
	{
		//初期化処理
		pNumber->Init();

		//テクスチャ割り当て
		pNumber->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//インスタンスを返す
	return pNumber;
}

//=======================================
//初期化処理
//=======================================
HRESULT CNumber::Init(void)
{
	//頂点座標の初期化
	if (FAILED(CObject2D::Init(VEC3_INIT, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(TEX_WIDTH, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_NUMBER);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CNumber::Uninit(void)
{
	//頂点バッファの破棄など
	CObject2D::Uninit();
}

//=======================================
//更新処理
//=======================================
void CNumber::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CNumber::Draw(void)
{
	//頂点バッファの破棄など
	CObject2D::Draw();
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CNumber::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}

//=======================================
//数字情報の設定処理
//=======================================
void CNumber::SetInfo(const int nNumber, const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	//自分の番号を設定
	m_nNumber = nNumber;

	//位置設定
	SetPosition(pos);

	//頂点座標・テクスチャ座標の設定
	SetVertexInfo(fWidth, fHeight, D3DXVECTOR2(TEX_WIDTH * (float)m_nNumber, 1.0f), TEX_WIDTH, 1.0f);
}

//=======================================
//テクスチャ座標の設定
//=======================================
void CNumber::SetTexturePos(const int nNumber)
{
	//自分の番号を設定
	m_nNumber = nNumber;

	//テクスチャ座標の設定
	SetVertexInfo(D3DXVECTOR2(TEX_WIDTH * (float)m_nNumber, 1.0f), TEX_WIDTH, 1.0f);
}