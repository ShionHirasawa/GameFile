/*==========================================================================================================================================================

															メイン処理[main.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _MAIN_H_						//このマクロ定義を一度もされていなかったら
#define _MAIN_H_

#include <Windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dInput.h"						//入力処理に必要
#include "XInput.h"
#include "xaudio2.h"
#include <stdio.h>

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//マクロ定義
#define ORIGIN_POS			(0.0f)		//ウインドウのサイズの原点
#define MAX_POS				(1.0f)		//ウインドウのサイズの限界点
#define SCREEN_WIDTH		(1280)		//ウインドウの幅
#define SCREEN_HEIGHT		(720)		//ウインドウの高さ
#define HALF_WIDTH			(640.0f)	//ウインドウ幅半分
#define HALF_HEIGHT			(360.0f)	//ウインドウ高さ半分
#define ZERO_SET			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//リセット用

//float型の座標マクロ
#define NIL_F				(0.0f)		//0.0f座標
#define HAL_F				(0.5f)		//0.5f座標
#define MAX_F				(1.0f)		//1.0f座標

//テクスチャ座標のマクロ
#define ORIGIN_TEX			D3DXVECTOR2(NIL_F, NIL_F)	//	0			0
#define LIMIT_TEX			D3DXVECTOR2(MAX_F, MAX_F)	//	1			1

#define HALF_TEX			D3DXVECTOR2(HAL_F, HAL_F)	//	0.5f		0.5f
#define ZE_HA_TEX			D3DXVECTOR2(NIL_F, HAL_F)	//	0			0.5f
#define HA_ZE_TEX			D3DXVECTOR2(HAL_F, NIL_F)	//	0.5f		0

#define ZE_MA_TEX			D3DXVECTOR2(NIL_F, MAX_F)	//	0			1.0f
#define MA_ZE_TEX			D3DXVECTOR2(MAX_F, NIL_F)	//	1.0f		0

#define HA_MA_TEX			D3DXVECTOR2(HAL_F, MAX_F)	//	0.5f		1
#define MA_HA_TEX			D3DXVECTOR2(MAX_F, HAL_F)	//	1			0.5f

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				//2D用頂点フォーマット
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//3D用頂点フォーマット
#define RHW_NUM				(1.0f)														//rhwに使用
#define SET_NOR				D3DXVECTOR3(0.0f, 1.0f, 0.0f)								//norに使用

#define START_CNT	(0)		//for文の初期設定に使用

/*=========================================================
						整数のマクロ
===========================================================*/
#define MINUS_ONE	(-1)	
#define ZERO		(0)
#define ONE			(1)
#define SEC			(2)
#define SAN			(3)
#define FOUR		(4)
#define FIVE		(5)

//整数のマクロ別パターン
#define SINGLE		(1)
#define DOUBLE		(2)
#define TRIPLE		(3)
#define QUATRO		(4)

#define MAX_MOJI	(512)	//読み込む文字の最大数

//回数のマクロ
#define ADD			(1)
#define ONCE		(1)
#define TWICE		(2)

/*=========================================================
						少数のマクロ
===========================================================*/
#define F_ZERO			(0.0f)
#define F_ONE			(0.1f)
#define F_SEC			(0.2f)
#define F_SAN			(0.3f)
#define F_FOUR			(0.4f)
#define F_FIVE			(0.5f)
#define F_SIX			(0.6f)
#define F_SEV			(0.7f)
#define F_ATE			(0.8f)
#define F_NINE			(0.9f)
#define F_TEN			(1.0f)

//少数のマクロ別パターン
#define F_SINGLE		(1.0f)
#define F_DOUBLE		(2.0f)
#define F_TRIPLE		(3.0f)
#define F_QUATRO		(4.0f)

//角度のマクロ
#define UP				(D3DX_PI * +1.0f)
#define DOWN			(D3DX_PI * 00.0f)
#define RIGHT			(D3DX_PI * 00.5f)
#define LEFT			(D3DX_PI * -0.5f)
#define FIX_ROT			(D3DX_PI * +2.0f)
#define LEFT_UP			(D3DX_PI * -0.75f)
#define RIGHT_UP		(D3DX_PI * +0.75f)
#define LEFT_DOWN		(D3DX_PI * -0.25f)
#define RIGHT_DOWN		(D3DX_PI * +0.25f)

//倍率のマクロ
#define HALF_MAG		(0.5f)		//半分
#define NORMAL_MAG		(1.0f)		//等倍
#define TWICE_MAG		(2.0f)		//２倍

#define FIX_FLOAT		(100.0f)	//角度をfloat型に修正

//画面モードの種類
typedef enum
{
	MODE_TITLE = ZERO,	//タイトル画面
	MODE_TUTORIAL,		//チュートリアル画面
	MODE_GAME,			//ゲーム画面
	MODE_RESULT,		//リザルト画面
	MODE_MAX
}MODE;

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換用係数(1.0fで固定)
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル　nor = Normal
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ
}VERTEX_3D;

//プレイモード構造体
typedef enum
{
	PLAYMODE_MODEL = ZERO,	//モデル操作可能
	PLAYMODE_FIELD,			//地面の操作可能
	PLAYMODE_WALL,			//壁の操作可能
	PLAYMODE_STAGE,			//地面と壁の操作可能
	PLAYMODE_CAMERA,		//カメラ操作可能
	PLAYMODE_MAX
}PLAYMODE;

//4つの頂点座標の列挙型
typedef enum
{
	VTX_LE_UP = ZERO,	//左上
	VTX_RI_UP,			//右上
	VTX_LE_DO,			//左下
	VTX_RI_DO,			//右下
	VTX_MAX
}VTX;

//FPSの種類
typedef enum
{
	FPS_30 = 30,
	FPS_60 = 60,
	FPS_120 = 120,
	FPS_MAX = 3,
}FPS_TIME;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(PLAYMODE mode);
void DrawFPSPlayer(void);
void DrawFPSField(void);
void DrawFPSWall(void);
void DrawFPSStage(void);
void DrawFPSCamera(void);
void DrawFPSALL(void);
void DrawFPSSecond(PLAYMODE mode);
void SetMode(MODE mode);
MODE GetMode(void);
void VectorProduct(float ParentX, float ChildZ, float ParentZ, float ChildX, float *pAnswer, float fMagnification);	// 外積計算
void InnerProduct(float Rot, D3DXVECTOR3 vecMove, D3DXVECTOR3 *pMove);												// 内積計算

#endif