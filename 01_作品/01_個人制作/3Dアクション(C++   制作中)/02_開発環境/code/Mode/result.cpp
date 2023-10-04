//================================================================================================
//
//リザルト画面処理[result.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "result.h"
#include "../Object/object.h"
#include "../2D/object2D.h"
#include "../2D/bg.h"
#include "../2D/fade.h"

//静的メンバ変数宣言
CResult::STATE CResult::s_State;

//テクスチャパス設定
char *CResult::BG_TEXTURE_PATH[STATE_MAX] = {
	"data\\TEXTURE\\GameOver.png",
	"data\\TEXTURE\\GameClear.png",
};

//=======================================
//コンストラクタ
//=======================================
CResult::CResult()
{

}

//=======================================
//デストラクタ
//=======================================
CResult::~CResult()
{
	s_State = STATE_OVER;
}

//=======================================
//初期化処理
//=======================================
HRESULT CResult::Init(void)
{
	//背景のテクスチャ読み込み
	CBg::Load(BG_TEXTURE_PATH[s_State]);

	//背景オブジェクト生成
	CBg *pBg = CBg::Create();
	pBg->SetVertexInfo(GetColor(COL_WHITE));

	//BGM再生
	CManager::GetSound()->Play(CSound::BGM_RESULT);

	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CResult::Uninit(void)
{
	//フェード以外のオブジェクト全破棄
	CObject::ExceptReleaseAll(CObject::TYPE_FADE);
}

//=======================================
//更新処理
//=======================================
void CResult::Update(void)
{
	//画面遷移
	if (CManager::IsInputTrigger(DIK_RETURN, CInputGamePad::A))
	{
		CFade::Set(CScene::TITLE, CFade::FADE_WIPE_OUT);
	}
}

//=======================================
//描画処理
//=======================================
void CResult::Draw(void)
{

}

//=======================================
//リザルト状態設定処理
//=======================================
void CResult::SetState(STATE state)
{
	//状態設定
	s_State = state;
}