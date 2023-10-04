//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Hirasawa Shion
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//サウンドクラス
class CSound
{
public:
	//******************************************
	// サウンド一覧
	//******************************************
	typedef enum
	{
		BGM_TITLE = 0,	// タイトルBGM
		BGM_TUTORIAL,	// チュートリアルBGM
		BGM_GAME,		// ゲームBGM
		BGM_RESULT,		// リザルトBGM
		SE_SHOT,		// 弾発射音
		SE_EXPLOSION,	// 爆発音
		SE_JEWEL_GET,	// 宝石取得音
		SE_SIREN_ROBOT,	// ロボット用サイレン
		SE_SIREN_CAMERA,// 防犯カメラ用サイレン
		SE_TIME_LIMIT,	// タイムリミット予告音
		LABEL_MAX,
	} LABEL;

	//******************************************
	// 再生属性
	//******************************************
	typedef enum
	{
		PLAY_LOOP = -1,	//ループ生成
		PLAY_ONCE = 0,	//単発再生
		PLAY_MAX = 2,
	}PLAY_ATR;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void	Uninit(void);
	HRESULT Play(LABEL label);
	void	Stop(LABEL label);
	void	Stop(void);

	//========================
	//サウンドの中断/再開処理
	//------------------------
	//引数１　：実行したいサウンドのラベル
	//引数２　：中断/再開　のどちらか　　true：再開  false：中断
	//========================
	void	Coroutine(LABEL label, bool bPlay);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2				*m_pXAudio2 = NULL;			// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice	*m_pMasteringVoice = NULL;	// マスターボイス
	IXAudio2SourceVoice		*m_apSourceVoice[LABEL_MAX];// ソースボイス
	BYTE					*m_apDataAudio[LABEL_MAX];	// オーディオデータ
	DWORD					m_aSizeAudio[LABEL_MAX];	// オーディオデータサイズ
	static char				*m_pFilename[LABEL_MAX];	// ファイル名
	static int				m_nCntLoop[LABEL_MAX];		// ループカウント
	static float			m_fVolume[LABEL_MAX];		// ボリューム
	static bool				m_bPlay;					// 再生するか否か
};

#endif