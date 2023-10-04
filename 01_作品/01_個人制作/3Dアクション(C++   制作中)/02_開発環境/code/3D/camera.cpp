//================================================================================================
//
//カメラ処理[camera.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "object3D.h"
#include "../Player/player.h"
#include "camera.h"

#define POS_POSV			D3DXVECTOR3(0.0f, 50.0f, -330.0f)	//視点の初期位置
#define POS_POSR			D3DXVECTOR3(0.0f, 0.0f, 0.0f)		//注視点の初期位置
#define POS_POSR_OFFSET		D3DXVECTOR3(0.0f, 50.0f, 0.0f)		//注視点オフセットの初期位置
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//上方向ベクトルの初期値
#define VIEW_ANGLE			(45.0f)				//視野角
#define MOVE_CAMERA_SPEED	(0.3f)				//カメラ・注視点座標の移動量
#define MIN_DRAW_Z			(10.0f)				//描画処理の最低奥行き
#define MAX_DRAW_Z			(2000.0f)			//描画処理の最高奥行き
#define MIN_LENGTH			(300.0f)			//視点と注視点の最低距離
#define MAX_LENGTH			(400.0f)			//視点と注視点の最大距離
#define DEFAULT_LENGTH		(350.0f)			//視点と注視点の標準距離
#define CHANGE_LENGHT		(0.1f)				//長さの移動量
#define SPIN_ROT			(0.005f)			//回転量
#define ROT_X_MAX_RANGE		(PI_RIGHT - 0.0001f)//Ⅹ軸の最大回転範囲（完全な半円だと限界値になったときに描画できなくなるので、補正値を引いておく
#define ROT_X_MIN_RANGE		(0.00001f)			//Ⅹ軸の最低回転範囲（床にめり込まないように調整

const float CCamera::MOUSE_ROT_SPEED = 0.005f;
const float CCamera::PAD_ROT_SPEED = 0.05f;

//=======================================
//コンストラクタ
//=======================================
CCamera::CCamera()
{

}

//=======================================
//デストラクタ
//=======================================
CCamera::~CCamera()
{

}

//=======================================
//カメラの初期化処理
//=======================================
HRESULT CCamera::Init(void)
{
	//カメラ情報初期化
	ResetPos();

	//初期化成功
	return S_OK;
}

//=======================================
//カメラの終了処理
//=======================================
void CCamera::Uninit(void)
{

}

//=======================================
//カメラの更新処理
//=======================================
void CCamera::Update(void)
{
	/*キーボード取得*/	CInputMouse *pMouse = CManager::GetMouse();

	/* 視点移動	*/	MovePosV(pMouse);
	/* 視点回転	*/	SpinPosV(pMouse);
	/*注視点移動*/	MovePosR(pMouse);

	//カメラを移動
	Move();
}

//=======================================
//視点移動
//=======================================
void CCamera::MovePosV(CInputMouse *pMouse)
{
	//距離調整
	Length();
}

//=======================================
//視点回転
//=======================================
void CCamera::SpinPosV(CInputMouse *pMouse)
{
	//パッド取得
	CInputGamePad *pPad = CManager::GetGamePad();

	//パッドの右スティックが倒れている
	if (pPad->GetRStickX() != 0 || pPad->GetRStickY() != 0)
	{
		//右スティックの角度を取得
		float fRot = pPad->GetRStickRot();

		//向きを変える
		m_rot.x -= cosf(fRot) * PAD_ROT_SPEED;
		m_rot.y -= sinf(fRot) * PAD_ROT_SPEED;
	}

	//左クリックが押されてて、右クリックは押されていない
	else if (pMouse->GetPress(pMouse->CLICK_LEFT))
	{
		//向きを変える
		m_rot.x += pMouse->GetMove().y * MOUSE_ROT_SPEED;
		m_rot.y -= pMouse->GetMove().x * MOUSE_ROT_SPEED;
	}

	//移動半径を超えていないかチェック
	Range(&m_rot.x, ROT_X_MIN_RANGE, ROT_X_MAX_RANGE);

	//角度修正
	FixRotate(&m_rot.y);
}

//=======================================
//注視点移動
//=======================================
void CCamera::MovePosR(CInputMouse *pMouse)
{
	if (m_pPlayer == NULL)
	{
		//注視点の位置を  注視点の初期位置　＋　オフセット位置　に設定する
		m_posR = POS_POSR + m_posROffset;
	}
	else
	{
		//注視点の位置を  プレイヤーの位置　＋　オフセット位置　に設定する
		m_posR = m_pPlayer->GetPosition() + m_posROffset;
	}
}

//=======================================
//カメラの移動処理
//=======================================
void CCamera::Move(void)
{
	//カメラの距離再計算（カメラの移動に慣性をかける
	{
		//実際の視点間の距離を求める
		D3DXVECTOR3 PosDiff = m_posV - m_posR;
		float fLength = D3DXVec3Length(&PosDiff);

		//長さを範囲内に収める
		Range(&fLength, MIN_LENGTH, MAX_LENGTH);

		//プレイヤーが移動したことが確認できた
		if (m_pPlayer != NULL)
		{
			//プレイヤーの移動量の長さを取得
			float fVecMoveLength = m_pPlayer->GetMoveLength();

			//ほぼ止まっている
			if (fVecMoveLength <= 0.5f)
			{
				//カメラの距離を補正
				m_fLength += (DEFAULT_LENGTH - m_fLength) * 0.05f;
			}
			//移動中
			else
			{
				//カメラの距離を補正
				m_fLength += (MAX_LENGTH - (fLength + fVecMoveLength)) * 0.1f;
			}

			//長さを範囲内に収める
			Range(&m_fLength, MIN_LENGTH, MAX_LENGTH);
		}
	}

	//Ｘ・Ｚ軸の半径
	float fFlatLength = cosf(m_rot.x) * m_fLength;

	m_posV.x = m_posR.x + sinf(D3DX_PI - m_rot.y) * fFlatLength;//座標を計算
	m_posV.z = m_posR.z + cosf(D3DX_PI - m_rot.y) * fFlatLength;//座標を計算
	m_posV.y = m_posR.y + sinf(D3DX_PI - m_rot.x) * m_fLength;	//座標を計算
}

//=======================================
//カメラの位置リセット
//=======================================
void CCamera::ResetPos(void)
{
	/*オフセット*/	m_posROffset = POS_POSR_OFFSET;
	/*視点*/		m_posV = POS_POSV;
	/*向き*/		m_rot = VEC3_INIT;
	/*上方向ベクトル*/ m_vecU = POS_VECU;

	//プレイヤーのポインタがある
	if (m_pPlayer != NULL)
	{
		//プレイヤーの位置に注視点を置く
		m_posR = m_pPlayer->GetPosition() + m_posROffset;
	}
	//無ければ原点座標にする
	else
	{
		m_posR = VEC3_INIT + m_posROffset;
	}

	//長さを算出
	m_fLength = D3DXVec3Length(&(m_posR - m_posV));

	//角度を算出する
	CalculateRotate();

	//位置を設定する
	Move();
}

//=======================================
//視点間の長さ処理
//=======================================
void CCamera::Length(void)
{
	//長さを範囲内に収める
	Range(&m_fLength, MIN_LENGTH, MAX_LENGTH);
}

//=======================================
//角度を計算する
//=======================================
void CCamera::CalculateRotate(void)
{
	//視点と注視点の位置の差
	D3DXVECTOR3 fDiffPos = m_posR - m_posV;

	//角度を計算する
	m_rot.x = +atan2f(fDiffPos.y, fDiffPos.z);
	m_rot.y = -atan2f(fDiffPos.x, fDiffPos.z);
}

//=======================================
//カメラの設定処理
//=======================================
void CCamera::Set(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(VIEW_ANGLE), SCREEN_WIDTH_F / SCREEN_HEIGHT_F, MIN_DRAW_Z, MAX_DRAW_Z);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックス
	D3DXMatrixIdentity(&m_mtxview);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxview, &m_posV, &m_posR, &m_vecU);

	//ビューマトリックス
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxview);
}

//=======================================
//カメラの逆角度を取得する
//=======================================
float CCamera::GetInverseRotate(void)
{
	return Inverse(m_rot.y);
}