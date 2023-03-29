//==========================================
//
//メッシュフィールドプログラム[meshfield.cpp]
//Author:飯田洲暉
//
//==========================================
#include "main.h"
#include "PvP_player.h"
#include "meshfield.h"
#include "file.h"
#include "color.h"
#include "input.h"
#include "timer.h"

//マクロ定義
#define MESHFIELD_SPLIT			(64)					//分割数
#define CENTER_AND_ONELAP		(2)						//中心点と１周で重なる点
#define MESHFIELD_ALL_VERTEX	(MESHFIELD_SPLIT + 1)	//周辺の分割地点と中心
#define MESHFIELD_ALL_INDEX		(MESHFIELD_SPLIT + 2)	//周辺の分割地点と中心・１周で重なる点
#define MESHFIELD_MIN_RADIUS	(250.0f)				//最小半径
#define MESHFIELD_SHRINK_SPEED	(0.1f)					//縮まるスピード
#define MESHFIELD_SHRINK_TIME	(70)					//縮まり始める時間

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMeshfield = NULL;		//テクスチャのポインタ
LPDIRECT3DINDEXBUFFER9	g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
D3DXMATRIX				g_mtxWorldMeshfield;			//ワールドマトリックス
MESHFIELD				g_MeshField;

//====================================================================
//メッシュフィールドの初期化処理
//====================================================================
void InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffMeshfield, NULL);

	//頂点情報設定
	SetFieldVertex();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESHFIELD_ALL_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,	&g_pIdxBuffMeshField, NULL);

	//インデックス情報を設定
	SetFieldIndex();
}

//メッシュフィールドの頂点情報を設定
void SetFieldVertex(void)
{
	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//頂点番号
	int nNumVtx = 0;

	float Rot = D3DX_PI;	//角度

	pVtx[nNumVtx].pos = ZERO_SET;				//中心座標代入
	pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//法線
	pVtx[nNumVtx].col = XCOL_WHITE;
	pVtx[nNumVtx].tex = D3DXVECTOR2(0.5f, 0.5f);
	nNumVtx++;

	//周辺の頂点情報の設定
	for (int nCntVtx = 0; nCntVtx < MESHFIELD_SPLIT; nCntVtx++, nNumVtx++)
	{
		pVtx[nNumVtx].pos = D3DXVECTOR3(
			sinf(Rot) * g_MeshField.fRadius,	//Xの位置
			0.0f,								//Yの位置
			cosf(Rot) * g_MeshField.fRadius);	//Zの位置

		pVtx[nNumVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nNumVtx].col = XCOL_WHITE;

		float aTexU = 0.5f - 0.5f * sinf(Rot);	//テクスチャ幅
		float aTexV = 0.5f - 0.5f * cosf(Rot);	//テクスチャ高さ

		pVtx[nNumVtx].tex = D3DXVECTOR2(aTexU, aTexV);

		//角度を　全体の角度÷分割数で割った答え分、引く
		Rot -= ONE_LAP / MESHFIELD_SPLIT;
	}

	//蝶バッファをアンロック
	g_pVtxBuffMeshfield->Unlock();
}

//メッシュフィールドのインデックス情報を設定
void SetFieldIndex(void)
{
	//インデックス情報へのポインタ
	WORD *pIdx;

	//インデックスバッファをロックし頂点情報へのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nNumIdx = 0; nNumIdx < MESHFIELD_ALL_INDEX; nNumIdx++)
	{
		//下の計算式では、　ちょうど1周するときに答えが1になる　　それ以外では、カウンター÷分割数　の余りが番号として設定される
		pIdx[nNumIdx] = (nNumIdx / (MESHFIELD_ALL_INDEX - 1)) + (nNumIdx % (MESHFIELD_ALL_INDEX - 1));
	}

	g_pIdxBuffMeshField->Unlock();
}

//====================================================================
//メッシュフィールドの終了処理
//====================================================================
void UninitMeshfield(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//====================================================================
//メッシュフィールドの更新処理
//====================================================================
void UpdateMeshfield(void)
{
	//PvPのゲームモード時だけ、縮小させる
	if (GetMode() == MODE_PvPGAME)
	{
		//一定時間後半径を縮める
		if (GetTime() <= MESHFIELD_SHRINK_TIME)
		{//半径を縮める
			g_MeshField.fRadius -= MESHFIELD_SHRINK_SPEED;
		}

		//最小半径よりも小さくなった
		if (g_MeshField.fRadius < MESHFIELD_MIN_RADIUS)
		{
			//最小半径に直す
			g_MeshField.fRadius = MESHFIELD_MIN_RADIUS;
		}

		//頂点座標設定
		SetFieldVertex();
	}
}

//====================================================================
//メッシュフィールドの描画処理
//====================================================================
void DrawMeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//両面カリングをON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshField.rot.y, g_MeshField.rot.x, g_MeshField.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshField.pos.x, g_MeshField.pos.y, g_MeshField.pos.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MESHFIELD_ALL_VERTEX, 0, MESHFIELD_SPLIT);

	//普通のカリングモードにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//====================================================================
//メッシュフィールドの設定処理
//Author:石原颯馬
//Memo:メッシュフィールド複数個使用するならfile.cppの構造変えなきゃならんので言って
//====================================================================
void SetMeshField(int nTexNum, MESHFIELD mf)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\grassland001.png", &g_pTextureMeshfield)))
	{
	}

	//情報代入
	g_MeshField = mf;
}

//====================================================================
//メッシュフィールドに乗ったかどうかの判定
//Author:平澤詩苑
//Memo:フィールドは当たり判定というより「乗る」イメージが強かったので、変数名を  Land (乗る)  にしています
//現在、プレイヤーの更新処理の関係で処理を行っておりません
//====================================================================
bool LandMeshfield(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	//現在・前回の位置とフィールドの中心位置の差を格納
	D3DXVECTOR2 PosDiff;

	PosDiff.x = powf(g_MeshField.pos.x - pPos->x, 2.0f);	//現在のＸ座標の差の２乗
	PosDiff.y = powf(g_MeshField.pos.z - pPos->z, 2.0f);	//現在のＺ座標の差の２乗	変数は  "Ｙ" だけど、Ｚ座標の差を入れます 

	float fLength = sqrtf(PosDiff.x + PosDiff.y);	//現在のＸ・Ｚの長さを取得

	//中心位置からの距離がフィールドの半径以下だった
	if (fLength <= g_MeshField.fRadius)
	{
		if (g_MeshField.pos.y <= pPosOld->y &&	//前回はフィールドから上に居て、
			g_MeshField.pos.y >= pPos->y)		//現在はフィールドから下にいる
		{
			pPos->y = g_MeshField.pos.y;	//フィールドの高さに戻す
			return true;					//乗った判定を返して終了			
		}
	}

	//乗っていなかった
	return false;
}

//メッシュフィールドの情報取得
MESHFIELD *GetMeshField(void)
{
	return &g_MeshField;
}