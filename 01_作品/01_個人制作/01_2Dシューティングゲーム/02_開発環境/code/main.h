#ifndef _MAIN_H_						//このマクロ定義を一度もされていなかったら
#define _MAIN_H_

#include <Windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dInput.h"						//入力処理に必要
#include "xaudio2.h"
#include <stdio.h>

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

//マクロ定義
#define ORIGIN_POS			(0.0f)		//ウインドウのサイズの原点
#define MAX_POS				(1.0f)		//ウインドウのサイズの限界点
#define SCREEN_WIDTH		(1280)		//ウインドウの幅
#define SCREEN_HEIGHT		(720)		//ウインドウの高さ
#define HALF_WIDTH			(640.0f)	//ウインドウ幅半分
#define HALF_HEIGHT			(360.0f)	//ウインドウ高さ半分

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//頂点フォーマット
#define RHW_NUM				(1.0f)	//rhwに使用

#define COLOR_MAX	(255)	//RGBACOLORの最大数

#define START_CNT	(0)		//for文の初期設定に使用

/*
ZERO
ZERO
ONE
ONE
SEC
SEC
SAN
SAN
*/

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
	MODE_TITLE = 0,	//タイトル画面
	MODE_TUTORIAL,	//チュートリアル画面
	MODE_GAME,		//ゲーム画面
	MODE_RESULT,	//リザルト画面
	MODE_RANKING,	//ランキング画面
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

//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//プレイヤーの弾
	BULLETTYPE_ENEMY,		//敵の弾
	BULLETTYPE_BOSS,		//ボスの弾
	BULLETTYPE_MAX,
}BULLETTYPE;

//ホーミング性能の構造体
typedef enum
{
	HOMING_POSSIBLE = 0,	//ホーミング可能
	HOMING_IMPOSSIBLE,		//ホーミング不可能
	HOMING_MAX
}HOMINGTYPE;

//弾の発射間隔
typedef enum
{
	BULLETINTERVAL_NORMAL = 180,	//通常弾
	BULLETINTERVAL_SPEED  = 90,		//スピード弾
	BULLETINTERVAL_RUSH1  = 90,		//3連弾(初弾)
	BULLETINTERVAL_RUSH2  = 95,		//3連弾(次弾)
	BULLETINTERVAL_RUSH3  = 100,	//3連弾(終弾)
	BULLETINTERVAL_BEAM   = 220,	//ビーム弾
	BULLETINTERVAL_MAX
}BULLETINTERVAL;

//弾のスピン情報
typedef enum
{
	REFLECTBULLET_NO = 0,		//跳ね返されていない
	REFLECTBULLET_YES,			//跳ね返された
	REFLECTBULLET_MAX
}REFLECTBULLET;

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXCOLOR col;			//色
	int nLife;				//寿命
	BULLETTYPE type;		//弾の種類
	float EffectAlphaColor;	//弾の軌跡のα値を指定する変数
	HOMINGTYPE homingtype;	//ホーミング性能の有無
	REFLECTBULLET Reflect;	//跳ね返されたかどうか
	bool bUse;				//使用しているかどうか
}Bullet;

//ビーム構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXCOLOR col;			//色
	BULLETTYPE type;		//弾の種類
	REFLECTBULLET Reflect;	//跳ね返されたかどうか
	bool bUse;				//使用しているかどうか
}Beam;

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		//出現状態（点滅）
	PLAYERSTATE_NORMAL,			//通常状態
	PLAYERSTATE_SPIN,			//スピン状態
	PLAYERSTATE_DAMAGE,			//ダメージ状態
	PLAYERSTATE_WAIT,			//出現待ち状態（表示なし）
	PLAYERSTATE_DEATH,			//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//向き
	int nLife;					//体力
	PLAYERSTATE state;			//プレイヤーの状態
	int LimitSpin;				//スピンによる跳ね返しの上限
	int SpinInterval;			//次にスピンできるまでのインターバル
	int PlayerSwitch;			//点滅表示ON/OFF
	int AppearCnt;				//点滅表示時間のカウント
	int nCounterState;			//状態管理カウント
	float fLength;				//対角線の長さ
	float fAngle;				//対角線の角度
	BULLETTYPE myBullet;		//弾の種類
	HOMINGTYPE homingtype;		//ホーミング性能の有無
	bool bDisp;					//プレイヤーを表示ON/OFF
}Player;

//BOSSの状態
typedef enum
{
	BOSSSTATE_APPEAR = 0,	//出現状態（点滅）
	BOSSSTATE_NORMAL,		//通常状態
	BOSSSTATE_SPIN,			//スピン状態
	BOSSSTATE_DAMAGE,		//ダメージ状態
	BOSSSTATE_WAIT,			//出現待ち状態（表示なし）
	BOSSSTATE_DEATH,		//死亡状態
	BOSSSTATE_MAX
}BOSSSTATE;

//ボス構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//向き
	int nLife;					//体力
	BOSSSTATE state;			//プレイヤーの状態
	int SpinInterval;			//次にスピンできるまでのインターバル
	int BossAttackCnt;			//ボスの攻撃間隔
	int nCounterState;			//状態管理カウント
	float fLength;				//対角線の長さ
	float fAngle;				//対角線の角度
	BULLETTYPE BulletType;		//弾の種類
	HOMINGTYPE homingtype;		//ホーミング性能の有無
	bool bDisp;					//プレイヤーを表示ON/OFF
}BOSS;

//敵のダメージ状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//通常
	ENEMYSTATE_DAMAGE,			//ダメージ状態
	ENEMYSTATE_MAX,
}ENEMYSTATE;

//敵構造体
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	float Standardpos;						//降下してから停止するまでの場所
	D3DXVECTOR3 move;						//移動量
	float fLength;							//対角線の長さ
	float fAngle;							//対角線の角度
	int nType;								//種類
	int nLife;								//体力
	int nLifeMax;							//体力の最大値
	float Return;							//移動折り返し
	ENEMYSTATE state = ENEMYSTATE_NORMAL;	//状態
	int nCounterState;						//状態管理カウンター
	int firstInterval;						//出現から弾発射可能状態までのインターバル
	BULLETTYPE enBullet;					//弾の種類
	HOMINGTYPE homingtype;					//ホーミング性能の有無
	int enemyAtackCounter;					//敵の攻撃ターン
	bool bUse;								//使用の有無
}Enemy;

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