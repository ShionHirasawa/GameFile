//================================================================================================
//
//チュートリアル画面処理[tutorial.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//説明テクスチャの総数
#define NUM_CONTROLL_TEXTURE	(2)

//オブジェクトの個性を説明するテクスチャの総数
#define NUM_OBJ_IDENTITY_TEXTURE (4)

//前方宣言
class CPlayer;
class CPoly2D;
class CPolyBill;

//チュートリアルクラス
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//操作説明の構造体
	struct Controll
	{
		CPoly2D		*pPoly2D;	// ポリゴン情報
		int			nTexID;		// 現在描画しているテクスチャ番号
		bool		bDispChange;// 表示方法切り替え　true：中央に表示  false：左上に小さく表示
		D3DXVECTOR3 pos;		// 説明テクスチャの位置
		float		fWidth;		// 説明テクスチャの幅
		float		fHeight;	// 説明テクスチャの高さ
	};

	//オブジェクトの個性説明の構造体
	struct ObjIdentity
	{
		CPolyBill	*pPolyBill;	// ポリゴン情報
		D3DXVECTOR3 pos;		// 説明テクスチャの位置
		float		fWidth;		// 説明テクスチャの幅
		float		fHeight;	// 説明テクスチャの高さ
		float		fTexV;		// 説明テクスチャのテクスチャＶ座標
	};

	//操作説明テクスチャ情報
	/*中央表示の位置*/ static const D3DXVECTOR3 CONTROLL_TEX_CENTER_POS;
	/*中央表示の幅	*/ static const float		CONTROLL_TEX_CENTER_WIDTH;
	/*中央表示の高さ*/ static const float		CONTROLL_TEX_CENTER_HEIGHT;
	/*左上ワイプ表示の位置*/ static const D3DXVECTOR3 CONTROLL_TEX_WIPE_POS;
	/*左上ワイプ表示の幅  */ static const float		  CONTROLL_TEX_WIPE_WIDTH;
	/*左上ワイプ表示の高さ*/ static const float		  CONTROLL_TEX_WIPE_HEIGHT;

	//オブジェクトの個性説明テクスチャ情報
	/*近くで見る時の幅	*/ static const float OBJ_IDENTITY_TEX_NEAR_WIDTH;
	/*近くで見る時の高さ*/ static const float OBJ_IDENTITY_TEX_NEAR_HEIGHT;
	/*近くで見る時のＹ座標*/static const float OBJ_IDENTITY_NEAR_SET_POS_Y;

	/*遠くで見る時の幅	*/ static const float OBJ_IDENTITY_TEX_FAR_WIDTH;
	/*遠くで見る時の高さ*/ static const float OBJ_IDENTITY_TEX_FAR_HEIGHT;
	/*遠くで見る時のＹ座標*/static const float OBJ_IDENTITY_FAR_SET_POS_Y;
	/*遠くで見る時のテクスチャＹ座標*/static const float OBJ_IDENTITY_FAR_TEX_Y;

	/*サイズ変化させる距離*/ static const float OBJ_IDENTITY_LENGTH;

	/*配置座標*/	static const D3DXVECTOR3 OBJ_IDENTITY_SET_POS[NUM_OBJ_IDENTITY_TEXTURE];

	static char *s_pControllTexturePath[NUM_CONTROLL_TEXTURE];	//操作説明テクスチャのファイルパス
	static char *s_pObjectTexturePath[NUM_OBJ_IDENTITY_TEXTURE];//オブジェクトの個性説明テクスチャのパス

	CPlayer *m_pPlayer;

	void SetStage(void);
	void TextureLoad(void);
	void SwapControllTexture(void);	//操作説明ポリゴン切り替え
	void SwitchDispPoly2D(void);	//操作説明ポリゴンの表示ON/OFF
	void UpdateObjIdentity(void);	//個性説明の更新処理

	void End(void);					//チュートリアル終了

	ObjIdentity	m_ObjIdentity[NUM_OBJ_IDENTITY_TEXTURE];
	Controll	m_Controll;
};

#endif