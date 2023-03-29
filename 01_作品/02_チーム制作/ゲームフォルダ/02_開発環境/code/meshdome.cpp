//--------------------------------------------------------------------------------------------------------
//
//メッシュドームの屋根・奈落[meshdome.cpp]
//Author:平澤詩苑
//
//--------------------------------------------------------------------------------------------------------
#include "main.h"
#include "meshdome.h"
#include "color.h"
#include "input.h"

//マクロ
#define NUM_MESHDOME				(1)						//メッシュドームの数
#define MESHDOME_TEX_WIDTH			(1.0f)					//背景の解像度
#define MESHDOME_TEX_HEIGHT			(1.0f)					//背景の解像度
#define MESHDOME_SPLIT				(16)					//背景の横の分割数		線は垂直に引いて、左右に分ける
#define MESHDOME_SEPALATE			(16)					//背景の縦の分割数		線は水平に引いて、上下に分ける
#define MESHDOME_NUM_OVERLAP		(2)						//最初と最後の頂点が重なる数

//頂点・インデックスの数　マクロ
#define MESHDOME_ALL_VERTEX			(MESHDOME_SPLIT * (MESHDOME_SEPALATE - 1) + MESHDOME_NUM_OVERLAP + (MESHDOME_SEPALATE - 1))	//全体の頂点数
#define MESHDOME_COVER_INDEX		(MESHDOME_SPLIT + MESHDOME_NUM_OVERLAP)					//フタのインデックス数
#define MESHDOME_SIDE_INDEX			((MESHDOME_SPLIT + 1) * (MESHDOME_SEPALATE - 2) * 2)	//側面のインデックス数  1周するのに必要な数 × 側面だけのインデックス × ペアでインデックスを設定する数（２）

//メッシュドームの構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	float			fRadius;	//半径
}MeshDome;

//グローバル変数
LPDIRECT3DTEXTURE9		g_pTextureMeshDome = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshDome = NULL;
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffCoverMeshDome = NULL,		//フタのインデックス番号
						g_pIdxBuffSideMeshDome = NULL,		//側面のインデックス番号
						g_pIdxBuffBottomMeshDome = NULL;	//底面のインデックス番号
D3DXMATRIX				g_mtxWorldMeshDome;
MeshDome				g_MeshDome;

//--------------------------------------------------------------------------------------------------------
//メッシュドームの初期化処理
//--------------------------------------------------------------------------------------------------------
void InitMeshDome(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\sky000.png", &g_pTextureMeshDome);

	//ドーム情報の初期化
	g_MeshDome.pos = ZERO_SET;
	g_MeshDome.rot = ZERO_SET;
	g_MeshDome.fRadius = MESHDOME_RADIUS;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHDOME_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshDome, NULL);

	//--------------------------------------
	//			頂点情報の設定
	//--------------------------------------
	SetMeshDomeVertexBuffer();

	//--------------------------------------
	//		インデックスバッファの生成
	//--------------------------------------
	//フタのインデックスバッファ
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_COVER_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffCoverMeshDome, NULL);
	
	//側面のインデックスバッファ
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_SIDE_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffSideMeshDome, NULL);

	//底面のインデックスバッファ
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHDOME_COVER_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIdxBuffBottomMeshDome, NULL);

	//--------------------------------------
	//		インデックスバッファの設定
	//--------------------------------------
	SetMeshDomeIndexBuffer();
}

//頂点座標の設定
void SetMeshDomeVertexBuffer(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffMeshDome->Lock(0, 0, (void**)&pVtx, 0);

	int nNumVtx = 0;			//頂点番号
	float yRadian = 0.0f;		//縦で分割した時の横１周分のY座標が決まる角度
	
	//天面の出っ張り頂点の設定
	pVtx[nNumVtx].pos = D3DXVECTOR3(0.0f, MESHDOME_RADIUS, 0.0f);
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
	nNumVtx++;
	
	//２番目の頂点から、横の分割数　‐　底面の出っ張りの１頂点　回数分 for文を回す
	for (int nCntDevideY = nNumVtx; nCntDevideY < MESHDOME_SEPALATE; nCntDevideY++)
	{
		//---------------------------------------------------------------------------------------------------------
		//	MEMO：縦に分割する数を使って原点から横１周分の高さまでの角度計算 (半周分で分割点が分かるので、3.14
		//---------------------------------------------------------------------------------------------------------
		yRadian += D3DX_PI / MESHDOME_SEPALATE;

		float TempLen = fabsf(sinf(yRadian) * g_MeshDome.fRadius);	//X・Zの半径
		float rot_Y = D3DX_PI;										//Y軸の角度
		float Height_Y = cosf(yRadian) * g_MeshDome.fRadius;		//Yの高さ
		float aTexV = (MESHDOME_TEX_HEIGHT / (MESHDOME_SEPALATE + 2)) * nCntDevideY;	//テクスチャの高さ

		int nStrageVtx = nNumVtx;		//１周するときに使用するため、１周の開始番号を格納

		for (int nCntDevideX = 0; nCntDevideX < MESHDOME_SPLIT; nCntDevideX++)
		{//横１周分の頂点座標を設定
			
			pVtx[nNumVtx].pos = 
				D3DXVECTOR3(
				sinf(rot_Y) * TempLen,		//Xの位置
				Height_Y,					//Yの位置
				cosf(rot_Y) * TempLen);		//Zの位置

			pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nNumVtx].col = XCOL_WHITE;

			float aTexU = (MESHDOME_TEX_WIDTH / (MESHDOME_SPLIT + 1)) * nCntDevideX;		//テクスチャの幅

			pVtx[nNumVtx++].tex = D3DXVECTOR2(aTexU, aTexV);
			
			//角度を　全体の角度÷分割数で割った答え分、引く
			rot_Y -= ONE_LAP / MESHDOME_SPLIT;
		}

		//１周させるため、１周の開始頂点情報を代入
		pVtx[nNumVtx].pos = pVtx[nStrageVtx].pos;
		pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nNumVtx].col = XCOL_WHITE;
		pVtx[nNumVtx++].tex = D3DXVECTOR2(1.0f, aTexV);
	}

	//底面の出っ張り頂点の設定
	pVtx[nNumVtx].pos = D3DXVECTOR3(0.0f, -MESHDOME_RADIUS, 0.0f);
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.5f, 1.0f);

	//頂点バッファのアンロック
	g_pVtxBuffMeshDome->Unlock();
}

//インデックス番号の設定
void SetMeshDomeIndexBuffer(void)
{
	WORD *pIdx;		//インデックスのポインタ

	//			／＼
	//		  ／天面＼		//「- 1」は天面の出っ張りを除くこと
	//		／番号設定＼	天井のでっぱりを除いた分カウンターを回す
		//フタのインデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffCoverMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	//インデックス番号
	int nNumIdx = 0;

	for (nNumIdx = 0; nNumIdx <= MESHDOME_COVER_INDEX - 1; nNumIdx++)
	{
		//下の計算式では、　ちょうど1周するときに答えが1になる　　それ以外では、カウンター÷分割数　の余りが番号として設定される
		//pIdx[nNumIdx] = (nNumIdx / (MESHDOME_COVER_INDEX - 1)) + (nNumIdx % (MESHDOME_COVER_INDEX - 1));
		pIdx[nNumIdx] = nNumIdx;
	}

	//天面のインデックスバッファのアンロック
	g_pIdxBuffCoverMeshDome->Unlock();

	//	／----------------＼
	// <	側面番号設定	>
	//	＼----------------／
	//インデックス番号初期化
	nNumIdx = 0;

	//側面のインデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffSideMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntHeight = 1; nCntHeight < MESHDOME_SEPALATE - 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < MESHDOME_SPLIT + 1; nCntWidth++)
		{
			int  Over = (nCntWidth + 1) + ((nCntHeight - 1) * (MESHDOME_SPLIT + 1)),
				Under = (nCntWidth + 1) + ((nCntHeight - 0) * (MESHDOME_SPLIT + 1));

			pIdx[nNumIdx++] = (nCntWidth + 1) + ((nCntHeight - 1) * (MESHDOME_SPLIT + 1));
			pIdx[nNumIdx++] = (nCntWidth + 1) + ((nCntHeight - 0) * (MESHDOME_SPLIT + 1));
		}
	}

	//側面のインデックスバッファのアンロック
	g_pIdxBuffSideMeshDome->Unlock();

	//		＼底面番号／
	//		  ＼設定／
	//			＼／
	//フタのインデックスバッファをロックし、頂点番号へのポインタを取得
	g_pIdxBuffBottomMeshDome->Lock(0, 0, (void**)&pIdx, 0);

	nNumIdx = 0;								//インデックス番号初期化
	int nBottomLap = MESHDOME_COVER_INDEX - 1;	//底面の１周分の数
	int BottomVtx = MESHDOME_ALL_VERTEX - 1;	//底面番号

	for (int nCntIdx = 0; nCntIdx <= MESHDOME_COVER_INDEX; nCntIdx++)
	{
		//--------------------------------------------------
		//全体の頂点数から逆算して番号を割り出す
		//
		//MEMO : カウンターがゼロの場合		=>	底面番号が代入される
		//		 カウンターが１・最後の場合	=>	同じ番号が代入される (理由は、最後の場合　計算式の答えが　「１ あまり ０」 になるため
		//		 それ以外の場合				=>	「底面の番号 - (カウンター ÷ 底面の１周分の数  の余り)」  で算出された番号が代入される
		//--------------------------------------------------
		pIdx[nNumIdx++] = BottomVtx - nCntIdx;
	}

	//底面のインデックスバッファのアンロック
	g_pIdxBuffBottomMeshDome->Unlock();
}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの終了処理
//--------------------------------------------------------------------------------------------------------
void UninitMeshDome(void)
{
	//フタのインデックスの破棄
	if (g_pIdxBuffCoverMeshDome != NULL)
	{
		g_pIdxBuffCoverMeshDome->Release();
		g_pIdxBuffCoverMeshDome = NULL;
	}

	//側面のインデックスの破棄
	if (g_pIdxBuffSideMeshDome != NULL)
	{
		g_pIdxBuffSideMeshDome->Release();
		g_pIdxBuffSideMeshDome = NULL;
	}

	//底のインデックスの破棄
	if (g_pIdxBuffBottomMeshDome != NULL)
	{
		g_pIdxBuffBottomMeshDome->Release();
		g_pIdxBuffBottomMeshDome = NULL;
	}

	//テクスチャの破棄
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshDome != NULL)
	{
		g_pVtxBuffMeshDome->Release();
		g_pVtxBuffMeshDome = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの更新処理
//--------------------------------------------------------------------------------------------------------
void UpdateMeshDome(void)
{
	
}

//--------------------------------------------------------------------------------------------------------
//メッシュドームの描画処理
//--------------------------------------------------------------------------------------------------------
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshDome);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshDome.rot.y, g_MeshDome.rot.x, g_MeshDome.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshDome, &g_mtxWorldMeshDome, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshDome.pos.x, g_MeshDome.pos.y, g_MeshDome.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshDome, &g_mtxWorldMeshDome, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshDome);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshDome, 0, sizeof(VERTEX_3D));

	//両面カリングをON
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//-----------------------------------------------------------
	//フタのインデックスを設定し、描画する
	//-----------------------------------------------------------
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffCoverMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHDOME_COVER_INDEX, 0, MESHDOME_SPLIT);

	//-----------------------------------------------------------
	//側面のインデックスを設定し、描画する
	//-----------------------------------------------------------
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffSideMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHDOME_SIDE_INDEX, 0, MESHDOME_SPLIT * 2 * (MESHDOME_SEPALATE - 1) - 6);

	//-----------------------------------------------------------
	//底面のインデックスを設定し、描画する
	//-----------------------------------------------------------
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffBottomMeshDome);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshDome);

	//ポリゴン描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHDOME_COVER_INDEX, 0, MESHDOME_SPLIT);

	//普通のカリングモードにする
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}