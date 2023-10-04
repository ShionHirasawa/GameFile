//================================================================================================
//
//オブジェクト処理[object.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/camera.h"
#include "object.h"

#define DEF_OBJ_PRIORITY	(3)		//基準の優先順位
#define MAX_OBJ_PRIORITY	(8)		//最高の優先順位

//静的メンバ変数宣言
CObject *CObject::m_pTop[MAX_PRIORITY] = {};	//先頭のポインタ
CObject *CObject::m_pCur[MAX_PRIORITY] = {};	//最後尾のポインタ

const int CObject::OBJECT_PRIORITY_DEF = DEF_OBJ_PRIORITY;
const int CObject::OBJECT_PRIORIRY_MAX = MAX_OBJ_PRIORITY;
int		 CObject::m_nNumAll[MAX_PRIORITY] = {};

//=======================================
//コンストラクタ
//=======================================
CObject::CObject(int nPriority)
{
	/*自分の優先順位を保存*/	m_nPriority = nPriority;
	/*     種類クリア     */	m_Type = TYPE_NONE;
	/* 総数カウントアップ */	m_nNumAll[nPriority]++;
	/*前のポインタをNULLに*/	m_pPrev = NULL;
	/*次のポインタをNULLに*/	m_pNext = NULL;
	/*  死亡フラグをOFFに */	m_bDeath = false;

	//先頭がまだ設定されていない
	if (m_pTop[nPriority] == NULL)
	{
		//自分を代入する
		m_pTop[nPriority] = this;
	}

	//最後尾が設定されている
	if (m_pCur[nPriority] != NULL)
	{
		//現在の最後尾を保存
		CObject *pCurLast = m_pCur[nPriority];

		//現在の最後尾の次のポインタに自分を代入する
		m_pCur[nPriority]->m_pNext = this;

		//さっきまでの最後尾を、最新の最後尾の前に設定
		this->m_pPrev = pCurLast;
	}

	//最後尾を自分にする
	m_pCur[nPriority] = this;
}

//=======================================
//デストラクタ
//=======================================
CObject::~CObject()
{

}

//=======================================
//全削除
//=======================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//最初に行うオブジェクトを代入する
		CObject *pObj = m_pTop[nCntPriority];

		//処理を行うオブジェクトがいなくなるまで無限ループ
		while (pObj != NULL)
		{
			//次に処理を行うオブジェクトを保存しておく
			CObject *pObjNext = pObj->m_pNext;

			//死亡フラグを立てる
			pObj->m_bDeath = true;

			//自分の終了処理
			pObj->Uninit();

			//次に処理を行うオブジェクトを代入する
			pObj = pObjNext;
		}
	}
}

//=======================================
//特定の種類のオブジェクト全削除
//=======================================
void CObject::ReleaseAll(CObject::TYPE type)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//最初に行うオブジェクトを代入する
		CObject *pObj = m_pTop[nCntPriority];

		//処理を行うオブジェクトがいなくなるまで無限ループ
		while (pObj != NULL)
		{
			//次に処理を行うオブジェクトを保存しておく
			CObject *pObjNext = pObj->m_pNext;

			//自分が消される種類のオブジェクトかどうか
			if (pObj->m_Type == type)
			{
				pObj->m_bDeath = true;	//死亡フラグを立てる
				pObj->Uninit();			//自分の終了処理
			}

			//次に処理を行うオブジェクトを代入する
			pObj = pObjNext;
		}
	}
}

//=======================================
//特定の種類以外のオブジェクト全削除
//=======================================
void CObject::ExceptReleaseAll(CObject::TYPE type)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//最初に行うオブジェクトを代入する
		CObject *pObj = m_pTop[nCntPriority];

		//処理を行うオブジェクトがいなくなるまで無限ループ
		while (pObj != NULL)
		{
			//次に処理を行うオブジェクトを保存しておく
			CObject *pObjNext = pObj->m_pNext;

			//保護されるオブジェクトではない
			if (pObj->m_Type != type)
			{
				pObj->m_bDeath = true;	//死亡フラグを立てる
				pObj->Uninit();			//自分の終了処理
			}

			//次に処理を行うオブジェクトを代入する
			pObj = pObjNext;
		}
	}
}

//=======================================
//全更新
//=======================================
void CObject::UpdateAll(void)
{
	//優先順位が低い順に更新していく
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//最初に行うオブジェクトを代入する
		CObject *pObj = m_pTop[nCntPriority];

		//処理を行うオブジェクトがいなくなるまで無限ループ
		while (pObj != NULL)
		{
			//次に処理を行うオブジェクトを保存しておく
			CObject *pObjNext = pObj->m_pNext;

			//自分に死亡フラグが立っていない
			if (!pObj->m_bDeath)
			{
				//自分の更新処理
				pObj->Update();
			}

			//次に処理を行うオブジェクトを代入する
			pObj = pObjNext;
		}
	}

	//優先順位が低い順に破棄していく
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//最初に行うオブジェクトを代入する
		CObject *pObj = m_pTop[nCntPriority];

		//処理を行うオブジェクトがいなくなるまで無限ループ
		while (pObj != NULL)
		{
			//次に処理を行うオブジェクトを保存しておく
			CObject *pObjNext = pObj->m_pNext;

			//自分の死亡フラグが立っている
			if (pObj->m_bDeath)
			{
				//終了処理
				pObj->Uninit();
			}

			//次に処理を行うオブジェクトを代入する
			pObj = pObjNext;
		}
	}
}

//=======================================
//全描画
//=======================================
void CObject::DrawAll(void)
{
	//カメラ取得
	CCamera *pCamera = CManager::GetCamera();

	//カメラを設定
	pCamera->Set();

	//優先順位が低い順に描画していく
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
	{
		//最初に行うオブジェクトを代入する
		CObject *pObj = m_pTop[nCntPriority];

		//処理を行うオブジェクトがいなくなるまで無限ループ
		while (pObj != NULL)
		{
			//自分の更新処理
			pObj->Draw();

			//次に処理を行うオブジェクトを代入する
			pObj = pObj->m_pNext;
		}
	}
}

//=======================================
//削除
//=======================================
void CObject::Release(void)
{
	//まだ自分の死亡フラグが立っていない
	if (m_bDeath == false)
	{
		//死亡フラグを立てて終了
		m_bDeath = true;
		return;
	}

	//死亡フラグが立っていれば破棄する
	//前後のオブジェクトのポインタを保存
	CObject *pPrev = m_pPrev;
	CObject *pNext = m_pNext;

	//自分の番号を保存
	int nPriority = m_nPriority;

	//次のオブジェクトが存在する
	if (m_pNext != NULL)
	{
		//次のオブジェクトの前は、自分の前のやつ
		m_pNext->m_pPrev = pPrev;

		//次のオブジェクトの前がいなかったら
		if (m_pNext->m_pPrev == NULL)
		{
			//先頭は次のオブジェクト
			m_pTop[nPriority] = m_pNext;
		}
	}
	//次のオブジェクトが存在しない（自分が最後尾だった
	else
	{
		//自分の前のオブジェクトを最後尾にする（もしくはNULLにする
		m_pCur[nPriority] = m_pPrev;
	}

	//前のオブジェクトが存在する
	if (m_pPrev != NULL)
	{
		//前のオブジェクトの次は、自分の次のやつ
		m_pPrev->m_pNext = pNext;

		//前のオブジェクトの次がいなかったら
		if (m_pPrev->m_pNext == NULL)
		{
			//最後尾は前のオブジェクト
			m_pCur[nPriority] = m_pPrev;
		}
	}
	//前のオブジェクトが存在しない（自分が先頭だった
	else
	{
		//自分の次のオブジェクトを先頭にする（もしくはNULLにする
		m_pTop[nPriority] = m_pNext;
	}

	//メモリ開放
	CObject *pMy = this;
	delete pMy;
	pMy = NULL;
	m_nNumAll[nPriority]--;//総数を減らす
}

//=======================================
//オブジェクトを取得
//=======================================
CObject *CObject::GetObject(const int nPriority, const int nIdx)
{
	if (0 <= nPriority && nPriority < MAX_PRIORITY &&	//優先番号が範囲内
		0 <= nIdx && nIdx < m_nNumAll[nPriority])		//オブジェクトの番号が範囲内
	{
		//先頭番号を保存
		CObject *pObj = m_pTop[nPriority];

		//取得したい番号のオブジェクトまで次のオブジェクトを代入し続ける
		for (int nCntObj = 0; nCntObj < nIdx; nCntObj++)
		{
			//オブジェクトのポインタが存在する
			if (pObj != NULL)
			{
				//次のオブジェクトを代入する
				pObj = pObj->m_pNext;
			}
		}

		//オブジェクトを返す
		return pObj;
	}

	//０未満だったらNULLを返す
	return NULL;
}