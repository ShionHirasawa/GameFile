//================================================================================================
//
//カメラ処理[camera.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//前方宣言
class CPlayer;

//カメラクラス
class CCamera
{
public:
	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void Set(void);//カメラの角度などを設定	
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }	//プレイヤー情報を取得

	D3DXVECTOR3 GetPosV(void) { return m_posV;  }	//視点の位置を取得
	 D3DXVECTOR3 GetRotate(void) { return m_rot; }	//カメラの角度を取得する
	 float GetInverseRotate(void);					//カメラの逆角度を取得する
	 void CalculateRotate(void);					//カメラの角度を計算する

private:
	static const float MOUSE_ROT_SPEED;
	static const float PAD_ROT_SPEED;

	void Move(void);
	void MovePosV(CInputMouse *pMouse);
	void SpinPosV(CInputMouse *pMouse);
	void MovePosR(CInputMouse *pMouse);
	void ResetPos(void);
	void Length(void);

	CPlayer *m_pPlayer;	//プレイヤー情報を格納

	D3DXMATRIX	m_mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX	m_mtxview;		//ビューマトリックス
	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_posROffset;	//注視点のオフセット
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXVECTOR3 m_rot;			//向き
	float		m_fLength;		//視点と注視点の距離
};

#endif 