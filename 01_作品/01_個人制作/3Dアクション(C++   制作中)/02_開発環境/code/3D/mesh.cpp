//================================================================================================
//
//メッシュ処理[billboard.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Object/object.h"
#include "mesh.h"
#include "../S_H_Lib/S_H_Calculation.h"

//テクスチャのパス
#define TEX_PATH		"data\\TEXTURE\\tree000.png"

//=======================================
//コンストラクタ
//=======================================
CMesh::CMesh(const int nPriority) : CObject(nPriority)
{
	//メンバ変数クリア
	m_nTextureID = ERROR_ID;
	m_fTexUSize = 0.0f;
	m_fTexVSize = 0.0f;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fDepth = 0.0f;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = VEC3_INIT;
	m_rot = VEC3_INIT;
	m_move = VEC3_INIT;
	m_posOld = VEC3_INIT;
	m_col = GetColor(COL_CLEAR);
	m_Buffer.nNumIdx = 0;
	m_Buffer.nNumPoly = 0;
	m_Buffer.nNumVtx = 0;
	m_bFieldType = true;
	m_bDisp = true;
}

//=======================================
//デストラクタ
//=======================================
CMesh::~CMesh()
{

}

//=======================================
//初期化処理（シリンダー用
//=======================================
HRESULT CMesh::Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, int nDevide, float fRadius, float fHeight, float fTexUSize, float fTexVSize)
{
	//----------	メンバ変数初期化	----------
	/*  位置設定  */	m_pos = pos;
	/* カラー設定 */	m_col = col;
	/* バッファー */	m_Buffer = GetMeshBuffer(nDevide);
	/*	 分割数	  */	m_nDevide = nDevide;
	/*	  高さ	  */	m_fRadius = fRadius;
	/*	  半径	  */	m_fHeight = fHeight;
	/*メッシュタイプ*/	m_bFieldType = false;

	m_nTextureID = CManager::GetTexture()->Regist("data\\TEXTURE\\ChargeCylinder.png");

	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_Buffer.nNumIdx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))	return E_FAIL;

	//インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * m_Buffer.nNumIdx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL)))	return E_FAIL;

	//頂点情報初期化
	if (FAILED(InitVtxInfo_Cylinder()))	return E_FAIL;

	//インデックス情報初期化
	if (FAILED(InitIdxInfo_Cylinder()))	return E_FAIL;

	return S_OK;
}

//=======================================
//頂点情報初期化設定（シリンダー用
//=======================================
HRESULT CMesh::InitVtxInfo_Cylinder(void)
{
	//頂点情報のポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))	return E_FAIL;

	//頂点の角度
	const float fRot = (D3DX_PI * 2.0f) / m_nDevide;
	float fRound = D3DX_PI;
	float fTexWidth = 1.0f / m_nDevide;

	//頂点情報を上下ペアで設定
	for (int nCntVtx = 0; nCntVtx < m_nDevide; nCntVtx++)
	{
		//底面の頂点番号
		int nLowID = nCntVtx + m_nDevide + 1;

		//設定する位置（Ｙ座標だけ変える
		D3DXVECTOR3 pos = D3DXVECTOR3(
			sinf(fRound) * m_fRadius,
			0.0f,
			cosf(fRound) * m_fRadius);

		// 頂点座標
		pVtx[nCntVtx].pos = pVtx[nLowID].pos = pos;
		pVtx[nCntVtx].pos.y = m_fHeight;//天面の高さ設定

		// テクスチャ座標
		pVtx[nCntVtx].tex.x = pVtx[nLowID].tex.x = fTexWidth * nCntVtx;
		pVtx[nCntVtx].tex.y = 0.0f;
		pVtx[nLowID].tex.y = 1.0f;

		pVtx[nCntVtx].nor = pVtx[nLowID].nor = NOR_INIT;// nor初期化
		pVtx[nCntVtx].col = pVtx[nLowID].col = m_col;	// 色指定

		fRound -= fRot;//次の頂点の角度へ設定
	}

	//最後の頂点を設定する
	pVtx[m_nDevide] = pVtx[0];
	pVtx[m_nDevide * 2 + 1] = pVtx[m_nDevide + 1];

	pVtx[m_nDevide].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[m_nDevide * 2 + 1].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))	return E_FAIL;

	//初期化完了
	return S_OK;
}

//=======================================
//インデックス情報初期化設定（シリンダー用
//=======================================
HRESULT CMesh::InitIdxInfo_Cylinder(void)
{
	WORD *pIdx;
	//インデックスバッファをロックし、頂点番号へのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void* *)&pIdx, 0);

	//底面の頂点番号
	int nBottomNumber = ODDPARITY + m_nDevide;

	//インデックス番号データの設定
	for (int nCntIdx = 0; nCntIdx < m_Buffer.nNumIdx; nCntIdx++)
	{
		//カウンターが偶数の場合、後半以降の計算式 ↓ の答えがゼロになる	奇数の場合は、シリンダー下部の基準頂点番号が加算される
		pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + (nBottomNumber * (nCntIdx % EVENPARITY));
	}

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();
	//初期化完了
	return S_OK;
}

//=======================================
//初期化処理（フィールド用
//=======================================
HRESULT CMesh::Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, int nColumn, int nRow, float fTexUSize, float fTexVSize)
{
	//----------	メンバ変数初期化	----------
	/*  位置設定  */	m_pos = pos;
	/* カラー設定 */	m_col = col;
	/* バッファー */	m_Buffer = GetMeshBuffer(nColumn, nRow);
	/*メッシュタイプ*/	m_bFieldType = true;

	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_Buffer.nNumIdx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))	return E_FAIL;

	//頂点情報初期化
	if (FAILED(InitVtxInfo_Field()))	return E_FAIL;

	//インデックス情報初期化
	if (FAILED(InitIdxInfo_Field()))	return E_FAIL;

	SetType(CObject::TYPE_MESH);

	return S_OK;
}

//=======================================
//頂点情報初期化設定（フィールド用
//=======================================
HRESULT CMesh::InitVtxInfo_Field(void)
{
	//頂点情報のポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))	return E_FAIL;

	//rhw・頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < m_Buffer.nNumIdx; nCntVtx++)
	{
		pVtx[nCntVtx].pos = VEC3_INIT;	// 頂点座標
		pVtx[nCntVtx].nor = NOR_INIT;	// nor初期化
		pVtx[nCntVtx].col = m_col;		// 色指定
		pVtx[nCntVtx].tex = VEC2_INIT;	// テクスチャ座標
	}

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))	return E_FAIL;

	//初期化完了
	return S_OK;
}

//=======================================
//インデックス情報初期化設定（フィールド用
//=======================================
HRESULT CMesh::InitIdxInfo_Field(void)
{
	//初期化完了
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CMesh::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 自分自身の終了処理
	this->Release();
}

//=======================================
//更新処理
//=======================================
void CMesh::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CMesh::Draw(void)
{
	//描画しない
	if (!m_bDisp) return;

	//デバイスを取得
	CRenderer *pRender = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

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

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(m_nTextureID));

	//両面カリング
	pRender->SetCulling(D3DCULL_NONE);

	//αテストON
	pRender->SetAlphaTest(true, 0);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_Buffer.nNumIdx, 0, m_Buffer.nNumIdx);

	//αテストOFF
	pRender->SetAlphaTest(false, 255);

	//通常カリング
	pRender->SetCulling();
}

//=======================================
//頂点カラー設定
//=======================================
HRESULT CMesh::SetVertexInfo(const D3DXCOLOR col)
{
	//ポインタ設定
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))return E_FAIL;

	//頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < m_Buffer.nNumVtx; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
	}
	m_col = col;

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))	return E_FAIL;

	//設定完了
	return S_OK;
}

//=======================================
//頂点座標設定
//=======================================
HRESULT CMesh::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth)
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
HRESULT CMesh::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col)
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
HRESULT CMesh::SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
HRESULT CMesh::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
HRESULT CMesh::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
HRESULT CMesh::GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID)
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