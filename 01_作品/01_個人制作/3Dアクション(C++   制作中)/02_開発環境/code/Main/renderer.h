//================================================================================================
//
//�����_�����O����[renderer.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//�����_���[�N���X
class CRenderer
{
public:
	//���u�����h�� [ ���Z�E���Z ] �����̂ǂ��炩
	typedef enum
	{
		TYPE_ADD = D3DBLENDOP_ADD,			//���Z����
		TYPE_SUB = D3DBLENDOP_REVSUBTRACT,	//���Z����
		VALUE_SET = D3DBLEND_ONE,			//���u�����h��ݒ�
		VALUE_RESET = D3DBLEND_INVSRCALPHA,	//���u�����h��߂�
	}ALPHABLEND;

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	void SetAlphaBlend(ALPHABLEND type, ALPHABLEND value);
	void SetAlphaTest(bool bTest, int nAlpha);
	void SetBackGroundColor(D3DXCOLOR col = DEFAULT_BG_COLOR) { m_BGColor = col; }

	//�����ɂ́u���ʃJ�����O�v�u�ʏ�J�����O�v�u���ʃJ�����O�v�̂����ꂩ��������
	//���ʃJ�����O = D3DCULL_NONE
	//�ʏ�J�����O = D3DCULL_CCW
	//���ʃJ�����O = D3DCULL_CW
	void SetCulling(_D3DCULL value = D3DCULL_CCW);

	bool Culling(void) { return m_bCull; }
	bool IsPause(void) { return m_bPause; }

private:
	static const D3DXCOLOR DEFAULT_BG_COLOR;//�f�t�H���g�w�i�F

	LPDIRECT3D9			m_pD3D = NULL;		// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9	m_pD3DDevice = NULL;// Direct3D�f�o�C�X�ւ̃|�C���^

	D3DXCOLOR m_BGColor;	//�w�i�F

	bool m_bPause;	// �|�[�Y
	bool m_bCull;	// ���ʃJ�����OON/OFF
};

#endif