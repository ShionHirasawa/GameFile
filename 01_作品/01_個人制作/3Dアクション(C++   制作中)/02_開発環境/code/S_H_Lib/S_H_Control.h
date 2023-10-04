//================================================================================================
//
//【自作ライブラリ】計算処理[S_H_Control.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _S_H_CONTROL_H_
#define _S_H_CONTROL_H_

class CMotion;

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
bool Move_XZPlane(float NowRot, float fMoveSpeed, D3DXVECTOR3 *pMove, float *pMoveRot = NULL, CMotion *pMotion = NULL, int nMotionType = ERROR_ID);

//XZ平面の移動処理でサブで移動させる情報
struct XZPlane_SubInfo
{
	int		Key1;		//メインに対して、ベクトルをもう一つ足すサブキー１
	int		Key2;		//メインに対して、ベクトルをもう一つ足すサブキー２
	int		Pad1;		//メインに対して、ベクトルをもう一つ足すサブパッド１
	int		Pad2;		//メインに対して、ベクトルをもう一つ足すサブパッド２
	float	MoveRot1;	//サブ１の移動方向
	float	MoveRot2;	//サブ２の移動方向
};

//========================
//XZ平面の移動処理（十字キー・十字パッド専用
//------------------------
//	引数１	MainKey		：移動させるメインのキー
//	引数２	MainPad		：移動させるメインのパッド
//	引数３	MainMoveRot	：メインの移動方向
//	引数４	NowRot		：移動させる者の現在の向き
//	引数５	fMoveSpeed	：基本の移動量
//	引数６	*pMove		：各ベクトルの移動量を格納するポインタ（なければ処理を行わない
//	引数７	*pSub		：サブ情報が入った構造体のポインタ（無くても大丈夫
//	引数８	*pMoveRot	：移動方向を格納するポインタ（無くても大丈夫
//	引数９	*pMotion	：モーション情報を格納するポインタ（無くても大丈夫
//	引数10	nMotionType	：モーションの種類（無くても大丈夫
//	返り値：メインのキーが押されたかどうか返す
//========================
bool Move_XZPlane(int MainKey, int MainPad, float MainMoveRot, float NowRot, float fMoveSpeed, D3DXVECTOR3 *pMove, XZPlane_SubInfo *pSub = NULL, float *pMoveRot = NULL, CMotion *pMotion = NULL, int nMotionType = ERROR_ID);

//========================
//現在の向きを進行方向に向かせる処理
//------------------------
//	引数１	*pCurRot：現在の角度
//	引数２	fDirRot ：進行方向
//	引数３	fRotSpeed：回転速度
//========================
void OrientToDir(float *pCurRot, float fDirRot, float fRotSpeed);

#endif