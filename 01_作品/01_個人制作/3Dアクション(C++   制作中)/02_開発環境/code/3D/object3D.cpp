//================================================================================================
//
//オブジェクト3D処理[object3D.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "object3D.h"

//減衰量
#define ATTENUATE	(0.1f)

//=======================================
//コンストラクタ
//=======================================
CObject3D::CObject3D(const int nPriority) : CObject(nPriority)
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
}

//=======================================
//デストラクタ
//=======================================
CObject3D::~CObject3D()
{

}

//=======================================
//初期化処理
// MEMO：引数で必要なもの
// テクスチャのパス、位置、幅、高さ、頂点カラー、テクスチャの左上の座標、テクスチャの右下の座標
//=======================================
HRESULT CObject3D::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax)
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
	pVtx[0].pos = D3DXVECTOR3(-fWidth, -fHeight, +fDepth);
	pVtx[1].pos = D3DXVECTOR3(+fWidth, -fHeight, +fDepth);
	pVtx[2].pos = D3DXVECTOR3(-fWidth, +fHeight, -fDepth);
	pVtx[3].pos = D3DXVECTOR3(+fWidth, +fHeight, -fDepth);

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

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CObject3D::Uninit(void)
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
void CObject3D::Update(void)
{
	//=============================
	//各種オブジェクトの更新処理
	//=============================
	CObject::UpdateAll();
}

//=======================================
//描画処理
//=======================================
void CObject3D::Draw(void)
{
	/*  デバイスを取得  */	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	/*計算用マトリックス*/	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

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
}

//=======================================
//移動量を減衰（減衰係数はデフォルト値
//=======================================
void CObject3D::MoveAttenuate(void)
{
	m_move += (VEC3_INIT - m_move) * ATTENUATE;
}

//=======================================
//重力を用いた移動減衰処理
//=======================================
void CObject3D::MoveAttenuateGravity(const float fGravityPower, const float fGravityMag)
{
	/*Ｘベクトル減衰*/ m_move.x += (0.0f - m_move.x) * ATTENUATE;
	/*Ｚベクトル減衰*/ m_move.z += (0.0f - m_move.z) * ATTENUATE;
	/*Ｙベクトル重力に従わせる*/ m_move.y += (fGravityPower - m_move.y) * fGravityMag;
}

//=======================================
//頂点カラー設定
//=======================================
HRESULT CObject3D::SetVertexInfo(const D3DXCOLOR col)
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
HRESULT CObject3D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fSizeWidth, -fSizeHeight, +fSizeDepth);
	pVtx[1].pos = D3DXVECTOR3(+fSizeWidth, -fSizeHeight, +fSizeDepth);
	pVtx[2].pos = D3DXVECTOR3(-fSizeWidth, +fSizeHeight, -fSizeDepth);
	pVtx[3].pos = D3DXVECTOR3(+fSizeWidth, +fSizeHeight, -fSizeDepth);

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
HRESULT CObject3D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col)
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
HRESULT CObject3D::SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
HRESULT CObject3D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//長さを計算
	float fLength = D3DXVec2Length(&D3DXVECTOR2(fSizeWidth, fSizeDepth));

	//４頂点全ての角度を計算
	float fVtxAngle[VTX_MAX] = {};

	fVtxAngle[0] = atan2f(-fSizeWidth, +fSizeDepth);
	fVtxAngle[1] = atan2f(+fSizeWidth, +fSizeDepth);
	fVtxAngle[2] = atan2f(-fSizeWidth, -fSizeDepth);
	fVtxAngle[3] = atan2f(+fSizeWidth, -fSizeDepth);

	//頂点座標の設定
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		pVtx[nCntVtx].pos.x = sinf(fVtxAngle[nCntVtx]) * fLength;
		pVtx[nCntVtx].pos.z = cosf(fVtxAngle[nCntVtx]) * fLength;
		pVtx[nCntVtx].pos.y = fSizeHeight * (1.0f - ((nCntVtx + 2) / 2));
	}

	/*原点から見て上の頂点*/pVtx[0].pos.y = pVtx[1].pos.y = +fSizeHeight;
	/*原点から見て下の頂点*/pVtx[2].pos.y = pVtx[3].pos.y = -fSizeHeight;

	D3DXVECTOR3 pos0 = pVtx[0].pos;
	D3DXVECTOR3 pos1 = pVtx[1].pos;
	D3DXVECTOR3 pos2 = pVtx[2].pos;
	D3DXVECTOR3 pos3 = pVtx[3].pos;

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
HRESULT CObject3D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
//一部の頂点座標を設定
//=======================================
HRESULT CObject3D::SetVertexInfo(float fAddHeight)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点１・２のＹ座標を上げ下げする
	pVtx[1].pos.y = pVtx[2].pos.y = m_pos.y + fAddHeight;

	D3DXVECTOR3 Vec1To0 = pVtx[1].pos - pVtx[0].pos;	//頂点０ -> 頂点１までのベクトル
	D3DXVECTOR3 Vec2To0 = pVtx[2].pos - pVtx[0].pos;	//頂点０ -> 頂点２までのベクトル

	D3DXVECTOR3 Vec1To3 = pVtx[1].pos - pVtx[3].pos;	//頂点３ -> 頂点１までのベクトル
	D3DXVECTOR3 Vec2To3 = pVtx[2].pos - pVtx[3].pos;	//頂点３ -> 頂点２までのベクトル

	D3DXVECTOR3 Nor0, Nor3;

	//外積を求める
	D3DXVec3Cross(&Nor0, &Vec1To0, &Vec2To0);
	D3DXVec3Cross(&Nor3, &Vec2To3, &Vec1To3);

	//正規化
	D3DXVec3Normalize(&Nor0, &Nor0);
	D3DXVec3Normalize(&Nor3, &Nor3);

	//法線を設定
	pVtx[0].nor = Nor0;
	pVtx[1].nor = (Nor0 + Nor3) * 0.5f;
	pVtx[2].nor = (Nor0 + Nor3) * 0.5f;
	pVtx[3].nor = Nor3;

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
HRESULT CObject3D::GetVertexPos(D3DXVECTOR3 &rPos, const int nVtxID)
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

//=======================================
//頂点座標と法線を取得
//=======================================
HRESULT CObject3D::GetVertexNor(D3DXVECTOR3 &rNor, const int nVtxID)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//指定された頂点座標と法線を代入
	rNor = pVtx[nVtxID].nor;

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//設定完了
	return S_OK;
}