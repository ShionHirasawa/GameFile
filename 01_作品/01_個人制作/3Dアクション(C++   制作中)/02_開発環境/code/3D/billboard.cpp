//================================================================================================
//
//ビルボード処理[billboard.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Object/object.h"
#include "billboard.h"

//テクスチャのパス
#define TEX_PATH		"data\\TEXTURE\\tree000.png"

//静的メンバ変数宣言
int CObjectBillboard::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CObjectBillboard::CObjectBillboard(const int nPriority) : CObject(nPriority)
{
	//メンバ変数クリア
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fDepth = 0.0f;
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_pos = VEC3_INIT;
	m_rot = VEC3_INIT;
	m_move = VEC3_INIT;
	m_posOld = VEC3_INIT;
	m_col = GetColor(COL_CLEAR);
	m_bZTest = true;
}

//=======================================
//デストラクタ
//=======================================
CObjectBillboard::~CObjectBillboard()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CObjectBillboard::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点情報のポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//----------	メンバ変数初期化	----------
	/*位置設定*/	m_pos = pos;
	/*カラー設定*/	m_col = col;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fWidth, +fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+fWidth, +fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+fWidth, -fHeight, 0.0f);

	//rhw・頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		pVtx[nCntVtx].nor = NOR_INIT;// nor初期化
		pVtx[nCntVtx].col = col;	// 色指定
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(TexMin.x, TexMin.y);
	pVtx[1].tex = D3DXVECTOR2(TexMax.x, TexMin.y);
	pVtx[2].tex = D3DXVECTOR2(TexMin.x, TexMax.y);
	pVtx[3].tex = D3DXVECTOR2(TexMax.x, TexMax.y);

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))
	{
		//失敗したらエラーを返す
		return E_FAIL;
	}

	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CObjectBillboard::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 自分自身の終了処理
	this->Release();
}

//=======================================
//更新処理
//=======================================
void CObjectBillboard::Update(void)
{
	//=============================
	//各種オブジェクトの更新処理
	//=============================
	CObject::UpdateAll();
}

//=======================================
//描画処理
//=======================================
void CObjectBillboard::Draw(void)
{
	/*		レンダラー取得		*/	CRenderer *pRender = CManager::GetRenderer();
	/*		デバイスを取得		*/	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	/*	  計算用マトリックス	*/	D3DXMATRIX mtxTrans;
	/* ビューマトリックス取得用 */	D3DXMATRIX mtxView;

	//αテストを有効にする
	pRender->SetAlphaTest(true, 0);

	//Zテストを無効にする
	if (m_bZTest)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスをGet
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//=========================================
	//ポリゴンをカメラに対して正面に向ける
	//=========================================
	//逆行列を求める
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	//反転させて求めた逆行列の不要な部分をゼロにする
	m_mtxWorld._41 = m_mtxWorld._42 = m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL は 「<=」のこと
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zバッファの有効無効設定

	//αテストを無効にする
	pRender->SetAlphaTest(false, 255);
}

//=======================================
//頂点カラー設定
//=======================================
HRESULT CObjectBillboard::SetVertexInfo(const D3DXCOLOR col)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点カラーの設定
	m_col = pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = col;

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//設定完了
	return S_OK;
}

//=======================================
//頂点座標設定
//=======================================
HRESULT CObjectBillboard::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fSizeWidth, -fSizeHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+fSizeWidth, -fSizeHeight, 0.0f);

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//設定完了
	return S_OK;
}

//=======================================
//頂点座標・カラーを変える
//=======================================
HRESULT CObjectBillboard::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fSizeWidth, -fSizeHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+fSizeWidth, -fSizeHeight, 0.0f);

	//頂点カラーの設定
	m_col = pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = col;

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//設定完了
	return S_OK;
}

//=======================================
//テクスチャ座標を設定
//=======================================
HRESULT CObjectBillboard::SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//テクスチャ座標の設定
	pVtx[0].tex.x = pVtx[2].tex.x = Texpos.x;				//左 = そのまま代入
	pVtx[1].tex.x = pVtx[3].tex.x = Texpos.x + fTexWidth;	//右 = 左の座標 + テクスチャの分割幅

	pVtx[0].tex.y = pVtx[1].tex.y = Texpos.y;				//上 = そのまま代入
	pVtx[2].tex.y = pVtx[3].tex.y = Texpos.y + fTexHeight;	//下 = 上の座標 + テクスチャの分割の厚さ

	if (FAILED(m_pVtxBuff->Unlock()))//頂点バッファを開放（アンロック）する
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//設定完了
	return S_OK;
}

//=======================================
//頂点座標・テクスチャ座標を設定
//=======================================
HRESULT CObjectBillboard::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fSizeWidth, -fSizeHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+fSizeWidth, -fSizeHeight, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex.x = pVtx[2].tex.x = Texpos.x;				//左 = そのまま代入
	pVtx[1].tex.x = pVtx[3].tex.x = Texpos.x + fTexWidth;	//右 = 左の座標 + テクスチャの分割幅

	pVtx[0].tex.y = pVtx[1].tex.y = Texpos.y;				//上 = そのまま代入
	pVtx[2].tex.y = pVtx[3].tex.y = Texpos.y + fTexHeight;	//下 = 上の座標 + テクスチャの分割の厚さ

															//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//設定完了
	return S_OK;
}

//=======================================
//頂点座標・頂点カラー・テクスチャ座標を設定
//=======================================
HRESULT CObjectBillboard::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos.x = sinf(m_rot.y + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = cosf(m_rot.y + PI_LEFT_UP) * fSizeHeight;
	pVtx[0].pos.z = cosf(m_rot.y + PI_LEFT_UP) * fSizeDepth;
	pVtx[1].pos.x = sinf(m_rot.y + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = cosf(m_rot.y + PI_RIGHT_UP) * fSizeHeight;
	pVtx[1].pos.z = cosf(m_rot.y + PI_RIGHT_UP) * fSizeDepth;
	pVtx[2].pos.x = sinf(m_rot.y + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = cosf(m_rot.y + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[2].pos.z = cosf(m_rot.y + PI_LEFT_DOWN) * fSizeDepth;
	pVtx[3].pos.x = sinf(m_rot.y + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = cosf(m_rot.y + PI_RIGHT_DOWN) * fSizeHeight;
	pVtx[3].pos.z = cosf(m_rot.y + PI_RIGHT_DOWN) * fSizeDepth;

	//頂点カラーの設定
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = col;

	//テクスチャ座標の設定
	pVtx[0].tex.x = pVtx[2].tex.x = Texpos.x;				//左 = そのまま代入
	pVtx[1].tex.x = pVtx[3].tex.x = Texpos.x + fTexWidth;	//右 = 左の座標 + テクスチャの分割幅

	pVtx[0].tex.y = pVtx[1].tex.y = Texpos.y;				//上 = そのまま代入
	pVtx[2].tex.y = pVtx[3].tex.y = Texpos.y + fTexHeight;	//下 = 上の座標 + テクスチャの分割の厚さ

															//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//設定完了
	return S_OK;
}

//=======================================
//頂点座標を取得
//=======================================
HRESULT CObjectBillboard::GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//指定された頂点座標を代入
	rPos = pVtx[nVtxID].pos;

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//設定完了
	return S_OK;
}