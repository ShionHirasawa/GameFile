//================================================================================================
//
//チュートリアル画面処理[tutorial.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Main/file.h"
#include "tutorial.h"
#include "../Object/object.h"
#include "../2D/object2D.h"
#include "../2D/Polygon2D.h"
#include "../2D/fade.h"
#include "../3D/object3D.h"
#include "../3D/floor.h"
#include "../3D/wall.h"
#include "../3D/camera.h"
#include "../3D/billboard.h"
#include "../3D/PolygonBill.h"
#include "../3D/bullet.h"
#include "../3D/effect.h"
#include "../3D/explosion.h"
#include "../3D/Polygon3D.h"
#include "../Number/number.h"
#include "../XFile/objectX.h"
#include "../XFile/enemy.h"
#include "../Player/player.h"

//操作説明テクスチャ情報
const D3DXVECTOR3	CTutorial::CONTROLL_TEX_CENTER_POS = D3DXVECTOR3(SCREEN_CENTER_POS);
const float			CTutorial::CONTROLL_TEX_CENTER_WIDTH = 600.0f;
const float			CTutorial::CONTROLL_TEX_CENTER_HEIGHT = 300.0f;
const D3DXVECTOR3	CTutorial::CONTROLL_TEX_WIPE_POS = D3DXVECTOR3(210.0f, 120.0f, 0.0f);
const float			CTutorial::CONTROLL_TEX_WIPE_WIDTH = 280.0f;
const float			CTutorial::CONTROLL_TEX_WIPE_HEIGHT = 140.0f;

//オブジェクトの個性説明テクスチャ情報
//近い時のサイズ情報など
const float CTutorial::OBJ_IDENTITY_TEX_NEAR_WIDTH = 80.0f;
const float CTutorial::OBJ_IDENTITY_TEX_NEAR_HEIGHT = 60.0f;
const float CTutorial::OBJ_IDENTITY_NEAR_SET_POS_Y = 120.0f;

//遠い時のサイズ情報など
const float CTutorial::OBJ_IDENTITY_TEX_FAR_WIDTH = 60.0f;
const float CTutorial::OBJ_IDENTITY_TEX_FAR_HEIGHT = 15.0f;
const float CTutorial::OBJ_IDENTITY_FAR_SET_POS_Y = 80.0f;
const float CTutorial::OBJ_IDENTITY_FAR_TEX_Y = 0.2f;

//その他共通情報
const float CTutorial::OBJ_IDENTITY_LENGTH = 90.0f;
const D3DXVECTOR3 CTutorial::OBJ_IDENTITY_SET_POS[NUM_OBJ_IDENTITY_TEXTURE] = {
	D3DXVECTOR3(+200.0f, OBJ_IDENTITY_FAR_SET_POS_Y, -100.0f),	//プランター
	D3DXVECTOR3(+200.0f, OBJ_IDENTITY_FAR_SET_POS_Y, -300.0f),	//段ボール
	D3DXVECTOR3(-200.0f, OBJ_IDENTITY_FAR_SET_POS_Y, -100.0f),	//木箱
	D3DXVECTOR3(-200.0f, OBJ_IDENTITY_FAR_SET_POS_Y, -300.0f),	//机
};

char *CTutorial::s_pControllTexturePath[NUM_CONTROLL_TEXTURE] = {
	"data\\TUTORIAL\\tutorial000.png",
	"data\\TUTORIAL\\tutorial001.png",
};

char *CTutorial::s_pObjectTexturePath[NUM_OBJ_IDENTITY_TEXTURE] = {
	"data\\TUTORIAL\\Identity000.png",
	"data\\TUTORIAL\\Identity001.png",
	"data\\TUTORIAL\\Identity002.png",
	"data\\TUTORIAL\\Identity003.png",
};

//=======================================
//コンストラクタ
//=======================================
CTutorial::CTutorial()
{
	//メンバ変数クリア
	m_pPlayer = NULL;

	//操作説明テクスチャの変数クリア
	m_Controll.pPoly2D = NULL;
	m_Controll.nTexID = 0;
	m_Controll.bDispChange = true;
	m_Controll.pos =		VEC3_INIT;
	m_Controll.fWidth = 0.0f;
	m_Controll.fHeight = 0.0f;

	//オブジェクトの個性説明テクスチャの変数クリア
	for (int nCntObj = 0; nCntObj < NUM_CONTROLL_TEXTURE; nCntObj++)
	{
		m_ObjIdentity[nCntObj].pPolyBill = NULL;
		m_ObjIdentity[nCntObj].pos = VEC3_INIT;
		m_ObjIdentity[nCntObj].fWidth = 0.0f;
		m_ObjIdentity[nCntObj].fHeight = 0.0f;
		m_ObjIdentity[nCntObj].fTexV = 0.0f;
	}
}

//=======================================
//デストラクタ
//=======================================
CTutorial::~CTutorial()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CTutorial::Init(void)
{
	//テクスチャを読み込み
	TextureLoad();

	//ステージ配置
	SetStage();

	//BGM再生
	CManager::GetSound()->Play(CSound::BGM_TUTORIAL);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CTutorial::SetStage(void)
{
	//チュートリアルで使うオブジェクトを総破棄
	CObject::ReleaseAll(CObject::TYPE_WALL);
	CObject::ReleaseAll(CObject::TYPE_FLOOR);
	CObject::ReleaseAll(CObject::TYPE_PLAYER);
	CObject::ReleaseAll(CObject::TYPE_ENEMY);
	CObject::ReleaseAll(CObject::TYPE_ROBOT);
	CObject::ReleaseAll(CObject::TYPE_XMODEL);
	CObject::ReleaseAll(CObject::TYPE_JEWEL);
	CObject::ReleaseAll(CObject::TYPE_POLY_2D);
	CObject::ReleaseAll(CObject::TYPE_POLY_3D);
	CObject::ReleaseAll(CObject::TYPE_POLY_BILL);
	CObject::ReleaseAll(CObject::TYPE_MESH);

	//フロア
	{
		//テクスチャ読み込み
		CFloor *pFloor = CFloor::Create();
		pFloor->Load("data\\TUTORIAL\\carpet001.jpg");
		pFloor->Set(D3DXVECTOR3(0, 0, 200), VEC3_INIT, D3DXVECTOR3(100.0f, 0.0f, 200.0f), D3DXVECTOR2(3.0f, 6.0f));
	}

	//プレイヤー生成
	{
		m_pPlayer = CPlayer::Create();
		m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, -900.0f));

		//カメラにプレイヤー情報を渡す
		CManager::GetCamera()->SetPlayer(m_pPlayer);
	}

	//ステージ配置物読み込み
	{
		//ファイルインスタンスを生成
		CFile *pFile = new CFile;
		if (pFile == NULL) return;

		//読込処理
		pFile->LoadStageInterior("data\\TUTORIAL\\interior.csv");
		pFile->LoadStageObject("data\\TUTORIAL\\object.csv");
		pFile->LoadStageEnemy("data\\TUTORIAL\\enemy.csv");

		//メモリ開放
		delete pFile;
		pFile = NULL;
	}

	//敵説明ポリゴン
	{
		CPoly3D *pEnemy = CPoly3D::Create("data\\TUTORIAL\\enemyTuto000.png");
		pEnemy->SetRotation(VEC3_INIT);
		pEnemy->SetInfo(D3DXVECTOR3(0.0f, 60.0f, 519.0f), 50.0f, 40.0f, 0.0f);
		pEnemy->DispSwitch(true);
	}

	//操作説明ポリゴン情報設定
	m_Controll.pos = CONTROLL_TEX_CENTER_POS;
	m_Controll.fWidth = CONTROLL_TEX_CENTER_WIDTH;
	m_Controll.fHeight = CONTROLL_TEX_CENTER_HEIGHT;
	m_Controll.pPoly2D = CPoly2D::Create(s_pControllTexturePath[0]);
	SwapControllTexture();

	//個性説明ポリゴン情報設定
	for (int nCntObj = 0; nCntObj < NUM_OBJ_IDENTITY_TEXTURE; nCntObj++)
	{
		//変数名長いので縮める
		ObjIdentity *pObj = &m_ObjIdentity[nCntObj];

		//位置情報設定
		pObj->pos = OBJ_IDENTITY_SET_POS[nCntObj];

		//サイズを遠い距離用に設定
		pObj->fWidth = OBJ_IDENTITY_TEX_FAR_WIDTH;
		pObj->fHeight = OBJ_IDENTITY_TEX_FAR_HEIGHT;
		pObj->fTexV = OBJ_IDENTITY_FAR_TEX_Y;

		//サイズを変えていく
		pObj->pPolyBill = CPolyBill::Create(s_pObjectTexturePath[nCntObj]);
		pObj->pPolyBill->ZTestSwitch(false);
		pObj->pPolyBill->SetInfo(pObj->pos, pObj->fWidth, pObj->fHeight, 0.0f, VEC2_INIT, 1.0f, pObj->fTexV);
	}
}

//=======================================
//オブジェクトのテクスチャ読み込み
//=======================================
void CTutorial::TextureLoad(void)
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//オブジェクトのテクスチャ読みこみ（２Ｄ・アルファベット順
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/*    弾    */	CBullet::Load();
	/*エフェクト*/	CEffect::Load();
	/*    敵    */	CEnemy::Load();
	/*   爆発   */	CExplosion::Load();
	/*数字テクスチャ*/CNumber::Load();
}

//=======================================
//終了処理
//=======================================
void CTutorial::Uninit(void)
{
	//フェード以外のオブジェクト全破棄
	CObject::ExceptReleaseAll(CObject::TYPE_FADE);
}

//=======================================
//更新処理
//=======================================
void CTutorial::Update(void)
{
	//操作説明テクスチャ表示ON/OFF切り替え
	SwitchDispPoly2D();

	//プレイヤーの操作切り替えはテクスチャの表示方法によって常に左右される
	m_pPlayer->IsTutorialControll(!m_Controll.bDispChange);

	//個性説明テクスチャのサイズ更新
	UpdateObjIdentity();

	//テクスチャを切り替える
	SwapControllTexture();

	//チュートリアル終了
	End();

#ifdef _DEBUG
	if (CManager::GetKeyboard()->GetTrigger(DIK_TAB)) SetStage();
#endif
}

//=======================================
//描画処理
//=======================================
void CTutorial::Draw(void)
{

}

//=======================================
//個性説明の更新処理
//=======================================
void CTutorial::UpdateObjIdentity(void)
{
	//プレイヤーの位置を格納
	D3DXVECTOR3 *pPlayerPos = NULL;

	const int PLAYER_PRIORITY = CPlayer::PRIORITY;
	const int NUM_ALL_OBJ = CObject::GetNumAll(PLAYER_PRIORITY);

	for (int nCntPlayer = 0; nCntPlayer < NUM_ALL_OBJ; nCntPlayer++)
	{
		//プレイヤー取得
		CPlayer *pPlayer = (CPlayer *)CObject::GetObjectA(PLAYER_PRIORITY, nCntPlayer++);

		//取得したのがプレイヤーではなかった
		if (pPlayer == NULL || pPlayer->GetType() != CObject::TYPE_PLAYER) continue;

		//プレイヤーの位置を格納
		pPlayerPos = &pPlayer->GetPosition();
		break;
	}

	//プレイヤーの位置を取得できなかった
	if (pPlayerPos == NULL) return;

	//各個性説明テクスチャを大きく見せるかどうか判定
	for (int nCntObj = 0; nCntObj < NUM_OBJ_IDENTITY_TEXTURE; nCntObj++)
	{
		//変数名長いので縮める
		ObjIdentity *pObj = &m_ObjIdentity[nCntObj];

		//位置の差分を格納
		D3DXVECTOR3 PosDiff = pObj->pos - *pPlayerPos;
		PosDiff.y = 0.0f;

		//近い距離にいる
		if (D3DXVec3Length(&PosDiff) <= OBJ_IDENTITY_LENGTH)
		{
			//サイズを近い距離用に変化させる
			pObj->fWidth +=	(OBJ_IDENTITY_TEX_NEAR_WIDTH - pObj->fWidth) * 0.1f;
			pObj->fHeight +=(OBJ_IDENTITY_TEX_NEAR_HEIGHT - pObj->fHeight) * 0.1f;
			pObj->pos.y += (OBJ_IDENTITY_NEAR_SET_POS_Y - pObj->pos.y) * 0.1f;
			pObj->fTexV += (1.0f - pObj->fTexV) * 0.1f;
			pObj->pPolyBill->ZTestSwitch(true);
		}
		else
		{
			//サイズを遠い距離用に変化させる
			pObj->fWidth +=	(OBJ_IDENTITY_TEX_FAR_WIDTH - pObj->fWidth) * 0.1f;
			pObj->fHeight +=(OBJ_IDENTITY_TEX_FAR_HEIGHT - pObj->fHeight) * 0.1f;
			pObj->pos.y += (OBJ_IDENTITY_FAR_SET_POS_Y - pObj->pos.y) * 0.1f;
			pObj->fTexV += (OBJ_IDENTITY_FAR_TEX_Y - pObj->fTexV) * 0.1f;
			pObj->pPolyBill->ZTestSwitch(false);
		}

		//サイズを変えていく
		pObj->pPolyBill->SetInfo(pObj->pos, pObj->fWidth, pObj->fHeight, 0.0f, VEC2_INIT, 1.0f, pObj->fTexV);
	}
}

//=======================================
//操作説明ポリゴンの表示切り替え
//=======================================
void CTutorial::SwitchDispPoly2D(void)
{
	//表示方法切り替え
	if (CManager::IsInputTrigger(DIK_Z, CInputGamePad::X)) m_Controll.bDispChange ^= TRUE;

	//表示テクスチャ切り替え
	if (CManager::IsInputTrigger(DIK_Q, CInputGamePad::L_TRIGGER))
	{
		m_Controll.nTexID = (m_Controll.nTexID + 1) % NUM_CONTROLL_TEXTURE;
		m_Controll.pPoly2D->SetTexture(s_pControllTexturePath[m_Controll.nTexID]);
	}
}

//=======================================
//チュートリアル終了
//=======================================
void CTutorial::End(void)
{
	//画面遷移
	if (CManager::IsInputTrigger(DIK_RETURN, CInputGamePad::A))
	{
		CFade::Set(CScene::GAME, CFade::FADE_WIPE_OUT);
	}
}

//=======================================
//操作説明テクスチャの切り替え
//=======================================
void CTutorial::SwapControllTexture(void)
{
	//現在の表示方法にあったサイズを格納
	D3DXVECTOR3 TargetPos = VEC3_INIT;
	float TargetWidth = 0.0f;
	float TargetHeight = 0.0f;

	//中央表示の場合
	if (m_Controll.bDispChange)
	{
		TargetPos = CONTROLL_TEX_CENTER_POS;
		TargetWidth = CONTROLL_TEX_CENTER_WIDTH;
		TargetHeight = CONTROLL_TEX_CENTER_HEIGHT;
	}
	//左上ワイプ表示の場合
	else
	{
		TargetPos = CONTROLL_TEX_WIPE_POS;
		TargetWidth = CONTROLL_TEX_WIPE_WIDTH;
		TargetHeight = CONTROLL_TEX_WIPE_HEIGHT;
	}

	//徐々に表示サイズに変化させていく
	m_Controll.pos.x += (TargetPos.x - m_Controll.pos.x) * 0.1f;
	m_Controll.pos.y += (TargetPos.y - m_Controll.pos.y) * 0.1f;
	m_Controll.fWidth += (TargetWidth - m_Controll.fWidth) * 0.1f;
	m_Controll.fHeight += (TargetHeight - m_Controll.fHeight) * 0.1f;

	//ポリゴンサイズを指定する
	m_Controll.pPoly2D->SetInfo(m_Controll.pos, m_Controll.fWidth, m_Controll.fHeight);
	m_Controll.pPoly2D->DispSwitch(false);
}