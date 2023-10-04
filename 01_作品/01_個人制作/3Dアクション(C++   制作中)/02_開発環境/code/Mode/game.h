//================================================================================================
//
//ゲーム画面処理[game.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

//ゲームクラス
class CGame : public CScene
{
public:
	CGame();
	virtual ~CGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
private:
	//====================
	//静的メンバ定数
	//====================
	static const char	*FILENAME_INTERIOR;	//内装情報ファイル名
	static const char	*FILENAME_OBJECT;	//オブジェクト情報ファイル名
	static const char	*FILENAME_ENEMY;	//敵の配置情報ファイル名
	static const int	SET_NUM_ITEM;		//アイテムの配置数

	void TextureLoad(void);		//オブジェクトのテクスチャ読み込み
	void SetItem(void);			//アイテムの配置
	void SetInterior(CFile *pFile);	//ステージの内装配置
	void SetObject(CFile *pFile);	//オブジェクトの配置
	void SetEnemy(CFile *pFile);	//敵の配置
};

#endif