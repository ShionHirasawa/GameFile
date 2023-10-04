//================================================================================================
//
//フェード処理[fade.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "fade.h"

//フェードの優先順位
const int CFade::FADE_PRIORITY = 7;

//フェードするスピード
const float CFade::FADE_SPEED = 0.015f;

CScene::MODE	CFade::s_NextMode;			//次のモード
CFade::FADE		CFade::s_state;				//フェード状態
D3DXCOLOR		CFade::s_Color;				//フェードカラー（主にα値を使用する
D3DXCOLOR		CFade::s_WipeColor[VTX_MAX];//ワイプフェードカラー（主にα値を使用するが、各要素ごとに変化するタイミングは異なる
int				CFade::s_WipeCounter = 0;	//ワイプフェードのカウンター
int				CFade::s_nCounter = 0;

//ワイプフェードのタイミング
const int CFade::c_WipeFadeTiming[VTX_MAX] = {
	0,		//左上の頂点は最初から
	20, 20,	//右上、左下の頂点は、同じタイミング
	40		//右下の頂点は、一番遅れて
};

//=======================================
//コンストラクタ
//=======================================
CFade::CFade() : CObject2D(FADE_PRIORITY)
{
	//色を設定
	s_Color = GetColor(COL_BLACK);
	s_WipeColor[0] = s_WipeColor[1] = s_WipeColor[2] = s_WipeColor[3] = s_Color;

	//フェードイン状態
	s_state = FADE_IN;
	
	s_WipeCounter = 0;
	s_nCounter = 0;
}

//=======================================
//デストラクタ
//=======================================
CFade::~CFade()
{

}

//=======================================
//生成処理
//=======================================
CFade *CFade::Create(void)
{
	CFade *pFade = new CFade;

	if (pFade == NULL) return NULL;

	pFade->Init();

	return pFade;
}

//=======================================
//初期化処理
//=======================================
HRESULT CFade::Init(void)
{
	if (FAILED(CObject2D::Init(D3DXVECTOR3(HALF_WIDTH, HALF_HEIGHT, 0.0f), HALF_WIDTH, HALF_HEIGHT, s_Color, VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//失敗したらエラーを返す
		return E_FAIL;
	}

	//種類設定
	SetType(CObject::TYPE_FADE);

	//初期化成功
	return S_OK;
}

//=======================================
//終了処理
//=======================================
void CFade::Uninit(void)
{
	//頂点バッファの破棄など
	CObject2D::Uninit();
}

//=======================================
//更新処理
//=======================================
void CFade::Update(void)
{
	//フェードの状態で更新処理を分ける
	switch (s_state)
	{
		case FADE_IN: Update_In();	SetVertexInfo(s_Color); break;
		case FADE_OUT:Update_Out();	SetVertexInfo(s_Color); break;
		case FADE_WIPE_IN: 
			Update_WipeIn();
			for (int nCntWipe = 0; nCntWipe < VTX_MAX; nCntWipe++)
			{
				SetVertexInfo(s_WipeColor[nCntWipe], nCntWipe);
			}
			break;

		case FADE_WIPE_OUT:
			Update_WipeOut();
			for (int nCntWipe = 0; nCntWipe < VTX_MAX; nCntWipe++)
			{
				SetVertexInfo(s_WipeColor[nCntWipe], nCntWipe);
			}
			break;
	}
}

//=======================================
//描画処理
//=======================================
void CFade::Draw(void)
{
	if (s_state == FADE_NONE) return;

	//自分を描画
	CObject2D::Draw();
}

//=======================================
//設定処理
//=======================================
void CFade::Set(CScene::MODE modeNext, FADE FadeOutType)
{
	//フェード中なら設定しない
	if (s_state != FADE_NONE || FadeOutType != FADE_OUT && FadeOutType != FADE_WIPE_OUT) return;

	if (FadeOutType == FADE_OUT || FadeOutType == FADE_WIPE_OUT)
	{
		s_state = FadeOutType;	//フェードアウト状態に
		s_NextMode = modeNext;	//次の画面(モード)を設定

		if(FadeOutType == FADE_OUT)	s_Color = GetColor(COL_CLEAR);	//黒いポリゴン(透明)にしておく
		else
		{
			//黒いポリゴン(透明)にしておく
			s_WipeColor[0] = s_WipeColor[1] = s_WipeColor[2] = s_WipeColor[3] = GetColor(COL_CLEAR);
			s_WipeCounter = 0;
		}

		s_nCounter++;
	}
}

//=======================================
//フェードイン中の更新
//=======================================
void CFade::Update_In(void)
{
	//ポリゴンを透明にしていく
	s_Color.a -= FADE_SPEED;

	//透明になった
	if (s_Color.a <= 0.0f)
	{
		s_Color.a = 0.0f;
		s_state = FADE_NONE;	//何もしていない状態に
	}
}

//=======================================
//フェードアウト中の更新
//=======================================
void CFade::Update_Out(void)
{
	//ポリゴンを不透明にしていく
	s_Color.a += FADE_SPEED;

	//完全にフェードした
	if (s_Color.a >= 1.0f)
	{
		s_Color.a = 1.0f;
		s_state = FADE_IN;	//フェードイン状態に

		CManager::SetMode(s_NextMode);//モード設定(次の画面に移行)
	}
}

//=======================================
//ワイプフェードイン中の更新
//=======================================
void CFade::Update_WipeIn(void)
{
	//ワイプカウンターを増やす
	s_WipeCounter++;

	for (int nCntFade = 0; nCntFade < VTX_MAX; nCntFade++)
	{
		//まだフェードするタイミングで無いのなら、次のフェードへ
		if (s_WipeCounter < c_WipeFadeTiming[nCntFade]) continue;

		//透明にしていく
		s_WipeColor[nCntFade].a -= FADE_SPEED;

		//α値が０を下回ったら０に戻す
		if (s_WipeColor[nCntFade].a < 0.0f) s_WipeColor[nCntFade].a = 0.0f;
	}

	//まだ全部が透明になっていない
	if (s_WipeColor[0].a > 0.0f || s_WipeColor[1].a > 0.0f || s_WipeColor[2].a > 0.0f || s_WipeColor[3].a > 0.0f) return;

	s_state = FADE_NONE;//何もしていない状態に
	s_WipeCounter = 0;	//カウンター初期化
}

//=======================================
//ワイプフェードアウト中の更新
//=======================================
void CFade::Update_WipeOut(void)
{
	//ワイプカウンターを増やす
	s_WipeCounter++;

	for (int nCntFade = 0; nCntFade < VTX_MAX; nCntFade++)
	{
		//まだフェードするタイミングで無いのなら、次のフェードへ
		if (s_WipeCounter < c_WipeFadeTiming[nCntFade]) continue;

		//透明にしていく
		s_WipeColor[nCntFade].a += FADE_SPEED;

		//α値が１を上回ったら１に戻す
		if (s_WipeColor[nCntFade].a > 1.0f) s_WipeColor[nCntFade].a = 1.0f;
	}

	//まだ全部が不透明になっていない
	if (s_WipeColor[0].a < 1.0f || s_WipeColor[1].a < 1.0f || s_WipeColor[2].a < 1.0f || s_WipeColor[3].a < 1.0f) return;

	s_state = FADE_WIPE_IN;			//フェードイン状態に
	s_WipeCounter = 0;				//カウンター初期化
	CManager::SetMode(s_NextMode);	//モード設定(次の画面に移行)
}