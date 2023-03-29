//==========================================
//
//当たり判定プログラム[collision.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "collision.h"
#include "fence.h"
#include "input.h"
#include "debugproc.h"
#include "object.h"

//マクロ
#define REFRECT_WEAK			(2.3f)	//反射を弱める・強めるのに使う
#define REFRECT_MAX				(50.0f)	//反射量最大値
#define CHARGE_DOWN				(0.8f)	//チャージゲージ減少量
#define VIBE_TIME				(20)	//バイブレーション時間
#define TREE_RADIUS				(50.0f)	//木の当たり判定（プレイヤーのサイズも考慮
#define TREE_HEIGHT				(70.0f)	//木の高さ判定（葉っぱは考慮していない
#define TREE_REFRECT			(-0.5f)	//木の反射係数

//当たり判定範囲構造体
typedef struct
{
	D3DXVECTOR3 pos0, pos1, pos2, pos3;
} CollisionPos;

//プロト
void GenerateCollision(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CollisionPos *pCollision, float fWidth, float fDepth);
void RefrectPlayer(Player *pPlayer, Fence *pFence, float fRate, D3DXVECTOR3 vecLine, D3DXVECTOR3 vecStartPos);
bool HitPlayerToPlayer(Player *pATKPlayer, Player *pHitPlayer, D3DXVECTOR3 collPos, D3DXVECTOR3 vecLine, float fRate);

//========================
//プレイヤー同士の衝突判定処理
//========================
bool CollisionPP(Player *pATKPlayer, float fWidth, float fHeight, float fDepth)
{
	//=pos0~pos3の説明==================
	// pos3		pos2
	//	・<-----・		矢印:vecLine
	//	｜		↑
	//	｜		｜
	//	↓		｜
	//	・----->・
	// pos0		pos1
	//==================================

	//頂点
	CollisionPos collPos;

	//ベクトル
	D3DXVECTOR3 vecLineRight, vecToPosRight, vecToPosOldRight;
	D3DXVECTOR3 vecLineLeft, vecToPosLeft, vecToPosOldLeft;
	D3DXVECTOR3 vecLineUp, vecToPosUp, vecToPosOldUp;
	D3DXVECTOR3 vecLineDown, vecToPosDown, vecToPosOldDown;
	D3DXVECTOR3 vecMove[2];

	//プレイヤー
	Player *pPlayer = GetPlayer();

	//2は2頂点の2
	float fAreaARight[2], fAreaALeft[2], fAreaBRight[2], fAreaBLeft[2];
	float fAreaAUp[2], fAreaADown[2], fAreaBUp[2], fAreaBDown[2];

	//未反映の位置考慮
	D3DXVECTOR3 posTemp = pATKPlayer->pos +pATKPlayer->move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++,pPlayer++)
	{
		if (pPlayer != pATKPlayer && pPlayer->nGhostItemTime <= 0 && pPlayer->bUsePlayer == true)
		{
			//頂点設定
			GenerateCollision(pPlayer->pos, pPlayer->rot, &collPos, fWidth, fDepth);

			//ベクトル求める
			//move
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				vecMove[nCntCollision] = (posTemp + pATKPlayer->faceCollider[nCntCollision]) - (pATKPlayer->posOld + pATKPlayer->faceCollider[nCntCollision]);
			}

			//X
			//右方向の計算
			vecLineRight = collPos.pos1 - collPos.pos0;
			vecToPosRight = posTemp - collPos.pos0;
			vecToPosOldRight = pATKPlayer->posOld - collPos.pos0;

			//左方向の計算
			vecLineLeft = collPos.pos3 - collPos.pos2;
			vecToPosLeft = posTemp - collPos.pos2;
			vecToPosOldLeft = pATKPlayer->posOld - collPos.pos2;

			//Z
			//上方向の計算
			vecLineUp = collPos.pos2 - collPos.pos1;
			vecToPosUp = posTemp - collPos.pos1;
			vecToPosOldUp = pATKPlayer->posOld - collPos.pos1;
			//下方向の計算
			vecLineDown = collPos.pos0 - collPos.pos3;
			vecToPosDown = posTemp - collPos.pos3;
			vecToPosOldDown = pATKPlayer->posOld - collPos.pos3;

			//当たり判定本番
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				//X
				//面積求める
				fAreaARight[nCntCollision] = TASUKIGAKE(vecToPosRight.x, vecToPosRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaALeft[nCntCollision] = TASUKIGAKE(vecToPosLeft.x, vecToPosLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBRight[nCntCollision] = TASUKIGAKE(vecLineRight.x, vecLineRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBLeft[nCntCollision] = TASUKIGAKE(vecLineLeft.x, vecLineLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//左側AND範囲内
				if ((vecLineRight.z * vecToPosRight.x) - (vecLineRight.x * vecToPosRight.z) <= 0.0f && (vecLineRight.z * vecToPosOldRight.x) - (vecLineRight.x * vecToPosOldRight.z) >= 0.0f)
				{
					if (fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] >= 0.0f && fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= pPlayer->pos.y && posTemp.y <= pPlayer->pos.y + fHeight)
						{
							float fRate = fAreaARight[nCntCollision] / fAreaBRight[nCntCollision];
							return HitPlayerToPlayer(pATKPlayer, pPlayer, collPos.pos0, vecLineRight, fRate);
							break;
						}
					}
				}
				else if ((vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) <= 0.0f && (vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f)
				{
					if (fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] >= 0.0f && fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= pPlayer->pos.y && posTemp.y <= pPlayer->pos.y + fHeight)
						{
							float fRate = fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision];
							return HitPlayerToPlayer(pATKPlayer, pPlayer, collPos.pos2, vecLineLeft, fRate);
							break;
						}
					}
				}

				//Z
				//面積求める
				fAreaAUp[nCntCollision] = TASUKIGAKE(vecToPosUp.x, vecToPosUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaADown[nCntCollision] = TASUKIGAKE(vecToPosDown.x, vecToPosDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBUp[nCntCollision] = TASUKIGAKE(vecLineUp.x, vecLineUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBDown[nCntCollision] = TASUKIGAKE(vecLineDown.x, vecLineDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//左側AND範囲内
				if ((vecLineUp.z * vecToPosUp.x) - (vecLineUp.x * vecToPosUp.z) <= 0.0f && (vecLineUp.z * vecToPosOldUp.x) - (vecLineUp.x * vecToPosOldUp.z) >= 0.0f)
				{
					if (fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] >= 0.0f && fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= pPlayer->pos.y && posTemp.y <= pPlayer->pos.y + fHeight)
						{
							float fRate = fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision];
							return HitPlayerToPlayer(pATKPlayer, pPlayer, collPos.pos1, vecLineUp, fRate);
							break;
						}
					}
				}
				else if ((vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) <= 0.0f && (vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f)
				{
					if (fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] >= 0.0f && fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] <= 1.0f)
					{
						if (posTemp.y >= pPlayer->pos.y && posTemp.y <= pPlayer->pos.y + fHeight)
						{
							float fRate = fAreaADown[nCntCollision] / fAreaBDown[nCntCollision];
							return HitPlayerToPlayer(pATKPlayer, pPlayer, collPos.pos3, vecLineDown, fRate);
							break;
						}
					}
				}
			}
		}
	}
	return false;
}

//========================
//ヒップドロップ時の衝突処理
//========================
bool CollisionHipDropPP(Player *pATKPlayer, float fWidth, float fHeight, float fDepth, float fRadius, float fPower)
{
	//=pos0~pos2の説明==================
	//
	//		・g_ap[nPNum].posOld
	//		↓
	//		・g_ap[nPNum].pos
	// pos1		pos0
	//	・<-----・		矢印:vecLine
	//	｜
	//	｜
	//	↓
	//　・
	// pos2
	//==================================

	//頂点
	CollisionPos collPos;

	//ベクトル
	D3DXVECTOR3 vecLineX, vecToPosX, vecToPosOldX;
	D3DXVECTOR3 vecLineZ, vecToPosZ, vecToPosOldZ;
	D3DXVECTOR3 vecMove;

	//プレイヤー
	Player *pPlayer = GetPlayer();

	//面積
	float fAreaAX, fAreaBX, fAreaAZ, fAreaBZ;

	//未反映の位置考慮
	D3DXVECTOR3 posTemp = pATKPlayer->pos + pATKPlayer->move;

	for (int nCntOtherPlayer = 0; nCntOtherPlayer < MAX_USE_GAMEPAD; nCntOtherPlayer++, pPlayer++)
	{
		if (pPlayer != pATKPlayer && pPlayer->nGhostItemTime <= 0 && pPlayer->bHipDrop == false && pPlayer->bUsePlayer == true)
		{
			//頂点設定
			GenerateCollision(pPlayer->pos, pPlayer->rot, &collPos, fWidth, fDepth);

			//ベクトル求める
			//move
			vecMove = posTemp - pATKPlayer->posOld;

			//X
			vecLineX = collPos.pos1 - collPos.pos0;
			vecToPosX = posTemp - collPos.pos0;
			vecToPosOldX = pATKPlayer->posOld - collPos.pos0;

			//Z
			vecLineZ = collPos.pos2 - collPos.pos1;
			vecToPosZ = posTemp - collPos.pos1;
			vecToPosOldZ = pATKPlayer->posOld - collPos.pos1;

			//当たり判定本番
			//X
			//面積求める
			fAreaAX = TASUKIGAKE(vecToPosX.x, vecToPosX.y, vecMove.x, vecMove.y);
			fAreaBX = TASUKIGAKE(vecLineX.x, vecLineX.y, vecMove.x, vecMove.y);
			fAreaAZ = TASUKIGAKE(vecToPosZ.z, vecToPosZ.y, vecMove.z, vecMove.y);
			fAreaBZ = TASUKIGAKE(vecLineZ.z, vecLineZ.y, vecMove.z, vecMove.y);
			//左側AND範囲内
			float fHeightDelta = posTemp.y - pPlayer->pos.y;
			if (fHeightDelta <= fHeight)
			{
				float fRadiusDelta = PYTHAGORAS(pPlayer->pos.x - posTemp.x,
					pPlayer->pos.z - posTemp.z);

				if (fRadiusDelta <= fRadius)
				{
					//移動量計算
					float fAngleHipDrop = atan2f(pPlayer->pos.x - posTemp.x,
						pPlayer->pos.z - posTemp.z);
					pPlayer->move.x = sinf(fAngleHipDrop) * fPower;
					pPlayer->move.z = -cosf(fAngleHipDrop) * fPower;

					//ちょっと飛ばす
					pPlayer->moveV0.y = fPower;
					pPlayer->jumpTime = 0;

					//硬直
					pPlayer->nActionRigor = 30;

					//振動
					SetPadVibration(pPlayer->nPlayerNum, VIBE_POWER_LEVEL_02, VIBE_TIME, VIBE_STATE_01_LOOP);

					//攻撃された扱いにする
					pPlayer->nLastHitPlayer = pATKPlayer->nPlayerNum;
					pPlayer->bJump = true;
				}
			}
		}
	}
	return false;
}

//========================
//フェンスの衝突処理（移動処理中に行う）
//========================
bool CollisionFence(Player *pPlayer, float fFenceWidth, float fPlayerHeight, float fFenceDepth, float fenceSize)
{
	//頂点
	CollisionPos collPos;

	//ベクトル
	D3DXVECTOR3 vecLineRight, vecToPosRight, vecToPosOldRight;
	D3DXVECTOR3 vecLineLeft, vecToPosLeft, vecToPosOldLeft;
	D3DXVECTOR3 vecLineUp, vecToPosUp, vecToPosOldUp;
	D3DXVECTOR3 vecLineDown, vecToPosDown, vecToPosOldDown;
	D3DXVECTOR3 vecMove[2];

	//フェンス
	Fence *pFence = GetFence();

	//2は2頂点の2
	float fAreaARight[2], fAreaALeft[2], fAreaBRight[2], fAreaBLeft[2];
	float fAreaAUp[2], fAreaADown[2], fAreaBUp[2], fAreaBDown[2];

	//未反映の位置考慮
	D3DXVECTOR3 posTemp = pPlayer->pos + pPlayer->move;

	for (int nCntFence = 0; nCntFence < MAX_USE_FENCE; nCntFence++, pFence++)
	{
		if (pFence->bUse == true && pFence->bCollision == true)
		{
			//頂点設定
			GenerateCollision(pFence->pos, pFence->rot, &collPos, fFenceWidth * fenceSize, fFenceDepth * fenceSize);

			//ベクトル求める
			//move
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				vecMove[nCntCollision] = (posTemp + pPlayer->faceCollider[nCntCollision]) - (pPlayer->posOld + pPlayer->faceCollider[nCntCollision]);
			}

			//X
			//右方向の計算
			vecLineRight = collPos.pos1 - collPos.pos0;
			vecToPosRight = posTemp - collPos.pos0;
			vecToPosOldRight = pPlayer->posOld - collPos.pos0;

			//左方向の計算
			vecLineLeft = collPos.pos3 - collPos.pos2;
			vecToPosLeft = posTemp - collPos.pos2;
			vecToPosOldLeft = pPlayer->posOld - collPos.pos2;

			//Z
			//上方向の計算
			vecLineUp = collPos.pos2 - collPos.pos1;
			vecToPosUp = posTemp - collPos.pos1;
			vecToPosOldUp = pPlayer->posOld - collPos.pos1;
			//下方向の計算
			vecLineDown = collPos.pos0 - collPos.pos3;
			vecToPosDown = posTemp - collPos.pos3;
			vecToPosOldDown = pPlayer->posOld - collPos.pos3;

			//当たり判定本番
			for (int nCntCollision = 0; nCntCollision < 2; nCntCollision++)
			{
				//X
				//面積求める
				fAreaARight[nCntCollision] = TASUKIGAKE(vecToPosRight.x, vecToPosRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaALeft[nCntCollision] = TASUKIGAKE(vecToPosLeft.x, vecToPosLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBRight[nCntCollision] = TASUKIGAKE(vecLineRight.x, vecLineRight.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBLeft[nCntCollision] = TASUKIGAKE(vecLineLeft.x, vecLineLeft.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//左側AND範囲内
				if ((vecLineRight.z * vecToPosRight.x) - (vecLineRight.x * vecToPosRight.z) <= 0.0f && (vecLineRight.z * vecToPosOldRight.x) - (vecLineRight.x * vecToPosOldRight.z) >= 0.0f)
				{
					if (fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] >= 0.0f && fAreaARight[nCntCollision] / fAreaBRight[nCntCollision] <= 1.0f)
					{
						//割合求める
						float fRate = fAreaARight[nCntCollision] / fAreaBRight[nCntCollision];

						//反射処理
						RefrectPlayer(pPlayer, pFence, fRate, vecLineRight, collPos.pos0);

						//終了
						return true;
					}
				}
				else if ((vecLineLeft.z * vecToPosLeft.x) - (vecLineLeft.x * vecToPosLeft.z) <= 0.0f && (vecLineLeft.z * vecToPosOldLeft.x) - (vecLineLeft.x * vecToPosOldLeft.z) >= 0.0f)
				{
					if (fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] >= 0.0f && fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision] <= 1.0f)
					{
						//割合求める
						float fRate = fAreaALeft[nCntCollision] / fAreaBLeft[nCntCollision];

						//反射処理
						RefrectPlayer(pPlayer, pFence, fRate, vecLineLeft, collPos.pos2);

						//終了
						return true;
					}
				}

				//Z
				//面積求める
				fAreaAUp[nCntCollision] = TASUKIGAKE(vecToPosUp.x, vecToPosUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaADown[nCntCollision] = TASUKIGAKE(vecToPosDown.x, vecToPosDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBUp[nCntCollision] = TASUKIGAKE(vecLineUp.x, vecLineUp.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);
				fAreaBDown[nCntCollision] = TASUKIGAKE(vecLineDown.x, vecLineDown.z, vecMove[nCntCollision].x, vecMove[nCntCollision].z);

				//左側AND範囲内
				if ((vecLineUp.z * vecToPosUp.x) - (vecLineUp.x * vecToPosUp.z) <= 0.0f && (vecLineUp.z * vecToPosOldUp.x) - (vecLineUp.x * vecToPosOldUp.z) >= 0.0f)
				{
					if (fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] >= 0.0f && fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision] <= 1.0f)
					{
						//割合求める
						float fRate = fAreaAUp[nCntCollision] / fAreaBUp[nCntCollision];

						//反射処理
						RefrectPlayer(pPlayer, pFence, fRate, vecLineUp, collPos.pos1);

						//終了
						return true;
					}
				}
				else if ((vecLineDown.z * vecToPosDown.x) - (vecLineDown.x * vecToPosDown.z) <= 0.0f && (vecLineDown.z * vecToPosOldDown.x) - (vecLineDown.x * vecToPosOldDown.z) >= 0.0f)
				{
					if (fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] >= 0.0f && fAreaADown[nCntCollision] / fAreaBDown[nCntCollision] <= 1.0f)
					{
						//割合求める
						float fRate = fAreaADown[nCntCollision] / fAreaBDown[nCntCollision];

						//反射処理
						RefrectPlayer(pPlayer, pFence, fRate, vecLineDown, collPos.pos3);

						//終了
						return true;
					}
				}
			}
		}
	}
	return false;
}

//========================
//フェンスの衝突処理（HDRのプレイヤー限定	フェンスは倒しません）
//========================
void CollisionFence(Player_HDR *pPlayer)
{
	float fPosDiffX = powf(fabsf(pPlayer->pos.x), 2.0f);	//Ⅹ座標の絶対値の２乗を格納
	float fPosDiffZ = powf(fabsf(pPlayer->pos.z), 2.0f);	//Ｚ座標の絶対値の２乗を格納
	float fAngle = atan2f(pPlayer->pos.x, pPlayer->pos.z);	//角度も計算
	float fLength = sqrtf(fPosDiffX + fPosDiffZ);			//原点とプレイヤーの位置の直線距離の長さを格納
	float Fenceforward = FENCE_RADIUS - pPlayer->fBodySize;	//フェンスの手前の距離（フェンスを越えていたら、その地点に戻す

	//原点位置からの距離が、フェンスの設置半径を超えていたら
	if (Fenceforward <= fLength)
	{
		pPlayer->pos.x = sinf(fAngle) * Fenceforward;//超えた先の角度を基に、フェンスの半径位置より手前に戻す
		pPlayer->pos.z = cosf(fAngle) * Fenceforward;//超えた先の角度を基に、フェンスの半径位置より手前に戻す
		PrintDebugProc("フェンスの設置半径：%f  フェンスの手前：%f\n", FENCE_RADIUS, Fenceforward);
	}
}

//========================
//木の当たり判定処理
//========================
void CollisionObject_Tree(Player *pPlayer)
{
	//オブジェクトのポインタを取得
	Object *pTree = GetObjectInfo();

	for (int nCtObj = 0; nCtObj < MAX_OBJECT; nCtObj++, pTree++)
	{
		//対象のオブジェクトが「木」であり、木の高さ内に居る
		if (pTree->type == OBJECTTYPE_TREE && pPlayer->pos.y <= TREE_HEIGHT)
		{
			float PosDiffX = powf(pPlayer->pos.x - pTree->pos.x, 2.0f);	//Ⅹ座標の差分を計算
			float PosDiffZ = powf(pPlayer->pos.z - pTree->pos.z, 2.0f);	//Ｚ座標の差分を計算
			float fLength = sqrtf(PosDiffX + PosDiffZ);		//木とプレイヤーの直線距離を計算

			//直線距離が、当たり判定の距離をの内側にいる
			if (fLength <= TREE_RADIUS)
			{
				float fAngle = atan2f(pPlayer->pos.x - pTree->pos.x, pPlayer->pos.z - pTree->pos.z);//角度計算
				pPlayer->pos.x = pTree->pos.x + sinf(fAngle) * TREE_RADIUS;	//プレイヤーのＸ位置修正
				pPlayer->pos.z = pTree->pos.z + cosf(fAngle) * TREE_RADIUS;	//プレイヤーのＺ位置修正
				pPlayer->move.x = pPlayer->move.z *= TREE_REFRECT;			//移動量を反転させる			
				break;
			}
		}
	}
}

//========================
//当たり判定生成
//========================
void GenerateCollision(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CollisionPos *pCollision, float fWidth, float fDepth)
{
	//各頂点を求めるのに必要な変数
	D3DXMATRIX mtxWorld;	//回転のベースとなるワールドマトリ
	D3DXMATRIX mtxRot;		//回転行列
	D3DXMATRIX mtxTrans;	//すべて変換後の行列
	D3DXVECTOR3 vtxTrans;	//変換後の点

	//-mtx----------------------------------------------------------------------------------------------------------------------------
	//回転行列を作る
	D3DXMatrixIdentity(&mtxWorld);

	//向き反映
	D3DXMatrixRotationY(&mtxRot, rot.y);
	D3DXMatrixMultiply(&mtxWorld, &mtxRot, &mtxWorld);

	//位置反映
	mtxWorld._41 = pos.x;
	mtxWorld._42 = 0.0f;
	mtxWorld._43 = pos.z;

	//-mtx----------------------------------------------------------------------------------------------------------------------------

	//-pos0---------------------------------------------------------------------------------------------------------------------------
	//回転行列をもとに頂点を回転する
	//ベースをコピー
	mtxTrans = mtxWorld;

	//0度のときの点を置く
	D3DXVECTOR3 vtxPos = D3DXVECTOR3(-fWidth, 0.0f, -fDepth);

	//回転行列とかけ合わせる
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	pCollision->pos0.x = vtxTrans.x;
	pCollision->pos0.y = vtxTrans.y;
	pCollision->pos0.z = vtxTrans.z;
	//-pos0---------------------------------------------------------------------------------------------------------------------------

	//-pos1---------------------------------------------------------------------------------------------------------------------------
	//回転行列をもとに頂点を回転する
	//ベースをコピー
	mtxTrans = mtxWorld;

	//0度のときの点を置く
	vtxPos = D3DXVECTOR3(+fWidth, 0.0f, -fDepth);

	//回転行列とかけ合わせる
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//変換後の点の場所を代入
	pCollision->pos1.x = vtxTrans.x;
	pCollision->pos1.y = vtxTrans.y;
	pCollision->pos1.z = vtxTrans.z;
	//-pos1---------------------------------------------------------------------------------------------------------------------------

	//-pos2---------------------------------------------------------------------------------------------------------------------------
	//回転行列をもとに頂点を回転する
	//ベースをコピー
	mtxTrans = mtxWorld;

	//0度のときの点を置く
	vtxPos = D3DXVECTOR3(+fWidth, 0.0f, +fDepth);

	//回転行列とかけ合わせる
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//変換後の点の場所を代入
	pCollision->pos2.x = vtxTrans.x;
	pCollision->pos2.y = vtxTrans.y;
	pCollision->pos2.z = vtxTrans.z;
	//-pos2---------------------------------------------------------------------------------------------------------------------------

	//-pos3---------------------------------------------------------------------------------------------------------------------------
	//回転行列をもとに頂点を回転する
	//ベースをコピー
	mtxTrans = mtxWorld;

	//0度のときの点を置く
	vtxPos = D3DXVECTOR3(-fWidth, 0.0f, +fDepth);

	//回転行列とかけ合わせる
	D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

	//変換後の点の場所を代入
	pCollision->pos3.x = vtxTrans.x;
	pCollision->pos3.y = vtxTrans.y;
	pCollision->pos3.z = vtxTrans.z;
	//-pos3---------------------------------------------------------------------------------------------------------------------------
}

//========================
//反射処理
//========================
void RefrectPlayer(Player *pPlayer, Fence *pFence, float fRate, D3DXVECTOR3 vecLine, D3DXVECTOR3 vecStartPos)
{
	//ベクトルの割合求める
	//とりあえず交わっている頂点を求める
	D3DXVECTOR3 posCross = (vecLine * fRate) + vecStartPos;

	//頂点をもとにベクトル求める
	D3DXVECTOR3 vecCross = pPlayer->pos - posCross;

	//求めたベクトルをもとに割合求める
	float fRateRemaining = D3DXVec3Length(&vecCross) / D3DXVec3Length(&pPlayer->move);
	fRateRemaining = 1.0f - fRateRemaining;

	//一旦進めるだけ進める
	pPlayer->pos.x += vecCross.x;
	pPlayer->pos.z += vecCross.z;

	//移動量引く
	pPlayer->move.x *= fRateRemaining;
	pPlayer->move.z *= fRateRemaining;


	//移動量のベクトルを代入
	D3DXVECTOR3 vecMove = pPlayer->move;

	//フェンスの法線ベクトルを求める
	D3DXVECTOR3 vecNor;
	vecNor.x = -sinf(pFence->rot.y);
	vecNor.y = 0.0f;
	vecNor.z = -cosf(pFence->rot.y);

	//とりあえず正規化
	D3DXVec3Normalize(&vecMove, &vecMove);
	D3DXVec3Normalize(&vecNor, &vecNor);

	//ドット積（内積）求める
	float fDot = D3DXVec3Dot(&vecMove, &vecNor);

	//反射ベクトルを求める
	D3DXVECTOR3 vecRefrect = vecMove - 2 * fDot * vecNor;

	//実際の移動量を設定
	float fVecLength = D3DXVec3Length(&pPlayer->move);
	if (fVecLength > REFRECT_MAX)
	{
		fVecLength = REFRECT_MAX;
	}

	vecRefrect *= fVecLength / fRateRemaining;

	//移動量変更
	pPlayer->move.x = vecRefrect.x * REFRECT_WEAK;
	pPlayer->move.z = vecRefrect.z * REFRECT_WEAK;

	//ちゃんと移動する
	pPlayer->pos.x += pPlayer->move.x;
	pPlayer->pos.z += pPlayer->move.z;

	//振動
	SetPadVibration(pPlayer->nPlayerNum, VIBE_POWER_LEVEL_02, VIBE_TIME, VIBE_STATE_01_LOOP);

	//フェンスの当たり判定を消す
	pFence->bCollision = false;
}

//========================
//当たった時の処理（返り値は動いているかどうか）
//========================
bool HitPlayerToPlayer(Player *pATKPlayer, Player *pHitPlayer, D3DXVECTOR3 collPos, D3DXVECTOR3 vecLine, float fRate)
{
	bool bMove = false;
	if (fabsf(pATKPlayer->move.x) > 0.0f || fabsf(pATKPlayer->move.z) > 0.0f)
	{//動いてる
		//そのフレームで当たったプレイヤーにする
		pATKPlayer->nFrameAtkPlayer = pHitPlayer->nPlayerNum;

		//チャージゲージ減少と最後に当たったプレイヤーにする
		pHitPlayer->moveGauge *= CHARGE_DOWN;
		pHitPlayer->nLastHitPlayer = pATKPlayer->nPlayerNum;

		//振動
		SetPadVibration(pATKPlayer->nPlayerNum, VIBE_POWER_LEVEL_03, VIBE_TIME, VIBE_STATE_01_LOOP);
		SetPadVibration(pHitPlayer->nPlayerNum, VIBE_POWER_LEVEL_03, VIBE_TIME, VIBE_STATE_01_LOOP);

		bMove = true;
	}
	//1.0f = pushback
	pATKPlayer->pos.x = collPos.x + (vecLine.x * fRate) - sinf(pHitPlayer->rot.y) / D3DX_PI * 1.0f;
	pATKPlayer->pos.z = collPos.z + (vecLine.z * fRate) - cosf(pHitPlayer->rot.y) / D3DX_PI * 1.0f;

	//動いているか返す
	return bMove;
}