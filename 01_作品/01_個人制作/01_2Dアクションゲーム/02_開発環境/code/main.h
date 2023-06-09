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

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//頂点フォーマット
#define RHW_NUM				(1.0f)	//rhwに使用

#define COLOR_MIN	(0)		//RGBACOLORの最小数
#define COLOR_MAX	(255)	//RGBACOLORの最大数

#define START_CNT	(0)		//for文の初期設定に使用

//整数のマクロ
#define MINUS_ONE	(-1)	
#define ZERO		(0)
#define ONE			(1)
#define SEC			(2)
#define SAN			(3)
#define FOUR		(4)
#define FIVE		(5)

#define NUM_VERTEX	(4)		//ポインタや頂点座標を４つ進めるのに使用

//少数のマクロ
#define F_ZERO		(0.0f)
#define F_ONE		(0.1f)
#define F_SEC		(0.2f)
#define F_SAN		(0.3f)
#define F_FOUR		(0.4f)
#define F_FIVE		(0.5f)
#define F_SIX		(0.6f)
#define F_SEV		(0.7f)
#define F_ATE		(0.8f)
#define F_NINE		(0.9f)
#define F_TEN		(1.0f)

//POS.Zに使用
#define POS_Z		(0.0f)

//角度のマクロ
#define UP			(1.0f)
#define DOWN		(0.0f)
#define RIGHT		(0.5f)
#define LEFT		(-0.5f)
#define UP_RIGHT	(0.75f)
#define UP_LEFT		(-0.75f)
#define DOWN_RIGHT	(0.25f)
#define DOWN_LEFT	(-0.25f)

//画面モードの種類
typedef enum
{
	MODE_LOADING = ZERO,	//ローディング画面
	MODE_GAME,				//ゲーム画面
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

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif