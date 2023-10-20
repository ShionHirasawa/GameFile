/*==========================================================================================================================

ローディング処理(何もロードしていない)[loading.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "loading.h"
#include "input.h"
#include "fade.h"

//マクロ定義
#define NUM_LOAD_TEX			(6)									//ローディング画像のテクスチャの数
#define LOADING_COUNT			(200)								//ローディング画面表示時間
#define LAODING_HEIGHT			(100.0f)							//ローディング画像の表示高さ

//ジャンプテクスチャのマクロ
#define LOAD_WAVE_TEXTURE		"data/TEXTURE/Wave.png"				//ウェーブのテクスチャパス
#define LOAD_WAVE_POS			(D3DXVECTOR3(700.0f, 500.0f, 0.0f))	//ウェーブの表示場所
#define LOAD_WAVE_WIDTH			(500.0f)							//ウェーブの表示幅
#define LOAD_WAVE_HEIGHT		(80.0f)								//ウェーブの表示高さ
#define	LOAD_WAVE_TEXU			(1.0f)								//テクスチャのU座標の幅
#define LOAD_WAVE_ANIM			(8)									//ウェーブのアニメーション変化カウント

//Loading...テクスチャのマクロ
#define LOAD_Load_TEXTURE		"data/TEXTURE/Loading.png"			//ウェーブのテクスチャパス
#define LOAD_Load_POS			(D3DXVECTOR3(800.0f, 600.0f, 0.0f))	//ウェーブの表示場所
#define LOAD_Load_WIDTH			(300.0f)							//ウェーブの表示幅
#define LOAD_Load_TEXU			(0.625f)							//テクスチャのU座標の幅
#define LOAD_ADD_TEXU			(0.125f)							//テクスチャの加算値
#define LOAD_Load_ANIM			(14)								//Loadingのアニメーション変化カウント

//PRESS ENTERのマクロ
#define LOAD_ENTER_TEXTURE		"data/TEXTURE/PressEnter.png"		//ウェーブのテクスチャパス
#define LOAD_ENTER_POS			(D3DXVECTOR3(800.0f, 600.0f, 0.0f))	//ウェーブの表示場所
#define LOAD_ENTER_WIDTH		(300.0f)							//ウェーブの表示幅
#define LOAD_ENTER_TEXU			(1.0f)								//テクスチャのU座標の幅
#define FLASH_ENTER				(5)									//表示を点滅させる

//----------------------------------------------------------------------
//							コマのマクロ
//----------------------------------------------------------------------
#define BIRTH_KOMA_COUNT		(10)								//コマが表示される透明度加算量
#define KOMA_WIDTH				(200.0f)							//コマの表示幅
#define KOMA_HEIGHT				(200.0f)							//コマの表示高さ
#define KOMA_TEXU				(1.0f)								//テクスチャのU座標の幅

//コマ１つ目のマクロ
#define LOAD_KOMA000_TEXTURE	"data/TEXTURE/Scene000.png"			//コマ1つ目のテクスチャパス
#define LOAD_KOMA000_POS		(D3DXVECTOR3(250.0f, 200.0f, 0.0f))	//コマ1つ目の表示場所
#define LOAD_KOMA000_BIRTH		(20)								//画像が現れるタイミング

//コマ2つ目のマクロ
#define LOAD_KOMA001_TEXTURE	"data/TEXTURE/Scene001.png"			//コマ2つ目のテクスチャパス
#define LOAD_KOMA001_POS		(D3DXVECTOR3(550.0f, 200.0f, 0.0f))	//コマ2つ目の表示場所
#define LOAD_KOMA001_BIRTH		(80)								//画像が現れるタイミング

//コマ3つ目のマクロ
#define LOAD_KOMA002_TEXTURE	"data/TEXTURE/Scene002.png"			//コマ2つ目のテクスチャパス
#define LOAD_KOMA002_POS		(D3DXVECTOR3(850.0f, 200.0f, 0.0f))	//コマ2つ目の表示場所
#define LOAD_KOMA002_BIRTH		(140)								//画像が現れるタイミング

//ローディングテクスチャの種類の構造体
typedef enum
{
	LOAD_TEX_WAVE = ZERO,
	LOAD_TEX_Load,
	LOAD_TEX_ENTER,
	LOAD_TEX_KOMA000,
	LOAD_TEX_KOMA001,
	LOAD_TEX_KOMA002,
	LOAD_TEX_KOMA003,
	LOAD_TEX_MAX,
}LOAD_TEX;

//ローディング画像の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	float fHeight;			//高さ
	float fWidht;			//幅
	float TexU;				//テクスチャのU座標
	float UWidth;			//テクスチャの幅
	int aCol;				//テクスチャの透明度
	int nCounterAnim;		//アニメーションカウンタ
	bool bUse;				//使用されているかどうか
}Loading;

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureLoading[NUM_LOAD_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffLoading = NULL;				//頂点バッファへのポインタ
Loading						g_Loading[NUM_LOAD_TEX];				//ローディングの構造体
int							g_nLoadingCounter;						//ローディング表示時間
bool						g_bFlash;								//PressEnterの点滅折り返し

//ローディングの初期化処理
void InitLoading(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ + 取得

	//ローディング情報の初期化
	InitInfoLoading();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, LOAD_WAVE_TEXTURE, &g_pTextureLoading[LOAD_TEX_WAVE]);		//ウェーブのテクスチャ
	D3DXCreateTextureFromFile(pDevice, LOAD_Load_TEXTURE, &g_pTextureLoading[LOAD_TEX_Load]);		//Loading... のテクスチャ
	D3DXCreateTextureFromFile(pDevice, LOAD_ENTER_TEXTURE, &g_pTextureLoading[LOAD_TEX_ENTER]);		//PRESS ENTER のテクスチャ

	//----------------------------------------------------------------------
	//						コマのテクスチャ読み込み
	//----------------------------------------------------------------------
	D3DXCreateTextureFromFile(pDevice, LOAD_KOMA000_TEXTURE, &g_pTextureLoading[LOAD_TEX_KOMA000]);	//コマ1つ目 のテクスチャ
	D3DXCreateTextureFromFile(pDevice, LOAD_KOMA001_TEXTURE, &g_pTextureLoading[LOAD_TEX_KOMA001]);	//コマ2つ目 のテクスチャ
	D3DXCreateTextureFromFile(pDevice, LOAD_KOMA002_TEXTURE, &g_pTextureLoading[LOAD_TEX_KOMA002]);	//コマ3つ目 のテクスチャ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUM_LOAD_TEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLoading, NULL);

	//頂点座標の設定
	VertexLoading();
}

//ローディング情報の初期化
void InitInfoLoading(void)
{
	//ローディングの情報初期化
	g_nLoadingCounter = ZERO;								//ローディング表示時間初期化

	//ウェーブする画像の初期化
	g_Loading[LOAD_TEX_WAVE].pos			 = LOAD_WAVE_POS;	//ウェーブ画像の位置初期化
	g_Loading[LOAD_TEX_WAVE].fWidht			 = LOAD_WAVE_WIDTH;	//表示幅の初期化
	g_Loading[LOAD_TEX_WAVE].fHeight		 = LOAD_WAVE_HEIGHT;//表示幅の初期化
	g_Loading[LOAD_TEX_WAVE].TexU			 = F_ZERO;			//テクスチャの座標初期化
	g_Loading[LOAD_TEX_WAVE].UWidth			 = LOAD_WAVE_TEXU;	//テクスチャの幅初期化
	g_Loading[LOAD_TEX_WAVE].aCol			 = COLOR_MAX;		//透明度初期化
	g_Loading[LOAD_TEX_WAVE].nCounterAnim	 = ZERO;			//アニメーションカウンター初期化
	g_Loading[LOAD_TEX_WAVE].bUse			 = true;			//使用されている状態に

	//Loading...　の画像の初期化
	g_Loading[LOAD_TEX_Load].pos			 = LOAD_Load_POS;	//Loading...画像の位置初期化
	g_Loading[LOAD_TEX_Load].fWidht			 = LOAD_Load_WIDTH;	//表示幅の初期化
	g_Loading[LOAD_TEX_Load].fHeight		 = LAODING_HEIGHT;	//表示高さの初期化
	g_Loading[LOAD_TEX_Load].TexU			 = F_ZERO;			//テクスチャの座標初期化
	g_Loading[LOAD_TEX_Load].UWidth			 = LOAD_Load_TEXU;	//テクスチャの幅初期化
	g_Loading[LOAD_TEX_Load].aCol			 = COLOR_MAX;		//透明度初期化
	g_Loading[LOAD_TEX_Load].nCounterAnim	 = ZERO;			//アニメーションカウンター初期化
	g_Loading[LOAD_TEX_Load].bUse			 = true;			//使用されている状態に

	//PRESS ENTER の画像の初期化
	g_Loading[LOAD_TEX_ENTER].pos			 = LOAD_ENTER_POS;		//PRESS ENTER画像の位置初期化
	g_Loading[LOAD_TEX_ENTER].fWidht		 = LOAD_ENTER_WIDTH;	//表示幅の初期化
	g_Loading[LOAD_TEX_ENTER].fHeight		 = LAODING_HEIGHT;		//表示高さの初期化
	g_Loading[LOAD_TEX_ENTER].TexU			 = F_ZERO;				//テクスチャの座標初期化
	g_Loading[LOAD_TEX_ENTER].UWidth		 = LOAD_ENTER_TEXU;		//テクスチャの幅初期化
	g_Loading[LOAD_TEX_ENTER].aCol			 = COLOR_MIN;			//透明度初期化
	g_Loading[LOAD_TEX_ENTER].nCounterAnim	 = ZERO;				//アニメーションカウンター初期化
	g_Loading[LOAD_TEX_ENTER].bUse			 = false;				//使用されてない状態に
	g_bFlash = false;												//点滅の折り返し初期化

	//----------------------------------------------------------------------
	//							コマの情報初期化
	//----------------------------------------------------------------------
	//コマ1つ目 の画像の初期化
	g_Loading[LOAD_TEX_KOMA000].pos			 = LOAD_KOMA000_POS;	//1つ目のコマの位置初期化
	g_Loading[LOAD_TEX_KOMA000].fWidht		 = KOMA_WIDTH;			//表示幅の初期化
	g_Loading[LOAD_TEX_KOMA000].fHeight		 = KOMA_HEIGHT;			//表示高さの初期化
	g_Loading[LOAD_TEX_KOMA000].TexU		 = F_ZERO;				//テクスチャの座標初期化
	g_Loading[LOAD_TEX_KOMA000].UWidth		 = KOMA_TEXU;			//テクスチャの幅初期化
	g_Loading[LOAD_TEX_KOMA000].aCol		 = COLOR_MIN;			//透明度初期化
	g_Loading[LOAD_TEX_KOMA000].nCounterAnim = ZERO;				//アニメーションカウンター初期化
	g_Loading[LOAD_TEX_KOMA000].bUse		 = false;				//使用されてない状態に

	//コマ2つ目 の画像の初期化
	g_Loading[LOAD_TEX_KOMA001].pos			 = LOAD_KOMA001_POS;	//2つ目のコマの位置初期化
	g_Loading[LOAD_TEX_KOMA001].fWidht		 = KOMA_WIDTH;			//表示幅の初期化
	g_Loading[LOAD_TEX_KOMA001].fHeight		 = KOMA_HEIGHT;			//表示高さの初期化
	g_Loading[LOAD_TEX_KOMA001].TexU		 = F_ZERO;				//テクスチャの座標初期化
	g_Loading[LOAD_TEX_KOMA001].UWidth		 = KOMA_TEXU;			//テクスチャの幅初期化
	g_Loading[LOAD_TEX_KOMA001].aCol		 = COLOR_MIN;			//透明度初期化
	g_Loading[LOAD_TEX_KOMA001].nCounterAnim = ZERO;				//アニメーションカウンター初期化
	g_Loading[LOAD_TEX_KOMA001].bUse		 = false;				//使用されてない状態に

	//コマ2つ目 の画像の初期化
	g_Loading[LOAD_TEX_KOMA002].pos			 = LOAD_KOMA002_POS;	//3つ目のコマの位置初期化
	g_Loading[LOAD_TEX_KOMA002].fWidht		 = KOMA_WIDTH;			//表示幅の初期化
	g_Loading[LOAD_TEX_KOMA002].fHeight		 = KOMA_HEIGHT;			//表示高さの初期化
	g_Loading[LOAD_TEX_KOMA002].TexU		 = F_ZERO;				//テクスチャの座標初期化
	g_Loading[LOAD_TEX_KOMA002].UWidth		 = KOMA_TEXU;			//テクスチャの幅初期化
	g_Loading[LOAD_TEX_KOMA002].aCol		 = COLOR_MIN;			//透明度初期化
	g_Loading[LOAD_TEX_KOMA002].nCounterAnim = ZERO;				//アニメーションカウンター初期化
	g_Loading[LOAD_TEX_KOMA002].bUse		 = false;				//使用されてない状態に
}

//ローディングの終了処理
void UninitLoading(void)
{
	//テクスチャの破棄
	for (int nCntLoad = START_CNT; nCntLoad < NUM_LOAD_TEX; nCntLoad++)
	{
		if (g_pTextureLoading[nCntLoad] != NULL)
		{
			g_pTextureLoading[nCntLoad]->Release();
			g_pTextureLoading[nCntLoad] = NULL;
		}

	}

	//頂点バッファの破棄
	if (g_pVtxBuffLoading != NULL)
	{
		g_pVtxBuffLoading->Release();
		g_pVtxBuffLoading = NULL;
	}
}

//ローディングの更新処理
void UpdateLoading(void)
{
	//Waveの更新処理
	UpdateTexWave();

	//Loading...の更新処理
	UpdateTexLoad();
	
	//PRSS ENTERの更新処理
	UpdateTexEnter();

	//コマ全体の更新処理
	UpdateTexKoma();

	//ローディング表示時間計測
	g_nLoadingCounter++;

	if (g_nLoadingCounter >= LOADING_COUNT)
	{
		g_Loading[LOAD_TEX_Load].bUse = false;		//Loading... は表示させない
		g_Loading[LOAD_TEX_ENTER].bUse = true;		//PRESS ENTERは表示させる
		XINPUT_STATE *xInput = GetXInputState();	//XInputのポインタ

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamePadTrigger(XINPUT_GAMEPAD_START))
		{
			//ゲーム画面に移行
			SetFade(MODE_GAME);
		}
	}

	//頂点座標の設定
	VertexLoading();
}

//ローディングの描画処理
void DrawLoading(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ + 取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffLoading, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntLoad = START_CNT; nCntLoad < NUM_LOAD_TEX; nCntLoad++)
	{
		//使用されている
		if (g_Loading[nCntLoad].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureLoading[nCntLoad]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLoad * NUM_VERTEX, SEC);
		}
	}
}

//ローディングの頂点設定
void VertexLoading(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLoading->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntLoad = START_CNT; nCntLoad < NUM_LOAD_TEX; nCntLoad++, pVtx += NUM_VERTEX)
	{
		if (g_Loading[nCntLoad].bUse == true)
		{
			//頂点座標の設定
			{
				//X座標の設定
				pVtx[ZERO].pos.x = g_Loading[nCntLoad].pos.x;
				pVtx[ONE].pos.x = g_Loading[nCntLoad].pos.x + g_Loading[nCntLoad].fWidht;
				pVtx[SEC].pos.x = g_Loading[nCntLoad].pos.x;
				pVtx[SAN].pos.x = g_Loading[nCntLoad].pos.x + g_Loading[nCntLoad].fWidht;

				//Y座標の設定
				pVtx[ZERO].pos.y = g_Loading[nCntLoad].pos.y;
				pVtx[ONE].pos.y = g_Loading[nCntLoad].pos.y;
				pVtx[SEC].pos.y = g_Loading[nCntLoad].pos.y + g_Loading[nCntLoad].fHeight;
				pVtx[SAN].pos.y = g_Loading[nCntLoad].pos.y + g_Loading[nCntLoad].fHeight;

				//Z座標の設定(変化なし)
				pVtx[ZERO].pos.z = g_Loading[nCntLoad].pos.z;
				pVtx[ONE].pos.z = g_Loading[nCntLoad].pos.z;
				pVtx[SEC].pos.z = g_Loading[nCntLoad].pos.z;
				pVtx[SAN].pos.z = g_Loading[nCntLoad].pos.z;
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
				
				pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Loading[nCntLoad].aCol);
				pVtx[ONE].col =  D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Loading[nCntLoad].aCol);
				pVtx[SEC].col =  D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Loading[nCntLoad].aCol);
				pVtx[SAN].col =  D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_Loading[nCntLoad].aCol);
			}

			//テクスチャの座標
			{
				pVtx[ZERO].tex = D3DXVECTOR2(g_Loading[nCntLoad].TexU, ORIGIN_POS);
				pVtx[ONE].tex = D3DXVECTOR2(g_Loading[nCntLoad].TexU + g_Loading[nCntLoad].UWidth, ORIGIN_POS);
				pVtx[SEC].tex = D3DXVECTOR2(g_Loading[nCntLoad].TexU, F_TEN);
				pVtx[SAN].tex = D3DXVECTOR2(g_Loading[nCntLoad].TexU + g_Loading[nCntLoad].UWidth, F_TEN);
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffLoading->Unlock();
}

//ウェーブテクスチャの更新処理
void UpdateTexWave(void)
{
	g_Loading[LOAD_TEX_WAVE].nCounterAnim++;

	if (g_Loading[LOAD_TEX_WAVE].nCounterAnim == LOAD_WAVE_ANIM)
	{
		g_Loading[LOAD_TEX_WAVE].nCounterAnim = ZERO;

		g_Loading[LOAD_TEX_WAVE].TexU -= F_ONE;

		if (g_Loading[LOAD_TEX_WAVE].TexU <= F_ZERO)
		{
			g_Loading[LOAD_TEX_WAVE].TexU = F_TEN;
		}
	}
}

//Loading...テクスチャの更新処理
void UpdateTexLoad(void)
{
	g_Loading[LOAD_TEX_Load].nCounterAnim++;

	if (g_Loading[LOAD_TEX_Load].nCounterAnim == LOAD_Load_ANIM)
	{
		g_Loading[LOAD_TEX_Load].nCounterAnim = ZERO;

		g_Loading[LOAD_TEX_Load].pos.x -= 10.0f;
		g_Loading[LOAD_TEX_Load].UWidth += LOAD_ADD_TEXU;
		g_Loading[LOAD_TEX_Load].fWidht += 40.0f;

		if (g_Loading[LOAD_TEX_Load].UWidth > F_TEN)
		{
			g_Loading[LOAD_TEX_Load].pos	= LOAD_Load_POS;
			g_Loading[LOAD_TEX_Load].UWidth = LOAD_Load_TEXU;
			g_Loading[LOAD_TEX_Load].fWidht = LOAD_Load_WIDTH;
		}
	}
}

//PRESS ENTERテクスチャの更新処理
void UpdateTexEnter(void)
{
	if (g_Loading[LOAD_TEX_ENTER].bUse == true)
	{
		//透明度を減算(現れる)
		if (g_bFlash == false)
		{
			//どんどん現れる
			g_Loading[LOAD_TEX_ENTER].aCol += FLASH_ENTER;

			//透明度折り返し
			if (g_Loading[LOAD_TEX_ENTER].aCol >= COLOR_MAX)
			{
				g_Loading[LOAD_TEX_ENTER].aCol = COLOR_MAX;
				g_bFlash = true;
			}
		}

		else
		{
			//どんどん薄れていく
			g_Loading[LOAD_TEX_ENTER].aCol -= FLASH_ENTER;
			
			//透明度折り返し
			if (g_Loading[LOAD_TEX_ENTER].aCol <= COLOR_MIN)
			{
				g_Loading[LOAD_TEX_ENTER].aCol = COLOR_MIN;
				g_bFlash = false;
			}
		}
	}
}

//----------------------------------------------------------------------
//							コマの更新処理
//----------------------------------------------------------------------

//コマ全体の更新処理
void UpdateTexKoma(void)
{
	//コマ1つ目の更新処理
	Koma000Update();

	//コマ2つ目の更新処理
	Koma001Update();

	//コマ3つ目の更新処理
	Koma002Update();
}

//コマ1つ目の更新処理
void Koma000Update(void)
{
	if (g_nLoadingCounter >= LOAD_KOMA000_BIRTH)
	{
		g_Loading[LOAD_TEX_KOMA000].bUse = true;

		g_Loading[LOAD_TEX_KOMA000].aCol += BIRTH_KOMA_COUNT;

		if (g_Loading[LOAD_TEX_KOMA000].aCol > COLOR_MAX)
		{
			g_Loading[LOAD_TEX_KOMA000].aCol = COLOR_MAX;
		}
	}
}

//コマ2つ目の更新処理
void Koma001Update(void)
{
	if (g_nLoadingCounter >= LOAD_KOMA001_BIRTH)
	{
		g_Loading[LOAD_TEX_KOMA001].bUse = true;

		g_Loading[LOAD_TEX_KOMA001].aCol += BIRTH_KOMA_COUNT;

		if (g_Loading[LOAD_TEX_KOMA001].aCol > COLOR_MAX)
		{
			g_Loading[LOAD_TEX_KOMA001].aCol = COLOR_MAX;
		}
	}
}

//コマ3つ目の更新処理
void Koma002Update(void)
{
	if (g_nLoadingCounter >= LOAD_KOMA002_BIRTH)
	{
		g_Loading[LOAD_TEX_KOMA002].bUse = true;

		g_Loading[LOAD_TEX_KOMA002].aCol += BIRTH_KOMA_COUNT;

		if (g_Loading[LOAD_TEX_KOMA002].aCol > COLOR_MAX)
		{
			g_Loading[LOAD_TEX_KOMA002].aCol = COLOR_MAX;
		}
	}
}