/*==========================================================================================================================================================

カメラ分割による境目の枠表示処理[camera_frame.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _CAMERAFRAME_H_
#define _CAMERAFRAME_H_

#include "camera.h"
#include "HDR_camera.h"

//使用する枠線の列挙型
typedef enum
{
	UseFrame_WIDTH = 0,	//横線
	UseFrame_HEIGHT,	//縦線
	UseFrame_HEIGHT4_1,	//縦線4分の1番目
	UseFrame_HEIGHT4_3,	//縦線4分の3番目
	UseFrame_MAX,
}UseFrame;

//プロトタイプ宣言
void InitCameraFrame(void);
void UninitCameraFrame(void);
void DrawCameraFrame(void);
void SetUseFrame(NumCamera type);

#endif