/*==========================================================================================================================

カメラ分割による境目の枠表示処理[camera_frame.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "camera_frame.h"
#include "color.h"
#include "input.h"

#define FRAME_LINE_SIZE		(4.0f)		//枠線の線の幅

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCameraFrame = NULL;		//頂点バッファへのポインタ
bool					g_bUseFrame[UseFrame_MAX];			//各フレームを使用するかどうか
const bool c_aUseFrame[NumCamera_MAX][UseFrame_MAX]=
{
	{ false ,false ,false ,false },	//枠線全てOFF
	{ false ,true ,false ,false },	//中央縦線のみをON
	{ true ,false ,false ,false },	//中央横線のみをON
	{ true ,true ,false ,false },	//中央の十字線をON
	{ false ,true ,true ,true }		//縦線すべてON
};

//==============================================================
//画面分割の枠の初期化処理
//===============================================================
void InitCameraFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * UseFrame_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCameraFrame, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCameraFrame->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntFrame = 0; nCntFrame < UseFrame_MAX; nCntFrame++, pVtx += VTX_MAX)
	{
		//枠が使われている
		g_bUseFrame[nCntFrame] = false;

		//頂点座標の設定

		switch (nCntFrame)
		{
		case UseFrame_WIDTH://横線の場合

			//画面の横幅の半分の位置を中心として、線のサイズ分　横に分岐させる
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 2) - FRAME_LINE_SIZE, 0.0f);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, (SCREEN_HEIGHT / 2) - FRAME_LINE_SIZE, 0.0f);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 2) + FRAME_LINE_SIZE, 0.0f);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, (SCREEN_HEIGHT / 2) + FRAME_LINE_SIZE, 0.0f);
			break;

		case UseFrame_HEIGHT://縦線の場合

			//画面の高さの半分を中心として、線のサイズ分　縦に分岐させる
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			break;

		case UseFrame_HEIGHT4_1://縦線4分の1番目の場合

			//画面の高さの4分の1を中心として、線のサイズ分　縦に分岐させる
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) - FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) + FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) - FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) + FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			break;

		case UseFrame_HEIGHT4_3://縦線4分の3番目の場合

			//画面の高さの4分の1を中心として、線のサイズ分　縦に分岐させる
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) * 3 - FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) * 3 + FRAME_LINE_SIZE, 0.0f, 0.0f);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) * 3 - FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 4) * 3 + FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
			break;
		}

		////横線の場合
		//if (nCntFrame == UseFrame_WIDTH)
		//{
		//	//画面の横幅の半分の位置を中心として、線のサイズ分　横に分岐させる
		//	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 2) - FRAME_LINE_SIZE, 0.0f);
		//	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, (SCREEN_HEIGHT / 2) - FRAME_LINE_SIZE, 0.0f);
		//	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 2) + FRAME_LINE_SIZE, 0.0f);
		//	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, (SCREEN_HEIGHT / 2) + FRAME_LINE_SIZE, 0.0f);
		//}

		////縦線の場合
		//else if (nCntFrame == UseFrame_HEIGHT)
		//{
		//	//画面の高さの半分を中心として、線のサイズ分　縦に分岐させる
		//	pVtx[VTX_LE_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - FRAME_LINE_SIZE, 0.0f, 0.0f);
		//	pVtx[VTX_RI_UP].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + FRAME_LINE_SIZE, 0.0f, 0.0f);
		//	pVtx[VTX_LE_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
		//	pVtx[VTX_RI_DO].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + FRAME_LINE_SIZE, SCREEN_HEIGHT, 0.0f);
		//}

		//rhwの設定
		{
			pVtx[VTX_LE_UP].rhw = RHW;
			pVtx[VTX_RI_UP].rhw = RHW;
			pVtx[VTX_LE_DO].rhw = RHW;
			pVtx[VTX_RI_DO].rhw = RHW;
		}

		//頂点カラーの設定
		{
			pVtx[VTX_LE_UP].col = RGBA_BLACK;
			pVtx[VTX_RI_UP].col = RGBA_BLACK;
			pVtx[VTX_LE_DO].col = RGBA_BLACK;
			pVtx[VTX_RI_DO].col = RGBA_BLACK;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffCameraFrame->Unlock();
}

//==============================================================
//画面分割の枠の終了処理
//===============================================================
void UninitCameraFrame(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffCameraFrame != NULL)
	{
		g_pVtxBuffCameraFrame->Release();
		g_pVtxBuffCameraFrame = NULL;
	}
}

//==============================================================
//画面分割の枠の描画処理
//===============================================================
void DrawCameraFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCameraFrame, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int nCntFrame = 0; nCntFrame < UseFrame_MAX; nCntFrame++)
	{
		//そのフレームが使われている
		if (g_bUseFrame[nCntFrame] == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFrame * VTX_MAX, 2);
		}
	}
}

//使用する枠を設定
void SetUseFrame(NumCamera type)
{
	for (int nCntCamera = 0; nCntCamera < UseFrame_MAX; nCntCamera++)
	{
		g_bUseFrame[nCntCamera] = c_aUseFrame[type][nCntCamera];
	}

}