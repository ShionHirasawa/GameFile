//================================================================================================
//
//アイテム処理[item.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../XFile/objectX.h"
#include "../3D/billboard.h"
#include "../Player/player.h"
#include "item.h"

//==============================
//マクロ
//==============================
//アイテムの処理優先順位
#define PRIORITY_ITEM	(3)

//アイテムポリゴンのサイズ
#define ITEM_SIZE	(30.0f)

//テクスチャのパス
#define TEX_PATH	"data\\TEXTURE\\Item.png"

//静的メンバ変数
int	CItem::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CItem::CItem() : CObjectBillboard(PRIORITY_ITEM)
{
	//メンバ変数クリア
}

//=======================================
//デストラクタ
//=======================================
CItem::~CItem()
{

}

//=======================================
//生成処理
//=======================================
CItem *CItem::Create(const D3DXVECTOR3 pos)
{
	//アイテムインスタンスの生成
	CItem *pItem = new CItem;

	//初期化処理
	pItem->Init();

	//テクスチャ割り当て
	pItem->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	pItem->Set(pos);

	//アイテムを返す
	return pItem;
}

//=======================================
//初期化処理
//=======================================
HRESULT CItem::Init(void)
{
	//頂点座標の初期化
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, ITEM_SIZE, ITEM_SIZE, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_ITEM);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CItem::Uninit(void)
{
	//頂点バッファの破棄など
	CObjectBillboard::Uninit();
}

//=======================================
//更新処理
//=======================================
void CItem::Update(void)
{
	//移動処理
	Move();

	//プレイヤーに当たらなかった
	if (!CollisionPlayer(GetPosition()))
	{
	 //頂点情報設定
		SetVertexInfo(ITEM_SIZE, ITEM_SIZE, 0.0f);
	}	
}

//=======================================
//描画処理
//=======================================
void CItem::Draw(void)
{
	//自分を描画
	CObjectBillboard::Draw();
}

//=======================================
//位置設定
//=======================================
void CItem::Set(const D3DXVECTOR3 pos)
{
	//位置・移動量・向きを設定
	CObjectBillboard::SetPosition(pos);
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CItem::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}

//=======================================
//プレイヤーとの当たり判定
//=======================================
bool CItem::CollisionPlayer(const D3DXVECTOR3 pos)
{
	//オブジェクト全体を判定
	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		//オブジェクトのポインタをもらう
		CObject *pObj = GetObject(CPlayer::PRIORITY, nCntObj);

		//オブジェクトのポインタがもらえた
		if (pObj != NULL)
		{
			//オブジェクトのタイプがプレイヤーだった
			if (pObj->GetType() == CObject::TYPE_PLAYER)
			{
				/*ブロックの左上位置を格納*/ D3DXVECTOR3 PlayerLeftOver;	pObj->GetVertexPos(PlayerLeftOver, 0);
				/*ブロックの右下位置を格納*/ D3DXVECTOR3 PlayerRightUnder;	pObj->GetVertexPos(PlayerRightUnder, 3);

				//当たり判定
				if ((pos.x >= PlayerLeftOver.x  &&	// プレイヤーの左端から右に、
					 pos.x <= PlayerRightUnder.x) &&// プレイヤーの右端から左に、
					(pos.y >= PlayerLeftOver.y  &&	// プレイヤーの上部から下に、
					 pos.y <= PlayerRightUnder.y))	// プレイヤーの下部から上に、アイテムがいる
				{
					this->Release();//己を消す
					return true;	//当たった
				}
			}
		}
	}

	//誰にも当たらなかった
	return false;
}