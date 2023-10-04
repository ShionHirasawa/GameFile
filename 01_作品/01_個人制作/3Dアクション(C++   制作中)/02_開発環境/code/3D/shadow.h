//================================================================================================
//
//影処理[shadow.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//前方宣言
class CPoly3D;

//影クラス
class CShadow
{
public:
	CShadow();
	~CShadow();

	HRESULT Init(void) { return S_OK; }
	HRESULT Init(D3DXVECTOR3 pos, float fSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static int Create(D3DXVECTOR3 pos, float fSize);
	static void SetPos(int nID, D3DXVECTOR3 pos);
	static void SetSize(int nID, float fSize);

	//-------------------------
	//特定の影削除
	//※引数を設定しない場合、影全削除
	//-------------------------
	//引数　自分の影の番号
	//-------------------------
	static void Delete(int nID);

	//-------------------------
	//影を全て削除
	//-------------------------
	static void Delete(void);

	//-------------------------
	//影の描画切り替え
	//-------------------------
	static void DispSwitch(int nID, bool bDisp);

private:
	static const D3DXCOLOR SHADOW_COLOR;
	static int s_nNumAll;
	static char *SHADOW_TEX_PATH;
	static CShadow *s_pTop;
	static CShadow *s_pCur;

	CShadow *m_pNextShadow;//次の影オブジェクトのポインタ
	CPoly3D *m_pPoly3D;	//自分のポインタ
	int		m_nID;		//自分の番号
};

#endif