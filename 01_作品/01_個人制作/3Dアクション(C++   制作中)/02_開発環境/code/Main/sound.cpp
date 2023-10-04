//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : Hirasawa Shion
//
//=============================================================================
#include "main.h"
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
//�T�E���h�̃t�@�C���p�X
char *CSound::m_pFilename[LABEL_MAX] = {
	"data\\BGM\\Title.wav",		// �^�C�g��BGM
	"data\\BGM\\Tutorial.wav",	// �`���[�g���A��BGM
	"data\\BGM\\Game.wav",		// �Q�[��BGM
	"data\\BGM\\Result.wav",	// ���U���gBGM
	"data\\SE\\Shooting.wav",	// �e���ˉ�
	"data\\SE\\Explosion.wav",	//  ������
	"data\\SE\\jewel.wav",		// ��Ύ擾��
	"data\\SE\\Siren_Robot.wav",	// ���{�b�g�p�T�C����
	"data\\SE\\Siren_Camera.wav",	// �h�ƃJ�����p�T�C����
	"data\\SE\\TimeLimit.wav",		// �^�C�����~�b�g�\����
};

//�Đ�����
int CSound::m_nCntLoop[LABEL_MAX] = {
	CSound::PLAY_LOOP,	// �^�C�g��BGM		�i���[�v�Đ�
	CSound::PLAY_LOOP,	// �`���[�g���A��BGM�i���[�v�Đ�
	CSound::PLAY_LOOP,	// �Q�[��BGM		�i���[�v�Đ�
	CSound::PLAY_LOOP,	// ���U���gBGM		�i���[�v�Đ�
	CSound::PLAY_ONCE,	// �e���ˉ�		�i�P���Đ�
	CSound::PLAY_ONCE,	//  ������		�i�P���Đ�
	CSound::PLAY_ONCE,	// ��Ύ擾��	�i�P���Đ�
	CSound::PLAY_LOOP,	// ���{�b�g�p�T�C�����i���[�v�Đ�
	CSound::PLAY_LOOP,	// �h�ƃJ�����p�T�C�����i���[�v�Đ�
	CSound::PLAY_ONCE,	// �^�C�����~�b�g�\����	�i�P���Đ�
};

//�{�����[��
float CSound::m_fVolume[LABEL_MAX] = {
	0.7f,	// �^�C�g��BGM
	0.7f,	// �`���[�g���A��BGM
	0.7f,	// �Q�[��BGM
	0.7f,	// ���U���gBGM
	0.5f,	// �e���ˉ�
	0.5f,	//  ������
	0.9f,	// ��Ύ擾��
	1.0f,	// ���{�b�g�p�T�C����
	1.0f,	// �h�ƃJ�����p�T�C����
	1.0f,	// �^�C�����~�b�g�\����
};

/*�Đ����邩�ۂ�*/	bool CSound::m_bPlay = true;

//=======================================
//�R���X�g���N�^
//=======================================
CSound::CSound()
{
	//�����o�ϐ��N���A
	m_pXAudio2 = NULL;			// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	m_pMasteringVoice = NULL;	// �}�X�^�[�{�C�X

	for (int nCntSound = 0; nCntSound < CSound::LABEL_MAX; nCntSound++)
	{
		m_aSizeAudio[nCntSound] = NULL;		// �I�[�f�B�I�f�[�^�T�C�Y
		m_apDataAudio[nCntSound] = NULL;	// �I�[�f�B�I�f�[�^
		m_apSourceVoice[nCntSound] = NULL;	// �\�[�X�{�C�X
	}

#ifdef _DEBUG
	m_bPlay = false;	//�f�o�b�O���[�h�̎������Đ����Ȃ�
#endif
}

//=======================================
//�f�X�g���N�^
//=======================================
CSound::~CSound()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(m_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < CSound::LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(m_pFilename[nCntSound], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = m_nCntLoop[nCntSound];

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSound::Uninit(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < CSound::LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;
	
	if(m_pXAudio2 != NULL)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT CSound::Play(LABEL label)
{
	//�Đ����Ă悢
	if (m_bPlay)
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[label];
		buffer.pAudioData = m_apDataAudio[label];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_nCntLoop[label];

		// ��Ԏ擾
		m_apSourceVoice[label]->GetState(&xa2state);

		//���ʒ���
		m_apSourceVoice[label]->SetVolume(m_fVolume[label]);

		if (xa2state.BuffersQueued != 0)
		{// �Đ���
			// �ꎞ��~
			m_apSourceVoice[label]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			m_apSourceVoice[label]->FlushSourceBuffers();
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

		// �Đ�
		m_apSourceVoice[label]->Start(0);
	}

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void CSound::Stop(LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);
	m_apSourceVoice[label]->SetVolume(m_fVolume[label]);

	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void CSound::Stop(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < CSound::LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//========================
//�T�E���h�̒��f/�ĊJ����
//------------------------
//�����P�@�F���s�������T�E���h�̃��x��
//�����Q�@�F���f/�ĊJ�@�̂ǂ��炩	true�F�ĊJ  false�F���f
//========================
void CSound::Coroutine(LABEL label, bool bPlay)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		// �ĊJ����
		if(bPlay)	m_apSourceVoice[label]->Start(0);

		//���f
		else		m_apSourceVoice[label]->Stop(0);
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}