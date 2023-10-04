//================================================================================================
//
//���C������[main.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "main.h"
#include "manager.h"
#include "renderer.h"

//�}�N����`
#define WINDOW_NAME	"Hide"	//�E�C���h�E�̖��O�i�L���v�V�����ɕ\���j

#define HALF_SEC	(500)		//0.5�b
#define ONE_FLAME	(1000 / 60)	//�P�t���[��

//----------------------------
//�t���X�N���[���ݒ�
//�����P	bFull�F�t���X�N���[���؂�ւ��i�t���X�N���[���ɂ���ꍇ�A�^�X�N�o�[�������j�@true:�t���X�N�@false:�ʏ�E�B���h�E
//�����Q	hWnd �F�E�B���h�E�X�e�[�^�X���
//----------------------------
void SetFullScreen(bool bFull, HWND hWnd);

//========================================
// ShowTaskBar�֐� - �^�X�N�o�[�̕\��/��\����؂�ւ��� -
//----------------------------
//�����P	bShow�F�^�X�N�o�[�\���X�e�[�^�X�@true:�\������@false:��\���ɂ���
//�����Q	hWnd �F�E�B���h�E�X�e�[�^�X���
// �Q�l: https://dixq.net/forum/viewtopic.php?t=12505
//----------------------------
void ShowTaskBar(bool bShow, HWND hWnd);

//======================================================================================================================
//														���C���֐�
//======================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//���������[�N���N�����Ă��邩���肷��iexe �����ƃ��[�N���Ă��郁��������������
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int		nCountFPS = 0;		// FPS�J�E���^
	CManager *pManager = NULL;	//�}�l����񐶐�
	DWORD dwCurrentTime;		//���ݎ���
	DWORD dwExecLastTime;		//�Ō�ɏ�����������
	DWORD dwFrameCount;			//�t���[���J�E���g
	DWORD dwFPSLastTime;		//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						//�E�C���h�E�̃X�^�C��
		WindowProc,						//�E�C���h�E�v���V�[�W��
		NULL,							//�O�ɂ���i�ʏ�͎g�p���Ȃ��j
		NULL,							//�O�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,						//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	//�^�X�N�o�[�̃A�C�R��
		LoadIcon(NULL,IDC_ARROW),		//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		//�N���C�A���g�̈�̔w�i�F
		NULL,							//���j���[�o�[
		CLASS_NAME,						//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),	//�t�@�C���̃A�C�R��
	};

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	//��ʃT�C�Y�̍\����
	RegisterClassEx(&wcex);								//�E�C���h�E�N���X�̓o�^

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	const HWND hWnd = CreateWindowEx(0,//�g���E�C���h�E�X�^�C��
		CLASS_NAME,					//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,				//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,				//�E�C���h�E�̍���w���W
		CW_USEDEFAULT,				//�E�C���h�E�̍���x���W
		(rect.right - rect.left),	//�E�C���h�E�̕�
		(rect.bottom - rect.top),	//�E�C���h�E�̍���
		NULL,						//�e�E�C���h�E�̃n���h��
		NULL,						//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,					//�C���X�^���g�n���h��
		NULL);						//�E�C���h�E�쐬�f�[�^

	SetFullScreen(true, hWnd);	//�S��ʐݒ�
	ShowWindow(hWnd, nCmdShow);	//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);			//�N���C�A���g�̈���X�V

	pManager = new CManager;		//�}�l����񐶐�
	pManager->Init(hInstance, hWnd, TRUE);//�}�l����񏉊���

	//����\��ݒ�
	timeBeginPeriod(1);

	//�`�揈���֘A�̕ϐ��̏�����
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	//FPS�֘A�̕ϐ��̏�����
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���i�[����ϐ�
	MSG mSg;

	//���b�Z�[�W���[�v
	while (true)
	{
		if (PeekMessage(&mSg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (mSg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&mSg);	//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&mSg);	//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= HALF_SEC)
			{//0.5�b�o��
			 //FPS���v��
				nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;				//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= ONE_FLAME)
			{//60����1�b�o��
			 //�����J�n�̎���[���ݎ���]��ۑ�
				dwExecLastTime = dwCurrentTime;

				//�X�V����
				pManager->Update();

				//FPS�J�E���^��n��
				CManager::GetDebugProc()->DebugPrint("FPS�F%d\n", nCountFPS);

				//�`�揈��
				pManager->Draw();

				//�t���[���J�E���g�����Z
				dwFrameCount++;
			}
		}
	}

	//�}�l����񂪐�������Ă���
	if (pManager != NULL)
	{
		pManager->Uninit();	//�I������
		delete pManager;	//�������폜
		pManager = NULL;	//NULL�|�C���^�ɂ���
	}

	//����\��߂�
	timeEndPeriod(1);

	//�^�X�N�o�[�\��
	ShowTaskBar(true, hWnd);

	//�E�C���h�E�N���X�̓o�^�̉���
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)mSg.wParam;
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�Ԃ�l���i�[
	int nID;

	switch (uMsg)
	{
		//�E�C���h�E�j���̃��b�Z�[�W
	case WM_DESTROY:
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:			//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
			//[ESC]�L�[�������ꂽ
		case VK_ESCAPE:
			//�I�����邩�̊m�F
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				MessageBox(NULL, "�܂����Ă��������ˁ`", "������", MB_OK);
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
				//�^�X�N�o�[�\��
				ShowTaskBar(true, hWnd);
				break;
			}
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//����̏�����Ԃ�
}

//----------------------------
//�t���X�N���[���ݒ�
//�����P	bFull�F�t���X�N���[���؂�ւ��i�t���X�N���[���ɂ���ꍇ�A�^�X�N�o�[�������j�@true:�t���X�N�@false:�ʏ�E�B���h�E
//�����Q	hWnd �F�E�B���h�E�X�e�[�^�X���
//----------------------------
void SetFullScreen(bool bFull, HWND hWnd)
{
	HDC hDC = GetDC(GetDesktopWindow());			// �f�X�N�g�b�v�̃n���h��
	int nScrSizeX = GetDeviceCaps(hDC, HORZRES);	// �f�X�N�g�b�v�̉�ʃT�C�YX
	int nScrSizeY = GetDeviceCaps(hDC, VERTRES);	// �f�X�N�g�b�v�̉�ʃT�C�YY

	ReleaseDC(GetDesktopWindow(), hDC);				// �f�X�N�g�b�v�̃n���h���������

	if (bFull)
	{// �S��ʃt���O���^�̎��A
	 // �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// �S��ʃt���O���U�̎��A
	 // �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			hWnd,
			HWND_TOP,
			(nScrSizeX / 2) - (SCREEN_WIDTH / 2),
			(nScrSizeY / 2) - (SCREEN_HEIGHT / 2),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	// �E�C���h�E�̕\����Ԃ�ݒ�
	ShowWindow(hWnd, SW_NORMAL);

	// �N���C�A���g�̈���X�V
	UpdateWindow(hWnd);

	// �^�X�N�o�[��\��/��\��
	ShowTaskBar(!bFull, hWnd);
}

//========================================
// ShowTaskBar�֐� - �^�X�N�o�[�̕\��/��\����؂�ւ��� -
//----------------------------
//�����P	bShow�F�^�X�N�o�[�\���X�e�[�^�X�@true:�\������@false:��\���ɂ���
//�����Q	hWnd �F�E�B���h�E�X�e�[�^�X���
// �Q�l: https://dixq.net/forum/viewtopic.php?t=12505
//----------------------------
void ShowTaskBar(bool bShow, HWND hWnd)
{
	// �^�X�N�o�[�̃n���h�����擾
	HWND hTask = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (bShow)
	{// �\��
	 // �^�X�N�o�[��\��
		ShowWindow(hTask, SW_SHOW);

		// �E�C���h�E���A�N�e�B�u�ɂ���
		SetFocus(hWnd);
	}
	else
	{// ��\��
	 // �^�X�N�o�[���\��
		ShowWindow(hTask, SW_HIDE);
	}
}