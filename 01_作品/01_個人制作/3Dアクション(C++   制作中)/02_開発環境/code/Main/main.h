//================================================================================================
//
//メイン処理[main.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MAIN_H_	//このマクロ定義を一度もされていなかったら
#define _MAIN_H_

#include <Windows.h>
#include <d3d9.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dInput.h"						//入力処理に必要
#include "XInput.h"
#include "xaudio2.h"
#include <stdio.h>
#include <time.h>

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//マクロ定義
//****************************************************************
//ウィンドウ系
//****************************************************************
#define CLASS_NAME	"WindowClass"	//ウインドウクラスの名前
#define ORIGIN_POS		(0.0f)		//ウインドウのサイズの原点
#define MAX_POS			(1.0f)		//ウインドウのサイズの限界点
#define SCREEN_WIDTH	(1280)		//ウインドウの幅
#define SCREEN_HEIGHT	(720)		//ウインドウの高さ
#define HALF_WIDTH		(640.0f)	//ウインドウ幅半分
#define HALF_HEIGHT		(360.0f)	//ウインドウ高さ半分
#define SCREEN_WIDTH_F	((float)SCREEN_WIDTH)		//ウインドウの幅
#define SCREEN_HEIGHT_F	((float)SCREEN_HEIGHT)		//ウインドウの高さ
#define SCREEN_CENTER_POS	(D3DXVECTOR3(HALF_WIDTH, HALF_HEIGHT, 0.0f))

//****************************************************************
//VECTOR系
//****************************************************************
#define VEC2_INIT	(D3DXVECTOR2(0.0f, 0.0f))		//初期化用
#define VEC3_INIT	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//初期化用
#define NOR_INIT	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	//norに使用

//****************************************************************
//float系
//****************************************************************
#define NIL_F		(0.0f)
#define HALF		(0.5f)
#define TWICE		(2.0f)

#define FULL_ANGLE	(628)	//全体の角度
#define FIX_ANGLE	(314)	//角度を修正する
#define FIX_FLOAT	(0.01f)	//角度をfloat型に修正

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//角度系
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PI_UP			(D3DX_PI * +1.0f)	/*  上  */
#define PI_DOWN			(D3DX_PI * 00.0f)	/*  下  */
#define PI_RIGHT		(D3DX_PI * 00.5f)	/*  右  */
#define PI_LEFT			(D3DX_PI * -0.5f)	/*  左  */
#define PI_LEFT_UP		(D3DX_PI * -0.75f)	/* 左上 */
#define PI_RIGHT_UP		(D3DX_PI * +0.75f)	/* 右上 */
#define PI_LEFT_DOWN	(D3DX_PI * -0.25f)	/* 左下 */
#define PI_RIGHT_DOWN	(D3DX_PI * +0.25f)	/* 右下 */
#define FIX_ROT			(D3DX_PI * 2.0f)	/*角度修正*/
#define HALF_PI			(D3DX_PI * 0.5f)	/*半分の角度*/

/***	最大FPS	***/
#define MAX_FPS	(60)

/***	2D用頂点フォーマット	***/
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
/***	3D用頂点フォーマット	***/
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
/***	rhwに使用	***/
#define RHW		(1.0f)

/***	読み込む文字の最大数	***/
#define MAX_STR	(512)
/***	CSVの区切り文字	***/
#define CSV_DELIMITER	","
/***	頂点数	***/
#define	VTX_MAX	(4)
/***	エラーID	***/
#define ERROR_ID	(-1)
//通常のatofだとdouble型で返してくるのでfloatキャストも行うようにしたもの
#define fatof(x)			(float)atof(x)

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float		rhw;	//座標変換用係数(1.0fで固定)
	D3DCOLOR	col;	//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル　nor = Normal
	D3DCOLOR	col;	//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ
}VERTEX_3D;

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


#endif