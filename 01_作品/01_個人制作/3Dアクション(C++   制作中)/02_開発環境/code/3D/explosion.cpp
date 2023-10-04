//================================================================================================
//
//爆発処理[explosion.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/billboard.h"
#include "explosion.h"
#include "object3D.h"
#include "wall.h"
#include "camera.h"

//==============================
//マクロ
//==============================
#define PRIORITY_EXPLOSION	(3)		//爆発の処理優先順位

#define EXPLOSION_WIDTH		(10.0f)	//ポリゴンの幅
#define EXPLOSION_HEIGHT	(10.0f)	//ポリゴンの高さ

//最大体力
#define MAX_LIFE		(100)

//テクスチャのパス
#define TEX_PATH		"data\\TEXTURE\\explosion.png"
//テクスチャの分割数
#define TEX_WIDTH		(1.0f / 8.0f)
//アニメーションの切り替える間隔
#define ANIM_INTERVAL	(8)
//アニメーションのパターン
#define ANIM_PATTERN	(8)

//静的メンバ変数
int	CExplosion::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CExplosion::CExplosion() : CObjectBillboard()
{
	//メンバ変数クリア
	m_nAnimCounter = 0;
	m_nAnimPattern = 0;
}

//=======================================
//デストラクタ
//=======================================
CExplosion::~CExplosion()
{

}

//=======================================
//生成処理
//=======================================
CExplosion *CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, bool bSound)
{
	//弾インスタンスの生成
	CExplosion *pExplosion = new CExplosion;

	//インスタンスが無事生成された
	if (pExplosion != NULL)
	{
		//初期化処理
		if (FAILED(pExplosion->Init()))
		{
			pExplosion->Uninit();	//終了処理
			pExplosion->Release();	//破棄
			pExplosion = NULL;		//ポインタをNULLに
			return NULL;			//NULLを返す
		}

		//テクスチャ割り当て
		pExplosion->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));

		//メンバ変数初期化
		/*カウンター*/	pExplosion->m_nAnimCounter = 0;
		/*パターン*/	pExplosion->m_nAnimPattern = 0;
		/*位置設定*/	pExplosion->Set(pos, move);

		//爆発音再生
		if(bSound)	CManager::GetSound()->Play(CSound::SE_EXPLOSION);
	}

	//弾を返す
	return pExplosion;
}

//=======================================
//初期化処理
//=======================================
HRESULT CExplosion::Init(void)
{
	//頂点座標の初期化
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, EXPLOSION_WIDTH, EXPLOSION_HEIGHT, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(TEX_WIDTH, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_EXPLOSION);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CExplosion::Uninit(void)
{
	//頂点バッファの破棄など
	CObjectBillboard::Uninit();
}

//=======================================
//更新処理
//=======================================
void CExplosion::Update(void)
{
	//移動するタイプのために位置更新
	Move();
	SetVertexInfo(EXPLOSION_WIDTH, EXPLOSION_WIDTH, 0.0f);

	//爆発アニメーション
	if (Animation())
	{
		//己を消す
		this->Release();
	}
	//アニメーションが終わってなかったら
	else
	{
		//テクスチャ座標設定
		SetVertexInfo(D3DXVECTOR2(TEX_WIDTH * (float)m_nAnimPattern, 0.0f), TEX_WIDTH, 1.0f);
	}
}

//---------------------
//アニメーション処理
//---------------------
bool CExplosion::Animation(void)
{
	//アニメーションが終わったか
	bool bEndAnim = false;

	//カウンターを回したら次のコマに進む時間になった
	if ((++m_nAnimCounter %= ANIM_INTERVAL) == 0)
	{
		//パターンを増やして最大数に達したら、使わなくする
		if (++m_nAnimPattern == ANIM_PATTERN)
		{
			//アニメーションが終わった
			bEndAnim = true;
		}
	}

	//終わったかどうか返す
	return bEndAnim;
}

//=======================================
//描画処理
//=======================================
void CExplosion::Draw(void)
{
	//自分とカメラの間にカメラが無ければ描画
	if (CWall::IsAmongWall(CManager::GetCamera()->GetPosV(), GetPosition())) return;

	//自分を描画
	CObjectBillboard::Draw();
}

//=======================================
//位置設定
//=======================================
void CExplosion::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	//位置・移動量・向きを設定
	CObjectBillboard::SetTransrate(pos, move, VEC3_INIT);

	//頂点座標設定
	SetVertexInfo(EXPLOSION_WIDTH, EXPLOSION_HEIGHT, 0.0f);

	//アニメーションカウンタとパターンを初期化
	m_nAnimCounter = m_nAnimPattern = 0;
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CExplosion::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}