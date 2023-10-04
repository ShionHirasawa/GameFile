//================================================================================================
//
//弾処理[bullet.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/billboard.h"
#include "object3D.h"
#include "bullet.h"
#include "explosion.h"
#include "particle.h"
#include "effect.h"
#include "wall.h"
#include "../XFile/objectX.h"
#include "../XFile/enemy.h"

//==============================
//マクロ
//==============================
//弾の処理優先順位
#define PRIORITY_BULLET	(3)

//弾ポリゴンのサイズ
#define BULLET_SIZE		(3.0f)

//最大体力
#define MAX_LIFE		(100)

//テクスチャのパス
#define TEX_PATH		"data\\TEXTURE\\bullet.png"

//最大移動速度
#define BULLET_SPEED	(5.0f)

/***		敵に当たったときのマクロ		***/
//エフェクトの拡散数（敵に当たったときだけ
#define NUM_EFFECT_SPREAD	(10)
//パーティクルのライフ
#define PARTICLE_LIFE		(5)

//静的メンバ変数
int	CBullet::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CBullet::CBullet() : CObjectBillboard(PRIORITY_BULLET)
{
	//メンバ変数クリア
	m_nLife = 0;
}

//=======================================
//デストラクタ
//=======================================
CBullet::~CBullet()
{

}

//=======================================
//生成処理
//=======================================
CBullet *CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//弾インスタンスの生成
	CBullet *pBullet = new CBullet;

	//初期化処理
	pBullet->Init();

	//テクスチャ割り当て
	pBullet->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	pBullet->Set(pos, rot);

	//弾を返す
	return pBullet;
}

//=======================================
//初期化処理
//=======================================
HRESULT CBullet::Init(void)
{
	//頂点座標の初期化
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, BULLET_SIZE, BULLET_SIZE, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_BULLET);

	/*寿命を初期化*/	m_nLife = 0;
	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CBullet::Uninit(void)
{
	//頂点バッファの破棄など
	CObjectBillboard::Uninit();
}

//=======================================
//更新処理
//=======================================
void CBullet::Update(void)
{
	//前回位置更新
	UpdatePosOld();

	//移動処理
	Move();

	//寿命管理
	if (!LifeManage())
	{//寿命が尽きなかったら
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 PosOld = GetPosOld();
		D3DXVECTOR3 move = PosOld - pos;

		//当たり判定
		if (!CollisionEnemy(pos) &&	// 敵に当たらなかった
			!CWall::Collision(&pos, GetPosOld(), &move, BULLET_SIZE, pos.y + BULLET_SIZE))//壁に当たらなかった
		{//敵に当たらなかった
			//頂点情報設定
			SetVertexInfo(BULLET_SIZE, BULLET_SIZE, 0.0f);
		}
		else
		{
			//どこかの当たり判定にひっかかった
			//爆発設定
			CExplosion *pExplosion = CExplosion::Create(pos);

			//己を消す
			this->Release();
		}
	}
}

//=======================================
//寿命管理
//=======================================
bool CBullet::LifeManage(void)
{
	//寿命が尽きたかどうか
	bool bDead = false;

	//寿命を減らす
	m_nLife--;

	//寿命が尽きたら
	if (m_nLife <= 0)
	{
		//爆発設定
		CExplosion *pExplosion = CExplosion::Create(CObjectBillboard::GetPosition());

		//己を消す
		this->Release();

		//寿命が尽きた
		bDead = true;
	}

	//生死を返す
	return bDead;
}

//=======================================
//描画処理
//=======================================
void CBullet::Draw(void)
{
	//自分を描画
	CObjectBillboard::Draw();
}

//=======================================
//位置設定
//=======================================
void CBullet::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//移動量を計測
	D3DXVECTOR3 move;

	move.x = sinf(rot.y + PI_UP) * BULLET_SPEED;
	move.z = cosf(rot.y + PI_UP) * BULLET_SPEED;
	move.y = 0.0f;

	//位置・移動量・向きを設定
	CObjectBillboard::SetTransrate(pos, move, rot);
	SetVertexInfo(BULLET_SIZE, BULLET_SIZE, 0.0f);

	//体力設定
	m_nLife = MAX_LIFE;

	//発射音再生
	CManager::GetSound()->Play(CSound::SE_SHOT);
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CBullet::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}

//=======================================
//敵との当たり判定
//=======================================
bool CBullet::CollisionEnemy(const D3DXVECTOR3 pos)
{
	//オブジェクト全体を判定
	for (int nCntPriority = 0; nCntPriority < CObject::OBJECT_PRIORIRY_MAX; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
		{
			//敵のポインタをもらう
			CEnemyRobot *pEnemy = (CEnemyRobot *)GetObject(nCntPriority, nCntObj);

			//敵のポインタが貰えなかった
			if (pEnemy == NULL || pEnemy->GetType() != TYPE_ROBOT) continue;

			//位置の差分を格納
			D3DXVECTOR3 PosDiff = pEnemy->GetPosition() - pos;
			PosDiff.y = 0.0f;

			//敵の当たり範囲と弾のサイズの合計を算出
			float fMargin = pEnemy->GetXModelMargin(pEnemy->GetMyXModelID()) + BULLET_SIZE;

			//敵との距離が当たり範囲以下
			if (fMargin >= D3DXVec3Length(&PosDiff))
			{
				//ダメージを与える
				pEnemy->GiveDamege(1);

				//パーティクルを通してエフェクトを複数生成
				CParticle *pPar = CParticle::Create(pos, true, NUM_EFFECT_SPREAD, PARTICLE_LIFE);

				return true;	//当たった
			}
		}
	}

	//パーティクルを通してエフェクトを1つ生成
	CParticle::Create(GetPosition(), false);

	//誰にも当たらなかった
	return false;
}

//=======================================
//ブロックとの当たり判定
//=======================================
bool CBullet::CollisionBlock(const D3DXVECTOR3 pos)
{
	//オブジェクト全体を判定
	for (int nCntPriority = 0; nCntPriority < CObject::OBJECT_PRIORIRY_MAX; nCntPriority++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
		{
			//オブジェクトのポインタをもらう
			CObject *pObj = GetObject(nCntPriority, nCntObj);

			//オブジェクトのポインタがもらえた
			if (pObj != NULL)
			{
				//オブジェクトの種類を取得
				TYPE type = pObj->GetType();

				//オブジェクトのタイプがブロックだった
				if (type == CObject::TYPE_BLOCK)
				{
					/*ブロックの左上位置を格納*/ D3DXVECTOR3 BlockLeftOver;		pObj->GetVertexPos(BlockLeftOver, 0);
					/*ブロックの右下位置を格納*/ D3DXVECTOR3 BlockRightUnder;	pObj->GetVertexPos(BlockRightUnder, 3);

					//当たり判定
					if ((pos.x >= BlockLeftOver.x  &&	// ブロックの左端から右に、
						pos.x <= BlockRightUnder.x) &&	// ブロックの右端から左に、
						(pos.y >= BlockLeftOver.y  &&	// ブロックの上部から下に、
						pos.y <= BlockRightUnder.y))	// ブロックの下部から上に、弾がいる
					{
						return true;	//当たった
					}
				}
			}
		}
	}
	//誰にも当たらなかった
	return false;
}