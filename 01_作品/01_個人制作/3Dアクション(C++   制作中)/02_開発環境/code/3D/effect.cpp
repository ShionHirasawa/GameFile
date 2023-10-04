//================================================================================================
//
//エフェクト処理[effect.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/billboard.h"
#include "effect.h"
#include "explosion.h"
#include "object3D.h"
#include "wall.h"
#include "camera.h"

//==============================
//マクロ
//==============================
//エフェクトの処理優先順位
#define PRIORITY_EFFECT	(3)

//エフェクトポリゴンのサイズ
#define EFFECT_SIZE		(10.0f)	

//テクスチャのパス
#define TEX_PATH	"data\\TEXTURE\\effect.jpg"

//最大体力
#define MAX_LIFE		(30)

//縮小スピード
#define SHRINK_SIZE		(1.0f)

//静的メンバ変数
int		CEffect::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CEffect::CEffect() : CObjectBillboard(PRIORITY_EFFECT)
{
	//メンバ変数クリア
	m_col = GetColor(COL_CLEAR);
	m_nLife = 0;
	m_fSize = 0.0f;
}

//=======================================
//デストラクタ
//=======================================
CEffect::~CEffect()
{

}

//=======================================
//生成処理（静止するエフェクト
//=======================================
CEffect *CEffect::Create(const D3DXVECTOR3 pos)
{
	//エフェクトインスタンスの生成
	CEffect *pEffect = new CEffect;

	//インスタンス生成成功
	if (pEffect != NULL)
	{
		//初期化処理
		pEffect->Init();
		pEffect->Set(pos);

		//テクスチャ割り当て
		pEffect->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//エフェクトを返す
	return pEffect;
}

//=======================================
//初期化処理
//=======================================
HRESULT CEffect::Init(void)
{
	//頂点座標の初期化
	if (FAILED(CObjectBillboard::Init(GetPosition(), EFFECT_SIZE, EFFECT_SIZE, 0.0f, GetColor(COL_EFFECT), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_EFFECT);

	/*寿命を初期化*/	m_nLife = 0;
	/*サイズを初期化*/	m_fSize = EFFECT_SIZE;
	/*カラー初期化*/	m_col = GetColor(COL_EFFECT);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CEffect::Uninit(void)
{
	//頂点バッファの破棄など
	CObjectBillboard::Uninit();
}

//=======================================
//更新処理
//=======================================
void CEffect::Update(void)
{
	//寿命管理
	if (!LifeManage())
	{//寿命が尽きなかった or サイズが無くならなかったら
	 //頂点情報設定
		SetVertexInfo(m_fSize, m_fSize, 0.0f);
	}
}

//=======================================
//寿命管理
//=======================================
bool CEffect::LifeManage(void)
{
	//寿命を減らす
	m_nLife--;

	//サイズを縮小する
	m_fSize -= SHRINK_SIZE;

	//寿命が尽きた	or	サイズが無になった
	if (m_nLife <= 0 || m_fSize <= 0.0f)
	{
		//己を消す
		this->Release();

		//お亡くなりになった
		return true;
	}

	//まだご存命
	return false;
}

//=======================================
//描画処理
//=======================================
void CEffect::Draw(void)
{
	//自分とカメラの間にカメラが無ければ描画
	if (CWall::IsAmongWall(CManager::GetCamera()->GetPosV(), GetPosition())) return;

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//αブレンディングを加算合成に設定
	CManager::GetRenderer()->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_SET);

	//自分を描画
	CObjectBillboard::Draw();

	//αブレンディングを元に戻す
	CManager::GetRenderer()->SetAlphaBlend(CRenderer::TYPE_ADD, CRenderer::VALUE_RESET);
}

//=======================================
//位置設定
//=======================================
void CEffect::Set(const D3DXVECTOR3 pos)
{
	//位置・移動量・向きを設定
	CObjectBillboard::SetPosition(pos);

	m_col = GetColor(COL_EFFECT);//カラー設定
	m_nLife = MAX_LIFE;			 //体力設定
	m_fSize = EFFECT_SIZE;		 //サイズ設定

	SetVertexInfo(m_fSize, m_fSize, 0.0f);//頂点座標設定
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CEffect::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}