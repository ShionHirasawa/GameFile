/*==========================================================================================================================================================

カメラが使われていなかったときのカーテン処理[curtain.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _CURTAIN_H_
#define _CURTAIN_H_

//プロトタイプ宣言
void InitSelModePvPCurtain(void);	//ゲームモードがPVPの場合のカーテンの設定処理
void InitSelModeHDRCurtain(void);	//ゲームモードがHDRの場合のカーテンの設定処理
void UninitCurtain(void);
void UpdateCurtain(void);
void DrawCurtain(int nCntCurtain);

#endif