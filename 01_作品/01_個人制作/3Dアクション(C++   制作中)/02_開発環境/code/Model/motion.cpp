//================================================================================================
//
//モーション管理処理[motion.h]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "motion.h"

//========================
//マクロ定義
//========================
//ブレンドに要する時間
#define BLEND_TIME	(30)

//========================
//コンストラクタ
//========================
CMotion::CMotion()
{
	//メンバ変数クリア
	//各モーション情報クリア
	for (int nCntInfo = 0; nCntInfo < MOTIONTYPE_MAX; nCntInfo++)
	{
		/*モーション情報クリア*/InitMotionInfo(&m_aInfo[nCntInfo]);
		/*ループ情報クリア*/m_aInfo[nCntInfo].bLoop = false;
		/*キーの総数クリア*/m_aInfo[nCntInfo].nNumKey = 0;
	}

	m_nNumAll = 0;
	m_nType = 0;
	m_bLoop = false;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nCounter = 0;
	m_bFinish = false;	//終了したかどうか
	m_nBlendType = 0;
	m_bBlend = false;
	m_bAction = false;

	m_ppModel = NULL;	//モデルへのポインタ
	m_nNumModel = 0;	//モデルの総数

	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		m_OffSetPos[nCntParts] = VEC3_INIT;	//初期位置
		m_OffSetRot[nCntParts] = VEC3_INIT;	//初期角度

		m_TargetPos[nCntParts] = m_MovePos[nCntParts] = VEC3_INIT;//パーツの現在位置・目標位置・移動量をクリア
		m_TargetRot[nCntParts] = m_MoveRot[nCntParts] = VEC3_INIT;//パーツの現在方向・目標方向・移動量をクリア
	}
}

//========================
//デストラクタ
//========================
CMotion::~CMotion()
{

}

//========================
//更新処理
//========================
void CMotion::Update(void)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		//現在位置を更新する
		m_Pos[nCntParts] += m_MovePos[nCntParts];
		m_Rot[nCntParts] += m_MoveRot[nCntParts];

		//位置・回転値を設定
		m_ppModel[nCntParts]->OffSetTranslate(m_Pos[nCntParts], m_Rot[nCntParts]);
	}

	//モーションカウンター増加
	m_nCounter++;

	//モーションカウンターが分割したキーの最大フレームに達した
	if (m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame <= m_nCounter)
	{
		//ブレンド中だった
		if (m_bBlend)
		{
			m_bBlend = false;		//ブレンド終了
			m_nType = m_nBlendType;	//種類を設定する
			m_nBlendType = 0;		//ブレンドする種類初期化
			m_nKey = -1;
		}

		StepKey();				//次のキーへ
	}

	//アクションモーションのキー番号１になったときに、攻撃アクション可能
	if (m_nType == MOTIONTYPE_ACTION && m_nKey == 1 && m_nCounter % 9 == 0)
	{
		m_bAction = true;
	}
	//条件を満たせないときは、アクションが出来なくする
	else
	{
		m_bAction = false;
	}
}

//========================
//次のモーションキーへ
//========================
void CMotion::StepKey(void)
{
	m_nCounter = 0;	//カウンター初期化
	m_nKey++;		//次の分割数へ

					//全体分割数に達した
	if (m_aInfo[m_nType].nNumKey <= m_nKey)
	{
		/*キーを最初に戻す*/	m_nKey = 0;
		/*モーション再生終了*/	m_bFinish = true;

		//モーションがループしない場合
		if (!m_aInfo[m_nType].bLoop)
		{
			//待機モーションへ
			m_nType = MOTIONTYPE_NEUTRAL;
		}
	}

	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		//各パーツの目標値を設定する
		m_TargetPos[nCntParts].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosX;
		m_TargetPos[nCntParts].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosY;
		m_TargetPos[nCntParts].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosZ;
		m_TargetRot[nCntParts].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotX;
		m_TargetRot[nCntParts].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotY;
		m_TargetRot[nCntParts].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotZ;
	}

	//移動量を計算する
	SetMoveParts();
}

//========================
//モーションの種類を設定
//========================
void CMotion::Set(int nType) 
{
	if (m_nType != nType || //現在のモーションの種類と、設定したいモーションの種類が違う
		(m_nType == nType && m_bBlend))	//現在のモーションと同じでも、モーションブレンド中である
	{
		m_nKey = 0;			//キー番号初期化
		m_nCounter = 0;		//モーションカウンター初期化
		m_nType = nType;	//種類を設定
		m_nBlendType = nType;//ブレンドしたい種類も同じものを設定
		m_bBlend = false;	//ブレンド状態解除
		m_bFinish = false;	//終了解除

		for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
		{
			//各パーツの目標値を設定する
			m_TargetPos[nCntParts].x = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fPosX;
			m_TargetPos[nCntParts].y = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fPosY;
			m_TargetPos[nCntParts].z = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fPosZ;
			m_TargetRot[nCntParts].x = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fRotX;
			m_TargetRot[nCntParts].y = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fRotY;
			m_TargetRot[nCntParts].z = m_aInfo[nType].aKeyInfo[0].aKey[nCntParts].fRotZ;
		}

		//各パーツの移動量を計算する
		SetMoveParts();
	}
}

//========================
//モーションの種類を設定
//========================
void CMotion::SetPos(void)
{
	//各パーツの現在の位置を設定する
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		m_Pos[nCntParts].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosX;
		m_Pos[nCntParts].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosY;
		m_Pos[nCntParts].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fPosZ;
		m_Rot[nCntParts].x = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotX;
		m_Rot[nCntParts].y = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotY;
		m_Rot[nCntParts].z = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntParts].fRotZ;
	}

	//移動量を設定する
	SetMoveParts();
}

//========================
//モーションの情報を設定する
//========================
void CMotion::SetInfo(MOTION_INFO info)
{
	//モーション情報設定
	m_aInfo[m_nType] = info;
}

//========================
//モーションをするモデルを設定する
//========================
void CMotion::SetModel(CModel **ppModel, int nNumModel)
{
	m_ppModel = ppModel;	//モデルの先頭アドレスを記憶する
	m_nNumModel = nNumModel;//モデルの総数を記憶する
}

//========================
//パーツの初期位置・角度設定
//========================
void CMotion::SetOffset(D3DXVECTOR3 offSet, D3DXVECTOR3 offRot, int nCntSet)
{
	m_OffSetPos[nCntSet] = offSet;	//初期位置設定
	m_OffSetRot[nCntSet] = offRot;	//初期角度設定
}

//========================
//モーションのブレンド設定
//========================
void CMotion::SetBlend(int nType)
{
	//現在のモーションの種類とブレンドしたいモーションが同じなら、ブレンドしない
	if (m_nType == nType)
	{
		return;
	}

	//ブレンドする種類を設定
	m_nBlendType = nType;
	m_bBlend = true;
}

//========================
//移動量を設定する
//========================
void CMotion::SetMoveParts(void)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		D3DXVECTOR3 *pMovePos = &m_MovePos[nCntParts];
		D3DXVECTOR3 *pMoveRot = &m_MoveRot[nCntParts];

		//各パーツごとの座標・向きの移動量を次のモーションのフレーム数を基に計算する
		m_MovePos[nCntParts] = (m_TargetPos[nCntParts] - m_Pos[nCntParts]) / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame;
		m_MoveRot[nCntParts] = (m_TargetRot[nCntParts] - m_Rot[nCntParts]) / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame;
	}
}

//========================
//パーツ情報リセット
//========================
void CMotion::ResetParts(KEY *pKEY)
{
	pKEY->fPosX = 0.0f;
	pKEY->fPosY = 0.0f;
	pKEY->fPosZ = 0.0f;
	pKEY->fRotX = 0.0f;
	pKEY->fRotY = 0.0f;
	pKEY->fRotZ = 0.0f;
}

//========================
//モーション情報初期化
//========================
void CMotion::InitMotionInfo(MOTION_INFO *pInfo)
{
	//もし引数のポインタがNULLだったら
	if (pInfo == NULL)
	{
		//処理をせず引き返す
		return;
	}

	//キー情報クリア
	for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
	{
		//各キーの座標・回転軸をリセット
		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			ResetParts(&pInfo->aKeyInfo[nCntKey].aKey[nCntParts]);
		}

		//フレームをクリア
		pInfo->aKeyInfo[nCntKey].nFrame = 0;
	}
}