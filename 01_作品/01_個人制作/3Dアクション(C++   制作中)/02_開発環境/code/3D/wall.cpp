//================================================================================================
//
//壁処理[wall.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "object3D.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "wall.h"

//壁の処理優先順位
#define PRIORITY_WALL	(2)
//当たり判定を行う頂点の数
#define NUM_COLLI_VTX	(3)
//テクスチャのパス
#define TEX_PATH		"data\\TEXTURE\\wall_dark.png"
//テクスチャの分割数
#define TEX_NUM_SEPARATE	(1.0f)

//=======================================
//コンストラクタ
//=======================================
CWall::CWall() : CObject3D(PRIORITY_WALL)
{
	//メンバ変数クリア
	m_nTextureID = ERROR_ID;
}

//=======================================
//デストラクタ
//=======================================
CWall::~CWall()
{

}

//=======================================
//生成処理
//=======================================
CWall *CWall::Create(void)
{
	//インスタンス生成
	CWall *pWall = new CWall;

	//インスタンスが生成された
	if (pWall != NULL)
	{
		//初期化処理に失敗
		if (FAILED(pWall->Init()))
		{
			delete pWall;	//メモリを解放
			pWall = NULL;	//NULLポインタに
			return NULL;	//NULLを返す
		}

		//テクスチャ読み込み
		pWall->Load();

		//テクスチャ割り当て
		pWall->BindTexture(CManager::GetTexture()->GetAddress(pWall->m_nTextureID));
	}

	//インスタンスを返す
	return pWall;
}

//=======================================
//初期化処理
//=======================================
HRESULT CWall::Init(void)
{
	//初期化処理
	if (FAILED(CObject3D::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(TEX_NUM_SEPARATE, TEX_NUM_SEPARATE))))
	{
		//初期化失敗
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_WALL);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CWall::Uninit(void)
{
	//終了処理
	CObject3D::Uninit();
}

//=======================================
//更新処理
//=======================================
void CWall::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CWall::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//両面カリングを行う
	if (CManager::GetRenderer()->Culling())
	{
		//両面カリングON
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//描画処理
	CObject3D::Draw();

	//通常カリングにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=======================================
//テクスチャ読み込み設定
//=======================================
HRESULT CWall::Load(void)
{
	//テクスチャ読み込み
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//テクスチャ読み込み成功
	return S_OK;
}

//=======================================
//設定処理
//=======================================
void CWall::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const D3DXVECTOR2 texSize)
{
	/* 位置設定 */	SetTransrate(pos, VEC3_INIT, rot);
	/*サイズ設定*/	SetSize(size);

	//頂点座標設定
	SetVertexInfo(size.x, size.y, size.z, VEC2_INIT, texSize.x, texSize.y);
}

//=======================================
//当たり判定処理
//※コメントの中の「PL」はプレイヤーの事
//=======================================
bool CWall::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, float fMargin, float fHeight)
{
	//移動ベクトルが０なら、当たり判定を行う必要がないので引き返す
	if (fabsf(pMove->x) <= 0.00001f && fabsf(pMove->z) <= 0.00001f) return false;

	//壁に一番近い位置を格納
	D3DXVECTOR3 NearestPos = VEC3_INIT;

	//壁の優先順位帯のオブジェクトの総数を取得
	const int NUM_OBJECT = CObject::GetNumAll(PRIORITY_WALL);

	for (int nCntWall = 0; nCntWall < NUM_OBJECT; nCntWall++)
	{
		//オブジェクトを取得する
		CWall *pWall = (CWall *)CObject::GetObject(PRIORITY_WALL, nCntWall);

		//壁オブジェクトを取得できなかったら、処理を飛ばす
		if (pWall == NULL || pWall->GetType() != CObject::TYPE_WALL) continue;

		//壁の座標・サイズ・回転値（Ｙ軸）を取得する
		const D3DXVECTOR3 pos = pWall->GetPosition();
		const D3DXVECTOR3 size = pWall->GetSize3D();
		const float rotY = pWall->GetRotation().y;

		//壁の底辺より頭の位置が低い
		if (pos.y - size.y > pPos->y + fHeight) continue;

		//左右の頂点のワールド座標を計算
		const D3DXVECTOR3 LeftPos = GetWorldPosition(PI_LEFT + rotY, size, pos);
		const D3DXVECTOR3 RightPos = GetWorldPosition(PI_RIGHT + rotY, size, pos);

		//+++++++++++++++++++++++++++++
		//壁に一番近い位置を算出
		//+++++++++++++++++++++++++++++
		{
			//一番近い角度を格納
			const float NearRot = NearRotate(rotY, fMargin, *pPos, LeftPos, RightPos);

			//壁に一番近い位置を算出
			NearestPos.x = pPos->x - sinf(NearRot) * fMargin;
			NearestPos.z = pPos->z - cosf(NearRot) * fMargin;
		}

		//境界線ベクトル（左右の頂点のベクトル
		D3DXVECTOR3 VecLine = RightPos - LeftPos;

		//+++++++++++++++++++++++++++++
		//壁の裏側にいるか
		//+++++++++++++++++++++++++++++
		{
			//左端から一番近い座標までのベクトル
			const D3DXVECTOR3 vecLeftPos = NearestPos - LeftPos;

			//現在位置が、境界線ベクトルの左側に居なければ処理を飛ばす
			if (VecLine.x * vecLeftPos.z - VecLine.z * vecLeftPos.x < 0.0f) continue;
		}

		//前回・現在の壁に一番近い距離の間を移動ベクトルとする
		D3DXVECTOR3 VecMove = NearestPos - *pPos;

		//+++++++++++++++++++++++++++++
		//交点を計算する
		//+++++++++++++++++++++++++++++
		D3DXVECTOR3 CrossPos = CrossPoint(NearestPos, VecMove, LeftPos, RightPos);

		//前回位置から、交点までの長さを求める
		const float PosOldLength = D3DXVec3Length(&(*pPos - CrossPos));

		//交点までの距離が移動ベクトルより大きい（まだ交点を追い越していない
		if (PosOldLength > D3DXVec3Length(&VecMove)) continue;

		//境界線ベクトルの距離
		const float VecLineLength = D3DXVec3Length(&VecLine);

		//頂点から交点までの長さが、左右の頂点までの長さの中に納まってなければ引き返す
		if (D3DXVec3Length(&(CrossPos - LeftPos)) > VecLineLength ||
			D3DXVec3Length(&(CrossPos - RightPos)) > VecLineLength) continue;

		//+++++++++++++++++++++++++++++
		//壁ずりする交点を求める
		//+++++++++++++++++++++++++++++
		{
			//現在位置を壁と直角の方向に戻した時の座標を格納
			const D3DXVECTOR3 ReturnPos = D3DXVECTOR3(pPos->x - sinf(rotY) * VecLineLength, 0.0f,
													  pPos->z - cosf(rotY) * VecLineLength);

			//交点座標を算出する
			CrossPos = CrossPoint(ReturnPos, ReturnPos - *pPos, LeftPos, RightPos);
		}

		//+++++++++++++++++++++++++++++
		//交点の位置に戻す（体形も考慮して
		//+++++++++++++++++++++++++++++
		pPos->x += CrossPos.x - NearestPos.x;
		pPos->z += CrossPos.z - NearestPos.z;

		//当たった
		return true;
	}

	return false;
}

//==========================================
//対象までの間に壁があるかどうか判定
//引数１　始点位置
//引数２　終点位置
//返り値：自分と目標の間に壁があるかどうか　true：壁がある　false：壁はない
//==========================================
bool CWall::IsAmongWall(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	return false;

	//壁と同じ優先順位のオブジェクトの総数
	const int NUM_OBJECT = CObject::GetNumAll(PRIORITY_WALL);

	//引数から得られる情報を格納
	const D3DXVECTOR3 VecMove = EndPos - StartPos;			//移動ベクトル
	const float vecMoveLength = D3DXVec3Length(&VecMove);	//移動ベクトルの長さ

	for (int nCntWall = 0; nCntWall < NUM_OBJECT; nCntWall++)
	{
		//オブジェクト取得
		CWall *pWall = (CWall *)CObject::GetObjectA(PRIORITY_WALL, nCntWall++);

		//壁じゃないオブジェクトを取得した
		if (pWall == NULL || pWall->GetType() != CObject::TYPE_WALL) continue;

		//壁の座標・サイズ・回転値（Ｙ軸）を取得する
		const D3DXVECTOR3 WallPos = pWall->GetPosition();
		const D3DXVECTOR3 WallSize = pWall->GetSize3D();
		const float rotY = pWall->GetRotation().y;

		//左右の頂点のワールド座標を計算
		const D3DXVECTOR3 LeftPos = GetWorldPosition(PI_LEFT + rotY, WallSize, WallPos);
		const D3DXVECTOR3 RightPos = GetWorldPosition(PI_RIGHT + rotY, WallSize, WallPos);
		const D3DXVECTOR3 VecLine = RightPos - LeftPos;
		const float vecLineLength = D3DXVec3Length(&VecLine);

		//+++++++++++++++++++++++++++++
		//交点を計算する
		//+++++++++++++++++++++++++++++
		D3DXVECTOR3 CrossPos = CrossPoint(StartPos, VecMove, LeftPos, RightPos);

		{
			//始点～交点までのベクトル
			const D3DXVECTOR3 VecCross = StartPos - CrossPos;

			//交点までの距離が移動ベクトルを上回っていたらダメ
			if (vecMoveLength < D3DXVec3Length(&VecCross)) continue;
		}

		//左右の頂点から交点までのベクトルを取得
		D3DXVECTOR3 vecLeftCross = LeftPos - CrossPos;
		D3DXVECTOR3 vecRightCross = RightPos - CrossPos;

		//左右の頂点から交点までのベクトルの長さが、どちらかでも頂点同士のベクトルより長いとダメ
		if (D3DXVec3Length(&vecLeftCross)  <= vecLineLength &&
			D3DXVec3Length(&vecRightCross) <= vecLineLength)return true;
	}

	//壁はなかった
	return false;
}