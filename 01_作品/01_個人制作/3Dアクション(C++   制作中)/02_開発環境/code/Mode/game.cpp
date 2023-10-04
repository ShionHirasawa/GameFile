//================================================================================================
//
//ゲーム画面処理[game.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Main/file.h"
#include "game.h"
#include "../Object/object.h"
#include "../2D/object2D.h"
#include "../3D/object3D.h"
#include "../3D/billboard.h"
#include "../XFile/objectX.h"
#include "../XFile/enemy.h"
#include "../2D/bg.h"
#include "../3D/bullet.h"
#include "../3D/effect.h"
#include "../3D/particle.h"
#include "../3D/explosion.h"
#include "../3D/item.h"
#include "../2D/2Dblock.h"
#include "../Number/number.h"
#include "../Number/score.h"
#include "../Number/timer.h"
#include "../3D/camera.h"
#include "../3D/light.h"
#include "../3D/floor.h"
#include "../3D/treeBillboard.h"
#include "../XFile/jewel.h"
#include "../Player/player.h"

//ブロックの配置数
#define SET_NUM_BLOCK	(3)
//ブロックのサイズ
#define BLOCK_SIZE		 (60.0f)
//小さめのブロックサイズ
#define BLOCK_SMALL_SIZE (20.0f)

//内装情報ファイル名
const char	*CGame::FILENAME_INTERIOR = "data\\CSV\\interior.csv";

//オブジェクト配置情報ファイル名
const char	*CGame::FILENAME_OBJECT = "data\\CSV\\object.csv";

//敵の配置情報ファイル名
const char	*CGame::FILENAME_ENEMY = "data\\CSV\\enemy.csv";

//アイテムの配置数
const int	CGame::SET_NUM_ITEM = 3;

//=======================================
//コンストラクタ
//=======================================
CGame::CGame()
{

}

//=======================================
//デストラクタ
//=======================================
CGame::~CGame()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CGame::Init(void)
{
	//オブジェクトのテクスチャ読み込み
	TextureLoad();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//ゲーム内オブジェクト生成
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//スコアオブジェクト生成
	CScore *pScore = CScore::Create();
	pScore->Set();

	//タイマーオブジェクト生成
	CTimer *pTimer = CTimer::Create();
	pTimer->Set(180);

	//プレイヤーオブジェクト生成
	CPlayer *pPlayer = CPlayer::Create();
	pPlayer->SetPos(D3DXVECTOR3(600.0f, 0.0f, 600.0f));

	//カメラにプレイヤー情報を渡す
	CManager::GetCamera()->SetPlayer(pPlayer);

	//================================================================
	//ステージ配置物読み込み
	//================================================================
	//ファイルインスタンスを生成
	CFile *pFile = new CFile;
	if (pFile == NULL) return E_FAIL;

	SetInterior(pFile);	// 内装 読み込み（壁、床、天井など）
	SetObject(pFile);	//配置物読み込み
	SetEnemy(pFile);	//敵の情報読み込み

	delete pFile;	//メモリ開放
	pFile = NULL;

	//ゲーム内BGM再生
	CManager::GetSound()->Play(CSound::BGM_GAME);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CGame::Uninit(void)
{
	//フェード以外のオブジェクト全破棄
	CObject::ExceptReleaseAll(CObject::TYPE_FADE);
}

//=======================================
//更新処理
//=======================================
void CGame::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CGame::Draw(void)
{

}

//=======================================
//オブジェクトのテクスチャ読み込み
//=======================================
void CGame::TextureLoad(void)
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
//アイテムの配置
//=======================================
void CGame::SetItem(void)
{
	for (int nCntItem = 0; nCntItem < SET_NUM_ITEM; nCntItem++)
	{
		CItem *pItem = CItem::Create(D3DXVECTOR3(360.0f + 40.0f * nCntItem, 330.0f, 0.0f));
	}
}

//=======================================
//ステージの床や壁などの内装を配置
//=======================================
void CGame::SetInterior(CFile *pFile)
{
	//床・壁をすべて削除する
	CObject::ReleaseAll(CObject::TYPE_FLOOR);
	CObject::ReleaseAll(CObject::TYPE_WALL);

	//内装情報を読み込み
	pFile->LoadStageInterior(FILENAME_INTERIOR);
}

//=======================================
//オブジェクトの配置
//=======================================
void CGame::SetObject(CFile *pFile)
{
	//Ⅹモデルをすべて削除する
	CObject::ReleaseAll(CObject::TYPE_XMODEL);

	//ステージに配置するオブジェクト情報を読み込み
	pFile->LoadStageObject(FILENAME_OBJECT);
}

//=======================================
//敵の配置
//=======================================
void CGame::SetEnemy(CFile *pFile)
{
	//敵をすべて削除する
	CObject::ReleaseAll(CObject::TYPE_ENEMY);

	//敵の配置情報を読み込み
	pFile->LoadStageEnemy(FILENAME_ENEMY);
}