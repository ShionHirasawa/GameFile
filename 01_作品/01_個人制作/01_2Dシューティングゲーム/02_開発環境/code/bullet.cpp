/*==========================================================================================================================

												弾の発射処理[bullet.cpp]
													Author:平澤詩苑

==========================================================================================================================*/
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "effect.h"
#include <math.h>					//絶対値を参照するのに必要
#include "particle.h"
#include "boss.h"

//マクロ定義
#define MAX_BULLET			(256)		//弾の最大数
#define BULLET_HP			(130)		//弾の体力
#define REFRECT_BULLET_HP	(200)		//跳ね返された弾の体力
#define FCORRECTION			(1.0f)		//ホーミング弾の補正係数の数値
#define FHOMINGMOVE			(5.0f)		//ホーミング弾の移動量
#define FANGLE_CORRECT		(6.28f)		//角度の修正に使用
#define DAMAGE				(1)			//弾の攻撃力

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];							//弾の情報
float g_fLengthBullet;				//対角線の長さ
float g_fAngleBullet;				//対角線の角度
bool g_F4;							//無敵ON/OFF

//弾の初期化処理
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &g_pTextureBullet);

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = BULLET_HP;
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].EffectAlphaColor = 0.5f;				//弾の軌跡のα値を指定する変数
		g_aBullet[nCntBullet].homingtype = HOMING_IMPOSSIBLE;		//ホーミング性能ナシの状態に設定
		g_aBullet[nCntBullet].Reflect = REFLECTBULLET_NO;			//跳ね返されていない状態に設定
		g_aBullet[nCntBullet].bUse = false;							//使用していない状態にする
	}

	g_F4 = false;					//無敵ON/OFFの初期化

	//対角線の長さを算出する
	g_fLengthBullet = sqrtf(20 * 20 + 20 * 20) * 0.3f;

	//対角線の角度を算出する
	g_fAngleBullet = atan2f(20, 20);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		{
			pVtx[0].pos = D3DXVECTOR3(  0.0f,   0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(  0.0f,   0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(  0.0f,   0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(  0.0f,   0.0f, 0.0f);
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
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャの座標
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffBullet->Unlock();
}

//弾の終了処理
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{			 
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}			 
				 
	if (g_pVtxBuffBullet != NULL)
	{			 
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//弾の更新処理
void UpdateBullet(void)
{
	int nCntBullet;
	int DeadCount = ZERO;

	//プレイヤーの無敵ON/OFF
	if (GetKeyboardTrigger(DIK_F4) == true)
	{
		g_F4 = g_F4 == true ? false : true;
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			//弾の種類による当たり判定
			switch (g_aBullet[nCntBullet].type)
			{
			case BULLETTYPE_PLAYER:
				//敵の取得
				CollisionEnemy(&g_aBullet[nCntBullet]);
				break;

			case BULLETTYPE_ENEMY:
			case BULLETTYPE_BOSS:
				//プレイヤーの取得
				CollisionPlayer(&g_aBullet[nCntBullet]);
				break;
			}

			//弾の位置の更新
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;

			/*--------------------------------------------------
								ホーミング処理
			----------------------------------------------------*/
			if (g_aBullet[nCntBullet].homingtype == HOMING_POSSIBLE)
				{
					float fRotmove, fRotDest, fRotDiff;		//ホーミング処理に必要な変数さんたち
					float fCorrection = 0.2f;				//補正係数
					Player *pPlayer = GetPlayer();			//プレイヤーの情報
					bool bEnemyUse = false;					//敵がいるかどうかのフラグ

					fRotmove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);		//現在の移動方向(角度)

					//敵の弾の場合
					if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
					{
						fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);	//目標の移動方向(角度)
					
						bEnemyUse = true;
					}

					//プレイヤーの弾の場合
					else if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
					{
						Enemy *pEnemy = GetEnemy();
						BOSS *pBoss = GetBoss();
						int nCntEnemy;
						float fposXDiff, fposYDiff;								//敵とプレイヤーのX、Y座標の差分
						D3DXVECTOR3 nTargetEnemypos;							//ターゲットの座標を記憶する

						//プレイヤーに一番近い敵を索敵
						for (nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
						{
							//敵が使用されているか
							if (pEnemy->bUse == true)
							{
								fposXDiff = pPlayer->pos.x - pEnemy->pos.x;		//X座標の差分を計算
								fposYDiff = pPlayer->pos.y - pEnemy->pos.y;		//Y座標の差分を計算

								bEnemyUse = true;	//敵がいるかどうかの判定

								//差分が小さければ敵との距離が近いということ
								if (fabsf(fposXDiff) < fabsf(g_aBullet[nCntBullet].pos.x - nTargetEnemypos.x) && fabsf(fposYDiff) < fabsf(g_aBullet[nCntBullet].pos.y - nTargetEnemypos.y))
								{									
									nTargetEnemypos = pEnemy->pos;		//nTargetEnemyposに敵の座標を代入
								}
							}

							//ボスが使用されているかどうか
							else if (pBoss->bDisp == true)
							{
								fposXDiff = pPlayer->pos.x - pBoss->pos.x;		//X座標の差分を計算
								fposYDiff = pPlayer->pos.y - pBoss->pos.y;		//Y座標の差分を計算

								bEnemyUse = true;	//敵がいるかどうかの判定

								//差分が小さければ敵との距離が近いということ
								if (fabsf(fposXDiff) < fabsf(g_aBullet[nCntBullet].pos.x - nTargetEnemypos.x) && fabsf(fposYDiff) < fabsf(g_aBullet[nCntBullet].pos.y - nTargetEnemypos.y))
								{
									//nTargetEnemyposに敵の座標を代入
									nTargetEnemypos = pBoss->pos;
								}

								fCorrection = FCORRECTION;	//ボスなら補正係数を最大に
							}
						}

						fRotDest = atan2f(nTargetEnemypos.x - g_aBullet[nCntBullet].pos.x, nTargetEnemypos.y - g_aBullet[nCntBullet].pos.y);//目標の移動方向(角度)
					}

					//敵がいるならホーミング
					if (bEnemyUse == true && pPlayer->bDisp == true)
					{
						//目標の移動方向への差分
						fRotDiff = fRotDest - fRotmove;

						//差分の角度を修正
						if (fRotDiff <= -D3DX_PI)
						{//-3.14以下なら
							fRotDiff = fRotDiff + FANGLE_CORRECT;
						}
						else if (fRotDiff >= D3DX_PI)
						{//3.14以上なら
							fRotDiff = fRotDiff - FANGLE_CORRECT;
						}

						//移動方向(角度)の補正
						fRotmove += fRotDiff * fCorrection;

						//移動方向の角度の修正
						if (fRotmove <= -D3DX_PI)
						{//-3.14以下なら
							fRotmove = fRotmove + FANGLE_CORRECT;
						}
						else if (fRotmove >= D3DX_PI)
						{//3.14以上なら
							fRotmove = fRotmove - FANGLE_CORRECT;
						}

						g_aBullet[nCntBullet].move.x = sinf(fRotmove) * FHOMINGMOVE;
						g_aBullet[nCntBullet].move.y = cosf(fRotmove) * FHOMINGMOVE;
					}
				}

			//頂点座標の設定
			{
				pVtx[ZERO].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_fAngleBullet)) * g_fLengthBullet;
				pVtx[ZERO].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (-D3DX_PI + g_fAngleBullet)) * g_fLengthBullet;
				pVtx[ONE].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_fAngleBullet)) * g_fLengthBullet;
				pVtx[ONE].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_fAngleBullet)) * g_fLengthBullet;
				pVtx[SEC].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (F_ZERO - g_fAngleBullet)) * g_fLengthBullet;
				pVtx[SEC].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (F_ZERO - g_fAngleBullet)) * g_fLengthBullet;
				pVtx[SAN].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (F_ZERO + g_fAngleBullet)) * g_fLengthBullet;
				pVtx[SAN].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (F_ZERO + g_fAngleBullet)) * g_fLengthBullet;
			}

			pVtx[ZERO].col = g_aBullet[nCntBullet].col;
			pVtx[ONE].col = g_aBullet[nCntBullet].col;
			pVtx[SEC].col = g_aBullet[nCntBullet].col;
			pVtx[SAN].col = g_aBullet[nCntBullet].col;

			//弾が画面外に出た場合
			if (g_aBullet[nCntBullet].pos.y >  730.0f || g_aBullet[nCntBullet].pos.y < F_ZERO
			 || g_aBullet[nCntBullet].pos.x > 1290.0f || g_aBullet[nCntBullet].pos.x < F_ZERO)
			{
				g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
			}

			//寿命のカウントダウン
			g_aBullet[nCntBullet].nLife--;			

			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(F_TEN, F_TEN, F_TEN, g_aBullet[nCntBullet].EffectAlphaColor), 8.0f, 60);

			//寿命が尽きた
			if (g_aBullet[nCntBullet].nLife <= ZERO)
			{
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
			}

			//使われなくなった弾の初期化
			if (g_aBullet[nCntBullet].bUse == false)
			{
				pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
				pVtx[ONE].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
				pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
				pVtx[SAN].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
			}
		}

		//頂点データのポインタを4つ分進める
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//弾の描画処理
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBullet, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureBullet);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * NUM_VERTEX, SEC);
		}
	}
}

//弾の設定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE type, HOMINGTYPE homing, float Alpha)
{
	int nCntBullet;
	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない

			//弾の情報を設定
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].rot.z = ZERO;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].EffectAlphaColor = Alpha;
			g_aBullet[nCntBullet].homingtype = homing;
			g_aBullet[nCntBullet].Reflect = REFLECTBULLET_NO;			
			
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aBullet[nCntBullet].move.x = move.x;
			g_aBullet[nCntBullet].move.y = move.y;
			g_aBullet[nCntBullet].move.z = F_ZERO;

			g_aBullet[nCntBullet].nLife = BULLET_HP;
			g_aBullet[nCntBullet].bUse = true;			//使用している状態にする
			break;
		}
		//頂点データのポインタを4つ分進める
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffBullet->Unlock();
}

//敵の当たり判定
void CollisionEnemy(Bullet *pBullet)
{
	Enemy *pEnemy = GetEnemy();
	BOSS *pBoss = GetBoss();
	int nCntEnemy;
	int nDamage = DAMAGE;		//攻撃力
	//跳ね返された弾ならダメージ２倍
	if (pBullet->Reflect == REFLECTBULLET_YES)
	{
		nDamage++;
	}

	for (nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されている
			if (pBullet->pos.x >= pEnemy->pos.x - 30.0f &&
				pBullet->pos.x <= pEnemy->pos.x + 30.0f &&
				pBullet->pos.y >= pEnemy->pos.y - 30.0f &&
				pBullet->pos.y <= pEnemy->pos.y + 30.0f)
			{
				HitEnemy(nCntEnemy, nDamage, pEnemy->nLifeMax, pBullet->Reflect);
				
				if (pEnemy->nLife > ONE)
				{
					SetParticle(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f), PARTICLETYPE_STAY);
				}

				PlaySound(SOUND_LABEL_SE_HIT);
				pBullet->bUse = false;
			}
		}
	}

	if (pBoss->state == BOSSSTATE_SPIN && pBullet->type == BULLETTYPE_PLAYER)
	{
		//ボスの弾との当たり判定
		if (pBullet->pos.x >= pBoss->pos.x - 50.0f &&
			pBullet->pos.x <= pBoss->pos.x + 50.0f &&
			pBullet->pos.y >= pBoss->pos.y - 20.0f &&
			pBullet->pos.y <= pBoss->pos.y + 20.0f)
		{
			//飛んできた方向へ跳ね返し
			pBullet->move *= -1.0f;

			//弾の寿命を回復
			pBullet->nLife = REFRECT_BULLET_HP;

			//弾の情報を塗り替え
			pBullet->type = BULLETTYPE_BOSS;
			pBullet->homingtype = HOMING_POSSIBLE;
			pBullet->Reflect = REFLECTBULLET_YES;
			pBullet->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

			PlaySound(SOUND_LABEL_SE_REFLECT);	//跳ね返し音を鳴らす
		}
	}

	//ボスが使われている & 当たり判定
	else if (pBullet->pos.x >= pBoss->pos.x - 50.0f &&
			 pBullet->pos.x <= pBoss->pos.x + 50.0f &&
			 pBullet->pos.y >= pBoss->pos.y - 20.0f &&
			 pBullet->pos.y <= pBoss->pos.y + 20.0f && pBoss->bDisp == true && pBoss->state == BOSSSTATE_NORMAL)
	{
		HitBoss(nDamage, pBullet->Reflect);
		PlaySound(SOUND_LABEL_SE_HIT);

		if (pBoss->nLife > ONE)
		{
			SetParticle(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f), PARTICLETYPE_STAY);
		}

		pBullet->bUse = false;
	}
}

//プレイヤーの当たり判定
void CollisionPlayer(Bullet *pBullet)
{
	Player *pPlayer = GetPlayer();

	//スピン中
	if (pPlayer->state == PLAYERSTATE_SPIN && pBullet->type != BULLETTYPE_PLAYER)
	{
		//敵の弾との当たり判定
		if (pBullet->pos.x >= pPlayer->pos.x - 30.0f &&
			pBullet->pos.x <= pPlayer->pos.x + 30.0f &&
			pBullet->pos.y >= pPlayer->pos.y - 30.0f &&
			pBullet->pos.y <= pPlayer->pos.y + 30.0f)
		{
			if (g_F4 == false)
			{
				int nDamage = 1;

				pPlayer->LimitSpin--;	//跳ね返し回数を減少

				if (pBullet->Reflect == REFLECTBULLET_YES)
				{
					nDamage = nDamage + nDamage;
					pPlayer->LimitSpin--;	//跳ね返し回数を減少
				}

				//上限を超えたら当たり判定復活
				if (pPlayer->LimitSpin <= ZERO)
				{
					//回転値を０に
					pPlayer->rot.z = F_ZERO;

					HitPlayer(nDamage);
					PlaySound(SOUND_LABEL_SE_HIT);	//ヒット音を鳴らす
				}

				//跳ね返し
				else
				{
					//飛んできた方向へ跳ね返し
					pBullet->move *= -1.0f;

					//弾の寿命を回復
					pBullet->nLife = REFRECT_BULLET_HP;

					//弾の情報を塗り替え
					pBullet->type = BULLETTYPE_PLAYER;
					pBullet->homingtype = HOMING_IMPOSSIBLE;
					pBullet->Reflect = REFLECTBULLET_YES;
					pBullet->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

					PlaySound(SOUND_LABEL_SE_REFLECT);	//跳ね返し音を鳴らす
				}
			}
		}
	}

	//当たり判定
	else if (pPlayer->state == PLAYERSTATE_NORMAL && pPlayer->bDisp == true)
	{
		//敵の弾との当たり判定
		if (pBullet->pos.x >= pPlayer->pos.x - 30.0f &&
			pBullet->pos.x <= pPlayer->pos.x + 30.0f &&
			pBullet->pos.y >= pPlayer->pos.y - 30.0f &&
			pBullet->pos.y <= pPlayer->pos.y + 30.0f)
		{
			//爆発の設定
			if (g_F4 == false)
			{
				HitPlayer(1);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
			pBullet->bUse = false;
		}
	}
}