//==========================================
//
//ファイル読み込みプログラム[file.cpp]
//Author:Hirasawa Shion
//
//==========================================
#include "main.h"
#include "../Model/model.h"
#include "../Model/motion.h"
#include "../2D/object2D.h"
#include "../3D/object3D.h"
#include "../3D/floor.h"
#include "../3D/wall.h"
#include "../3D/billboard.h"
#include "../XFile/objectX.h"
#include "../XFile/jewel.h"
#include "../XFile/enemy.h"
#include "file.h"
#include <assert.h>

CObjectX *CFile::m_pObjectX = NULL;

//========================
//コンストラクタ
//========================
CFile::CFile()
{

}

//========================
//デストラクタ
//========================
CFile::~CFile()
{

}

//========================
//初期化処理
//========================
void CFile::Init(void)
{

}

//========================
//終了処理
//========================
void CFile::Uninit(void)
{

}

//========================
//モーション読み込み処理
// 引数１	*pPath：モーションの設定ファイルのパス
// 引数２	**ppModel：Xファイルから読み取ったパーツを格納する
// 引数３	*pMotion：モーション情報を格納する
// 引数４	*pNumParts：パーツの総数を格納する
// 引数５	*pParentNumber：各パーツの親番号を保存する
//========================
void CFile::LoadMotion(const char *pPath, CModel **ppModel, CMotion *pMotion, int *pNumParts, int *pParentNumber)
{
	//ファイルを開く
	FILE *pFile = fopen(pPath, "r");

	//ファイルが読み込めた
	if (pFile != NULL)
	{
		/*読み込むパターン*/	MOTION_READSTATE ReadState = MOTION_READSTATE_NONE;
		/*モーション情報保存*/	CMotion::MOTION_INFO motionInfo;
		//変数初期化
		int CounterMotionInfo = 0;
		int CounterKeyInfo = 0;
		int CounterKey = 0;
		int nIdxParts = 0;

		char aCode[MAX_STR];	//１行読み取るのに使用する
		char *pSprit;			//タブ文字や空白など、処理に邪魔なものを格納する
		bool bRead = false;		//読み取りを続行するかどうか
		int nReadModel = 0;		//いくつのパーツを読み取ったか

		char XModelFilePath[MAX_PARTS][MAX_STR];//モデルパーツのXファイルのパスを格納
		D3DXVECTOR3 pos[MAX_PARTS];		//		位置	（配列番号は [ nIdxParts ] を使用する
		D3DXVECTOR3 rot[MAX_PARTS];		//		向き	（配列番号は [ nIdxParts ] を使用する

		while (TRUE)
		{
			//１行読み込み
			fgets(&aCode[0], MAX_STR, pFile);

			//コメントアウトチェック
			char *pCharPos = strchr(&aCode[0], CHR_COMMENT);
			if (pCharPos != nullptr)
			{//コメントアウト用の文字があった
				*pCharPos = CHR_END;//その文字以降を削除
			}

			//タブ消去
			while (aCode[0] == CHR_TAB)
			{
				char aCodeBackup[MAX_STR];
				strcpy(&aCodeBackup[0], &aCode[0]);//読み込んだ１行を保存する
				strcpy(&aCode[0], &aCodeBackup[1]);//頭のタブ文字を外した次からの文字で上書きする
			}

			//読み取り開始
			if (strncmp(&aCode[0], CODE_SCRIPT, sizeof CODE_SCRIPT - 1) == 0)bRead = true;

			//読み取り終了
			else if (strncmp(&aCode[0], CODE_END_SCRIPT, sizeof CODE_END_SCRIPT - 1) == 0 || aCode[0] == EOF && feof(pFile))	break;

			//++++++++++++++++++++++++++++++++
			//読み取り
			//++++++++++++++++++++++++++++++++
			else if (bRead == true)
			{
				//読み取る情報によって処理を分岐させる
				switch (ReadState)
				{
					//******************************************************************
					//次に行う処理を確認する
					//******************************************************************
					case MOTION_READSTATE_NONE:
					
					//++++++++++++++++++++++++++++++++++++++
					//モデルパーツの総数だった場合
					//++++++++++++++++++++++++++++++++++++++
					if (strncmp(&aCode[0], CODE_NUM_MODEL, sizeof CODE_NUM_MODEL - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//総数を代入
						*pNumParts = LoadInt(NULL, CHR_PAUSE);
					}

					//++++++++++++++++++++++++++++++++++++++
					//モデルパーツのファイルパスだった場合
					//++++++++++++++++++++++++++++++++++++++
					if (strncmp(&aCode[0], CODE_MODEL_FILENAME, sizeof CODE_MODEL_FILENAME - 1) == 0)
					{
						//読み込んだパーツの数が最大数に達していない
						if (nReadModel >= MAX_PARTS) return;

						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//パーツのファイルパスを代入する
						strcpy(&XModelFilePath[nReadModel][0], strtok(NULL, CHR_PAUSE));

						//タブ文字が入っていればそれ以降を消す
						char *pCharPos = strchr(&XModelFilePath[nReadModel][0], CHR_TAB);
						{
							*pCharPos = CHR_END;
						}

						//パーツの読み込んだ数を加算
						nReadModel++;
					}

					//++++++++++++++++++++++++++++++++++++++
					//キャラクター情報の設定だった場合
					//++++++++++++++++++++++++++++++++++++++
					if (strncmp(&aCode[0], CODE_CHARACTERSET, sizeof CODE_CHARACTERSET - 1) == 0)
					{
						//次に行う処理を「キャラクター情報設定」にする
						ReadState = MOTION_READSTATE_CHARACTERSET;
					}

					//++++++++++++++++++++++++++++++++++++++
					//モーション情報の設定だった場合
					//++++++++++++++++++++++++++++++++++++++
					if (strncmp(&aCode[0], CODE_MOTIONSET, sizeof CODE_MOTIONSET - 1) == 0)
					{
						//読み込んだモーションの数が、全モーションの数を超えていない
						if (CounterMotionInfo < CMotion::MOTIONTYPE_MAX)
						{
							//モーションの設定へ
							ReadState = MOTION_READSTATE_MOTIONSET;
						}
					}
					break;

					//******************************************************************
					//モデル情報取得
					//******************************************************************
				case MOTION_READSTATE_CHARACTERSET:
					//キャラクター設定終了
					if (strncmp(&aCode[0], CODE_END_CHARACTERSET, sizeof CODE_END_CHARACTERSET - 1) == 0)
					{
						//次の処理取得へ
						ReadState = MOTION_READSTATE_NONE;
					}

					//パーツ設定へ
					else if (strncmp(&aCode[0], CODE_PARTSSET, sizeof CODE_PARTSSET - 1) == 0)
					{
						ReadState = MOTION_READSTATE_PARTSSET;
					}
					break;

					//******************************************************************
					//パーツ情報取得
					//******************************************************************
				case MOTION_READSTATE_PARTSSET:
					//パーツ設定終了
					if (strncmp(&aCode[0], CODE_END_PARTSSET, sizeof CODE_END_PARTSSET - 1) == 0)
					{
						//取得したパーツ番号の情報をモデルに送る
						ppModel[nIdxParts] = CModel::Create(XModelFilePath[nIdxParts], pos[nIdxParts], rot[nIdxParts]);
						
						//キャラクター設定状態に戻る
						ReadState = MOTION_READSTATE_CHARACTERSET;
					}

					//パーツ番号
					else if (strncmp(&aCode[0], CODE_INDEX, sizeof CODE_INDEX - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//パーツ番号取得
						nIdxParts = LoadInt(NULL, CHR_PAUSE);
					}

					//親の番号
					else if (strncmp(&aCode[0], CODE_PARENT, sizeof CODE_PARENT - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//親番号取得
						pParentNumber[nIdxParts] = LoadInt(NULL, CHR_PAUSE);
					}

					//位置設定
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						pos[nIdxParts].x = LoadFloat(NULL, CHR_PAUSE);//X座標読み取り
						pos[nIdxParts].y = LoadFloat(NULL, CHR_PAUSE);//Y座標読み取り
						pos[nIdxParts].z = LoadFloat(NULL, CHR_PAUSE);//Z座標読み取り
					}

					//向き設定
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						rot[nIdxParts].x = LoadFloat(NULL, CHR_PAUSE);//Xの回転値読み取り
						rot[nIdxParts].y = LoadFloat(NULL, CHR_PAUSE);//Yの回転値読み取り
						rot[nIdxParts].z = LoadFloat(NULL, CHR_PAUSE);//Zの回転値読み取り
					}
					break;

					//******************************************************************
					//モーション情報取得
					//******************************************************************
				case MOTION_READSTATE_MOTIONSET:
					if (strncmp(&aCode[0], CODE_KEYSET, sizeof CODE_KEYSET - 1) == 0)
					{
						ReadState = MOTION_READSTATE_KEYSET;
					}
					else if (strncmp(&aCode[0], CODE_END_MOTIONSET, sizeof CODE_END_MOTIONSET - 1) == 0)
					{
						pMotion->Set(CounterMotionInfo);	//モーション番号を設定
						pMotion->SetInfo(motionInfo);		//モーション情報を代入

						CounterMotionInfo++;
						CounterKeyInfo = 0;
						ReadState = MOTION_READSTATE_NONE;
					}
					else if (strncmp(&aCode[0], CODE_LOOP, sizeof CODE_LOOP - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//ループ可否読み取り
						motionInfo.bLoop = LoadInt(NULL, CHR_PAUSE) == 1 ? true : false;
					}
					else if (strncmp(&aCode[0], CODE_NUM_KEY, sizeof CODE_NUM_KEY - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//キー数読み取り
						motionInfo.nNumKey = LoadInt(NULL, CHR_PAUSE);
					}
					break;

					//******************************************************************
					//キー情報設定
					//******************************************************************
				case MOTION_READSTATE_KEYSET:
					if (strncmp(&aCode[0], CODE_KEY, sizeof CODE_KEY - 1) == 0)
					{
						ReadState = MOTION_READSTATE_KEY;
					}
					else if (strncmp(&aCode[0], CODE_END_KEYSET, sizeof CODE_END_KEYSET - 1) == 0)
					{
						CounterKeyInfo++;
						CounterKey = 0;
						ReadState = MOTION_READSTATE_MOTIONSET;
					}
					else if (strncmp(&aCode[0], CODE_FRAME, sizeof CODE_FRAME - 1) == 0)
					{
						pSprit = strtok(&aCode[0], CHR_PAUSE);	//設定する項目名を消す

						//フレーム数読み取り
						motionInfo.aKeyInfo[CounterKeyInfo].nFrame = LoadInt(NULL, CHR_PAUSE);
					}
					break;

					//******************************************************************
					//キー情報取得
					//******************************************************************
				case MOTION_READSTATE_KEY:
					if (strncmp(&aCode[0], CODE_END_KEY, sizeof CODE_END_KEY - 1) == 0)
					{
						CounterKey++;
						ReadState = MOTION_READSTATE_KEYSET;
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fPosX = LoadFloat(NULL, CHR_PAUSE);//X座標読み取り
						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fPosY = LoadFloat(NULL, CHR_PAUSE);//Y座標読み取り
						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fPosZ = LoadFloat(NULL, CHR_PAUSE);//Z座標読み取り
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT - 1) == 0)
					{
						//設定する項目名を消す
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fRotX = LoadFloat(NULL, CHR_PAUSE);//X向き読み取り
						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fRotY = LoadFloat(NULL, CHR_PAUSE);//Y向き読み取り
						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fRotZ = LoadFloat(NULL, CHR_PAUSE);//Z向き読み取り
					}
					break;
				}
			}
		}

		//ファイル閉じる
		fclose(pFile);
	}
}

//========================
//ステージの内装情報を読み込み
//========================
void CFile::LoadStageInterior(const char *pPath)
{
	//ファイルを開く
	FILE *pFile = fopen(pPath, "r");

	//ファイルが開けた
	if (pFile != NULL)
	{
		//オブジェクトの配置情報を格納する
		D3DXVECTOR3 pos = VEC3_INIT;
		D3DXVECTOR3 rot = VEC3_INIT;
		D3DXVECTOR3 size = VEC3_INIT;
		D3DXVECTOR2 tex = VEC2_INIT;

		char cReadTxt[MAX_STR] = {};
		char temp[MAX_STR] = {};
		char ObjectFilePath[MAX_STR] = {};
		char *pTitle = NULL;
		int nNumSetFloor = 0;
		int nNumSetWall = 0;

		//文字列を読み取れるだけ全て読み取る
		while (TRUE)
		{
			//1行読み取り
			fgets(&cReadTxt[0], MAX_STR, pFile);
			strcpy(&temp[0], &cReadTxt[0]);

			//読み込んだ文字列の中にコメントがあるかチェック
			char *pCharPos = strchr(&cReadTxt[0], CHR_COMMENT);

			//コメントアウト用の文字があったらその文字以降を削除
			if (pCharPos != nullptr)*pCharPos = CHR_END;

			//ファイルを読み切ったら終了
			if (feof(pFile) != 0)break;

			//+++++++++++++++++++++++++
			//タイトルを読み取る
			//+++++++++++++++++++++++++
			pTitle = strtok(&cReadTxt[0], CHR_COMMA);

			//タイトルが無かった
			if (pTitle == NULL)	continue;

			//+++++++++++++++++++++++++
			//情報読み取り終了
			//+++++++++++++++++++++++++
			//床の場合
			if (strcmp(pTitle, CODE_END_FLOORSET) == 0)
			{
				//床インスタンス生成
				CFloor *pFloor = CFloor::Create();

				//インスタンス生成成功
				if (pFloor == NULL) return;

				//位置・サイズ情報を反映
				pFloor->Load("data\\TEXTURE\\carpet64.png");
				pFloor->Set(pos, rot, size, tex);
				nNumSetFloor++;
			}

			//壁の場合
			else if (strcmp(pTitle, CODE_END_WALLSET) == 0)
			{
				//壁インスタンス生成
				CWall *pWall = CWall::Create();

				//インスタンス生成成功
				if (pWall != NULL)
				{
					//位置・サイズ情報を反映
					pWall->Set(pos, rot, size, tex);
					nNumSetWall++;
				}
			}

			//位置設定の場合
			else if (strcmp(pTitle, CODE_POS) == 0)	pos = LoadVec3(NULL, CHR_COMMA);

			//位置設定の場合
			else if (strcmp(pTitle, CODE_ROT) == 0)	rot = LoadVec3(NULL, CHR_COMMA);

			//サイズ設定の場合
			else if (strcmp(pTitle, CODE_SIZE) == 0)size = LoadVec3(NULL, CHR_COMMA);

			//テクスチャサイズ設定の場合
			else if (strcmp(pTitle, CODE_TEX_SIZE) == 0)tex = LoadVec2(NULL, CHR_COMMA);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//========================
//ステージの配置物を読み込み
//========================
void CFile::LoadStageObject(const char *pPath)
{
	//ファイルを開く
	FILE *pFile = fopen(pPath, "r");

	//ファイルが開けた
	if (pFile != NULL)
	{
		CObjectX::XMODELTYPE type[MAX_MODEL] = {};
		int nLife[MAX_MODEL] = {};
		char cReadTxt[MAX_STR] = {};
		char ObjectFilePath[MAX_MODEL][MAX_STR] = { {} };
		char *pTitle = NULL;

		bool bRead = false;

		int NUM_OBJECT = 0;
		int nSetObject = 0;
		int nSetJewel = 0;
		int nReadObjCount = 0;

		//文字列を読み取れるだけ全て読み取る
		while (TRUE)
		{
			//1行読み取り
			fgets(&cReadTxt[0], MAX_STR, pFile);

			//読み込んだ文字列の中にコメントがあるかチェック
			char *pCharPos = strchr(&cReadTxt[0], CHR_COMMENT);
			if (pCharPos != nullptr)
			{//コメントアウト用の文字があった
				*pCharPos = CHR_END;//その文字以降を削除
			}

			//ファイルを読み切った
			if (feof(pFile) != 0)
			{//読み取り終了
				bRead = false;
				break;
			}

			//+++++++++++++++++++++++++
			//タイトルを読み取る
			//+++++++++++++++++++++++++
			pTitle = strtok(&cReadTxt[0], CHR_COMMA);

			//タイトルが無い
			if (pTitle == NULL) continue;

			//オブジェクトの総数を保存
			if (strcmp(pTitle, CODE_NUM_OBJECT) == 0) NUM_OBJECT = LoadInt(NULL, CHR_COMMA);

			//オブジェクトのファイルパスの場合
			else if (strcmp(pTitle, CODE_OBJECT_FILENAME) == 0)
			{
				//読み込んだオブジェクトの数が、総数を超えていない
				if (nReadObjCount >= MAX_MODEL) return;

				//文字列をコピーする
				SetFilePath(&ObjectFilePath[nReadObjCount][0], NULL, CHR_COMMA);

				//ファイルパス内の不要な文字列を削除する
				DeleteString(&ObjectFilePath[nReadObjCount][0], CHR_NEWLINE);

				//読み込んだ数を増加
				nReadObjCount++;
			}

			//Ⅹモデルの個性の場合
			else if (strcmp(pTitle, CODE_XMODEL_TYPE) == 0)
			{
				//番号読み取り
				const int nINDEX = LoadInt(NULL, CHR_COMMA);

				//個性読み取り
				type[nINDEX] = (CObjectX::XMODELTYPE)LoadInt(NULL, CHR_COMMA);

				//体力読み取り
				nLife[nINDEX] = LoadInt(NULL, CHR_COMMA);
			}

			//オブジェクト配置の場合
			else if (strcmp(pTitle, CODE_OBJECTSET) == 0)
			{
				//配置した数が、総数を超えていない
				if (nSetObject >= NUM_OBJECT) return;

				//インスタンスを生成する
				CObjectX *pObjX = new CObjectX;

				//インスタンス生成成功
				if (pObjX == NULL) return;

				//オブジェクトの配置情報を格納する
				int nINDEX = 0;
				D3DXVECTOR3 pos = VEC3_INIT, rot = VEC3_INIT;

				while (TRUE)
				{
					//次の１行読み取り
					fgets(&cReadTxt[0], MAX_STR, pFile);

					//+++++++++++++++++++++++++
					//タイトルを読み取る
					//+++++++++++++++++++++++++
					pTitle = strtok(&cReadTxt[0], CHR_COMMA);

					//オブジェクト設定終了の場合、設定処理へ
					if (strcmp(pTitle, CODE_END_OBJECTSET) == 0) break;

					//オブジェクト番号の場合
					else if (strcmp(pTitle, CODE_INDEX) == 0)nINDEX = LoadInt(NULL, CHR_COMMA);

					//位置設定の場合
					else if (strcmp(pTitle, CODE_POS) == 0)	pos = LoadVec3(NULL, CHR_COMMA);

					//向き設定の場合
					else if (strcmp(pTitle, CODE_ROT) == 0)	rot = LoadVec3(NULL, CHR_COMMA);
				}

				/*	初期化処理	*/	pObjX->Init(&ObjectFilePath[nINDEX][0], type[nINDEX], nLife[nINDEX]);
				/*	 情報設定   */	pObjX->SetTransrate(pos, VEC3_INIT, rot);
				/*カウントアップ*/	nSetObject++;
			}

			//宝石配置の場合
			else if (strcmp(pTitle, CODE_JEWELSET) == 0)
			{
				//配置した数が、総数を超えていない
				if (nSetJewel < NUM_JEWEL)
				{
					//インスタンスを生成する
					CJewel *pJewel = new CJewel;

					//インスタンス生成成功
					if (pJewel != NULL)
					{
						//オブジェクトの配置情報を格納する
						int nINDEX = 0;
						D3DXVECTOR3 pos = VEC3_INIT, rot = VEC3_INIT, GoalPos = VEC3_INIT;
						float fLength = 0.0f;

						while (TRUE)
						{
							//次の１行読み取り
							fgets(&cReadTxt[0], MAX_STR, pFile);

							//+++++++++++++++++++++++++
							//タイトルを読み取る
							//+++++++++++++++++++++++++
							pTitle = strtok(&cReadTxt[0], CHR_COMMA);

							//オブジェクト設定終了の場合
							if (strcmp(pTitle, CODE_END_JEWELSET) == 0)
							{
								break;	//オブジェクトの情報読み取りを終えて、設定処理へ
							}

							//オブジェクト番号の場合
							else if (strcmp(pTitle, CODE_INDEX) == 0)nINDEX = atoi(strtok(NULL, CHR_COMMA));

							//位置設定の場合
							else if (strcmp(pTitle, CODE_POS) == 0)	pos = LoadVec3(NULL, CHR_COMMA);

							//位置設定の場合
							else if (strcmp(pTitle, CODE_GOAL_POINT) == 0)	GoalPos = LoadVec3(NULL, CHR_COMMA);

							//位置設定の場合
							else if (strcmp(pTitle, CODE_GOAL_LENGTH) == 0)	fLength = LoadFloat(NULL, CHR_COMMA);
						}

						/*	初期化処理	*/	pJewel->Init(&ObjectFilePath[nINDEX][0], pos, GoalPos, fLength);
						/*カウントアップ*/	nSetJewel++;
					}
				}
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//========================
//敵の配置情報を読み込み
//========================
void CFile::LoadStageEnemy(const char *pPath)
{
	//ファイルを開く
	FILE *pFile = fopen(pPath, "r");

	//ファイルが開けなかったら引き返す
	if (pFile == NULL) return;

	//設定する敵の種類列挙
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_NONE = -1,	//設定する敵なし
		ENEMY_TYPE_CAMERA,		//防犯カメラ
		ENEMY_TYPE_ROBOT,		//ロボット
		ENEMY_TYPE_MAX,
	};

	//配置情報を格納する変数
	int nType = ENEMY_TYPE_NONE;	//設定する敵の種類
	char EnemyFilePath[ENEMY_TYPE_MAX][MAX_STR] = {{}};	//敵のXファイルのパス
	int nSetEnemy = 0;			//配置した敵の数を保存
	int nCntReadEnemyType = 0;	//読み込んだ敵の種類を格納
	D3DXVECTOR3 pos = VEC3_INIT;//移動量を保存
	D3DXVECTOR3 rot = VEC3_INIT;//回転量を保存

	//ロボット型の敵専用変数
	float fMoveSpeed = 0.0f;
	int nAttackPower = 1;
	int nNumAll = 0;
	D3DXVECTOR3 Point[6] = {};
	CEnemyRobot::CheckPoint CheckPoint = {};

	//防犯カメラ型の敵専用変数
	float fPatRot_Camera = 0.0f;

	do
	{
		//１行分の文字を格納
		char cReadTxt[MAX_STR] = {};
		char *pTitle = NULL;

		//1行読み取り
		fgets(&cReadTxt[0], MAX_STR, pFile);

		//読み込んだ文字列の中にコメントがあるかチェック
		DeleteString(&cReadTxt[0], CHR_COMMENT);

		//ファイルを読み切ったら読み取り終了
		if (feof(pFile) != 0)break;

		//+++++++++++++++++++++++++
		//タイトルを読み取る
		//+++++++++++++++++++++++++
		pTitle = strtok(&cReadTxt[0], CHR_COMMA);

		//敵のファイルパス
		if (strcmp(pTitle, "ENEMY_FILENAME") == 0)
		{
			//読み込んだ敵の種類が、種類の総数を超えていない
			if (nCntReadEnemyType >= ENEMY_TYPE_MAX) return;

			//文字列をコピーする
			SetFilePath(&EnemyFilePath[nCntReadEnemyType][0], NULL, CHR_COMMA);

			//ファイルパス内の不要な文字列を削除する
			DeleteString(&EnemyFilePath[nCntReadEnemyType][0], CHR_NEWLINE);

			//読み込んだ数を増加
			nCntReadEnemyType++;
		}

		//種類番号
		else if (strcmp(pTitle, CODE_INDEX) == 0)	nType = LoadInt(NULL, CHR_COMMA);

		//配置座標
		else if (strcmp(pTitle, CODE_POS) == 0)		pos = LoadVec3(NULL, CHR_COMMA);

		//配置方向
		else if (strcmp(pTitle, CODE_ROT) == 0)		rot = LoadVec3(NULL, CHR_COMMA);

		//攻撃力
		else if (strcmp(pTitle, "PATROL_ROT") == 0) fPatRot_Camera = LoadFloat(NULL, CHR_COMMA);

		//攻撃力
		else if (strcmp(pTitle, "ATTACKPOWER") == 0) nAttackPower = LoadInt(NULL, CHR_COMMA);

		//チェックポイントを設定
		else if (strcmp(pTitle, "SET_CHECKPOINT") == 0)	LoadCheckPoint(pFile, &fMoveSpeed, &nNumAll, &Point[0]);

		//敵を配置
		else if (strcmp(pTitle, "END_ENEMYSET") == 0)
		{
			switch (nType)
			{
				case ENEMY_TYPE_CAMERA: SetEnemyCamera(&EnemyFilePath[nType][0], pos, rot, fPatRot_Camera);	break;
				case ENEMY_TYPE_ROBOT:	SetEnemyRobot(&EnemyFilePath[nType][0], pos, rot, fMoveSpeed, nAttackPower, &nNumAll, &Point[0]);	break;
			}
		}

	} while (true);

	//ファイルを閉じる
	fclose(pFile);
}

//========================
//D3DXVECTOR2の各ベクトルを読み取り
//========================
D3DXVECTOR2 CFile::LoadVec2(char *pString, char *pPunc)
{
	D3DXVECTOR2 v = VEC2_INIT;

	v.x = LoadFloat(pString, pPunc);
	v.y = LoadFloat(pString, pPunc);

	return v;
}

//========================
//D3DXVECTOR2の各ベクトルを読み取り
//========================
D3DXVECTOR3 CFile::LoadVec3(char *pString, char *pPunc)
{
	D3DXVECTOR3 v = VEC3_INIT;

	v.x = LoadFloat(pString, pPunc);
	v.y = LoadFloat(pString, pPunc);
	v.z = LoadFloat(pString, pPunc);

	return v;
}

//========================
//チェックポイントを読み取り
//========================
void CFile::LoadCheckPoint(FILE *pFile, float *pMoveSpeed, int *pNumAll, D3DXVECTOR3 *pCheckPoint)
{
	//設定するポインタが無ければ引き返す
	if (pFile == NULL || pMoveSpeed == NULL || pNumAll == NULL || *pCheckPoint == NULL) return;

	int nCntCheckPoint = 0;

	do
	{
		//１行分の文字を格納
		char cReadTxt[MAX_STR] = {};
		char *pTitle = NULL;

		//1行読み取り
		fgets(&cReadTxt[0], MAX_STR, pFile);

		//読み込んだ文字列の中にコメントがあるかチェック
		DeleteString(&cReadTxt[0], CHR_COMMENT);

		//タイトルを読み取る
		pTitle = strtok(&cReadTxt[0], CHR_COMMA);

		//移動量読み取り
		if (strcmp(pTitle, "MOVE") == 0) *pMoveSpeed = LoadFloat(NULL, CHR_COMMA);

		//チェックポイントの総数
		else if (strcmp(pTitle, "NUM_CHECKPOINT") == 0)
		{
			//総数読み込み
			*pNumAll = LoadInt(NULL, CHR_COMMA);
		}

		//チェックポイントの位置を読み込み
		else if (strcmp(pTitle, "SET_POINT") == 0 && nCntCheckPoint < *pNumAll)
		{
			pCheckPoint[nCntCheckPoint] = LoadVec3(NULL, CHR_COMMA);
			nCntCheckPoint++;
		}

		//設定終了
		else if (strcmp(pTitle, "END_CHECKPOINT") == 0) break;

	} while (true);
}

//========================
//不要な文字を削除する
//========================
void CFile::DeleteString(char *pString, char cDeleteStr)
{
	//読み込んだ文字列の中にコメントがあるかチェック
	char *pCharPos = strchr(pString, cDeleteStr);

	//コメントアウト用の文字があったらその文字以降を削除
	if (pCharPos != nullptr) *pCharPos = CHR_END;
}

//========================
//ファイルパスを保存する
//========================
void CFile::SetFilePath(char *pStrage, char *pString, char *pPunc)
{
	//保存する先、区切り文字がないなら引き返す
	if(pStrage == NULL || pPunc == NULL) return;

	//文字列をコピーする
	strcpy(pStrage, strtok(pString, pPunc));
}

//========================
//カメラ型の敵を配置する
//========================
bool CFile::SetEnemyCamera(char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fPatRot)
{
	if (pFilePath == NULL) return false;

	//生成する
	CEnemySecurityCamera *pEnemy = CEnemySecurityCamera::Create(pFilePath);

	//生成失敗
	if (pEnemy == NULL) return false;

	//配置情報反映
	pEnemy->SetInfo(pos, rot, fPatRot);

	//生成成功
	return true;
}

//========================
//ロボット型の敵を配置する
//========================
bool CFile::SetEnemyRobot(char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fMoveSpeed, int nAttackPower, int *pNumAll, D3DXVECTOR3 *pCheckPoint)
{
	CEnemyRobot::CheckPoint CheckPoint = {};

	//設定情報をコピーする
	{
		CheckPoint.nIdx = 0;
		CheckPoint.fMove = fMoveSpeed;
		CheckPoint.nNumAll = *pNumAll;

		int size = sizeof(*pCheckPoint);
		CheckPoint.pPoint = new D3DXVECTOR3[*pNumAll];

		for (int nCntCheckPoint = 0; nCntCheckPoint < *pNumAll; nCntCheckPoint++)
		{
			memcpy(CheckPoint.pPoint[nCntCheckPoint], pCheckPoint[nCntCheckPoint], size);
		}
	}

	if (pFilePath == NULL) return false;

	//生成する
	CEnemyRobot *pEnemy = CEnemyRobot::Create(pFilePath);

	//生成失敗
	if (pEnemy == NULL) return false;

	//配置情報反映
	pEnemy->SetInfo(pos, rot, CheckPoint, nAttackPower);

	if (CheckPoint.pPoint != NULL)
	{
		delete[] CheckPoint.pPoint;
		CheckPoint.pPoint = NULL;
	}

	//生成成功
	return true;
}