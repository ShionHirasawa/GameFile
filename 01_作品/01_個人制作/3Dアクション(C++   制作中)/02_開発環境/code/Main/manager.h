//================================================================================================
//
//�}�l�[�W���[����[manager.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "color.h"
#include "texture.h"

//�O���錾
class CRenderer;
class CDebugProc;
class CInputKeyboard;
class CInputMouse;
class CInputGamePad;
class CSound;
class CCamera;
class CLight;
class CTexture;

//�V�[���N���X
class CScene
{
public:
	//���[�h�̗񋓌^
	enum MODE
	{
		TITLE = 0,	// �^�C�g��
		TUTORIAL,	// �`���[�g���A��
		GAME,		// �Q�[��
		RESULT,	// ���U���g
		MAX
	};

	CScene();
	virtual ~CScene();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CScene *Create(MODE mode);	//��������
	MODE Get(void) { return m_mode; }	//�擾����

private:
	static CScene *s_pMode;	//���[�h���Ƃ̃N���X���i�[

	MODE m_mode;	//���݂̃��[�h���i�[
};

//�}�l�����N���X
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void);

	static bool IsInputPress(int nKey, int nPadButton);
	static bool IsInputTrigger(int nKey, int nPadButton);

	//�e��I�u�W�F�N�g�̃|�C���^�擾����
	static CRenderer		*GetRenderer(void)	{ return s_pRenderer;}
	static CScene			*GetScene(void)		{ return s_pScene; }
	static CDebugProc		*GetDebugProc(void)	{ return s_pDebugProc;}
	static CInputKeyboard	*GetKeyboard(void)	{ return s_pKeyboard;}
	static CInputGamePad	*GetGamePad(void)	{ return s_pGamePad; }
	static CInputMouse		*GetMouse(void)		{ return s_pMouse; }
	static CSound			*GetSound(void)		{ return s_pSound; }
	static CCamera			*GetCamera(void)	{ return s_pCamera;}
	static CLight			*GetLight(void)		{ return s_pLight;	}
	static CTexture			*GetTexture(void)	{ return s_pTexture; }

private:
	void OpenPointer(void);			//�������J��
	HRESULT CreateStatic(void);		//�ÓI�����o�ϐ��𐶐�
	static void CreateFade(void);	//�t�F�[�h�𐶐�����

	bool SelectStartMode(void);		//�J�n���郂�[�h��I��
	bool m_bSelect;

	static CRenderer		*s_pRenderer;
	static CScene			*s_pScene;
	static CDebugProc		*s_pDebugProc;
	static CInputKeyboard	*s_pKeyboard;
	static CInputGamePad	*s_pGamePad;
	static CInputMouse		*s_pMouse;
	static CSound			*s_pSound;
	static CCamera			*s_pCamera;
	static CLight			*s_pLight;
	static CTexture			*s_pTexture;

	//�ÓI�����o�ϐ������p�ɕێ�
	static HINSTANCE		s_hInstance;
	static HWND				s_hWnd;

	bool m_bAutoFade;
	int type;//FADE_OUT�@���@FADE_WIPE_OUT�̂ǂ��炩
};

#endif