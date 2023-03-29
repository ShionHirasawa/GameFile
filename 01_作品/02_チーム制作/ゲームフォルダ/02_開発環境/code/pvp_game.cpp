/*==========================================================================================================================================================

ゲーム画面処理[pvp_game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "curtain.h"
#include "pvp_game.h"
#include "input.h"
#include "file.h"
#include "color.h"
#include "fade.h"
#include "camera.h"
#include "camera_frame.h"
#include "pause.h"
#include "PvP_player.h"
#include "model.h"
#include "light.h"
#include "meshdome.h"
#include "meshfield.h"
#include "meshfault.h"
#include "gauge.h"
#include "score.h"
#include "bonus.h"
#include "item.h"
#include "timer.h"
#include "particle.h"
#include "charge_effect.h"
#include "attack_effect.h"
#include "tremor_effect.h"
#include "charge_cylinder.h"
#include "eff_shock-wave_00.h"
#include "sound.h"
#include "comai.h"
#include "fence.h"
#include "GaugeFrame.h"
#include "object.h"

//マクロ
#define FENCE_DROPPED_TIME		(48)	//フェンス落下注意効果音鳴らす時間
#define COUNTDOWN_TIME			(6)		//カウントダウン効果音鳴らす時間

//グローバル変数宣言
bool g_bPause_PvP = false;				// ポーズ
int  g_nUseContNum_PvP;					// 使用しているコントローラーの数
bool g_abUsePlayer_PvP[MAX_USE_GAMEPAD];// 使用しているプレイヤー（接続チェックに使用）
bool g_bDisconnectPlayer_PvP;			// 接続が切れたことを確認する
int  g_numGamePad_PvP;
CHECKMODE	g_CheckMode_PvP;
NumCamera	g_NumCamera_PvP;
bool		g_bPhotoMode_PvP;			// フォトモード切替		true:ポーズ画面非表示	false:ボーズ画面表示
bool g_bDroppedFence = false;			// フェンス落下注意効果音鳴らしたか
bool g_bCountDowned = false;			// カウントダウン効果音鳴らしたか

//------------------------------------------------
//				ゲームの初期化処理
//------------------------------------------------
void InitPvPGame(void)
{
	g_nUseContNum_PvP = SetUseController_PvP();			// コントローラーの使用設定
	InitFile();											// ファイルの初期化処理（モデルビューワーファイル読み込み前に行うこと！）
	InitObject();										// オブジェクト初期化
	LoadModelViewerFile("data\\model.txt");				// モデルビューワーファイル読み込み（各オブジェクト初期化前に行うこと！）
	LoadModelOriginalFile("data\\originalmodel.txt");	// モデルオリジナルファイル読み込み
	g_NumCamera_PvP = NumCamera_FOUR_Separate;			// 初期カメラの設定（現在はPlayer0を注視点としたカメラ　　画面分割ナシ）
	
	InitLight();				// ライト初期化処理
	InitMeshDome();				// メッシュドーム初期化処理
	InitMeshfield();			// ステージ初期化処理
	InitMeshFault();			// メッシュの断面初期化処理
	InitModel();				// モデルの初期化処理（モデルを使用する初期化の前に行うこと！）
	InitFence();				// フェンスの初期化処理
	InitComAI();				// コンピューターAIの初期化処理（プレイヤーの前に行うこと）
	InitPlayer();				// プレイヤーの初期化処理
	InitBonus();				// ボーナスの初期化処理
	InitItem();					// アイテムの初期化処理
	InitCameraFrame();			// 画面分割の枠初期化処理
	InitCamera(g_NumCamera_PvP);// カメラの初期化処理
	InitPause();				// ポーズ画面の初期化処理
	InitParticle();				// パーティクルの初期化処理
	{// エフェクトの初期化処理
		InitChargeEffect();		//チャージエフェクト
		InitChargeCylinder();	//チャージエフェクト(しりんだー)
		InitAttackEffect();		//攻撃エフェクト
		InitTremorEffect();		//ヒップドロップエフェクト
		InitEff_shockWave_00(); //ダッシュ衝撃波エフェクト
	}	
	InitScore();				// スコアの初期化
	InitTime();					// タイマーの初期化処理
	InitGauge();				// ゲージの初期化処理
	InitGaugeFrame();			// ゲージ枠の初期化処理
	SetTime(LIMIT_TIMER);		// タイマーの設定処理
	SetScore(0,4);				// スコアの設定処理
	InitSelModePvPCurtain();	// カーテンの初期化処理

	g_bPause_PvP = false;				// ポーズの初期化
	g_bDisconnectPlayer_PvP = false;	// 正常にコントローラーが接続されている状態とする
	g_bPhotoMode_PvP = false;			// フォトモード初期化
	g_bDroppedFence = false;			// 効果音系初期化
	g_bCountDowned = false;

	//ゲームBGM開始
	PlaySoundBGM(SOUND_LABEL_BGM_GAME_PVP);
}

//------------------------------------------------
//				ゲームの終了処理
//------------------------------------------------
void UninitPvPGame(void)
{
	/*======================================================================
	各種オブジェクトの終了処理
	========================================================================*/

	/*エフェクトなどの板ポリゴンの終了処理*/
	UninitLight();			// ライト終了処理
	UninitMeshfield();		// ステージ終了処理
	UninitMeshDome();		// メッシュドーム終了処理
	UninitMeshFault();		// メッシュの断面終了処理
	UninitObject();			// オブジェクト終了処理
	UninitFence();			// フェンスの終了処理（形式上置いておいただけ）
	UninitCamera();			// カメラの終了処理
	UninitComAI();			// コンピューターAIの終了処理
	UninitPlayer();			// プレイヤーの終了処理
	UninitBonus();			// ボーナスの終了処理
	UninitItem();			// アイテムの終了処理
	UninitPause();			// ポーズ画面の終了処理
	UninitGaugeFrame();		// ゲージ枠の終了処理
	UninitGauge();			// ゲージの終了処理
	UninitModel();			// モデルの終了処理
	UninitParticle();		// パーティクルの終了処理
	{	// エフェクトの終了処理
		UninitChargeEffect();		//チャージエフェクト
		UninitChargeCylinder();		//チャージエフェクト(しりんだー)
		UninitAttackEffect();		//攻撃エフェクト
		UninitTremorEffect();		//ヒップドロップエフェクト
		UninitEff_shockWave_00();	//ダッシュ衝撃波エフェクト
	}
	UninitScore();			// スコアの終了処理
	UninitCameraFrame();	// 画面分割の枠終了処理
	UninitTime();			// タイマーの終了処理（ここは順番は問わない）
	UninitCurtain();		// カーテンの終了処理

	//ゲームBGM停止
	StopSoundBGM(SOUND_LABEL_BGM_GAME_PVP);
}

//------------------------------------------------
//				ゲームの更新処理
//------------------------------------------------
void UpdatePvPGame(void)
{
	FADE fadeState = GetFade();
	Player *pPlayer = GetPlayer();	

	//ポーズがOFF
	if (g_bPause_PvP == false)
	{
		UpdateLight();			// ライトの更新処理
		UpdateMeshfield();		// ステージ更新処理
		UpdateMeshDome();		// メッシュドーム更新処理
		UpdateMeshFault();		// メッシュの断面更新処理
		UpdateFence();			// フェンスの更新処理
		UpdateObject();			// オブジェクト更新処理
		UpdatePlayer();			// プレイヤーの更新処理
		UpdateBonus();			// ボーナスの更新処理
		UpdateItem();			// アイテムの更新処理
		UpdateCamera();			// カメラの更新処理
		UpdateGauge();			// ゲージの更新処理
		UpdateGaugeFrame();		// ゲージ枠の更新処理（中身はないが形式上）
		{// エフェクトの更新処理
			UpdateChargeEffect();		//チャージエフェクト
			UpdateChargeCylinder();		//チャージエフェクト(しりんだー)
			UpdateAttackEffect();		//攻撃エフェクト
			UpdateTremorEffect();		//ヒップドロップエフェクト
			UpdateEff_shockWave_00();	//ダッシュ衝撃波エフェクト
		}
		UpdateParticle();	// パーティクルの更新処理
		UpdateScore();		//スコアの更新処理
		UpdateTime(MODE_PvPGAME);		//タイマーの更新処理

		ChangeNumCamera_PvP();	//カメラの数変更処理

		//ポーズ取得
		for (int nCntPause = 0; nCntPause < 4; nCntPause++)
		{
			//ポーズ状態切り替え
			if (GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(nCntPause, XINPUT_GAMEPAD_START) == true)
			{
				//何番目のゲームパッドか保存する
				g_numGamePad_PvP = nCntPause;

				PlaySoundSE(SOUND_LABEL_SE_PAUSE_DECISION, 0);
				PauseSound(SOUND_LABEL_BGM_GAME_PVP);

				//ポーズ状態にする
				g_bPause_PvP = true;

				//ポーズしたコントローラーをpause.cppに渡す
				SetPadPause(false, nCntPause);
			}
		}

		//フェンス落下注意効果音鳴らすかどうか
		if (GetTime() < FENCE_DROPPED_TIME && g_bDroppedFence == false)
		{
			PlaySoundSE(SOUND_LABEL_SE_WARNING, 0);
			g_bDroppedFence = true;
		}

		//カウントダウン効果音鳴らすかどうか
		if (GetTime() < COUNTDOWN_TIME && g_bCountDowned == false)
		{
			PlaySoundSE(SOUND_LABEL_SE_COUNTDOWN, 0);
			g_bCountDowned = true;
		}
	}

	else
	{
		//ポーズ画面の更新処理
		UpdatePause(MODE_PvPGAME);

		//フォトモードON
		if (g_bPhotoMode_PvP)
		{
			UpdateCamera();

			//1フレームだけ更新する
			if (GetKeyboardTrigger(DIK_RIGHT))
			{
				UpdateLight();			// ライトの更新処理
				UpdateMeshfield();		// ステージ更新処理
				UpdateMeshDome();		// メッシュドーム更新処理
				UpdateMeshFault();		// メッシュの断面更新処理
				UpdateFence();			// フェンスの更新処理
				UpdateObject();			// オブジェクト更新処理
				UpdatePlayer();			// プレイヤーの更新処理
				UpdateBonus();			// ボーナスの更新処理
				UpdateItem();			// アイテムの更新処理
				UpdateGauge();			// ゲージの更新処理
				UpdateGaugeFrame();		// ゲージ枠の更新処理（中身はないが形式上）
				{// エフェクトの更新処理
					UpdateChargeEffect();	//チャージエフェクト
					UpdateChargeCylinder();	//チャージエフェクト(しりんだー)
					UpdateAttackEffect();	//攻撃エフェクト
					UpdateTremorEffect();	//ヒップドロップエフェクト
				}
				UpdateParticle();	// パーティクルの更新処理
				UpdateScore();		//スコアの更新処理
				UpdateTime(MODE_PvPGAME);		//タイマーの更新処理
			}
		}
	}

	//コントローラーの使用チェックはポーズ状態関係なく呼び出し
	CheckUseController_PvP(CHECKMODE_DISCONPAUSE);

	//フォトモード切替
	if (GetKeyboardTrigger(DIK_F9))
	{
		g_bPhotoMode_PvP = g_bPhotoMode_PvP ? false : true;
	}
}

//------------------------------------------------
//				ゲームの描画処理
//------------------------------------------------
void DrawPvPGame(void)
{
	/*======================================================================
	各種オブジェクトの描画処理
	========================================================================*/
	for (int nCntCamera = 0; nCntCamera < NUM_CAMERA; nCntCamera++)
	{
		//ゲーム内オブジェクトの描画処理
		SetCamera(nCntCamera);		// カメラの設定処理

		//対象のプレイヤーが使われていたら表示
		if (GetPlayer()[nCntCamera].bUsePlayer)
		{
			DrawMeshfield();			// ステージの描画処理
			DrawMeshDome();				// メッシュドームの描画処理	
			DrawMeshFault();			// メッシュの断面描画処理
			DrawFence();				// フェンスの描画処理
			DrawObject();				// オブジェクト描画処理
			{// エフェクトの描画処理
				DrawChargeEffect();		//チャージエフェクト
				DrawChargeCylinder();	//チャージエフェクト(しりんだー)
				DrawAttackEffect();		//攻撃エフェクト
				DrawTremorEffect();		//ヒップドロップエフェクト
				DrawEff_shockWave_00();	//ダッシュ衝撃波エフェクト
			}
			DrawParticle();				// パーティクルの描画処理
			DrawPlayer();				// プレイヤーの描画処理
			DrawBonus();				// ボーナスの描画処理
			DrawItem();					// アイテムの描画処理
			DrawGaugeFrame();			// ゲージ枠の描画処理（ゲージより前に行うこと）
			DrawGauge();				// ゲージの描画処理
			DrawScore();				// スコアの描画処理
			DrawCameraFrame();			// 画面分割の枠描画処理
		}
		else
		{
			//幕を描画
			DrawCurtain(nCntCamera);
		}

		//タイマーの描画処理
		DrawTime();

		//ポーズがON
		if (g_bPause_PvP == true && g_bPhotoMode_PvP == false)
		{
			DrawPause();		//ポーズ画面描画処理
		}
	}
}

//------------------------------------------------
//		使用するコントローラーの設定
//		Author:石原颯馬
//		返り値:使用されている（接続されている）コントローラーの数
//		Memo:InitGameで呼び出し（GetUseControllerが呼び出される前に呼び出しすること）
//------------------------------------------------
int SetUseController_PvP(void)
{
	int nUseController = 0;
	for (int nCntController = 0; nCntController < MAX_USE_GAMEPAD; nCntController++)
	{
		if (GetUseGamepad(nCntController) == true)
		{//使用されている
			g_abUsePlayer_PvP[nCntController] = true;
			nUseController++;	//コントローラーの数追加
		}
		else
		{//使用されていない
			g_abUsePlayer_PvP[nCntController] = false;
		}
	}
	return nUseController;	//使用されているコントローラーの数を返す
}

//------------------------------------------------
//		使用しているコントローラーの接続チェック
//		Author:石原颯馬
//		引数1:チェックモード指定
//		CHECKMODE_DISCONNECT:切断されている場合強制ポーズ状態にするモード
//		CHECKMODE_REMOVE	:切断されている場合そのコントローラーを使用していない状態にする
//		Memo:UpdateGame内でポーズ中でも呼び出しすること
//------------------------------------------------
bool CheckUseController_PvP(CHECKMODE mode)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_USE_GAMEPAD; nCntPlayer++)
	{//対応コントローラー分繰り返す
		if (g_abUsePlayer_PvP[nCntPlayer] == true && g_abUsePlayer_PvP[nCntPlayer] != GetUseGamepad(nCntPlayer))
		{//切断を検知
			switch (mode)
			{
			case CHECKMODE_DISCONPAUSE:
				g_bPause_PvP = true;				//強制的にポーズ状態にする
				g_bDisconnectPlayer_PvP = true;		//切断されたことにする
				SetPadPause(true);
				return true;	//関数終了
				break;
			case CHECKMODE_DISCONNOPAUSE:
				g_bDisconnectPlayer_PvP = true;		//切断されたことにする
				return true;	//関数終了
				break;
			case CHECKMODE_REMOVE:
				g_abUsePlayer_PvP[nCntPlayer] = false;
				g_nUseContNum_PvP--;	//コントローラー使用数減らす
				break;
			default:
				break;
			}
		}
	}
	g_bDisconnectPlayer_PvP = false;	//for文が正常に終了したら問題ない
	return false;
}

//------------------------------------------------
//		コントローラーの使用有無取得処理
//		Author:石原颯馬
//		引数1:調べるパッドの番号
//------------------------------------------------
bool GetUseController_PvP(int nPadNum)
{
	return g_abUsePlayer_PvP[nPadNum];
}

//------------------------------------------------
//		コントローラーの使用個数取得処理
//		Author:石原颯馬
//------------------------------------------------
int GetUseControllerNum_PvP(void)
{
	return g_nUseContNum_PvP;
}

//------------------------------------------------
//		ポーズ状態の設定処理
//		Author:石原颯馬
//------------------------------------------------
void SetEnablePause_PvP(bool pause)
{
	g_bPause_PvP = pause;
}

//------------------------------------------------
//		カメラの数変更処理（デバック限定
//		Author:平澤詩苑
//------------------------------------------------
void ChangeNumCamera_PvP(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F7))
	{
		//現在のカメラの種類を保存
		int nType = g_NumCamera_PvP;

		//次の種類に変更
		nType++;

		//変更に伴い、全種類の数を超えないようにする
		nType %= NumCamera_MAX;

		//変更したものを入れる
		g_NumCamera_PvP = (NumCamera)nType;

		//カメラの種類を設定
		Set_NumCamera(g_NumCamera_PvP);
	}
#endif // _DEBUG
}