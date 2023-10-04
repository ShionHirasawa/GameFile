//================================================================================================
//
//プレイヤー処理[player.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/object3D.h"
#include "../3D/floor.h"
#include "../3D/billboard.h"
#include "../3D/camera.h"
#include "../3D/bullet.h"
#include "../3D/wall.h"
#include "../Main/file.h"
#include "../Player/player.h"
#include "../XFile/jewel.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "../S_H_Lib/S_H_Control.h"
#include "../2D/object2D.h"
#include "../2D/fade.h"
#include "../2D/Polygon2D.h"
#include "../Mode/result.h"
#include "../3D/PolygonBill.h"
#include "../3D/shadow.h"

//==============================
//プレイヤーマクロ
//==============================
/* ポリゴンの幅 */	#define PLAYER_WIDTH	(35.0f)
/*ポリゴンの高さ*/	#define PLAYER_HEIGHT	(40.0f)

//モデルパーツ情報が書かれたパス
#define MODEL_PARTS_INFO	"data\\motion.txt"

//スニーク速度
#define SNEAK_SPEED		(0.4f)
//最大スニーク速度
#define MAX_SNEAK_SPEED	(1.0f)

//歩き速度
#define WALK_SPEED		(1.0f)
//最大歩き速度
#define MAX_WALK_SPEED	(3.0f)

//移動減衰量
#define MOVE_ATTENUATE	(0.2f)
//ジャンプ量
#define JUMP_POWER		(20.0f)
//回転量
#define ROTATE_SPEED	(0.08f)

//移動モーションを止める移動ベクトル
#define STOP_MOTION_VECTOR	(0.1f)

//重力係数
#define GRAVITY_MAG		(0.08f)
//重力加速度
#define GRAVITY_POWER	(9.0f)

//弾の発射角度の回転量
#define BULLET_ANGLE_ROTATE	(0.03f)
//弾の発射間隔
#define BULLET_INTERVAL		(5)
//弾の発射補正位置
#define BULLET_FIRE_POS		(D3DXVECTOR3(0.0f, 70.0f, 0.0f))

//変身できる距離
#define TRANSFORM_LENGTH	(100.0f)
//変身可能な角度
#define TRANSFORM_ANGLE		(0.3f)

//静的メンバ変数
const float	CPlayer::WIDTH_MARGIN = 0.7f;
const float	CPlayer::HEIGHT_MARGIN = 0.7f;
const float	CPlayer::COLLISION_MARGIN = 20.0f;
const float	CPlayer::COLLISION_LENGTH = 100.0f;
const int	CPlayer::PRIORITY = 4;
const int	CPlayer::MAX_LIFE = 100;
const int	CPlayer::DAMAGE_INTERVAL = 40;
const int	CPlayer::DEATH_INTERVAL = 100;
const int	CPlayer::SWITCH_DISP = 2;
const float CPlayer::PUT_OBJX_LENGTH = 50.0f;
const float CPlayer::NIMBLE_SPEED = 1.5f;
const float CPlayer::TOUGH_SPEED = 00.5f;

//体力ポリゴンの背景
const	D3DXVECTOR3 CPlayer::LIFE_BG_POS = D3DXVECTOR3(1140.0f, 650.0f, 0.0f);
const	float		CPlayer::LIFE_BG_WIDTH = 130.0f;
const	float		CPlayer::LIFE_BG_HEIGHT = 60.0f;
const	D3DXCOLOR	 CPlayer::LIFE_BG_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);

//体力のロゴ
char	*CPlayer::LOGO_TEXTURE_PATH = "data\\TEXTURE\\Life.png";
const	D3DXVECTOR3 CPlayer::LIFE_LOGO_POS = D3DXVECTOR3(1080.0f, 630.0f, 0.0f);
const	float		CPlayer::LIFE_LOGO_WIDTH = 50.0f;
const	float		CPlayer::LIFE_LOGO_HEIGHT = 20.0f;

//体力ポリゴン
char	*CPlayer::LIFE_TEXTURE_PATH = "data\\TEXTURE\\number001.png";
const	D3DXVECTOR3 CPlayer::LIFE_POS = D3DXVECTOR3(1200.0f, 660.0f, 0.0f);
const	float CPlayer::LIFE_WIDTH = 20.0f;
const	float CPlayer::LIFE_HEIGHT = 30.0f;
const	float CPlayer::LIFE_MARGIN = 20.0f;
const	float CPlayer::LIFE_TEX_WIDTH = 0.1f;
const	float CPlayer::LIFE_TEX_HEIGHT = 1.0f;

//サイズ拡縮の遷移スピード
const float CPlayer::SCALING_SPEED = 0.05f;

//影ポリゴンのサイズ
const float CPlayer::SHADOW_SIZE = 15.0f;

//=======================================
//コンストラクタ
//=======================================
CPlayer::CPlayer() : CObject(PRIORITY)
{
	//メンバ変数クリア
	/*影番号*/		m_nShadowID = ERROR_ID;
	/*スニーク*/	m_bSneak = false;
	/*描画ON/OFF*/	m_bDisp = true;
	/*操作ON/OFF*/	m_bControll = true;
	/*変身ON/OFF*/	m_bTransform = true;
	/*拡縮ON/OFF*/	m_bScaling = true;
	/*拡縮サイズ*/	m_fScal = 1.0f;
	/*モデル総数*/	m_nNumModel = 0;
	/*変身番号*/	m_nTransformModelID = m_nNextModelID = ERROR_ID;	
	/*位置初期化*/	m_pos = VEC3_INIT;
	/*向き初期化*/	m_rot = VEC3_INIT;
	/*体力クリア*/	m_nLifeMax = m_nLife = MAX_LIFE;
	/*状態クリア*/	m_State = STATE_NORMAL;
	/*ヒット硬直クリア*/m_nDamageInterval = 0;
	/*死亡インターバル*/m_nDeathInterval = 0;
	/*個性による移動量増減量*/m_fSpecialMaxSpeed = 1.0f;
	/*足跡を表示するための移動カウンター*/m_nMoveCounter = 0;
	/*次の足跡を表示する時間*/m_nNextFootPrint = 5;


	//収納するⅩオブジェクトをクリア
	m_nNumTakeObjX = 0;
	for (int nCntObjX = 0; nCntObjX < NUM_TAKE_OBJECTX; nCntObjX++)
	{
		m_pObjX[nCntObjX] = NULL;
	}

	//モデルのポインタをNULLに
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;
	}

	//体力系ポリゴンのポインタをNULLに
	m_pLifeBG = NULL;
	m_pLifeLogo = NULL;
	for(int nCntLife = 0; nCntLife < NUM_LIFE_DEJIT; nCntLife++)
	{
		m_pLife[nCntLife] = NULL;
	}

	//モーションのポインタをNULLに
	m_pMotion = NULL;
}

//=======================================
//デストラクタ
//=======================================
CPlayer::~CPlayer()
{

}

//=======================================
//生成処理
//=======================================
CPlayer *CPlayer::Create(void)
{
	//プレイヤーインスタンスの生成
	CPlayer *pPlayer = new CPlayer;

	//初期化処理
	pPlayer->Init();

	//プレイヤーを返す
	return pPlayer;
}

//=======================================
//初期化処理
//=======================================
HRESULT CPlayer::Init(void)
{
	//モーションインスタンス生成
	m_pMotion = new CMotion;

	//モデルパーツの情報読み取り
	LoadPartsInfo();

	//モーションを「待機」状態に設定
	m_pMotion->Set(CMotion::MOTIONTYPE_NEUTRAL);
	m_pMotion->SetPos();

	//体力ポリゴン2D生成
	SetLifePoly2D();

	//影ポリゴン生成
	m_nShadowID = CShadow::Create(m_pos, SHADOW_SIZE);

	//種類設定
	SetType(CObject::TYPE_PLAYER);
	
	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CPlayer::Uninit(void)
{
	//保有している全モデル終了処理
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();	//終了処理へ
		delete m_apModel[nCntModel];	//ポインタ破棄
		m_apModel[nCntModel] = NULL;	//NULLポインタに
	}

	//モーションのポインタ破棄
	if (m_pMotion != NULL)
	{
		delete m_pMotion;	//メモリ削除
		m_pMotion = NULL;	//ポインタをNULLに
	}

	//プレイヤーは消えるので、覚えてくれていたポインタをNULLにする
	CManager::GetCamera()->SetPlayer(NULL);
	CManager::GetCamera()->Init();

	//自分の影を削除
	CShadow::Delete(m_nShadowID);

	//頂点バッファの破棄など
	this->Release();
}

//=======================================
//更新処理
//=======================================
void CPlayer::Update(void)
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_F1))
	{
		m_bDisp = !m_bDisp;
	}

	//前回の位置更新
	m_posOld = m_pos;

	//移動操作
	ActionControl();

	//当たり判定まとめ
	CollisionWhole();

	/* 移動量を減衰 */ m_move.x += (0.0f - m_move.x) * MOVE_ATTENUATE;
	/* 移動量を減衰 */ m_move.z += (0.0f - m_move.z) * MOVE_ATTENUATE;
	///*重力に従わせる*/ m_move.y -= (GRAVITY_POWER + m_move.y) * GRAVITY_MAG;

	//オブジェクトの個性別の更新処理
	TransformUpdateEntrance();

	//状態別の更新処理
	StateUpdateEntrance();

	//モーションの更新処理
	m_pMotion->Update();

	//弾を発射する
	SetBullet();

	//変身
	Transform();

	//ゴール処理
	Goal();

	//体力ポリゴン2D生成
	SetLifePoly2D();

	//影の位置設定
	Shadow();

	//行動していない & モーションが終了した
	if (m_pMotion->IsFinish())
	{
		//待機モーションへ移行
		m_pMotion->SetBlend(CMotion::MOTIONTYPE_NEUTRAL);
	}
}

//=======================================
//アクション操作処理
//=======================================
void CPlayer::ActionControl(void)
{
	//死んでいる or チュートリアル中の特殊状態 or 変身中ならアクション不能
	if (m_State == STATE_DEATH || !m_bControll || !m_bTransform) return;

	/*カメラの角度*/	float CameraRot = CManager::GetCamera()->GetInverseRotate();
	/*プレイヤーを向かせる角度*/ float PlayerRotate = m_rot.y;

	//移動にまつわる変数の初期値は通常移動の場合
	/*移動速度*/		float fMoveSpeed = WALK_SPEED;
	/*最大移動量*/		float fMaxMoveSpeed = MAX_WALK_SPEED * m_fSpecialMaxSpeed;
	/*移動モーション*/	int nMoveMotionType = CMotion::MOTIONTYPE_WALK;

	//スニーク状態切り替え
	if (CManager::IsInputPress(DIK_LCONTROL, CInputGamePad::L_THUMB))	m_bSneak ^= 1;

	//スニーク状態の場合
	if (m_bSneak)
	{
		/*移動速度をスニーク専用に*/	fMoveSpeed = SNEAK_SPEED;
		/*最大移動速度もスニーク用*/	fMaxMoveSpeed = MAX_SNEAK_SPEED * m_fSpecialMaxSpeed;
		/*モーションも【スニーク】*/	nMoveMotionType = CMotion::MOTIONTYPE_SNEAK;
	}

	{
		//サブキーの情報
		XZPlane_SubInfo Sub[2];

		//左に移動する場合のサブ情報設定
		Sub[0].Key1 = DIK_S;		Sub[0].Key2 = DIK_W;
		Sub[0].MoveRot1 = PI_LEFT_UP;	Sub[0].MoveRot2 = PI_LEFT_DOWN;

		//右に移動する場合のサブ情報設定
		Sub[1].Key1 = DIK_S;		Sub[1].Key2 = DIK_W;
		Sub[1].MoveRot1 = PI_RIGHT_UP;	Sub[1].MoveRot2 = PI_RIGHT_DOWN;

		if		(Move_XZPlane(CameraRot, fMoveSpeed, &m_move, &PlayerRotate, m_pMotion, nMoveMotionType));
		else if	(Move_XZPlane(DIK_A, CInputGamePad::LEFT,	PI_LEFT,	CameraRot, fMoveSpeed, &m_move, &Sub[0], &PlayerRotate, m_pMotion, nMoveMotionType));
		else if (Move_XZPlane(DIK_D, CInputGamePad::RIGHT,	PI_RIGHT,	CameraRot, fMoveSpeed, &m_move, &Sub[1], &PlayerRotate, m_pMotion, nMoveMotionType));
		else if (Move_XZPlane(DIK_S, CInputGamePad::DOWN,	PI_UP,		CameraRot, fMoveSpeed, &m_move, NULL,	 &PlayerRotate, m_pMotion, nMoveMotionType));
		else if (Move_XZPlane(DIK_W, CInputGamePad::UP,		PI_DOWN,	CameraRot, fMoveSpeed, &m_move, NULL,	 &PlayerRotate, m_pMotion, nMoveMotionType));
		else if (CManager::IsInputTrigger(DIK_RSHIFT, CInputGamePad::R_TRIGGER))	m_rot.y += 0.01f;//時計回りに回転
		FixRotate(&PlayerRotate);			//プレイヤーの角度修正
	}

	//プレイヤーを進行方向に向かせる
	OrientToDir(&m_rot.y, PlayerRotate, ROTATE_SPEED);

	//Bキーが押されたら銃を構える（人間状態のとき限定
	if (CManager::IsInputTrigger(DIK_B, CInputGamePad::B) && m_nTransformModelID == ERROR_ID)	m_pMotion->Set(CMotion::MOTIONTYPE_ACTION);

	//横の移動量を範囲内に収める
	Range(&m_move.x, fMaxMoveSpeed);
	Range(&m_move.z, fMaxMoveSpeed);

	//歩いた軌跡を生成
	FootPrint();

	//移動ベクトルがほぼ無い
	if (D3DXVec3Length(&m_move) <= STOP_MOTION_VECTOR)
	{
		//移動カウンタークリア
		m_nMoveCounter = 0;

		//現在の移動モーションから、待機モーションへ移行させる
		if (m_pMotion->GetType() == nMoveMotionType)
		{
			//待機モーションへ移行
			m_pMotion->SetBlend(CMotion::MOTIONTYPE_NEUTRAL);
		}
	}
}

//=======================================
//描画処理
//=======================================
void CPlayer::Draw(void)
{
	//影表示切替
	CShadow::DispSwitch(m_nShadowID, m_bDisp);

	//描画しない
	if (!m_bDisp) return;

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	/*計算用マトリックス*/	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//サイズ設定
	D3DXMatrixScaling(&m_mtxWorld, m_fScal, m_fScal, m_fScal);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//人間状態のとき
	if (m_nTransformModelID == ERROR_ID)
	{
		//保持しているモデルの数描画する
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->Draw();
		}
	}
	//人間以外に変身中
	else
	{
		//モデル取得
		CObjectX::XModel *pModel = CObjectX::GetXModel(m_nTransformModelID);

		//Ⅹモデルが取得できなかったら
		if (pModel == NULL)
		{
			//描画せずに引き返す
			return;
		}

		/* レンダラーのポインタ */ CRenderer *pRender = CManager::GetRenderer();
		/* デバイスへのポインタ */ LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
		/*現在のマテリアル保存用*/ D3DMATERIAL9 matDef;
		/* マテリアルのポインタ */ D3DXMATERIAL *pMat;

		//αテストを有効にする
		pRender->SetAlphaTest(true, 0);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL *)pModel->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(pModel->pTextureID[nCntMat]));

			//モデル(パーツ)の描画
			pModel->pMesh->DrawSubset(nCntMat);
		}

		//αテストを無効にする
		pRender->SetAlphaTest(false, 0);

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================
//変身したⅩモデルの個性別の更新処理
//=======================================
void CPlayer::TransformUpdateEntrance(void)
{
	//人間状態なら更新無し
	if (m_nTransformModelID == ERROR_ID || !m_bControll) return;

	//個性を取得
	switch (CObjectX::GetXModelType(m_nTransformModelID))
	{
		case CObjectX::XMODELTYPE_NIMBLE:	TransformUpdate_Nimble(); break;//個性「俊敏」
		case CObjectX::XMODELTYPE_TOUGH:	TransformUpdate_Tough(); break;//個性「強靭」
		case CObjectX::XMODELTYPE_TAKE:		TransformUpdate_Take();	 break;//個性「収納」
		case CObjectX::XMODELTYPE_BIND:		TransformUpdate_Bind();	 break;//個性「束縛」
		default:break;	//「無個性」は更新処理無し
	}
}

//=======================================
//個性「俊敏」の更新処理
//=======================================
void CPlayer::TransformUpdate_Nimble(void)
{
	//最大移動量にバフをかける
	m_fSpecialMaxSpeed = NIMBLE_SPEED;
}

//=======================================
//個性「強靭」の更新処理
//=======================================
void CPlayer::TransformUpdate_Tough(void)
{
	//最大移動量にデバフをかける
	m_fSpecialMaxSpeed = TOUGH_SPEED;
}

//=======================================
//個性「収納」の更新処理
//=======================================
void CPlayer::TransformUpdate_Take(void)
{
	//総数を参照
	int& rTmpNum = m_nNumTakeObjX;

	//Ⅹオブジェクトを収納
	if (rTmpNum < NUM_TAKE_OBJECTX && CManager::IsInputTrigger(DIK_G, CInputGamePad::L_SHOULDER))
	{
		//収納する
		m_pObjX[rTmpNum] = CObjectX::TakeXModel(m_pos, m_rot.y, TRANSFORM_ANGLE, COLLISION_LENGTH);

		//収納できた
		if (m_pObjX[rTmpNum] != NULL)
		{
			m_pObjX[rTmpNum]->DispSwitch(false);//消す
			rTmpNum++;							//収納した数増加
		}
	}

	//Ⅹオブジェクトを取り出し
	else if (CManager::IsInputTrigger(DIK_H, CInputGamePad::R_SHOULDER) && rTmpNum > 0 && m_pObjX[rTmpNum - 1] != NULL)
	{
		//収納している数減少
		rTmpNum--;

		//配置する場所までの長さ
		const int XModelID = m_pObjX[rTmpNum]->GetMyXModelID();
		const float fMargin = m_pObjX[rTmpNum]->GetXModelMargin(XModelID) + PUT_OBJX_LENGTH;
		
		//Ⅹオブジェクトを取り出す位置
		const D3DXVECTOR3 PutPos = D3DXVECTOR3(
			m_pos.x - sinf(m_rot.y) * fMargin,
			m_pos.y - 0.0f,
			m_pos.z - cosf(m_rot.y) * fMargin);
		
		m_pObjX[rTmpNum]->SetTransrate(PutPos, VEC3_INIT, m_rot);//位置・向きを設定
		m_pObjX[rTmpNum]->DispSwitch(true);	//出現
		m_pObjX[rTmpNum] = NULL;			//ポイント削除
	}
}

//=======================================
//個性「束縛」の更新処理
//=======================================
void CPlayer::TransformUpdate_Bind(void)
{

}

//=======================================
//ステートの更新処理窓口
//=======================================
void CPlayer::StateUpdateEntrance(void)
{
	//状態ごとに更新処理を分岐
	switch (m_State)
	{
		case CPlayer::STATE_DAMAGE:	StateUpdate_Damage();	break;//ダメージステートの更新処理
		case CPlayer::STATE_DEATH:	StateUpdate_Death();	break;//死亡ステートの更新処理
	}
}

//=======================================
//ダメージステートの更新処理
//=======================================
void CPlayer::StateUpdate_Damage(void)
{
	//インターバル減少
	m_nDamageInterval--;

	//インターバル終了
	if (m_nDamageInterval <= 0)
	{
		m_nDamageInterval = 0;	//インターバルクリア
		m_State = STATE_NORMAL;	//通常状態へ
		m_bDisp = true;			//描画する
	}
	//描画ON/OFF切り替え
	else if (m_nDamageInterval % SWITCH_DISP == 0)
	{
		m_bDisp ^= true;
	}
}

//=======================================
//死亡ステートの更新処理
//=======================================
void CPlayer::StateUpdate_Death(void)
{
	//人間に戻す
	m_nTransformModelID = ERROR_ID;

	//インターバル減少
	m_nDeathInterval--;

	//インターバル終了			フェード処理
	if (m_nDeathInterval <= 0)	CFade::Set(CScene::RESULT, CFade::FADE_WIPE_OUT);

	//描画しない
	else if (m_nDeathInterval <= 30)	m_bDisp = false;

	//描画ON/OFF切り替え
	else if (m_nDeathInterval % SWITCH_DISP == 0) m_bDisp ^= true;
}

//=======================================
//体力ポリゴン2D生成
//=======================================
void CPlayer::SetLifePoly2D(void)
{
	//体力描画用の背景が使われていない
	if (m_pLifeBG == NULL)
	{
		m_pLifeBG = CPoly2D::Create(NULL);

		if (m_pLifeBG == NULL) return;

		//情報設定
		m_pLifeBG->SetInfo(LIFE_BG_POS, LIFE_BG_WIDTH, LIFE_BG_HEIGHT);
		m_pLifeBG->SetVertexInfo(LIFE_BG_COLOR);
	}

	//体力のロゴが使われていない
	if (m_pLifeLogo == NULL)
	{
		m_pLifeLogo = CPoly2D::Create(LOGO_TEXTURE_PATH);

		if (m_pLifeLogo == NULL) return;

		//情報設定
		m_pLifeLogo->SetInfo(LIFE_LOGO_POS, LIFE_LOGO_WIDTH, LIFE_LOGO_HEIGHT);
	}

	//体力保存
	int nLifeTmp = m_nLife;

	//桁数
	int nNumDejit = Dejit(m_nLife);

	for (int nCntLife = 0; nCntLife < NUM_LIFE_DEJIT; nCntLife++)
	{
		//体力用ポリゴンが生成されていない
		if (m_pLife[nCntLife] == NULL)
		{//生成
			m_pLife[nCntLife] = CPoly2D::Create(LIFE_TEXTURE_PATH);
		}

		//体力用ポリゴンが生成されている
		if (m_pLife[nCntLife] != NULL)
		{
			//桁数より多ければ非表示にする
			if (nCntLife >= nNumDejit)
			{
				m_pLife[nCntLife]->DispSwitch(false);
				continue;
			}

			//１の位を抽出する
			const int nMod = nLifeTmp % 10;
			nLifeTmp /= 10;

			//座標設定（Ⅹ座標だけ桁ごとに左へずらす
			D3DXVECTOR3 pos = LIFE_POS;
			pos.x -= ((float)nCntLife * LIFE_WIDTH + (float)nCntLife * LIFE_MARGIN);

			//テクスチャ座標を設定
			D3DXVECTOR2 TexPos = VEC2_INIT;
			TexPos.x = nMod * LIFE_TEX_WIDTH;

			//ポリゴン情報設定
			m_pLife[nCntLife]->SetInfo(pos, LIFE_WIDTH, LIFE_HEIGHT, TexPos, LIFE_TEX_WIDTH, LIFE_TEX_HEIGHT);
			m_pLife[nCntLife]->DispSwitch(true);
			m_pLife[nCntLife]->SetVertexInfo(D3DXCOLOR(1.0f, (float)m_nLife / (float)MAX_LIFE, 0.0f, 1.0f));
		}

		m_pLife[nCntLife]->DispSwitch(false);
	}

	m_pLifeBG->DispSwitch(false);
	m_pLifeLogo->DispSwitch(false);
}

//=======================================
//ダメージ処理
//=======================================
void CPlayer::GiveDamege(int nDamage)
{
	//死体蹴りをさせない
	if (m_State == STATE_DEATH) return;

	//ダメージを与える
	m_nLife -= nDamage;

	//死んだ
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		m_State = STATE_DEATH;
		m_nDeathInterval = DEATH_INTERVAL;
		CResult::SetState(CResult::STATE_OVER);
	}
	//まだ生きてる
	else
	{
		m_State = STATE_DAMAGE;
		m_nDamageInterval = DAMAGE_INTERVAL;
	}
}

//=======================================
//当たり判定まとめ
//=======================================
void CPlayer::CollisionWhole(void)
{
	//チュートリアルの特殊状態なら当たり判定無し
	if (CManager::GetMode() == CScene::TUTORIAL && !m_bControll) return;

	for (int nCntType = 0; nCntType <= CObjectX::TYPE_Z; nCntType++)
	{
		//順番に移動させる
		switch (nCntType)
		{
			case CObjectX::TYPE_X:	m_pos.x += m_move.x; break;
			case CObjectX::TYPE_Y:	m_pos.y += m_move.y; break;
			case CObjectX::TYPE_Z:	m_pos.z += m_move.z; break;
		}

		if (m_nTransformModelID == -1)	HumanCollisionWhole(nCntType);		//人間状態のとき
		else							TransformCollisionWhole(nCntType);	//変身している時
	}

	//宝石に触れているか
	CJewel::Collision(m_pos);
}

//=======================================
//人間状態のときの当たり判定
//=======================================
void CPlayer::HumanCollisionWhole(int nType)
{
	/*Ⅹモデルに触れてるか*/ CObjectX::CollisionXModel(&m_pos, m_posOld, &m_move, (CObjectX::TYPE)nType, COLLISION_LENGTH, COLLISION_MARGIN);
	/*  壁に触れているか  */ CWall::Collision(&m_pos, m_posOld, &m_move, COLLISION_MARGIN, 90.0f);
}

//=======================================
//変身状態のときの当たり判定
//=======================================
void CPlayer::TransformCollisionWhole(int nType)
{
	//当たり判定の範囲を取得
	const float fMargin = CObjectX::GetXModelMargin(m_nTransformModelID);
	const float fHeight = CObjectX::GetPosition(m_nTransformModelID, CObjectX::TYPE_Y, (CObjectX::TYPE)CObjectX::TYPE_MAX);

	/*Xモデルに触れてるか*/	CObjectX::CollisionXModel(&m_pos, m_posOld, &m_move, (CObjectX::TYPE)nType, fMargin, m_nTransformModelID);
	/*	壁に触れているか */	CWall::Collision(&m_pos, m_posOld, &m_move, fMargin, fHeight);
}

//=======================================
//弾の発射位置
//=======================================
void CPlayer::SetBullet(void)
{
	//攻撃が発生したら（人間状態のとき限定
	if (CManager::IsInputPress(DIK_B, CInputGamePad::B) && m_nTransformModelID == ERROR_ID && m_bControll)
	{
		m_rot.y = -CManager::GetCamera()->GetInverseRotate();

		if (m_pMotion->IsAction())
		{
			//弾の発射設定
			CBullet *pBullet = CBullet::Create(m_pos + BULLET_FIRE_POS, m_rot);
		}
	}
}

//=======================================
//変身処理
//=======================================
void CPlayer::Transform(void)
{
	//変身番号を格納
	const int nTempID = CObjectX::TransformXModel(m_pos, m_rot.y, TRANSFORM_ANGLE, COLLISION_LENGTH, m_nNextModelID);

	//変身
	if (CManager::IsInputTrigger(DIK_T, CInputGamePad::Y) && m_bControll && m_bTransform)
	{
		//次に変身するXモデルの番号を格納
		m_nNextModelID = nTempID;

		//モデル番号が変わっていなかったら以降の処理をせず終了
		if (m_nTransformModelID == m_nNextModelID) return;

		//変身不可にし、縮小していく
		m_bTransform = false;
		m_bScaling = false;
	}

	//変身中
	if (!m_bTransform)
	{
		//拡大中
		if (m_bScaling)
		{
			m_fScal += SCALING_SPEED;

			//拡大終了
			if (m_fScal >= 1.0f)
			{
				//通常サイズに戻し、変身可能に
				m_fScal = 1.0f;
				m_bScaling = false;
				m_bTransform = true;
			}
		}

		//縮小中
		else
		{
			m_fScal -= SCALING_SPEED;

			//縮小終了
			if (m_fScal <= 0.0f)
			{
				//変身による移動量の変化をクリア
				m_fSpecialMaxSpeed = 1.0f;

				//サイズを無に帰し、拡大中に遷移
				m_fScal = 0.0f;
				m_bScaling = true;

				//変身するモデル番号を設定して体力変化
				m_nTransformModelID = m_nNextModelID;
				TransformSetLife();
			}
		}
	}
}

//=======================================
//変身した時の体力変化
//=======================================
void CPlayer::TransformSetLife(void)
{
	//設定する体力初期化
	int nLife = MAX_LIFE;

	//オブジェクトに変身中ならそのオブジェクトの体力を取得
	if(m_nTransformModelID != ERROR_ID) nLife = CObjectX::GetXModelLife(m_nTransformModelID);

	//今の体力が最大体力と同じ or 設定する体力より今の体力が多い
	if (m_nLife == m_nLifeMax || m_nLife > nLife)
	{
		//変身したモデルの体力にする
		m_nLife = nLife;
	}

	//最大体力設定
	m_nLifeMax = nLife;
}

//=======================================
//パーツ情報読み取り
//=======================================
void CPlayer::LoadPartsInfo(void)
{
	//親番号の配列を格納
	int aParentNumber[MAX_PARTS];

	//ファイルインスタンス生成
	CFile *pFile = new CFile;

	//パーツ・モーションの情報を読み込む
	pFile->LoadMotion(MODEL_PARTS_INFO, &m_apModel[0], m_pMotion, &m_nNumModel, &aParentNumber[0]);

	//モデルパーツの親モデルを設定する
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		//親番号を参照する
		int &ParentID = aParentNumber[nCntModel];

		//親がいない場合
		if (ParentID == -1)
		{
			//NULLを代入する
			m_apModel[nCntModel]->SetParent(NULL);
		}
		else
		{
			//親のモデルを代入する
			m_apModel[nCntModel]->SetParent(m_apModel[ParentID]);
		}
	}

	//モーションにモデル情報を共有する
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	//メモリ開放
	delete pFile;
	pFile = NULL;
}

//=======================================
//位置設定
//=======================================
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	//発生位置設定
	m_pos = m_posOld = pos;
}

//=======================================
//ゴール処理
//=======================================
void CPlayer::Goal(void)
{
	//ゴール判定
	CJewel::Goal(m_pos);
}

//=======================================
//チュートリアル限定の操作ON/OFF
//=======================================
void CPlayer::IsTutorialControll(bool bControll)
{
	//チュートリアル中なら、操作ON/OFFを切り替える
	if (CManager::GetMode() == CScene::TUTORIAL) m_bControll = bControll;

	//チュートリアル以外なら、操作は常にON
	else m_bControll = true;
}

const int	CPlayer::FOOTPRINT_SPAWN_RANGE = 10;	//足跡を発生する間隔
const int	CPlayer::FOOTPRINT_SPAWN_MIN = 3;		//足跡発生の最低インターバル
const int	CPlayer::FOOTPRINT_RANGE = 200;			//足跡を飛び散らせる範囲
const float	CPlayer::FOOTPRINT_SIZE = 2.0f;			//足跡サイズ
const float	CPlayer::FOOTPRINT_GRAVITY = -2.0f;		//足跡を落下させる重力量
const float	CPlayer::FOOTPRINT_GRAVITY_CURRECT = 0.1f;//重力係数
const float	CPlayer::FOOTPRINT_JUMP = 2.0f;			//発生した際の浮遊量
const float	CPlayer::FOOTPRINT_SPEED = 1.0f;		//飛び散る速度
const int	CPlayer::FOOTPRINT_LIFE = 20;			//寿命

//=======================================
//歩いた軌跡を生成
//=======================================
void CPlayer::FootPrint(void)
{
	//移動時間増加
	m_nMoveCounter = (m_nMoveCounter + 1) % m_nNextFootPrint;

	if (m_nMoveCounter == 0)
	{
		//次に足跡が発生するタイミングを決める
		m_nNextFootPrint = rand() % FOOTPRINT_SPAWN_RANGE + FOOTPRINT_SPAWN_MIN;

		//足跡生成
		CPolyBill::CreateFootPrint(m_pos, m_rot.y, FOOTPRINT_RANGE,
								   FOOTPRINT_SIZE, GetColor(COL_BLACK), FOOTPRINT_GRAVITY, FOOTPRINT_GRAVITY_CURRECT,
								   FOOTPRINT_JUMP, FOOTPRINT_SPEED, FOOTPRINT_LIFE);
	}
}

//=======================================
//影設定
//=======================================
void CPlayer::Shadow(void)
{
	//位置設定
	CShadow::SetPos(m_nShadowID, m_pos);

	//影のサイズ設定
	float fSize = SHADOW_SIZE;

	//Ⅹモデルに変身中
	if (m_nTransformModelID != ERROR_ID)
	{
		//Ⅹモデルの当たり判定範囲を取得
		fSize = CObjectX::GetXModelMargin(m_nTransformModelID);
	}

	//影のサイズを設定（変身中の場合、拡縮サイズも設定する
	CShadow::SetSize(m_nShadowID, fSize * m_fScal);
}