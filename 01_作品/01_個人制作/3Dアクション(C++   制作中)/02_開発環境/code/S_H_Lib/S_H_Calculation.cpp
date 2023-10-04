//================================================================================================
//
//【自作ライブラリ】計算処理[S_H_Calculation.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include"../Main/main.h"
#include "S_H_Calculation.h"

//========================
//float型の範囲計算
//関数の説明などはヘッダファイルへ
//========================
float Range(float *pValue, float fAbs)
{
	return Range(pValue, -fAbs, fAbs);
}

//========================
//float型の範囲計算
//関数の説明などはヘッダファイルへ
//========================
float Range(float *pValue, float fMin, float fMax)
{
	//もともと範囲に収まっていればそのまま返す
	if(fMin <= *pValue && *pValue <= fMax)	return *pValue;

	//最小値を下回っているか
	else if (*pValue < fMin) return *pValue = fMin;

	//最大値を上回っている
	return *pValue = fMax;
}

//========================
//D3DXVECTOR3型の範囲計算
//関数の説明などはヘッダファイルへ
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, float fAbs)
{
	//各ベクトルを範囲に収める
	Range(&pValue->x, -fAbs, fAbs);
	Range(&pValue->y, -fAbs, fAbs);
	Range(&pValue->z, -fAbs, fAbs);

	//修正結果を返す
	return *pValue;
}

//========================
//D3DXVECTOR3型の範囲計算
//関数の説明などはヘッダファイルへ
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, float fMin, float fMax)
{
	//各ベクトルを範囲に収める
	pValue->x = Range(&pValue->x, fMin, fMax);
	pValue->y = Range(&pValue->y, fMin, fMax);
	pValue->z = Range(&pValue->z, fMin, fMax);

	//修正結果を返す
	return *pValue;
}

//========================
//float型の範囲計算
//関数の説明などはヘッダファイルへ
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, D3DXVECTOR3 Abs)
{
	//各ベクトルを範囲に収める
	Range(&pValue->x, -Abs.x, Abs.x);
	Range(&pValue->y, -Abs.y, Abs.y);
	Range(&pValue->z, -Abs.z, Abs.z);

	//修正結果を返す
	return *pValue;
}

//========================
//D3DXVECTOR3型の範囲計算
//関数の説明などはヘッダファイルへ
//========================
D3DXVECTOR3 Range(D3DXVECTOR3 *pValue, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	//各ベクトルを範囲に収める
	pValue->x = Range(&pValue->x, Min.x, Max.x);
	pValue->y = Range(&pValue->y, Min.y, Max.y);
	pValue->z = Range(&pValue->z, Min.z, Max.z);

	//修正結果を返す
	return *pValue;
}

//========================
//逆の角度を求める
//関数の説明などはヘッダファイルへ
//========================
float Inverse(float rot)
{
	//半円分の角度を足して、１周分の角度で割ったあまりを代入
	rot = fmodf(rot + D3DX_PI, FIX_ROT);

	//角度修正したものを返す
	return FixRotate(&rot);
}

//========================
//角度修正
//関数の説明などはヘッダファイルへ
//========================
float FixRotate(float *pRot)
{
	//角度修正
	return *pRot = fmodf(*pRot + D3DX_PI * 3, D3DX_PI * 2) - D3DX_PI;
}

//========================
//角度修正
//関数の説明などはヘッダファイルへ
//========================
D3DXVECTOR3 FixRotate(D3DXVECTOR3 *pRot)
{
	FixRotate(&pRot->x);//Ⅹ軸の回転値修正
	FixRotate(&pRot->y);//Ｙ軸の回転値修正
	FixRotate(&pRot->z);//Ｚ軸の回転値修正

	return *pRot;		//角度を返す
}

//========================
//ワールド座標を取得
//関数の説明などはヘッダファイルへ
//========================
D3DXVECTOR3 GetWorldPosition(float Rot, D3DXVECTOR3 size, D3DXVECTOR3 Pos)
{
	D3DXVECTOR3 pos = VEC3_INIT;

	//各座標のワールド座標を計算
	pos.x = roundf(sinf(Rot) * size.x + Pos.x);
	pos.z = roundf(cosf(Rot) * size.x + Pos.z);

	//座標を返す
	return pos;
}

//========================
//一番近い角度を返す
//関数の説明などはヘッダファイルへ
//========================
float NearRotate(float Rot, float fFrontLength, D3DXVECTOR3 Pos, D3DXVECTOR3 RangeLeft, D3DXVECTOR3 RangeRight)
{
	//左端の正面位置
	D3DXVECTOR3 vecFront;
	vecFront.x = sinf(Rot) * -fFrontLength + RangeLeft.x;
	vecFront.z = cosf(Rot) * -fFrontLength + RangeLeft.z;

	//左端から、左端の正面までのベクトル
	D3DXVECTOR3 vecFrontLine = vecFront - RangeLeft;

	//左端から位置までのベクトル
	D3DXVECTOR3 vecPos = Pos - RangeLeft;

	//現在位置までの向きが、境界線ベクトルの左側に居なければ処理を飛ばす
	if (vecFrontLine.x * vecPos.z - vecFrontLine.z * vecPos.x < 0.0f)
	{
		//角度計算した結果を代入　まずは左側への角度を計算
		return atan2f(Pos - RangeLeft);
	}

	//右端の正面位置
	vecFront.x = sinf(Rot) * -fFrontLength + RangeRight.x;
	vecFront.z = cosf(Rot) * -fFrontLength + RangeRight.z;

	//右端から、右端の正面までのベクトル
	vecFrontLine = vecFront - RangeRight;

	//右端から位置までのベクトル
	vecPos = Pos - RangeRight;

	//範囲の右側より右にいる場合、右側への角度を一番近い角度とする
	if (vecFrontLine.x * vecPos.z - vecFrontLine.z * vecPos.x > 0.0f)
	{
		//角度計算した結果を代入　まずは左側への角度を計算
		return atan2f(Pos - RangeRight);
	}

	//左右の内側にいるので逆角度を一番近い角度とする
	return Inverse(Rot);
}

//========================
//交点を求める処理
//関数の説明などはヘッダファイルへ
//========================
D3DXVECTOR3 CrossPoint(D3DXVECTOR3 pos, D3DXVECTOR3 VecMove, D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	/*境界線ベクトル*/ const D3DXVECTOR3 VecLine = EndPos - StartPos;
	/*対象位置から開始座標までのベクトル*/ const D3DXVECTOR3 VecPosStart = StartPos - pos;
	/*終端座標から対象位置までのベクトル*/ const D3DXVECTOR3 VecPosEnd = pos - EndPos;

	//移動ベクトルを境にした、終始の頂点までの三角形の面積を計算する
	const float fAReaStart = ((VecPosStart.x * VecMove.z) - (VecPosStart.z * VecMove.x)) * HALF;
	const float fAReaEnd = ((VecPosEnd.x * VecMove.z) - (VecPosEnd.z * VecMove.x)) * HALF;

	//２つの三角形の合計値を求める
	const float fAreaAll = fAReaStart + fAReaEnd;

	//割り算計算に０で割るといけないので、位置を返す
	if (fAreaAll == 0.0f)	return pos;

	//開始座標側の三角形の面積の割合を計算する
	const float fRateStart = fAReaStart / fAreaAll;

	D3DXVECTOR3 CrossPos  = VEC3_INIT;

	/*交点Ⅹ・Ｚ座標を算出する*/
	CrossPos.x = StartPos.x + (VecLine.x * fRateStart);
	CrossPos.z = StartPos.z + (VecLine.z * fRateStart);

	//交点座標を返す
	return CrossPos;
}

//========================
//D3DXVECTOR3型の角度計算
//関数の説明などはヘッダファイルへ
//========================
float atan2f(D3DXVECTOR3 v)
{
	//角度を計算して値を返す
	return atan2f(v.x, v.z);
}

//========================
//D3DXVECTOR3型の各ベクトルの絶対値を返す
//関数の説明などはヘッダファイルへ
//========================
D3DXVECTOR3 fabsV3(D3DXVECTOR3 v)
{
	//各ベクトルの絶対値を取得して返す
	return D3DXVECTOR3(fabsf(v.x),
					   fabsf(v.y),
					   fabsf(v.z));
}

//========================
//一定の範囲内に対象物がいるかどうか捜索する
//関数の説明などはヘッダファイルへ
//========================
bool Search(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos, float fLeftAngle, float fRightAngle, float fLength)
{
	//左端の位置（以下変数名をLRPとする
	const D3DXVECTOR3 LeftRangePos = D3DXVECTOR3(sinf(fLeftAngle) * fLength + pos.x,	pos.y,
												 cosf(fLeftAngle) * fLength + pos.z);

	//右端の位置（以下変数名をRRPとする
	const D3DXVECTOR3 RightRangePos = D3DXVECTOR3(sinf(fRightAngle) * fLength + pos.x,	pos.y,
												  cosf(fRightAngle) * fLength + pos.z);

	//左右の範囲の限界座標までのベクトル
	const D3DXVECTOR3 VecLRP = LeftRangePos - pos;
	const D3DXVECTOR3 VecRRP = RightRangePos - pos;

	//目標までのベクトルと距離
	D3DXVECTOR3 VecTarget = TargetPos - pos;
	const float VecTargetLength = D3DXVec3Length(&VecTarget);

	//目標までの距離が範囲外なら処理を飛ばす
	if (fLength < VecTargetLength)	return false;

	//範囲内にいるかどうか判定
	if (VecLRP.x * VecTarget.z - VecLRP.z * VecTarget.x <= 0.0f &&	//範囲の中に居る
		VecRRP.x * VecTarget.z - VecRRP.z * VecTarget.x >= 0.0f)	return true;

	//範囲の外に居る
	return false;
}

//========================
//桁数を調べる
//関数の説明などはヘッダファイルへ
//========================
int Dejit(int nData)
{
	//桁数を保存
	int nDejit = 0;

	do
	{
		//１桁下げて桁数増加
		nData /= 10;
		nDejit++;
	} while (nData > 0);

	//桁数を返す
	return nDejit;
}

//========================
//対象物の中にめり込んでいるかどうか判定
//関数の説明などはヘッダファイルへ
//========================
bool IsBoxCollision(D3DXVECTOR3 MinPos, D3DXVECTOR3 MaxPos, D3DXVECTOR3 TargetMinPos, D3DXVECTOR3 TargetMaxPos)
{
	//めり込んでいるか判定
	if (MinPos.x < TargetMaxPos.x && TargetMinPos.x < MaxPos.x &&
		MinPos.z < TargetMaxPos.z && TargetMinPos.z < MaxPos.z)return true;

	else return false;
}

//=========================
//メッシュの種類別頂点数とインデックス数を取得
//=========================
CMesh::Buffer GetMeshBuffer(int nColumn, int nRow)
{
	//メッシュの各総数を保存
	CMesh::Buffer  Buffer = { 0, 0, 0 };

	//重なる頂点の数
	const int nMultiTop = (nColumn - 2) + (nRow - 2);

	//１行のポリゴン数
	const int nColumnPoly = (nColumn - 1) * 2;

	//頂点数（列　×　行
	Buffer.nNumVtx = nColumn * nRow;

	//ポリゴン数（１行のポリゴン　×　（列-1）　＋　重なる頂点の数　×　１つの頂点に重なる数
	Buffer.nNumPoly = nColumnPoly * (nRow - 1) + nMultiTop * 2;

	//インデックス数（１行のポリゴン　×　列　＋　//重なる頂点の数
	Buffer.nNumIdx = nColumnPoly * nRow + nMultiTop;

	//バッファーを返す
	return Buffer;
}

//=========================
//シリンダー型のメッシュバッファーを取得
//=========================
CMesh::Buffer GetMeshBuffer(int nDevide)
{
	//メッシュの各総数を保存
	CMesh::Buffer  Buffer = { 0, 0, 0 };

	//バッファー計算（全て同じ？
	Buffer.nNumIdx = Buffer.nNumPoly = Buffer.nNumVtx = (nDevide + 1) * 2;

	//バッファーを返す
	return Buffer;
}

//=========================
//ランダムで角度を取得
//=========================
float RandomRotate(void)
{
	//角度を返す
	return (float)((rand() % FULL_ANGLE - FIX_ANGLE) * FIX_FLOAT);
}