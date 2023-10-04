//================================================================================================
//
//プレイヤー処理[player.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../Model/model.h"
#include "../Model/motion.h"
#include "../XFile/objectX.h"

//収納するⅩオブジェクトの数
#define NUM_TAKE_OBJECTX	(3)

//ライフの桁数
#define NUM_LIFE_DEJIT		(3)

//前方宣言
class CPoly2D;
class CShadow;

//プレイヤークラス
class CPlayer : public CObject
{
public:
	/*プレイヤーのポリゴン左右余白*/ static const float WIDTH_MARGIN;
	/*プレイヤーのポリゴン上下余白*/ static const float HEIGHT_MARGIN;
	/*プレイヤーの当たり判定範囲*/	 static const float COLLISION_MARGIN;
	/*プレイヤーの当たり判定の長さ*/ static const float COLLISION_LENGTH;
	/*プレイヤーの優先順位*/static const int PRIORITY;

	//プレイヤーの状態列挙
	enum STATE{
		STATE_NORMAL = 0,	//通常状態
		STATE_DAMAGE,		//ダメージ状態
		STATE_DEATH,		//死亡状態
		STATE_MAX
	};

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(void);
	void SetPos(const D3DXVECTOR3 pos);

	void GiveDamege(int nDamage = 1);
	
	//取得系
	STATE GetState(void)			{ return m_State; }
	int GetTransformID(void)		{ return m_nTransformModelID; }
	int GetDamageInterval(void)		{ return m_nDamageInterval; }
	D3DXVECTOR3 GetPosition(void)	{ return m_pos; }
	D3DXVECTOR3 GetMoving(void)		{ return m_move; }
	float GetMoveLength(void)		{ return D3DXVec3Length(&m_move); }

	//チュートリアル限定の操作ON/OFF
	void IsTutorialControll(bool bControll);

private:
	static const int MAX_LIFE;			//最大体力
	static const int DAMAGE_INTERVAL;	//食らい硬直
	static const int DEATH_INTERVAL;	//死亡インターバル
	static const int SWITCH_DISP;		//描画ON/OFF切り替え時間
	static const float PUT_OBJX_LENGTH;	//Ⅹオブジェクトを取り出す長さ
	static const float NIMBLE_SPEED;	//個性「俊敏」の移動量
	static const float TOUGH_SPEED;		//個性「強靭」の移動量

	static const D3DXVECTOR3 LIFE_BG_POS;	//体力背景の基準位置
	static const float		 LIFE_BG_WIDTH;	//体力背景の幅
	static const float		 LIFE_BG_HEIGHT;//体力背景の高さ
	static const D3DXCOLOR	 LIFE_BG_COLOR;	//体力背景の色

	static char	*LOGO_TEXTURE_PATH;				//体力ロゴのテクスチャ
	static const D3DXVECTOR3 LIFE_LOGO_POS;		//体力ロゴの基準位置
	static const float		 LIFE_LOGO_WIDTH;	//体力ロゴの幅
	static const float		 LIFE_LOGO_HEIGHT;	//体力ロゴの高さ

	static char	*LIFE_TEXTURE_PATH;		//体力を表すテクスチャ
	static const D3DXVECTOR3 LIFE_POS;	//体力ポリゴンの基準位置
	static const float LIFE_WIDTH;		//体力ポリゴンの幅
	static const float LIFE_HEIGHT;		//体力ポリゴンの高さ
	static const float LIFE_MARGIN;		//体力ポリゴンの隣との間
	static const float LIFE_TEX_WIDTH;	//体力テクスチャの幅
	static const float LIFE_TEX_HEIGHT;	//体力テクスチャの高さ

	static const float SCALING_SPEED;	//サイズ拡縮の遷移スピード

	static const int	FOOTPRINT_SPAWN_RANGE;	//足跡を発生する間隔
	static const int	FOOTPRINT_SPAWN_MIN;	//足跡発生の最低インターバル
	static const int	FOOTPRINT_RANGE;		//足跡を飛び散らせる範囲
	static const float	FOOTPRINT_SIZE;			//足跡サイズ
	static const float	FOOTPRINT_GRAVITY;		//足跡を落下させる重力量
	static const float	FOOTPRINT_GRAVITY_CURRECT;//重力係数
	static const float	FOOTPRINT_JUMP;			//発生した際の浮遊量
	static const float	FOOTPRINT_SPEED;		//飛び散る速度
	static const int	FOOTPRINT_LIFE;			//寿命

	static const float SHADOW_SIZE;	//影ポリゴンのサイズ

	void SetLifePoly2D(void);	//体力ポリゴン2D生成

	void TransformUpdateEntrance(void);	//変身したⅩモデルの個性別の更新処理
	void TransformUpdate_Nimble(void);	//個性「俊敏」の更新処理
	void TransformUpdate_Tough(void);	//個性「強靭」の更新処理
	void TransformUpdate_Take(void);	//個性「収納」の更新処理
	void TransformUpdate_Bind(void);	//個性「束縛」の更新処理

	void StateUpdateEntrance(void);	//状態ごとの更新処理はここから
	void StateUpdate_Damage(void);	//ダメージ状態の更新処理
	void StateUpdate_Death(void);	//死亡状態の更新処理

	void SetBullet(void);		//弾発射処理
	void ActionControl(void);	//行動制御処理
	void LoadPartsInfo(void);	//パーツ情報読み取り
	void Transform(void);		//変身処理

	void Goal(void);			//ゴール処理

	void CollisionWhole(void);					//当たり判定まとめ
	void HumanCollisionWhole(int nType);		//人間状態のときの当たり判定
	void TransformCollisionWhole(int nType);	//変身状態のときの当たり判定

	void TransformSetLife(void);	//変身した時の体力変化

	void FootPrint(void);	//歩いた軌跡を生成
	void Shadow(void);		//影設定

	D3DXMATRIX	m_mtxWorld;	//ワールドマトリックス
	CModel		*m_apModel[MAX_PARTS];
	int			m_nNumModel;

	int m_nTransformModelID;//変身するモデル番号（人間状態は-１　それ以外は０以上の自然数

	CObjectX	*m_pObjX[NUM_TAKE_OBJECTX];	//Ⅹオブジェクトを収納する
	int			m_nNumTakeObjX;				//収納しているⅩオブジェクトの数

	CPoly2D		*m_pLifeBG;		//残り体力の背景
	CPoly2D		*m_pLifeLogo;	//残り体力のロゴ
	CPoly2D		*m_pLife[NUM_LIFE_DEJIT];	//残り体力を描画するのに使用

	CMotion *m_pMotion;		//モーション情報

	STATE m_State;			//状態

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_posOld;	//前回の位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_rot;		//向き

	int		m_nLifeMax;			//最大体力
	int		m_nLife;			//体力
	int		m_nDamageInterval;	//ダメージを受けてからの時間
	int		m_nDeathInterval;	//死亡してからのインターバル
	bool	m_bSneak;			//スニーク中かどうか
	bool	m_bDisp;			//描画するかどうか
	float	m_fSpecialMaxSpeed;	//個性による最大移動量の増減量

	bool	m_bTransform;		//変身可能どうか	true：変身可能	false：変身できない
	bool	m_bScaling;			//変身中の拡縮		true：拡大中	false：縮小中
	float	m_fScal;			//拡縮サイズ
	int		m_nNextModelID;		//変身するモデル番号

	bool	m_bControll;		//チュートリアル限定で操作ON/OFFを切り替える

	int m_nMoveCounter;	//動いた時間を計測
	int m_nNextFootPrint;//次に足跡が出現するタイミング

	int m_nShadowID;	//影番号
};

#endif