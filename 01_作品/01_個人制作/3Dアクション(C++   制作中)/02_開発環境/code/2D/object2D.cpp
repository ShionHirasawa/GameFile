//================================================================================================
//
//オブジェクト2D処理[object2D.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../XFile/objectX.h"
#include "../3D/billboard.h"
#include "object2D.h"
#include "../Player/player.h"
#include "bg.h"

//減衰量
#define ATTENUATE	(0.1f)

//=======================================
//コンストラクタ
//=======================================
CObject2D::CObject2D(const int nPriority) : CObject(nPriority)
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
CObject2D::~CObject2D()
{

}

//=======================================
//初期化処理
// MEMO：引数で必要なもの
// テクスチャのパス、位置、幅、高さ、頂点カラー、テクスチャの左上の座標、テクスチャの右下の座標
//=======================================
HRESULT CObject2D::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点情報のポインタ
	VERTEX_2D *pVtx = NULL;

	//頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
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
	/*サイズ設定*/	SetSize(D3DXVECTOR2(fWidth, fHeight));

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y - fHeight, pos.z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y - fHeight, pos.z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y + fHeight, pos.z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHeight, pos.z);

	//rhw・頂点カラーの設定
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		pVtx[nCntVtx].rhw = RHW;	// 1.0固定
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
void CObject2D::Uninit(void)
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
void CObject2D::Update(void)
{
	//=============================
	//各種オブジェクトの更新処理
	//=============================
	CObject::UpdateAll();
}

//=======================================
//描画処理
//=======================================
void CObject2D::Draw(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=======================================
//移動量を減衰（減衰係数はデフォルト値
//=======================================
void CObject2D::MoveAttenuate(void)
{
	m_move += (VEC3_INIT - m_move) * ATTENUATE;
}

//=======================================
//テクスチャを割り当てる
//=======================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//テクスチャを割り当てる
	m_pTexture = pTexture;
}

//=======================================
//重力を用いた移動減衰処理
//=======================================
void CObject2D::MoveAttenuateGravity(const float fGravityPower, const float fGravityMag)
{
	//Ｘの移動量を減衰
	m_move.x += (0.0f - m_move.x) * ATTENUATE;

	//Ｙの移動量を重力に従わせる
	m_move.y += (fGravityPower - m_move.y) * fGravityMag;
}

//=======================================
//画面内にとどめる処理（原点位置を基準
//=======================================
bool CObject2D::KeepOnWindow(void)
{
	//当たったかどうか
	bool bCollision = false;

	//画面の左端より、左に行ってしまった
	if (m_pos.x <= 0.0f)
	{
		m_pos.x = 0.0f;		//左端に戻す
		m_move.x = 0.0f;	//横の移動量を消す
		bCollision = true;	//当たった
	}

	//画面の右端より、右に行ってしまった
	if (m_pos.x >= SCREEN_WIDTH_F)
	{
		m_pos.x = SCREEN_WIDTH_F;	//右端に戻す
		m_move.x = 0.0f;			//横の移動量を消す
		bCollision = true;			//当たった
	}

	//画面の天井より、上に行ってしまった
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;		//天井に戻す
		m_move.y = 0.0f;	//下の移動量を消す
		bCollision = true;	//当たった
	}

	//画面の底より、下に行ってしまった
	if (m_pos.y >= SCREEN_HEIGHT_F)
	{
		m_pos.y = SCREEN_HEIGHT_F;	//底に戻す
		m_move.y = 0.0f;			//下の移動量を消す
		bCollision = true;			//当たった
	}

	//当たったかどうか判定を返す
	return bCollision;
}

//=======================================
//画面内にとどめる処理（中心位置からの幅、高さを考慮する
//=======================================
bool CObject2D::KeepOnWindow(const float fWidth, const float fHeight)
{
	//当たったかどうか
	bool bCollision = false;

	//画面の左端より、左に行ってしまった
	if (m_pos.x - fWidth <= 0.0f)
	{
		m_pos.x = fWidth;	//左端から横幅の分 戻す
		m_move.x = 0.0f;	//横の移動量を消す
		bCollision = true;	//当たった
	}

	//画面の右端より、右に行ってしまった
	if (m_pos.x + fWidth >= SCREEN_WIDTH_F)
	{
		m_pos.x = SCREEN_WIDTH_F - fWidth;	//右端から横幅の分 戻す
		m_move.x = 0.0f;					//横の移動量を消す
		bCollision = true;					//当たった
	}

	//画面の天井より、上に行ってしまった
	if (m_pos.y - fHeight <= 0.0f)
	{
		m_pos.y = fHeight;	//高さの分 戻す
		m_move.y = 0.0f;	//下の移動量を消す
		bCollision = true;	//当たった
	}

	//画面の底より、下に行ってしまった
	if (m_pos.y + fHeight >= SCREEN_HEIGHT_F)
	{
		m_pos.y = SCREEN_HEIGHT_F - fHeight;//底に戻す
		m_move.y = 0.0f;					//下の移動量を消す
		bCollision = true;					//当たった
	}

	//当たったかどうか判定を返す
	return bCollision;
}

//=======================================
//頂点カラー設定
//=======================================
HRESULT CObject2D::SetVertexInfo(const D3DXCOLOR col)
{
	//ポインタ設定
	VERTEX_2D *pVtx = NULL;

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
//頂点カラー設定
//=======================================
HRESULT CObject2D::SetVertexInfo(const D3DXCOLOR col, const int nVtxID)
{
	//ポインタ設定
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))return E_FAIL;

	//頂点カラーの設定
	pVtx[nVtxID].col = col;

	//頂点バッファを開放（アンロック）する
	if (FAILED(m_pVtxBuff->Unlock()))return E_FAIL;

	//設定完了
	return S_OK;
}

//=======================================
//頂点座標設定
//=======================================
HRESULT CObject2D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight)
{
	//ポインタ設定
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_UP) * fSizeHeight;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_UP) * fSizeHeight;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_DOWN) * fSizeHeight;

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
HRESULT CObject2D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const D3DXCOLOR col)
{
	//ポインタ設定
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_UP) * fSizeHeight;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_UP) * fSizeHeight;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_DOWN) * fSizeHeight;

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
HRESULT CObject2D::SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//ポインタ設定
	VERTEX_2D *pVtx = NULL;

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
HRESULT CObject2D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//ポインタ設定
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_UP) * fSizeHeight;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_UP) * fSizeHeight;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_DOWN) * fSizeHeight;

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
HRESULT CObject2D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//ポインタ設定
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_UP) * fSizeHeight;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_UP) * fSizeHeight;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_DOWN) * fSizeHeight;

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
HRESULT CObject2D::GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID)
{
	//ポインタ設定
	VERTEX_2D *pVtx = NULL;

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