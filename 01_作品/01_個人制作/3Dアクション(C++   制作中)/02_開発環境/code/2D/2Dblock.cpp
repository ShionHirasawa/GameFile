//================================================================================================
//
//2Dブロックオブジェクト処理[2Dblock.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../XFile/objectX.h"
#include "object2D.h"
#include "2Dblock.h"
#include "../Player/player.h"

/***  テクスチャのパス  ***/ 
#define TEX_PATH			"data\\TEXTURE\\block.jpg"
/***	ブロックの処理優先順位	***/
#define PRIORITY_2DBLOCK	(3)

//静的メンバ変数宣言
/**/ int CBlock2D::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CBlock2D::CBlock2D() : CObject2D(PRIORITY_2DBLOCK)
{ 

}

//=======================================
//デストラクタ
//=======================================
CBlock2D::~CBlock2D()
{

}

//=======================================
//生成処理
//=======================================
CBlock2D *CBlock2D::Create(void)
{
	//インスタンス生成
	CBlock2D *pBlock2D = new CBlock2D;

	//インスタンスが生成された
	if (pBlock2D != NULL)
	{
		//初期化処理に失敗
		if (FAILED(pBlock2D->Init()))
		{
			delete pBlock2D;//メモリを解放
			pBlock2D = NULL;//NULLポインタに
			return NULL;	//NULLを返す
		}

		//テクスチャ割り当て
		pBlock2D->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//インスタンスを返す
	return pBlock2D;
}

//=======================================
//初期化処理
//=======================================
HRESULT CBlock2D::Init(void)
{
	//初期化処理
	if (FAILED(CObject2D::Init(VEC3_INIT, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
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
void CBlock2D::Uninit(void)
{
	//終了処理
	CObject2D::Uninit();
}

//=======================================
//更新処理
//=======================================
void CBlock2D::Update(void)
{
	//プレイヤーとの当たり判定
	CollisionPlayer(GetPosition(), GetSize2D());
}

//=======================================
//描画処理
//=======================================
void CBlock2D::Draw(void)
{ 
	//描画処理
	CObject2D::Draw();
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CBlock2D::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}

//=======================================
//設定処理
//=======================================
void CBlock2D::Set(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	/* 位置設定 */	SetPosition(pos);
	/*サイズ設定*/	SetSize(D3DXVECTOR2(fWidth, fHeight));

	//頂点座標設定
	SetVertexInfo(fWidth, fHeight);
}

//=======================================
//プレイヤーとの当たり判定処理
//=======================================
//
//当たり判定のイメージ
//・底面より上に頭がある┓
//・天井より下に足がある┻この2つを満たすなら、当たり判定を実行する
//
//		↓Player		↓天井
//		 0  ←＼  ／←┏━━━━┓
//	   ／l＼	><    ┃ブロック┃
//	   ／ ＼←／  ＼←┗━━━━┛
//						↑底面
//
//=======================================
void CBlock2D::CollisionPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR2 size)
{
	/*ブロックの左上座標を格納*/D3DXVECTOR3 BlockLeftUp;
	/*ブロックの右下座標を格納*/D3DXVECTOR3 BlockRightDown;
	
	/*ブロックの左上座標を取得*/GetVertexInfo(BlockLeftUp, 0);
	/*ブロックの右下座標を取得*/GetVertexInfo(BlockRightDown, 3);

	//オブジェクト全体を判定
	for (int nCntPlayer = 0; nCntPlayer < MAX_OBJ; nCntPlayer++)
	{
		//オブジェクトのポインタをもらう
		CObject *pPlayer = GetObject(CPlayer::PRIORITY, nCntPlayer);

		//オブジェクトのポインタがもらえた
		if (pPlayer != NULL)
		{
			//オブジェクトのタイプがプレイヤーだった
			if (pPlayer->GetType() == CObject::TYPE_PLAYER)
			{
				//サイズ・位置を格納
				/*プレイヤーのサイズ*/ D3DXVECTOR2 PlayerSize = pPlayer->GetSize2D();// 後ろの係数  はポリゴンの余白部分を削っている
				/*プレイヤーの移動量*/ D3DXVECTOR3 PlayerMove = pPlayer->GetMoving();
				/*プレイヤーの現在地*/ D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
				/*プレイヤーの前回地*/ D3DXVECTOR3 PlayerPosOld = pPlayer->GetPosOld();

				/*修正する位置を格納*/ D3DXVECTOR3 FixPos = PlayerPos;

				/*ポリゴンの余白調整*/PlayerSize = D3DXVECTOR2(PlayerSize.x * CPlayer::WIDTH_MARGIN, PlayerSize.y * CPlayer::HEIGHT_MARGIN);

				//===============================================================
				// STEP１: ブロックの天井～底面までの間にプレイヤーがいる
				//===============================================================
				if (BlockLeftUp.y <= PlayerPos.y + PlayerSize.y		&&	// ブロックの天井 <= プレイヤーの足
					PlayerPos.y - PlayerSize.y <= BlockRightDown.y	&&	// プレイヤーの頭 <= ブロックの底面
					PlayerPosOld.y + PlayerSize.y != BlockLeftUp.y	&&	// 前回ブロックに乗っていない
					PlayerPosOld.y - PlayerSize.y != BlockRightDown.y)	// 前回ブロックに頭が当たってない
				{
					//===============================================================
					// STEP２ : ブロックの左右どちらから来たのか、前回の位置を用いて判定
					//			判明したら、ブロックの端に幅がめり込まない位置を算出する
					//===============================================================
					//左からめり込んだか
					if (BlockLeftUp.x >= PlayerPosOld.x + PlayerSize.x &&	//前回はブロックより左に居て
						BlockLeftUp.x <= PlayerPos.x + PlayerSize.x)		//今はブロックの左端にめり込んでいる
					{
						// ブロックの左端から、プレイヤーの横幅を引いた分を修正位置とする
						FixPos.x = BlockLeftUp.x - PlayerSize.x;

						//Ⅹの移動量を0に
						PlayerMove.x = 0.0f;
					}

					//右からめり込んだか
					if (BlockRightDown.x <= PlayerPosOld.x - PlayerSize.x &&//前回はブロックより右に居て
						BlockRightDown.x >= PlayerPos.x - PlayerSize.x)		//今はブロックの右端にめり込んでいる
					{
						// ブロックの右端から、プレイヤーの横幅を足した分を修正位置とする
						FixPos.x = BlockRightDown.x + PlayerSize.x;

						//Ⅹの移動量を0に
						PlayerMove.x = 0.0f;
					}
				}

				//===============================================================
				// STEP３ : ブロックの左端から右端の中にプレイヤーがいる
				//===============================================================
				if (BlockLeftUp.x <= PlayerPos.x + PlayerSize.x &&		// ブロックの左端 <= プレイヤーの右端
					PlayerPos.x - PlayerSize.x <= BlockRightDown.x &&	// プレイヤーの左端 <= ブロックの右端
					PlayerPosOld.x + PlayerSize.x != BlockLeftUp.x &&	// 前回ブロックの左端に沿っていない
					PlayerPosOld.x - PlayerSize.x != BlockRightDown.x)	// 前回ブロックの右端に沿っていない
				{
					//===============================================================
					// STEP４ : ブロックの上下どちらから来たのか、前回の位置を用いて判定
					//			判明したら、ブロックの上下に高さがめり込まない位置を算出する
					//===============================================================
					//上からめり込んだか
					if (BlockLeftUp.y >= PlayerPosOld.y + PlayerSize.y &&	//前回はブロックより上に居て
						BlockLeftUp.y <= PlayerPos.y + PlayerSize.y)		//今はブロックの天井にめり込んでいる
					{
						// ブロックの天井から、プレイヤーの高さ分を引いた位置を修正位置とする
						FixPos.y = BlockLeftUp.y - PlayerSize.y;

						//Ｙの移動量を0に
						PlayerMove.y = 0.0f;
					}

					//下からめり込んだか
					if (BlockRightDown.y <= PlayerPosOld.y - PlayerSize.y &&//前回はブロックより下に居て
						BlockRightDown.y >= PlayerPos.y - PlayerSize.y)		//今はブロックの底面にめり込んでいる
					{
						// ブロックの底面から、プレイヤーの高さを足した位置を修正位置とする
						FixPos.y = BlockRightDown.y + PlayerSize.y;

						//プレイヤーが上昇中
						if (PlayerMove.y < 0.0f)
						{
							//Ｙの移動量を0に
							PlayerMove.y = 0.0f;
						}
					}
				}

				/* 位置を修正する */ pPlayer->SetPosition(FixPos);
				/*移動量を修正する*/ pPlayer->SetMoving(PlayerMove);
			}
		}
	}
}