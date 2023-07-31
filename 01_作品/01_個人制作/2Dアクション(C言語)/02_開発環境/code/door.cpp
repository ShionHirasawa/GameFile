/*==========================================================================================================================

ゴール(ステージ入り口)のドアの処理[door.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "door.h"
#include "player.h"
#include "move.h"
#include "input.h"
#include "fade.h"

//マクロ定義
#define NUM_DOOR		(1)										//扉の数
#define DOOR_POS		(D3DXVECTOR3(4800.0f,430.0f, 0.0f))	//扉の位置
#define DOOR_WIDTH		(150.0f)								//ドアの横幅
#define DOOR_HEIGHT		(100.0f)								//ドアの高さ

typedef struct
{
	D3DXVECTOR3 pos;	//ドアの位置
	float TexDoor;		//ドアのテクスチャ座標
	int nCounterAnim;	//アニメーションカウンタ
	int nJumpCnt;		//ジャンプの回数
	bool bOpen;			//ドアが開いているか
}Door;

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureDoor = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffDoor = NULL;	//頂点バッファへのポインタ
Door g_Door;										//ドアの情報

//扉の初期化処理
void InitDoor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ + 取得

	//ドアの情報初期化
	g_Door.pos = DOOR_POS;			//ドアの座標
	g_Door.TexDoor = F_ZERO;		//ドアは閉じている
	g_Door.nCounterAnim = ZERO;		//アニメーションカウンターゼロ
	g_Door.nJumpCnt = ZERO;			//ジャンプしていない(プレイヤーが)
	g_Door.bOpen = false;			//ドアが開いていない

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/door.png", &g_pTextureDoor);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_DOOR, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffDoor, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDoor->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[ZERO].pos = g_Door.pos;
		pVtx[ONE].pos = D3DXVECTOR3(g_Door.pos.x + DOOR_WIDTH,   g_Door.pos.y, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_Door.pos.x,  DOOR_HEIGHT + g_Door.pos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_Door.pos.x + DOOR_WIDTH,   DOOR_HEIGHT + g_Door.pos.y, ORIGIN_POS);
	}

	//rhwの設定
	{
		pVtx[ZERO].rhw = RHW_NUM;
		pVtx[ONE].rhw = RHW_NUM;
		pVtx[SEC].rhw = RHW_NUM;
		pVtx[SAN].rhw = RHW_NUM;
	}

	//頂点カラーの設定   COLOR_MIN
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX);
	}

	//テクスチャ座標
	{
		pVtx[ZERO].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[ONE]. tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[SEC]. tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[SAN]. tex = D3DXVECTOR2(0.25f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffDoor->Unlock();
}

//扉の終了処理
void UninitDoor(void)
{
	//テクスチャの破棄
	if (g_pTextureDoor != NULL)
	{
		g_pTextureDoor->Release();
		g_pTextureDoor = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffDoor != NULL)
	{
		g_pVtxBuffDoor->Release();
		g_pVtxBuffDoor = NULL;
	}
}

//扉の更新処理
void UpdateDoor(void)
{
	Player *pPlayer = GetPlayer();		//プレイヤーの情報

	if (g_Door.bOpen == true)
	{
		g_Door.bOpen = true;

		g_Door.nCounterAnim++;

		pPlayer->nPatternAnim = 5;

		if (g_Door.nCounterAnim == 5)
		{
			g_Door.nCounterAnim = ZERO;

			if (g_Door.TexDoor < 0.75f)
			{
				g_Door.TexDoor += 0.25f;
			}

			if (g_Door.TexDoor >= 0.75f && pPlayer->bJump == false && g_Door.nJumpCnt >= 3)
			{
				g_Door.bOpen = false;

				SetFade(MODE_GAME);
			}
		}

		//プレイヤーを３回ジャンプさせる
		if (pPlayer->bJump == false && g_Door.nJumpCnt < 3)
		{
			pPlayer->move.y = JUMP_MOVE;
			pPlayer->bJump = true;
			pPlayer->state = PLAYERSTATE_GOAL;
			g_Door.nJumpCnt++;

		}
	}

	if (pPlayer->state == PLAYERSTATE_APPEAR)
	{
		g_Door.pos = DOOR_POS;
	}

	//ドアに入る
	EnterDoor();
	
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDoor->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//座標を更新
	moveObject(&g_Door.pos);

	//頂点座標の設定
	{
		pVtx[ZERO].pos = g_Door.pos;
		pVtx[ONE].pos = D3DXVECTOR3(g_Door.pos.x + DOOR_WIDTH, g_Door.pos.y, ORIGIN_POS);
		pVtx[SEC].pos = D3DXVECTOR3(g_Door.pos.x,  DOOR_HEIGHT + g_Door.pos.y, ORIGIN_POS);
		pVtx[SAN].pos = D3DXVECTOR3(g_Door.pos.x + DOOR_WIDTH,   DOOR_HEIGHT + g_Door.pos.y, ORIGIN_POS);
	}

	//テクスチャ座標
	{
		pVtx[ZERO].tex = D3DXVECTOR2(g_Door.TexDoor, 0.0f);
		pVtx[ONE].tex = D3DXVECTOR2(g_Door.TexDoor + 0.25f, 0.0f);
		pVtx[SEC].tex = D3DXVECTOR2(g_Door.TexDoor, 1.0f);
		pVtx[SAN].tex = D3DXVECTOR2(g_Door.TexDoor + 0.25f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffDoor->Unlock();
}

//扉の描画処理
void DrawDoor(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ + 取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffDoor, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureDoor);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}

//ドアに入る
void EnterDoor(void)
{
	Player *pPlayer = GetPlayer();		//プレイヤーの情報

	if (pPlayer->pos.x >= g_Door.pos.x && pPlayer->pos.x <= g_Door.pos.x + DOOR_WIDTH && pPlayer->bJump == false)
	{
		if ((GetKeyboardTrigger(DIK_W) == true || GetGamePadTrigger(XINPUT_GAMEPAD_DPAD_UP)) && pPlayer->state != PLAYERSTATE_GOAL)
		{
			//プレイヤーの設定
			pPlayer->state = PLAYERSTATE_GOAL;
			pPlayer->Rot = PLAYER_ROT_FORWARD;
			pPlayer->nPatternAnim = 5;
			pPlayer->move.y = JUMP_MOVE;
			pPlayer->bJump = true;

			//ドアの設定
			g_Door.TexDoor = F_ZERO;		//ドアのテクスチャは閉じた状態から
			g_Door.nCounterAnim = ZERO;		//アニメーションカウンターはゼロ
			g_Door.nJumpCnt = ONE;			//ジャンプ回数は１
			g_Door.bOpen = true;			//ドアを開ける
		}
	}
}