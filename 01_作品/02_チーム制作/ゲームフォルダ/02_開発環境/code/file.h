//==========================================
//
//ファイル読み込みプログラムのヘッダ[file.h]
//Author:石原颯馬
//
//==========================================
#ifndef _FILE_H_
#define _FILE_H_

#include "main.h"
#include "model.h"
#include "meshfield.h"
#include "meshfault.h"

//マクロ
#define MAX_NUM_MODEL	(128)	//最大モデル数

//コード名
//共通
//読み込み開始・終了
#define CODE_SCRIPT				"SCRIPT"			//スクリプト開始
#define CODE_END_SCRIPT			"END_SCRIPT"		//スクリプト終了

//モデルビューワー・モーションビューワー用
//モデルビューワ
//テクスチャ数
#define CODE_NUM_TEXTURE		"NUM_TEXTURE"		//テクスチャ数読み取り
//テクスチャファイル名
#define CODE_TEXTURE_FILENAME	"TEXTURE_FILENAME"	//テクスチャファイルパス読み取り
//モデル数
#define CODE_NUM_MODEL			"NUM_MODEL"			//モデル数読み取り
//モデルファイル名
#define CODE_MODEL_FILENAME		"MODEL_FILENAME"	//モデルファイルパス読み取り
//カメラ情報
#define CODE_CAMERASET			"CAMERASET"			//pvpカメラの設定開始
#define CODE_END_CAMERASET		"END_CAMERASET"		//pvpカメラの設定終了
#define CODE_HDR_CAMERASET		"HDR_CAMERASET"		//hdrカメラの設定開始
#define CODE_END_HDR_CAMERASET	"END_HDR_CAMERASET"	//hdrカメラの設定終了
//ライト情報
#define CODE_LIGHTSET			"LIGHTSET"			//ライト設定開始
#define CODE_END_LIGHTSET		"END_LIGHTSET"		//ライト設定終了
//空情報
#define CODE_SKYSET				"SKYSET"			//空設定開始
#define CODE_END_SKYSET			"END_SKYSET"		//空設定終了
//山情報
#define CODE_MOUNTAINSET		"MOUNTAINSET"		//山設定開始
#define CODE_END_MOUNTAINSET	"END_MOUNTAINSET"	//山設定終了
//地面情報
#define CODE_FIELDSET			"FIELDSET"			//地面設定開始
#define CODE_END_FIELDSET		"END_FIELDSET"		//地面設定終了
//壁情報
#define CODE_WALLSET			"WALLSET"			//壁設定開始
#define CODE_END_WALLSET		"END_WALLSET"		//壁設定終了
//モデル情報
#define CODE_MODELSET			"MODELSET"			//モデル配置開始
#define CODE_END_MODELSET		"END_MODELSET"		//モデル配置終了
//ビルボード情報
#define CODE_BILLBOARDSET		"BILLBOARDSET"		//ビルボード配置開始
#define CODE_END_BILLBOARDSET	"END_BILLBOARDSET"	//ビルボード配置終了
//プレイヤー情報
#define CODE_PLAYERSET			"PLAYERSET"			//プレイヤー配置開始
#define CODE_END_PLAYERSET		"END_PLAYERSET"		//プレイヤー配置終了

//モーションビューワー
//初期
#define CODE_NUM_MODEL			"NUM_MODEL"			//モデル数
#define CODE_MODEL_FILENAME		"MODEL_FILENAME"	//モデルのファイルパス
#define CODE_CHARACTERSET		"CHARACTERSET"		//キャラクター情報
#define CODE_END_CHARACTERSET	"END_CHARACTERSET"

//パーツ設定
#define CODE_PARTSSET			"PARTSSET"			//パーツ設定開始
#define CODE_END_PARTSSET		"END_PARTSSET"		//パーツ設定終了

//モーション設定
#define CODE_MOTIONSET			"MOTIONSET"			//モーション設定開始
#define CODE_END_MOTIONSET		"END_MOTIONSET"		//モーション設定終了
//キーセット設定
#define CODE_KEYSET				"KEYSET"			//キーセット設定開始
#define CODE_END_KEYSET			"END_KEYSET"		//キーセット設定終了
//キー設定
#define CODE_KEY				"KEY"				//キー設定開始
#define CODE_END_KEY			"END_KEY"			//キー設定終了

//モデルオリジナル用
//断面図
#define CODE_FAULTSET			"FAULTSET"			//断面図設定開始
#define CODE_END_FAULTSET		"END_FAULTSET"		//断面図設定終了

//パラメータ類
//モデル・モーション・オリジナル共通
#define CODE_POS				"POS"				//位置
#define CODE_ROT				"ROT"				//向き
#define CODE_RADIUS				"RADIUS"			//半径
//モデルビューワー用
#define CODE_REF				"REF"				//注視点
#define CODE_DIRECTION			"DIRECTION"			//方向
#define CODE_DIFFUSE			"DIFFUSE"			//拡散光
#define CODE_TEXTYPE			"TEXTYPE"			//テクスチャ種類
#define CODE_MOVE				"MOVE"				//移動量
#define CODE_BLOCK				"BLOCK"				//マス数
#define CODE_SIZE				"SIZE"				//1マス当たりの大きさ
#define CODE_TYPE				"TYPE"				//モデル類タイプ
#define CODE_STATE				"STATE"				//状態（何に使うか忘れた
#define CODE_COLLISION			"COLLISION"			//当たり判定（何に使うか忘れた
#define CODE_SHADOW				"SHADOW"			//影設定
#define CODE_ORIGIN				"ORIGIN"			//なにこれ
#define CODE_BLEND				"BLEND"				//合成の設定じゃないかな
#define CODE_MOTION_FILENAME	"MOTION_FILENAME"	//モーションのファイルパス
//モーションビューワー用
#define CODE_MOVE				"MOVE"				//移動量
#define CODE_JUMP				"JUMP"				//ジャンプ量
#define CODE_NUM_PARTS			"NUM_PARTS"			//パーツ数
#define CODE_INDEX				"INDEX"				//番号
#define CODE_PARENT				"PARENT"			//親モデルの番号
#define CODE_LOOP				"LOOP"				//ループするかの設定
#define CODE_NUM_KEY			"NUM_KEY"			//キー数
#define CODE_FRAME				"FRAME"				//フレーム数

//構造体類
//テクスチャ
typedef struct
{
	int numTexture;	//テクスチャ数
	char fileName[MAX_TEXTURE][MAX_PATH_STR];
} ReadTextureData;

//モデル
typedef struct
{
	int numTexture;	//テクスチャ数
	char fileName[MAX_TEXTURE][MAX_PATH_STR];
} ReadModelData;

//カメラはCamera.hのものを使う
//ライトはD3DLIGHT9のものを使う

//空（使いたければ移動してOK）
typedef struct
{
	int texType;	//テクスチャ種類（読み込んだテクスチャの中から選ぶ）
	float fMove;	//移動量
} ReadSky;

//山はテクスチャ種類設定するだけだからそっちで用意して

//メッシュフィールド
typedef struct
{
	int texType;	//テクスチャ種類（読み込んだテクスチャの中から選ぶ）
	MESHFIELD mf;	//メッシュフィールド生成に必要なデータ類
} ReadMeshField;

//メッシュ壁（使いたければ移動してOK）
typedef struct
{
	int texType;		//テクスチャ種類（読み込んだテクスチャの中から選ぶ）
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	int blockX;			//Xの分割数
	int blockZ;			//Zの分割数
	int sizeX;			//Xのサイズ
	int sizeZ;			//Zのサイズ
} MeshWall;

//モデル読み込みに使う
typedef struct
{
	int modelType;		//モデル種類
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	int state;			//状態（何に使うか忘れた）
	int collision;		//当たり判定（何に使うか忘れた）
	int shadow;			//影（シャドウマトリックスなるものを使うなら要らないかも）
} ReadModel;

//ビルボード
typedef struct
{
	int texType;		//テクスチャ種類
	D3DXVECTOR3 pos;	//位置
	int sizeX;			//Xのサイズ
	int sizeZ;			//Zのサイズ
	int originX;		//Xのなにこれ
	int originZ;		//Zのなにこれ
	int blend;			//合成の設定とかじゃないかな
	int shadow;			//影（何に使うか忘れた）
} ReadBillBoard;

//プレイヤーモデル
typedef struct
{
	char motionFileName[MAX_PATH_STR];	//モーションファイルのパス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
} ReadPlayerModel;

//プロト
void InitFile(void);
void UninitFile(void);
void UpdateFile(void);
void LoadModelViewerFile(const char *path);	//モデルビューワーの設定ファイル読み込み（引数は設定ファイルのパス指定）
void LoadMotionViewerFile(const char *path, Model *pModel);
void LoadModelOriginalFile(const char *path);
MOTION_INFO GetMotionInfo(ANIMAL animal, MOTIONTYPE type);
Fault GetFaultData(void);
char *GetTextureFilePath(int nTexNum);
char *GetModelFilePath(int nModNum);
char *GetMotionFilePath(int animal);

#endif