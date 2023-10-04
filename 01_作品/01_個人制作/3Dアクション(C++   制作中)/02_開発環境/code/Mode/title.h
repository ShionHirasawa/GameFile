//================================================================================================
//
//タイトル画面処理[title.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "../2D/object2D.h"
#include "../2D/Polygon2D.h"

//タイトルクラス
class CTitle : public CScene
{
public:
	CTitle();
	virtual ~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//====================
	//静的メンバ定数
	//====================
	static char *LOGO_TEXTURE_PATH;		//ロゴのテクスチャパス
	static const D3DXVECTOR3 LOGO_POS;	//ロゴの基準位置
	static const float LOGO_WIDTH;		//ロゴの幅
	static const float LOGO_HEIGHT;		//ロゴの高さ

	static char *PRESS_TEXTURE_PATH;	//プレス表示のテクスチャパス
	static const D3DXVECTOR3 PRESS_POS;	//プレス表示の基準位置
	static const float PRESS_WIDTH;		//プレス表示の幅
	static const float PRESS_HEIGHT;	//プレス表示の高さ

	CPoly2D	*m_pLogo;	//ロゴのポインタ
	CPoly2D *m_pPress;	//プレス表示のUI

	static const int FLASH_TIME;	//点滅間隔
	static const float FLASH_SPEED;	//点滅スピード
	D3DXCOLOR m_PressColor;			//プレス表示UIの色（透明度だけ
	int m_nFlashCounter;			//プレス表示UIの点滅カウンター
};

#endif