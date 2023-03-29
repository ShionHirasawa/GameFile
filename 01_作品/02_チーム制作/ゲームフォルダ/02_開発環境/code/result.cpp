/*----------------------------------------------------------------------------------------------------------------------------------------------------------

リザルト画面処理[result.cpp]
Author:平澤詩苑

------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "result.h"
#include "color.h"
#include "fade.h"
#include "sound.h"
#include "file.h"
#include "model.h"
#include "resultPlayer.h"
#include "resultCamera.h"
#include "VictoryStand.h"
#include "resultCylinder.h"
#include "meshfield.h"
#include "conversioninput.h"

#define NUM_RESULT_UI		(3)			//スキップ時に上下から挟む用の幕（２つ）とスキップ表示UIの　計３つ
#define SKIP_UI_TEX			"data\\TEXTURE\\ResultSKipUI.png"		//スキップ表示UIのテクスチャパス
#define SKIP_UI_POSX		(1050.0f)	//スキップ表示UIのX位置
#define SKIP_UI_POSY		(650.0f)	//スキップ表示UIのY位置
#define SKIP_UI_WIDTH		(200.0f)	//スキップ表示UIの幅
#define SKIP_UI_HEIGHT		(100.0f)	//スキップ表示UIの高さ

#define HALF_SCREEN_HEIGHT	(SCREEN_HEIGHT / 2)	//画面半分の高さ
#define SKIP_SHUTTER_HEIGHT	(SCREEN_HEIGHT / 4)	//シャッターの高さ（画面の半分の半分
#define SKIP_SHUTTER_POS_X	(SCREEN_WIDTH / 2)	//シャッターのX位置(画面の半分
#define SHUTTER_SPEED		(100.0f)			//シャッターの開閉速度

//リザルトスキップの構造体
typedef struct
{
	D3DXVECTOR3 pos[NUM_RESULT_UI];	//各UIの原点位置
	bool bShutter;					//シャッターの開閉ON/OFF
	int nUI_DispCounter;			//UIの表示時間
}SkipUI;

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffResult = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureResult = NULL;	//テクスチャのポインタ
SkipUI					g_SkipUI;					//リザルトスキップ時の情報

//************************************************
//				リザルトの初期化処理
//************************************************
void InitResult(void)
{
	//デバイスへのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SKIP_UI_TEX, &g_pTextureResult);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * NUM_RESULT_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < NUM_RESULT_UI; nCntResult++, pVtx += VTX_MAX)
	{
		//UIの設定
		if (nCntResult == 0)
		{
			//UIの位置設定
			g_SkipUI.pos[nCntResult] = D3DXVECTOR3(SKIP_UI_POSX, SKIP_UI_POSY, 0.0f);

			//頂点座標の設定（原点から上下左右に頂点を設定する
			pVtx[0].pos = D3DXVECTOR3(SKIP_UI_POSX - SKIP_UI_WIDTH, SKIP_UI_POSY - SKIP_UI_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SKIP_UI_POSX + SKIP_UI_WIDTH, SKIP_UI_POSY - SKIP_UI_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SKIP_UI_POSX - SKIP_UI_WIDTH, SKIP_UI_POSY + SKIP_UI_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SKIP_UI_POSX + SKIP_UI_WIDTH, SKIP_UI_POSY + SKIP_UI_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = XCOL_WHITE;
		}

		//シャッターフレームの設定
		// MEMO :カウンターが「１」なら上のシャッター、カウンターが「２」なら下のシャッターとして使用する
		else
		{
			//上のシャッタフレームの場合
			if (nCntResult == 1)
			{
				//上のシャッターフレームの位置設定
				g_SkipUI.pos[nCntResult] = D3DXVECTOR3(SKIP_SHUTTER_POS_X, -SKIP_SHUTTER_HEIGHT, 0.0f);
			}
			else
			{
				//上のシャッターフレームの位置設定
				g_SkipUI.pos[nCntResult] = D3DXVECTOR3(SKIP_SHUTTER_POS_X, SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT, 0.0f);
			}

			//頂点座標の設定（原点から上下左右に頂点を設定する
			pVtx[0].pos = D3DXVECTOR3(SKIP_SHUTTER_POS_X - SKIP_SHUTTER_POS_X, g_SkipUI.pos[nCntResult].y - SKIP_SHUTTER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SKIP_SHUTTER_POS_X + SKIP_SHUTTER_POS_X, g_SkipUI.pos[nCntResult].y - SKIP_SHUTTER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SKIP_SHUTTER_POS_X - SKIP_SHUTTER_POS_X, g_SkipUI.pos[nCntResult].y + SKIP_SHUTTER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SKIP_SHUTTER_POS_X + SKIP_SHUTTER_POS_X, g_SkipUI.pos[nCntResult].y + SKIP_SHUTTER_HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = XCOL_BLACK;
		}

		//rhwの設定
		pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = RHW;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffResult->Unlock();

	//----------------------------------------------=
	//リザルト用オブジェクトの初期化処理関数呼び出し
	//----------------------------------------------=
	InitResultObject();

	g_SkipUI.bShutter = true;	//シャッターを開ける
	PlaySoundBGM(SOUND_LABEL_BGM_RESULT_01);	//最初のリザルト音楽を流す
}

//------------------------------------------------
//		リザルト用オブジェクトの初期化処理
//------------------------------------------------
void InitResultObject(void)
{
	InitFile();				// ファイル初期化
	LoadModelViewerFile("data\\model.txt");	// モデルビューワーファイル読み込み（各オブジェクト初期化前に行うこと！）
	InitAnimalModel();		// 動物モデル初期化
	InitPlayer_RESULT();	// プレイヤーの初期化処理
	InitMeshfield();		// 地面の初期化処理
	InitResultCylinder();	// 背景の初期化処理
	InitVictoryStand();		// 表彰台の初期化処理
	InitResultCamera();		// カメラの初期化処理
}

//************************************************
//				リザルトの終了処理
//************************************************
void UninitResult(void)
{
	//テクスチャの破棄
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//----------------------------------------------=
	//リザルト用オブジェクトの終了処理関数呼び出し
	//----------------------------------------------=
	UninitResultObject();
}

//------------------------------------------------
//		リザルト用オブジェクトの終了処理
//------------------------------------------------
void UninitResultObject(void)
{
	UninitMeshfield();			//地面の終了処理
	UninitResultCylinder();		//背景の終了処理
	UninitModel();				//動物モデル終了処理
	UninitPlayer_RESULT();		//プレイヤーの終了処理
	UninitResultCamera();		//カメラの終了処理
	UninitVictoryStand();		//表彰台の終了処理
}

//************************************************
//				リザルトの更新処理
//************************************************
void UpdateResult(void)
{
	//リザルトを飛ばす
	for (int nCntPad = 0; nCntPad < MAX_USE_GAMEPAD; nCntPad++)
	{
		//キーボードの　ENTERキー　または、接続されているコントローラーのAボタンが押された
		if (GetKeyboardTrigger(DIK_RETURN) || GetButton(nCntPad, INPUTTYPE_TRIGGER, BUTTON_A))
		{
			//リザルトを飛ばす処理へ
			SkipResult();
			break;
		}
	}

	//シャッターを閉じるか開けるか
	MoveSkipShutter();

	//スキップUIの頂点情報の更新
	UpdateSkipUIVtx();

	//----------------------------------------------=
	//リザルト用オブジェクトの更新処理関数呼び出し
	//----------------------------------------------=
	UpdateResultObject();
}

//リザルトを飛ばす
void SkipResult(void)
{
	//現在のウェーブを取得
	WAVECamera *pWave = GetWaveCamera();

	switch (*pWave)
	{
		//------------------
		//	全ウェーブ終了
		//------------------
	case WAVECamera_FINISH:
		SetFade(MODE_TITLE);	//タイトル画面へ
		break;

		//------------------
		//まだウェーブが終わってない
		//------------------
	default:
		g_SkipUI.bShutter = false;				//シャッターを閉じる
		break;
	}
}

//シャッターを閉じるか開けるか
void MoveSkipShutter(void)
{
	//現在のウェーブを取得
	WAVECamera *pWave = GetWaveCamera();

	//シャッターを開ける
	if (g_SkipUI.bShutter)
	{
		g_SkipUI.pos[1].y -= SHUTTER_SPEED;//上のシャッターを上に上げる

		if (g_SkipUI.pos[1].y <= -SKIP_SHUTTER_HEIGHT)
		{//上のシャッターが画面から消えた
			g_SkipUI.pos[1].y = -SKIP_SHUTTER_HEIGHT;		//画面の上に戻す
		}

		g_SkipUI.pos[2].y += SHUTTER_SPEED;//下のシャッターを下に下げる

		if (g_SkipUI.pos[2].y >= SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT)
		{//下のシャッターが画面から消えた
			g_SkipUI.pos[2].y = SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT;//画面の下に戻す
		}
	}

	//シャッターを閉じる
	else
	{
		//上のシャッターを下に下げる			//下のシャッターを上に上げる
		g_SkipUI.pos[1].y += SHUTTER_SPEED;		g_SkipUI.pos[2].y -= SHUTTER_SPEED;

		//どちらかのシャッターが画面の半分まで来た
		if (g_SkipUI.pos[1].y >= HALF_SCREEN_HEIGHT - SKIP_SHUTTER_HEIGHT || 
			g_SkipUI.pos[2].y <= HALF_SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT)
		{
			//画面の半分の位置に戻す
			g_SkipUI.pos[1].y = HALF_SCREEN_HEIGHT - SKIP_SHUTTER_HEIGHT;
			g_SkipUI.pos[2].y = HALF_SCREEN_HEIGHT + SKIP_SHUTTER_HEIGHT;

			//シャッターを開ける
			g_SkipUI.bShutter = true;

			switch (*pWave)
			{
				//-----------------
				//	表彰台シーン
				//-----------------
			case WAVECamera_LAST_VictoryStand:
				SkipVicStd_Player();		//表彰台の登場を終わらせて、プレイヤーを表彰台に立たせる
				*pWave = WAVECamera_FINISH;	//ウェーブ終了
				StopSoundBGM(SOUND_LABEL_BGM_RESULT_01);	//開始リザルトBGM停止
				PlaySoundBGM(SOUND_LABEL_BGM_RESULT_02);	//余韻リザルトBGM再生
				break;

				//------------------
				//	ウェーブ途中
				//------------------
			default:
				*pWave = WAVECamera_LAST_VictoryStand;	//最後のウェーブへ
				SetNextWave(*pWave);					//最後のウェーブ設定へ
				SpecialWave();							//特殊演出設定
				break;
			}
		}
	}
}

//スキップUIの頂点情報の更新処理
void UpdateSkipUIVtx(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntShutter = 0; nCntShutter < NUM_RESULT_UI; nCntShutter++, pVtx += VTX_MAX)
	{
		if (nCntShutter != 0)
		{
			//頂点座標の設定（高さしか変わらないので、Ｙ座標の設定だけ
			pVtx[0].pos.y = g_SkipUI.pos[nCntShutter].y - SKIP_SHUTTER_HEIGHT;
			pVtx[1].pos.y = g_SkipUI.pos[nCntShutter].y - SKIP_SHUTTER_HEIGHT;
			pVtx[2].pos.y = g_SkipUI.pos[nCntShutter].y + SKIP_SHUTTER_HEIGHT;
			pVtx[3].pos.y = g_SkipUI.pos[nCntShutter].y + SKIP_SHUTTER_HEIGHT;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffResult->Unlock();
}

//------------------------------------------------
//		リザルト用オブジェクトの更新処理
//------------------------------------------------
void UpdateResultObject(void)
{
	UpdatePlayer_RESULT();	//プレイヤーの更新処理
	UpdateResultCamera();	//カメラの更新処理
	UpdateVictoryStand();	//表彰台の更新処理
	UpdateResultCylinder();	//背景の更新処理
	UpdateMeshfield();		//フィールドの更新処理
}

//************************************************
//				リザルトの描画処理
//MEMO:2DUIを描画する場合はαテストを忘れずに！
//************************************************
void DrawResult(void)
{
	//デバイスのポインタ + 取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntResult = 0; nCntResult < NUM_RESULT_UI; nCntResult++)
	{
		//スキップ表示UIはウェーブが初期の時のみ表示する
		if (nCntResult == 0)
		{
			if (*GetWaveCamera() == WAVECamera_01_SideWays)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureResult);

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * VTX_MAX, 2);
			}
		}
		else
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * VTX_MAX, 2);
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//----------------------------------------------=
	//リザルト用オブジェクトの描画処理関数呼び出し
	//----------------------------------------------=
	DrawResultObject();
}

//------------------------------------------------
//		リザルト用オブジェクトの描画処理
//------------------------------------------------
void DrawResultObject(void)
{
	SetResultCamera();			//カメラの設定処理
	DrawResultCylinder();		//背景の描画処理
	DrawMeshfield();			//地面の描画処理
	DrawPlayer_RESULT();		//プレイヤーの描画処理
	DrawVictoryStand();			//表彰台の描画処理
}