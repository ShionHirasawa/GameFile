//================================================================================================
//
//パーティクル処理[particle.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/billboard.h"
#include "effect.h"
#include "particle.h"
#include "explosion.h"
#include "../S_H_Lib/S_H_Calculation.h"

//==============================
//マクロ
//==============================
//パーティクルの処理優先順位
#define PRIORITY_PARTICLE	(3)
//一度に生成する最大数
#define MAX_SET				(20)
//拡散スピード
#define SPREAD_SPEED		(700)
//最低拡散スピード
#define MIN_SPREAD_SPEED	(0.5f)

//=======================================
//コンストラクタ
//=======================================
CParticle::CParticle() : CObjectBillboard(PRIORITY_PARTICLE)
{
	//メンバ変数クリア
	m_nLife = 0;
	m_pos = VEC3_INIT;
	m_move = VEC3_INIT;
}

//=======================================
//デストラクタ
//=======================================
CParticle::~CParticle()
{

}

//=======================================
//生成処理
//=======================================
CParticle *CParticle::Create(const D3DXVECTOR3 pos, const bool bMove, const int nNumSet, const int nParticleLife)
{
	//設定する数ループ
	for (int nCntParticle = 0; nCntParticle < nNumSet; nCntParticle++)
	{
		//発生数が上限を超えた
		if (nCntParticle >= MAX_SET)
		{//処理を抜ける
			break;
		}

		//パーティクルのインスタンス生成
		CParticle *pParticle = new CParticle;

		//インスタンス生成成功
		if (pParticle != NULL)
		{
			/* 位置 設定*/pParticle->m_pos = pos;
			/* 寿命 設定*/pParticle->m_nLife = nParticleLife;
			/*タイプ設定*/pParticle->SetType(TYPE_PARTICLE);

			//移動させない場合
			if (!bMove)
			{
				//移動させない
				pParticle->m_move = VEC3_INIT;
			}
			else
			{
				//移動量を計測
				pParticle->m_move.x = sinf(RandomRotate()) * (float)(rand() % SPREAD_SPEED) * FIX_FLOAT + MIN_SPREAD_SPEED;
				pParticle->m_move.y = cosf(RandomRotate()) * (float)(rand() % SPREAD_SPEED) * FIX_FLOAT + MIN_SPREAD_SPEED;
				pParticle->m_move.z = cosf(RandomRotate()) * (float)(rand() % SPREAD_SPEED) * FIX_FLOAT + MIN_SPREAD_SPEED;
			}

			//初期化処理
			pParticle->Init();
		}
	}

	//パーティクルを返す
	return NULL;
}

//=======================================
//初期化処理
//=======================================
HRESULT CParticle::Init(void)
{
	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CParticle::Uninit(void)
{
	//自分自身を破棄
	this->Release();
}

//=======================================
//更新処理
//=======================================
void CParticle::Update(void)
{
	//エフェクト生成
	CEffect *pEff = CEffect::Create(m_pos);

	//移動
	m_pos += m_move;

	//寿命減少
	m_nLife--;

	//寿命が尽きた
	if (m_nLife <= 0)
	{
		//自分を破棄
		this->Release();
	}
}

//=======================================
//描画処理
//=======================================
void CParticle::Draw(void)
{

}