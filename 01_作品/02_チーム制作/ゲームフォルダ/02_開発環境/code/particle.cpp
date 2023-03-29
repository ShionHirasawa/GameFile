#include"main.h"
#include"particle.h"
#include"bonus.h"
#include"PvP_Player.h"

//**********************************************************************
//マクロ定義
//**********************************************************************
#define PARTICLE_DIRECTION_MOVE	(10.0f)		//収束するタイプのパーティクルの収縮量
#define PARTICLE_TEX_PASS		"data/TEXTURE/effect000.jpg"

//**********************************************************************
//グローバル変数
//**********************************************************************
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;			//テクスチャバッファ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffParticle = NULL;			//頂点バッファ
D3DXMATRIX				g_mtxWorldParticle;					//ワールドマトリックス
Particle				g_aParticle[MAX_PARTICLE];			//パーティクルの情報

//======================================================================
//パーティクルの初期化処理
//======================================================================
void InitParticle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ポインタを取得
	VERTEX_3D *pVtx;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEX_PASS, &g_pTextureParticle);

	//エフェクトのデータの初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = 
		g_aParticle[nCntParticle].move = ZERO_SET;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_PARTICLE,	D3DUSAGE_WRITEONLY,	FVF_VERTEX_3D, D3DPOOL_MANAGED,&g_pVtxBuffParticle,	NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += VTX_MAX)
	{
		//位置の設定
		pVtx[VTX_LE_UP].pos = pVtx[VTX_RI_UP].pos = pVtx[VTX_LE_DO].pos = pVtx[VTX_RI_DO].pos = ZERO_SET;

		//法線の設定
		pVtx[VTX_LE_UP].nor = pVtx[VTX_RI_UP].nor = pVtx[VTX_LE_DO].nor = pVtx[VTX_RI_DO].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//色の設定
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}
//======================================================================
//パーティクルの終了処理
//======================================================================
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}
//======================================================================
//パーティクルの更新処理
//======================================================================
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += VTX_MAX)
	{
		//使用されているとき
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//パーティクルの状態設定
			StateParticle(nCntParticle, pVtx);
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}
//======================================================================
//パーティクルの描画処理
//======================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();  //デバイスの取得
	D3DXMATRIX		  mtxTrans;				  //計算用マトリックス
	D3DXMATRIX		  mtxView;				  //ビューマトリックス取得用

	//頂点バッファのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureParticle);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	
	

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].nObject != OBJECT_PLAYER_GHOST)
		{//アルファブレンディングを加算合成に設定（ゴースト以外）
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else
		{//アルファブレンディングを減算合成に設定（ゴースト）
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldParticle);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに向ける
			D3DXMatrixInverse(&g_mtxWorldParticle, NULL, &mtxView);		//逆行列を求める
			g_mtxWorldParticle._41 = 0.0f;
			g_mtxWorldParticle._42 = 0.0f;
			g_mtxWorldParticle._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle, &g_mtxWorldParticle, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * 4, 2);
		}
	}

	//元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
//======================================================================
//パーティクルのセット処理
//======================================================================
void SetParticle(D3DXVECTOR3 pos, float fRadius, int nLife, PARTICLE nType, PARTICLE_OBJECT nObject)
{
	int ParticleCount = 0;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//パーティクルが使われていない
		if (g_aParticle[nCntParticle].bUse == false)
		{
			//位置の設定
			g_aParticle[nCntParticle].pos = pos;

			//半径の設定
			g_aParticle[nCntParticle].fRadius = fRadius;

			//寿命の数値保存
			g_aParticle[nCntParticle].nLifeTemp = g_aParticle[nCntParticle].nLife;

			//種類の設定
			g_aParticle[nCntParticle].nType = nType;

			//オブジェクト種類の設定
			g_aParticle[nCntParticle].nObject = nObject;

			switch (nType)
			{
				//通常のパーティクル
			case PARTICLE_NORMAL:

				//移動量の設定
				g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * (float)(rand() % 8 - 4) * 1.0f;
				break;

				//吸収のパーティクル
			case PARTICLE_ACSORPTION:

				g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].pos.x + sinf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;
				g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].pos.y + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;
				g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].pos.z + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *3.0f;

				//パーティクルの移動方向設定
				DirectionParticle(nCntParticle);

				break;

				//吸収のパーティクル（ゴースト）
			case PARTICLE_ACSORPTION_GHOST:

				g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].pos.x + sinf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *1.0f;
				g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].pos.y + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *1.0f;
				g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].pos.z + cosf((float)(rand() % 629 - 314) / 100.0f)* (float)(rand() % 2 + 20) *1.0f;

				//パーティクルの移動方向設定
				DirectionParticle(nCntParticle);

				break;
			}

			//寿命の設定
			g_aParticle[nCntParticle].nLife = (rand() % 30 - 15) + nLife;

			//発生量カウント
			ParticleCount++;

			//パーティクルが使われている状態にする
			g_aParticle[nCntParticle].bUse = true;

			//種類別発生量における処理終了判定
			if (nType == PARTICLE_NORMAL && ParticleCount == NORMAL_PARTICLE)
			{
				break;
			}
			else if (nType == PARTICLE_ACSORPTION && ParticleCount == ACSORPTION_PARTICLE)
			{
				break;
			}
			else if (nType == PARTICLE_ACSORPTION_GHOST && ParticleCount == ACSORPTION_PARTICLE)
			{
				break;
			}
		}
	}
}
//======================================================================
//パーティクルの状態設定
//======================================================================
void StateParticle(int nCount,VERTEX_3D *pVtx)
{
	//ボーナスの体力取得
	int BonusLife = GetBonus().nLife;

	//寿命の減少
	g_aParticle[nCount].nLife--;

	//パーティクルの破棄
	if (g_aParticle[nCount].nLife <= 0)
	{
		g_aParticle[nCount].bUse = false;
		return;//処理を終える
	}

	//座標の設定
	pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aParticle[nCount].fRadius, +g_aParticle[nCount].fRadius, 0.0f);
	pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aParticle[nCount].fRadius, +g_aParticle[nCount].fRadius, 0.0f);
	pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aParticle[nCount].fRadius, -g_aParticle[nCount].fRadius, 0.0f);
	pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aParticle[nCount].fRadius, -g_aParticle[nCount].fRadius, 0.0f);

	//移動量の設定
	g_aParticle[nCount].pos.x += g_aParticle[nCount].move.x;
	g_aParticle[nCount].pos.y += g_aParticle[nCount].move.y;
	g_aParticle[nCount].pos.z += g_aParticle[nCount].move.z;

	//*************************************//
	//パーティクルの種類別 移動量の調整処理//
	//*************************************//
	switch (g_aParticle[nCount].nType)
	{
		//******************//
		//==================//
		//通常のパーティクル//
		//==================//
		//******************//
		case PARTICLE_NORMAL:
		{
			g_aParticle[nCount].move.x += (0.0f - g_aParticle[nCount].move.x) * 0.3f;
			g_aParticle[nCount].move.y += (0.0f - g_aParticle[nCount].move.y) * 0.3f;
			g_aParticle[nCount].move.z += (0.0f - g_aParticle[nCount].move.z) * 0.3f;
		}
		break;

		//******************//
		//==================//
		//吸収のパーティクル//
		//==================//
		//******************//
		case PARTICLE_ACSORPTION:
		{
			g_aParticle[nCount].move.x += (0.0f - g_aParticle[nCount].move.x) * 0.2f;
			g_aParticle[nCount].move.y += (0.0f - g_aParticle[nCount].move.y) * 0.2f;
			g_aParticle[nCount].move.z += (0.0f - g_aParticle[nCount].move.z) * 0.2f;
		}
		break;

		case PARTICLE_ACSORPTION_GHOST:
		{
			g_aParticle[nCount].move.x += (0.0f - g_aParticle[nCount].move.x) * 0.5f;
			g_aParticle[nCount].move.y += (0.0f - g_aParticle[nCount].move.y) * 0.5f;
			g_aParticle[nCount].move.z += (0.0f - g_aParticle[nCount].move.z) * 0.5f;
		}
		break;
	}

	//パーティクルの寿命の割合を算出
	float LifeParcent = (float)g_aParticle[nCount].nLife / (float)g_aParticle[nCount].nLifeTemp;

	//*********************************//
	//パーティクルの持ち主別 色分け処理//
	//*********************************//
	switch (g_aParticle[nCount].nObject)
	{
		//=======================================
		//		プレイヤーのバフ パーティクル
		//=======================================
		//アタック強化
	case OBJECT_PLAYER_ATK:
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.1f);
		break;
		
		//ガード強化
	case OBJECT_PLAYER_DEF:
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f);
		break;

		//ゴースト化
	case OBJECT_PLAYER_GHOST:
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.15f);
		break;

	case OBJECT_PLAYER_INVINCIBLE:
		pVtx[VTX_LE_UP].col = pVtx[VTX_RI_UP].col = pVtx[VTX_LE_DO].col = pVtx[VTX_RI_DO].col = D3DXCOLOR(1.0f, 0.75f, 0.0f, 0.1f);
		break;

		//ボーナスパーティクル
	case OBJECT_BONUS:
		if (BonusLife == 3)
		{
			pVtx[VTX_LE_UP].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(0.6f, 0.2f, 0.4f, LifeParcent);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(0.4f, 0.2f, 0.6f, LifeParcent);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(0.2f, 0.2f, 0.8f, LifeParcent);
		}
		else if (BonusLife == 2)
		{
			pVtx[VTX_LE_UP].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(0.6f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(0.4f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, LifeParcent);
		}
		else
		{
			pVtx[VTX_LE_UP].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, LifeParcent);
			pVtx[VTX_RI_UP].col = D3DXCOLOR(0.6f, 0.4f, 0.2f, LifeParcent);
			pVtx[VTX_LE_DO].col = D3DXCOLOR(0.4f, 0.6f, 0.2f, LifeParcent);
			pVtx[VTX_RI_DO].col = D3DXCOLOR(0.2f, 0.8f, 0.2f, LifeParcent);
		}
		break;
	}
}
//======================================================================
//パーティクルの移動方向設定
//======================================================================
void DirectionParticle(int nCount)
{
	//持ち主の座標保存用
	D3DXVECTOR3 OrnerPos;

	switch (g_aParticle[nCount].nObject)
	{
		//プレイヤーパーティクル
	case OBJECT_PLAYER_ATK:
	case OBJECT_PLAYER_DEF:
	case OBJECT_PLAYER_GHOST:
		OrnerPos = GetPlayer()->pos;
		break;

		//ボーナスパーティクル
	case OBJECT_BONUS:
		OrnerPos = GetBonus().pos;
		break;
	}

	//持ち主が右にいる
	if (g_aParticle[nCount].pos.x < OrnerPos.x)
	{
		//右（プラス）方向への移動量を代入
		g_aParticle[nCount].move.x = PARTICLE_DIRECTION_MOVE;
	}
	//持ち主が左にいる
	else if (OrnerPos.x < g_aParticle[nCount].pos.x)
	{
		//左（マイナス）方向への移動量を代入
		g_aParticle[nCount].move.x = -PARTICLE_DIRECTION_MOVE;
	}

	//持ち主が奥にいる
	if (OrnerPos.z > g_aParticle[nCount].pos.z)
	{
		//奥（プラス）方向への移動量を代入
		g_aParticle[nCount].move.z = PARTICLE_DIRECTION_MOVE;
	}
	//持ち主が手前にいる
	else if (OrnerPos.z < g_aParticle[nCount].pos.z)
	{
		//前（マイナス）方向への移動量を代入
		g_aParticle[nCount].move.z = -PARTICLE_DIRECTION_MOVE;
	}

	//持ち主が下にいる
	if (g_aParticle[nCount].pos.y < OrnerPos.y)
	{
		//上（プラス）方向への移動量を代入
		g_aParticle[nCount].move.y = PARTICLE_DIRECTION_MOVE;
	}
	//持ち主が上にいる
	else if (OrnerPos.y < g_aParticle[nCount].pos.y)
	{
		//下（マイナス）方向への移動量を代入
		g_aParticle[nCount].move.y = -PARTICLE_DIRECTION_MOVE;
	}
}