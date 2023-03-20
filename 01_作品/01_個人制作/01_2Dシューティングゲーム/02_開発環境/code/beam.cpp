/*==========================================================================================================================

											ビーム発射処理[beam.cpp]
												Author:平澤詩苑

==========================================================================================================================*/
#include "enemy.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "effect.h"
#include "particle.h"
#include "boss.h"
#include "beam.h"

//マクロ定義
#define MAX_BEAM			(256)		//ビームの最大数
#define FCORRECTION			(1.0f)		//ホーミング弾の補正係数の数値
#define FHOMINGMOVE			(0.3f)		//ホーミング弾の移動量
#define FANGLE_CORRECT		(6.28f)		//角度の修正に使用
#define BEAM_HEIGHT			(100.0f)	//ビームの高さ
#define BEAM_WIDTH			(30.0f)		//ビームの幅
#define BEAM_DAMAGE			(2)			//ビームの攻撃力

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureBeam = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBeam = NULL;		//頂点バッファへのポインタ
Beam g_aBeam[MAX_BEAM];							//ビームの情報
float g_fLengthBeam;				//対角線の長さ
float g_fAngleBeam;					//対角線の角度
bool g_f4;							//無敵ON/OFF

//ビームの初期化処理
void InitBeam(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntBeam;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/beam.png", &g_pTextureBeam);

	//弾の情報の初期化
	for (nCntBeam = START_CNT; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		g_aBeam[nCntBeam].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
		g_aBeam[nCntBeam].move = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
		g_aBeam[nCntBeam].rot = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
		g_aBeam[nCntBeam].col = D3DXCOLOR(F_ZERO, F_ZERO, F_ZERO, F_ZERO);
		g_aBeam[nCntBeam].type = BULLETTYPE_ENEMY;
		g_aBeam[nCntBeam].Reflect = REFLECTBULLET_NO;			//跳ね返されていない状態に設定
		g_aBeam[nCntBeam].bUse = false;							//使用していない状態にする
	}

	g_f4 = false;					//無敵ON/OFFの初期化

	//対角線の長さを算出する
	g_fLengthBeam = sqrtf(BEAM_WIDTH * BEAM_WIDTH + BEAM_HEIGHT * BEAM_HEIGHT) * F_ATE;

	//対角線の角度を算出する
	g_fAngleBeam = atan2f(BEAM_WIDTH, BEAM_HEIGHT);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BEAM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBeam, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBeam->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBeam = START_CNT; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		//頂点座標の設定
		{
			pVtx[ZERO].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
			pVtx[ONE].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
			pVtx[SEC].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
			pVtx[SAN].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
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
			pVtx[ZERO].col = D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN);
			pVtx[ONE].col = D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN);
			pVtx[SEC].col = D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN);
			pVtx[SAN].col = D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , F_TEN);
		}

		pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffBeam->Unlock();
}

//ビームの終了処理
void UninitBeam(void)
{
	//テクスチャの破棄
	if (g_pTextureBeam != NULL)
	{
		g_pTextureBeam->Release();
		g_pTextureBeam = NULL;
	}

	//頂点座標の破棄
	if (g_pVtxBuffBeam != NULL)
	{
		g_pVtxBuffBeam->Release();
		g_pVtxBuffBeam = NULL;
	}
}

//ビームの更新処理
void UpdateBeam(void)
{
	int nCntBeam;
	int DeadCount = ZERO;

	//プレイヤーの無敵ON/OFF
	if (GetKeyboardTrigger(DIK_F4) == true)
	{
		g_f4 = g_f4 ? false : true;
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBeam->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBeam = ZERO; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		if (g_aBeam[nCntBeam].bUse == true)
		{//弾が使用されている
		 //弾の種類による当たり判定
			switch (g_aBeam[nCntBeam].type)
			{
			case BULLETTYPE_PLAYER:
				//敵の取得
				CollisionEnemy_Beam(&g_aBeam[nCntBeam]);
				break;

			case BULLETTYPE_ENEMY:
			case BULLETTYPE_BOSS:
				//プレイヤーの取得
				CollisionPlayer_Beam(&g_aBeam[nCntBeam]);
				break;
			}

			//弾の位置の更新
			g_aBeam[nCntBeam].pos.x += g_aBeam[nCntBeam].move.x;
			g_aBeam[nCntBeam].pos.y += g_aBeam[nCntBeam].move.y;

			//頂点座標の設定
			{
				pVtx[ZERO].pos.x = g_aBeam[nCntBeam].pos.x + sinf(g_aBeam[nCntBeam].rot.z + (-D3DX_PI + g_fAngleBeam)) * g_fLengthBeam;
				pVtx[ZERO].pos.y = g_aBeam[nCntBeam].pos.y + cosf(g_aBeam[nCntBeam].rot.z + (-D3DX_PI + g_fAngleBeam)) * g_fLengthBeam;
				pVtx[ONE].pos.x = g_aBeam[nCntBeam].pos.x + sinf(g_aBeam[nCntBeam].rot.z + ( D3DX_PI - g_fAngleBeam)) * g_fLengthBeam;
				pVtx[ONE].pos.y = g_aBeam[nCntBeam].pos.y + cosf(g_aBeam[nCntBeam].rot.z + ( D3DX_PI - g_fAngleBeam)) * g_fLengthBeam;
				pVtx[SEC].pos.x = g_aBeam[nCntBeam].pos.x + sinf(g_aBeam[nCntBeam].rot.z + (F_ZERO - g_fAngleBeam)) * g_fLengthBeam;
				pVtx[SEC].pos.y = g_aBeam[nCntBeam].pos.y + cosf(g_aBeam[nCntBeam].rot.z + (F_ZERO - g_fAngleBeam)) * g_fLengthBeam;
				pVtx[SAN].pos.x = g_aBeam[nCntBeam].pos.x + sinf(g_aBeam[nCntBeam].rot.z + (F_ZERO + g_fAngleBeam)) * g_fLengthBeam;
				pVtx[SAN].pos.y = g_aBeam[nCntBeam].pos.y + cosf(g_aBeam[nCntBeam].rot.z + (F_ZERO + g_fAngleBeam)) * g_fLengthBeam;
			}

			pVtx[ZERO].col = g_aBeam[nCntBeam].col;
			pVtx[ONE].col = g_aBeam[nCntBeam].col;
			pVtx[SEC].col = g_aBeam[nCntBeam].col;
			pVtx[SAN].col = g_aBeam[nCntBeam].col;

			//弾が画面外に出た場合
			if (g_aBeam[nCntBeam].pos.y >  730.0f || g_aBeam[nCntBeam].pos.y < 0.0f
			 || g_aBeam[nCntBeam].pos.x > 1290.0f || g_aBeam[nCntBeam].pos.x < 0.0f)
			{
				g_aBeam[nCntBeam].bUse = false;		//使用していない状態にする
			}

			//ビームが使用されなくなった後の初期化
			if (g_aBeam[nCntBeam].bUse == false)
			{
				pVtx[ZERO].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
				pVtx[ONE].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
				pVtx[SEC].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
				pVtx[SAN].pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);
			}
		}

		//頂点データのポインタを4つ分進める
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBeam->Unlock();
}

//ビームの描画処理
void DrawBeam(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntBeam;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffBeam, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureBeam);

	for (nCntBeam = START_CNT; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		if (g_aBeam[nCntBeam].bUse == true)
		{//弾が使用されている
		 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBeam * NUM_VERTEX, SEC);
		}
	}
}

//弾の設定処理
void SetBeam(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col)
{
	int nCntBeam;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBeam->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBeam = START_CNT; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		if (g_aBeam[nCntBeam].bUse == false)
		{//弾が使用されていない

			//弾の情報を設定
			g_aBeam[nCntBeam].pos.x = pos.x;
			g_aBeam[nCntBeam].pos.y = pos.y;
			g_aBeam[nCntBeam].rot.z = pos.z;	//SetBeamを読んだキャラの角度がpos.zに入っている
			g_aBeam[nCntBeam].type = BULLETTYPE_ENEMY;
			g_aBeam[nCntBeam].col = col;
			g_aBeam[nCntBeam].Reflect = REFLECTBULLET_NO;

			pVtx[ZERO].col = col;
			pVtx[ONE].col = col;
			pVtx[SEC].col = col;
			pVtx[SAN].col = col;

			g_aBeam[nCntBeam].move.x = move.x;
			g_aBeam[nCntBeam].move.y = move.y;
			g_aBeam[nCntBeam].move.z = F_ZERO;

			g_aBeam[nCntBeam].bUse = true;			//使用している状態にする
			break;
		}
		//頂点データのポインタを4つ分進める
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをロックする
	g_pVtxBuffBeam->Unlock();
}

//敵の当たり判定
void CollisionEnemy_Beam(Beam *pBeam)
{
	Enemy *pEnemy = GetEnemy();
	BOSS *pBoss = GetBoss();
	int nCntEnemy;
	int nDamage = BEAM_DAMAGE;		//攻撃力

	//跳ね返された弾ならダメージ２倍
	if (pBeam->Reflect == REFLECTBULLET_YES)
	{
		nDamage = nDamage + nDamage;
	}

	for (nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されている
			if (pBeam->pos.x >= pEnemy->pos.x - 30.0f &&
				pBeam->pos.x <= pEnemy->pos.x + 30.0f &&
				pBeam->pos.y >= pEnemy->pos.y - 30.0f &&
				pBeam->pos.y <= pEnemy->pos.y + 30.0f)
			{
				HitEnemy(nCntEnemy, nDamage, pEnemy->nLifeMax, pBeam->Reflect);

				if (pEnemy->nLife > ONE)
				{
					SetParticle(pBeam->pos, D3DXCOLOR(F_TEN, F_TEN, F_ZERO, F_ATE), PARTICLETYPE_STAY);
				}

				PlaySound(SOUND_LABEL_SE_HIT);
			}
		}
	}

	if (pBoss->state == BOSSSTATE_SPIN && pBeam->type == BULLETTYPE_PLAYER)
	{
		//ビーム弾とボスとの当たり判定
		if (pBeam->pos.x >= pBoss->pos.x - 50.0f &&
			pBeam->pos.x <= pBoss->pos.x + 50.0f &&
			pBeam->pos.y >= pBoss->pos.y - 20.0f &&
			pBeam->pos.y <= pBoss->pos.y + 20.0f)
		{
			//飛んできた方向へ跳ね返し
			pBeam->move *= -1.0f;

			//弾の情報を塗り替え
			pBeam->type = BULLETTYPE_BOSS;
			pBeam->Reflect = REFLECTBULLET_YES;
			pBeam->col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		}
	}

	//ボスが使われている & 当たり判定
	else if (pBeam->pos.x >= pBoss->pos.x - 50.0f &&
			 pBeam->pos.x <= pBoss->pos.x + 50.0f &&
			 pBeam->pos.y >= pBoss->pos.y - 20.0f &&
			 pBeam->pos.y <= pBoss->pos.y + 20.0f && pBoss->bDisp == true && pBoss->state == BOSSSTATE_NORMAL)
	{
		HitBoss(nDamage, pBeam->Reflect);

		if (pBoss->nLife > 1)
		{
			SetParticle(pBeam->pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f), PARTICLETYPE_STAY);
		}
	}
}

//プレイヤーの当たり判定
void CollisionPlayer_Beam(Beam *pBeam)
{
	Player *pPlayer = GetPlayer();

	//スピン中
	if (pPlayer->state == PLAYERSTATE_SPIN && pBeam->type != BULLETTYPE_PLAYER)
	{
		//敵の弾との当たり判定
		if (pBeam->pos.x >= pPlayer->pos.x - BEAM_WIDTH &&
			pBeam->pos.x <= pPlayer->pos.x + BEAM_WIDTH &&
			pBeam->pos.y >= pPlayer->pos.y - BEAM_HEIGHT &&
			pBeam->pos.y <= pPlayer->pos.y + BEAM_HEIGHT)
		{
			if (g_f4 == false)
			{
				pPlayer->LimitSpin--;	//跳ね返し回数を減少

				//上限を超えたら当たり判定復活
				if (pPlayer->LimitSpin <= 0)
				{
					//回転値を０に
					pPlayer->rot.z = 0.0f;

					HitPlayer(BEAM_DAMAGE);
					PlaySound(SOUND_LABEL_SE_HIT);
				}

				//跳ね返し
				else
				{
					//飛んできた方向へ跳ね返し
					pBeam->move *= -1.0f;

					//弾の情報を塗り替え
					pBeam->type = BULLETTYPE_PLAYER;
					pBeam->Reflect = REFLECTBULLET_YES;
					if (pBeam->rot.z <= 0.0f)
					{
						pBeam->rot.z = pBeam->rot.z + D3DX_PI;
					}
					else
					{
						pBeam->rot.z = pBeam->rot.z - D3DX_PI;
					}

					pBeam->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				}
			}
		}
	}

	//当たり判定
	else if (pPlayer->state == PLAYERSTATE_NORMAL && pPlayer->bDisp == true)
	{
		//敵の弾との当たり判定
		if (pBeam->pos.x >= pPlayer->pos.x - BEAM_WIDTH &&
			pBeam->pos.x <= pPlayer->pos.x + BEAM_WIDTH &&
			pBeam->pos.y >= pPlayer->pos.y - BEAM_HEIGHT &&
			pBeam->pos.y <= pPlayer->pos.y + BEAM_HEIGHT)
		{
			//爆発の設定
			if (g_f4 == false)
			{
				HitPlayer(BEAM_DAMAGE);
				PlaySound(SOUND_LABEL_SE_HIT);
			}
			pBeam->bUse = false;
		}
	}
}