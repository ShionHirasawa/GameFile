/*==========================================================================================================================================================

プレイヤーの位置プレビュー処理[preview.h]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "hdr_player.h"
#include "debugproc.h"
#include "preview.h"
#include "block.h"
#include "color.h"
#include "input.h"

typedef enum
{
	PREVIEW_UI_COURSE = 0,
	PREVIEW_UI_PLAYER,
	PREVIEW_UI_MAX
}PREVIEW_UI;

//マクロ定義
#define NUM_PREVIEW			(4)		//プレビュー数
#define ALL_PREBIEW			NUM_PREVIEW * PREVIEW_UI_MAX	//全体プレビュー数
#define PREVIEW_TEX_COURSE	"data/TEXTURE/HDR_Course.png"	//コースUI
#define PREVIEW_TEX_PLAYER	"data/TEXTURE/PlayerUI000.png"	//プレイヤーUI
#define PREVIEW_HOR_SEP		(SCREEN_WIDTH / 4)	//縦４分割
#define PREVIEW_POS_Y		(570.0f)			//配置する高さ

#define PREVIEW_COURSE_MARGIN_WIDTH	(70.0f)		//コースUIの左にずらす余白
#define PREVIEW_COURSE_WIDTH		(60.0f)		//コースUIの幅
#define PREVIEW_COURSE_HEIGHT		(370.0f)	//コースUIの高さ

#define PREVIEW_PLAYER_UI_FIX_Y			(359.0f)//プレイヤーUIの初期高さ
#define PREVIEW_PLAYER_MARGIN_WIDTH		(34.0f)	//プレイヤーUIの左にずらす余白
#define PREVIEW_PLAYER_MARGIN_HEIGHT	(5.0f)	//プレイヤーUIの上にずらす余白（コースのUIの下が少し浮いているので、その調整用

#define PREVIEW_PLAYER_SIZE			(20.0f)		//プレイヤーUIのサイズ（正方形
#define PREVIEW_PLAYER_TEX_WIDTH	(0.25f)		//プレイヤーUIのテクスチャ幅（４分割

#define PLAYER_START_HEIGHT	(MAX_BLOCK * COLLISION_SIZE_Y)	//プレイヤーのスタート地点


//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPreview = NULL;				//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTexturePreview[PREVIEW_UI_MAX] = {};	//テクスチャへのポインタ
Preview					g_Preview[NUM_PREVIEW];					//UI情報

//--------------------------------------------
//			位置プレビュー初期化処理
//--------------------------------------------
void InitPreview(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PREVIEW_TEX_COURSE, &g_pTexturePreview[PREVIEW_UI_COURSE]);
	D3DXCreateTextureFromFile(pDevice, PREVIEW_TEX_PLAYER, &g_pTexturePreview[PREVIEW_UI_PLAYER]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * ALL_PREBIEW, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPreview, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPreview->Lock(0, 0, (void* *)&pVtx, 0);

	//プレビュー情報のポインタ取得
	Preview *pPreview = &g_Preview[0];

	//頂点情報の設定
	for (int nCntVtx = 0; nCntVtx < NUM_PREVIEW; nCntVtx++, pVtx += VTX_MAX * PREVIEW_UI_MAX, pPreview++)
	{
		// 位置初期化
		pPreview->pos = D3DXVECTOR3(
			(float)(PREVIEW_HOR_SEP + PREVIEW_HOR_SEP * nCntVtx), //画面を縦に４等分して、その分割点から左に少しずらす
			PREVIEW_POS_Y, 0.0f);
		pPreview->fPlayerUIHeight = PREVIEW_PLAYER_UI_FIX_Y;	//プレイヤーUIの表示する高さ
		pPreview->fHeightPer = 1.0f;							//プレイヤーUIの表示する高さのパーセンテージ

		/*******************************************************************
							コースとプレイヤーUIを同時に設定
			MEMO:コースのポインタ番号０～３　プレイヤーのポインタ番号４～７
		*********************************************************************/
		//=================================================================
		//			コースの頂点座標・テクスチャ座標の設定
		//=================================================================
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pPreview->pos.x - PREVIEW_COURSE_WIDTH - PREVIEW_COURSE_MARGIN_WIDTH, pPreview->pos.y - PREVIEW_COURSE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pPreview->pos.x + PREVIEW_COURSE_WIDTH - PREVIEW_COURSE_MARGIN_WIDTH, pPreview->pos.y - PREVIEW_COURSE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pPreview->pos.x - PREVIEW_COURSE_WIDTH - PREVIEW_COURSE_MARGIN_WIDTH, pPreview->pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pPreview->pos.x + PREVIEW_COURSE_WIDTH - PREVIEW_COURSE_MARGIN_WIDTH, pPreview->pos.y, 0.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//=================================================================
		//			プレイヤーUIの頂点座標・テクスチャ座標の設定
		//=================================================================
		//頂点座標の設定
		pVtx[4].pos = D3DXVECTOR3(pPreview->pos.x - PREVIEW_PLAYER_SIZE - PREVIEW_PLAYER_MARGIN_WIDTH, (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_SIZE * 2.0f - PREVIEW_PLAYER_MARGIN_HEIGHT, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(pPreview->pos.x + PREVIEW_PLAYER_SIZE - PREVIEW_PLAYER_MARGIN_WIDTH, (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_SIZE * 2.0f - PREVIEW_PLAYER_MARGIN_HEIGHT, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(pPreview->pos.x - PREVIEW_PLAYER_SIZE - PREVIEW_PLAYER_MARGIN_WIDTH, (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_MARGIN_HEIGHT, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(pPreview->pos.x + PREVIEW_PLAYER_SIZE - PREVIEW_PLAYER_MARGIN_WIDTH, (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_MARGIN_HEIGHT, 0.0f);

		//テクスチャの幅
		float aTexU = PREVIEW_PLAYER_TEX_WIDTH * nCntVtx;

		//テクスチャ座標
		pVtx[4].tex = D3DXVECTOR2(aTexU, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(aTexU + PREVIEW_PLAYER_TEX_WIDTH, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(aTexU, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(aTexU + PREVIEW_PLAYER_TEX_WIDTH, 1.0f);

		//=================================================================
		//	両方の RHW・頂点カラーは設定が同じなので、一緒に設定
		//=================================================================
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = pVtx[4].rhw = pVtx[5].rhw = pVtx[6].rhw = pVtx[7].rhw = RHW;		//rhwの設定
		pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = pVtx[4].col = pVtx[5].col = pVtx[6].col = pVtx[7].col = XCOL_WHITE;	//頂点カラーの設定
	}

	//頂点バッファをロックする
	g_pVtxBuffPreview->Unlock();
}

//------------------------------------------
//			プレビュー終了処理
//------------------------------------------
void UninitPreview(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < PREVIEW_UI_MAX; nCntTex++)
	{
		if (g_pTexturePreview[nCntTex] != NULL)
		{
			g_pTexturePreview[nCntTex]->Release();
			g_pTexturePreview[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPreview != NULL)
	{
		g_pVtxBuffPreview->Release();
		g_pVtxBuffPreview = NULL;
	}
}

//--------------------------------------
//			プレビュー更新処理
//--------------------------------------
void UpdatePreview(void)
{
	//プレビュー情報のポインタを取得
	Preview *pPreview = &g_Preview[0];

	//ポインタを設定
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPreview->Lock(0, 0, (void* *)&pVtx, 0);

	//一つ目のコースの頂点バッファを飛ばす
	pVtx += VTX_MAX;

	for (int nCntPreview = 0; nCntPreview < NUM_PREVIEW; nCntPreview++, pVtx += VTX_MAX * PREVIEW_UI_MAX, pPreview++)
	{
		//プレイヤーUIの表示する高さを算出
		FixPlayerUIHeight(nCntPreview);

		//頂点座標の設定
		float fUp = pVtx[0].pos.y = pVtx[1].pos.y = (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_SIZE * 2.0f - PREVIEW_PLAYER_MARGIN_HEIGHT;	//プレイヤーUIの上辺
		float fDo = pVtx[2].pos.y = pVtx[3].pos.y = (pPreview->pos.y - pPreview->fPlayerUIHeight) - PREVIEW_PLAYER_MARGIN_HEIGHT;								//プレイヤーUIの底辺

		PrintDebugProc("上辺の位置：%f    底辺の位置：%f\n", fUp, fDo);
	}

	//頂点バッファをロックする
	g_pVtxBuffPreview->Unlock();
}

//プレイヤーUIの表示する高さを修正
void FixPlayerUIHeight(int nNumPreview)
{
	//プレイヤーの現在の高さの割合を求める
	float fPercent = GetPlayer_HDR()[nNumPreview].pos.y / PLAYER_START_HEIGHT;
	g_Preview[nNumPreview].fPlayerUIHeight = fPercent * PREVIEW_PLAYER_UI_FIX_Y;

	PrintDebugProc("%dPの割合：%f  最大の高さ：%f  現在の高さ：%f    ", nNumPreview + 1, fPercent, PREVIEW_PLAYER_UI_FIX_Y, g_Preview[nNumPreview].fPlayerUIHeight);
}

//--------------------------------------
//			プレビュー描画処理
//--------------------------------------
void DrawPreview(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Player_HDR *pPlayer = GetPlayer_HDR();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPreview, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntPreview = 0; nCntPreview < ALL_PREBIEW; nCntPreview++)
	{
		if ((pPlayer + (nCntPreview / PREVIEW_UI_MAX))->bGoal == false)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePreview[nCntPreview % EVENPARITY]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPreview * VTX_MAX, 2);
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}