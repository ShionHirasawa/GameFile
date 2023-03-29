/*==========================================================================================================================================================

リザルト用カメラ処理[resultCamera.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _RESULTCAMERA_H_
#define _RESULTCAMERA_H_

#include "main.h"

//カメラ移動のWAVE
//*************************        WAVE説明        *************************
// WAVE１：プレイヤー１からプレイヤー４までカメラを横移動させてプレイヤーを見せる
//			４Ｐを見せ終わった後は	次のウェーブへ
//
// WAVE２：視点を手前に下がらせ、注視点を２Ｐ・３Ｐの間にスライドさせる
//			４Ｐの右手前から俯瞰で見る（注視点はプレイヤー２・３の間）
//			視点の位置設定が終わったら全プレイヤーを前方にジャンプさせる
//			ジャンプしてある程度落下したのを見届けたら	次のウェーブへ
//
// WAVE３：ジャンプしてから一定時間後に全プレイヤーが画面内に入るように追いかける
//			さらに一定時間後に全プレイヤーがヒップドロップをする
//
// WAVE LAST：地表までカメラを移動して表彰台とプレイヤーの着地が重なるように調整して、終了
//**************************************************************************
typedef enum
{
	WAVECamera_01_SideWays = 0,		//横移動
	WAVECamera_02_BirdEyes,			//俯瞰視点	（俯瞰視点を訳すと「bird's eys」というらしい...
	WAVECamera_03_SkyDiving,		//飛び降りの空撮
	WAVECamera_LAST_VictoryStand,	//表彰式
	WAVECamera_FINISH,				//全ウェーブ終了
	WAVECamera_MAX
}WAVECamera;

//リザルトカメラのアニメーション構造体
typedef struct
{
	int nFrameCounter;					//移動経過時間
	int nWholeFrame;					//全体フレーム
	bool bEnd;							//全体フレームに達したかどうか
	int nWaitCount;						//待機経過時間
	int WaitTime;						//目標位置に達した後の待ち時間
	D3DXVECTOR3 StartPosV, StartPosR;	//視点・注視点の開始位置
	D3DXVECTOR3 OffsetPosV, OffsetPosR;	//視点・注視点の目標位置
	D3DXVECTOR3 movePosV, movePosR;		//視点・注視点の移動量
}AnimResCamera;

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXVECTOR3 rot;			//向き
	float		fLength;		//視点と注視点の距離
	D3DXMATRIX	mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX	mtxview;		//ビューマトリックス
}ResultCamera;

void InitResultCamera(void);
void LoadWaveCamera(void);
void UninitResultCamera(void);
void UpdateResultCamera(void);

void WaveResultCamera(void);
void SpecialWave(void);
void SetNextWave(int nWave);

void SetResultCamera(void);
ResultCamera *GetResultCamera(void);
WAVECamera *GetWaveCamera(void);

#endif 