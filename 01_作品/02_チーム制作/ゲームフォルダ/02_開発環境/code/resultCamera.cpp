/*==========================================================================================================================================================

リザルト用カメラ処理[resultCamera.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "debugproc.h"
#include "resultCamera.h"
#include "resultPlayer.h"
#include "conversioninput.h"
#include "VictoryStand.h"
#include "sound.h"

//マクロ定義
#define POS_POSV			D3DXVECTOR3(0.0f, 30.0f, -200.0f)	//視点の初期位置
#define POS_POSR			D3DXVECTOR3(0.0f, 30.0f, 0.0f)		//注視点の初期位置
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//上方向ベクトルの初期値
#define VIEW_ANGLE			(45.0f)								//視野角
#define MOVE_CAMERA_SPEED	(3.0f)								//カメラ・注視点座標の移動量
#define SPIN_CAMERA_SPEED	(0.03f)								//カメラの回転スピード
#define MIN_DRAW_Z			(10.0f)								//描画処理の最低奥行き
#define MAX_DRAW_Z			(2000.0f)							//描画処理の最高奥行き
#define MIN_LENGTH			(100.0f)							//視点と注視点の最低距離
#define MAX_LENGTH			(300.0f)							//視点と注視点の最大距離
#define CHANGE_LENGHT		(0.5f)								//長さの移動量
#define SPIN_ROT			(0.05f)								//回転量
#define COR_ROT_POSV		(3.14f)								//角度計算による視点操作の調整値
#define COR_ROT_POSR		(0.0f)								//角度計算による注視点操作の調整値
#define DRAW_LENGTH			(500.0f)							//オブジェクトを描画させてあげる範囲
#define WAVE_CAMERA_INFO	"data\\CSV\\WaveCamera.csv"			//アニメーションカメラの情報ファイル

//特殊演出のマクロ
#define SP_PLAYER_DIVE_TIME		(80)		//プレイヤーが飛び込む時間（ウェーブ２の硬直フレーム中
#define SP_WAVE_03_START_Y		(100.0f)	//ウェーブ３開始時のプレイヤーＹ位置調整
#define SP_WAVE_LAST_START_Y	(100.0f)	//ウェーブ最後開始時のプレイヤーＹ位置調整

//グローバル変数
ResultCamera		g_ResultCamera;						//カメラの情報
WAVECamera			g_WaveCamera;						//カメラのウェーブ情報
AnimResCamera		g_AnimResCamera[WAVECamera_MAX];	//アニメーションカメラの情報
bool				g_bFinishSound = false;				//最後のBGMを流していない
int					g_nAnimTime = 0;					//上映時間を計算

//------------------------------------------------
//			リザルト用カメラの初期化処理
//------------------------------------------------
void InitResultCamera(void)
{
	//アニメーション用カメラ情報の初期化
	LoadWaveCamera();

	g_ResultCamera.vecU = POS_VECU;			//上方向ベクトル
	g_WaveCamera = WAVECamera_01_SideWays;	//ウェーブ情報初期化
	SetNextWave(WAVECamera_01_SideWays);	//初期位置設定
	g_nAnimTime = 0;						//上映時間を初期化
	g_bFinishSound = false;					//最後のBGMを流していない

	//---------------------------------
	//カメラの角度や距離を算出
	//---------------------------------
	{
		g_ResultCamera.rot.y = 0.0f;// 角度設定

		float PosDiffX, PosDiffZ;

		PosDiffX = powf(POS_POSR.x - POS_POSV.x, 2.0f);	//2乗
		PosDiffZ = powf(POS_POSR.z - POS_POSV.z, 2.0f);	//2乗

		g_ResultCamera.fLength = sqrtf(PosDiffX + PosDiffZ);	//長さを算出
	}
}

//リザルト用カメラのアニメーション情報初期化
void LoadWaveCamera(void)
{
	FILE *pFile;
	char aData[256];
	AnimResCamera *pAnimCamera = &g_AnimResCamera[0];

	//ファイル読み込み
	pFile = fopen(WAVE_CAMERA_INFO, "r");

	//ファイルが読み込めている
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[0]);	//最初のセルの項目名読み込み

		for (int nCntWave = 0; nCntWave < WAVECamera_FINISH; nCntWave++, pAnimCamera++)
		{
			//一行読み込み
			fscanf(pFile, "%s", &aData[0]);

			//経過フレーム初期化
			pAnimCamera->nFrameCounter = 0;

			//全体フレーム
			pAnimCamera->nWholeFrame = atoi(strtok(&aData[0], ","));

			//フレーム到達判定初期化
			pAnimCamera->bEnd = false;

			//硬直フレーム初期化
			pAnimCamera->nWaitCount = 0;

			//全体硬直フレーム
			pAnimCamera->WaitTime = atoi(strtok(NULL, ","));

			//**************************************************
			//					視点	開始位置
			//**************************************************
			pAnimCamera->StartPosV.x = atof(strtok(NULL, ","));
			pAnimCamera->StartPosV.y = atof(strtok(NULL, ","));
			pAnimCamera->StartPosV.z = atof(strtok(NULL, ","));

			//**************************************************
			//				   注視点	開始位置
			//**************************************************
			pAnimCamera->StartPosR.x = atof(strtok(NULL, ","));
			pAnimCamera->StartPosR.y = atof(strtok(NULL, ","));
			pAnimCamera->StartPosR.z = atof(strtok(NULL, ","));

			//**************************************************
			//					視点	目標位置
			//**************************************************
			pAnimCamera->OffsetPosV.x = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosV.y = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosV.z = atof(strtok(NULL, ","));

			//**************************************************
			//					注視点	目標位置
			//**************************************************
			pAnimCamera->OffsetPosR.x = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosR.y = atof(strtok(NULL, ","));
			pAnimCamera->OffsetPosR.z = atof(strtok(NULL, ","));

			//**************************************************
			//					視点	移動量
			//**************************************************
			pAnimCamera->movePosV.x = (pAnimCamera->OffsetPosV.x - pAnimCamera->StartPosV.x) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosV.y = (pAnimCamera->OffsetPosV.y - pAnimCamera->StartPosV.y) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosV.z = (pAnimCamera->OffsetPosV.z - pAnimCamera->StartPosV.z) / pAnimCamera->nWholeFrame;

			//**************************************************
			//					注視点	移動量
			//**************************************************
			pAnimCamera->movePosR.x = (pAnimCamera->OffsetPosR.x - pAnimCamera->StartPosR.x) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosR.y = (pAnimCamera->OffsetPosR.y - pAnimCamera->StartPosR.y) / pAnimCamera->nWholeFrame;
			pAnimCamera->movePosR.z = (pAnimCamera->OffsetPosR.z - pAnimCamera->StartPosR.z) / pAnimCamera->nWholeFrame;
		}
	}
}

//------------------------------------------------
//			リザルト用カメラの終了処理
//------------------------------------------------
void UninitResultCamera(void)
{

}

//------------------------------------------------
//			リザルト用カメラの更新処理
//------------------------------------------------
void UpdateResultCamera(void)
{
	//カメラのウェーブ処理
	WaveResultCamera();

	//ウェーブ別の特別処理
	SpecialWave();
	
	int nWave = g_WaveCamera;
	ResultCamera Camera = g_ResultCamera;
	AnimResCamera AnimCam = g_AnimResCamera[nWave];
}

//カメラのアニメーション処理
void WaveResultCamera(void)
{
	//ウェーブが終了していない
	if (g_WaveCamera != WAVECamera_FINISH)
	{
		int nWave = g_WaveCamera;								//現在のウェーブを取得
		ResultCamera *pCamera = &g_ResultCamera;				//カメラのポインタ取得
		AnimResCamera *pAnimCamera = &g_AnimResCamera[nWave];	//アニメーション用カメラのポインタ

		//未だ全体フレームに到達していない
		if (!pAnimCamera->bEnd)
		{
			//視点・注視点移動
			pCamera->posV += pAnimCamera->movePosV;
			pCamera->posR += pAnimCamera->movePosR;

			//経過フレーム加算
			pAnimCamera->nFrameCounter++;

			//経過フレームが全体フレームに達した
			if (pAnimCamera->nWholeFrame <= pAnimCamera->nFrameCounter)
			{
				//全体フレームに到達した
				pAnimCamera->bEnd = true;
			}
		}
		//全体フレームに到達している
		else
		{
			//硬直フレーム加算
			pAnimCamera->nWaitCount++;

			//硬直フレームが終わった
			if (pAnimCamera->WaitTime <= pAnimCamera->nWaitCount)
			{
				//次のウェーブへ
				g_WaveCamera = (WAVECamera)(g_WaveCamera + 1);

				//ウェーブが終了していない
				if (g_WaveCamera != WAVECamera_FINISH)
				{
					//次のアニメーションカメラへ
					pAnimCamera++;

					//次のウェーブへ
					SetNextWave(g_WaveCamera);
				}
			}
		}
	}
}

//ウェーブ別の特別処理
void SpecialWave(void)
{
	int nWave = g_WaveCamera;							//現在のウェーブを取得
	AnimResCamera AnimCamera = g_AnimResCamera[nWave];	//アニメーション用カメラのポインタ

	//----------------------------------------------
	//			ウェーブ別  特殊 設定
	//----------------------------------------------
	switch (g_WaveCamera)
	{
	//*******************************
	//	ウェーブ２の場合の特殊処理
	//*******************************
	case WAVECamera_02_BirdEyes:
	{
		//--------------------------------------------
		//			ウェーブ２の初期設定
		// MEMO:硬直フレーム中　一定間隔でプレイヤーを飛び込ませる
		//--------------------------------------------
		//硬直フレームが飛び込み時間になった
		if (AnimCamera.nWaitCount % SP_PLAYER_DIVE_TIME == 0 && AnimCamera.nWaitCount > 0)
		{
			//プレイヤーを１体ずつ飛び込ませる
			SetDivePlayer();
		}
	}
	break;

	//*******************************
	//	ウェーブ３の場合の特殊処理
	//*******************************
	case WAVECamera_03_SkyDiving:
	{
		//--------------------------------------------
		//			ウェーブ３の初期設定
		// MEMO:プレイヤーの開始位置を設定する
		//--------------------------------------------
		if (AnimCamera.nFrameCounter == 0)
		{
			//プレイヤーの初期位置を変える
			for (int nCntSpWave = 0; nCntSpWave < MAX_USE_GAMEPAD; nCntSpWave++)
			{
				//注視点の開始位置　+　調整量　の場所にプレイヤーをセット
				GetPlayer_RESULT()[nCntSpWave].pos.y = AnimCamera.StartPosR.y + SP_WAVE_03_START_Y + (nCntSpWave * SP_WAVE_03_START_Y);
			}
		}
	}
	break;

	//*******************************
	//	ウェーブ４の場合の特殊処理
	//*******************************
	case WAVECamera_LAST_VictoryStand:
	{
		//--------------------------------------------
		//			ウェーブ４の初期設定
		// MEMO:プレイヤーの開始位置を設定する
		//		表彰台を登場させる
		//--------------------------------------------
		if (AnimCamera.nFrameCounter == 0)
		{
			//プレイヤーの情報取得
			Player_RESULT *pPlayer = GetPlayer_RESULT();

			//プレイヤーの初期情報設定
			for (int nCntSpWave = 0; nCntSpWave < MAX_USE_GAMEPAD; nCntSpWave++, pPlayer++)
			{
				//注視点の開始位置　+　調整量　の場所にプレイヤーをセット
				pPlayer->pos.y = AnimCamera.StartPosR.y + SP_WAVE_LAST_START_Y + (nCntSpWave * SP_WAVE_LAST_START_Y);
				pPlayer->pos.z = 0.0f;
				pPlayer->bDive = true;
				pPlayer->bHipDrop = true;
			}

			//表彰台を登場させる
			SetAppearVictoryStand();
		}
	}
	break;

	//*******************************
	//	フィニッシュウェーブの特殊処理
	//*******************************
	case WAVECamera_FINISH:
	{
		//最後の音楽を流していない
		if (!g_bFinishSound)
		{
			StopSoundBGM(SOUND_LABEL_BGM_RESULT_01);	//開始リザルトBGM停止
			PlaySoundBGM(SOUND_LABEL_BGM_RESULT_02);	//余韻リザルトBGM再生
			g_bFinishSound = true;						//サウンドを再生した
		}
	}
		break;
	}
}

//次のウェーブへ
void SetNextWave(int nWave)
{
	ResultCamera *pCamera = &g_ResultCamera;
	AnimResCamera AnimCamera = g_AnimResCamera[nWave];

	//視点・注視点を初期位置へ
	pCamera->posV = AnimCamera.StartPosV;
	pCamera->posR = AnimCamera.StartPosR;
}

//------------------------------------------------
//			リザルト用カメラの設定処理
//------------------------------------------------
void SetResultCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

												//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&g_ResultCamera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_ResultCamera.mtxProjection, D3DXToRadian(VIEW_ANGLE), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_Z, MAX_DRAW_Z);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_ResultCamera.mtxProjection);

	//ビューマトリックス
	D3DXMatrixIdentity(&g_ResultCamera.mtxview);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_ResultCamera.mtxview,
		&g_ResultCamera.posV,		//視点
		&g_ResultCamera.posR,		//注視点
		&g_ResultCamera.vecU);	//上方向ベクトル

							//ビューマトリックス
	pDevice->SetTransform(D3DTS_VIEW, &g_ResultCamera.mtxview);
}

//リザルト用カメラの情報取得
ResultCamera *GetResultCamera(void)
{
	return &g_ResultCamera;
}

//ウェーブの状態を返す
WAVECamera *GetWaveCamera(void)
{
	return &g_WaveCamera;
}