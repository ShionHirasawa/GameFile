//================================================================================================
//
//モデル管理処理[model.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//モデルの総数
#define MAX_PARTS		(20)

//モデルクラス
class CModel
{
public:
	CModel();
	~CModel();

	HRESULT Init(const char *pFilePath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModel *Create(const char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetParent(CModel *pModel);
	D3DXMATRIX GetMtxWorld(void);

	HRESULT LoadModelParts(const char *pFilePath);

	void OffSetTranslate(D3DXVECTOR3 Setpos, D3DXVECTOR3 SetRot) { m_pos = m_posOffSet + Setpos; m_rot = m_rotOffSet + SetRot; }

private:
	void InitSetPosRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	LPD3DXMESH		m_pMesh;		//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER	m_pBuffMat;		//マテリアルのポインタ
	DWORD			m_dwNumMat;		//マテリアルの数
	int				*m_pTextureID;	//テクスチャIDのポインタ

	D3DXVECTOR3 m_pos, m_posOffSet;	//現在地、基準位置
	D3DXVECTOR3 m_rot, m_rotOffSet;	//現在角度、基準角度
	D3DXMATRIX	m_mtxWorld;
	CModel *m_pParent;
};

#endif