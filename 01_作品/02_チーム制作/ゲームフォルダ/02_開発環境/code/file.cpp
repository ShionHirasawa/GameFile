//==========================================
//
//ファイル読み込みプログラム[file.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "file.h"
#include "camera.h"
#include "HDR_camera.h"
#include "light.h"
#include "model.h"
#include "meshfault.h"
#include "object.h"
#include <stdio.h>
#include <assert.h>

//コード関係
#define CODE_LENGTH	(128 + MAX_PATH_STR)

//読み取り状態列挙
typedef enum
{
	//共通
	READSTAT_NONE = 0,	//何もしてない

	//-モデルビューワー-----------------------
	READSTAT_CAMERASET,
	READSTAT_HDR_CAMERASET,
	READSTAT_LIGHTSET,
	READSTAT_SKYSET,
	READSTAT_MOUNTAINSET,
	READSTAT_FIELDSET,
	READSTAT_WALLSET,
	READSTAT_MODELSET,
	READSTAT_BILLBOARDSET,
	READSTAT_PLAYERSET,
	//----------------------------------------

	//-配置情報-------------------------------
	READSTAT_SETENEMY,
	//----------------------------------------

	//--モーションビューワー------------------
	READSTAT_CHARACTERSET,
	READSTAT_PARTSSET,
	READSTAT_MOTIONSET,
	READSTAT_KEYSET,
	READSTAT_KEY,
	//----------------------------------------

	//-オリジナル-----------------------------
	READSTAT_FAULTSET,
	//----------------------------------------
	READSTAT_MAX
} READSTAT;

//グローバル
READSTAT g_readStat = READSTAT_NONE;

//取得したもの
//記憶する
char g_aTexFilePath[MAX_TEXTURE][MAX_PATH_STR];
char g_aModelFilePath[MAX_NUM_MODEL][MAX_PATH_STR];
ReadPlayerModel g_readPlayermodel[ANIMAL_MAX];

//使い捨て
ReadTextureData g_readtexdata;
ReadModelData g_readmodeldata;
Camera g_readCamera;
HDRCamera g_readHDRCamera;
D3DLIGHT9 g_readLight;
ReadSky g_readsky;
int g_nMountainTexNum;
ReadMeshField g_readmeshfield;
MeshWall g_readMeshWall;
ReadModel g_readmodel;
ReadBillBoard g_readbillboard;
MOTION_INFO g_motionInfo[MOTIONTYPE_MAX];
Parts g_readParts;
Fault g_readFault;

//↑に使うもの
int g_counterMotionInfo;
int g_counterKeyInfo;
int g_counterKey;
int g_counterReadTexture;
int g_counterReadModel;		//モデルビューワー・モーションビューワー共通で使用可
int g_counterReadCamera;	//カメラの数カウント
int g_counterReadHDRCamera;	//レースゲームカメラの数カウント
int g_counterReadLight;
int g_counterReadAnimal;
int g_nIdxParts;

//========================
//ファイル初期化処理
//========================
void InitFile(void)
{
	//変数初期化
	//取得に使うもの
	g_readtexdata = {};
	g_readmodeldata = {};
	g_readCamera = {};
	g_readHDRCamera = {};
	g_readLight = {};
	g_readsky = {};
	g_nMountainTexNum = 0;
	g_readmeshfield = {};
	g_readMeshWall = {};
	g_readmodel = {};
	g_readbillboard = {};
	for (int nCntAnimal = 0; nCntAnimal < ANIMAL_MAX; nCntAnimal++)
	{
		g_readPlayermodel[nCntAnimal] = {};
	}
	g_readParts = {};

	//↑に使うもの
	g_counterMotionInfo = 0;
	g_counterKeyInfo = 0;
	g_counterKey = 0;
	g_counterReadTexture = 0;
	g_counterReadModel = 0;
	g_counterReadCamera = 0;
	g_counterReadHDRCamera = 0;
	g_counterReadLight = 0;
	g_counterReadAnimal = 0;
	g_nIdxParts = -1;

	g_readStat = READSTAT_NONE;
}

//========================
//ファイル終了処理
//========================
void UninitFile(void)
{
	//一旦なし
}

//========================
//ファイル更新処理
//========================
void UpdateFile(void)
{
	//一旦なし
}

//========================
//マップ・敵読み込み処理
//========================
void LoadModelViewerFile(const char *path)
{
	FILE *pFile;
	char aCode[CODE_LENGTH];
	char *pSprit;
	bool bRead = false;
	int nCntInit = 0;	//初期化カウンタ

	//変数初期化
	g_counterReadModel = 0;

	//読み込みファイル設定
	pFile = fopen(path, "r");

	//敵配置情報を取得
	if (pFile != NULL)
	{
		while (1)
		{
			fgets(&aCode[0], CODE_LENGTH, pFile);

			//コメントアウトチェック
			char *pCharPos = strchr(&aCode[0], '#');
			if (pCharPos != nullptr)
			{//strchrの返り値がぬるぽではない
				*pCharPos = '\0';
			}

			//タブ消去
			while (aCode[0] == '\t')
			{
				char aCodeBackup[CODE_LENGTH];
				strcpy(&aCodeBackup[0], &aCode[0]);
				strcpy(&aCode[0], &aCodeBackup[1]);
			}

			//文字列チェック
			if (strncmp(&aCode[0], CODE_SCRIPT, sizeof CODE_SCRIPT / sizeof(char) - 1) == 0)
			{//読み取り開始
				bRead = true;
			}
			else if (strncmp(&aCode[0], CODE_END_SCRIPT, sizeof CODE_END_SCRIPT / sizeof(char) - 1) == 0)
			{//読み取り終了
				bRead = false;
				break;
			}
			else if (aCode[0] == EOF)
			{//EOFかもしれない
				if (feof(pFile))
				{//いや、これはEOFだ
					bRead = false;
					break;
				}
			}
			else if (bRead == true)
			{//読み取り
				switch (g_readStat)
				{
				case READSTAT_NONE:	//処理取得
					//処理部分
					if (strncmp(&aCode[0], CODE_CAMERASET, sizeof CODE_CAMERASET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_CAMERASET;
					}
					else if (strncmp(&aCode[0], CODE_HDR_CAMERASET, sizeof CODE_HDR_CAMERASET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_HDR_CAMERASET;
					}
					else if (strncmp(&aCode[0], CODE_LIGHTSET, sizeof CODE_LIGHTSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_LIGHTSET;
					}
					else if (strncmp(&aCode[0], CODE_SKYSET, sizeof CODE_SKYSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_SKYSET;
					}
					else if (strncmp(&aCode[0], CODE_MOUNTAINSET, sizeof CODE_MOUNTAINSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_MOUNTAINSET;
					}
					else if (strncmp(&aCode[0], CODE_FIELDSET, sizeof CODE_FIELDSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_FIELDSET;
					}
					else if (strncmp(&aCode[0], CODE_WALLSET, sizeof CODE_WALLSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_WALLSET;
					}
					else if (strncmp(&aCode[0], CODE_MODELSET, sizeof CODE_MODELSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_MODELSET;
					}
					else if (strncmp(&aCode[0], CODE_BILLBOARDSET, sizeof CODE_BILLBOARDSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_BILLBOARDSET;
					}
					else if (strncmp(&aCode[0], CODE_PLAYERSET, sizeof CODE_PLAYERSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_PLAYERSET;
					}
					//取得部分
					else if (strncmp(&aCode[0], CODE_TEXTURE_FILENAME, sizeof CODE_TEXTURE_FILENAME / sizeof(char) - 1) == 0)
					{
						if (g_counterReadTexture < MAX_TEXTURE)
						{
							pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

							//パス取得
							pSprit = strtok(NULL, " =\n");
							strcpy(&g_aTexFilePath[g_counterReadTexture][0], pSprit);

							//タブ文字が入っているところを消す
							char *pCharPos = strchr(&g_aTexFilePath[g_counterReadTexture][0], '\t');
							if (pCharPos != nullptr)
							{//strchrの返り値がぬるぽではない
								*pCharPos = '\0';
							}

							//加算
							g_counterReadTexture++;
						}
					}
					else if (strncmp(&aCode[0], CODE_MODEL_FILENAME, sizeof CODE_MODEL_FILENAME / sizeof(char) - 1) == 0)
					{
						if (g_counterReadModel< MAX_NUM_MODEL)
						{
							pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

							//パス取得
							pSprit = strtok(NULL, " =\n");
							strcpy(&g_aModelFilePath[g_counterReadModel][0], pSprit);

							//タブ文字が入っているところを消す
							char *pCharPos = strchr(&g_aModelFilePath[g_counterReadModel][0], '\t');
							if (pCharPos != nullptr)
							{//strchrの返り値がぬるぽではない
								*pCharPos = '\0';
							}

							//加算
							g_counterReadModel++;
						}
					}
					break;
				case READSTAT_CAMERASET:	//pvpカメラ情報取得
					if (strncmp(&aCode[0], CODE_END_CAMERASET, sizeof CODE_END_CAMERASET / sizeof(char) - 1) == 0)
					{
						//カメラ設定
						InitSetCameraPos(g_readCamera.posV, g_readCamera.posR, g_counterReadCamera);
						g_readStat = READSTAT_NONE;
						g_counterReadCamera++;		//加算
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readCamera.posV.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readCamera.posV.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readCamera.posV.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_REF, sizeof CODE_REF / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readCamera.posR.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readCamera.posR.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readCamera.posR.z = fatof(pSprit);
					}
					break;
				case READSTAT_HDR_CAMERASET:	//hdrカメラ情報取得
					if (strncmp(&aCode[0], CODE_END_HDR_CAMERASET, sizeof CODE_END_HDR_CAMERASET / sizeof(char) - 1) == 0)
					{
						//カメラ設定
						InitSetHDRCameraPos(g_readHDRCamera.posV, g_readHDRCamera.posR, g_counterReadHDRCamera);
						g_readStat = READSTAT_NONE;
						g_counterReadHDRCamera++;		//加算
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

															//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readHDRCamera.posV.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readHDRCamera.posV.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readHDRCamera.posV.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_REF, sizeof CODE_REF / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

															//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readHDRCamera.posR.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readHDRCamera.posR.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readHDRCamera.posR.z = fatof(pSprit);
					}
					break;
				case READSTAT_LIGHTSET:		//ライト情報取得
					if (strncmp(&aCode[0], CODE_END_LIGHTSET, sizeof CODE_END_LIGHTSET / sizeof(char) - 1) == 0)
					{
						//ライト設定
						SetLight(g_counterReadLight, g_readLight);
						g_readStat = READSTAT_NONE;

						g_counterReadLight++;
					}
					else if (strncmp(&aCode[0], CODE_DIRECTION, sizeof CODE_DIRECTION / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//仮置き変数宣言
						D3DXVECTOR3 vecDir;

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						vecDir.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						vecDir.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						vecDir.z = fatof(pSprit);

						//ベクトル正規化
						D3DXVec3Normalize(&vecDir, &vecDir);

						//代入
						g_readLight.Direction = vecDir;
					}
					else if (strncmp(&aCode[0], CODE_DIFFUSE, sizeof CODE_DIFFUSE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//赤
						pSprit = strtok(NULL, " =\n");
						g_readLight.Diffuse.r = fatof(pSprit);

						//緑
						pSprit = strtok(NULL, " =\n");
						g_readLight.Diffuse.g = fatof(pSprit);

						//青
						pSprit = strtok(NULL, " =\n");
						g_readLight.Diffuse.b = fatof(pSprit);

						//アルファ値は1.0固定
						g_readLight.Diffuse.a = 1.0f;
					}
					break;
				case READSTAT_SKYSET:		//空情報取得
					if (strncmp(&aCode[0], CODE_END_SKYSET, sizeof CODE_END_SKYSET / sizeof(char) - 1) == 0)
					{
						//空設定して
						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_TEXTYPE, sizeof CODE_TEXTYPE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(NULL, " =\n");
						g_readsky.texType = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_MOVE, sizeof CODE_MOVE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(NULL, " =\n");
						g_readsky.fMove = fatof(pSprit);
					}
					break;
				case READSTAT_MOUNTAINSET:	//山情報取得
					if (strncmp(&aCode[0], CODE_END_MOUNTAINSET, sizeof CODE_END_MOUNTAINSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_TEXTYPE, sizeof CODE_TEXTYPE / sizeof(char) - 1) == 0)
					{
						//取得
						pSprit = strtok(NULL, " =\n");
						g_nMountainTexNum = atoi(pSprit);
					}
					break;
				case READSTAT_FIELDSET:		//床情報取得
					if (strncmp(&aCode[0], CODE_END_FIELDSET, sizeof CODE_END_FIELDSET / sizeof(char) - 1) == 0)
					{
						//地面設定
						SetMeshField(g_readmeshfield.texType, g_readmeshfield.mf);
						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_TEXTYPE, sizeof CODE_TEXTYPE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(NULL, " =\n");
						g_readmeshfield.texType = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmeshfield.mf.pos.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmeshfield.mf.pos.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmeshfield.mf.pos.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmeshfield.mf.rot.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmeshfield.mf.rot.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmeshfield.mf.rot.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_SIZE, sizeof CODE_SIZE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//サイズ読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmeshfield.mf.fRadius = fatof(pSprit);
					}
					break;
				case READSTAT_WALLSET:		//壁情報取得
					if (strncmp(&aCode[0], CODE_END_WALLSET, sizeof CODE_END_WALLSET / sizeof(char) - 1) == 0)
					{
						//壁生成して
						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_TEXTYPE, sizeof CODE_TEXTYPE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.texType = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//位置読み取り
						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.pos.x = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.pos.y = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.pos.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//向き読み取り
						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.rot.x = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.rot.y = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.rot.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_BLOCK, sizeof CODE_BLOCK / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//分割数読み取り
						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.blockX = atoi(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.blockZ = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_SIZE, sizeof CODE_SIZE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//サイズ読み取り
						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.sizeX = atoi(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readMeshWall.sizeZ = atoi(pSprit);
					}
					break;
				case READSTAT_MODELSET:		//モデル情報取得
					if (strncmp(&aCode[0], CODE_END_MODELSET, sizeof CODE_END_MODELSET / sizeof(char) - 1) == 0)
					{
						SetObject(g_readmodel.pos, g_readmodel.rot, (OBJECTTYPE)g_readmodel.modelType);
						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_TYPE, sizeof CODE_TYPE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//種類読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmodel.modelType = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmodel.pos.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmodel.pos.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmodel.pos.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmodel.rot.x = (fatof(pSprit) / 180) * D3DX_PI;

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmodel.rot.y = (fatof(pSprit) / 180) * D3DX_PI;

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readmodel.rot.z = (fatof(pSprit) / 180) * D3DX_PI;
					}
					else if (strncmp(&aCode[0], CODE_STATE, sizeof CODE_STATE / sizeof(char) - 1) == 0)
					{//状態指定
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//状態設定
						pSprit = strtok(NULL, " =\n");
						g_readmodel.state = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_COLLISION, sizeof CODE_COLLISION / sizeof(char) - 1) == 0)
					{//0なら当たり判定無効
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//当たり判定設定
						pSprit = strtok(NULL, " =\n");
						g_readmodel.collision = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_SHADOW, sizeof CODE_SHADOW / sizeof(char) - 1) == 0)
					{//0なら影を使用しない
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						pSprit = strtok(NULL, " =\n");
						g_readmodel.shadow = atoi(pSprit);
					}
					break;
				case READSTAT_BILLBOARDSET:	//ビルボード情報取得
					if (strncmp(&aCode[0], CODE_END_BILLBOARDSET, sizeof CODE_END_BILLBOARDSET / sizeof(char) - 1) == 0)
					{
						//ビルボード設定して
						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_TEXTYPE, sizeof CODE_TEXTYPE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//種類読み取り
						pSprit = strtok(NULL, " =\n");
						g_readbillboard.texType = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//位置読み取り
						pSprit = strtok(NULL, " =\n");
						g_readbillboard.pos.x = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readbillboard.pos.y = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readbillboard.pos.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_SIZE, sizeof CODE_SIZE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//サイズ読み取り
						pSprit = strtok(NULL, " =\n");
						g_readbillboard.sizeX = atoi(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readbillboard.sizeZ = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_ORIGIN, sizeof CODE_ORIGIN / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//何に使うのかわからないもの読み取り
						pSprit = strtok(NULL, " =\n");
						g_readbillboard.originX = atoi(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readbillboard.originZ = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_BLEND, sizeof CODE_BLEND / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//合成モード読み取り
						pSprit = strtok(NULL, " =\n");
						g_readbillboard.blend = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_SHADOW, sizeof CODE_SHADOW / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//影使用有無読み取り
						pSprit = strtok(NULL, " =\n");
						g_readbillboard.shadow = atoi(pSprit);
					}
					break;
				case READSTAT_PLAYERSET:	//プレイヤーモデル情報取得
					if (strncmp(&aCode[0], CODE_END_PLAYERSET, sizeof CODE_END_PLAYERSET / sizeof(char) - 1) == 0)
					{
						//プレイヤーモデル取得完了
						g_readStat = READSTAT_NONE;

						//加算
						g_counterReadAnimal++;
					}
					else if (strncmp(&aCode[0], CODE_MOTION_FILENAME, sizeof CODE_MOTION_FILENAME / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(NULL, " =\n");
						strcpy(&g_readPlayermodel[g_counterReadAnimal].motionFileName[0], pSprit);

						//タブ文字が入っているところを消す
						char *pCharPos = strchr(&g_readPlayermodel[g_counterReadAnimal].motionFileName[0], '\t');
						if (pCharPos != nullptr)
						{//strchrの返り値がぬるぽではない
							*pCharPos = '\0';
						}
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//位置読み取り
						pSprit = strtok(NULL, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.x = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.y = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//向き読み取り
						pSprit = strtok(NULL, " =\n");
						g_readPlayermodel[g_counterReadAnimal].rot.x = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readPlayermodel[g_counterReadAnimal].rot.y = fatof(pSprit);

						pSprit = strtok(NULL, " =\n");
						g_readPlayermodel[g_counterReadAnimal].rot.z = fatof(pSprit);
					}
					break;
				}
			}
		}
	}
	else
	{
		assert(pFile != NULL);
	}

	//ファイル閉じる
	fclose(pFile);
}

//========================
//モーション読み込み処理
//引数1:モーションビューワーの設定ファイルのパス
//引数2:入れたいモデル構造体のポインタ
//========================
void LoadMotionViewerFile(const char *path, Model *pModel)
{
	FILE *pFile;
	char aCode[CODE_LENGTH];
	char *pSprit;
	bool bRead = false;

	//変数初期化
	g_counterReadModel = 0;

	//モーション情報
	//読み込みファイル設定
	pFile = fopen(path, "r");

	//敵配置情報を取得
	if (pFile != NULL)
	{
		while (1)
		{
			fgets(&aCode[0], CODE_LENGTH, pFile);

			//コメントアウトチェック
			char *pCharPos = strchr(&aCode[0], '#');
			if (pCharPos != nullptr)
			{//strchrの返り値がぬるぽではない
				*pCharPos = '\0';
			}

			//タブ消去
			while (aCode[0] == '\t')
			{
				char aCodeBackup[CODE_LENGTH];
				strcpy(&aCodeBackup[0], &aCode[0]);
				strcpy(&aCode[0], &aCodeBackup[1]);
			}

			//文字列チェック
			if (strncmp(&aCode[0], CODE_SCRIPT, sizeof CODE_SCRIPT / sizeof(char) - 1) == 0)
			{//読み取り開始
				bRead = true;
			}
			else if (strncmp(&aCode[0], CODE_END_SCRIPT, sizeof CODE_END_SCRIPT / sizeof(char) - 1) == 0)
			{//読み取り終了
				bRead = false;
				break;
			}
			else if (aCode[0] == EOF)
			{//EOFかもしれない
				if (feof(pFile))
				{//いや、これはEOFだ
					bRead = false;
					break;
				}
			}
			else if (bRead == true)
			{//読み取り
				switch (g_readStat)
				{
				case READSTAT_NONE:	//処理取得
					if (strncmp(&aCode[0], CODE_MODEL_FILENAME, sizeof CODE_MODEL_FILENAME / sizeof(char) - 1) == 0)
					{
						if (g_counterReadModel < MAX_PARTS)
						{
							pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

							//代入
							strcpy(pModel->aModelFileName[g_counterReadModel], strtok(NULL, " =\n"));
							//タブ文字が入っているところを消す
							char *pCharPos = strchr(&pModel->aModelFileName[g_counterReadModel][0], '\t');
							if (pCharPos != nullptr)
							{//strchrの返り値がぬるぽではない
								*pCharPos = '\0';
							}

							//加算
							g_counterReadModel++;
						}
					}
					if (strncmp(&aCode[0], CODE_CHARACTERSET, sizeof CODE_CHARACTERSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_CHARACTERSET;
					}
					if (strncmp(&aCode[0], CODE_MOTIONSET, sizeof CODE_MOTIONSET / sizeof(char) - 1) == 0)
					{
						if (g_counterMotionInfo < MOTIONTYPE_MAX)
						{
							g_readStat = READSTAT_MOTIONSET;
						}
					}
					break;
				case READSTAT_CHARACTERSET:		//モデル情報取得
					if (strncmp(&aCode[0], CODE_END_CHARACTERSET, sizeof CODE_END_CHARACTERSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_PARTSSET, sizeof CODE_PARTSSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_PARTSSET;
					}
					else if (strncmp(&aCode[0], CODE_MOVE, sizeof CODE_MOVE / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//移動量取得
						pSprit = strtok(NULL, " =\n");
						pModel->fMove = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_JUMP, sizeof CODE_JUMP / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//ジャンプ量取得
						pSprit = strtok(NULL, " =\n");
						pModel->fJump = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_RADIUS, sizeof CODE_RADIUS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//半径取得
						pSprit = strtok(NULL, " =\n");
						pModel->fRadius = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_NUM_PARTS, sizeof CODE_NUM_PARTS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//パーツ数取得
						pSprit = strtok(NULL, " =\n");
						pModel->nNumParts = atoi(pSprit);
					}
					break;
				case READSTAT_PARTSSET:
					if (strncmp(&aCode[0], CODE_END_PARTSSET, sizeof CODE_END_PARTSSET / sizeof(char) - 1) == 0)
					{
						//取得したものをもらったポインタに代入
						pModel->aParts[g_nIdxParts] = g_readParts;

						//取得したものを消す
						g_readParts = {};

						//キャラクター設定状態に戻る
						g_readStat = READSTAT_CHARACTERSET;
					}
					else if (strncmp(&aCode[0], CODE_INDEX, sizeof CODE_INDEX / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//番号取得
						pSprit = strtok(NULL, " =\n");
						g_nIdxParts = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_PARENT, sizeof CODE_PARENT / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//親番号取得
						pSprit = strtok(NULL, " =\n");
						g_readParts.mIdxModelParent = atoi(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readParts.posOffset.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readParts.posOffset.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readParts.posOffset.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readParts.rotOffset.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readParts.rotOffset.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readParts.rotOffset.z = fatof(pSprit);
					}
					break;
				case READSTAT_MOTIONSET:
					if (strncmp(&aCode[0], CODE_KEYSET, sizeof CODE_KEYSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_KEYSET;
					}
					else if (strncmp(&aCode[0], CODE_END_MOTIONSET, sizeof CODE_END_MOTIONSET / sizeof(char) - 1) == 0)
					{
						g_counterMotionInfo++;
						g_counterKeyInfo = 0;
						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_LOOP, sizeof CODE_LOOP / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//ループ可否読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].bLoop = atoi(pSprit) == 1 ? true : false;
					}
					else if (strncmp(&aCode[0], CODE_NUM_KEY, sizeof CODE_NUM_KEY / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

															//キー数読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].nNumKey = atoi(pSprit);
					}
					break;
				case READSTAT_KEYSET:
					if (strncmp(&aCode[0], CODE_KEY, sizeof CODE_KEY / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_KEY;
					}
					else if (strncmp(&aCode[0], CODE_END_KEYSET, sizeof CODE_END_KEYSET / sizeof(char) - 1) == 0)
					{
						g_counterKeyInfo++;
						g_counterKey = 0;
						g_readStat = READSTAT_MOTIONSET;
					}
					else if (strncmp(&aCode[0], CODE_FRAME, sizeof CODE_FRAME / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//フレーム数読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].aKeyInfo[g_counterKeyInfo].nFrame = atoi(pSprit);
					}
					break;
				case READSTAT_KEY:
					if (strncmp(&aCode[0], CODE_END_KEY, sizeof CODE_END_KEY / sizeof(char) - 1) == 0)
					{
						g_counterKey++;
						g_readStat = READSTAT_KEYSET;
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].aKeyInfo[g_counterKeyInfo].aKey[g_counterKey].fPosX = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].aKeyInfo[g_counterKeyInfo].aKey[g_counterKey].fPosY = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].aKeyInfo[g_counterKeyInfo].aKey[g_counterKey].fPosZ = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X向き読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].aKeyInfo[g_counterKeyInfo].aKey[g_counterKey].fRotX = fatof(pSprit);

						//Y向き読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].aKeyInfo[g_counterKeyInfo].aKey[g_counterKey].fRotY = fatof(pSprit);

						//Z向き読み取り
						pSprit = strtok(NULL, " =\n");
						g_motionInfo[g_counterMotionInfo].aKeyInfo[g_counterKeyInfo].aKey[g_counterKey].fRotZ = fatof(pSprit);
					}
					break;
				}
			}
		}
	}
	else
	{
		assert(pFile != NULL);
	}

	//ファイル閉じる
	fclose(pFile);
}

//========================
//オリジナルモデル配置情報読み込み処理
//引数1:オリジナルモデル配置情報の設定ファイルのパス
//========================
void LoadModelOriginalFile(const char *path)
{
	FILE *pFile;
	char aCode[CODE_LENGTH];
	char *pSprit;
	bool bRead = false;

	//変数初期化
	g_counterReadModel = 0;

	//モーション情報
	//読み込みファイル設定
	pFile = fopen(path, "r");

	//敵配置情報を取得
	if (pFile != NULL)
	{
		while (1)
		{
			fgets(&aCode[0], CODE_LENGTH, pFile);

			//コメントアウトチェック
			char *pCharPos = strchr(&aCode[0], '#');
			if (pCharPos != nullptr)
			{//strchrの返り値がぬるぽではない
				*pCharPos = '\0';
			}

			//タブ消去
			while (aCode[0] == '\t')
			{
				char aCodeBackup[CODE_LENGTH];
				strcpy(&aCodeBackup[0], &aCode[0]);
				strcpy(&aCode[0], &aCodeBackup[1]);
			}

			//文字列チェック
			if (strncmp(&aCode[0], CODE_SCRIPT, sizeof CODE_SCRIPT / sizeof(char) - 1) == 0)
			{//読み取り開始
				bRead = true;
			}
			else if (strncmp(&aCode[0], CODE_END_SCRIPT, sizeof CODE_END_SCRIPT / sizeof(char) - 1) == 0)
			{//読み取り終了
				bRead = false;
				break;
			}
			else if (aCode[0] == EOF)
			{//EOFかもしれない
				if (feof(pFile))
				{//いや、これはEOFだ
					bRead = false;
					break;
				}
			}
			else if (bRead == true)
			{//読み取り
				switch (g_readStat)
				{
				case READSTAT_NONE:	//処理取得
					if (strncmp(&aCode[0], CODE_FAULTSET, sizeof CODE_FAULTSET / sizeof(char) - 1) == 0)
					{
						g_readStat = READSTAT_FAULTSET;
					}
					break;
				case READSTAT_FAULTSET:		//モデル情報取得
					if (strncmp(&aCode[0], CODE_END_FAULTSET, sizeof CODE_END_FAULTSET / sizeof(char) - 1) == 0)
					{

						g_readStat = READSTAT_NONE;
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readFault.pos.x = fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readFault.pos.y = fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readFault.pos.z = fatof(pSprit);
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readFault.rot.x = (fatof(pSprit) / 180) * D3DX_PI;

						//Y座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readFault.rot.y = (fatof(pSprit) / 180) * D3DX_PI;

						//Z座標読み取り
						pSprit = strtok(NULL, " =\n");
						g_readFault.rot.z = (fatof(pSprit) / 180) * D3DX_PI;
					}
					else if (strncmp(&aCode[0], CODE_RADIUS, sizeof CODE_RADIUS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//半径取得
						pSprit = strtok(NULL, " =\n");
						g_readFault.fRadius = fatof(pSprit);
					}
					break;
				}
			}
		}
	}
	else
	{
		assert(pFile != NULL);
	}

	//ファイル閉じる
	fclose(pFile);
}

//========================
//テクスチャパス取得処理
//========================
char *GetTextureFilePath(int nTexNum)
{
	return &g_aTexFilePath[nTexNum][0];
}

//========================
//モデルパス取得処理
//========================
char *GetModelFilePath(int nModNum)
{
	return &g_aModelFilePath[nModNum][0];
}

//========================
//モーションパス処理
//========================
char *GetMotionFilePath(int animal)
{
	return &g_readPlayermodel[animal].motionFileName[0];
}

//========================
//モーション取得処理
//========================
MOTION_INFO GetMotionInfo(ANIMAL animal, MOTIONTYPE type)
{
	return g_motionInfo[type];
}

//========================
//断面図情報取得処理
//========================
Fault GetFaultData(void)
{
	return g_readFault;
}