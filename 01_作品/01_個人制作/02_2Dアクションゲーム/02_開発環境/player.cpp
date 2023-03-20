/*==========================================================================================================================

プレイヤーの処理[player.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "player.h"
#include "particle.h"
#include "fade.h"
#include "hako.h"
#include "input.h"
#include "gravity.h"

//マクロ定義
//プレイヤーの情報マクロ
#define PLAYER_POS				D3DXVECTOR3(300.0f, 0.0f, F_ZERO)
#define PLAYER_MOVE				D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO)
#define PLAYER_TEX				"data/TEXTURE/プレイヤーデータ.png"
#define MOVE_MAX_TEX			(10)		//歩きモーションのテクスチャ最大数
#define PLAYER_WIDTH			(35.0f)		//プレイヤーの幅(中心から左右に伸びるので端から端の半分)
#define PLAYER_HEIGHT			(75.0f)		//プレイヤーの高さ
#define COLLISION_PLAYER		(5.0f)		//プレイヤーの当たり判定調整マクロ 横
#define COLLISION_HEIGHT		(40.0f)		//プレイヤーの当たり判定調整マクロ 縦
#define COLLISION_SIZE			(1.31f)		//当たり判定の倍率
#define FALL_DEATH_POS			(720.0f)	//落ちた時のパーティクルが発生するY座標
#define BIRTH_PARTICLE_PLAYER	(20)		//パーティクルの発生量

//その他のマクロ
#define MOVE_SPEED				(0.8f)		//移動量
#define DEATH_TIME				(60)		//死亡の時間
#define APPEARCNT				(200)		//出現演出時間
#define MOVE_LIMIT_R			(800.0f)	//プレイヤーの画面端移動限界　右
#define MOVE_LIMIT_L			(0.0f)		//プレイヤーの画面端移動限界　左
#define MOVE_LIMIT_CEILING		(0.0f)		//プレイヤーの画面端移動限界　天井
#define MOVE_LIMIT_FLOOR		(800.0f)	//プレイヤーの画面端移動限界　床

//アニメーションの構造体
typedef enum
{
	PLAYER_ANIM_NORMAL = ZERO,	//通常時
	PLAYER_ANIM_JUMP,			//ジャンプ
	PLAYER_ANIM_DELIVERY,		//産み出し
	PLAYER_ANIM_SLEEP,			//眠る
	PLAYER_ANIM_SIT,			//座る or 目覚め
	PLAYER_ANIM_HAPPY,			//嬉し顔
	PLAYER_ANIM_DAMAGE,			//ダメージ顔
}PLAYERANIM;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTexturePlayer;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
Player g_player;

//プレイヤーの初期化処理
void InitPlayer(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プレイヤー情報の初期化
	g_player.pos = PLAYER_POS;					//現在の位置を初期化する
	g_player.posOld = PLAYER_POS;				//前回の位置を初期化
	g_player.move = PLAYER_MOVE;				//移動量を初期化する
	g_player.state = PLAYERSTATE_APPEAR;		//状態初期化
	g_player.AppearCnt = APPEARCNT;				//点滅表示時間カウンタ初期化
	g_player.Rot = PLAYER_ROT_FORWARD;			//プレイヤーの向き初期化(正面
	g_player.OldRot = PLAYER_ROT_FORWARD;		//プレイヤーの前回の向き初期化(正面
	g_player.nCounterAnim = ZERO;				//アニメーションカウンター初期化
	g_player.nPatternAnim = PLAYER_ANIM_SLEEP;	//アニメーションパターン初期化
	g_player.nCounterState = ZERO;				//状態管理カウンタ初期化
	g_player.DeathCounter = ZERO;				//死亡カウント
	g_player.bJump = false;						//ジャンプをしていない状態に
	g_player.bOnBlock = false;					//ブロックに乗っていない
	g_player.bOnHako = false;					//ハコに乗っていない
	g_player.bSideBlock = false;				//ブロックにぶつかっていない
	g_player.bDisp = true;						//表示ON/OFF(最初はON)
	g_player.fWidth = PLAYER_WIDTH;				//プレイヤーの幅を設定する
	g_player.fHeight = PLAYER_HEIGHT;			//プレイヤーの高さを設定する
	g_player.pBlock = NULL;						//対象ブロックへのアドレスはNULL	

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
		/*pVtx[ZERO].pos.x = g_player.pos.x + sinf(g_player.rot.z + (-D3DX_PI + g_player.fAngle)) * g_player.fLength;
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
		pVtx[SAN].pos.z = ORIGIN_POS;*/

		pVtx[ZERO].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y - g_player.fHeight, ORIGIN_POS);
		pVtx[ONE].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth,  g_player.pos.y - g_player.fHeight, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth,  g_player.pos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth,  g_player.pos.y, ORIGIN_POS);
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
		pVtx[ONE].tex = D3DXVECTOR2(0.1f, ORIGIN_POS);
		pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, 0.25f);
		pVtx[SAN].tex = D3DXVECTOR2(0.1f, 0.25f);
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

//プレイヤーの更新で最初に行う処理
void FirstUpdatePlayer(void)
{
	g_player.posOld = g_player.pos;		//前回の位置を記憶
	g_player.bOnBlock = false;			//ブロックに乗っていない
	g_player.bOnHako = false;			//ハコに乗っていない
}

//プレイヤーの更新処理
void UpdatePlayer(void)
{
	//最初に行う更新処理
	FirstUpdatePlayer();

	if (g_player.pBlock != NULL)
	{//ブロックが移動した分だけプレイヤーに加算する
		g_player.move.x += (g_player.pBlock->pos.x - g_player.pBlock->posOld.x) * 0.1f;
	}

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//プレイヤーの移動処理
	MovePlayer();

	//ジャンプ中じゃないときにハコを生み出したり、ジャンプしたり
	if (g_player.bJump == false)
	{
		//ハコを生み出す
		if ((GetKeyboardPress(DIK_RETURN) == true || GetGamePadPress(XINPUT_GAMEPAD_X) == true) 
			&& g_player.state != PLAYERSTATE_CARRY && g_player.state != PLAYERSTATE_APPEAR && g_player.state != PLAYERSTATE_GOAL && g_player.state != PLAYERSTATE_THROW)
		{
			if (g_player.Rot != PLAYER_ROT_FORWARD)
			{
				g_player.OldRot = g_player.Rot;			//前回の向き更新
			}

			g_player.state = PLAYERSTATE_DELIVERY;		//ハコ産み出し状態に
			g_player.Rot = PLAYER_ROT_FORWARD;			//正面を向く

			//ハコの生成
			SetHako();
			
			g_player.nPatternAnim = PLAYER_ANIM_DELIVERY;
		}

		else
		{
			FixHakoCol();
		}

		//スペースキーが押された
		if ((GetKeyboardPress(DIK_SPACE) == true || GetGamePadPress(XINPUT_GAMEPAD_A) == true) && g_player.state != PLAYERSTATE_APPEAR)
		{
			g_player.bJump = true;							//ジャンプ中にする
			g_player.move.y = JUMP_MOVE;					//飛び上がらせる
		}

		//しゃがむ
		if ((GetKeyboardPress(DIK_S) == true || GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN) == true) && g_player.bJump == false && g_player.state != PLAYERSTATE_DELIVERY)
		{
			if (g_player.Rot != PLAYER_ROT_FORWARD)
			{
				g_player.OldRot = g_player.Rot;			//前回の向き更新
			}

			g_player.Rot = PLAYER_ROT_FORWARD;			//正面向く

			g_player.nPatternAnim = PLAYER_ANIM_SIT;
		}

		//前回の向きに直す
		if (GetKeyboardRelease(DIK_S) == true || GetKeyboardRelease(DIK_RETURN) == true || 
			GetGamePadRelease(XINPUT_GAMEPAD_DPAD_DOWN) == true || GetGamePadRelease(XINPUT_GAMEPAD_X) == true)
		{
			g_player.Rot = g_player.OldRot;		
		}
	}
	
	//移動量を更新(減衰させる)
	g_player.move.x += (F_ZERO - g_player.move.x) * 0.2f;

	//重力を常に加える
	CalculationGravity(&g_player.move);

	//Y座標の位置更新と当たり判定
	CollisionAndPos_y();

	//X座標の位置更新と当たり判定
	CollisionAndPos_x();

	//ブロックにもハコにも乗っていない
	if(g_player.bOnBlock == false && g_player.bOnHako == false)
	{
		g_player.bJump = true;		//空中に浮いているためジャンプ状態にする
		g_player.nCounterAnim = ZERO;
		g_player.nPatternAnim = PLAYER_ANIM_JUMP;
	}

	else
	{
		//自分が動いているときにアニメーションカウンターを増やす
		if (fabsf(g_player.move.x) >= F_ONE)
		{
			g_player.nCounterAnim++;
		}
	}

	//プレイヤーの状態情報分岐
	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_player.AppearCnt--;
		g_player.nPatternAnim = PLAYER_ANIM_SLEEP;

		//目覚ましアニメーション
		if ((g_player.AppearCnt < 40 && g_player.AppearCnt >= 35) || g_player.AppearCnt < 20)
		{
			g_player.nPatternAnim = PLAYER_ANIM_SIT;
		}

		//飛び起きアニメーション
		if (g_player.AppearCnt == ZERO)
		{
			g_player.nPatternAnim = PLAYER_ANIM_JUMP;
			g_player.bJump = true;							//ジャンプ中にする
			g_player.move.y = JUMP_MOVE;					//飛び上がらせる
		}

		//着地したら操作開始
		else if(g_player.AppearCnt < ZERO)
		{
			g_player.nPatternAnim = PLAYER_ANIM_JUMP;

			if (g_player.bJump == false)
			{
				g_player.state = PLAYERSTATE_NORMAL;
			}
		}
		break;

	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_THROW:
		if (GetKeyboardRelease(DIK_RETURN) == true || GetGamePadRelease(XINPUT_GAMEPAD_X) == true)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nPatternAnim = PLAYER_ANIM_DAMAGE;
		if (g_player.move.y > F_ZERO)
		{
			SetParticle(g_player.pos, BIRTH_PARTICLE_PLAYER);
			g_player.state = PLAYERSTATE_DEATH;
			g_player.DeathCounter = DEATH_TIME;
		}
		break;

	case PLAYERSTATE_FALL:
		SetParticle(D3DXVECTOR3(g_player.pos.x , FALL_DEATH_POS, g_player.pos.z), BIRTH_PARTICLE_PLAYER);
		g_player.state = PLAYERSTATE_DEATH;
		g_player.DeathCounter = DEATH_TIME;
		break;

	case PLAYERSTATE_DEATH:
		g_player.DeathCounter--;
		g_player.bDisp = false;

		if (g_player.DeathCounter <= ZERO)
		{
			SetFade(MODE_GAME);
		}
		break;

	case PLAYERSTATE_GOAL:
		g_player.nPatternAnim = PLAYER_ANIM_HAPPY;
		g_player.Rot = PLAYER_ROT_FORWARD;
		break;
	}

	//アニメーション設定
	if (g_player.nCounterAnim == 8)
	{
		g_player.nCounterAnim = ZERO;	//アニメーションカウンターをゼロにする
		g_player.nPatternAnim++;		//アニメーションパターン更新

		//テクスチャパターンが総数を超えた
		if (g_player.nPatternAnim >= MOVE_MAX_TEX)
		{//テクスチャ座標を初期値に戻す
			g_player.nPatternAnim = ZERO;
		}
	}

	//プレイヤーの向き(テクスチャ座標)の設定
	switch (g_player.Rot)
	{
	case PLAYER_ROT_FORWARD:
		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS + g_player.nPatternAnim * 0.1f, 0.0f);
			pVtx[ONE].tex = D3DXVECTOR2(0.1f        + g_player.nPatternAnim * 0.1f, 0.0f);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS  + g_player.nPatternAnim * 0.1f, 0.25f);
			pVtx[SAN].tex = D3DXVECTOR2(0.1f        + g_player.nPatternAnim * 0.1f, 0.25f);
		}
		break;

	case PLAYER_ROT_RIGHT:
	case PLAYER_ROT_LEFT:
		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS + g_player.nPatternAnim * 0.1f, ((float)g_player.Rot * 0.25f));
			pVtx[ONE].tex = D3DXVECTOR2(0.1f + g_player.nPatternAnim * 0.1f,        ((float)g_player.Rot * 0.25f));
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS + g_player.nPatternAnim * 0.1f,  ((float)g_player.Rot * 0.25f) + 0.25f);
			pVtx[SAN].tex = D3DXVECTOR2(0.1f + g_player.nPatternAnim * 0.1f,        ((float)g_player.Rot * 0.25f) + 0.25f);
		}
		break;
	}

	//画面端処理
	{
		//横端処理
		{
			//画面端なら、押し戻す
			if (g_player.pos.x > MOVE_LIMIT_R)
			{//右端
				g_player.pos.x = MOVE_LIMIT_R;
				//g_player.move.x = F_ZERO;
			}

			if (g_player.pos.x - g_player.fWidth + COLLISION_PLAYER < MOVE_LIMIT_L)
			{//左端
				g_player.pos.x = MOVE_LIMIT_L + g_player.fWidth - COLLISION_PLAYER;
			}
		}

		//縦端処理
		{
			//天井を超える
			if (g_player.pos.y < MOVE_LIMIT_CEILING)
			{//底面
				g_player.pos.y = MOVE_LIMIT_CEILING;
				g_player.move = ZERO_SET;
			}

			//地面に着地
			if (g_player.pos.y > MOVE_LIMIT_FLOOR && g_player.state != PLAYERSTATE_FALL && g_player.state != PLAYERSTATE_DEATH && g_player.state != PLAYERSTATE_DAMAGE)
			{//底面
				g_player.state = PLAYERSTATE_FALL;
			}
		}
	}

	//頂点座標の設定
	{
		pVtx[ZERO].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y - g_player.fHeight, ORIGIN_POS);
		pVtx[ONE].pos  = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y - g_player.fHeight, ORIGIN_POS);
		pVtx[SEC].pos  = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y, ORIGIN_POS);
		pVtx[SAN].pos  = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y, ORIGIN_POS);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//プレイヤーの描画処理
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice= GetDevice();			//デバイスへのポインタ

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
void HitPlayer(PLAYERSTATE state)
{
	if (g_player.state != PLAYERSTATE_DAMAGE && g_player.state != PLAYERSTATE_FALL && g_player.state != PLAYERSTATE_DEATH)
	{
		//ゲーム状態の設定
		g_player.state = state;
		g_player.nPatternAnim = PLAYER_ANIM_DAMAGE;
		g_player.Rot = PLAYER_ROT_FORWARD;

		if (state ==  PLAYERSTATE_DAMAGE)
		{//痛くて飛び上がった感じを演出
			g_player.move.x *= (float)MINUS_ONE;
			g_player.move.y = JUMP_MOVE;
		}
	}
	
}

void MovePlayer(void)
{
	//プレイヤーの状態が動ける場合
	if ((g_player.state == PLAYERSTATE_NORMAL || g_player.state == PLAYERSTATE_CARRY || g_player.state == PLAYERSTATE_THROW)
		&& GetKeyboardPress(DIK_RETURN) == false && GetGamePadPress(XINPUT_GAMEPAD_X) == false &&
		GetKeyboardPress(DIK_S) == false && GetGamePadPress(XINPUT_GAMEPAD_DPAD_DOWN) == false)
	{
		//左に移動
		if (GetKeyboardPress(DIK_D) == true || GetGamePadPress(XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{//Dキーが押された
			g_player.move.x += sinf(D3DX_PI * RIGHT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * RIGHT) * MOVE_SPEED;

			//右向きに
			g_player.Rot = PLAYER_ROT_RIGHT;
		}

		//右に移動
		else if (GetKeyboardPress(DIK_A) == true || GetGamePadPress(XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{//Aキーが押された

			g_player.move.x += sinf(D3DX_PI * LEFT) * MOVE_SPEED;
			g_player.move.y += cosf(D3DX_PI * LEFT) * MOVE_SPEED;

			//左向きに
			g_player.Rot = PLAYER_ROT_LEFT;
		}

		//動いていない
		else
		{//テクスチャ座標やアニメーションカウンターをゼロにする
			g_player.move.x = F_ZERO;
			g_player.nCounterAnim = ZERO;
			g_player.nPatternAnim = ZERO;
		}
	}
}

//横の当たり判定と座標更新
void CollisionAndPos_x(void)
{
	//X座標更新
	g_player.pos.x += g_player.move.x;

	//ブロックとの当たり判定　横
	g_player.bSideBlock = X_CollisionBlockToPlayer(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fWidth - COLLISION_PLAYER, g_player.fHeight, &g_player.pBlock);

	//ハコとの当たり判定 横
	X_CollisionHako();
}

//縦の当たり判定と座標更新
void CollisionAndPos_y(void)
{
	//Y更新を更新
	g_player.pos.y += g_player.move.y;

	//ブロックとの当たり判定 + ジャンプできるかどうか
	if (Y_CollisionBlockToPlayer(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fWidth - COLLISION_PLAYER, g_player.fHeight, &g_player.pBlock) == true)
	{
		g_player.bJump = false;				//着地状態にする

		g_player.bOnBlock = true;			//ブロックに乗っている
	}

	//ハコとの当たり判定
	if (Y_CollisionHako() == true)
	{
		g_player.bJump = false;		//着地状態にする

		g_player.bOnHako = true;	//ハコに乗っている
	}
}