//================================================================================================
//
//タイトル画面処理[title.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Object/object.h"
#include "title.h"
#include "../2D/bg.h"
#include "../2D/fade.h"

//ロゴの静的情報
char *CTitle::LOGO_TEXTURE_PATH = "data\\TEXTURE\\LOGO.png";
const D3DXVECTOR3 CTitle::LOGO_POS = D3DXVECTOR3(200.0f, 200.0f, 0.0f);
const float CTitle::LOGO_WIDTH = 200.0f;	
const float CTitle::LOGO_HEIGHT = 100.0f;

//プレス表示の静的情報
char *CTitle::PRESS_TEXTURE_PATH = "data\\TEXTURE\\PressButton.png";
const D3DXVECTOR3 CTitle::PRESS_POS = D3DXVECTOR3(950.0f, 600.0f, 0.0f);
const float CTitle::PRESS_WIDTH = 300.0f;
const float CTitle::PRESS_HEIGHT = 50.0f;

//点滅の静的情報
const int CTitle::FLASH_TIME = 100;
const float CTitle::FLASH_SPEED = 1.0f / CTitle::FLASH_TIME;

//=======================================
//コンストラクタ
//=======================================
CTitle::CTitle()
{
	//メンバ変数クリア
	m_pLogo = NULL;
	m_pPress = NULL;
	m_nFlashCounter = 0;
	m_PressColor = GetColor(COL_WHITE);
	m_PressColor.a = 0.0f;
}

//=======================================
//デストラクタ
//=======================================
CTitle::~CTitle()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CTitle::Init(void)
{
	//背景色を変更
	CManager::GetRenderer()->SetBackGroundColor(COL_BLACK);

	//ロゴオブジェクト生成
	m_pLogo = CPoly2D::Create(LOGO_TEXTURE_PATH);
	if (m_pLogo != NULL)	m_pLogo->SetInfo(LOGO_POS, LOGO_WIDTH, LOGO_HEIGHT);

	//プレス表示オブジェクト生成
	m_pPress = CPoly2D::Create(PRESS_TEXTURE_PATH);
	if (m_pPress != NULL)	m_pPress->SetInfo(PRESS_POS, PRESS_WIDTH, PRESS_HEIGHT);

	//BGM再生
	CManager::GetSound()->Play(CSound::BGM_TITLE);

	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CTitle::Uninit(void)
{
	//フェード以外のオブジェクト全破棄
	CObject::ExceptReleaseAll(CObject::TYPE_FADE);

	//背景色を戻す
	CManager::GetRenderer()->SetBackGroundColor();
}

//=======================================
//更新処理
//=======================================
void CTitle::Update(void)
{
	//画面遷移
	if (CManager::IsInputTrigger(DIK_RETURN, CInputGamePad::A))
	{
		CFade::Set(CScene::TUTORIAL, CFade::FADE_WIPE_OUT);
	}

	//点滅カウンター増加
	m_nFlashCounter = (m_nFlashCounter + 1) % (FLASH_TIME * 2);

	//カウンターを点滅の間隔で割って、偶数の場合
	//もしくはフェード中なら消えていく
	if ((m_nFlashCounter / FLASH_TIME) % EVENPARITY == 0 ||
		CFade::Get() != CFade::FADE_NONE)
	{
		//透明にしていく
		m_PressColor.a -= FLASH_SPEED;

		//透明度が０を下回ったら０に戻す
		if (m_PressColor.a < 0.0f) m_PressColor.a = 0.0f;
	}
	//奇数なら現れる
	else
	{
		//表示していく
		m_PressColor.a += FLASH_SPEED;

		//透明度が１を上回ったら１に戻す
		if (m_PressColor.a > 1.0f) m_PressColor.a = 1.0f;
	}

	//プレス表示UIの色を設定
	m_pPress->SetVertexInfo(m_PressColor);
}

//=======================================
//描画処理
//=======================================
void CTitle::Draw(void)
{

}