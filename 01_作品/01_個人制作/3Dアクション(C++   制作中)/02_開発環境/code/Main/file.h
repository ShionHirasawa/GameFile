//==========================================
//
//ファイル読み込み処理[file.h]
//Author:Hirasawa Shion
//
//==========================================
#ifndef _FILE_H_
#define _FILE_H_

//コード名
/*スクリプト開始*/	#define CODE_SCRIPT		"SCRIPT"
/*スクリプト終了*/	#define CODE_END_SCRIPT	"END_SCRIPT"

//モデル情報関連
/*テクスチャの総数*/#define CODE_NUM_TEXTURE	"NUM_TEXTURE"
/*テクスチャのパス*/#define CODE_TEX_FILENAME	"TEXTURE_FILENAME"
/*パーツの総数*/	#define CODE_NUM_MODEL		"NUM_MODEL"
/*ファイルパス*/	#define CODE_MODEL_FILENAME	"MODEL_FILENAME"
/*Ⅹモデルの個性*/	#define CODE_XMODEL_TYPE	"XMODELTYPE"
/*モデル配置開始*/	#define CODE_MODELSET		"MODELSET"
/*モデル配置終了*/	#define CODE_END_MODELSET	"END_MODELSET"
/*プレイヤ配置開始*/#define CODE_PLAYERSET		"PLAYERSET"
/*プレイヤ配置終了*/#define CODE_END_PLAYERSET	"END_PLAYERSET"

//モーション
/*初期状態読み取り開始*/#define CODE_CHARACTERSET		"CHARACTERSET"
/*初期状態読み取り終了*/#define CODE_END_CHARACTERSET	"END_CHARACTERSET"

/* 移動速度 */	#define CODE_MOVE		"MOVE"
/*ジャンプ量*/	#define CODE_JUMP		"JUMP"
/* パーツ数 */	#define CODE_NUM_PARTS	"NUM_PARTS"
/*自分の番号*/	#define CODE_INDEX		"INDEX"
/* 親の番号 */	#define CODE_PARENT		"PARENT"
/*ループ設定*/	#define CODE_LOOP		"LOOP"
/*キーの総数*/	#define CODE_NUM_KEY	"NUM_KEY"
/*フレーム数*/	#define CODE_FRAME		"FRAME"

//パーツ設定
/*設定開始*/#define CODE_PARTSSET		"PARTSSET"
/*設定終了*/#define CODE_END_PARTSSET	"END_PARTSSET"

//キーセット設定
/*設定開始*/#define CODE_KEYSET		"KEYSET"
/*設定終了*/#define CODE_END_KEYSET	"END_KEYSET"

//キー設定
/*設定開始*/#define CODE_KEY		"KEY"
/*設定終了*/#define CODE_END_KEY	"END_KEY"

//モーション設定
/*設定開始*/#define CODE_MOTIONSET		"MOTIONSET"
/*設定終了*/#define CODE_END_MOTIONSET	"END_MOTIONSET"

//パラメータ類
/*位置情報*/#define CODE_POS	"POS"
/*向き情報*/#define CODE_ROT	"ROT"
/*サイズ情報*/#define CODE_SIZE	"SIZE"
/*テクスチャサイズ情報*/#define CODE_TEX_SIZE	"TEX_SIZE"

//Ⅹファイル
/*Xファイルの総数*/	#define CODE_NUM_XFILE			"NUM_XFILE"
/*Xファイルパス*/	#define CODE_OBJECT_FILENAME	"OBJECT_FILENAME"

//オブジェクト
/*設定開始*/#define CODE_OBJECTSET		"OBJECTSET"
/*設定終了*/#define CODE_END_OBJECTSET	"END_OBJECTSET"
/*オブジェクトの数*/#define CODE_NUM_OBJECT	"NUM_OBJECT"

//宝石
/* 設定開始 */#define CODE_JEWELSET		"JEWELSET"
/*ゴール地点*/#define CODE_GOAL_POINT	"GOALPOINT"
/*ゴール半径*/#define CODE_GOAL_LENGTH	"GOALLENGTH"
/* 設定終了 */#define CODE_END_JEWELSET	"END_JEWELSET"

//内装
/*内装の数*/#define CODE_NUM_INTERIOR	"NUM_INTERIOR"

//床
/*設定開始*/#define CODE_FLOORSET		"FLOORSET"
/*設定終了*/#define CODE_END_FLOORSET	"END_FLOORSET"

//壁
/*設定開始*/#define CODE_WALLSET		"WALLSET"
/*設定終了*/#define CODE_END_WALLSET	"END_WALLSET"

//天井
/*設定開始*/#define CODE_CEILINGSET		"CEILINGSET"
/*設定終了*/#define CODE_END_CEILINGSET	"END_CEILINGSET"

//文字添削
/*コメント文字*/#define CHR_COMMENT	'#'
/*  タブ文字  */#define CHR_TAB		'\t'
/* 区切り文字 */#define CHR_PAUSE	" =\n"
/*  終端文字  */#define CHR_END		'\0'
/*カンマ区切り*/#define CHR_COMMA	","
/*  改行文字  */#define CHR_NEWLINE	'\n'

/*モデルの最大数*/#define MAX_MODEL	(10)

//前方宣言
class CModel;
class CMotion;
class CObjectX;

//ファイルクラス
class CFile
{
public:
	//モーション読み取り状態列挙
	typedef enum
	{
		MOTION_READSTATE_NONE = 0,		//次の情報を読み取る
		MOTION_READSTATE_CHARACTERSET,	//キャラクター設定
		MOTION_READSTATE_PARTSSET,		//パーツ情報設定
		MOTION_READSTATE_MOTIONSET,		//モーション設定
		MOTION_READSTATE_KEYSET,		//モーションのキー設定
		MOTION_READSTATE_KEY,			//各パーツごとのキー設定
		MOTION_READSTATE_MAX
	} MOTION_READSTATE;

	//オブジェクト配置情報読み取り状態列挙
	typedef enum
	{
		OBJECT_READSTATE_NONE = 0,	//次の情報が何かを読み取る
		OBJECT_READSTATE_SET,
		OBJECT_READSTATE_MAX
	} OBJECT_READSTATE;

	CFile();
	~CFile();

	void Init(void);
	void Uninit(void);
	void LoadMotion(const char *pPath, CModel **ppModel, CMotion *pMotion, int *pNumParts, int *pParentNumber);
	void LoadStageInterior(const char *pPath);
	void LoadStageObject(const char *pPath);
	void LoadStageEnemy(const char *pPath);

private:
	int LoadInt(char *pString, char *pPunc) { return atoi(strtok(pString, pPunc)); }
	float LoadFloat(char *pString, char *pPunc) { return fatof(strtok(pString, pPunc)); }
	D3DXVECTOR2 LoadVec2(char *pString, char *pPunc);
	D3DXVECTOR3 LoadVec3(char *pString, char *pPunc);
	void LoadCheckPoint(FILE *pFile, float *pMoveSpeed, int *pNumAll, D3DXVECTOR3 *pCheckPoint);

	void DeleteString(char *pString, char cDeleteStr);
	void SetFilePath(char *pStrage, char *pString, char *pPunc);

	bool SetEnemyCamera(char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fPatRot);
	bool SetEnemyRobot(char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fMoveSpeed, int nAttackPower, int *pNumAll, D3DXVECTOR3 *pCheckPoint);
	int m_nNumObjectX;
	static CObjectX *m_pObjectX;
};

#endif