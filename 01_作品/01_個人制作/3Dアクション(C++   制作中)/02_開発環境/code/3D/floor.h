//================================================================================================
//
//床処理[floor.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _FLOOR_H
#define _FLOOR_H

//床クラス
class CFloor : public CObject3D
{
public:
	CFloor();
	~CFloor();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	HRESULT Load(char *pPath);

	static CFloor *Create(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const D3DXVECTOR2 texSize);

	static bool LandField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

private:
	static int	m_nTextureID;	// テクスチャ番号
	float m_fLandHeight;	//現在の着地点
	float m_fLandHeightOld;	//前回の着地点
	float m_fAddHeight;		//頂点１・２を上げ下げする変数
	float m_fAddHeightOld;	//前回の上げ下げする変数の値
};

#endif