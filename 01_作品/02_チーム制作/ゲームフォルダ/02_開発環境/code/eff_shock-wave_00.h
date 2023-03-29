//========================================
// 
// EFF:衝撃波[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** eff_shock-wave_00.h ***
//========================================
#ifndef _EFF_SHOCKWAVE_00_H_		//このマクロ定義がされてなかったら
#define _EFF_SHOCKWAVE_00_H_		//二重インクルード防止のマクロを定義する

//*****************************************
// プロトタイプ宣言
//*****************************************
// EFF:衝撃波[00] の初期化処理
void InitEff_shockWave_00(void);
// EFF:衝撃波[00] の終了処理
void UninitEff_shockWave_00(void);
// EFF:衝撃波[00] の更新処理
void UpdateEff_shockWave_00(void);
// EFF:衝撃波[00] の描画処理
void DrawEff_shockWave_00(void);

// EFF:衝撃波[00] の設定処理
void SetEff_shockWave_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fThickness, float fEdgeHeight, Color inCol, Color outCol, int nLife);

#endif
