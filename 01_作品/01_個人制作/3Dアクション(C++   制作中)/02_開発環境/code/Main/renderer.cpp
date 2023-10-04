//================================================================================================
//
//レンダリング処理[renderer.cpp]
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

//背景の色
const D3DXCOLOR CRenderer::DEFAULT_BG_COLOR = D3DCOLOR_RGBA(40, 40, 40, 255);

//=======================================
//コンストラクタ
//=======================================
CRenderer::CRenderer()
{
	//メンバ変数クリア
	//ポインタをすべてNULLにする
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	//背景色クリア
	m_BGColor = DEFAULT_BG_COLOR;

	m_bPause = false;	//ポーズOFF
	m_bCull = false;	//両面カリングOFF
}

//=======================================
//デストラクタ
//=======================================
CRenderer::~CRenderer()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE			d3ddm;//ディスプレイモード
	D3DPRESENT_PARAMETERS	d3dpp;//プレゼンテーションパラメータ

								  //DirectX3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現存のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//ゲーム画面のサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//ゲーム画面のサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						//バックバッファの形式
	d3dpp.BackBufferCount = 1;									//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

																//Direct3Dデバイスの生成
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
	//各種オブジェクトの初期化処理
	//==============================
	//レンダーステート
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//テクスチャの繰り返しの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CRenderer::Uninit(void)
{
	//分解能を戻す
	timeEndPeriod(1);

	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=======================================
//更新処理
//=======================================
void CRenderer::Update(void)
{
	//ポーズキーが押された
	if (CManager::IsInputTrigger(DIK_P, CInputGamePad::START))
	{
		//ポーズON/OFF 切り替え
		m_bPause = m_bPause ? false : true;
	}

	//ポーズ中じゃない
	if (!m_bPause)
	{
		//オブジェクトの更新処理
		CObject::UpdateAll();
	}
}

//=======================================
//描画処理
//=======================================
void CRenderer::Draw(void)
{
	//画面クリア(バックバッファ＆Zバッファのクリア)
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), m_BGColor, 1.0f, 0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//==============================
		//各種オブジェクトの描画処理
		//==============================
		CObject::DrawAll();

		//デバッグ表示
		CManager::GetDebugProc()->Draw();

		//描画終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=======================================
//αブレンドの設定
//=======================================
void CRenderer::SetAlphaBlend(ALPHABLEND BlendType, ALPHABLEND value)
{
	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, (D3DBLENDOP)BlendType);	//ここで加算合成・減算合成のどちらかが指定される
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, (D3DBLEND)value);		//ここで通常のレンダーにもどるか、どちらかの合成用になるか
}

//=======================================
//αテストの設定
//=======================================
void CRenderer::SetAlphaTest(bool bTest, int nAlpha)
{
	//αテストを有効にする
	if (bTest)
	{
		m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
	//αテストを無効にする
	else
	{
		m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	}

	//α値（透明度）の設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, nAlpha);
}

//=======================================
//カリング設定
//=======================================
void CRenderer::SetCulling(_D3DCULL value)
{
	//カリング設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, value);
}