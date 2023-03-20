/*==========================================================================================================================

												敵出現　＆　当たり判定処理[bullet.cpp]
														   Author:平澤詩苑

==========================================================================================================================*/
#include "enemy.h"
#include "bullet.h"
#include "beam.h"
#include "explosion.h"
#include "score.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "particle.h"
#include "input.h"
#include "boss.h"
#include <stdlib.h>
#include <string.h>

//マクロ定義
#define ENEMY_TYPE		(4)								//敵の種類設定
#define ENEMY_HP		(4)								//敵の体力の設定
#define SCORE_DEFO		(1000)							//スコアの基準値
#define HOMING			(20)							//敵がホーミング弾を有する確率計算に使用
#define ENEMY_SIZE		(30.0f)							//敵のサイズ
#define ENEMY_LENGTH	ENEMY_SIZE * ENEMY_SIZE			//敵の対角線の長さ計算に使用
#define STANDARD_POS	(100.0f)						//敵が左右移動するY座標
#define ST_POS_DIS		(10.0f)							//スタンダードポジションの間隔
#define OUTSIDE_FALL	(1.0f)							//画面外で降下する速度
#define FALL_APPEAR		(1.6f)							//敵が画面内に居て発射可能状態になるまで降下する速度
#define FALL_ENEMY		(0.6f)							//敵が画面内で降下する速度
#define RETURN_ENEMY	(8.0f)							//敵が左に折り返す速度
#define TURN_LEFT_SIDE	(80.0f)							//右に折り返し
#define TURN_RIGHT_SIDE	(1200.0f)						//左に折り返し

//弾の速さのマクロ
#define ENEMY_BULLET_SPEED_NORMAL	(3.0f)				//普通・三連弾を打つ敵の弾の速さ
#define ENEMY_BULLET_SPEED_SPEED	(5.0f)				//倍速弾を打つ敵の弾の速さ
#define ENEMY_BULLET_SPEED_BEAM		(8.0f)				//ビーム弾を打つ敵の弾の速さ

#define MOJI			(300)							//読み取る文字の上限数
#define ENEMY_INFO		"data/敵の配置情報.csv"			//敵の情報が書かれたファイル名
#define ENEMY_TEX		"data/TEXTURE/enemy000.png"		//敵の画像

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureEnemy = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;		//頂点バッファへのポインタ
Enemy g_aEnemy[MAX_ENEMY];							//敵の情報
DWORD g_StartTime;									//敵出現の時間
int g_nNumEnemy;									//敵の総数

//敵の初期化処理
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//ランダム関数のシードを時刻より設定
	srand((unsigned int)time(NULL));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEX, &g_pTextureEnemy);

	//乱数調整用の変数さんたち
	float Stpos = 0.0f;
	FILE *pFile;
	char aData[MOJI], *f;

	pFile = fopen(ENEMY_INFO, "r");

	//敵の情報の初期化
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);

		for (int nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);

			f = strtok(&aData[ZERO], ",");		//設定文字

			Stpos = (float)(rand() % HOMING) * ST_POS_DIS + STANDARD_POS;		//スタンダードポジションの初期化
			g_aEnemy[nCntEnemy].Standardpos = Stpos;	//位置の初期化

			g_aEnemy[nCntEnemy].pos.x = atof(strtok(NULL, ","));		//出現位置の初期化
			g_aEnemy[nCntEnemy].pos.y = atof(strtok(NULL, ","));		//出現位置の初期化
			g_aEnemy[nCntEnemy].pos.z = F_ZERO;							//出現位置の初期化
			g_aEnemy[nCntEnemy].nLife = atoi(strtok(NULL, ","));		//ライフ初期化
			g_aEnemy[nCntEnemy].nLifeMax = g_aEnemy[nCntEnemy].nLife;	//ライフ初期化
			g_aEnemy[nCntEnemy].nType = atoi(strtok(NULL, ","));		//種類初期化
			g_aEnemy[nCntEnemy].move.x = atof(strtok(NULL, ","));		//敵の移動量の初期化
			g_aEnemy[nCntEnemy].move.y = atof(strtok(NULL, ","));		//敵の移動量の初期化
			g_aEnemy[nCntEnemy].enBullet = BULLETTYPE_ENEMY;			//弾の種類の初期化
			g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;				//攻撃ターンの初期化
			g_aEnemy[nCntEnemy].nCounterState = ZERO;					//状態管理カウンターの初期化
			g_aEnemy[nCntEnemy].Return = F_ZERO;						//移動変数の減算値の初期化
			g_aEnemy[nCntEnemy].bUse = false;							//使用していない状態にする
			g_aEnemy[nCntEnemy].fLength = sqrtf(ENEMY_LENGTH + ENEMY_LENGTH) * F_SEV;	//対角線の長さ
			g_aEnemy[nCntEnemy].fAngle = atan2f(ENEMY_SIZE, ENEMY_SIZE);				//対角線の計算

			//ホーミング性能アリ　ビーム弾を打つ敵以外に付与
			if (Stpos == STANDARD_POS && g_aEnemy[nCntEnemy].nType != SAN)
			{
				g_aEnemy[nCntEnemy].homingtype = HOMING_POSSIBLE;
			}
			//ホーミング性能ナシ
			else
			{
				g_aEnemy[nCntEnemy].homingtype = HOMING_IMPOSSIBLE;
			}
		}

		fclose(pFile);
	}

	g_nNumEnemy = ZERO;				//敵の総数をクリア

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		{
			pVtx[ZERO].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(-D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[ZERO].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(-D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[ZERO].pos.z = ORIGIN_POS;
			pVtx[ONE].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[ONE].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[ONE].pos.z = ORIGIN_POS;
			pVtx[SEC].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(ORIGIN_POS - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[SEC].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(ORIGIN_POS - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[SEC].pos.z = ORIGIN_POS;
			pVtx[SAN].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(ORIGIN_POS + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[SAN].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(ORIGIN_POS + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[SAN].pos.z = ORIGIN_POS;
		}

		//rhwの設定
		{
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;
		}

		//頂点カラーの設定
		{
			pVtx[ZERO].col = D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS);
			pVtx[ONE].col = D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS);
			pVtx[SEC].col = D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS);
			pVtx[SAN].col = D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(MAX_POS, ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, MAX_POS);
			pVtx[SAN].tex = D3DXVECTOR2(MAX_POS   , MAX_POS);
		}

		pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
	}

	//ゲーム開始時間の時間
	g_StartTime = timeGetTime();

	//頂点バッファをロックする
	g_pVtxBuffEnemy->Unlock();
}

//敵の終了処理
void UninitEnemy(void)
{
	//テクスチャの破棄
	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//敵の更新処理
void UpdateEnemy(void)
{
	int nCntEnemy;
	int	BulletSound = ZERO, BeamSound = ZERO;		//弾とビーム音のサウンドカウンタ
	float EnemyBulletAngle;
	Player *pPlayer = GetPlayer();
	DWORD SetEnemyTime = timeGetTime();		//敵配置の時間

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//敵が降下
			if (g_aEnemy[nCntEnemy].pos.y < g_aEnemy[nCntEnemy].Standardpos)
			{
				if (g_aEnemy[nCntEnemy].pos.y < ORIGIN_POS)
				{
					g_aEnemy[nCntEnemy].pos.y += OUTSIDE_FALL;
				}
				else
				{
					g_aEnemy[nCntEnemy].pos.y += FALL_APPEAR;
				}
			}

			//左右移動や弾の発射など
			else
			{
				//敵の弾発射処理
				if (pPlayer->state != PLAYERSTATE_APPEAR)
				{
					if (g_aEnemy[nCntEnemy].pos.y >= STANDARD_POS)
					{
						g_aEnemy[nCntEnemy].enemyAtackCounter++;
						//敵の弾を発射
						//敵の左側にプレイヤー
						if (g_aEnemy[nCntEnemy].pos.x - 100.0f > pPlayer->pos.x)
						{
							//左上にプレイヤー
							if (g_aEnemy[nCntEnemy].pos.y - 100.0f > pPlayer->pos.y)
							{
								EnemyBulletAngle = -0.75f;
							}

							//左下にプレイヤー
							else if (g_aEnemy[nCntEnemy].pos.y + 100.0f < pPlayer->pos.y)
							{
								EnemyBulletAngle = -0.25f;
							}

							//敵と同じくらいのX座標にプレイヤー
							else
							{
								EnemyBulletAngle = -0.5f;
							}
						}

						//敵の右側にプレイヤー
						else if (g_aEnemy[nCntEnemy].pos.x + 100.0f < pPlayer->pos.x)
						{
							//右上にプレイヤー
							if (g_aEnemy[nCntEnemy].pos.y - 100.0f > pPlayer->pos.y)
							{
								EnemyBulletAngle = 0.75f;
							}

							//右下にプレイヤー
							else if (g_aEnemy[nCntEnemy].pos.y + 100.0f < pPlayer->pos.y)
							{
								EnemyBulletAngle = 0.25f;
							}

							//敵と同じくらいのX座標にプレイヤー
							else
							{
								EnemyBulletAngle = 0.5f;
							}
						}

						//敵と同じくらいのY座標にプレイヤー
						else
						{
							if (g_aEnemy[nCntEnemy].pos.y + 30.0f < pPlayer->pos.y)
							{
								EnemyBulletAngle = 0.0f;
							}
							else
							{
								EnemyBulletAngle = 1.0f;
							}

						}						

						//敵の種類による弾の発射間隔変化
						switch (g_aEnemy[nCntEnemy].nType)
						{
						case 0:		//通常速度の発射	赤色
							if (g_aEnemy[nCntEnemy].enemyAtackCounter % BULLETINTERVAL_NORMAL == ZERO)
							{
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_NORMAL, cosf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_NORMAL, ORIGIN_POS), D3DXCOLOR(MAX_POS, ORIGIN_POS, ORIGIN_POS, MAX_POS), g_aEnemy[nCntEnemy].enBullet, g_aEnemy[nCntEnemy].homingtype, BULLETEFFECT_ENEMY);
								BulletSound = ONE;
								g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;
							}

							//頂点カラーの設定( オレンジ )
							{
								pVtx[0].col = D3DXCOLOR(MAX_POS, F_SEC, F_FIVE, MAX_POS);
								pVtx[1].col = D3DXCOLOR(MAX_POS, F_SEC, F_FIVE, MAX_POS);
								pVtx[2].col = D3DXCOLOR(MAX_POS, F_SEC, F_FIVE, MAX_POS);
								pVtx[3].col = D3DXCOLOR(MAX_POS, F_SEC, F_FIVE, MAX_POS);
							}
							break;

						case 1:		//通常の倍の速度の発射	水色
							if (g_aEnemy[nCntEnemy].enemyAtackCounter % BULLETINTERVAL_SPEED == ZERO)
							{
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_SPEED, cosf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_SPEED, ORIGIN_POS), D3DXCOLOR(ORIGIN_POS, MAX_POS, MAX_POS, MAX_POS), g_aEnemy[nCntEnemy].enBullet, g_aEnemy[nCntEnemy].homingtype, BULLETEFFECT_FAST_ENEMY);
								BulletSound = ONE;
								g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;
							}

							//頂点カラーの設定( 青 )
							{
								pVtx[0].col = D3DXCOLOR(ORIGIN_POS, ORIGIN_POS, MAX_POS, MAX_POS);
								pVtx[1].col = D3DXCOLOR(ORIGIN_POS, ORIGIN_POS, MAX_POS, MAX_POS);
								pVtx[2].col = D3DXCOLOR(ORIGIN_POS, ORIGIN_POS, MAX_POS, MAX_POS);
								pVtx[3].col = D3DXCOLOR(ORIGIN_POS, ORIGIN_POS, MAX_POS, MAX_POS);
							}
							break;

						case 2:		//3連弾発射		白色
							if (g_aEnemy[nCntEnemy].enemyAtackCounter == BULLETINTERVAL_RUSH1 || g_aEnemy[nCntEnemy].enemyAtackCounter == BULLETINTERVAL_RUSH2 || g_aEnemy[nCntEnemy].enemyAtackCounter == BULLETINTERVAL_RUSH3)
							{
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_NORMAL, cosf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_NORMAL, ORIGIN_POS), D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS), g_aEnemy[nCntEnemy].enBullet, g_aEnemy[nCntEnemy].homingtype, BULLETEFFECT_ENEMY);
								BulletSound = ONE;
								if (g_aEnemy[nCntEnemy].enemyAtackCounter == BULLETINTERVAL_RUSH3)
								{
									g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;
								}
							}

							//頂点カラーの設定( 緑 )
							{
								pVtx[0].col = D3DXCOLOR(ORIGIN_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[1].col = D3DXCOLOR(ORIGIN_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[2].col = D3DXCOLOR(ORIGIN_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[3].col = D3DXCOLOR(ORIGIN_POS, MAX_POS, ORIGIN_POS, MAX_POS);
							}
							break;

						case 3:		//ビーム発射		緑色
							if (g_aEnemy[nCntEnemy].enemyAtackCounter % BULLETINTERVAL_BEAM == ZERO)
							{
								SetBeam(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, D3DX_PI * EnemyBulletAngle), D3DXVECTOR3(sinf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_BEAM, cosf(D3DX_PI * EnemyBulletAngle) * ENEMY_BULLET_SPEED_BEAM, ORIGIN_POS), D3DXCOLOR(MAX_POS, MAX_POS, MAX_POS, MAX_POS));

								BeamSound = ONE;
								g_aEnemy[nCntEnemy].enemyAtackCounter = ZERO;
							}

							//頂点カラーの設定( 紫 )
							{
								pVtx[0].col = D3DXCOLOR(MAX_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[1].col = D3DXCOLOR(MAX_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[2].col = D3DXCOLOR(MAX_POS, MAX_POS, ORIGIN_POS, MAX_POS);
								pVtx[3].col = D3DXCOLOR(MAX_POS, MAX_POS, ORIGIN_POS, MAX_POS);
							}
							break;
						}
					}
				}

				//敵の移動移動
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

				//降下
				g_aEnemy[nCntEnemy].pos.y += FALL_ENEMY;

				//敵が画面外に行ってしまった
				if (g_aEnemy[nCntEnemy].pos.y >= SCREEN_HEIGHT || g_aEnemy[nCntEnemy].pos.x <= ORIGIN_POS || g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH)
				{
					g_nNumEnemy--;						//敵の総数を減らす
					g_aEnemy[nCntEnemy].bUse = false;	//敵を未使用状態に設定

					if (g_nNumEnemy <= ZERO)
					{
						//ボスを召喚
						SetBoss();
					}
				}

				//ホーミング性能アリなら敵の色を虹色に変える
				if (g_aEnemy[nCntEnemy].homingtype == HOMING_POSSIBLE && g_aEnemy[nCntEnemy].state == ENEMYSTATE_NORMAL)
				{
					//頂点カラーの設定
					{
						pVtx[0].col = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f);
					}
				}
			}

			//敵のダメージ状態遷移
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= ZERO)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					//ポリゴンを通常色に
					//頂点カラーの設定
					{
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}

				break;
			}

			//頂点座標の設定
			{
				pVtx[ZERO].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(-D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[ZERO].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(-D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[ONE].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[ONE].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[SEC].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(ORIGIN_POS - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[SEC].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(ORIGIN_POS - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[SAN].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(ORIGIN_POS + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
				pVtx[SAN].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(ORIGIN_POS + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			}
		}
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffEnemy->Unlock();

	if (BulletSound == ONE)
	{
		PlaySound(SOUND_LABEL_SE_SHOT);
		BulletSound = 0;
	}

	if (BeamSound == ONE)
	{
		PlaySound(SOUND_LABEL_BEAM_SHOT);
		BeamSound = 0;
	}
}

//敵の描画処理
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEnemy);

		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * NUM_VERTEX, SEC);
		}
	}
}

//敵の設定処理
void SetEnemy()
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が使用されていない

			//敵情報の設定
			g_aEnemy[nCntEnemy].enBullet = BULLETTYPE_ENEMY;	//弾の種類の設定

			//頂点情報の設定
			//頂点座標の設定
			{
				pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - ENEMY_VERTEX;
				pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - ENEMY_VERTEX;
				pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + ENEMY_VERTEX;
				pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - ENEMY_VERTEX;
				pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - ENEMY_VERTEX;
				pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + ENEMY_VERTEX;
				pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + ENEMY_VERTEX;
				pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + ENEMY_VERTEX;
			}

			g_aEnemy[nCntEnemy].bUse = true;			//使用している状態にする
			g_nNumEnemy++;
			break;
		}
		pVtx += NUM_VERTEX;
	}
	//頂点バッファをロックする
	g_pVtxBuffEnemy->Unlock();
}

//敵の取得
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];		//敵の情報の先頭アドレスを返す
}

//敵のヒット処理
void HitEnemy(int nCntEnemy, int nDamage, int nType, REFLECTBULLET type)
{
	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);	//パーティクルを５回発生
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);
		SetParticle(g_aEnemy[nCntEnemy].pos,  D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.8f), PARTICLETYPE_MOVE);
		g_nNumEnemy--;
		g_aEnemy[nCntEnemy].bUse = false;
		
		//リフレクトボーナス
		int BonusScore = 1;	
		if (type == REFLECTBULLET_YES)
		{
			BonusScore++;
		}

		AddScore(SCORE_DEFO * nType * nDamage * BonusScore);

		if (g_nNumEnemy <= 0)
		{
			//ボスを召喚
			SetBoss();
		}
	}

	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		//ダメージ色設定
		
		VERTEX_2D *pVtx;

		//頂点アンロック
		g_pVtxBuffEnemy->Lock(0, 0, (void* *)&pVtx, 0);

		pVtx += nCntEnemy * NUM_VERTEX;

		//頂点カラーの設定
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}

		//頂点バッファをロックする
		g_pVtxBuffEnemy->Unlock();
	}
}

//敵の総数を返す
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}