/*==========================================================================================================================

												プレイヤーの処理[boss.cpp]
													Author:平澤詩苑

==========================================================================================================================*/
#include "boss.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "game.h"
#include "explosion.h"
#include "enemy.h"
#include "particle.h"
#include "score.h"

//マクロ定義
#define BOSS_HP			(90)		//体力
#define DESTROY_SCORE	(80000)		//ボス撃破時のスコア
#define MOVE_SPIN		(-60.0f)	//スピンの回転値
#define ADD_SPIN		(0.5f)		//回転の加算値
#define BULLET_SP_BOSS	(10.0f)		//ボスの弾の速さ
#define SPIN_INTERVAL	(300)		//スピンのインターバル
#define NUM_PARTICLE	(50)		//ボス撃破時のパーティクル発生量
#define BOSS_TEX		"data/TEXTURE/Boss.png"		//ボスの画像名

#define NORMAL_BONUS	(1)			//不通に倒した時のボーナススコア
#define SPECIAL_BONUS	(3)			//リフレクト弾で倒した時のボーナススコア倍率

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureBoss;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;	//頂点バッファへのポインタ
BOSS g_Boss;									//ボスの情報

//ボスの初期化処理
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//ボス情報の初期化
	{
		g_Boss.pos = D3DXVECTOR3(600.0f, -100.0f, 0.0f);//位置を初期化する
		g_Boss.move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);	//移動量を初期化する
		g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きを初期化する(Z値を使用)
		g_Boss.BulletType = BULLETTYPE_BOSS;			//弾の種類を設定
		g_Boss.homingtype = HOMING_IMPOSSIBLE;			//ホーミング性能の初期化（最初はナシ)
		g_Boss.SpinInterval = SPIN_INTERVAL;			//スピンインターバルの初期化
		g_Boss.BossAttackCnt = 30;						//ボスの攻撃間隔の初期化
		g_Boss.nLife = BOSS_HP;							//体力初期化
		g_Boss.state = BOSSSTATE_APPEAR;				//状態初期化
		g_Boss.nCounterState = 0;						//状態管理カウンタ初期化
		g_Boss.bDisp = false;							//表示ON/OFF(最初はOFF)
	}

	//対角線の長さを算出する
	g_Boss.fLength = sqrtf(60 * 60 + 60 * 60) * 1.0f;

	//対角線の角度を算出する
	g_Boss.fAngle = atan2f(60, 60);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BOSS_TEX, &g_pTextureBoss);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBoss, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void* *)&pVtx, 0);

	//頂点座標の設定
	{
		pVtx[0].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (-D3DX_PI + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[0].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (-D3DX_PI + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[0].pos.z = 0.0f;
		
		pVtx[1].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z +  (D3DX_PI - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[1].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z +  (D3DX_PI - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[1].pos.z = 0.0f;
		
		pVtx[2].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (0 - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[2].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (0 - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[2].pos.z = 0.0f;
		
		pVtx[3].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (0 + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[3].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (0 + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[3].pos.z = 0.0f;
	}

	//rhwの設定
	{
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}

	//頂点カラーの設定
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//テクスチャの座標
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffBoss->Unlock();
}

//ボスの終了処理
void UninitBoss(void)
{
	//テクスチャの破棄
	if (g_pTextureBoss != NULL)
	{
		g_pTextureBoss->Release();
		g_pTextureBoss = NULL;
	}

	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//ボスの更新処理
void UpdateBoss(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void* *)&pVtx, 0);

	Player *pPlayer = GetPlayer();

	//ボスの状態情報分岐
	switch (g_Boss.state)
	{
	case BOSSSTATE_APPEAR:
		//頂点カラーの設定
		g_Boss.pos.y += 1.0f;

		if (g_Boss.pos.y >= 120.0f)
		{
			g_Boss.state = BOSSSTATE_NORMAL;
		}
		break;

	case BOSSSTATE_NORMAL:
		//ボスとプレイヤーの当たり判定
		if (pPlayer->pos.x - ENEMY_VERTEX >= g_Boss.pos.x - 30.0f &&
			pPlayer->pos.x + ENEMY_VERTEX <= g_Boss.pos.x + 30.0f &&
			pPlayer->pos.y - ENEMY_VERTEX >= g_Boss.pos.y - 30.0f &&
			pPlayer->pos.y + ENEMY_VERTEX <= g_Boss.pos.y + 30.0f)
		{
			HitPlayer(1);
		}

		//弾発射
		if (g_Boss.nLife > 0)
		{
			g_Boss.BossAttackCnt--;

			if (g_Boss.BossAttackCnt <= 0)
			{
				SetBullet(D3DXVECTOR3(g_Boss.pos.x - 40.0f, g_Boss.pos.y + 30.0f, g_Boss.pos.z), D3DXVECTOR3(sinf(g_Boss.rot.z + (D3DX_PI * 0.0f)) * BULLET_SP_BOSS, cosf(g_Boss.rot.z + (D3DX_PI * 0.0f)) * BULLET_SP_BOSS, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_Boss.BulletType, g_Boss.homingtype, BULLETEFFECT_PLAYER);
				SetBullet(D3DXVECTOR3(g_Boss.pos.x + 40.0f, g_Boss.pos.y + 30.0f, g_Boss.pos.z), D3DXVECTOR3(sinf(g_Boss.rot.z + (D3DX_PI * 0.0f)) * BULLET_SP_BOSS, cosf(g_Boss.rot.z + (D3DX_PI * 0.0f)) * BULLET_SP_BOSS, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_Boss.BulletType, g_Boss.homingtype, BULLETEFFECT_PLAYER);
				
				g_Boss.BossAttackCnt = 30;
			}
		}

		//インターバル減少
		g_Boss.SpinInterval--;

		//スピン開始
		if (g_Boss.SpinInterval <= 0)
		{
			//状態をスピン状態に(弾をまき散らす)
			g_Boss.state = BOSSSTATE_SPIN;
			g_Boss.SpinInterval = SPIN_INTERVAL;
			g_Boss.BossAttackCnt = 3;

			//向きを更新(Z値を回転させる)
			g_Boss.rot.z = MOVE_SPIN;
		}
		break;

	case BOSSSTATE_SPIN:
		if (g_Boss.rot.z < 0.0f)
		{
			g_Boss.rot.z += ADD_SPIN;
			g_Boss.BossAttackCnt--;

			if (g_Boss.BossAttackCnt == 0)
			{
				SetBullet(D3DXVECTOR3(g_Boss.pos.x - 15.0f, g_Boss.pos.y + 30.0f, g_Boss.pos.z), D3DXVECTOR3(sinf(g_Boss.rot.z + (D3DX_PI * 1.0f)) * BULLET_SP_BOSS, cosf(g_Boss.rot.z + (D3DX_PI * 1.0f)) * BULLET_SP_BOSS, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), g_Boss.BulletType, HOMING_IMPOSSIBLE, BULLETEFFECT_PLAYER);
			
				g_Boss.BossAttackCnt = 1;
			}

			if (g_Boss.rot.z >= 0.0f)
			{
				//状態を普通に
				g_Boss.state = BOSSSTATE_NORMAL;
				g_Boss.rot.z = 0.0f;
				g_Boss.SpinInterval = SPIN_INTERVAL;
				g_Boss.BossAttackCnt = 60;
			}
		}
		break;

	case BOSSSTATE_DAMAGE:
		g_Boss.nCounterState--;

		if (g_Boss.nCounterState <= 0)
		{
			g_Boss.state = BOSSSTATE_NORMAL;

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case BOSSSTATE_DEATH:
		g_Boss.nLife = 0;
		g_Boss.bDisp = false;
	}

	if (g_Boss.state != BOSSSTATE_APPEAR)
	{
		//位置を更新
		g_Boss.pos.x += g_Boss.move.x;

		if (g_Boss.pos.x >= 1240.0f || g_Boss.pos.x <= 50.0f)
		{
			g_Boss.move *= -1.0f;
		}
	}

	//頂点座標の設定
	{
		pVtx[0].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (-D3DX_PI + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[0].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (-D3DX_PI + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[1].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (D3DX_PI - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[1].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (D3DX_PI - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[2].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (0 - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[2].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (0 - g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[3].pos.x = g_Boss.pos.x + sinf(g_Boss.rot.z + (0 + g_Boss.fAngle)) * g_Boss.fLength;
		pVtx[3].pos.y = g_Boss.pos.y + cosf(g_Boss.rot.z + (0 + g_Boss.fAngle)) * g_Boss.fLength;
	}

	//画面範囲外処理
	{
		//横の範囲外処理
		{
			//画面外に出たら、反対側に戻す
			if (g_Boss.pos.x > 1240.0f)
			{//右端
				g_Boss.pos.x = 1240.0f;
			}

			if (g_Boss.pos.x < 40.0f)
			{//左端
				g_Boss.pos.x = 40.0f;
			}
		}

		//縦の範囲外処理
		{
			//画面外に出たら、反対側に戻す
			if (g_Boss.pos.y > 680.0f)
			{//底面
				g_Boss.pos.y = 680.0f;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBoss->Unlock();
}

//ボスの描画処理
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Boss.bDisp == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBoss);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, SEC);
	}
}

//ボスの設定処理
void SetBoss(void)
{
	g_Boss.pos = D3DXVECTOR3(600.0f, -100.0f, 0.0f);//位置を初期化する
	g_Boss.move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);	//移動量を初期化する
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きを初期化する(Z値を使用)
	g_Boss.BulletType = BULLETTYPE_BOSS;			//弾の種類を設定
	g_Boss.homingtype = HOMING_IMPOSSIBLE;			//ホーミング性能の初期化（最初はナシ)
	g_Boss.SpinInterval = SPIN_INTERVAL;			//スピンインターバルの初期化
	g_Boss.BossAttackCnt = 30;						//ボスの攻撃間隔の初期化
	g_Boss.nLife = BOSS_HP;							//体力初期化
	g_Boss.state = BOSSSTATE_APPEAR;				//状態初期化
	g_Boss.nCounterState = 0;						//状態管理カウンタ初期化
	g_Boss.bDisp = true;
}

//ボスの取得
BOSS *GetBoss(void)
{
	return &g_Boss;
}

//ボスのヒット処理
void HitBoss(int nDamage, REFLECTBULLET type)
{
	g_Boss.nLife -= nDamage;

	if (g_Boss.nLife <= 0)
	{
		SetExplosion(g_Boss.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//ゲーム状態の設定
		SetGameState(GAMESTATE_END, 60);
		g_Boss.state = BOSSSTATE_DEATH;

		//リフレクトボーナス
		int BonusScore = NORMAL_BONUS;

		if (type == REFLECTBULLET_YES)
		{
			BonusScore = SPECIAL_BONUS;
		}

		AddScore(DESTROY_SCORE * BonusScore);
		g_Boss.bDisp = false;

		for (int nCntBoss = 0; nCntBoss < NUM_PARTICLE; nCntBoss++)
		{
			D3DXVECTOR3 fRandom = D3DXVECTOR3((float)(rand() % 20) - 5.0f, (float)(rand() % 20) - 5.0f, 0.0f); //パーティクルSTAYの発生場所

			SetParticle(g_Boss.pos, D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);	//パーティクルを発生
			SetParticle(g_Boss.pos + fRandom, D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_STAY);	//パーティクルを発生
		}
	}

	else
	{
		//ポインタを設定
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffBoss->Lock(0, 0, (void* *)&pVtx, 0);

		g_Boss.state = BOSSSTATE_DAMAGE;
		g_Boss.nCounterState = 2;

		//頂点カラーの設定
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		}

		//頂点バッファをロックする
		g_pVtxBuffBoss->Unlock();
	}
}