//================================================================================================
//
//ビルボード処理[billboard.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "../Main/manager.h"
#include "../Object/object.h"

//ビルボードクラス
class CObjectBillboard : public CObject
{
public:
	CObjectBillboard(const int nPriority = CObject::OBJECT_PRIORITY_DEF);
	~CObjectBillboard();

	HRESULT Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//サイズを設定する処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void SetWidth(const float fWidth)	 { m_fWidth = fWidth; }
	void SetHeight(const float fHeight)  { m_fHeight = fHeight; }
	void SetDepth(const float fDepth)	 { m_fDepth = fDepth; }
	void SetSize(const D3DXVECTOR3 size) { m_fWidth = size.x; m_fHeight = size.y; m_fDepth = size.z; }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//サイズを取得する処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	D3DXVECTOR3 GetSize3D(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth); }

	//====================================================
	// D3DXVECTOR3 の変数を設定する処理
	//====================================================
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	void SetMoving(D3DXVECTOR3 move) { m_move = move; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetTransrate(const D3DXVECTOR3 pos, const  D3DXVECTOR3 move, const  D3DXVECTOR3 rot) { SetPosition(pos); SetMoving(move); SetRotation(rot); }

	//====================================================
	// D3DXVECTOR3 の変数を取得する処理
	//====================================================
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	D3DXVECTOR3 GetMoving(void) { return m_move; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	void GetTransrate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot) { *pPos = GetPosition(); *pMove = GetMoving(); *pRot = GetRotation(); }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	//移動にまつわる処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++
	void UpdatePosOld(void) { m_posOld = m_pos; }
	void Move(void)			{ m_pos += m_move; }
	void MoveAttenuate(float fMagAttenuate) { m_move += (VEC3_INIT - m_move) * fMagAttenuate; }//移動量を減衰（減衰係数は任意で設定可能
	void MoveStop(void) { m_move = VEC3_INIT; }

	//====================================================
	//各種頂点情報を設定する処理
	//====================================================
	/*頂点カラーを変える*/		HRESULT SetVertexInfo(const D3DXCOLOR col);
	/*頂点座標を変える*/		HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth);
	/*頂点座標・カラーを変える*/HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col);
	/*テクスチャ座標を変える*/	HRESULT SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);
	/*頂点・Tex座標を変える*/	HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);
	/*頂点情報３種を変える*/	HRESULT SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight);

	//====================================================
	//各種頂点情報を取得する処理
	//====================================================
	/*座標を取得*/	 HRESULT GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID);

	/*テクスチャ割り当て*/	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	/*Ｚテスト無効設定*/	virtual void ZTestSwitch(bool bZTest) { m_bZTest = bZTest; }

private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;	// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファ
	D3DXMATRIX				m_mtxWorld;			// ワールドマトリックス

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_posOld;
	D3DXCOLOR	m_col;

	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	float m_fDepth;		//奥行き

	static int	m_nTextureID;	// テクスチャ番号
	bool m_bZTest;	//描画時にZテストを無効にするかどうか　　true：無効にする  false：無効にしない
};

#endif 