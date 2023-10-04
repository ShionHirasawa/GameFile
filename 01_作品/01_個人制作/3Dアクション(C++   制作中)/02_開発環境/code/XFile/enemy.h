//================================================================================================
//
//敵処理[enemy.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//敵の最大数
#define MAX_ENEMY		(10)

//敵クラス
class CEnemy : public CObjectX
{
public:
	CEnemy(const int nPriority = OBJECT_PRIORITY_DEF);
	virtual ~CEnemy();
	virtual HRESULT Init(char *pFilePath);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Draw(D3DXCOLOR *pDiffuse);

	static HRESULT Load(void);

	static CEnemy *Create(char *pFilePath);
	void SetPos(const D3DXVECTOR3 pos);

private:
	void	MoveControl(void);
	static int	m_nTextureID;	// テクスチャ番号
};

//防犯カメラクラス
class CEnemySecurityCamera : public CEnemy
{
public:
	CEnemySecurityCamera(const int nPriority = OBJECT_PRIORITY_DEF);
	~CEnemySecurityCamera();
	HRESULT Init(char *pFilePath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemySecurityCamera *Create(char *pFilePath);

	//============================================
	//防犯カメラの設定処理
	//引数１　pos	：位置
	//引数２　rot	：初期向き
	//引数３　fPatRot:索敵中に見る範囲
	//============================================
	void SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fPatRot);

private:
	static const D3DXCOLOR SEARCH_LIGHT_COLOR;//サーチライト色
	static const float SEARCH_LENGTH;	//索敵距離の長さ
	static const float SEARCH_RADIUS;	//索敵半径

	static const int NUM_DEVIDE;	//円錐メッシュの分割数
	static const int ALL_VERTEX;	//円錐メッシュの全体頂点バッファ数
	static const int ALL_INDEX;		//円錐メッシュの全体インデックスバッファ数

	static const float ROTAION_SPEED;	//回転速度
	static const int ROTAION_INTERVAL;	//回転中止のインターバル

	void Patrol(void);	//巡回処理
	void SearchLight(void);	//プレイヤーがサーチライト内に居るか判定する処理
	void Chaice(void);	//プレイヤーを追いかける処理

	HRESULT SetVtxInfo(void);
	HRESULT SetIdxInfo(void);
	void DrawMesh(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	// インデックスバッファ
	D3DXMATRIX				m_mtxWorld;	// ワールドマトリックス

	D3DXVECTOR3 m_CirclePos;	//サーチライトの中心位置
	bool	m_bFind;			//プレイヤーがサーチライトの中にいるかどうか
	float	m_fPatrolStartRot;	//パトロールする開始角度
	float	m_fPatrolEndRot;	//パトロールする終了角度
	float	m_fRotationSpeed;	//終了角度へ回転する際の回転量
	bool	m_bRotation;	//見渡す際の回転方向　　true：終了角度へ　　false：開始角度へ
	int		m_nRotationInterval;//パトロールの限界角度に達した時の、一時的に回転中止するインターバル
};

//ロボットクラス
class CEnemyRobot : public CEnemy
{
public:
	//チェックポイントの情報
	struct CheckPoint
	{
		float fMove;		//移動量
		int nNumAll;		//チェックポイントの総数
		int nIdx;			//チェックポイントの番号
		D3DXVECTOR3 *pPoint;//チェックポイントの場所
	};

	CEnemyRobot(const int nPriority = 1);
	~CEnemyRobot();
	HRESULT Init(char *pFilePath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void GiveDamege(int nDamage = 1);

	static CEnemyRobot *Create(char *pFilePath);

	void SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CheckPoint CheckPoint, int nAttackPower);

private:
	static const float SEARCH_RANGE;	//索敵範囲の角度
	static const float SEARCH_LENGTH;	//索敵範囲の長さ
	static const float CHACE_SPEED;		//追跡スピード
	static const float ROTATE_SPEED;	//回転量
	static const int MISS_INTERVAL;		//見失ってから索敵する時間
	static const int RESTART_INTERVAL;	//再起動までにかかる時間
	static const int PATROL_INTERVAL;	//巡回中に立ち止まる時間
	static const int ATTACK_INTERVAL;	//攻撃硬直

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

	static bool s_bSiren;	//警告音を鳴らすかどうか

	void PlaySiren(bool bPlay);	//警告音を鳴らす処理

	void UpdateRestart(void);
	bool SearchPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot);
	bool CollisionPlayer(const D3DXVECTOR3 PlayerPos, const float fPlayerMargin, D3DXVECTOR3 *pPos, const float fMargin);
	void Patrol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot);		//チェックポイントを巡回する
	bool IsPassCheckPoint(D3DXVECTOR3 TargetPos, D3DXVECTOR3 Pos);				//チェックポイントを通過したか
	void CountInterval(void);	//インターバルを減少させる
	bool IsAnyInterval(void);	//各種インターバルが残っているか判定
	void CollisionWhole(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot);

	void FootPrint(D3DXVECTOR3 move, D3DXVECTOR3 pos, float rotY);	//歩いた軌跡を生成

	CheckPoint	m_CheckPoint;	//チェックポイントのポインタ
	int m_nPatrolInterval;		//巡回中のチェックポイントに到達した時の立ち止まるインターバル
	int m_nMissInterval;		//見失ってから索敵するインターバル
	int m_nReStartInterval;		//再起動までのインターバル
	int m_nAttackInterval;		//攻撃硬直インターバル
	int m_nAttackPower;			//攻撃力

	int m_nLife;	//体力

	int m_nMoveCounter;	//動いた時間を計測
	int m_nNextFootPrint;//次に足跡が出現するタイミング

	int m_nShadowID;	//影番号
};

#endif