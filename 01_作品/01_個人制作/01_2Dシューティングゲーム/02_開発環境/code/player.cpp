/*==========================================================================================================================

												プレイヤーの処理[player.cpp]
													Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "game.h"
#include "explosion.h"
#include "enemy.h"
#include "beam.h"
#include "particle.h"

//マクロ定義
#define PLAYER_POS			D3DXVECTOR3(640.0f, 450.0f, F_ZERO)
#define PLAYER_MOVE			D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO)
#define PLAYER_ROT			D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO)
#define PLAYER_TEX			"data/TEXTURE/自機.png"
#define PLAYER_LENGTH		(60.0f)		//プレイヤーの大きさ
#define COLLISION_PLAYER	(30.0f)		//プレイヤーの当たり判定
#define COLLISION_SIZE		(1.31f)		//当たり判定の倍率
#define PLAYER_HP			(5)			//体力
#define DAMAGE_TIME			(60)		//ダメージ状態の時間
#define MOVE_SPIN			(-18.0f)	//スピンの回転値
#define MOVE_SPEED			(1.0f)		//プレイヤーの移動量
#define ADD_SPIN			(0.2f)		//回転の加算値
#define LIMIT_SPIN			(8)			//跳ね返し上限
#define SPIN_INTERVAL		(100)		//スピンのインターバル
#define APPEARCNT			(120)		//出現時の点滅時間

#define MOVE_LIMIT_R		(1240.0f)	//プレイヤーの画面端移動限界　右
#define MOVE_LIMIT_L		(40.0f)		//プレイヤーの画面端移動限界　左
#define MOVE_LIMIT_CEILING	(60.0f)		//プレイヤーの画面端移動限界　上
#define MOVE_LIMIT_FLOOR	(680.0f)	//プレイヤーの画面端移動限界　下

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTexturePlayer;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
Player g_player;

//プレイヤーの初期化処理
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//プレイヤー情報の初期化
	{
		g_player.pos = PLAYER_POS;								//位置を初期化する
		g_player.move = PLAYER_MOVE;							//移動量を初期化する
		g_player.rot = PLAYER_ROT;								//向きを初期化する(Z値を使用)
		g_player.myBullet = BULLETTYPE_PLAYER;					//弾の種類を設定
		g_player.homingtype = HOMING_IMPOSSIBLE;				//ホーミング性能の初期化（最初はナシ)
		g_player.LimitSpin = LIMIT_SPIN;						//跳ね返しの上限
		g_player.SpinInterval = ZERO;							//スピンインターバルの初期化
		g_player.nLife = PLAYER_HP;								//体力初期化
		g_player.state = PLAYERSTATE_APPEAR;					//状態初期化
		g_player.PlayerSwitch = ONE;							//点滅表示カウンタ初期化
		g_player.AppearCnt = ZERO;								//点滅表示時間カウンタ初期化
		g_player.nCounterState = ZERO;							//状態管理カウンタ初期化
		g_player.bDisp = true;									//表示ON/OFF(最初はON)
		g_player.fLength = sqrtf(PLAYER_LENGTH * PLAYER_LENGTH + PLAYER_LENGTH * PLAYER_LENGTH) * F_FIVE;	//対角線の長さを算出する
		g_player.fAngle = atan2f(PLAYER_LENGTH, PLAYER_LENGTH);	//対角線の角度を算出する
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &g_pTexturePlayer);
		
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[ZERO].pos.x = g_player.pos.x + sinf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ZERO].pos.y = g_player.pos.y + cosf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ZERO].pos.z = ORIGIN_POS;
		pVtx[ONE].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.z = ORIGIN_POS;
		pVtx[SEC].pos.x = g_player.pos.x + sinf(g_player.rot.z + (ORIGIN_POS - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.y = g_player.pos.y + cosf(g_player.rot.z + (ORIGIN_POS - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.z = ORIGIN_POS;
		pVtx[SAN].pos.x = g_player.pos.x + sinf(g_player.rot.z + (ORIGIN_POS + g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.y = g_player.pos.y + cosf(g_player.rot.z + (ORIGIN_POS + g_player.fAngle)) * g_player.fLength;
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
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
	}

	//テクスチャの座標
	{
		pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		pVtx[ONE].tex = D3DXVECTOR2(MAX_POS, ORIGIN_POS);
		pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, MAX_POS);
		pVtx[SAN].tex = D3DXVECTOR2(MAX_POS   , MAX_POS);
	}

	//頂点バッファをロックする
	g_pVtxBuffPlayer->Unlock();
}

//プレイヤーの終了処理
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//プレイヤーの更新処理
void UpdatePlayer(void)
{
	if (GetKeyboardPress(DIK_D) == true)
	{//Dキーが押された
		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された
			g_player.move.x += sinf(D3DX_PI * UP_RIGHT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * UP_RIGHT) * MOVE_SPEED;
		}

		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された
		 //右下入力
			g_player.move.x += sinf(D3DX_PI * DOWN_RIGHT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * DOWN_RIGHT) * MOVE_SPEED;
		}

		else
		{
			g_player.move.x += sinf(D3DX_PI * RIGHT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * RIGHT) * MOVE_SPEED;
		}
	}

	else if (GetKeyboardPress(DIK_A) == true)
	{//Aキーが押された
		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された
		 //左上入力
			g_player.move.x += sinf(D3DX_PI * UP_LEFT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * UP_LEFT) * MOVE_SPEED;
		}

		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された
		 //左下入力
			g_player.move.x += sinf(D3DX_PI * DOWN_LEFT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * DOWN_LEFT) * MOVE_SPEED;
		}

		else
		{
			g_player.move.x += sinf(D3DX_PI * LEFT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * LEFT) * MOVE_SPEED;
		}
	}

	else if (GetKeyboardPress(DIK_W) == true)
	{//Wキーが押された
		g_player.move.y += cosf(-D3DX_PI * UP) * MOVE_SPEED;
	}

	else if (GetKeyboardPress(DIK_S) == true)
	{//Sキーが押された
		g_player.move.y += cosf(D3DX_PI * DOWN) * MOVE_SPEED;
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//位置を更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;

	//移動量を更新(減衰させる)
	g_player.move.x += (F_ZERO - g_player.move.x) * F_ONE;
	g_player.move.y += (F_ZERO - g_player.move.y) * F_ONE;
	Enemy *pEnemy = GetEnemy();

	//プレイヤーの状態情報分岐
	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_player.AppearCnt++;

		//プレイヤーを点滅させる
		if (g_player.AppearCnt % FOUR == ZERO)
		{
			g_player.PlayerSwitch = g_player.PlayerSwitch ^ ONE;
		}

		//頂点カラーの設定
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX * g_player.PlayerSwitch);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX * g_player.PlayerSwitch);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX * g_player.PlayerSwitch);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX * g_player.PlayerSwitch);
		}

		if (g_player.AppearCnt >= APPEARCNT)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
		break;

	case PLAYERSTATE_NORMAL:
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			if (g_player.nLife > 0)
			{
				SetBullet(D3DXVECTOR3(g_player.pos.x - 5.0f, g_player.pos.y - 20.0f, g_player.pos.z), D3DXVECTOR3(sinf(g_player.rot.z + (D3DX_PI * 1.0f)) * 6.0f, cosf(g_player.rot.z + (D3DX_PI * 1.0f)) * 6.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), g_player.myBullet, g_player.homingtype, BULLETEFFECT_PLAYER);
				SetBullet(D3DXVECTOR3(g_player.pos.x + 5.0f, g_player.pos.y - 20.0f, g_player.pos.z), D3DXVECTOR3(sinf(g_player.rot.z + (D3DX_PI * 1.0f)) * 6.0f, cosf(g_player.rot.z + (D3DX_PI * 1.0f)) * 6.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), g_player.myBullet, g_player.homingtype, BULLETEFFECT_PLAYER);
			}
		}

		if (g_player.SpinInterval > ZERO && g_player.state == PLAYERSTATE_NORMAL)
		{
			g_player.SpinInterval--;

			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, ZERO, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, ZERO, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, ZERO, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, ZERO, COLOR_MAX);
		}
		else
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		if (GetKeyboardPress(DIK_SPACE) == true && g_player.SpinInterval <= ZERO)
		{
			//状態をスピン状態に(攻撃不能)
			g_player.state = PLAYERSTATE_SPIN;

			//跳ね返す上限を設定
			g_player.LimitSpin = LIMIT_SPIN;

			//向きを更新(Z値を回転させる)
			g_player.rot.z = MOVE_SPIN;
		}

		//敵とプレイヤーの当たり判定
		for (int nCntEnemy = START_CNT; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				//敵とプレイヤーの距離とそれぞれの半径の長さの合計
				float pos = sqrtf((g_player.pos.x - pEnemy->pos.x) * (g_player.pos.x - pEnemy->pos.x) + (g_player.pos.y - pEnemy->pos.y) * (g_player.pos.y - pEnemy->pos.y)) * COLLISION_SIZE,
					len = pEnemy->fLength + g_player.fLength;

				if (len >= pos)
				{
					HitPlayer(ONE);
				}
			}
		}
		break;

	case PLAYERSTATE_SPIN:
		if (g_player.rot.z <= F_ZERO)
		{
			g_player.rot.z += ADD_SPIN;

			if (g_player.rot.z > F_ZERO)
			{
				//状態を普通に
				g_player.state = PLAYERSTATE_NORMAL;
				g_player.rot.z = F_ZERO;
				g_player.SpinInterval = SPIN_INTERVAL;
			}
		}
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;

		//赤く点滅させる
		if (g_player.nCounterState % FIVE == ZERO)
		{
			//画面に移すかの判定
			g_player.bDisp = g_player.bDisp ? false : true;
		}

		//ダメージ状態の時間がゼロになった
		if (g_player.nCounterState <= ZERO)
		{
			g_player.state = PLAYERSTATE_NORMAL;		//プレイヤーの状態をノーマルに治す
			g_player.bDisp = true;						//自分を画面に移す

			//色を元に戻す
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		}

		break;

	case PLAYERSTATE_DEATH:
		break;
	}

	//頂点座標の設定
	{
		pVtx[ZERO].pos.x = g_player.pos.x + sinf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ZERO].pos.y = g_player.pos.y + cosf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[ONE].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.x = g_player.pos.x + sinf(g_player.rot.z + (F_ZERO - g_player.fAngle)) * g_player.fLength;
		pVtx[SEC].pos.y = g_player.pos.y + cosf(g_player.rot.z + (F_ZERO - g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.x = g_player.pos.x + sinf(g_player.rot.z + (F_ZERO + g_player.fAngle)) * g_player.fLength;
		pVtx[SAN].pos.y = g_player.pos.y + cosf(g_player.rot.z + (F_ZERO + g_player.fAngle)) * g_player.fLength;
	}

	//画面端処理
	{
		//横端処理
		{
			//画面端なら、押し戻す
			if (g_player.pos.x > MOVE_LIMIT_R)
			{//右端
				g_player.pos.x = MOVE_LIMIT_R;
			}

			if (g_player.pos.x < MOVE_LIMIT_L)
			{//左端
				g_player.pos.x = MOVE_LIMIT_L;
			}
		}

		//縦端処理
		{
			//画面端なら、押し戻す
			if (g_player.pos.y > MOVE_LIMIT_FLOOR)
			{//底面
				g_player.pos.y = MOVE_LIMIT_FLOOR;
			}

			if (g_player.pos.y < MOVE_LIMIT_CEILING)
			{//天井
				g_player.pos.y = MOVE_LIMIT_CEILING;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//プレイヤーの描画処理
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffPlayer, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.bDisp == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTexturePlayer);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
	}
}

//プレイヤーの取得
Player *GetPlayer(void)
{
	return &g_player;
}

//プレイヤーのヒット処理
void HitPlayer(int nDamage)
{
	g_player.nLife -= nDamage;	

	if (g_player.nLife <= ZERO)
	{
		g_player.nLife = ZERO;
		SetExplosion(g_player.pos, D3DXCOLOR(F_TEN, F_TEN, F_TEN, F_TEN));
		//ゲーム状態の設定
		SetGameState(GAMESTATE_END, END_FADE);
		g_player.state = PLAYERSTATE_DEATH;
		g_player.bDisp = false;
	}

	else
	{
		//ポインタを設定
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = DAMAGE_TIME;

		//頂点カラーの設定
		{
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, ZERO, ZERO, COLOR_MAX);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, ZERO, ZERO, COLOR_MAX);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, ZERO, ZERO, COLOR_MAX);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, ZERO, ZERO, COLOR_MAX);
		}

		//頂点バッファをロックする
		g_pVtxBuffPlayer->Unlock();
	}
}