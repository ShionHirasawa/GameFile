//================================================================================================
//
//モデル管理処理[model.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "model.h"

//=======================================
//コンストラクタ
//=======================================
CModel::CModel()
{
	//メンバ変数クリア
	m_posOffSet = m_pos = VEC3_INIT;
	m_rotOffSet = m_rot = VEC3_INIT;
	m_pParent = NULL;
}

//=======================================
//デストラクタ
//=======================================
CModel::~CModel()
{

}

//=======================================
//生成処理
//=======================================
CModel *CModel::Create(const char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//モデルインスタンスの生成
	CModel *pModel = new CModel;

	//インスタンスが生成された
	if (pModel != NULL)
	{
		//初期化処理
		if (FAILED(pModel->Init(pFilePath)))
		{
			//失敗したら
			delete pModel;	//メモリ削除
			pModel = NULL;	//ポインタをNULLに
			return NULL;	//NULLを返す
		}

		//位置・向きを設定（この関数に渡された引数の値がモデルの基準値なので、同じ値を渡す
		pModel->InitSetPosRot(pos, rot);
	}

	//モデルを返す
	return pModel;
}

//=======================================
//初期化処理
//=======================================
HRESULT CModel::Init(const char *pFilePath)
{
	//Xファイル読み込み
	if (FAILED(LoadModelParts(pFilePath)))
	{
		//初期化失敗
		return E_FAIL;
	}

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CModel::Uninit(void)
{
	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//テクスチャIDが使われている
	if (m_pTextureID != NULL)
	{
		delete[] m_pTextureID;
		m_pTextureID = NULL;
	}
}

//=======================================
//更新処理
//=======================================
void CModel::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CModel::Draw(void)
{
	/*  デバイスを取得  */	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	/* 親のマトリックス */	D3DXMATRIX mtxParent;
	/*計算用マトリックス*/	D3DXMATRIX mtxRot, mtxTrans;
	/*マテリアルポインタ*/	D3DXMATERIAL *pMat;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//親が存在する
	if (m_pParent != NULL)
	{
		//親のマトリックスを取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	//親が存在しない
	else
	{	
		//現在（最新）のマトリックスを取得する[ = プレイヤーのマトリックス]
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//自分と親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(m_pTextureID[nCntMat]));

		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
}

//=======================================
//親の設定
//=======================================
void CModel::SetParent(CModel *pModel)
{
	//親モデルを代入
	m_pParent = pModel;
}

//=======================================
//ワールドマトリックスを取得
//=======================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	//ワールドマトリックスを返す
	return m_mtxWorld;
}

//=======================================
//モデルパーツ読み込み
//=======================================
HRESULT CModel::LoadModelParts(const char *pFilePath)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Ⅹファイルが読み込めなかった
	if (D3DXLoadMeshFromX(pFilePath, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_dwNumMat, &m_pMesh) != D3D_OK)
	{
		//初期化失敗
		return E_FAIL;
	}

	//マテリアルの数だけポインタを確保
	m_pTextureID = new int[m_dwNumMat];

	//マテリアルデータへのポインタを取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//エラー番号を初期値として代入
		m_pTextureID[nCntMat] = ERROR_ID;

		//テクスチャファイルが存在する
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャ番号格納 <- テクスチャの読み込み
			m_pTextureID[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
		}
	}

	//初期化成功
	return S_OK;
}

//=======================================
//位置・角度（基準値も含む）の初期設定
//=======================================
void CModel::InitSetPosRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{	
	m_posOffSet = m_pos = pos;//位置設定
	m_rotOffSet = m_rot = rot;//角度設定
}