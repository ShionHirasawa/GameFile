//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Hirasawa Shion
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//�T�E���h�N���X
class CSound
{
public:
	//******************************************
	// �T�E���h�ꗗ
	//******************************************
	typedef enum
	{
		BGM_TITLE = 0,	// �^�C�g��BGM
		BGM_TUTORIAL,	// �`���[�g���A��BGM
		BGM_GAME,		// �Q�[��BGM
		BGM_RESULT,		// ���U���gBGM
		SE_SHOT,		// �e���ˉ�
		SE_EXPLOSION,	// ������
		SE_JEWEL_GET,	// ��Ύ擾��
		SE_SIREN_ROBOT,	// ���{�b�g�p�T�C����
		SE_SIREN_CAMERA,// �h�ƃJ�����p�T�C����
		SE_TIME_LIMIT,	// �^�C�����~�b�g�\����
		LABEL_MAX,
	} LABEL;

	//******************************************
	// �Đ�����
	//******************************************
	typedef enum
	{
		PLAY_LOOP = -1,	//���[�v����
		PLAY_ONCE = 0,	//�P���Đ�
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
	//�T�E���h�̒��f/�ĊJ����
	//------------------------
	//�����P�@�F���s�������T�E���h�̃��x��
	//�����Q�@�F���f/�ĊJ�@�̂ǂ��炩�@�@true�F�ĊJ  false�F���f
	//========================
	void	Coroutine(LABEL label, bool bPlay);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2				*m_pXAudio2 = NULL;			// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice	*m_pMasteringVoice = NULL;	// �}�X�^�[�{�C�X
	IXAudio2SourceVoice		*m_apSourceVoice[LABEL_MAX];// �\�[�X�{�C�X
	BYTE					*m_apDataAudio[LABEL_MAX];	// �I�[�f�B�I�f�[�^
	DWORD					m_aSizeAudio[LABEL_MAX];	// �I�[�f�B�I�f�[�^�T�C�Y
	static char				*m_pFilename[LABEL_MAX];	// �t�@�C����
	static int				m_nCntLoop[LABEL_MAX];		// ���[�v�J�E���g
	static float			m_fVolume[LABEL_MAX];		// �{�����[��
	static bool				m_bPlay;					// �Đ����邩�ۂ�
};

#endif