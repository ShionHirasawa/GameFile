//================================================================================================
//
//敵処理[enemy.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Number/score.h"
#include "objectX.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "../S_H_Lib/S_H_Control.h"
#include "../Player/player.h"
#include "../3D/object3D.h"
#include "../3D/wall.h"
#include "../3D/billboard.h"
#include "../3D/explosion.h"
#include "../3D/PolygonBill.h"
#include "../3D/shadow.h"
#include "../3D/mesh.h"
#include "../Main/sound.h"
#include "enemy.h"

/*敵の処理優先順位*/ #define PRIORITY_ENEMY	(3)
/*ポリゴンの幅	*/	 #define ENEMY_WIDTH	(60.0f)
/*ポリゴンの高さ*/	 #define ENEMY_HEIGHT	(60.0f)

/*テクスチャのパス*/ #define TEX_PATH	"data\\TEXTURE\\enemy.png"

/*移動速度*/		 #define MOVE_SPEED		(0.5f)
/*最大移動速度*/	 #define MAX_SPEED		(5.0f)
/*移動減衰量*/		 #define MOVE_ATTENUATE	(0.2f)
/*	 体力	*/		 #define ENEMY_LIFE		(5)

//静的メンバ変数
int	CEnemy::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CEnemy::CEnemy(int nPriority) : CObjectX(nPriority)
{
	//メンバ変数クリア
}

//=======================================
//デストラクタ
//=======================================
CEnemy::~CEnemy()
{

}

//=======================================
//生成処理
//=======================================
CEnemy *CEnemy::Create(char *pFilePath)
{
	//敵インスタンスの生成
	CEnemy *pEnemy = new CEnemy;

	if (pEnemy == NULL) return NULL;

	//初期化処理
	pEnemy->Init(pFilePath);

	//敵を返す
	return pEnemy;
}

//=======================================
//初期化処理
//=======================================
HRESULT CEnemy::Init(char *pFilePath)
{
	//頂点座標の初期化
	if(FAILED(CObjectX::Init(pFilePath, XMODELTYPE_NONE, 100)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_ENEMY);
	
	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CEnemy::Uninit(void)
{
	//頂点バッファの破棄など
	CObjectX::Uninit();
}

//=======================================
//更新処理
//=======================================
void CEnemy::Update(void)
{
	//移動操作
	MoveControl();
}

//---------------------
//移動操作処理
//---------------------
void CEnemy::MoveControl(void)
{
	//移動させる
	Move();
}

//=======================================
//描画処理
//=======================================
void CEnemy::Draw(void) { CEnemy::Draw(NULL); }

//=======================================
//描画処理
//=======================================
void CEnemy::Draw(D3DXCOLOR *pDiffuse)
{
	//自分を描画
	CObjectX::Draw(pDiffuse);
}

//=======================================
//位置設定
//=======================================
void CEnemy::SetPos(const D3DXVECTOR3 pos)
{
	CObjectX::SetPosition(pos);
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CEnemy::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}

//======================================================================================================
//												防犯カメラ
//======================================================================================================

//サーチライト色
const D3DXCOLOR CEnemySecurityCamera::SEARCH_LIGHT_COLOR = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.2f);

//索敵距離の長さ
const float CEnemySecurityCamera::SEARCH_LENGTH = 200.0f;

//索敵半径
const float CEnemySecurityCamera::SEARCH_RADIUS = 20.0f;

//円錐メッシュの分割数
const int CEnemySecurityCamera::NUM_DEVIDE = 16;

//円錐メッシュの全体頂点バッファ数
const int CEnemySecurityCamera::ALL_VERTEX = 1 + CEnemySecurityCamera::NUM_DEVIDE;

//円錐メッシュの全体インデックスバッファ数
const int CEnemySecurityCamera::ALL_INDEX = 1 + CEnemySecurityCamera::NUM_DEVIDE;

//回転速度
const float CEnemySecurityCamera::ROTAION_SPEED = 0.006f;

//回転中止のインターバル
const int CEnemySecurityCamera::ROTAION_INTERVAL = 50;

//=======================================
//コンストラクタ
//=======================================
CEnemySecurityCamera::CEnemySecurityCamera(int nPriority) : CEnemy(nPriority)
{
	//メンバ変数クリア
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_bFind = false;
	m_fPatrolStartRot = 0.0f;	//パトロールする開始角度
	m_fPatrolEndRot = 0.0f;		//パトロールする終了角度
	m_fRotationSpeed = 0.0f;	//回転速度
	m_bRotation = false;		//見渡す際の回転方向　　true：時計回り　　false：反時計回り
	m_nRotationInterval = 0;	//パトロールの限界角度に達した時の、一時的に回転中止するインターバル
}

//=======================================
//デストラクタ
//=======================================
CEnemySecurityCamera::~CEnemySecurityCamera()
{

}

//=======================================
//生成処理
//=======================================
CEnemySecurityCamera *CEnemySecurityCamera::Create(char *pFilePath)
{
	//インスタンス生成
	CEnemySecurityCamera *pCamera = new CEnemySecurityCamera(2);

	//インスタンス生成失敗
	if (!pCamera) return NULL;

	//初期化
	pCamera->Init(pFilePath);

	//生成したものを返す
	return pCamera;
}

//=======================================
//初期化処理
//=======================================
HRESULT CEnemySecurityCamera::Init(char *pFilePath)
{
	CEnemy::Init(pFilePath);

	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
		return E_FAIL;

	//頂点情報設定
	if (FAILED(SetVtxInfo()))
		return E_FAIL;

	//インデックスバッファを生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * ALL_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL)))	return E_FAIL;

	//インデックス情報設定
	if (FAILED(SetIdxInfo()))
		return E_FAIL;

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CEnemySecurityCamera::Uninit(void)
{
	//頂点バッファ破棄
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファ破棄
	if (m_pIdxBuff != NULL) {
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	CEnemy::Uninit();
}

//=======================================
//更新処理
//=======================================
void CEnemySecurityCamera::Update(void)
{
	//プレイヤーを発見している
	if (m_bFind)
	{
		//プレイヤーを追いかける処理
		Chaice();
	}
	else
	{
		//プレイヤーを探す
		Patrol();
	}

	//プレイヤーがサーチライト内に居るか判定
	//SearchLight();

	//頂点情報設定
	SetVtxInfo();
}

//=======================================
//描画処理
//=======================================
void CEnemySecurityCamera::Draw(void)
{
	//自分を描画
	CEnemy::Draw(NULL);

	//メッシュを描画
	DrawMesh();
}

//=======================================
//メッシュ描画処理
//=======================================
void CEnemySecurityCamera::DrawMesh(void)
{
	//デバイスを取得
	CRenderer *pRender = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//両面カリング
	pRender->SetCulling(D3DCULL_CW);

	//αテストON
	pRender->SetAlphaTest(true, 0);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, ALL_INDEX, 0, ALL_INDEX);

	//αテストOFF
	pRender->SetAlphaTest(false, 255);

	//通常カリング
	pRender->SetCulling();
}

//=======================================
//頂点情報設定
//=======================================
HRESULT CEnemySecurityCamera::SetVtxInfo(void)
{
	//頂点情報のポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))	return E_FAIL;

	//頂点座標の設定
	pVtx[0].pos = VEC3_INIT;
	pVtx[0].pos.y = -13.0f;

	// nor初期化
	pVtx[0].nor = NOR_INIT;

	// 色指定
	pVtx[0].col = SEARCH_LIGHT_COLOR;

	//テクスチャ座標の設定
	pVtx[0].tex = VEC2_INIT;

	//生成する角度を格納
	float Rot = D3DX_PI;

	//サーチライトの中心位置
	m_CirclePos = D3DXVECTOR3(
		sinf(D3DX_PI) * SEARCH_LENGTH,
		-GetPosition().y,
		cosf(D3DX_PI) * SEARCH_LENGTH);

	for (int nCntVtx = 1; nCntVtx < ALL_VERTEX; nCntVtx++)
	{
		D3DXVECTOR3 pos = VEC3_INIT;
		//頂点座標
		pos.x = pVtx[nCntVtx].pos.x = m_CirclePos.x + sinf(Rot) * SEARCH_RADIUS;
		pos.z = pVtx[nCntVtx].pos.z = m_CirclePos.z + cosf(Rot) * SEARCH_RADIUS;
		pos.y = pVtx[nCntVtx].pos.y = m_CirclePos.y;

		// nor初期化
		pVtx[nCntVtx].nor = NOR_INIT;

		// 色指定
		pVtx[nCntVtx].col = SEARCH_LIGHT_COLOR;

		//テクスチャ座標の設定
		pVtx[nCntVtx].tex = VEC2_INIT;

		//次に設定する頂点座標の角度を計算
		Rot -= FIX_ROT / (float)NUM_DEVIDE;
	}

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))	return E_FAIL;

	//設定完了
	return S_OK;
}

//=======================================
//インデックス情報設定
//=======================================
HRESULT CEnemySecurityCamera::SetIdxInfo(void)
{
	//頂点情報のポインタ
	WORD *pIdx = NULL;

	//インデックスバッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pIdxBuff->Lock(0, 0, (void* *)&pIdx, 0)))	return E_FAIL;

	//カウント数
	const int NUM_COUNT = ALL_INDEX - 1;

	for (int nCntIdx = 0; nCntIdx <= NUM_COUNT; nCntIdx++)
	{
		pIdx[nCntIdx] = nCntIdx % NUM_COUNT + nCntIdx / NUM_COUNT;
	}

	//インデックスバッファを開放（アンロック）する
	if (FAILED(m_pIdxBuff->Unlock()))	return E_FAIL;

	//設定完了
	return S_OK;
}

//============================================
//防犯カメラの設定処理
//引数１　pos	：位置
//引数２　rot	：初期向き
//引数３　fPatRot:索敵中に見る範囲
//============================================
void CEnemySecurityCamera::SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fPatRot)
{
	//３大ベクトル情報設定
	SetTransrate(pos, VEC3_INIT, rot);

	//パトロール情報設定
	m_fPatrolStartRot = rot.y;	//パトロールの初期角度
	m_fPatrolEndRot = fPatRot;	//パトロールの限界角度
	m_bRotation = false;		//回転方向初期化
	m_nRotationInterval = 0;	//インターバル初期化

	//終了角度が、初期角度から見て時計回り
	if (rot.y > fPatRot)	m_fRotationSpeed = ROTAION_SPEED;

	//反時計回りの場合
	else	m_fRotationSpeed = -ROTAION_SPEED;
}

//=======================================
//プレイヤーを探す処理
//=======================================
void CEnemySecurityCamera::Patrol(void)
{
	//回転一時停止中
	if (m_nRotationInterval > 0)
	{
		//停止時間減少
		m_nRotationInterval--;
		return;
	}

	//向きを取得
	D3DXVECTOR3 rot = GetRotation();

	//目標の角度を格納
	float TargetRot = 0.0f;

	//終了角度へ回転
	if (m_bRotation)
	{
		//終了角度を設定
		TargetRot = m_fPatrolEndRot;
	}
	else
	{
		//開始角度を設定
		TargetRot = m_fPatrolStartRot;
	}

	//回転させる
	rot.y += m_fRotationSpeed;

	//角度が設定した角度にほぼ同じ
	if (fabsf(rot.y - TargetRot) <= ROTAION_SPEED)
	{
		rot.y = TargetRot;			//角度を設定
		m_fRotationSpeed *= -1.0f;	//回転速度を反転
		m_bRotation = !m_bRotation;	//回転方向を反転
		m_nRotationInterval = ROTAION_INTERVAL;//一時停止
	}

	//向き修正
	FixRotate(&rot.y);

	//向き設定
	SetRotation(rot);
}

//=======================================
//プレイヤーがサーチライト内に居るか判定する処理
//=======================================
void CEnemySecurityCamera::SearchLight(void)
{
	return;
	//角度を取得
	D3DXVECTOR3 Rot = GetRotation();

	//サーチライトの場所を計算
	const D3DXVECTOR3 LightPos = D3DXVECTOR3(
		sinf(Rot.y) * SEARCH_LENGTH,
		0.0f,
		cosf(Rot.y) * SEARCH_LENGTH);

	const int PLAYER_PRIORITY = CPlayer::PRIORITY;
	const int NUM_OBJECT = CObject::GetNumAll(PLAYER_PRIORITY);

	for (int nCntPlayer = 0; nCntPlayer < NUM_OBJECT; nCntPlayer++)
	{
		//プレイヤーを取得
		CPlayer *pPlayer = (CPlayer *)CObject::GetObject(PLAYER_PRIORITY, nCntPlayer);

		//プレイヤーを取得できなかった OR プレイヤーはもう死んでいる
		if (pPlayer == NULL || pPlayer->GetType() != CObject::TYPE_PLAYER || pPlayer->GetState() == CPlayer::STATE_DEATH) continue;

		//プレイヤーの位置を取得
		const D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
		const D3DXVECTOR3 PosDiff = PlayerPos - LightPos;

		//*************************
		//プレイヤーが範囲内にいるかどうか　AND　プレイヤーとの間に壁があるかどうか
		//				↓
		//範囲内に居た場合 人間かどうか　または　人間状態じゃなくとも動いているかどうか
		//*************************
		if (D3DXVec3Length(&PosDiff) <= SEARCH_RADIUS &&
			pPlayer->GetTransformID() == ERROR_ID || D3DXVec3Length(&pPlayer->GetMoving()) > 0.1f)
			//プレイヤーが人間状態  OR  動いている  OR  見失ってから間もない
		{
			//プレイヤーまでの角度を取得
			const float fPosDiffRot = atan2f(LightPos - PlayerPos);

			//プレイヤーの方へ向かわせる
			OrientToDir(&Rot.y, fPosDiffRot, m_fRotationSpeed);
			SetRotation(Rot);
			m_bFind = true;
			break;

			//警告音を鳴らす
			//PlaySiren(true);

			//プレイヤー追尾中
			//return true;
		}

		//見つけていない
		m_bFind = false;
		//警告音を止める
		//PlaySiren(false);
	}
}

//=======================================
//プレイヤーを追いかける処理
//=======================================
void CEnemySecurityCamera::Chaice(void)
{

}

//======================================================================================================
//												ロボット
//======================================================================================================
const float CEnemyRobot::SEARCH_RANGE = 1.0f;	//索敵範囲の角度
const float CEnemyRobot::SEARCH_LENGTH = 150.0f;//索敵範囲の長さ
const float CEnemyRobot::CHACE_SPEED = 3.0f;	//追跡スピード
const float CEnemyRobot::ROTATE_SPEED = 0.2f;	//回転量
const int CEnemyRobot::MISS_INTERVAL = 90;		//見失ってから索敵する時間
const int CEnemyRobot::RESTART_INTERVAL = 3000;	//再起動までにかかる時間
const int CEnemyRobot::PATROL_INTERVAL = 60;	//巡回中に立ち止まる時間
const int CEnemyRobot::ATTACK_INTERVAL = 100;	//攻撃硬直

const float CEnemyRobot::SHADOW_SIZE = 25.0f;		//影ポリゴンのサイズ

bool CEnemyRobot::s_bSiren = false;

//=======================================
//コンストラクタ
//=======================================
CEnemyRobot::CEnemyRobot(int nPriority) : CEnemy(nPriority)
{
	//メンバ変数クリア
	m_nPatrolInterval = 0;	//巡回中のチェックポイントに到達した時の立ち止まるインターバル
	m_nMissInterval = 0;	//見失ってから索敵するインターバル
	m_nReStartInterval = 0;	//再起動インターバル
	m_nAttackInterval = 0;	//攻撃硬直インターバル
	m_nAttackPower = 1;		//攻撃力
	m_nLife = ENEMY_LIFE;	//体力

	m_nNextFootPrint = 5;	//次の足跡を表示する時間
	m_nMoveCounter = 0;		//足跡を表示するための移動カウンター

	m_nShadowID = ERROR_ID; //影番号
}

//=======================================
//デストラクタ
//=======================================
CEnemyRobot::~CEnemyRobot()
{

}

//=======================================
//生成処理
//=======================================
CEnemyRobot *CEnemyRobot::Create(char *pFilePath)
{
	//インスタンス生成
	CEnemyRobot *pRobot = new CEnemyRobot;

	//インスタンス生成失敗
	if (!pRobot) return NULL;

	//初期化
	pRobot->Init(pFilePath);

	//生成したものを返す
	return pRobot;
}

//=======================================
//初期化処理
//=======================================
HRESULT CEnemyRobot::Init(char *pFilePath)
{
	//初期化
	CEnemy::Init(pFilePath);

	//種類設定
	SetType(TYPE_ROBOT);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CEnemyRobot::Uninit(void)
{
	//チェックポイントが設定されている
	if (m_CheckPoint.pPoint != NULL)
	{
		delete m_CheckPoint.pPoint;
		m_CheckPoint.pPoint = NULL;
	}

	//自分の影を削除
	CShadow::Delete(m_nShadowID);

	//終了処理
	CEnemy::Uninit();
}

//=======================================
//更新処理
//=======================================
void CEnemyRobot::Update(void)
{
	//体力がないのなら専用更新処理へ
	if (m_nLife <= 0)
	{
		UpdateRestart();
		return;
	}

	//前回位置の更新
	UpdatePosOld();

	//関数で扱う３大VECTOR3型を宣言
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 move = VEC3_INIT;

	//プレイヤーを探す					->	//見つけられなかったらパトロール
	if(!SearchPlayer(&pos, &move, &rot))	Patrol(&pos, &move, &rot);

	//インターバルを経過
	CountInterval();

	//当たり判定
	CollisionWhole(&pos, &move, &rot);

	//３大VECTOR3型の代入
	SetTransrate(pos, move, rot);

	//足跡生成
	FootPrint(move, pos, rot.y);

	//影生成
	CShadow::SetPos(m_nShadowID, pos);
}

//=======================================
//再起動時の専用更新処理
//=======================================
void CEnemyRobot::UpdateRestart(void)
{
	//インターバル減少
	m_nReStartInterval--;

	//一定間隔で煙を表示
	if (m_nReStartInterval % 20 == 0)
	{
		//発生位置を敵の位置より少しずらす用
		D3DXVECTOR3 Range = VEC3_INIT;

		//角度を取得
		const float fRot = RandomRotate();

		//モデルID格納
		const int nID = GetMyXModelID();

		//発生半径取得
		const float fMargin = GetXModelMargin(nID);

		//発生位置取得
		Range.x = sinf(fRot) * fMargin;
		Range.z = cosf(fRot) * fMargin;

		int nHeight = (int)(GetPosition(nID, TYPE_Y, CObjectX::TYPE_MAX) * 100.0f);
		Range.y = (float)(rand() % nHeight) * FIX_FLOAT;

		CExplosion *pExplosion = CExplosion::Create(GetPosition() + Range, D3DXVECTOR3(0.0f, (float)(rand() % 100) * FIX_FLOAT + 0.02f, 0.0f), false);
		pExplosion->SetVertexInfo(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));		
	}

	//インターバル終了
	if (m_nReStartInterval <= 0)
	{
		//全インターバルクリア
		m_nAttackInterval = 0;
		m_nMissInterval = 0;
		m_nPatrolInterval = 0;
		m_nReStartInterval = 0;

		//体力を戻す
		m_nLife = ENEMY_LIFE;

		//攻撃力を２倍に
		m_nAttackPower *= 2;
	}
}

//=======================================
//描画処理
//=======================================
void CEnemyRobot::Draw(void)
{
	//モデルの色設定
	D3DXCOLOR *pDiffuse = m_nLife <= 0 ? &D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f): NULL;

	//描画
	CEnemy::Draw(pDiffuse);
}

//=======================================
//各種インターバルを減らす判定
//=======================================
void CEnemyRobot::CountInterval(void)
{
	//チェックポイントに到達して立ち止まっているインターバル
	if (m_nPatrolInterval > 0)	m_nPatrolInterval--;

	//プレイヤーに攻撃したインターバル
	if (m_nAttackInterval > 0)	m_nAttackInterval--;

	//見失ったインターバル
	if (m_nMissInterval > 0)
	{
		m_nMissInterval--;

		//チェックポイントを次の場所にする
		m_CheckPoint.nIdx = (m_CheckPoint.nIdx + 1) % m_CheckPoint.nNumAll;
	}
}

//=======================================
//巡回処理
//=======================================
void CEnemyRobot::Patrol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot)
{
	//チェックポイントが無いまたは何かしらインターバルがあるなら引き返す
	if (m_CheckPoint.pPoint == NULL || IsAnyInterval()) return;

	//現在と次のチェックポイントの番号を別名で代入
	int NOW_INDEX = m_CheckPoint.nIdx;
	int NEXT_INDEX = (NOW_INDEX + 1) % m_CheckPoint.nNumAll;

	//次のチェックポイントまでの角度を取得
	const float fDirRot = atan2f(m_CheckPoint.pPoint[NEXT_INDEX] - *pPos);

	//移動量を設定
	pMove->x = sinf(fDirRot) * m_CheckPoint.fMove;
	pMove->z = cosf(fDirRot) * m_CheckPoint.fMove;

	//チェックポイントを通過したか判定
	if (IsPassCheckPoint(m_CheckPoint.pPoint[NEXT_INDEX], *pPos + *pMove))
	{
		*pMove = VEC3_INIT;						//移動量を消す
		*pPos = m_CheckPoint.pPoint[NEXT_INDEX];//チェックポイントに移動
		m_CheckPoint.nIdx = NEXT_INDEX;			//次のチェックポイント番号へ
		m_nPatrolInterval = PATROL_INTERVAL;	//立ち止まる時間
	}

	//プレイヤーの方へ向かわせる
	OrientToDir(&pRot->y, atan2f(*pPos - m_CheckPoint.pPoint[NEXT_INDEX]), ROTATE_SPEED);
}

//=======================================
//各種インターバルが残っているか判定
//=======================================
bool CEnemyRobot::IsAnyInterval(void)
{
	//チェックポイントに到達して立ち止まっている
	if (m_nPatrolInterval > 0 || m_nMissInterval > 0 || m_nAttackInterval > 0) return true;

	//判定を返す
	return false;
}

//=======================================
//チェックポイントを通過したかどうか判定
//=======================================
bool CEnemyRobot::IsPassCheckPoint(D3DXVECTOR3 TargetPos, D3DXVECTOR3 Pos)
{
	//目的地までの距離を計算
	D3DXVECTOR3 posDiff = TargetPos - Pos;

	//目的地までの距離が近いなら通過したとする
	if (D3DXVec3Length(&posDiff) <= 2.0f) return true;

	//目的地までの距離が遠い
	return false;
}

//=======================================
//プレイヤーを探す処理
//=======================================
bool CEnemyRobot::SearchPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot)
{
	//ポインタがどれかがNULLなら引き返す
	if (pPos == NULL || pMove == NULL || pRot == NULL) return false;

	const int PLAYER_PRIORITY = CPlayer::PRIORITY;
	const int NUM_OBJECT = CObject::GetNumAll(PLAYER_PRIORITY);

	for (int nCntPlayer = 0; nCntPlayer < NUM_OBJECT; nCntPlayer++)
	{
		//プレイヤーを取得
		CPlayer *pPlayer = (CPlayer *)CObject::GetObject(PLAYER_PRIORITY, nCntPlayer);

		//プレイヤーを取得できなかった OR プレイヤーはもう死んでいる
		if (pPlayer == NULL || pPlayer->GetType() != CObject::TYPE_PLAYER || pPlayer->GetState() == CPlayer::STATE_DEATH) continue;

		//プレイヤーの位置を取得
		const D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		//*************************
		//プレイヤーが範囲内にいるかどうか　AND　プレイヤーとの間に壁があるかどうか
		//				↓
		//範囲内に居た場合 人間かどうか　または　人間状態じゃなくとも動いているかどうか
		//*************************
		if (Search(*pPos, PlayerPos, pRot->y + SEARCH_RANGE, pRot->y - SEARCH_RANGE, SEARCH_LENGTH) &&
			!CWall::IsAmongWall(*pPos, PlayerPos))
		{
			//プレイヤーが人間状態  OR  動いている  OR  見失ってから間もない
			if (pPlayer->GetTransformID() == ERROR_ID || D3DXVec3Length(&pPlayer->GetMoving()) > 0.1f || m_nMissInterval > 0)
			{
				//見失ってから索敵する時間
				m_nMissInterval = MISS_INTERVAL;

				//プレイヤーまでの角度を取得
				const float fRot = atan2f(*pPos - PlayerPos);

				//移動量を設定
				pMove->x = -sinf(fRot) * CHACE_SPEED;
				pMove->z = -cosf(fRot) * CHACE_SPEED;

				//プレイヤーの方へ向かわせる
				OrientToDir(&pRot->y, fRot, ROTATE_SPEED);

				//プレイヤーとの当たり判定
				if (CollisionPlayer(PlayerPos, pPlayer->COLLISION_MARGIN, pPos, CObjectX::GetXModelMargin(GetMyXModelID())) &&
					m_nAttackInterval == 0)
				{//当たった
					m_nAttackInterval = ATTACK_INTERVAL;//攻撃硬直
					pPlayer->GiveDamege(m_nAttackPower);//ダメージを与える
				}

				//警告音を鳴らす
				PlaySiren(true);

				//プレイヤー追尾中
				return true;
			}
		}

		//警告音を止める
		PlaySiren(false);
	}

	//プレイヤーを見つけられなかった
	return false;
}

//=======================================
//プレイヤーとの当たり判定処理
//=======================================
bool CEnemyRobot::CollisionPlayer(const D3DXVECTOR3 PlayerPos, const float fPlayerMargin, D3DXVECTOR3 *pPos, const float fMargin)
{
	//当たり範囲を足したもの
	const float fColliMargin = fPlayerMargin + fMargin;

	//プレイヤーとの距離を測定
	D3DXVECTOR3 PosDiff = *pPos - PlayerPos;
	const float fLength = D3DXVec3Length(&PosDiff);

	//当たり範囲にめり込んでいない
	if(fLength > fColliMargin)	return false;

	//*******************************************
	//プレイヤーにめり込まないように敵の位置調整
	//*******************************************
	//プレイヤーから敵までの角度
	const float fAngle = atan2f(PosDiff);

	//位置修正
	pPos->x = PlayerPos.x + sinf(fAngle) * fColliMargin;
	pPos->z = PlayerPos.z + cosf(fAngle) * fColliMargin;

	//当たった
	return true;
}

//=======================================
//ダメージ処理
//=======================================
void CEnemyRobot::GiveDamege(int nDamage)
{
	//ダメージを与える
	m_nLife -= nDamage;

	//体力無くなったら再起動準備
	if (m_nLife <= 0)
	{
		//再起動にかかる時間を設定
		m_nReStartInterval = RESTART_INTERVAL;
	}
}

//=======================================
//設定情報
//=======================================
void CEnemyRobot::SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CheckPoint CheckPoint, int nAttackPower)
{
	//影生成
	m_nShadowID = CShadow::Create(pos, SHADOW_SIZE);

	//攻撃力の設定
	m_nAttackPower = nAttackPower;

	//影設定
	CShadow::SetPos(m_nShadowID, pos);

	//３大VECTOR3型の設定
	SetTransrate(pos, VEC3_INIT, rot);

	//チェックポイントの情報を渡す
	m_CheckPoint.fMove = CheckPoint.fMove;
	m_CheckPoint.nIdx = CheckPoint.nIdx;
	m_CheckPoint.nNumAll = CheckPoint.nNumAll;

	//メモリのサイズを計算
	const int size = sizeof D3DXVECTOR3 * m_CheckPoint.nNumAll;

	//メモリを動的確保
	m_CheckPoint.pPoint = new D3DXVECTOR3[size];
	memcpy(m_CheckPoint.pPoint, CheckPoint.pPoint, size * m_CheckPoint.nNumAll);
}

//=======================================
//当たり判定まとめ
//=======================================
void CEnemyRobot::CollisionWhole(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot)
{
	const int nXModelID = GetMyXModelID();			//自分のⅩモデル番号を取得
	const D3DXVECTOR3 PosOld = GetPosOld();			//前回位置取得
	const float fMargin = GetXModelMargin(nXModelID);//当たり判定のサイズを取得
	const float fHeight = GetPosition(nXModelID, TYPE_Y, (CObjectX::TYPE)TYPE_MAX);

	for (int nCntType = 0; nCntType <= CObjectX::TYPE_Z; nCntType++)
	{	
		//順番に移動させる
		switch (nCntType)
		{
			case CObjectX::TYPE_X:	pPos->x += pMove->x; break;
			case CObjectX::TYPE_Y:	pPos->y += pMove->y; break;
			case CObjectX::TYPE_Z:	pPos->z += pMove->z; break;
		}

		//Ⅹモデルに触れてるか
		CObjectX::CollisionXModel(pPos, PosOld, pMove, (CObjectX::TYPE)nCntType, fMargin, nXModelID);
	}

	//壁に触れているか
	CWall::Collision(pPos, PosOld, pMove, fMargin, fHeight);
}

const int	CEnemyRobot::FOOTPRINT_SPAWN_RANGE = 10;	//足跡を発生する間隔
const int	CEnemyRobot::FOOTPRINT_SPAWN_MIN = 3;		//足跡発生の最低インターバル
const int	CEnemyRobot::FOOTPRINT_RANGE = 200;			//足跡を飛び散らせる範囲
const float	CEnemyRobot::FOOTPRINT_SIZE = 2.0f;			//足跡サイズ
const float	CEnemyRobot::FOOTPRINT_GRAVITY = -2.0f;		//足跡を落下させる重力量
const float	CEnemyRobot::FOOTPRINT_GRAVITY_CURRECT = 0.1f;//重力係数
const float	CEnemyRobot::FOOTPRINT_JUMP = 2.0f;			//発生した際の浮遊量
const float	CEnemyRobot::FOOTPRINT_SPEED = 1.0f;		//飛び散る速度
const int	CEnemyRobot::FOOTPRINT_LIFE = 20;			//寿命

//=======================================
//歩いた軌跡を生成
//=======================================
void CEnemyRobot::FootPrint(D3DXVECTOR3 move, D3DXVECTOR3 pos, float rotY)
{
	if (move == VEC3_INIT)
	{
		//移動カウンタークリア
		m_nMoveCounter = 0;
		return;
	}

	//移動時間増加
	m_nMoveCounter = (m_nMoveCounter + 1) % m_nNextFootPrint;

	if (m_nMoveCounter == 0)
	{
		//次に足跡が発生するタイミングを決める
		m_nNextFootPrint = rand() % FOOTPRINT_SPAWN_RANGE + FOOTPRINT_SPAWN_MIN;

		//足跡生成
		CPolyBill::CreateFootPrint(pos, rotY, FOOTPRINT_RANGE,
								   FOOTPRINT_SIZE, GetColor(COL_BLACK), FOOTPRINT_GRAVITY, FOOTPRINT_GRAVITY_CURRECT,
								   FOOTPRINT_JUMP, FOOTPRINT_SPEED, FOOTPRINT_LIFE);
	}
}

//=======================================
//警告音を鳴らす処理
//=======================================
void CEnemyRobot::PlaySiren(bool bPlay)
{
	//設定する状態と、現在の状態が同じなら処理をしない
	if (s_bSiren == bPlay) return;

	//状態更新
	s_bSiren = bPlay;

	//サウンド再生切り替え
	CManager::GetSound()->Coroutine(CSound::SE_SIREN_ROBOT, bPlay);
}