/*==========================================================================================================================

													タイトル画面処理[title.cpp]
														Author:平澤詩苑

==========================================================================================================================*/
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//マクロ定義
#define TITLE_LOGO		(3)			//タイトル画面で使うテクスチャの数

//タイトル名のマクロ
#define SET_TITLE_POS	D3DXVECTOR3(640.0f, 230.0f, F_ZERO)	//タイトル名のセットポジション

#define TITLE_POSX		(640.0f)	// X座標初期位置
#define TITLE_POSY		(-50.0f)	// Y座標初期位置
#define TITLE_WIDTH		(400.0f)	// 幅
#define TITLE_HEIGHT	(110.0f)	// 高さ
#define FALL_TITLE		(2.0f)		//タイトル名落下数値
#define LENGTH_TIMES	(0.5f)		//長さの計算に用いる倍率

//ENTERロゴのマクロ
#define ENTER_POS_ZERO	D3DXVECTOR3(540.0f, 550.0f, 0.0f)
#define ENTER_POS_ONE	D3DXVECTOR3(740.0f, 550.0f, 0.0f)
#define ENTER_POS_SEC	D3DXVECTOR3(540.0f, 620.0f, 0.0f)
#define ENTER_POS_SAN	D3DXVECTOR3(740.0f, 620.0f, 0.0f)

//色関連のマクロ
#define COLOR_ON		(255)		//有色
#define COLOR_OFF		(0)			//無色
#define FLASH_CNT		(-3)		//点滅カウンタの初期値
#define FLASH_INTERVAL	(3)			//点滅の間隔
#define SLOW_FLASH		(4)			//ゆっくり点滅させるためのマクロ
#define MAKE_CLEAR		(250)		//透明にする方向転換地
#define MAKE_OPAQUE		(20)		//不透明にする方向転換地
#define FLASH_TIME		(100)		//点滅表示時間( ENTER が押されてからフェードに移行するまで)

//タイトル名のアニメーションマクロ
#define ADDSPIN			(0.19f)		//回転の加算値
#define LIMITSPIN		(31.35f)	//回転の上限値
#define DEFSPIN			(31.42f)	//回転の基準値
#define MINUSLENGTH		(1.0f)		//長さの減算値
#define MINUSCROSS		(2.6f)		//落下の減算値
#define CROSS_POS		(400.0f)	//斜めの初期値
#define FDEFAULT		(0.0f)		//基準値

#define TITLE_COLOR		(130)		//タイトルのRカラー

#define TITLE_BG_TEX	"data/TEXTURE/背景.png"        	
#define TITLE_NAME_TEX	"data/TEXTURE/作品名.png"      
#define TITLE_ENTER_TEX	"data/TEXTURE/エンター入力.png"

//タイトル名構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	float fAngle;			//中心から頂点までの角度
	float flength;			//中心から頂点までの長さ
	float fLengthMinus;		//長さを徐々に元に戻すための変数
}TitleName;

//タイトル係数
typedef enum
{
	TITLE_DISPLAY = ZERO,
	TITLE_NAME,
	TITLE_ENTER,
	TITLE_MAX
}TITLE;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureTitle[TITLE_LOGO] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				//頂点バッファへのポインタ
TitleName g_Title;
float g_CrossDown;											//斜め右上からタイトル表示
int g_FlashCnt;												//PRESS ENTERを点滅させるカウンタ
int g_SlowFlash;											//ゆっくり点滅させる
int g_aFlash;												//点滅用の変数

void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_BG_TEX   , &g_pTextureTitle[TITLE_DISPLAY]);	//タイトル画面背景
	D3DXCreateTextureFromFile(pDevice, TITLE_NAME_TEX , &g_pTextureTitle[TITLE_NAME]);		//ゲームタイトル
	D3DXCreateTextureFromFile(pDevice, TITLE_ENTER_TEX, &g_pTextureTitle[TITLE_ENTER]);		//PRESS THE ENTER

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TITLE_LOGO, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	g_Title.pos  = D3DXVECTOR3(TITLE_POSX, TITLE_POSY, F_ZERO);
	g_Title.move = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);		//移動量を初期化する
	g_Title.rot  = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);		//向きを初期化する(Z値を使用)

	//長さを算出
	g_Title.flength = sqrtf(TITLE_WIDTH * TITLE_WIDTH + TITLE_HEIGHT * TITLE_HEIGHT) * LENGTH_TIMES;

	//長さに変化を付ける変数の初期化
	g_Title.fLengthMinus = g_Title.flength;

	//角度を算出
	g_Title.fAngle = atan2f(TITLE_WIDTH, TITLE_HEIGHT);

	//斜めからの座標を初期化
	g_CrossDown = CROSS_POS;

	//それぞれのUIを初期化
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++, pVtx += NUM_VERTEX)
	{
		switch (nCnt)
		{
		case TITLE_DISPLAY:		//タイトル画面
			{
			//頂点座標の設定
			pVtx[ZERO].pos = D3DXVECTOR3(ORIGIN_POS , ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
			pVtx[SEC].pos = D3DXVECTOR3(ORIGIN_POS  , SCREEN_HEIGHT, ORIGIN_POS);
			pVtx[SAN].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);

			//rhwの設定
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;
			
			//頂点カラーの設定
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			
			//テクスチャの座標
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , F_TEN);
			}
			break;

		case TITLE_NAME:		//ゲームタイトル
		{
			//頂点座標の設定
			pVtx[ZERO].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (-D3DX_PI + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
			pVtx[ZERO].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (-D3DX_PI + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
			pVtx[ONE].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (D3DX_PI - g_Title.fAngle))  * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
			pVtx[ONE].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (D3DX_PI - g_Title.fAngle))  * (g_Title.flength - g_Title.fLengthMinus);
			pVtx[SEC].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (ZERO - g_Title.fAngle))     * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
			pVtx[SEC].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (ZERO - g_Title.fAngle))     * (g_Title.flength - g_Title.fLengthMinus);
			pVtx[SAN].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (ZERO + g_Title.fAngle))     * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
			pVtx[SAN].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (ZERO + g_Title.fAngle))     * (g_Title.flength - g_Title.fLengthMinus);

			//rhwの設定
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;

			//頂点カラーの設定
			pVtx[ZERO].col = D3DCOLOR_RGBA(TITLE_COLOR, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[ONE].col = D3DCOLOR_RGBA(TITLE_COLOR, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SEC].col = D3DCOLOR_RGBA(TITLE_COLOR, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SAN].col = D3DCOLOR_RGBA(TITLE_COLOR, COLOR_ON, COLOR_ON, COLOR_ON);

			//テクスチャの座標
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN  , F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO , F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN  , F_TEN);
		}
			break;

		case TITLE_ENTER:		//PRESS THE ENTER
			{
			//頂点座標の設定
			pVtx[ZERO].pos = ENTER_POS_ZERO;
			pVtx[ONE].pos = ENTER_POS_ONE;
			pVtx[SEC].pos = ENTER_POS_SEC;
			pVtx[SAN].pos = ENTER_POS_SAN;

			//rhwの設定
			pVtx[ZERO].rhw = RHW_NUM;
			pVtx[ONE].rhw = RHW_NUM;
			pVtx[SEC].rhw = RHW_NUM;
			pVtx[SAN].rhw = RHW_NUM;

			//頂点カラーの設定
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, COLOR_ON);

			//テクスチャの座標
			pVtx[ZERO].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN, F_ZERO);
			pVtx[SEC].tex = D3DXVECTOR2(F_ZERO, F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
			}
			break;
		}
	}
	g_FlashCnt = FLASH_CNT;			//点滅カウンタの初期化
	g_aFlash = COLOR_ON;			//点滅の色初期化
	g_SlowFlash = SLOW_FLASH;		//スロー点滅の初期化

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM000);
}

void UninitTitle(void)
{
	//テクスチャ(三枚)の破棄
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

void UpdateTitle(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//背景以外を更新するため
	pVtx += NUM_VERTEX;

	//長さを少しずつ戻す
	if (g_Title.fLengthMinus > FDEFAULT)
	{
		g_Title.fLengthMinus -= MINUSLENGTH;
	}

	//回転値を加算
	if (g_Title.rot.z < LIMITSPIN)
	{
		g_Title.rot.z += ADDSPIN;

		if (g_Title.rot.z >= LIMITSPIN)
		{
			g_Title.rot.z = DEFSPIN;
		}
	}

	//斜めからの落下を減算
	if (g_CrossDown >= FDEFAULT)
	{
		g_CrossDown -= MINUSCROSS;
	}

	pVtx[ZERO].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (-D3DX_PI + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
	pVtx[ZERO].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (-D3DX_PI + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
	pVtx[ONE].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + ( D3DX_PI - g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
	pVtx[ONE].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + ( D3DX_PI - g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
	pVtx[SEC].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (    ZERO - g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
	pVtx[SEC].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (    ZERO - g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
	pVtx[SAN].pos.x = g_Title.pos.x + sinf(g_Title.rot.z + (    ZERO + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus) + g_CrossDown;
	pVtx[SAN].pos.y = g_Title.pos.y + cosf(g_Title.rot.z + (    ZERO + g_Title.fAngle)) * (g_Title.flength - g_Title.fLengthMinus);
	
	//PRESS THE ENTER の頂点ポインタ
	pVtx += NUM_VERTEX;

	//ゆっくり点滅させる
	if (g_FlashCnt == FLASH_CNT)
	{
		g_aFlash += g_SlowFlash;

		//不透明にする
		if (g_aFlash < MAKE_OPAQUE)
		{
			g_aFlash = MAKE_OPAQUE;

			g_SlowFlash *= MINUS_ONE;		//方向転換
		}

		//透明にする
		else if (g_aFlash > MAKE_CLEAR)
		{
			g_aFlash = MAKE_CLEAR;

			g_SlowFlash *= MINUS_ONE;		//方向転換
		}
	}

	//ゲーム開始
	if (g_Title.pos == SET_TITLE_POS)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true && g_FlashCnt == FLASH_CNT)
		{//ENTERキーが押された
			g_FlashCnt++;
			PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音を鳴らす
		}

		if (g_FlashCnt > FLASH_CNT)
		{
			g_FlashCnt++;

			//点滅させる
			if (g_FlashCnt % FLASH_INTERVAL == COLOR_OFF)
			{
				g_aFlash = g_aFlash ? COLOR_OFF : COLOR_ON;
			}

			//次の画面に移行
			if (g_FlashCnt >= FLASH_TIME)
			{
				//フェード呼び出し
				SetFade(MODE_TUTORIAL);
			}
		}
	}

	else
	{
		//頂点座標の設定
		g_Title.pos.y += FALL_TITLE;

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//タイトルとそれに関連する変数を標準化
			g_Title.pos = SET_TITLE_POS;
			g_Title.rot.z = DEFSPIN;
			g_Title.fLengthMinus = FDEFAULT;
			g_CrossDown = FDEFAULT;
		}
	}

	//PRESS THE ENTERの頂点カラー
	pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, g_aFlash);
	pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, g_aFlash);
	pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, g_aFlash);
	pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_ON, COLOR_ON, COLOR_ON, g_aFlash);

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();
}

void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTitle, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
		
	//テクスチャの設定
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++)
	{
		pDevice->SetTexture(ZERO, g_pTextureTitle[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * NUM_VERTEX, SEC);
	}
}