//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : 石原颯馬  平澤詩苑
// ほとんどのプログラム俺らが作ったね　どうしてだろうね？ << 石原クン
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SOUND_LOOP		(-1)		//ループする
#define SOUND_ONCE		(0)			//１回かぎり

//*****************************************************************************
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
	float Volume;		// ボリューム
	bool bPlay;			//再生するかどうか
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

HRESULT InitSoundBGM(HWND hWnd);
HRESULT InitSoundSE(HWND hWnd);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//BGM
IXAudio2 *g_pXAudio2BGM = NULL;										// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoiceBGM = NULL;				// マスターボイス
IXAudio2SourceVoice *g_apSourceVoiceBGM[SOUND_LABEL_BGM_MAX] = {};	// ソースボイス
BYTE *g_apDataAudioBGM[SOUND_LABEL_BGM_MAX] = {};	// オーディオデータ
DWORD g_aSizeAudioBGM[SOUND_LABEL_BGM_MAX] = {};	// オーディオデータサイズ

//SE
IXAudio2 *g_pXAudio2SE = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoiceSE = NULL;			// マスターボイス
IXAudio2SourceVoice *g_apSourceVoiceSE[SOUND_LABEL_SE_MAX][NUM_DUPLICATION] = { {} };	// ソースボイス
BYTE *g_apDataAudioSE[SOUND_LABEL_SE_MAX][NUM_DUPLICATION] = { {} };	// オーディオデータ
DWORD g_aSizeAudioSE[SOUND_LABEL_SE_MAX][NUM_DUPLICATION] = { {} };	// オーディオデータサイズ

//その他
bool g_bAllPlaySound = true;	// 再生するかどうか

// BGMサウンドの情報
SOUNDINFO g_aSoundInfoBGM[SOUND_LABEL_BGM_MAX] =
{
	{ "data/SOUND/BGM/Title.wav",			SOUND_LOOP, 0.7f,true },		// タイトル音
	{ "data/SOUND/BGM/Tutorial.wav",		SOUND_LOOP, 0.7f,true },		// チュートリアル音
	{ "data/SOUND/BGM/PvP_Game_Mixed.wav",	SOUND_LOOP, 0.7f,true },		// ゲーム音
	{ "data/SOUND/BGM/Game000.wav",			SOUND_LOOP, 0.7f,true },		// ゲーム音
	{ "data/SOUND/BGM/Result_01.wav",		SOUND_LOOP, 0.7f,true },		// リザルト音1（開始
	{ "data/SOUND/BGM/Result_02.wav",		SOUND_LOOP, 0.7f,true },		// リザルト音2（余韻
};

// SEサウンドの情報
SOUNDINFO g_aSoundInfoSE[SOUND_LABEL_SE_MAX] =
{
	{ "data/SOUND/SE/Warning-fence.wav",		 SOUND_ONCE, 0.5f, true },	// フェンス落下注意効果音
	{ "data/SOUND/SE/PvP_Game_CountDown.wav",	 SOUND_ONCE, 0.5f, true },	// カウントダウン効果音
	{ "data/SOUND/SE/Uribou.wav",				 SOUND_ONCE, 0.5f, true },	// コントローラー接続SE
	{ "data/SOUND/SE/Title_Decide.wav",			 SOUND_ONCE, 0.6f, true },	// タイトルの決定SE
	{ "data/SOUND/SE/Collision.wav",			 SOUND_ONCE, 0.6f, true },	// プレイヤー同士の衝突音SE
	{ "data/SOUND/SE/Energy.wav",				 SOUND_ONCE, 0.6f, true },	// プレイヤーのゲージ充填音
	{ "data/SOUND/SE/Drop000.wav",				 SOUND_ONCE, 0.6f, true },	// プレイヤーの落下音
	{ "data/SOUND/SE/GrassDash000.wav",			 SOUND_ONCE, 0.6f, true },	// プレイヤーのダッシュ音
	{ "data/SOUND/SE/HipdropSpin_001.wav",		 SOUND_ONCE, 0.6f, true },	// プレイヤーのヒップドロップスピン音
	{ "data/SOUND/SE/HipDrop_Attack_002.wav",	 SOUND_ONCE, 0.6f, true },	// プレイヤーのヒップドロップ音
	{ "data/SOUND/SE/Jamp001.wav",				 SOUND_ONCE, 0.4f, true },	// プレイヤーのジャンプ音
	{ "data/SOUND/SE/HipDropRank00_Bad.wav",	 SOUND_ONCE, 0.5f, true },	// ヒップドロップランク 「Bad...」の効果音
	{ "data/SOUND/SE/HipDropRank01_Nice.wav",	 SOUND_ONCE, 0.5f, true },	//			〃			「NICE」
	{ "data/SOUND/SE/HipDropRank02_Good.wav",	 SOUND_ONCE, 0.5f, true },	//			〃			「GOOD!」
	{ "data/SOUND/SE/HipDropRank03_Great.wav",	 SOUND_ONCE, 0.5f, true },	//			〃			「☆GREAT☆」
	{ "data/SOUND/SE/HipDropRank04_PERFECT.wav", SOUND_ONCE, 0.4f, true },	//			〃			「★PERFECT★」
	{ "data/SOUND/SE/piano_00_Do.wav",			 SOUND_ONCE, 0.4f, true },	// ブロック破壊	１個目の効果音
	{ "data/SOUND/SE/piano_01_Re.wav",			 SOUND_ONCE, 0.4f, true },	//		〃		２個目
	{ "data/SOUND/SE/piano_02_Mi.wav",			 SOUND_ONCE, 0.4f, true },	//		〃		３個目
	{ "data/SOUND/SE/piano_03_Fa.wav",			 SOUND_ONCE, 0.5f, true },	//		〃		４個目
	{ "data/SOUND/SE/piano_04_So.wav",			 SOUND_ONCE, 0.5f, true },	//		〃		５個目
	{ "data/SOUND/SE/piano_05_Ra.wav",			 SOUND_ONCE, 0.5f, true },	//		〃		６個目
	{ "data/SOUND/SE/piano_06_Ti.wav",			 SOUND_ONCE, 0.6f, true },	//		〃		７個目	英語で「シ」は「Ti」と書くらしいです
	{ "data/SOUND/SE/piano_07_Do.wav",			 SOUND_ONCE, 0.7f, true },	//		〃		８個目
	{ "data/SOUND/SE/PauseDecision000.wav",		 SOUND_ONCE, 0.6f, true },	// ポーズの決定音
	{ "data/SOUND/SE/PauseSelection000.wav",	 SOUND_ONCE, 0.6f, true },	// ポーズの選択音
	{ "data/SOUND/SE/PauseTransition000.wav",	 SOUND_ONCE, 0.6f, true },	// ポーズの遷移音
	{ "data/SOUND/SE/Roar000.wav",				 SOUND_ONCE, 0.6f, true },	// ポーズの咆哮音
	{ "data/SOUND/SE/Cheers.wav",				 SOUND_ONCE, 0.6f, true },	// リザルトの歓声
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	//BGM
	if (FAILED(InitSoundBGM(hWnd)))
	{
		return E_FAIL;
	}

	//SE
	if (FAILED(InitSoundSE(hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//BGMの初期化処理
HRESULT InitSoundBGM(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2BGM, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = g_pXAudio2BGM->CreateMasteringVoice(&g_pMasteringVoiceBGM);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (g_pXAudio2BGM != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2BGM->Release();
			g_pXAudio2BGM = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_BGM_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfoBGM[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			// オーディオデータ読み込み
			hr = CheckChunk(hFile, 'atad', &g_aSizeAudioBGM[nCntSound], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			g_apDataAudioBGM[nCntSound] = (BYTE*)malloc(g_aSizeAudioBGM[nCntSound]);
			hr = ReadChunkData(hFile, g_apDataAudioBGM[nCntSound], g_aSizeAudioBGM[nCntSound], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// ソースボイスの生成
			hr = g_pXAudio2BGM->CreateSourceVoice(&g_apSourceVoiceBGM[nCntSound], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// バッファの値設定
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = g_aSizeAudioBGM[nCntSound];
			buffer.pAudioData = g_apDataAudioBGM[nCntSound];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = g_aSoundInfoBGM[nCntSound].nCntLoop;

			// オーディオバッファの登録
			g_apSourceVoiceBGM[nCntSound]->SubmitSourceBuffer(&buffer);
		}

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//SEの初期化処理
HRESULT InitSoundSE(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2SE, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = g_pXAudio2SE->CreateMasteringVoice(&g_pMasteringVoiceSE);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (g_pXAudio2SE != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2SE->Release();
			g_pXAudio2SE = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_SE_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfoSE[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			// オーディオデータ読み込み
			hr = CheckChunk(hFile, 'atad', &g_aSizeAudioSE[nCntSound][nCntDupl], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			g_apDataAudioSE[nCntSound][nCntDupl] = (BYTE*)malloc(g_aSizeAudioSE[nCntSound][nCntDupl]);
			hr = ReadChunkData(hFile, g_apDataAudioSE[nCntSound][nCntDupl], g_aSizeAudioSE[nCntSound][nCntDupl], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// ソースボイスの生成
			hr = g_pXAudio2SE->CreateSourceVoice(&g_apSourceVoiceSE[nCntSound][nCntDupl], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// バッファの値設定
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = g_aSizeAudioSE[nCntSound][nCntDupl];
			buffer.pAudioData = g_apDataAudioSE[nCntSound][nCntDupl];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = g_aSoundInfoSE[nCntSound].nCntLoop;

			// オーディオバッファの登録
			g_apSourceVoiceSE[nCntSound][nCntDupl]->SubmitSourceBuffer(&buffer);
		}

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	//BGM
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_BGM_MAX; nCntSound++)
	{
		if (g_apSourceVoiceBGM[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoiceBGM[nCntSound]->Stop(0);

			// ソースボイスの破棄
			g_apSourceVoiceBGM[nCntSound]->DestroyVoice();
			g_apSourceVoiceBGM[nCntSound] = NULL;

			// オーディオデータの開放
			free(g_apDataAudioBGM[nCntSound]);
			g_apDataAudioBGM[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoiceBGM->DestroyVoice();
	g_pMasteringVoiceBGM = NULL;
	
	// XAudio2オブジェクトの開放
	if(g_pXAudio2BGM != NULL)
	{
		g_pXAudio2BGM->Release();
		g_pXAudio2BGM = NULL;
	}


	//SE
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_SE_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceSE[nCntSound] != NULL)
			{
				// 一時停止
				g_apSourceVoiceSE[nCntSound][nCntDupl]->Stop(0);

				// ソースボイスの破棄
				g_apSourceVoiceSE[nCntSound][nCntDupl]->DestroyVoice();
				g_apSourceVoiceSE[nCntSound][nCntDupl] = NULL;

				// オーディオデータの開放
				free(g_apDataAudioSE[nCntSound][nCntDupl]);
				g_apDataAudioSE[nCntSound][nCntDupl] = NULL;
			}
		}
	}

	// マスターボイスの破棄
	g_pMasteringVoiceSE->DestroyVoice();
	g_pMasteringVoiceSE = NULL;

	// XAudio2オブジェクトの開放
	if (g_pXAudio2SE != NULL)
	{
		g_pXAudio2SE->Release();
		g_pXAudio2SE = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// 全サウンドの停止処理
//=============================================================================
void StopAllSound(void)
{
	// BGM 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_BGM_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceBGM[nCntSound] != NULL)
			{
				// 一時停止
				g_apSourceVoiceBGM[nCntSound]->Stop(0);
			}
		}
	}

	// SE 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_SE_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceSE[nCntSound][nCntDupl] != NULL)
			{
				// 一時停止
				g_apSourceVoiceSE[nCntSound][nCntDupl]->Stop(0);
			}
		}
	}
}

//BGM
//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySoundBGM(SOUND_LABEL_BGM label)
{
	//ラベルの再生がON		かつ		再生がON
	if (g_aSoundInfoBGM[label].bPlay == true && g_bAllPlaySound == true)
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudioBGM[label];
		buffer.pAudioData = g_apDataAudioBGM[label];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfoBGM[label].nCntLoop;

		// 状態取得
		g_apSourceVoiceBGM[label]->GetState(&xa2state);

		//音量調整
		g_apSourceVoiceBGM[label]->SetVolume(g_aSoundInfoBGM[label].Volume);

		// 再生中
		if (xa2state.BuffersQueued != 0)
		{
			// 一時停止
			g_apSourceVoiceBGM[label]->Stop(0);

			// オーディオバッファの削除
			g_apSourceVoiceBGM[label]->FlushSourceBuffers();
		}

		// オーディオバッファの登録
		g_apSourceVoiceBGM[label]->SubmitSourceBuffer(&buffer);

		// 再生
		g_apSourceVoiceBGM[label]->Start(0);
	}

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSoundBGM(SOUND_LABEL_BGM label)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		// 状態取得
		g_apSourceVoiceBGM[label]->GetState(&xa2state);
		g_apSourceVoiceBGM[label]->SetVolume(g_aSoundInfoBGM[label].Volume);

		if (xa2state.BuffersQueued != 0)
		{// 再生中
		 // 一時停止
			g_apSourceVoiceBGM[label]->Stop(0);

			// オーディオバッファの削除
			g_apSourceVoiceBGM[label]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSoundBGM(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_BGM_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceBGM[nCntSound] != NULL)
			{
				// 一時停止
				g_apSourceVoiceBGM[nCntSound]->Stop(0);
			}
		}
	}
}

//=============================================================================
// セグメント一時停止(ラベル指定)
//=============================================================================
void PauseSound(SOUND_LABEL_BGM label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoiceBGM[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		g_apSourceVoiceBGM[label]->Stop(0);
	}
}

//=============================================================================
// セグメント再開(ラベル指定)
//=============================================================================
void RestartSound(SOUND_LABEL_BGM label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoiceBGM[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 一時停止中
	 // 再生
		g_apSourceVoiceBGM[label]->Start(0);
	}
}

//SE
//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySoundSE(SOUND_LABEL_SE label, int nNumSound)
{
	//サウンド番号を算出
	nNumSound %= NUM_DUPLICATION;

	//ラベルの再生がON		かつ		再生がON
	if (g_aSoundInfoSE[label].bPlay == true && g_bAllPlaySound == true)
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudioSE[label][nNumSound];
		buffer.pAudioData = g_apDataAudioSE[label][nNumSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aSoundInfoSE[label].nCntLoop;

		// 状態取得
		g_apSourceVoiceSE[label][nNumSound]->GetState(&xa2state);

		//音量調整
		g_apSourceVoiceSE[label][nNumSound]->SetVolume(g_aSoundInfoSE[label].Volume);

		// 再生中
		if (xa2state.BuffersQueued != 0)
		{
			// 一時停止
			g_apSourceVoiceSE[label][nNumSound]->Stop(0);

			// オーディオバッファの削除
			g_apSourceVoiceSE[label][nNumSound]->FlushSourceBuffers();
		}

		// オーディオバッファの登録
		g_apSourceVoiceSE[label][nNumSound]->SubmitSourceBuffer(&buffer);

		// 再生
		g_apSourceVoiceSE[label][nNumSound]->Start(0);
	}

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSoundSE(SOUND_LABEL_SE label)
{
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
	{
		// 状態取得
		g_apSourceVoiceSE[label][nCntDupl]->GetState(&xa2state);
		g_apSourceVoiceSE[label][nCntDupl]->SetVolume(g_aSoundInfoSE[label].Volume);

		if (xa2state.BuffersQueued != 0)
		{// 再生中
		 // 一時停止
			g_apSourceVoiceSE[label][nCntDupl]->Stop(0);

			// オーディオバッファの削除
			g_apSourceVoiceSE[label][nCntDupl]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSoundSE(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_SE_MAX; nCntSound++)
	{
		for (int nCntDupl = 0; nCntDupl < NUM_DUPLICATION; nCntDupl++)
		{
			if (g_apSourceVoiceSE[nCntSound][nCntDupl] != NULL)
			{
				// 一時停止
				g_apSourceVoiceSE[nCntSound][nCntDupl]->Stop(0);
			}
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}