//================================================================================================
//
//Xファイルのビル群モデル管理処理[buildsX.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "objectX.h"
#include "jewel.h"
#include "../2D/object2D.h"
#include "../2D/fade.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "../Mode/result.h"

//宝石を取得する距離
#define TOUCH_LENGTH	(30.0f)

//静的メンバ変数宣言
D3DXVECTOR3 CJewel::m_pos = VEC3_INIT;
bool		CJewel::m_bDisp = true;
float		CJewel::m_fGoalLength = 0.0f;
D3DXVECTOR3 CJewel::m_GoalPoint = VEC3_INIT;
CMesh		*CJewel::m_pMesh = NULL;

//=======================================
//コンストラクタ
//=======================================
CJewel::CJewel()
{
	//メンバ変数クリア
	m_pos = VEC3_INIT;
	m_bDisp = true;
	m_GoalPoint = VEC3_INIT;
	m_fGoalLength = 0.0f;
	m_pMesh = NULL;
}

//=======================================
//デストラクタ
//=======================================
CJewel::~CJewel()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CJewel::Init(char *pXFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 GoalPoint, float fLength)
{
	//オブジェクトⅩ初期化
	if (FAILED(CObjectX::Init(pXFilePath, XMODELTYPE_NONE, 0)))
	{
		//初期化失敗
		return E_FAIL;
	}

	//位置・移動量・向き設定
	SetTransrate(pos, VEC3_INIT, VEC3_INIT);
	m_pos = pos;

	//ゴール地点・半径設定
	m_GoalPoint = GoalPoint;
	m_fGoalLength = fLength;

	//ゴール生成
	m_pMesh = new CMesh;
	m_pMesh->Init(m_GoalPoint, GetColor(COL_WHITE), 16, m_fGoalLength * 0.5f, 150.0f, 3.0f, 1.0f);
	m_pMesh->DispSwitch(false);

	//種類設定
	CObject::SetType(TYPE_JEWEL);

	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CJewel::Uninit(void)
{
	CObjectX::Uninit();
}

//=======================================
//更新処理
//=======================================
void CJewel::Update(void)
{
	//ゴールが描画されていないなら引き返す
	if (m_bDisp || m_pMesh == NULL) return;

	//回す
	D3DXVECTOR3 rot = m_pMesh->GetRotation();

	rot.y += 0.01f;

	FixRotate(&rot.y);

	m_pMesh->SetRotation(rot);
}

//=======================================
//描画処理
//=======================================
void CJewel::Draw(void)
{
	//描画する
	if (m_bDisp)
	{
		//描画処理
		CObjectX::Draw(NULL);
	}
}

//=======================================
//当たり判定処理
//=======================================
void CJewel::Collision(D3DXVECTOR3 pos)
{
	//位置の差分を取得
	D3DXVECTOR3 posDiff = pos - m_pos;

	//直線距離が、当たり判定未満
	if (D3DXVec3Length(&posDiff) <= TOUCH_LENGTH && m_bDisp)
	{
		//宝石ゲット（描画させない
		m_bDisp = false;
		CManager::GetSound()->Play(CSound::SE_JEWEL_GET);

		//ゴール出現
		m_pMesh->DispSwitch(true);
	}
}

//=======================================
//ゴール達成処理
//=======================================
void CJewel::Goal(D3DXVECTOR3 pos)
{
	//まだ宝石が盗られていない
	if (m_bDisp) return;

	//位置の差分
	D3DXVECTOR3 posDiff = pos - m_GoalPoint;

	//ゴールの半径の中に入っている
	if (D3DXVec3Length(&posDiff) <= m_fGoalLength)
	{
		//次のモードを取得
		int nMode = (CManager::GetMode() + 1) % CScene::MAX;

		//次のモードへ移行
		CFade::Set((CScene::MODE)nMode, CFade::FADE_OUT);
		CResult::SetState(CResult::STATE_CLEAR);
	}
}