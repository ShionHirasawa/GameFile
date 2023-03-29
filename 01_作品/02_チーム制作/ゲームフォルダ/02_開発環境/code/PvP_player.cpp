//==========================================
//
//プレイヤープログラム[player.cpp]
//Author:石原颯馬  平澤詩苑
//
//==========================================
#include "main.h"
#include "pvp_game.h"
#include "PvP_player.h"
#include "model.h"
#include "input.h"
#include "file.h"
#include "score.h"
#include "debugproc.h"
#include "camera.h"
#include <assert.h>
#include "color.h"
#include "gauge.h"
#include "sound.h"
#include "charge_cylinder.h"
#include "charge_effect.h"
#include "attack_effect.h"
#include "tremor_effect.h"
#include "item.h"
#include "meshfield.h"
#include "meshdome.h"
#include "bonus.h"
#include "particle.h"
#include "comai.h"
#include "conversioninput.h"
#include "collision.h"
#include "fence.h"
#include "TypeFrame.h"

//マクロ
#define PLAYER_HIPDROP_WAIT		(15)		//ヒップドロップの「開始・着地」硬直時間
#define PLAYER_BLOWING_POWER	(5.0f)		//ヒップドロップされた方の移動量
#define PLAYER_HIPDROP_POWER	(-15.0f)	//ヒップドロップするときの移動量
#define PLAYER_ROTATE_SPEED		(0.02f * D3DX_PI)	//回転速度
#define PLAYER_HIPSPIN_SPEED	(-0.5f)		//ヒップドロップスピンの回転値
#define PLAYER_HIPSPIN_LAP		(2.0f * -D3DX_PI)	//ヒップドロップスピンしたときの１周判定をとる値
#define PLAYER_HIPDROP_ACTIONRIGOR	(45)		//ヒップドロップのアクション硬直の時間
#define PLAYER_DASH_ACTIONRIGOR	(20)		//ダッシュのアクション硬直のベースの時間
#define PLAYER_JUMP_ACTIONRIGOR	(30)		//ジャンプのアクション硬直時間
#define PLAYER_D_ACTRIGOR_CALC(x)	(powf((x - 0.75f), 2) + 1)

//移動量関係
#define PLAYER_MOVE_SPEED		(20.0f)		//プレイヤー移動速度
#define PLAYER_JUMP_SPEED		(7.7f)		//プレイヤージャンプ速度
#define PLAYER_WEIGHT			(50)		//質量
#define PLAYER_POWER_ADD		(0.025f)	//移動の強さの増加値
#define DUMP_COEF				(0.04f)		//減衰係数
#define DECIMAL_PLACE			(1)			//小数点第何位まで移動していることにするか
#define HIPDROP_RADIUS			(60.0f)		//ヒップドロップ判定範囲
#define REBOUND_RATIO			(0.15f)		//当たった後の自分の分の移動量割合

//アイテム関係
#define ACCELERATION_CONS		(0.5f)		//加速定数（1.0で全部渡す）
#define ACCELERATION_ITEMADD	(0.35f)		//アイテム所持中の強化量
#define DEFANCE_CONS			(0.0f)		//防御定数（1.0で完全防御）
#define DEFANCE_ITEMADD			(0.4f)		//アイテム所持中の強化量
#define INVINCIBLE_ATK			(1.0f)		//無敵状態の自分の変換割合
#define INVINCIBLE_DEF			(0.0f)		//無敵状態の相手の変換割合

//ゴースト化状態
#define GOAST_ALPHA				(0.25f)		//不透明度
#define GOAST_FLASHSTART		(240)		//点滅開始する残り時間
#define GOAST_FLASHPULSE		(20)		//点滅の切り替え時間

#define PLAYER_SIZE_WIDTH		(12.5f)
#define PLAYER_SIZE_HEIGHT		(15.0f)
#define PLAYER_SIZE_DEPTH		(20.0f)

//向き
#define ROT_P3	(-0.75f * D3DX_PI)	//左上
#define ROT_P4	(0.75f * D3DX_PI)	//右上
#define ROT_P1	(-0.25f * D3DX_PI)	//左下
#define ROT_P2	(0.25f * D3DX_PI)	//右下

//プロト
void ControllPlayer(int nPlayerNum);

void MovePlayer(int nPadNum);
void RotatePlayer(int nPadNum);				//MovePlayer のrot.y の計算式だけを残しています

void ChargePlayer(int nChargePlayer);		//チャージ処理
void DashPlayer(int nDashPlayer);			//ダッシュ処理
void JumpPlayer(int nJumpPlayer);			//ジャンプ処理
void HipDropPlayer(int nHipDropPlayer);		//ヒップドロップ処理

void DownPlayer(int nDownPlayerNum);		//ダウンしたプレイヤーの処理
void RespawnPlayer(int nRespawnPlayer);		//リスポーン処理

void DecrementItemTime(int nPlayerNum);		//アイテムカウントをすべて減らす
void ItemStateParticle(int nPlayerNum);		//アイテムパーティクル表示処理

void SetMotion(int nPlayerNum, MOTIONTYPE type);	//モーション設定処理
void UpdateMotion(int nPlayerNum);					//モーション更新処理

//グローバル変数
Player g_aPlayerPvP[MAX_USE_GAMEPAD];
int g_nIdxShadow = -1;
bool g_bDebugMove = false;	//[デバッグ用]自由に動き回れるかどうか
Player *g_pNotMove[MAX_USE_GAMEPAD];	//移動していないプレイヤー検知用ポインタ

//初期位置向き
const D3DXVECTOR3 c_aPosRot[MAX_USE_GAMEPAD][2] =
{
	{ D3DXVECTOR3(-200.0f,0.0f,200.0f) ,D3DXVECTOR3(0.0f,ROT_P1,0.0f) },
	{ D3DXVECTOR3(200.0f,0.0f,200.0f) ,D3DXVECTOR3(0.0f,ROT_P2,0.0f) },
	{ D3DXVECTOR3(-200.0f,0.0f,-200.0f) ,D3DXVECTOR3(0.0f,ROT_P3,0.0f) },
	{ D3DXVECTOR3(200.0f,0.0f,-200.0f) ,D3DXVECTOR3(0.0f,ROT_P4,0.0f) },
};

//[デバッグ用]AI挙動させるプレイヤー指定（コントローラーが刺さっていればそれを優先）
bool g_aAIMove_PvP[MAX_USE_GAMEPAD] = { true,true,true,true };

//========================
//初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//変数初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//変数初期化
		g_aPlayerPvP[nCntPlayer].pos = c_aPosRot[nCntPlayer][0];
		g_aPlayerPvP[nCntPlayer].posOld = g_aPlayerPvP[nCntPlayer].pos;
		g_aPlayerPvP[nCntPlayer].move = ZERO_SET;
		g_aPlayerPvP[nCntPlayer].moveV0 = ZERO_SET;
		g_aPlayerPvP[nCntPlayer].rot = c_aPosRot[nCntPlayer][1];
		g_aPlayerPvP[nCntPlayer].moveGauge = 0;
		g_aPlayerPvP[nCntPlayer].jumpTime = 0;
		g_aPlayerPvP[nCntPlayer].bJump = false;
		g_aPlayerPvP[nCntPlayer].bHipDrop = false;
		g_aPlayerPvP[nCntPlayer].bHipDropSpin = false;
		g_aPlayerPvP[nCntPlayer].nHipDropWait = 0;
		g_aPlayerPvP[nCntPlayer].bNotMove = true;
		g_aPlayerPvP[nCntPlayer].nRespawnPosNum = nCntPlayer;
		g_aPlayerPvP[nCntPlayer].bAnimed = false;

		g_aPlayerPvP[nCntPlayer].faceCollider[0] = D3DXVECTOR3(PLAYER_SIZE_WIDTH, 0.0f, PLAYER_SIZE_WIDTH);
		g_aPlayerPvP[nCntPlayer].faceCollider[1] = D3DXVECTOR3(-PLAYER_SIZE_WIDTH, 0.0f, PLAYER_SIZE_WIDTH);

		g_aPlayerPvP[nCntPlayer].animal = ANIMAL_WILDBOAR;
		g_aPlayerPvP[nCntPlayer].nScore = 0;
		g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer = -1;
		g_aPlayerPvP[nCntPlayer].nLastHitPlayer = -1;
		g_aPlayerPvP[nCntPlayer].stat = PLAYERSTAT_WAIT;
		g_aPlayerPvP[nCntPlayer].nPlayerNum = nCntPlayer;

		g_aPlayerPvP[nCntPlayer].nATKItemTime = 0;
		g_aPlayerPvP[nCntPlayer].nDEFItemTime = 0;
		g_aPlayerPvP[nCntPlayer].nGhostItemTime = 0;
		g_aPlayerPvP[nCntPlayer].nInvincibleTime = 0;

		g_aPlayerPvP[nCntPlayer].pAI = NULL;

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			g_aPlayerPvP[nCntPlayer].animalInst[nCntParts] = {};
		}

		g_pNotMove[nCntPlayer] = &g_aPlayerPvP[nCntPlayer];

		//AI設定
		if (g_aAIMove_PvP[nCntPlayer] == true)
		{//AIは脳みそポインタもらって使用していることにする
			g_aPlayerPvP[nCntPlayer].pAI = GetAI(GetAIDiff());
			g_aPlayerPvP[nCntPlayer].bUsePlayer = true;
		}
	}

#ifdef _DEBUG
	//[デバッグ]コントローラーが接続されていなければ1Pのみ有効化する
	if (GetUseControllerNum_PvP() == 0)
	{
		g_aPlayerPvP[0].bUsePlayer = true;
	}
#endif // _DEBUG
}

//========================
//終了処理
//========================
void UninitPlayer(void)
{
	
}

//========================
//更新処理
//========================
void UpdatePlayer(void)
{
	//プレイヤー人数分繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//前回の情報初期化
		g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer = -1;

		//現在の位置を前回の位置にする
		g_aPlayerPvP[nCntPlayer].posOld = g_aPlayerPvP[nCntPlayer].pos;

		//ジャンプ時間を増やす
		g_aPlayerPvP[nCntPlayer].jumpTime++;

		//パーティクル表示
		ItemStateParticle(nCntPlayer);

		//アイテム持続時間減らす
		DecrementItemTime(nCntPlayer);

		if (g_aPlayerPvP[nCntPlayer].bUsePlayer == true)
		{//使用時のみ行う
			//アクション硬直時間減らす
			g_aPlayerPvP[nCntPlayer].nActionRigor--;

			if (g_aPlayerPvP[nCntPlayer].nActionRigor <= 0)
			{
				//AI処理
				if (g_aPlayerPvP[nCntPlayer].pAI != NULL)
				{//AI
					//AIがコントローラー操作
					SelectAIMove(&g_aPlayerPvP[nCntPlayer]);
				}
				else
				{//プレイヤー
					//（下の部分ゲームパッドが一つも使用されていなければ無視）
					if (GetUseControllerNum_PvP() != 0)
					{
						g_aPlayerPvP[nCntPlayer].bUsePlayer = GetUseController_PvP(nCntPlayer);
					}
				}

				//操作処理
				ControllPlayer(nCntPlayer);

				//当たり判定類
				if (g_aPlayerPvP[nCntPlayer].nGhostItemTime <= 0)
				{//ゴースト化状態でなければ
					if (CollisionPP(&g_aPlayerPvP[nCntPlayer], PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT, PLAYER_SIZE_DEPTH) == true)
					{
						SetAttackEffect(g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer].pos, g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer);
					}

					CollisionIP(nCntPlayer);	//アイテムとプレイヤーの当たり判定
					CollisionObject_Tree(&g_aPlayerPvP[nCntPlayer]);	//木のオブジェクトと、プレイヤーの当たり判定

					if (g_aPlayerPvP[nCntPlayer].stat == PLAYERSTAT_HIPDROP)
					{//ヒップドロップ中なら
						CollisionHipDropPP(&g_aPlayerPvP[nCntPlayer], PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT, PLAYER_SIZE_DEPTH, HIPDROP_RADIUS, PLAYER_BLOWING_POWER);
					}
				}
			}

			//回転（チャージ中でも回転可にした）
			RotatePlayer(nCntPlayer);
		}

		//使用されているかにかかわらず行う
		//ジャンプ量設定
		if (g_aPlayerPvP[nCntPlayer].bHipDrop == true)
		{
			g_aPlayerPvP[nCntPlayer].move.y = g_aPlayerPvP[nCntPlayer].moveV0.y;
		}
		else
		{
			g_aPlayerPvP[nCntPlayer].move.y = g_aPlayerPvP[nCntPlayer].moveV0.y - (ACCELERATION_GRAVITY * g_aPlayerPvP[nCntPlayer].jumpTime / MAX_FPS);
		}

		//移動後がy<0であり、現在の位置が、フィールド以上の高さにいるなら移動量消す
		if (g_aPlayerPvP[nCntPlayer].pos.y + g_aPlayerPvP[nCntPlayer].move.y < 0.0f && g_aPlayerPvP[nCntPlayer].pos.y >= 0.0f)
		{
			//原点位置からのプレイヤーの距離を計算
			float fLength = sqrtf(powf((g_aPlayerPvP[nCntPlayer].pos.x + g_aPlayerPvP[nCntPlayer].move.x), 2) +
				powf((g_aPlayerPvP[nCntPlayer].pos.z + g_aPlayerPvP[nCntPlayer].move.z), 2));

			//原点位置からの距離が、フィールドの半径以下　　なら、フィールドに乗っている
			if (fLength <= GetMeshField()->fRadius)
			{
				if (g_aPlayerPvP[nCntPlayer].bHipDrop == true)
				{//ヒップドロップしてたならエフェクト出す
					SetTremorEffect(g_aPlayerPvP[nCntPlayer].pos);
					g_aPlayerPvP[nCntPlayer].bHipDrop = false;    //ヒップドロップしてない
					g_aPlayerPvP[nCntPlayer].nActionRigor = PLAYER_HIPDROP_ACTIONRIGOR;
					g_aPlayerPvP[nCntPlayer].stat = PLAYERSTAT_WAIT;
					PlaySoundSE(SOUND_LABEL_SE_HIPDROP, nCntPlayer);
				}
				else if (g_aPlayerPvP[nCntPlayer].stat == PLAYERSTAT_JUMP)
				{
					g_aPlayerPvP[nCntPlayer].nActionRigor = PLAYER_JUMP_ACTIONRIGOR;
					g_aPlayerPvP[nCntPlayer].stat = PLAYERSTAT_LAND;
					g_aPlayerPvP[nCntPlayer].bAnimed = false;
				}
				else if(g_aPlayerPvP[nCntPlayer].stat != PLAYERSTAT_DASH && g_aPlayerPvP[nCntPlayer].stat != PLAYERSTAT_CHARGE && g_aPlayerPvP[nCntPlayer].bAnimed == true)
				{
					g_aPlayerPvP[nCntPlayer].stat = PLAYERSTAT_WAIT;
					g_aPlayerPvP[nCntPlayer].bAnimed = false;
				}
				g_aPlayerPvP[nCntPlayer].bJump = false;
				g_aPlayerPvP[nCntPlayer].moveV0.y = 0.0f;
				g_aPlayerPvP[nCntPlayer].move.y = 0.0f;
				g_aPlayerPvP[nCntPlayer].jumpTime = 0;
				g_aPlayerPvP[nCntPlayer].pos.y = 0.0f;
				g_aPlayerPvP[nCntPlayer].nHipDropWait = 0;
			}
			else
			{
				g_aPlayerPvP[nCntPlayer].stat = PLAYERSTAT_FALL;
			}
		}

		if (D3DXVec3Length(&g_aPlayerPvP[nCntPlayer].pos) >= MESHDOME_RADIUS)
		{//吹っ飛んだ
			DownPlayer(nCntPlayer);
		}

		//ボーナスの当たり判定
		CollisionBonus(g_aPlayerPvP[nCntPlayer].pos, nCntPlayer);

		//アニメーション設定
		switch (g_aPlayerPvP[nCntPlayer].stat)
		{
		case PLAYERSTAT_WAIT:	//待機・落下・ヒップドロップは一緒
			if (g_aPlayerPvP[nCntPlayer].motion.motionType != MOTIONTYPE_NEUTRAL)
			{
				SetMotion(nCntPlayer, MOTIONTYPE_NEUTRAL);
				g_aPlayerPvP[nCntPlayer].bAnimed = false;
			}
			break;
		case PLAYERSTAT_FALL:
			if (g_aPlayerPvP[nCntPlayer].motion.motionType != MOTIONTYPE_NEUTRAL)
			{
				SetMotion(nCntPlayer, MOTIONTYPE_NEUTRAL);
				g_aPlayerPvP[nCntPlayer].bAnimed = false;
			}
			break;
		case PLAYERSTAT_HIPDROP:
			if (g_aPlayerPvP[nCntPlayer].motion.motionType != MOTIONTYPE_NEUTRAL)
			{
				SetMotion(nCntPlayer, MOTIONTYPE_NEUTRAL);
				g_aPlayerPvP[nCntPlayer].bAnimed = false;
			}
			break;
		case PLAYERSTAT_CHARGE:
			if (g_aPlayerPvP[nCntPlayer].motion.motionType != MOTIONTYPE_CHARGE && g_aPlayerPvP[nCntPlayer].bAnimed == false)
			{
				SetMotion(nCntPlayer, MOTIONTYPE_CHARGE);
			}
			break;
		case PLAYERSTAT_DASH:
			if (g_aPlayerPvP[nCntPlayer].motion.motionType != MOTIONTYPE_DASH && g_aPlayerPvP[nCntPlayer].bAnimed == false)
			{
				SetMotion(nCntPlayer, MOTIONTYPE_DASH);
			}
			break;
		case PLAYERSTAT_JUMP:
			if (g_aPlayerPvP[nCntPlayer].motion.motionType != MOTIONTYPE_JUMP && g_aPlayerPvP[nCntPlayer].bAnimed == false)
			{
				SetMotion(nCntPlayer, MOTIONTYPE_JUMP);
			}
			break;
		case PLAYERSTAT_LAND:
			if (g_aPlayerPvP[nCntPlayer].motion.motionType != MOTIONTYPE_LAND && g_aPlayerPvP[nCntPlayer].bAnimed == false)
			{
				SetMotion(nCntPlayer, MOTIONTYPE_LAND);
			}
			break;
		}

		UpdateMotion(nCntPlayer);
	}

	//移動量と衝突判定をもとに移動する
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		//移動検知
		if (g_aPlayerPvP[nCntPlayer].bNotMove == true)
		{
			if (fabsf(g_aPlayerPvP[nCntPlayer].move.x) > 0.0f || fabsf(g_aPlayerPvP[nCntPlayer].move.z) > 0.0f)
			{
				g_aPlayerPvP[nCntPlayer].bNotMove = false;
			}
		}

		if (g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer == -1)
		{//ぶつかってないまたは移動量交換済み		
			//ヒップドロップの硬直中ではない
			if (g_aPlayerPvP[nCntPlayer].nHipDropWait <= 0)
			{
				//フェンスに衝突していなければ
				if (CollisionFence(&g_aPlayerPvP[nCntPlayer], FENCE_WIDTH, PLAYER_SIZE_HEIGHT, FENCE_DEPTH, FENCE_SCALE) == false)
				{
					//普通に移動
					g_aPlayerPvP[nCntPlayer].pos += g_aPlayerPvP[nCntPlayer].move;
				}//衝突していれば関数内で移動処理を行う
			}
		}
		else
		{//ぶつかった
			//移動量交換
			D3DXVECTOR3 moveTmp1 = g_aPlayerPvP[nCntPlayer].move;
			D3DXVECTOR3 moveTmp2 = g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer].move;

			//割合設定
			float fPowerConvertion1, fPowerConvertion2;
			if (g_aPlayerPvP[nCntPlayer].nInvincibleTime > 0)
			{//我無敵也(自分0%,相手100%)
				fPowerConvertion1 = INVINCIBLE_DEF;
				fPowerConvertion2 = INVINCIBLE_ATK;
			}
			else if (g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer].nInvincibleTime > 0)
			{//相手無敵也(自分100%,相手0%)
				fPowerConvertion1 = INVINCIBLE_ATK;
				fPowerConvertion2 = INVINCIBLE_DEF;
			}
			else
			{//どっちもむてきじゃないよ
				fPowerConvertion1 = ACCELERATION_CONS + (g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMADD : 0.0f) -
					DEFANCE_CONS + (g_aPlayerPvP[nCntPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);

				fPowerConvertion2 = ACCELERATION_CONS + (g_aPlayerPvP[nCntPlayer].nATKItemTime > 0 ? ACCELERATION_ITEMADD : 0.0f) -
					DEFANCE_CONS + (g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer].nDEFItemTime > 0 ? DEFANCE_ITEMADD : 0.0f);
			}

			g_aPlayerPvP[nCntPlayer].move = (moveTmp2 * fPowerConvertion1) + (moveTmp1 * (1.0f - fPowerConvertion2) * REBOUND_RATIO);
			g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer].move = (moveTmp1 * fPowerConvertion2) + (moveTmp2 * (1.0f - fPowerConvertion1) * REBOUND_RATIO);

			//移動量交換済み扱いにする
			g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer = -1;
			g_aPlayerPvP[g_aPlayerPvP[nCntPlayer].nFrameAtkPlayer].nFrameAtkPlayer = -1;

			//フェンスに衝突していなければ
			if (CollisionFence(&g_aPlayerPvP[nCntPlayer], PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT, PLAYER_SIZE_DEPTH, FENCE_SCALE) == false)
			{
				//普通に移動
				g_aPlayerPvP[nCntPlayer].pos += g_aPlayerPvP[nCntPlayer].move;
			}//衝突していれば関数内で移動処理を行う
		}

		//移動量減衰
		g_aPlayerPvP[nCntPlayer].move.x += (0 - g_aPlayerPvP[nCntPlayer].move.x) * DUMP_COEF;
		g_aPlayerPvP[nCntPlayer].move.z += (0 - g_aPlayerPvP[nCntPlayer].move.z) * DUMP_COEF;
	}
}

//========================
//描画処理
//========================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//プレイヤーの数だけ繰り返す
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{
		if (g_aPlayerPvP[nCntPlayer].bUsePlayer == true)
		{
			Model useAnimal = GetAnimal(g_aPlayerPvP[nCntPlayer].animal);

			//"プレイヤーの"ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_aPlayerPvP[nCntPlayer].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayerPvP[nCntPlayer].rot.y, g_aPlayerPvP[nCntPlayer].rot.x, g_aPlayerPvP[nCntPlayer].rot.z);
			D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].mtxWorld, &g_aPlayerPvP[nCntPlayer].mtxWorld, &mtxRot);

			//位置反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayerPvP[nCntPlayer].pos.x, g_aPlayerPvP[nCntPlayer].pos.y, g_aPlayerPvP[nCntPlayer].pos.z);
			D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].mtxWorld, &g_aPlayerPvP[nCntPlayer].mtxWorld, &mtxTrans);

			//"プレイヤーの"ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerPvP[nCntPlayer].mtxWorld);

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{
				if (useAnimal.aParts[nCntParts].bUse == true)
				{
					D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用
					D3DXMATRIX mtxParent;					//親のマトリ

															//ここ新仕様
															//"モデルの"ワールドマトリックス初期化
					D3DXMatrixIdentity(&g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld);

					//向きを反映
					D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].rot.y, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].rot.x, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].rot.z);
					D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxRotModel);

					//位置反映
					D3DXMatrixTranslation(&mtxTransModel, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].pos.x, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].pos.y, g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].pos.z);
					D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxTransModel);

					//パーツの親マトリ設定
					if (useAnimal.aParts[nCntParts].mIdxModelParent != -1)
					{
						mtxParent = g_aPlayerPvP[nCntPlayer].animalInst[useAnimal.aParts[nCntParts].mIdxModelParent].mtxWorld;
					}
					else
					{
						mtxParent = g_aPlayerPvP[nCntPlayer].mtxWorld;
					}

					//パーツのマトリと親マトリをかけ合わせる
					D3DXMatrixMultiply(&g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxParent);

					//"モデルの"ワールドマトリックス設定
					pDevice->SetTransform(D3DTS_WORLD, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld);

					//マテリアルデータへのポインタ取得
					pMat = (D3DXMATERIAL*)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
					{
						//ゴースト用
						D3DMATERIAL9 matChange = pMat[nCntMat].MatD3D;

						//ゴースト状態なら消す
						if (g_aPlayerPvP[nCntPlayer].nGhostItemTime > 0)
						{
							//アルファテストを有効にする
							pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
							pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
							pDevice->SetRenderState(D3DRS_ALPHAREF, 10);
							matChange.Diffuse.a = 0.0f;
						}
						else if (nCntParts == MAX_PARTS - 1)
						{//ゼッケンの時は色変更
							matChange.Diffuse = c_aColPlayer[nCntPlayer];
						}

						//マテリアル設定
						pDevice->SetMaterial(&matChange);

						//テクスチャ設定
						pDevice->SetTexture(0, useAnimal.aParts[nCntParts].apTexture[nCntMat]);

						//モデル描画
						useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
					}

					//アルファテストを無効にする
					pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
					pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
					pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

					/*------------------------------------------------------------------
					影の描画		Author:平澤詩苑 石原颯馬
					--------------------------------------------------------------------*/
					if (g_aPlayerPvP[nCntPlayer].pos.y >= 0.0f)
					{
						D3DXMATRIX	mtxShadow;		//シャドウマトリックス
						D3DLIGHT9	light;			//ライト情報
						D3DXVECTOR4	posLight;		//ライトの位置
						D3DXVECTOR3	pos, normal;	//平面上の任意の点、法線ベクトル
						D3DXPLANE	plane;			//平面情報

													//ライトの位置を設定
						pDevice->GetLight(0, &light);
						posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

						//平面情報を生成
						pos = ZERO_SET;
						normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
						D3DXPlaneFromPointNormal(&plane, &pos, &normal);

						//シャドウマトリックスの初期化
						D3DXMatrixIdentity(&mtxShadow);

						//シャドウマトリックスの作成
						D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
						D3DXMatrixMultiply(&mtxShadow, &g_aPlayerPvP[nCntPlayer].animalInst[nCntParts].mtxWorld, &mtxShadow);

						//シャドウマトリックスの設定
						pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

						//マテリアルデータへのポインタを取得
						pMat = (D3DXMATERIAL *)useAnimal.aParts[nCntParts].pBuffMat->GetBufferPointer();

						for (int nCntMat = 0; nCntMat < (int)useAnimal.aParts[nCntParts].dwNumMatModel; nCntMat++)
						{
							D3DMATERIAL9 MatCopy = pMat[nCntMat].MatD3D;	//マテリアルデータ複製

																			//黒色に設定						//自己発光を無くす
							MatCopy.Diffuse = XCOL_BLACKSHADOW;	MatCopy.Emissive = XCOL_BLACK;

							//マテリアル設定
							pDevice->SetMaterial(&MatCopy);

							//テクスチャ設定
							pDevice->SetTexture(0, NULL);

							//モデル描画
							useAnimal.aParts[nCntParts].pMesh->DrawSubset(nCntMat);
						}
					}
				}
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//プレイヤーのチャージ処理
//========================
void ChargePlayer(int nChargePlayer)
{
	g_aPlayerPvP[nChargePlayer].moveGauge += PLAYER_POWER_ADD;

	if (g_aPlayerPvP[nChargePlayer].moveGauge >= PLAYER_POWER_MAX)
	{
		g_aPlayerPvP[nChargePlayer].moveGauge = PLAYER_POWER_MAX;
		StopSoundSE(SOUND_LABEL_SE_ENERGY_00);
	}
	if (g_aPlayerPvP[nChargePlayer].moveGauge <= PLAYER_POWER_ADD)
	{
		PlaySoundSE(SOUND_LABEL_SE_ENERGY_00, nChargePlayer);
	}

	g_aPlayerPvP[nChargePlayer].stat = PLAYERSTAT_CHARGE;
	SetChargeEffect(g_aPlayerPvP[nChargePlayer].pos, nChargePlayer);
	SetChargeCylinder(g_aPlayerPvP[nChargePlayer].pos, nChargePlayer);
}

//========================
//プレイヤーのダッシュ処理
//========================
void DashPlayer(int nDashPlayer)
{
	PlaySoundSE(SOUND_LABEL_SE_GRASSDASH, nDashPlayer);

	//アクション硬直用にパワー移動
	g_aPlayerPvP[nDashPlayer].fOldMoveGauge = g_aPlayerPvP[nDashPlayer].moveGauge;

	//進行方向の設定
	g_aPlayerPvP[nDashPlayer].move.x = -sinf(g_aPlayerPvP[nDashPlayer].rot.y) * g_aPlayerPvP[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;
	g_aPlayerPvP[nDashPlayer].move.z = -cosf(g_aPlayerPvP[nDashPlayer].rot.y) * g_aPlayerPvP[nDashPlayer].moveGauge * PLAYER_MOVE_SPEED;

	g_aPlayerPvP[nDashPlayer].stat = PLAYERSTAT_DASH;
	g_aPlayerPvP[nDashPlayer].moveGauge = 0;
}

//========================
//プレイヤーのヒップドロップ処理
//========================
void HipDropPlayer(int nHipDropPlayer)
{
	PlaySoundSE(SOUND_LABEL_SE_HIPSPIN, nHipDropPlayer);//ヒップドロップ音再生

	g_aPlayerPvP[nHipDropPlayer].moveV0.y = PLAYER_HIPDROP_POWER;		//ヒップドロップの降下速度を代入
	g_aPlayerPvP[nHipDropPlayer].move.x = 0.0f;							//X・Zの移動量消す
	g_aPlayerPvP[nHipDropPlayer].move.z = 0.0f;
	g_aPlayerPvP[nHipDropPlayer].moveGauge = 0.0f;
	g_aPlayerPvP[nHipDropPlayer].jumpTime = 0;							//ジャンプ時間リセット
	g_aPlayerPvP[nHipDropPlayer].bHipDrop = true;						//ヒップドロップしている
	g_aPlayerPvP[nHipDropPlayer].bHipDropSpin = true;					//スピンしている
	g_aPlayerPvP[nHipDropPlayer].stat = PLAYERSTAT_HIPDROP;
}

//========================
//プレイヤーのジャンプ処理
//========================
void JumpPlayer(int nJumpPlayer)
{
	PlaySoundSE(SOUND_LABEL_SE_JUMP, nJumpPlayer);

	g_aPlayerPvP[nJumpPlayer].moveV0.y = PLAYER_JUMP_SPEED;//移動量設定
	g_aPlayerPvP[nJumpPlayer].jumpTime = 0;	//ジャンプ時間リセット
	g_aPlayerPvP[nJumpPlayer].bJump = true;
	g_aPlayerPvP[nJumpPlayer].stat = PLAYERSTAT_JUMP;
}

//========================
//プレイヤーの処理
//========================
void ControllPlayer(int nPlayerNum)
{
	//ヒップドロップ中でなければ操作できる
	if (g_aPlayerPvP[nPlayerNum].bHipDrop == false)
	{
		if ((int)(g_aPlayerPvP[nPlayerNum].move.x * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0
			&& (int)(g_aPlayerPvP[nPlayerNum].move.z * pow(10, DECIMAL_PLACE + 1)) / (int)pow(10, DECIMAL_PLACE) == 0)
		{//もうこれ動いてるって言わないよね（ほぼ動いていない）
			if (g_aPlayerPvP[nPlayerNum].stat == PLAYERSTAT_DASH)
			{
				g_aPlayerPvP[nPlayerNum].nActionRigor = PLAYER_DASH_ACTIONRIGOR * PLAYER_D_ACTRIGOR_CALC(g_aPlayerPvP[nPlayerNum].fOldMoveGauge);
				g_aPlayerPvP[nPlayerNum].stat = PLAYERSTAT_WAIT;
			}
			else if (g_aPlayerPvP[nPlayerNum].stat == PLAYERSTAT_JUMP)
			{
				if (GetButton(nPlayerNum, INPUTTYPE_PRESS, BUTTON_X) == true)
				{//Xボタンが押された					 
				 //プレイヤーのチャージ処理
					ChargePlayer(nPlayerNum);
				}
				else if (GetButton(nPlayerNum, INPUTTYPE_RELEASE, BUTTON_X) == true)
				{//Xボタンが離された
				 //プレイヤーのダッシュ処理
					DashPlayer(nPlayerNum);
				}
			}
			else if (g_aPlayerPvP[nPlayerNum].stat != PLAYERSTAT_LAND)
			{
				g_aPlayerPvP[nPlayerNum].nActionRigor--;
				if (g_aPlayerPvP[nPlayerNum].nActionRigor <= 0)
				{
					if (GetButton(nPlayerNum, INPUTTYPE_PRESS, BUTTON_X) == true)
					{//Xボタンが押された					 
					 //プレイヤーのチャージ処理
						ChargePlayer(nPlayerNum);
					}
					else if (GetButton(nPlayerNum, INPUTTYPE_RELEASE, BUTTON_X) == true)
					{//Xボタンが離された
					 //プレイヤーのダッシュ処理
						DashPlayer(nPlayerNum);
					}
					else
					{
						g_aPlayerPvP[nPlayerNum].stat = PLAYERSTAT_WAIT;
					}
				}
			}
		}

		//ジャンプ・ヒップドロップ
		if (GetButton(nPlayerNum, INPUTTYPE_TRIGGER, BUTTON_A) == true && g_aPlayerPvP[nPlayerNum].bHipDrop == false && g_aPlayerPvP[nPlayerNum].nActionRigor <= 0)
		{
			if (g_aPlayerPvP[nPlayerNum].bJump)
			{
				HipDropPlayer(nPlayerNum);									//プレイヤーのヒップドロップ処理
				g_aPlayerPvP[nPlayerNum].nHipDropWait = PLAYER_HIPDROP_WAIT;	//ヒップドロップの開始硬直を設定
			}
			else
			{
				JumpPlayer(nPlayerNum);			//プレイヤーのジャンプ処理
			}
		}
	}
	//ヒップドロップ中
	else
	{
		//ヒップドロップのスピン中	
		if (g_aPlayerPvP[nPlayerNum].bHipDropSpin)
		{
			//前転させる				
			g_aPlayerPvP[nPlayerNum].rot.x += PLAYER_HIPSPIN_SPEED;

			//１周した				
			if (g_aPlayerPvP[nPlayerNum].rot.x <= PLAYER_HIPSPIN_LAP)
			{
				g_aPlayerPvP[nPlayerNum].rot.x = 0.0f;			//回転値を元に戻す
				g_aPlayerPvP[nPlayerNum].bHipDropSpin = false;	//スピンし終わった		
			}
		}

		//ヒップドロップ硬直時間がある
		else
		{
			g_aPlayerPvP[nPlayerNum].nHipDropWait--;		//硬直時間を減らしていく
		}
	}
}

//========================
//プレイヤーの向き変更処理
//========================
void RotatePlayer(int nPadNum)
{
	if (!g_bDebugMove)
	{//リリース用
	 //ゲームパッド部
		if (GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_LEFT)
		{
			g_aPlayerPvP[nPadNum].rot.y -= PLAYER_ROTATE_SPEED;
		}
		else if(GetStick(nPadNum, INPUTTYPE_PRESS).x == CONVSTICK_RIGHT)
		{
			g_aPlayerPvP[nPadNum].rot.y += PLAYER_ROTATE_SPEED;
		}
	}
	else
	{//デバッグ用
		//モデル移動
		//ゲームパッド部
		if (GetLStickX(nPadNum) > 0 || GetLStickX(nPadNum) < 0)
		{//X方向のスティックが傾いている
			g_aPlayerPvP[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
		}
		else if (GetLStickY(nPadNum) > 0 || GetLStickY(nPadNum) < 0)
		{//Y方向のスティックだけ傾いている
			g_aPlayerPvP[nPadNum].rot.y = FIX_ROT(atan2f(GetLStickX(nPadNum), GetLStickY(nPadNum)) + D3DX_PI);
		}
		//キーボード部
		else if (GetKeyboardPress(DIK_W) == true)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_P3);
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_P4);
			}
			else
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + (1.0f * D3DX_PI));
			}
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_P1);
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + ROT_P2);
			}
			else
			{
				g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y);
			}
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{
			g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + (-0.5f * D3DX_PI));
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aPlayerPvP[nPadNum].rot.y = -FIX_ROT(GetCamera()->rot.y + (0.5f * D3DX_PI));
		}
		else
		{
			return;
		}
	}
}

//========================
//ダウンしたプレイヤーの処理
//========================
void DownPlayer(int nDownPlayerNum)
{
	if (g_aPlayerPvP[nDownPlayerNum].nLastHitPlayer != -1)
	{//当てられて落ちた場合
		g_aPlayerPvP[g_aPlayerPvP[nDownPlayerNum].nLastHitPlayer].nScore++;
		AddScore(1, g_aPlayerPvP[nDownPlayerNum].nLastHitPlayer);
	}
	else
	{//ただ単に自滅した場合
		for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++)
		{
			if (nCntOtherPlayer != nDownPlayerNum)
			{
				g_aPlayerPvP[nCntOtherPlayer].nScore++;
				AddScore(1, nCntOtherPlayer);
			}
		}
	}

	PlaySoundSE(SOUND_LABEL_SE_DROP, nDownPlayerNum);

	RespawnPlayer(nDownPlayerNum);
}

//========================
//プレイヤーのリスポーン処理
//========================
void RespawnPlayer(int nRespawnPlayer)
{
	//各ステータス再設定
	//位置設定（かぶらないようにする）
	for (int nCntNUse = 0; nCntNUse < MAX_USE_GAMEPAD; nCntNUse++)
	{
		if (g_pNotMove[nCntNUse] == NULL || g_pNotMove[nCntNUse]->bNotMove == false)
		{
			//前にリスポーンした位置番号のポインタをNULLにする
			if (g_pNotMove[g_aPlayerPvP[nRespawnPlayer].nRespawnPosNum] == &g_aPlayerPvP[nRespawnPlayer])
			{
				g_pNotMove[g_aPlayerPvP[nRespawnPlayer].nRespawnPosNum] = NULL;
			}			

			//新しい位置を入れる
			g_aPlayerPvP[nRespawnPlayer].nRespawnPosNum = nCntNUse;
			g_aPlayerPvP[nRespawnPlayer].pos = D3DXVECTOR3(c_aPosRot[nCntNUse][0].x, 70.0f, c_aPosRot[nCntNUse][0].z);
			//角度も調整するならここに書く

			//動いていないことにする
			g_aPlayerPvP[nRespawnPlayer].bNotMove = true;

			//参照用ポインタ代入
			g_pNotMove[nCntNUse] = &g_aPlayerPvP[nRespawnPlayer];
			break;
		}
	}

	//その他設定
	g_aPlayerPvP[nRespawnPlayer].posOld = g_aPlayerPvP[nRespawnPlayer].pos;
	g_aPlayerPvP[nRespawnPlayer].move = ZERO_SET;
	g_aPlayerPvP[nRespawnPlayer].moveV0 = ZERO_SET;
	g_aPlayerPvP[nRespawnPlayer].rot = ZERO_SET;
	g_aPlayerPvP[nRespawnPlayer].moveGauge = 0;
	g_aPlayerPvP[nRespawnPlayer].jumpTime = 0;
	g_aPlayerPvP[nRespawnPlayer].nHipDropWait = 0;
	g_aPlayerPvP[nRespawnPlayer].bJump = true;
	g_aPlayerPvP[nRespawnPlayer].bHipDrop = false;
	g_aPlayerPvP[nRespawnPlayer].bHipDropSpin = false;

	g_aPlayerPvP[nRespawnPlayer].nFrameAtkPlayer = -1;
	g_aPlayerPvP[nRespawnPlayer].nLastHitPlayer = -1;
	g_aPlayerPvP[nRespawnPlayer].stat = PLAYERSTAT_WAIT;

	g_aPlayerPvP[nRespawnPlayer].nATKItemTime = 0;
	g_aPlayerPvP[nRespawnPlayer].nDEFItemTime = 0;
	g_aPlayerPvP[nRespawnPlayer].nGhostItemTime = 0;
}

//========================
//アイテムカウント減らす処理
//========================
void DecrementItemTime(int nPlayerNum)
{
	g_aPlayerPvP[nPlayerNum].nATKItemTime--;
	g_aPlayerPvP[nPlayerNum].nDEFItemTime--;
	g_aPlayerPvP[nPlayerNum].nGhostItemTime--;
	g_aPlayerPvP[nPlayerNum].nInvincibleTime--;
}

//========================
//アイテムパーティクル表示処理
//========================
void ItemStateParticle(int nPlayerNum)
{
	if (g_aPlayerPvP[nPlayerNum].nATKItemTime > 0)
	{
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 12.0f, 15, PARTICLE_ACSORPTION, OBJECT_PLAYER_ATK);
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 7.0f, 15, PARTICLE_ACSORPTION, OBJECT_PLAYER_ATK);
	}


	if (g_aPlayerPvP[nPlayerNum].nDEFItemTime > 0)
	{
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 5.0f, 15, PARTICLE_NORMAL, OBJECT_PLAYER_DEF);
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 7.0f, 15, PARTICLE_NORMAL, OBJECT_PLAYER_DEF);
	}


	if (g_aPlayerPvP[nPlayerNum].nGhostItemTime > 0)
	{
		SetParticle(g_aPlayerPvP[nPlayerNum].pos + D3DXVECTOR3(0.0f,7.0f,0.0f), 12.0f, 10, PARTICLE_ACSORPTION_GHOST, OBJECT_PLAYER_GHOST);
		SetParticle(g_aPlayerPvP[nPlayerNum].pos + D3DXVECTOR3(0.0f,7.0f,0.0f), 7.0f, 10, PARTICLE_ACSORPTION_GHOST, OBJECT_PLAYER_GHOST);
	}


	if (g_aPlayerPvP[nPlayerNum].nInvincibleTime > 0)
	{
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 12.0f, 15, PARTICLE_ACSORPTION, OBJECT_PLAYER_INVINCIBLE);
		SetParticle(g_aPlayerPvP[nPlayerNum].pos, 7.0f, 15, PARTICLE_ACSORPTION, OBJECT_PLAYER_INVINCIBLE);
	}
}

//========================
//モーション設定処理
//========================
void SetMotion(int nPlayerNum, MOTIONTYPE type)
{
	g_aPlayerPvP[nPlayerNum].motion.motionType = type;
	g_aPlayerPvP[nPlayerNum].motion.nNowKey = 0;
	g_aPlayerPvP[nPlayerNum].motion.nCounterMotion = 0;
}

//========================
//モーション更新処理
//========================
void UpdateMotion(int nPlayerNum)
{
	Model model = GetAnimal(g_aPlayerPvP[nPlayerNum].animal);
	MOTION_INFO mi = GetMotionInfo(g_aPlayerPvP[nPlayerNum].animal, g_aPlayerPvP[nPlayerNum].motion.motionType);

	//今のキーと次のキーを入れておく
	int nNowKey = g_aPlayerPvP[nPlayerNum].motion.nNowKey;
	int nNextKey = g_aPlayerPvP[nPlayerNum].motion.nNowKey + 1;

	for (int CntModel = 0; CntModel < MAX_PARTS; CntModel++)
	{
		//オフセットを入れておく
		g_aPlayerPvP[nPlayerNum].motionPosOffset = model.aParts[CntModel].posOffset;
		g_aPlayerPvP[nPlayerNum].motionRotOffset = model.aParts[CntModel].rotOffset;

		//差分算出
		float posDiffX = mi.aKeyInfo[nNextKey].aKey[CntModel].fPosX -
			mi.aKeyInfo[nNowKey].aKey[CntModel].fPosX;
		float posDiffY = mi.aKeyInfo[nNextKey].aKey[CntModel].fPosY -
			mi.aKeyInfo[nNowKey].aKey[CntModel].fPosY;
		float posDiffZ = mi.aKeyInfo[nNextKey].aKey[CntModel].fPosZ -
			mi.aKeyInfo[nNowKey].aKey[CntModel].fPosZ;
		float rotDiffX = mi.aKeyInfo[nNextKey].aKey[CntModel].fRotX -
			mi.aKeyInfo[nNowKey].aKey[CntModel].fRotX;
		float rotDiffY = mi.aKeyInfo[nNextKey].aKey[CntModel].fRotY -
			mi.aKeyInfo[nNowKey].aKey[CntModel].fRotY;
		float rotDiffZ = mi.aKeyInfo[nNextKey].aKey[CntModel].fRotZ -
			mi.aKeyInfo[nNowKey].aKey[CntModel].fRotZ;

		//位置向き算出
		float posDemandX = mi.aKeyInfo[nNowKey].aKey[CntModel].fPosX +
			posDiffX * ((float)g_aPlayerPvP[nPlayerNum].motion.nCounterMotion / mi.aKeyInfo[nNowKey].nFrame);
		float posDemandY = mi.aKeyInfo[nNowKey].aKey[CntModel].fPosY +
			posDiffY * ((float)g_aPlayerPvP[nPlayerNum].motion.nCounterMotion / mi.aKeyInfo[nNowKey].nFrame);
		float posDemandZ = mi.aKeyInfo[nNowKey].aKey[CntModel].fPosZ +
			posDiffZ * ((float)g_aPlayerPvP[nPlayerNum].motion.nCounterMotion / mi.aKeyInfo[nNowKey].nFrame);
		float rotDemandX = mi.aKeyInfo[nNowKey].aKey[CntModel].fRotX +
			rotDiffX * ((float)g_aPlayerPvP[nPlayerNum].motion.nCounterMotion / mi.aKeyInfo[nNowKey].nFrame);
		float rotDemandY = mi.aKeyInfo[nNowKey].aKey[CntModel].fRotY +
			rotDiffY * ((float)g_aPlayerPvP[nPlayerNum].motion.nCounterMotion / mi.aKeyInfo[nNowKey].nFrame);
		float rotDemandZ = mi.aKeyInfo[nNowKey].aKey[CntModel].fRotZ +
			rotDiffZ * ((float)g_aPlayerPvP[nPlayerNum].motion.nCounterMotion / mi.aKeyInfo[nNowKey].nFrame);

		//パーツの位置向き設定
		g_aPlayerPvP[nPlayerNum].animalInst[CntModel].pos = g_aPlayerPvP[nPlayerNum].motionPosOffset + D3DXVECTOR3(posDemandX, posDemandY, posDemandZ);
		g_aPlayerPvP[nPlayerNum].animalInst[CntModel].rot = g_aPlayerPvP[nPlayerNum].motionRotOffset + D3DXVECTOR3(rotDemandX, rotDemandY, rotDemandZ);
	}
	g_aPlayerPvP[nPlayerNum].motion.nCounterMotion++;

	//再生フレーム後の挙動
	if (g_aPlayerPvP[nPlayerNum].motion.nCounterMotion == mi.aKeyInfo[nNowKey].nFrame)
	{//再生フレーム数に達したら
		//カウンターをリセットしてキーを一つ増やす
		g_aPlayerPvP[nPlayerNum].motion.nCounterMotion = 0;
		g_aPlayerPvP[nPlayerNum].motion.nNowKey++;

		//キーの最大数に達したらループするか否かに応じて再設定する
		if (g_aPlayerPvP[nPlayerNum].motion.nNowKey == mi.nNumKey)
		{
			if (mi.bLoop == true)
			{//ループさせる
				g_aPlayerPvP[nPlayerNum].motion.nNowKey = 0;
			}
			else
			{//通常はNEUTRAL状態にする
				SetMotion(nPlayerNum, MOTIONTYPE_NEUTRAL);
				g_aPlayerPvP[nPlayerNum].bAnimed = true;
			}
		}
	}
}

//========================
//プレイヤー指定処理
//========================
void SetPlayerType_PvP(int nPlayerNum, bool bUse, bool bAIUse)
{
	//プレイヤー使用するか指定
	g_aPlayerPvP[nPlayerNum].bUsePlayer = bUse;

	//プレイヤー使用するならAI指定
	if (g_aPlayerPvP[nPlayerNum].bUsePlayer == true)
	{
		g_aAIMove_PvP[nPlayerNum] = bAIUse;
	}
	else
	{//使用しないならAIも使用しない
		g_aAIMove_PvP[nPlayerNum] = false;
	}
}

//========================
//取得処理
//========================
Player *GetPlayer(void)
{
	return &g_aPlayerPvP[0];
}