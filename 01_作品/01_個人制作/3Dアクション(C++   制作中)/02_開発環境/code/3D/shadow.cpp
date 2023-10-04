//================================================================================================
//
//影処理[shadow.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "Polygon3D.h"
#include "shadow.h"

const D3DXCOLOR CShadow::SHADOW_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
char	*CShadow::SHADOW_TEX_PATH = "data\\TEXTURE\\effect.jpg";
int		 CShadow::s_nNumAll = 0;
CShadow *CShadow::s_pTop = NULL;
CShadow *CShadow::s_pCur = NULL;

//=======================================
//コンストラクタ
//=======================================
CShadow::CShadow()
{
	//メンバ変数クリア
	m_pPoly3D = NULL;

	//自分のIDを設定し、総数を増加
	m_nID = s_nNumAll++;

	//先頭のポインタが無ければ、自分を先頭とする
	if (s_pTop == NULL)
	{
		s_pTop = this;
	}

	//最後尾が設定されている場合
	if (s_pCur != NULL)
	{
		//最後尾の次を自分とする
		s_pCur->m_pNextShadow = this;
	}

	//最後尾を自分とする
	s_pCur = this;
}

//=======================================
//デストラクタ
//=======================================
CShadow::~CShadow()
{
	//総数減少
	s_nNumAll--;
}

//=======================================
//生成処理
//=======================================
int CShadow::Create(D3DXVECTOR3 pos, float fSize)
{
	//影インスタンス生成
	CShadow *pShadow = new CShadow;

	//生成完了
	if (pShadow != NULL)
	{
		//影ポリゴン初期化
		pShadow->Init(pos, fSize);
	}

	//インスタンスを返す
	return pShadow->m_nID;
}

//=======================================
//初期化処理
//=======================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, float fSize)
{
	//影ポリゴン設定
	m_pPoly3D = CPoly3D::Create(SHADOW_TEX_PATH);
	m_pPoly3D->SetInfo(pos, fSize, 0.0f, fSize);
	m_pPoly3D->SetVertexInfo(SHADOW_COLOR);
	m_pPoly3D->SetSubBlend(true);
	m_pPoly3D->SetAlphaTest(true);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CShadow::Uninit(void)
{
	//自分を複製
	CShadow *pShadow = this;

	//自分の次を保存
	CShadow *pNext = pShadow->m_pNextShadow;

	//メモリ開放
	delete pShadow;
	pShadow = NULL;

	//自分の次がいなかった（=自分が最後尾）
	if (pNext == NULL)
	{
		//最後尾をNULLに
		s_pCur = NULL;
	}
}

//=======================================
//更新処理
//=======================================
void CShadow::Update(void)
{

}

//=======================================
//描画処理
//=======================================
void CShadow::Draw(void)
{

}

//=======================================
//位置設定処理
//=======================================
void CShadow::SetPos(int nID, D3DXVECTOR3 pos)
{
	//影が存在していない  or  引数の番号が総数以上  or  先頭が設定されていなければ、引き返す
	if (s_nNumAll <= 0 || s_nNumAll <= nID ||
		s_pTop == NULL) return;

	//先頭を格納
	CShadow *pShadow = s_pTop;

	for(int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//影の番号と引数の番号が一致している
		if (pShadow->m_nID == nID)
		{
			//位置設定
			pos.y = 0.0f;
			pShadow->m_pPoly3D->SetPosition(pos);
			pShadow->Update();
			break;
		}

		//次のポインタを設定する
		pShadow = pShadow->m_pNextShadow;
	}
}

//=======================================
//影を全て削除
//=======================================
void CShadow::Delete(void)
{
	//影が存在していない  or  先頭が設定されていなければ、引き返す
	if (s_nNumAll <= 0 || s_pTop == NULL) return;

	//先頭を格納
	CShadow *pShadow = s_pTop;

	for (int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//影のポインタが無い
		if (pShadow == NULL) break;

		//次の影を保存
		CShadow *pNext = pShadow->m_pNextShadow;

		//終了処理
		pShadow->Uninit();
		pShadow = NULL;

		//次の影を代入
		s_pTop = pShadow = pNext;
	}
}

//=======================================
//影削除処理
//=======================================
void CShadow::Delete(int nID)
{
	//影が存在していない  or  引数の番号が総数以上  or  先頭が設定されていなければ、引き返す
	if (s_nNumAll <= 0 || s_pTop == NULL) return;

	//先頭を格納
	CShadow *pShadow = s_pTop;

	//前の影を保存
	CShadow *pPrev = NULL;

	for (int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//次の影を保存
		CShadow *pNext = pShadow->m_pNextShadow;

		//影の番号と引数の番号が一致している
		if (pShadow->m_nID == nID)
		{
			//前回記憶した影が存在する
			if (pPrev != NULL)
			{
				//自分の前後の影を繋ぐ
				pPrev->m_pNextShadow = pShadow->m_pNextShadow;
			}
			//前回記憶した影が存在しない（=自分が先頭)の場合
			else
			{
				//自分の次のやつを先頭にする
				s_pTop = pShadow->m_pNextShadow;
			}

			//終了処理
			pShadow->Uninit();
			pShadow = NULL;
			break;
		}

		//今の影を保存
		pPrev = pShadow;

		//次のポインタを設定する
		pShadow = pNext;
	}
}

//=======================================
//位置設定処理
//=======================================
void CShadow::DispSwitch(int nID, bool bDisp)
{
	//影が存在していない  or  引数の番号が総数以上  or  先頭が設定されていなければ、引き返す
	if (s_nNumAll <= 0 || s_nNumAll <= nID ||
		s_pTop == NULL) return;

	//先頭を格納
	CShadow *pShadow = s_pTop;

	for (int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//影の番号と引数の番号が一致している
		if (pShadow->m_nID == nID)
		{
			//位置設定
			pShadow->m_pPoly3D->DispSwitch(bDisp);
			break;
		}

		//次のポインタを設定する
		pShadow = pShadow->m_pNextShadow;
	}
}

//=======================================
//影のサイズ設定
//=======================================
void CShadow::SetSize(int nID, float fSize)
{
	//影が存在していない  or  引数の番号が総数以上  or  先頭が設定されていなければ、引き返す
	if (s_nNumAll <= 0 || s_nNumAll <= nID ||
		s_pTop == NULL) return;

	//先頭を格納
	CShadow *pShadow = s_pTop;

	for (int nCntShadow = 0; nCntShadow < s_nNumAll; nCntShadow++)
	{
		//影の番号と引数の番号が一致している
		if (pShadow->m_nID == nID)
		{
			//サイズ設定
			pShadow->m_pPoly3D->SetVertexInfo(fSize, 0.0f, fSize);
			break;
		}

		//次のポインタを設定する
		pShadow = pShadow->m_pNextShadow;
	}
}