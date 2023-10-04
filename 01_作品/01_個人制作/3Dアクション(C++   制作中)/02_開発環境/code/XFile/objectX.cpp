//================================================================================================
//
//Xファイルを使用したモデル管理処理[objectX.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "objectX.h"
#include "../3D/Polygon3D.h"
#include "../3D/PolygonBill.h"
#include "../3D/shadow.h"

//*********************
//マクロ定義
//*********************
//減衰量
#define ATTENUATE	(0.1f)

//変身時の当たり判定の範囲
#define TRANSFORM_COLLISION_RANGE	(100.0f)

//静的メンバ変数
CObjectX::XModel	*CObjectX::m_apXModel[MAX_X_MODEL] = {};
char				*CObjectX::m_pFilePath[MAX_X_MODEL] = {};
int					CObjectX::m_nNumAll = 0;

//3Dターゲットの静的情報
char		*CObjectX::TARGET_3D_TEX_PATH = "data\\TEXTURE\\Target_Circle.png";
const float  CObjectX::TARGET_3D_SIZE = 40.0f;	//幅・奥行き
CPoly3D		*CObjectX::m_pTarget3D = NULL;

//ビルボードターゲットの静的情報
char		*CObjectX::TARGET_BILL_TEX_PATH = "data\\TEXTURE\\Target_Triangle.png";
const float  CObjectX::TARGET_BILL_SIZE = 30.0f;	//幅・高さ
const float  CObjectX::TARGET_BILL_FLOAT = 50.0f;	//表示する位置をターゲットの最大Ｙ座標から浮かせる量
CPolyBill	*CObjectX::m_pTargetBill = NULL;

const float CObjectX::SHADOW_SIZE_CORRECT = 1.8f;//影ポリゴンの補正値

//=======================================
//コンストラクタ
//=======================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	//メンバ変数クリア
	m_nShadowID = ERROR_ID;/*影番号*/
	m_pos = VEC3_INIT;	// 位置
	m_move = VEC3_INIT;	// 移動量
	m_rot = VEC3_INIT;	// 向き
	m_nXmodelID = 0;	// 自分の番号
	m_bDisp = true;		// 描画する

	m_pTarget3D = NULL;		//変身できるオブジェクトの足元に表示
	m_pTargetBill = NULL;	//変身できるオブジェクトの頭上に表示
}

//=======================================
//デストラクタ
//=======================================
CObjectX::~CObjectX()
{

}

//=======================================
//初期化処理
//=======================================
HRESULT CObjectX::Init(char *pXFilePath, XMODELTYPE type, int nLife)
{
	//自分のⅩモデル番号を取得する
	m_nXmodelID = Regist(pXFilePath, type, nLife);

	//種類を設定する
	CObject::SetType(CObject::TYPE_XMODEL);

	//ターゲットポリゴンの初期化
	InitTargetPolygon();

	//影の位置を計算
	D3DXVECTOR3 pos = GetPosition();
	pos.y = 0.0f;

	//影生成
	m_nShadowID = CShadow::Create(pos, GetXModelMargin(m_nXmodelID) * SHADOW_SIZE_CORRECT);
	CShadow::DispSwitch(m_nShadowID, false);

	//初期化成功
	return S_OK;
}

//=======================================
//ターゲットポリゴンの初期化
//=======================================
void CObjectX::InitTargetPolygon(void)
{
	//3Dターゲットの生成
	if (m_pTarget3D == NULL)	{
		m_pTarget3D = CPoly3D::Create(TARGET_3D_TEX_PATH);
		m_pTarget3D->SetInfo(VEC3_INIT, TARGET_3D_SIZE, 0.0f, TARGET_3D_SIZE);
		m_pTarget3D->DispSwitch(false);
	}

	//ビルボードターゲットの生成
	if (m_pTargetBill == NULL)	{
		m_pTargetBill = CPolyBill ::Create(TARGET_BILL_TEX_PATH);
		m_pTargetBill->SetInfo(VEC3_INIT, TARGET_BILL_SIZE, TARGET_BILL_SIZE, 0.0f);
		m_pTargetBill->DispSwitch(false);
		m_pTargetBill->ZTestSwitch(true);
	}
}

//=======================================
//終了処理
//=======================================
void CObjectX::Uninit(void)
{
	//自分の影を破棄
	CShadow::Delete(m_nShadowID);

	for (int nCntXModel = 0; nCntXModel < MAX_X_MODEL; nCntXModel++)
	{
		//Ⅹモデルが使われている
		if (m_apXModel[nCntXModel] != NULL)
		{
			//メッシュの破棄
			if (m_apXModel[nCntXModel]->pMesh != NULL)
			{
				m_apXModel[nCntXModel]->pMesh->Release();
				m_apXModel[nCntXModel]->pMesh = NULL;
			}

			//マテリアルの破棄
			if (m_apXModel[nCntXModel]->pBuffMat != NULL)
			{
				m_apXModel[nCntXModel]->pBuffMat->Release();
				m_apXModel[nCntXModel]->pBuffMat = NULL;
			}

			//テクスチャIDが使われている
			if (m_apXModel[nCntXModel]->pTextureID != NULL)
			{
				delete[] m_apXModel[nCntXModel]->pTextureID;
				m_apXModel[nCntXModel]->pTextureID = NULL;
			}

			//Ⅹモデルのメモリを開放する
			delete m_apXModel[nCntXModel];
			m_apXModel[nCntXModel] = NULL;
		}

		//メモリが使われていたら、破棄する
		if (m_pFilePath[nCntXModel] != NULL)
		{
			delete m_pFilePath[nCntXModel];
			m_pFilePath[nCntXModel] = NULL;
		}
	}

	//自分自身を破棄
	this->Release();

	m_nNumAll = 0;
}

//=======================================
//更新処理
//=======================================
void CObjectX::Update(void)
{
	//自分の影位置設定
	CShadow::DispSwitch(m_nShadowID, m_bDisp);
	CShadow::SetPos(m_nShadowID, GetPosition());
}

//=======================================
//描画処理
//=======================================
void CObjectX::Draw(D3DXCOLOR *pDiffuse)
{
	//描画しない
	if (!m_bDisp) return;

	//自分のⅩモデルを取得
	CObjectX::XModel *pXModel = GetXModel(m_nXmodelID);

	//Ⅹモデルが取得できなかったら
	if (pXModel == NULL)
	{
		//描画せずに引き返す
		return;
	}

	/* レンダラーのポインタ */ CRenderer *pRender = CManager::GetRenderer();
	/* デバイスへのポインタ */ LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	/*  計算用マトリックス  */ D3DXMATRIX mtxRot, mtxTrans;
	/*現在のマテリアル保存用*/ D3DMATERIAL9 matDef;
	/* マテリアルのポインタ */ D3DXMATERIAL *pMat;

	//αテストを有効にする
	pRender->SetAlphaTest(true, 0);

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

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL *)pXModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pXModel->dwNumMat; nCntMat++)
	{
		//一時的にマテリアルを保管
		const D3DMATERIAL9 MatTmp = pMat[nCntMat].MatD3D;

		//色変更指定アリ
		if (pDiffuse != nullptr)
		{
			//色変え
			pMat[nCntMat].MatD3D.Diffuse = *pDiffuse;
		}

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(pXModel->pTextureID[nCntMat]));

		//モデル(パーツ)の描画
		pXModel->pMesh->DrawSubset(nCntMat);

		//保管したマテリアルを戻す
		pMat[nCntMat].MatD3D = MatTmp;
	}

	//αテストを無効にする
	pRender->SetAlphaTest(false, 255);

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=======================================
//移動量を減衰（減衰係数はデフォルト値
//=======================================
void CObjectX::MoveAttenuate(void)
{
	m_move += (VEC3_INIT - m_move) * ATTENUATE;
}

//=======================================
//重力を用いた移動減衰処理
//=======================================
void CObjectX::MoveAttenuateGravity(const float fGravityPower, const float fGravityMag)
{
	/*Ｘベクトル減衰*/ m_move.x += (0.0f - m_move.x) * ATTENUATE;
	/*Ｚベクトル減衰*/ m_move.z += (0.0f - m_move.z) * ATTENUATE;
	/*Ｙベクトル重力に従わせる*/ m_move.y -= (fGravityPower + m_move.y) * fGravityMag;
}

//=======================================
//既存のⅩモデルを返すか、新規登録をする
//=======================================
int CObjectX::Regist(char *pFileName, XMODELTYPE type, int nLife)
{
	// Ⅹモデル番号
	int nXModelIdx = 0;

	//すでに登録されたⅩモデルと同じものがないか確認
	for (nXModelIdx = 0; nXModelIdx < m_nNumAll; nXModelIdx++)
	{
		//指定されたファイル名と既に登録されてあるファイル名が一致する
		if (strcmp(pFileName, m_pFilePath[nXModelIdx]) == 0)
		{
			//既に登録されているⅩモデルを返す
			return nXModelIdx;
		}
	}

	//==============================================
	//新規Ⅹモデル登録
	//==============================================
	/*総数が最大数未満*/
	if (m_nNumAll < MAX_X_MODEL)
	{
		//デバイスへのポインタ
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//メモリが確保されていない
		if (m_apXModel[nXModelIdx] == NULL)
		{
			//インスタンスを生成する
			m_apXModel[nXModelIdx] = new XModel;
		}

		//インスタンスが生成されている
		if (m_apXModel[nXModelIdx] != NULL &&
			D3DXLoadMeshFromX(pFileName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_apXModel[nXModelIdx]->pBuffMat, NULL,
			&m_apXModel[nXModelIdx]->dwNumMat, &m_apXModel[nXModelIdx]->pMesh) == D3D_OK)
		{
			/*Ⅹモデルのポインタ取得*/	 XModel *pXModel = m_apXModel[nXModelIdx];

			/*マテリアル分のポインタ確保*/	pXModel->pTextureID = new int[(int)pXModel->dwNumMat];
			/*マテリアルデータのポインタ*/	D3DXMATERIAL *pMat = (D3DXMATERIAL *)pXModel->pBuffMat->GetBufferPointer();

			//+++++++++++++++++++++++++++++++++++++++++++++++
			//テクスチャの設定
			//+++++++++++++++++++++++++++++++++++++++++++++++
			for (int nCntMat = 0; nCntMat < (int)pXModel->dwNumMat; nCntMat++)
			{
				//エラー番号を初期値として代入
				pXModel->pTextureID[nCntMat] = ERROR_ID;

				//テクスチャファイルが存在する
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//テクスチャ番号格納 <- テクスチャの読み込み
					pXModel->pTextureID[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
				}
			}
			//-----------------------------------------------
			//テクスチャ設定終了
			//-----------------------------------------------

			//+++++++++++++++++++++++++++++++++++++++++++++++
			//最小・最高座標の設定
			//+++++++++++++++++++++++++++++++++++++++++++++++
			{
				/*頂点バッファへのポインタ*/ BYTE *pVtxBuff;
				/*メッシュのポインタ取得*/	 LPD3DXMESH pMesh = pXModel->pMesh;

				/*頂点数を取得する*/	const int	nNumVtx = pMesh->GetNumVertices();
				/*頂点のサイズを取得*/	const DWORD dwSizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

				//頂点バッファをロック
				pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

				//最初の頂点座標を最小・最大座標に設定
				pXModel->MinPos = pXModel->MaxPos = *(D3DXVECTOR3 *)pVtxBuff;

				pVtxBuff += dwSizeFVF;						//頂点フォーマットのサイズ分、ポインタを進める
				D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;	//２番目の頂点座標を取得する

				for (int nCntVtx = 1; nCntVtx < nNumVtx; nCntVtx++)
				{
					//---------------------------------------
					//設定されている座標より小さかったら代入
					//---------------------------------------
					pXModel->MinPos.x = vtx.x < pXModel->MinPos.x ? vtx.x : pXModel->MinPos.x;
					pXModel->MinPos.y = vtx.y < pXModel->MinPos.y ? vtx.y : pXModel->MinPos.y;
					pXModel->MinPos.z = vtx.z < pXModel->MinPos.z ? vtx.z : pXModel->MinPos.z;

					//---------------------------------------
					//設定されている座標より大きかったら代入
					//---------------------------------------
					pXModel->MaxPos.x = vtx.x > pXModel->MaxPos.x ? vtx.x : pXModel->MaxPos.x;
					pXModel->MaxPos.y = vtx.y > pXModel->MaxPos.y ? vtx.y : pXModel->MaxPos.y;
					pXModel->MaxPos.z = vtx.z > pXModel->MaxPos.z ? vtx.z : pXModel->MaxPos.z;

					pVtxBuff += dwSizeFVF;			//頂点フォーマットのサイズ分、ポインタを進める
					vtx = *(D3DXVECTOR3 *)pVtxBuff;	//頂点座標の代入
				}

				//頂点バッファをアンロック
				pXModel->pMesh->UnlockVertexBuffer();
			}
			//-----------------------------------------------
			//最小・最高座標設定終了
			//-----------------------------------------------

			//+++++++++++++++++++++++++++++++++++++++++++++++
			//当たり判定の範囲設定（高さは考慮しない
			//+++++++++++++++++++++++++++++++++++++++++++++++
			{
				//Ｘ・Ｚの最大範囲を格納
				D3DXVECTOR3 MaxMargin = VEC3_INIT;
				const D3DXVECTOR3 MaxPos = pXModel->MaxPos;
				const D3DXVECTOR3 MinPos = pXModel->MinPos;

				//原点から一番遠いほうの座標を格納する
				MaxMargin.x = fabsf(MinPos.x) <= fabsf(MaxPos.x) ? MaxPos.x : MinPos.x;
				MaxMargin.z = fabsf(MinPos.z) <= fabsf(MaxPos.z) ? MaxPos.z : MinPos.z;

				//当たり判定の範囲を設定する
				pXModel->fMargin = D3DXVec3Length(&MaxMargin);
			}
			//-----------------------------------------------
			//当たり判定の範囲設定終了
			//-----------------------------------------------

			//+++++++++++++++++++++++++++++++++++++++++++++++
			//ファイルバスを設定
			//+++++++++++++++++++++++++++++++++++++++++++++++
			{
				//もしもメモリが使われていたら、破棄する
				if (m_pFilePath[nXModelIdx] != NULL)
				{
					delete m_pFilePath[nXModelIdx];
					m_pFilePath[nXModelIdx] = NULL;
				}

				/*パスの長さ＋終端文字分のメモリ確保*/	m_pFilePath[nXModelIdx] = new char[(int)strlen(pFileName) + 1];
				/*Ⅹモデルのファイルパスを保存*/		strcpy(m_pFilePath[nXModelIdx], pFileName);
			}
			//-----------------------------------------------
			//ファイルバス設定終了
			//-----------------------------------------------

			/*個性の設定*/	pXModel->Type = type;
			/*体力の設定*/	pXModel->nLife = nLife;

			/*Ⅹモデルの総数を増やす*/	m_nNumAll++;
			/*Ⅹモデルの番号を返す*/	return nXModelIdx;
		}
	}

	//新規登録失敗でエラー番号を返す
	return ERROR_ID;
}

//=======================================
//指定の番号のⅩモデルを返す
//=======================================
CObjectX::XModel *CObjectX::GetXModel(int nXModelID)
{
	//配列の範囲外か、総数より多い番号が指定された
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll)
	{
		//そんなものはないので、NULLを返す
		return NULL;
	}

	//正しく指定されたⅩモデルを返す
	return m_apXModel[nXModelID];
}

//=======================================
//当たり判定の範囲取得（高さは考慮してない
//=======================================
float CObjectX::GetXModelMargin(int nXModelID)
{
	//配列の範囲外を指定されたらそんなものはないので、無個性を返す
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll) return 0.0f;

	//当たり判定の範囲を返す
	return m_apXModel[nXModelID]->fMargin;
}

//=======================================
//Ⅹモデルの個性を取得
//=======================================
CObjectX::XMODELTYPE CObjectX::GetXModelType(int nXModelID)
{
	//配列の範囲外を指定されたらそんなものはないので、無個性を返す
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll) return XMODELTYPE_NONE;

	//個性を返す
	return m_apXModel[nXModelID]->Type;
}

//=======================================
//Ⅹモデルの体力を取得
//=======================================
int CObjectX::GetXModelLife(int nXModelID)
{
	//配列の範囲外を指定されたらそんなものはないので、０を返す
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll) return 0;

	//体力を返す
	return m_apXModel[nXModelID]->nLife;
}

//=======================================
//ＸＹＺのどれかの最小・最高座標を返す
//=======================================
float CObjectX::GetPosition(int nXModelID, TYPE posType, TYPE typeH_L)
{
	//配列の範囲が指定されたらそんなものはないので、0.0fを返す
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll) return 0.0f;

	//最大座標を返す場合
	if (typeH_L == TYPE_MAX)
	{
		//座標の種類別で値を返す
		switch (posType)
		{
		case TYPE_X:return m_apXModel[nXModelID]->MaxPos.x;	break;	//Ｘ座標を返す
		case TYPE_Y:return m_apXModel[nXModelID]->MaxPos.y;	break;	//Ｙ座標を返す
		case TYPE_Z:return m_apXModel[nXModelID]->MaxPos.z;	break;	//Ｚ座標を返す
		default:	return 0.0f; break;
		}
	}
	//最小座標を返す場合
	else if (typeH_L == TYPE_MIN)
	{
		//座標の種類別で値を返す
		switch (posType)
		{
			case TYPE_X:return m_apXModel[nXModelID]->MinPos.x;	break;	//Ｘ座標を返す
			case TYPE_Y:return m_apXModel[nXModelID]->MinPos.y;	break;	//Ｙ座標を返す
			case TYPE_Z:return m_apXModel[nXModelID]->MinPos.z;	break;	//Ｚ座標を返す
			default: return 0.0f; break;
		}
	}

	//引数に不適切な値が入った場合
	else return 0.0f;
}

//=======================================
//３頂点の最小・最高座標を返す
//=======================================
D3DXVECTOR3 CObjectX::GetPosition(int nXModelID, TYPE typeH_L)
{
	//座標を返す
	return D3DXVECTOR3(
		GetPosition(nXModelID, TYPE_X, typeH_L),	//Ｘ座標を取得
		GetPosition(nXModelID, TYPE_Y, typeH_L),	//Ｙ座標を取得
		GetPosition(nXModelID, TYPE_Z, typeH_L));	//Ｚ座標を取得
}

//=======================================
//３頂点の最小・最高座標を返す
//=======================================
D3DXVECTOR3 CObjectX::GetPosition(int nXModelID, TYPE Xtype, TYPE Ytype, TYPE Ztype)
{
	//座標を返す
	return D3DXVECTOR3(
		GetPosition(nXModelID, TYPE_X, Xtype),	//Ｘ座標を取得
		GetPosition(nXModelID, TYPE_Y, Ytype),	//Ｙ座標を取得
		GetPosition(nXModelID, TYPE_Z, Ztype));	//Ｚ座標を取得
}

//=======================================
//Ⅹモデルとの当たり判定（人間状態）
//=======================================
void CObjectX::CollisionXModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, TYPE nType, float fRange, float fMarginSize)
{
	//引数に必要な情報がそろっていなかったら引き返す
	if (pPos == NULL || pMove == NULL || nType != TYPE_X && nType != TYPE_Y && nType != TYPE_Z) return;

	//オブジェクトの総数
	const int NUM_OBJECT = CObject::GetNumAll(CObject::OBJECT_PRIORITY_DEF);

	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		//Ⅹモデルを取得
		CObjectX *pObjX = (CObjectX *)CObject::GetObject(CObject::OBJECT_PRIORITY_DEF, nCntModel);

		//Ⅹモデルかどうか判定
		if (pObjX == NULL || pObjX->GetType() != CObject::TYPE_XMODEL || !pObjX->m_bDisp) continue;

		/*オブジェクトの現在位置*/ D3DXVECTOR3 ObjPos = pObjX->GetPosition();
		/*オブジェクトまでの差分*/ D3DXVECTOR3 posDiff = *pPos - ObjPos;
		/*オブジェクトまでの距離*/ float fXModelLength = D3DXVec3Length(&posDiff);

		//オブジェクトの距離が当たり判定の範囲内
		if (fXModelLength > fRange) continue;

		//最小位置、最大位置を取得
		D3DXVECTOR3 ObjMinPos = ObjPos + pObjX->GetPosition(pObjX->m_nXmodelID, TYPE_MIN);
		D3DXVECTOR3 ObjMaxPos = ObjPos + pObjX->GetPosition(pObjX->m_nXmodelID, TYPE_MAX);

		//XZのみの範囲で、現在地が中に入っていない
		if (!IsBoxCollision(
			D3DXVECTOR3(pPos->x - fMarginSize, pPos->y - fMarginSize, pPos->z - fMarginSize),	//自分の最小位置
			D3DXVECTOR3(pPos->x + fMarginSize, pPos->y + fMarginSize, pPos->z + fMarginSize),	//自分の最大位置
			ObjMinPos, ObjMaxPos))
			continue;

		//X座標の修正
		if (nType == TYPE_X)
		{
			//前回の位置が、オブジェクトの最小位置よりマイナスの方にいた
			if (PosOld.x <= ObjMinPos.x)		pPos->x = ObjMinPos.x - fMarginSize;

			//前回の位置が、オブジェクトの最大位置よりプラスの方にいた
			else if(PosOld.x >= ObjMaxPos.x)	pPos->x = ObjMaxPos.x + fMarginSize;

			//移動量を消す
			pMove->x = 0.0f;
		}

		//Z座標の修正
		else if (nType == TYPE_Z)
		{
			//前回はオブジェの最小位置にめり込んでなかった
			if (PosOld.z <= ObjMinPos.z)		pPos->z = ObjMinPos.z - fMarginSize;

			//前回はオブジェの最大位置にめり込んでなかった
			else if (PosOld.z >= ObjMaxPos.z)	pPos->z = ObjMaxPos.z + fMarginSize;

			//移動量を消す
			pMove->z = 0.0f;
		}
		break;
	}
}

//=======================================
//Ⅹモデルとの当たり判定（変身状態）
//=======================================
void CObjectX::CollisionXModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, TYPE nType, float fMargin, int nXModelID)
{
	//引数に必要な情報がそろっていなかったら引き返す
	if (pPos == NULL || pMove == NULL || nType != TYPE_X && nType != TYPE_Y && nType != TYPE_Z) return;

	//オブジェクトの総数
	const int NUM_OBJECT = CObject::GetNumAll(CObject::OBJECT_PRIORITY_DEF);

	//変身したオブジェクトの最小・最大位置取得
	D3DXVECTOR3 MinPos = GetPosition(nXModelID, TYPE_MIN);
	D3DXVECTOR3 MaxPos = GetPosition(nXModelID, TYPE_MAX);
	D3DXVECTOR3 SelfMinPos	= *pPos + MinPos;
	D3DXVECTOR3 SelfMaxPos	= *pPos + MaxPos;
	D3DXVECTOR3 SelfMinPosOld = PosOld + MinPos;
	D3DXVECTOR3 SelfMaxPosOld = PosOld + MaxPos;

	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		//Ⅹモデルを取得
		CObjectX *pObjX = (CObjectX *)CObject::GetObject(CObject::OBJECT_PRIORITY_DEF, nCntModel);

		//Ⅹモデルかどうか判定
		if (pObjX != NULL && pObjX->GetType() == CObject::TYPE_XMODEL && pObjX->m_bDisp)
		{
			/*オブジェクトの現在位置*/ D3DXVECTOR3 ObjPos = pObjX->GetPosition();
			/*オブジェクトまでの差分*/ D3DXVECTOR3 posDiff = *pPos - ObjPos;
			/*オブジェクトまでの距離*/ float fXModelLength = D3DXVec3Length(&posDiff);

			//最小位置、最大位置を取得
			const D3DXVECTOR3 ObjMinPos = ObjPos + pObjX->GetPosition(pObjX->m_nXmodelID, TYPE_MIN);
			const D3DXVECTOR3 ObjMaxPos = ObjPos + pObjX->GetPosition(pObjX->m_nXmodelID, TYPE_MAX);

			//オブジェクトの距離が当たり判定の範囲内
			if (fXModelLength <= fMargin + pObjX->GetXModelMargin(pObjX->m_nXmodelID))
			{
				//以降のコメントの説明
				if (!IsBoxCollision(SelfMinPos, SelfMaxPos, ObjMinPos, ObjMaxPos)) continue;

				//++++++++++++++++++++++++++++++++++++++
				//Ｚ座標の修正
				//++++++++++++++++++++++++++++++++++++++
				if (nType == TYPE_Z)
				{
					//前回はオブジェの最小位置にめり込んでなかった
					if (SelfMaxPosOld.z <= ObjMinPos.z)		 pPos->z = ObjMinPos.z - MaxPos.z;

					//前回はオブジェの最小位置にめり込んでなかった
					else if (SelfMinPosOld.z >= ObjMaxPos.z) pPos->z = ObjMaxPos.z - MinPos.z;
				}

				//++++++++++++++++++++++++++++++++++++++
				//Ｘ座標の修正
				//++++++++++++++++++++++++++++++++++++++
				else if (nType == TYPE_X)
				{
					//前回はオブジェの最小位置にめり込んでなかった
					if (SelfMaxPosOld.x <= ObjMinPos.x)		 pPos->x = ObjMinPos.x - MaxPos.x;

					//前回はオブジェの最小位置にめり込んでなかった
					else if (SelfMinPosOld.x >= ObjMaxPos.x) pPos->x = ObjMaxPos.x - MinPos.x;
				}
			}
		}
	}
}

//=======================================
//収納できるオブジェクトを取得
//=======================================
CObjectX *CObjectX::TakeXModel(D3DXVECTOR3 pos, float rot, float fRangeAngle, float fLength)
{
	//オブジェクトの総数
	const int NUM_OBJECT = CObject::GetNumAll(CObject::OBJECT_PRIORITY_DEF);

	//左右の角度を計算する
	float fLeftAngle = rot + fRangeAngle; FixRotate(&fLeftAngle);
	float fRightAngle = rot - fRangeAngle; FixRotate(&fRightAngle);

	//変身するオブジェクトまでの距離が一番小さいものを格納
	float fMinLength = fLength;

	//収納するⅩオブジェクトを収納
	CObjectX *pTakeObjX = NULL;

	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		//Ⅹモデルを取得
		CObjectX *pObjX = (CObjectX *)CObject::GetObject(CObject::OBJECT_PRIORITY_DEF, nCntModel);

		//Ⅹモデルかどうか判定
		if (pObjX == NULL || pObjX->GetType() != CObject::TYPE_XMODEL || !pObjX->m_bDisp) continue;

		//Ⅹモデルの座標
		const D3DXVECTOR3 XModelPos = pObjX->GetPosition();
		const float VecXModelLength = D3DXVec3Length(&(XModelPos - pos));

		//範囲外にいた
		if (fMinLength < VecXModelLength || !Search(pos, XModelPos, fLeftAngle, fRightAngle, fLength)) continue;

		//範囲内に居る
		fMinLength = VecXModelLength;	//変身可能オブジェクトの最短距離を更新
		pTakeObjX = pObjX;				//Ⅹオブジェクトのポインタを保存
	}

	//Ⅹオブジェクトを返す
	return pTakeObjX;
}

//=======================================
//目の前に変身できるオブジェクトがあるかどうか判定
//=======================================
int CObjectX::TransformXModel(D3DXVECTOR3 pos, float rot, float fRangeAngle, float fLength, int nTransformModelID)
{
	//オブジェクトの総数
	const int NUM_OBJECT = CObject::GetNumAll(CObject::OBJECT_PRIORITY_DEF);

	//左右の角度を計算する
	float fLeftAngle  = rot + fRangeAngle; FixRotate(&fLeftAngle);
	float fRightAngle = rot - fRangeAngle; FixRotate(&fRightAngle);

	float fMinLength = fLength;	//変身するオブジェクトまでの距離が一番小さいものを格納
	int nTransformID = ERROR_ID;//変身するオブジェクトの番号を取得

	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		//Ⅹモデルを取得
		CObjectX *pObjX = (CObjectX *)CObject::GetObject(CObject::OBJECT_PRIORITY_DEF, nCntModel);

		//Ⅹモデルかどうか判定
		if (pObjX == NULL || pObjX->GetType() != CObject::TYPE_XMODEL || !pObjX->m_bDisp) continue;

		//Ⅹモデルの座標
		const D3DXVECTOR3 XModelPos = pObjX->GetPosition();
		const float VecXModelLength = D3DXVec3Length(&(XModelPos - pos));

		//範囲外にいた
		if(fMinLength < VecXModelLength ||	!Search(pos, XModelPos, fLeftAngle, fRightAngle, fLength)) continue;

		//範囲内に居る
		fMinLength = VecXModelLength;		//変身可能オブジェクトの最短距離を更新
		nTransformID = pObjX->m_nXmodelID;	//オブジェクトの番号を格納する

		//ターゲットポリゴンを表示
		SetTargetPolygon(true, XModelPos, GetPosition(nTransformID, TYPE_Y, (TYPE)TYPE_MAX));
	}

	//人間 or 今の変身している番号と同じなら、ターゲットポリゴンを表示しない
	if (nTransformID == ERROR_ID || nTransformID == nTransformModelID) SetTargetPolygon(false);

	//変身番号を返す
	return nTransformID;
}

//=======================================
//ターゲットポリゴンの設定
//---------------------------------------
//引数１　bDisp		：表示するか否か
//引数２　TargetPos ：ターゲットの位置
//引数３　MaxPosY	：ターゲットの最高Ｙ座標
//=======================================
void CObjectX::SetTargetPolygon(bool bDisp, D3DXVECTOR3 TargetPos, float MaxPosY)
{
	//表示するかどうか設定
	m_pTarget3D->DispSwitch(bDisp);
	m_pTargetBill->DispSwitch(bDisp);

	//3Dターゲットの位置情報設定
	m_pTarget3D->SetInfo(TargetPos, TARGET_3D_SIZE, 0.0f, TARGET_3D_SIZE);

	//ビルボードターゲットは発生位置を浮かせて設定
	TargetPos.y = MaxPosY + TARGET_BILL_FLOAT;
	m_pTargetBill->SetInfo(TargetPos, TARGET_BILL_SIZE, TARGET_BILL_SIZE, 0.0f);
}

//=======================================
//描画スイッチON/OFF
//=======================================
void CObjectX::DispSwitch(bool bDisp)
{
	//描画ON/OFF（影も同じ設定
	m_bDisp = bDisp;
	CShadow::DispSwitch(m_nShadowID, bDisp);
	CShadow::SetPos(m_nShadowID, GetPosition());
}