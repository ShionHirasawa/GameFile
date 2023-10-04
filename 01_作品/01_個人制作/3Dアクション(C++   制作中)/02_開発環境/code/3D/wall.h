//================================================================================================
//
//壁処理[wall.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _WALL_H
#define _WALL_H

//壁クラス
class CWall : public CObject3D
{
public:
	CWall();
	~CWall();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT Load(void);

	static CWall *Create(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const D3DXVECTOR2 texSize);
	static bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, float fMargin, float fHeight);

	//==========================================
	//対象までの間に壁があるかどうか判定
	//引数１　始点位置
	//引数２　終点位置
	//返り値：自分と目標の間に壁があるかどうか　true：壁がある　false：壁はない
	//==========================================
	static bool IsAmongWall(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos);

private:
	int	m_nTextureID;	// テクスチャ番号
};

#endif