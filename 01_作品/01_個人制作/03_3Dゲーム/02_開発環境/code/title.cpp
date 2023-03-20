/*==========================================================================================================================================================

														タイトル画面処理[title.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "title.h"
#include "color.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

#define NUM_TITLE_TEX	(3)									// タイトルに使う画像の数
#define TITLE_INFOFILE	"data/CSV/title.csv"				// タイトルの情報が入ったファイル名

#define TITLE_POS		(D3DXVECTOR3(NIL_F, NIL_F, NIL_F))	//タイトルの位置
#define TITLE_WIDTH		(510.0f)							//タイトルの幅
#define TITLE_HEIGHT	(215.0f)							//タイトルの高さ

//タイトルの情報構造体
typedef struct
{
	D3DXVECTOR2 pos;		//位置
	float		fWidth;		//幅
	float		fHeight;	//高さ
}Title;

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;					//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureTitle[NUM_TITLE_TEX] = {};	//テクスチャのポインタ
Title						g_Title[NUM_TITLE_TEX];					//タイトルの情報

//タイトル画面に使用するテクスチャのパス
const char *c_apTitleTexName[NUM_TITLE_TEX] = {
	"data/TEXTURE/title.png",
	"data/TEXTURE/press.png",
	"data/TEXTURE/enter.png",
};

//------------------------------------------------
//				タイトルの初期化処理
//------------------------------------------------
void InitTitle(void)
{
	//デバイスへのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_TITLE_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//タイトルの情報読み込み
	LoadTitle();

	for (int nCntTitle = START_CNT; nCntTitle < NUM_TITLE_TEX; nCntTitle++, pVtx += VTX_MAX)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, c_apTitleTexName[nCntTitle], &g_pTextureTitle[nCntTitle]);

		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x - g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y - g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x + g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y - g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x - g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y + g_Title[nCntTitle].fHeight, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(g_Title[nCntTitle].pos.x + g_Title[nCntTitle].fWidth, g_Title[nCntTitle].pos.y + g_Title[nCntTitle].fHeight, NIL_F);
		}

		//rhwの設定
		{
			pVtx[VTX_LE_UP].rhw = RHW_NUM;
			pVtx[VTX_RI_UP].rhw = RHW_NUM;
			pVtx[VTX_LE_DO].rhw = RHW_NUM;
			pVtx[VTX_RI_DO].rhw = RHW_NUM;
		}

		//頂点カラーの設定
		{
			pVtx[VTX_LE_UP].col = XCOL_WHITE;
			pVtx[VTX_RI_UP].col = XCOL_WHITE;
			pVtx[VTX_LE_DO].col = XCOL_WHITE;
			pVtx[VTX_RI_DO].col = XCOL_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = ORIGIN_TEX;
			pVtx[VTX_RI_UP].tex = MA_ZE_TEX;
			pVtx[VTX_LE_DO].tex = ZE_MA_TEX;
			pVtx[VTX_RI_DO].tex = LIMIT_TEX;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();

	//タイトルBGM開始
	PlaySound(SOUND_LABEL_TITLEBGM);
}

//タイトルの情報読み込み処理
void LoadTitle(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//読み込んだ文字を格納

	pFile = fopen(TITLE_INFOFILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);		//最初のセルタイトル読み込み

		for (int nCntTitle = START_CNT; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//一行読み込み

			g_Title[nCntTitle].pos.x = atof(strtok(&aData[ZERO], ","));		// 表示位置初期化
			g_Title[nCntTitle].pos.y = atof(strtok(NULL, ","));				// 表示位置初期化

			g_Title[nCntTitle].fWidth = atof(strtok(NULL, ","));			// 幅初期化
			g_Title[nCntTitle].fHeight = atof(strtok(NULL, ","));			// 幅初期化
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//------------------------------------------------
//				タイトルの終了処理
//------------------------------------------------
void UninitTitle(void)
{
	//テクスチャの破棄
	for (int nCntTitle = START_CNT; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//------------------------------------------------
//				タイトルの更新処理
//------------------------------------------------
void UpdateTitle(void)
{
	//Enterが押された　もしくは　ゲームパッドのSTART or A ボタンが押された
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START) == true || GetGamePadTrigger(XINPUT_GAMEPAD_A) == true)
	{
		PlaySound(SOUND_LABEL_SE_DECISION);

		//チュートリアル画面に遷移
		SetFade(MODE_TUTORIAL);
	}
}

//------------------------------------------------
//				タイトルの描画処理
//------------------------------------------------
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ + 取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTitle, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = START_CNT; nCntTitle < NUM_TITLE_TEX; nCntTitle++)
	{
		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTextureTitle[nCntTitle]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * VTX_MAX, SEC);
	}
}