//================================================================================================
//
//床処理[floor.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "object3D.h"
#include "floor.h"

//床の処理優先順位
#define PRIORITY_FIELD	(2)
//当たり判定を行う頂点の数
#define NUM_COLLI_VTX	(3)
//テクスチャの分割数
#define TEX_NUM_SEPARATE	(1.0f)

int	CFloor::m_nTextureID = ERROR_ID;

//=======================================
//コンストラクタ
//=======================================
CFloor::CFloor() : CObject3D(PRIORITY_FIELD)
{
	//メンバ変数クリア
	m_nTextureID = ERROR_ID;
	m_fAddHeight = m_fAddHeightOld = 0.0f;
	m_fLandHeight = m_fLandHeightOld = 0.0f;
}

//=======================================
//デストラクタ
//=======================================
CFloor::~CFloor()
{

}

//=======================================
//生成処理
//=======================================
CFloor *CFloor::Create(void)
{
	//インスタンス生成
	CFloor *pFloor = new CFloor;

	//インスタンスが生成された
	if (pFloor != NULL)
	{
		//初期化処理に失敗
		if (FAILED(pFloor->Init()))
		{
			delete pFloor;//メモリを解放
			pFloor = NULL;//NULLポインタに
			return NULL;	//NULLを返す
		}
	}

	//インスタンスを返す
	return pFloor;
}

//=======================================
//初期化処理
//=======================================
HRESULT CFloor::Init(void)
{
	//初期化処理
	if (FAILED(CObject3D::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(TEX_NUM_SEPARATE, TEX_NUM_SEPARATE))))
	{
		//初期化失敗
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_FLOOR);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CFloor::Uninit(void)
{
	//終了処理
	CObject3D::Uninit();
}

//=======================================
//更新処理
//=======================================
void CFloor::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CFloor::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//両面カリングON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//描画処理
	CObject3D::Draw();

	//通常カリングにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CFloor::Load(char *pPath)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(pPath);

	CObject3D::BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));

	//テクスチャ読み込み成功
	return S_OK;
}

//=======================================
//設定処理
//=======================================
void CFloor::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const D3DXVECTOR2 texSize)
{
	/* 位置設定 */	SetTransrate(pos, VEC3_INIT, rot);
	/*サイズ設定*/	SetSize(size);

	//頂点座標設定
	SetVertexInfo(size.x, size.y, size.z, VEC2_INIT, texSize.x, texSize.y);
}

//=======================================
//当たり判定処理
//=======================================
bool CFloor::LandField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	//床の優先順位帯のオブジェクトの総数を取得
	const int NUM_OBJECT = CObject::GetNumAll(PRIORITY_FIELD);

	//乗ったかどうかを判定する
	bool bLand = false;

	for (int nCntObj = 0; nCntObj < NUM_OBJECT; nCntObj++)
	{
		//床と同じ優先順位のオブジェクトを取得
		CObject *pFloor = GetObject(PRIORITY_FIELD, nCntObj);

		//オブジェクトが無事取得でき、そのオブジェクトのタイプが床である
		if (pFloor != NULL && pFloor->GetType() == CObject::TYPE_FLOOR)
		{
			/* 位置を取得 */	D3DXVECTOR3 FieldPos = pFloor->GetPosition();
			/* 横幅を取得 */	float fWidth = pFloor->GetWidth();
			/* 奥行を取得 */	float fDepth = pFloor->GetDepth();

			//床を真上から見下ろしたとき、その中にプレイヤーがいる
			if (FieldPos.x - fWidth <= pPos->x && pPos->x <= FieldPos.x + fWidth &&
				FieldPos.z - fDepth <= pPos->z && pPos->z <= FieldPos.z + fDepth)
			{
				if (FieldPos.y <= pPosOld->y &&	//前回は床の上に居て
					FieldPos.y >= pPos->y)		//現在は床にめり込んでいる
				{
					pPos->y = FieldPos.y;	//床の上に戻す
					pMove->y = 0.0f;		//落下速度を０に
					bLand = true;			//乗った
				}
			}
		}
	}

	//乗ったかどうかを返す
	return bLand;
}