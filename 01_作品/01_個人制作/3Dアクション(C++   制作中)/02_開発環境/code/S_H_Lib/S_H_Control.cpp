//================================================================================================
//
//【自作ライブラリ】計算処理[S_H_Control.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Model/motion.h"
#include "S_H_Calculation.h"
#include "S_H_Control.h"

//========================
//XZ平面の移動処理（パッドのスティック専用
//------------------------
//	引数１	NowRot		：移動させる者の現在の向き
//	引数２	fMoveSpeed	：基本の移動量
//	引数３	*pMove		：各ベクトルの移動量を格納するポインタ（なければ処理を行わない
//	引数４	*pMoveRot	：移動方向を格納するポインタ（無くても大丈夫
//	引数５	*pMotion	：モーション情報を格納するポインタ（無くても大丈夫
//	引数６	nMotionType	：モーションの種類（無くても大丈夫
//	返り値：メインのキーが押されたかどうか返す
//========================
bool Move_XZPlane(float NowRot, float fMoveSpeed, D3DXVECTOR3 *pMove, float *pMoveRot, CMotion *pMotion, int nMotionType)
{
	//移動量を設定するポインタが無いなら引き返す
	if (pMove == NULL) return false;

	//パッド取得
	CInputGamePad *pPad = CManager::GetGamePad();

	//スティックが倒れていない
	if (pPad->GetLStickX() == 0 && pPad->GetLStickY() == 0) return false;

	//倒れてたらこの先↓の処理を実行

	//移動方向を格納
	float fMoveRot = pPad->GetLStickRot() - NowRot;
	FixRotate(&fMoveRot);

	pMove->x -= sinf(fMoveRot) * fMoveSpeed;	//Ⅹの移動量を代入
	pMove->z -= cosf(fMoveRot) * fMoveSpeed;	//Ｚの移動量を代入

	if (pMoveRot != NULL) *pMoveRot = fMoveRot;	//移動方向を代入
	if (pMotion != NULL) pMotion->Set(nMotionType);//モーション設定可能なら設定する

	return true;//操作したかどうか返す
}

//========================
//XZ平面の移動処理（十字キー・十字パッド専用
//------------------------
//	引数１	MainKey		：移動させるメインのキー
//	引数２	MainMoveRot	：メインの移動方向
//	引数３	NowRot		：移動させる者の現在の向き
//	引数４	fMoveSpeed	：基本の移動量
//	引数５	*pMove		：各ベクトルの移動量を格納するポインタ（なければ処理を行わない
//	引数６	*pSub		：サブ情報が入った構造体のポインタ（無くても大丈夫
//	引数７	*pMoveRot	：移動方向を格納するポインタ（無くても大丈夫
//	引数８	*pMotion	：モーション情報を格納するポインタ（無くても大丈夫
//	引数８	*pMotionType：モーションの種類が格納されたポインタ（無くても大丈夫
//	返り値：メインのキーが押されたかどうか返す
//========================
bool Move_XZPlane(int MainKey, int MainPad, float MainMoveRot, float NowRot, float fMoveSpeed, D3DXVECTOR3 *pMove, XZPlane_SubInfo *pSub, float *pMoveRot, CMotion *pMotion, int nMotionType)
{
	//移動量を設定するポインタが無いなら引き返す
	if (pMove == NULL) return false;

	//メインのキー/パッドが押されなかった
	if (!CManager::IsInputPress(MainKey, MainPad)) return false;

	//押されたらこの先↓の処理を実行
	//移動方向を格納
	float fMoveRot = 0.0f;

	//各キーが持つ移動方向を代入する
	if		(pSub != NULL && CManager::IsInputPress(pSub->Key1, pSub->Pad1))	fMoveRot = pSub->MoveRot1 - NowRot;//サブ１方向に移動
	else if (pSub != NULL && CManager::IsInputPress(pSub->Key2, pSub->Pad2))	fMoveRot = pSub->MoveRot2 - NowRot;//サブ２方向に移動
	else	fMoveRot = MainMoveRot - NowRot;	//メイン方向に移動

	pMove->x -= sinf(fMoveRot) * fMoveSpeed;	//Ⅹの移動量を代入
	pMove->z -= cosf(fMoveRot) * fMoveSpeed;	//Ｚの移動量を代入

	if (pMoveRot != NULL) *pMoveRot = fMoveRot;	//移動方向を代入
	if (pMotion != NULL) pMotion->Set(nMotionType);//モーション設定可能なら設定する

	//操作したかどうか返す
	return true;
}

//========================
//現在の向きを進行方向に向かせる処理
//------------------------
//	引数１	*pCurRot：現在の角度
//	引数２	fDirRot ：進行方向
//	引数３	fRotSpeed：回転速度
//========================
void OrientToDir(float *pCurRot, float fDirRot, float fRotSpeed)
{
	//もし移動方向への差が半周を超えていたら
	if (fabsf(fDirRot - *pCurRot) > D3DX_PI)
	{
		//移動角度への向き方を、逆方向から向かせる
		fDirRot = *pCurRot - fDirRot;
	}
	else
	{
		//移動方向とプレイヤーの現在の角度の差を計算する
		fDirRot -= *pCurRot;
	}

	//プレイヤーの角度を移動方向へ向ける
	*pCurRot += fDirRot * fRotSpeed;

	//角度修正
	FixRotate(pCurRot);
}