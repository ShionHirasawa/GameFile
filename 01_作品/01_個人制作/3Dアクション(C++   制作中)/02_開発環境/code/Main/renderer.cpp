//================================================================================================
//
//�����_�����O����[renderer.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "main.h"
#include "renderer.h"
#include "../Object/object.h"
#include "input.h"
#include "manager.h"
#include "color.h"
#include "../Player/player.h"
#include "../3D/object3D.h"
#include "../3D/wall.h"

//�w�i�̐F
const D3DXCOLOR CRenderer::DEFAULT_BG_COLOR = D3DCOLOR_RGBA(40, 40, 40, 255);

//=======================================
//�R���X�g���N�^
//=======================================
CRenderer::CRenderer()
{
	//�����o�ϐ��N���A
	//�|�C���^�����ׂ�NULL�ɂ���
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	//�w�i�F�N���A
	m_BGColor = DEFAULT_BG_COLOR;

	m_bPause = false;	//�|�[�YOFF
	m_bCull = false;	//���ʃJ�����OOFF
}

//=======================================
//�f�X�g���N�^
//=======================================
CRenderer::~CRenderer()
{

}

//=======================================
//����������
//=======================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE			d3ddm;//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS	d3dpp;//�v���[���e�[�V�����p�����[�^

								  //DirectX3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//�����̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));			//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//�Q�[����ʂ̃T�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//�Q�[����ʂ̃T�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

																//Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//==============================
	//�e��I�u�W�F�N�g�̏���������
	//==============================
	//�����_�[�X�e�[�g
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//�e�N�X�`���̌J��Ԃ��̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CRenderer::Uninit(void)
{
	//����\��߂�
	timeEndPeriod(1);

	//Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=======================================
//�X�V����
//=======================================
void CRenderer::Update(void)
{
	//�|�[�Y�L�[�������ꂽ
	if (CManager::IsInputTrigger(DIK_P, CInputGamePad::START))
	{
		//�|�[�YON/OFF �؂�ւ�
		m_bPause = m_bPause ? false : true;
	}

	//�|�[�Y������Ȃ�
	if (!m_bPause)
	{
		//�I�u�W�F�N�g�̍X�V����
		CObject::UpdateAll();
	}
}

//=======================================
//�`�揈��
//=======================================
void CRenderer::Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), m_BGColor, 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//==============================
		//�e��I�u�W�F�N�g�̕`�揈��
		//==============================
		CObject::DrawAll();

		//�f�o�b�O�\��
		CManager::GetDebugProc()->Draw();

		//�`��I��
		m_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=======================================
//���u�����h�̐ݒ�
//=======================================
void CRenderer::SetAlphaBlend(ALPHABLEND BlendType, ALPHABLEND value)
{
	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, (D3DBLENDOP)BlendType);	//�����ŉ��Z�����E���Z�����̂ǂ��炩���w�肳���
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, (D3DBLEND)value);		//�����Œʏ�̃����_�[�ɂ��ǂ邩�A�ǂ��炩�̍����p�ɂȂ邩
}

//=======================================
//���e�X�g�̐ݒ�
//=======================================
void CRenderer::SetAlphaTest(bool bTest, int nAlpha)
{
	//���e�X�g��L���ɂ���
	if (bTest)
	{
		m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
	//���e�X�g�𖳌��ɂ���
	else
	{
		m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	}

	//���l�i�����x�j�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, nAlpha);
}

//=======================================
//�J�����O�ݒ�
//=======================================
void CRenderer::SetCulling(_D3DCULL value)
{
	//�J�����O�ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, value);
}