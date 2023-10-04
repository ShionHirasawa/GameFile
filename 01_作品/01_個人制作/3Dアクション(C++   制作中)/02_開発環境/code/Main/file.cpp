//==========================================
//
//�t�@�C���ǂݍ��݃v���O����[file.cpp]
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
//�R���X�g���N�^
//========================
CFile::CFile()
{

}

//========================
//�f�X�g���N�^
//========================
CFile::~CFile()
{

}

//========================
//����������
//========================
void CFile::Init(void)
{

}

//========================
//�I������
//========================
void CFile::Uninit(void)
{

}

//========================
//���[�V�����ǂݍ��ݏ���
// �����P	*pPath�F���[�V�����̐ݒ�t�@�C���̃p�X
// �����Q	**ppModel�FX�t�@�C������ǂݎ�����p�[�c���i�[����
// �����R	*pMotion�F���[�V���������i�[����
// �����S	*pNumParts�F�p�[�c�̑������i�[����
// �����T	*pParentNumber�F�e�p�[�c�̐e�ԍ���ۑ�����
//========================
void CFile::LoadMotion(const char *pPath, CModel **ppModel, CMotion *pMotion, int *pNumParts, int *pParentNumber)
{
	//�t�@�C�����J��
	FILE *pFile = fopen(pPath, "r");

	//�t�@�C�����ǂݍ��߂�
	if (pFile != NULL)
	{
		/*�ǂݍ��ރp�^�[��*/	MOTION_READSTATE ReadState = MOTION_READSTATE_NONE;
		/*���[�V�������ۑ�*/	CMotion::MOTION_INFO motionInfo;
		//�ϐ�������
		int CounterMotionInfo = 0;
		int CounterKeyInfo = 0;
		int CounterKey = 0;
		int nIdxParts = 0;

		char aCode[MAX_STR];	//�P�s�ǂݎ��̂Ɏg�p����
		char *pSprit;			//�^�u������󔒂ȂǁA�����Ɏז��Ȃ��̂��i�[����
		bool bRead = false;		//�ǂݎ��𑱍s���邩�ǂ���
		int nReadModel = 0;		//�����̃p�[�c��ǂݎ������

		char XModelFilePath[MAX_PARTS][MAX_STR];//���f���p�[�c��X�t�@�C���̃p�X���i�[
		D3DXVECTOR3 pos[MAX_PARTS];		//		�ʒu	�i�z��ԍ��� [ nIdxParts ] ���g�p����
		D3DXVECTOR3 rot[MAX_PARTS];		//		����	�i�z��ԍ��� [ nIdxParts ] ���g�p����

		while (TRUE)
		{
			//�P�s�ǂݍ���
			fgets(&aCode[0], MAX_STR, pFile);

			//�R�����g�A�E�g�`�F�b�N
			char *pCharPos = strchr(&aCode[0], CHR_COMMENT);
			if (pCharPos != nullptr)
			{//�R�����g�A�E�g�p�̕�����������
				*pCharPos = CHR_END;//���̕����ȍ~���폜
			}

			//�^�u����
			while (aCode[0] == CHR_TAB)
			{
				char aCodeBackup[MAX_STR];
				strcpy(&aCodeBackup[0], &aCode[0]);//�ǂݍ��񂾂P�s��ۑ�����
				strcpy(&aCode[0], &aCodeBackup[1]);//���̃^�u�������O����������̕����ŏ㏑������
			}

			//�ǂݎ��J�n
			if (strncmp(&aCode[0], CODE_SCRIPT, sizeof CODE_SCRIPT - 1) == 0)bRead = true;

			//�ǂݎ��I��
			else if (strncmp(&aCode[0], CODE_END_SCRIPT, sizeof CODE_END_SCRIPT - 1) == 0 || aCode[0] == EOF && feof(pFile))	break;

			//++++++++++++++++++++++++++++++++
			//�ǂݎ��
			//++++++++++++++++++++++++++++++++
			else if (bRead == true)
			{
				//�ǂݎ����ɂ���ď����𕪊򂳂���
				switch (ReadState)
				{
					//******************************************************************
					//���ɍs���������m�F����
					//******************************************************************
					case MOTION_READSTATE_NONE:
					
					//++++++++++++++++++++++++++++++++++++++
					//���f���p�[�c�̑����������ꍇ
					//++++++++++++++++++++++++++++++++++++++
					if (strncmp(&aCode[0], CODE_NUM_MODEL, sizeof CODE_NUM_MODEL - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//��������
						*pNumParts = LoadInt(NULL, CHR_PAUSE);
					}

					//++++++++++++++++++++++++++++++++++++++
					//���f���p�[�c�̃t�@�C���p�X�������ꍇ
					//++++++++++++++++++++++++++++++++++++++
					if (strncmp(&aCode[0], CODE_MODEL_FILENAME, sizeof CODE_MODEL_FILENAME - 1) == 0)
					{
						//�ǂݍ��񂾃p�[�c�̐����ő吔�ɒB���Ă��Ȃ�
						if (nReadModel >= MAX_PARTS) return;

						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//�p�[�c�̃t�@�C���p�X��������
						strcpy(&XModelFilePath[nReadModel][0], strtok(NULL, CHR_PAUSE));

						//�^�u�����������Ă���΂���ȍ~������
						char *pCharPos = strchr(&XModelFilePath[nReadModel][0], CHR_TAB);
						{
							*pCharPos = CHR_END;
						}

						//�p�[�c�̓ǂݍ��񂾐������Z
						nReadModel++;
					}

					//++++++++++++++++++++++++++++++++++++++
					//�L�����N�^�[���̐ݒ肾�����ꍇ
					//++++++++++++++++++++++++++++++++++++++
					if (strncmp(&aCode[0], CODE_CHARACTERSET, sizeof CODE_CHARACTERSET - 1) == 0)
					{
						//���ɍs���������u�L�����N�^�[���ݒ�v�ɂ���
						ReadState = MOTION_READSTATE_CHARACTERSET;
					}

					//++++++++++++++++++++++++++++++++++++++
					//���[�V�������̐ݒ肾�����ꍇ
					//++++++++++++++++++++++++++++++++++++++
					if (strncmp(&aCode[0], CODE_MOTIONSET, sizeof CODE_MOTIONSET - 1) == 0)
					{
						//�ǂݍ��񂾃��[�V�����̐����A�S���[�V�����̐��𒴂��Ă��Ȃ�
						if (CounterMotionInfo < CMotion::MOTIONTYPE_MAX)
						{
							//���[�V�����̐ݒ��
							ReadState = MOTION_READSTATE_MOTIONSET;
						}
					}
					break;

					//******************************************************************
					//���f�����擾
					//******************************************************************
				case MOTION_READSTATE_CHARACTERSET:
					//�L�����N�^�[�ݒ�I��
					if (strncmp(&aCode[0], CODE_END_CHARACTERSET, sizeof CODE_END_CHARACTERSET - 1) == 0)
					{
						//���̏����擾��
						ReadState = MOTION_READSTATE_NONE;
					}

					//�p�[�c�ݒ��
					else if (strncmp(&aCode[0], CODE_PARTSSET, sizeof CODE_PARTSSET - 1) == 0)
					{
						ReadState = MOTION_READSTATE_PARTSSET;
					}
					break;

					//******************************************************************
					//�p�[�c���擾
					//******************************************************************
				case MOTION_READSTATE_PARTSSET:
					//�p�[�c�ݒ�I��
					if (strncmp(&aCode[0], CODE_END_PARTSSET, sizeof CODE_END_PARTSSET - 1) == 0)
					{
						//�擾�����p�[�c�ԍ��̏������f���ɑ���
						ppModel[nIdxParts] = CModel::Create(XModelFilePath[nIdxParts], pos[nIdxParts], rot[nIdxParts]);
						
						//�L�����N�^�[�ݒ��Ԃɖ߂�
						ReadState = MOTION_READSTATE_CHARACTERSET;
					}

					//�p�[�c�ԍ�
					else if (strncmp(&aCode[0], CODE_INDEX, sizeof CODE_INDEX - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//�p�[�c�ԍ��擾
						nIdxParts = LoadInt(NULL, CHR_PAUSE);
					}

					//�e�̔ԍ�
					else if (strncmp(&aCode[0], CODE_PARENT, sizeof CODE_PARENT - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//�e�ԍ��擾
						pParentNumber[nIdxParts] = LoadInt(NULL, CHR_PAUSE);
					}

					//�ʒu�ݒ�
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						pos[nIdxParts].x = LoadFloat(NULL, CHR_PAUSE);//X���W�ǂݎ��
						pos[nIdxParts].y = LoadFloat(NULL, CHR_PAUSE);//Y���W�ǂݎ��
						pos[nIdxParts].z = LoadFloat(NULL, CHR_PAUSE);//Z���W�ǂݎ��
					}

					//�����ݒ�
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						rot[nIdxParts].x = LoadFloat(NULL, CHR_PAUSE);//X�̉�]�l�ǂݎ��
						rot[nIdxParts].y = LoadFloat(NULL, CHR_PAUSE);//Y�̉�]�l�ǂݎ��
						rot[nIdxParts].z = LoadFloat(NULL, CHR_PAUSE);//Z�̉�]�l�ǂݎ��
					}
					break;

					//******************************************************************
					//���[�V�������擾
					//******************************************************************
				case MOTION_READSTATE_MOTIONSET:
					if (strncmp(&aCode[0], CODE_KEYSET, sizeof CODE_KEYSET - 1) == 0)
					{
						ReadState = MOTION_READSTATE_KEYSET;
					}
					else if (strncmp(&aCode[0], CODE_END_MOTIONSET, sizeof CODE_END_MOTIONSET - 1) == 0)
					{
						pMotion->Set(CounterMotionInfo);	//���[�V�����ԍ���ݒ�
						pMotion->SetInfo(motionInfo);		//���[�V����������

						CounterMotionInfo++;
						CounterKeyInfo = 0;
						ReadState = MOTION_READSTATE_NONE;
					}
					else if (strncmp(&aCode[0], CODE_LOOP, sizeof CODE_LOOP - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//���[�v�ۓǂݎ��
						motionInfo.bLoop = LoadInt(NULL, CHR_PAUSE) == 1 ? true : false;
					}
					else if (strncmp(&aCode[0], CODE_NUM_KEY, sizeof CODE_NUM_KEY - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						//�L�[���ǂݎ��
						motionInfo.nNumKey = LoadInt(NULL, CHR_PAUSE);
					}
					break;

					//******************************************************************
					//�L�[���ݒ�
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
						pSprit = strtok(&aCode[0], CHR_PAUSE);	//�ݒ肷�鍀�ږ�������

						//�t���[�����ǂݎ��
						motionInfo.aKeyInfo[CounterKeyInfo].nFrame = LoadInt(NULL, CHR_PAUSE);
					}
					break;

					//******************************************************************
					//�L�[���擾
					//******************************************************************
				case MOTION_READSTATE_KEY:
					if (strncmp(&aCode[0], CODE_END_KEY, sizeof CODE_END_KEY - 1) == 0)
					{
						CounterKey++;
						ReadState = MOTION_READSTATE_KEYSET;
					}
					else if (strncmp(&aCode[0], CODE_POS, sizeof CODE_POS - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fPosX = LoadFloat(NULL, CHR_PAUSE);//X���W�ǂݎ��
						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fPosY = LoadFloat(NULL, CHR_PAUSE);//Y���W�ǂݎ��
						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fPosZ = LoadFloat(NULL, CHR_PAUSE);//Z���W�ǂݎ��
					}
					else if (strncmp(&aCode[0], CODE_ROT, sizeof CODE_ROT - 1) == 0)
					{
						//�ݒ肷�鍀�ږ�������
						pSprit = strtok(&aCode[0], CHR_PAUSE);

						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fRotX = LoadFloat(NULL, CHR_PAUSE);//X�����ǂݎ��
						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fRotY = LoadFloat(NULL, CHR_PAUSE);//Y�����ǂݎ��
						motionInfo.aKeyInfo[CounterKeyInfo].aKey[CounterKey].fRotZ = LoadFloat(NULL, CHR_PAUSE);//Z�����ǂݎ��
					}
					break;
				}
			}
		}

		//�t�@�C������
		fclose(pFile);
	}
}

//========================
//�X�e�[�W�̓�������ǂݍ���
//========================
void CFile::LoadStageInterior(const char *pPath)
{
	//�t�@�C�����J��
	FILE *pFile = fopen(pPath, "r");

	//�t�@�C�����J����
	if (pFile != NULL)
	{
		//�I�u�W�F�N�g�̔z�u�����i�[����
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

		//�������ǂݎ��邾���S�ēǂݎ��
		while (TRUE)
		{
			//1�s�ǂݎ��
			fgets(&cReadTxt[0], MAX_STR, pFile);
			strcpy(&temp[0], &cReadTxt[0]);

			//�ǂݍ��񂾕�����̒��ɃR�����g�����邩�`�F�b�N
			char *pCharPos = strchr(&cReadTxt[0], CHR_COMMENT);

			//�R�����g�A�E�g�p�̕������������炻�̕����ȍ~���폜
			if (pCharPos != nullptr)*pCharPos = CHR_END;

			//�t�@�C����ǂݐ؂�����I��
			if (feof(pFile) != 0)break;

			//+++++++++++++++++++++++++
			//�^�C�g����ǂݎ��
			//+++++++++++++++++++++++++
			pTitle = strtok(&cReadTxt[0], CHR_COMMA);

			//�^�C�g������������
			if (pTitle == NULL)	continue;

			//+++++++++++++++++++++++++
			//���ǂݎ��I��
			//+++++++++++++++++++++++++
			//���̏ꍇ
			if (strcmp(pTitle, CODE_END_FLOORSET) == 0)
			{
				//���C���X�^���X����
				CFloor *pFloor = CFloor::Create();

				//�C���X�^���X��������
				if (pFloor == NULL) return;

				//�ʒu�E�T�C�Y���𔽉f
				pFloor->Load("data\\TEXTURE\\carpet64.png");
				pFloor->Set(pos, rot, size, tex);
				nNumSetFloor++;
			}

			//�ǂ̏ꍇ
			else if (strcmp(pTitle, CODE_END_WALLSET) == 0)
			{
				//�ǃC���X�^���X����
				CWall *pWall = CWall::Create();

				//�C���X�^���X��������
				if (pWall != NULL)
				{
					//�ʒu�E�T�C�Y���𔽉f
					pWall->Set(pos, rot, size, tex);
					nNumSetWall++;
				}
			}

			//�ʒu�ݒ�̏ꍇ
			else if (strcmp(pTitle, CODE_POS) == 0)	pos = LoadVec3(NULL, CHR_COMMA);

			//�ʒu�ݒ�̏ꍇ
			else if (strcmp(pTitle, CODE_ROT) == 0)	rot = LoadVec3(NULL, CHR_COMMA);

			//�T�C�Y�ݒ�̏ꍇ
			else if (strcmp(pTitle, CODE_SIZE) == 0)size = LoadVec3(NULL, CHR_COMMA);

			//�e�N�X�`���T�C�Y�ݒ�̏ꍇ
			else if (strcmp(pTitle, CODE_TEX_SIZE) == 0)tex = LoadVec2(NULL, CHR_COMMA);
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//========================
//�X�e�[�W�̔z�u����ǂݍ���
//========================
void CFile::LoadStageObject(const char *pPath)
{
	//�t�@�C�����J��
	FILE *pFile = fopen(pPath, "r");

	//�t�@�C�����J����
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

		//�������ǂݎ��邾���S�ēǂݎ��
		while (TRUE)
		{
			//1�s�ǂݎ��
			fgets(&cReadTxt[0], MAX_STR, pFile);

			//�ǂݍ��񂾕�����̒��ɃR�����g�����邩�`�F�b�N
			char *pCharPos = strchr(&cReadTxt[0], CHR_COMMENT);
			if (pCharPos != nullptr)
			{//�R�����g�A�E�g�p�̕�����������
				*pCharPos = CHR_END;//���̕����ȍ~���폜
			}

			//�t�@�C����ǂݐ؂���
			if (feof(pFile) != 0)
			{//�ǂݎ��I��
				bRead = false;
				break;
			}

			//+++++++++++++++++++++++++
			//�^�C�g����ǂݎ��
			//+++++++++++++++++++++++++
			pTitle = strtok(&cReadTxt[0], CHR_COMMA);

			//�^�C�g��������
			if (pTitle == NULL) continue;

			//�I�u�W�F�N�g�̑�����ۑ�
			if (strcmp(pTitle, CODE_NUM_OBJECT) == 0) NUM_OBJECT = LoadInt(NULL, CHR_COMMA);

			//�I�u�W�F�N�g�̃t�@�C���p�X�̏ꍇ
			else if (strcmp(pTitle, CODE_OBJECT_FILENAME) == 0)
			{
				//�ǂݍ��񂾃I�u�W�F�N�g�̐����A�����𒴂��Ă��Ȃ�
				if (nReadObjCount >= MAX_MODEL) return;

				//��������R�s�[����
				SetFilePath(&ObjectFilePath[nReadObjCount][0], NULL, CHR_COMMA);

				//�t�@�C���p�X���̕s�v�ȕ�������폜����
				DeleteString(&ObjectFilePath[nReadObjCount][0], CHR_NEWLINE);

				//�ǂݍ��񂾐��𑝉�
				nReadObjCount++;
			}

			//�]���f���̌��̏ꍇ
			else if (strcmp(pTitle, CODE_XMODEL_TYPE) == 0)
			{
				//�ԍ��ǂݎ��
				const int nINDEX = LoadInt(NULL, CHR_COMMA);

				//���ǂݎ��
				type[nINDEX] = (CObjectX::XMODELTYPE)LoadInt(NULL, CHR_COMMA);

				//�̗͓ǂݎ��
				nLife[nINDEX] = LoadInt(NULL, CHR_COMMA);
			}

			//�I�u�W�F�N�g�z�u�̏ꍇ
			else if (strcmp(pTitle, CODE_OBJECTSET) == 0)
			{
				//�z�u���������A�����𒴂��Ă��Ȃ�
				if (nSetObject >= NUM_OBJECT) return;

				//�C���X�^���X�𐶐�����
				CObjectX *pObjX = new CObjectX;

				//�C���X�^���X��������
				if (pObjX == NULL) return;

				//�I�u�W�F�N�g�̔z�u�����i�[����
				int nINDEX = 0;
				D3DXVECTOR3 pos = VEC3_INIT, rot = VEC3_INIT;

				while (TRUE)
				{
					//���̂P�s�ǂݎ��
					fgets(&cReadTxt[0], MAX_STR, pFile);

					//+++++++++++++++++++++++++
					//�^�C�g����ǂݎ��
					//+++++++++++++++++++++++++
					pTitle = strtok(&cReadTxt[0], CHR_COMMA);

					//�I�u�W�F�N�g�ݒ�I���̏ꍇ�A�ݒ菈����
					if (strcmp(pTitle, CODE_END_OBJECTSET) == 0) break;

					//�I�u�W�F�N�g�ԍ��̏ꍇ
					else if (strcmp(pTitle, CODE_INDEX) == 0)nINDEX = LoadInt(NULL, CHR_COMMA);

					//�ʒu�ݒ�̏ꍇ
					else if (strcmp(pTitle, CODE_POS) == 0)	pos = LoadVec3(NULL, CHR_COMMA);

					//�����ݒ�̏ꍇ
					else if (strcmp(pTitle, CODE_ROT) == 0)	rot = LoadVec3(NULL, CHR_COMMA);
				}

				/*	����������	*/	pObjX->Init(&ObjectFilePath[nINDEX][0], type[nINDEX], nLife[nINDEX]);
				/*	 ���ݒ�   */	pObjX->SetTransrate(pos, VEC3_INIT, rot);
				/*�J�E���g�A�b�v*/	nSetObject++;
			}

			//��Δz�u�̏ꍇ
			else if (strcmp(pTitle, CODE_JEWELSET) == 0)
			{
				//�z�u���������A�����𒴂��Ă��Ȃ�
				if (nSetJewel < NUM_JEWEL)
				{
					//�C���X�^���X�𐶐�����
					CJewel *pJewel = new CJewel;

					//�C���X�^���X��������
					if (pJewel != NULL)
					{
						//�I�u�W�F�N�g�̔z�u�����i�[����
						int nINDEX = 0;
						D3DXVECTOR3 pos = VEC3_INIT, rot = VEC3_INIT, GoalPos = VEC3_INIT;
						float fLength = 0.0f;

						while (TRUE)
						{
							//���̂P�s�ǂݎ��
							fgets(&cReadTxt[0], MAX_STR, pFile);

							//+++++++++++++++++++++++++
							//�^�C�g����ǂݎ��
							//+++++++++++++++++++++++++
							pTitle = strtok(&cReadTxt[0], CHR_COMMA);

							//�I�u�W�F�N�g�ݒ�I���̏ꍇ
							if (strcmp(pTitle, CODE_END_JEWELSET) == 0)
							{
								break;	//�I�u�W�F�N�g�̏��ǂݎ����I���āA�ݒ菈����
							}

							//�I�u�W�F�N�g�ԍ��̏ꍇ
							else if (strcmp(pTitle, CODE_INDEX) == 0)nINDEX = atoi(strtok(NULL, CHR_COMMA));

							//�ʒu�ݒ�̏ꍇ
							else if (strcmp(pTitle, CODE_POS) == 0)	pos = LoadVec3(NULL, CHR_COMMA);

							//�ʒu�ݒ�̏ꍇ
							else if (strcmp(pTitle, CODE_GOAL_POINT) == 0)	GoalPos = LoadVec3(NULL, CHR_COMMA);

							//�ʒu�ݒ�̏ꍇ
							else if (strcmp(pTitle, CODE_GOAL_LENGTH) == 0)	fLength = LoadFloat(NULL, CHR_COMMA);
						}

						/*	����������	*/	pJewel->Init(&ObjectFilePath[nINDEX][0], pos, GoalPos, fLength);
						/*�J�E���g�A�b�v*/	nSetJewel++;
					}
				}
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//========================
//�G�̔z�u����ǂݍ���
//========================
void CFile::LoadStageEnemy(const char *pPath)
{
	//�t�@�C�����J��
	FILE *pFile = fopen(pPath, "r");

	//�t�@�C�����J���Ȃ�����������Ԃ�
	if (pFile == NULL) return;

	//�ݒ肷��G�̎�ޗ�
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_NONE = -1,	//�ݒ肷��G�Ȃ�
		ENEMY_TYPE_CAMERA,		//�h�ƃJ����
		ENEMY_TYPE_ROBOT,		//���{�b�g
		ENEMY_TYPE_MAX,
	};

	//�z�u�����i�[����ϐ�
	int nType = ENEMY_TYPE_NONE;	//�ݒ肷��G�̎��
	char EnemyFilePath[ENEMY_TYPE_MAX][MAX_STR] = {{}};	//�G��X�t�@�C���̃p�X
	int nSetEnemy = 0;			//�z�u�����G�̐���ۑ�
	int nCntReadEnemyType = 0;	//�ǂݍ��񂾓G�̎�ނ��i�[
	D3DXVECTOR3 pos = VEC3_INIT;//�ړ��ʂ�ۑ�
	D3DXVECTOR3 rot = VEC3_INIT;//��]�ʂ�ۑ�

	//���{�b�g�^�̓G��p�ϐ�
	float fMoveSpeed = 0.0f;
	int nAttackPower = 1;
	int nNumAll = 0;
	D3DXVECTOR3 Point[6] = {};
	CEnemyRobot::CheckPoint CheckPoint = {};

	//�h�ƃJ�����^�̓G��p�ϐ�
	float fPatRot_Camera = 0.0f;

	do
	{
		//�P�s���̕������i�[
		char cReadTxt[MAX_STR] = {};
		char *pTitle = NULL;

		//1�s�ǂݎ��
		fgets(&cReadTxt[0], MAX_STR, pFile);

		//�ǂݍ��񂾕�����̒��ɃR�����g�����邩�`�F�b�N
		DeleteString(&cReadTxt[0], CHR_COMMENT);

		//�t�@�C����ǂݐ؂�����ǂݎ��I��
		if (feof(pFile) != 0)break;

		//+++++++++++++++++++++++++
		//�^�C�g����ǂݎ��
		//+++++++++++++++++++++++++
		pTitle = strtok(&cReadTxt[0], CHR_COMMA);

		//�G�̃t�@�C���p�X
		if (strcmp(pTitle, "ENEMY_FILENAME") == 0)
		{
			//�ǂݍ��񂾓G�̎�ނ��A��ނ̑����𒴂��Ă��Ȃ�
			if (nCntReadEnemyType >= ENEMY_TYPE_MAX) return;

			//��������R�s�[����
			SetFilePath(&EnemyFilePath[nCntReadEnemyType][0], NULL, CHR_COMMA);

			//�t�@�C���p�X���̕s�v�ȕ�������폜����
			DeleteString(&EnemyFilePath[nCntReadEnemyType][0], CHR_NEWLINE);

			//�ǂݍ��񂾐��𑝉�
			nCntReadEnemyType++;
		}

		//��ޔԍ�
		else if (strcmp(pTitle, CODE_INDEX) == 0)	nType = LoadInt(NULL, CHR_COMMA);

		//�z�u���W
		else if (strcmp(pTitle, CODE_POS) == 0)		pos = LoadVec3(NULL, CHR_COMMA);

		//�z�u����
		else if (strcmp(pTitle, CODE_ROT) == 0)		rot = LoadVec3(NULL, CHR_COMMA);

		//�U����
		else if (strcmp(pTitle, "PATROL_ROT") == 0) fPatRot_Camera = LoadFloat(NULL, CHR_COMMA);

		//�U����
		else if (strcmp(pTitle, "ATTACKPOWER") == 0) nAttackPower = LoadInt(NULL, CHR_COMMA);

		//�`�F�b�N�|�C���g��ݒ�
		else if (strcmp(pTitle, "SET_CHECKPOINT") == 0)	LoadCheckPoint(pFile, &fMoveSpeed, &nNumAll, &Point[0]);

		//�G��z�u
		else if (strcmp(pTitle, "END_ENEMYSET") == 0)
		{
			switch (nType)
			{
				case ENEMY_TYPE_CAMERA: SetEnemyCamera(&EnemyFilePath[nType][0], pos, rot, fPatRot_Camera);	break;
				case ENEMY_TYPE_ROBOT:	SetEnemyRobot(&EnemyFilePath[nType][0], pos, rot, fMoveSpeed, nAttackPower, &nNumAll, &Point[0]);	break;
			}
		}

	} while (true);

	//�t�@�C�������
	fclose(pFile);
}

//========================
//D3DXVECTOR2�̊e�x�N�g����ǂݎ��
//========================
D3DXVECTOR2 CFile::LoadVec2(char *pString, char *pPunc)
{
	D3DXVECTOR2 v = VEC2_INIT;

	v.x = LoadFloat(pString, pPunc);
	v.y = LoadFloat(pString, pPunc);

	return v;
}

//========================
//D3DXVECTOR2�̊e�x�N�g����ǂݎ��
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
//�`�F�b�N�|�C���g��ǂݎ��
//========================
void CFile::LoadCheckPoint(FILE *pFile, float *pMoveSpeed, int *pNumAll, D3DXVECTOR3 *pCheckPoint)
{
	//�ݒ肷��|�C���^��������Έ����Ԃ�
	if (pFile == NULL || pMoveSpeed == NULL || pNumAll == NULL || *pCheckPoint == NULL) return;

	int nCntCheckPoint = 0;

	do
	{
		//�P�s���̕������i�[
		char cReadTxt[MAX_STR] = {};
		char *pTitle = NULL;

		//1�s�ǂݎ��
		fgets(&cReadTxt[0], MAX_STR, pFile);

		//�ǂݍ��񂾕�����̒��ɃR�����g�����邩�`�F�b�N
		DeleteString(&cReadTxt[0], CHR_COMMENT);

		//�^�C�g����ǂݎ��
		pTitle = strtok(&cReadTxt[0], CHR_COMMA);

		//�ړ��ʓǂݎ��
		if (strcmp(pTitle, "MOVE") == 0) *pMoveSpeed = LoadFloat(NULL, CHR_COMMA);

		//�`�F�b�N�|�C���g�̑���
		else if (strcmp(pTitle, "NUM_CHECKPOINT") == 0)
		{
			//�����ǂݍ���
			*pNumAll = LoadInt(NULL, CHR_COMMA);
		}

		//�`�F�b�N�|�C���g�̈ʒu��ǂݍ���
		else if (strcmp(pTitle, "SET_POINT") == 0 && nCntCheckPoint < *pNumAll)
		{
			pCheckPoint[nCntCheckPoint] = LoadVec3(NULL, CHR_COMMA);
			nCntCheckPoint++;
		}

		//�ݒ�I��
		else if (strcmp(pTitle, "END_CHECKPOINT") == 0) break;

	} while (true);
}

//========================
//�s�v�ȕ������폜����
//========================
void CFile::DeleteString(char *pString, char cDeleteStr)
{
	//�ǂݍ��񂾕�����̒��ɃR�����g�����邩�`�F�b�N
	char *pCharPos = strchr(pString, cDeleteStr);

	//�R�����g�A�E�g�p�̕������������炻�̕����ȍ~���폜
	if (pCharPos != nullptr) *pCharPos = CHR_END;
}

//========================
//�t�@�C���p�X��ۑ�����
//========================
void CFile::SetFilePath(char *pStrage, char *pString, char *pPunc)
{
	//�ۑ������A��؂蕶�����Ȃ��Ȃ�����Ԃ�
	if(pStrage == NULL || pPunc == NULL) return;

	//��������R�s�[����
	strcpy(pStrage, strtok(pString, pPunc));
}

//========================
//�J�����^�̓G��z�u����
//========================
bool CFile::SetEnemyCamera(char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fPatRot)
{
	if (pFilePath == NULL) return false;

	//��������
	CEnemySecurityCamera *pEnemy = CEnemySecurityCamera::Create(pFilePath);

	//�������s
	if (pEnemy == NULL) return false;

	//�z�u��񔽉f
	pEnemy->SetInfo(pos, rot, fPatRot);

	//��������
	return true;
}

//========================
//���{�b�g�^�̓G��z�u����
//========================
bool CFile::SetEnemyRobot(char *pFilePath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fMoveSpeed, int nAttackPower, int *pNumAll, D3DXVECTOR3 *pCheckPoint)
{
	CEnemyRobot::CheckPoint CheckPoint = {};

	//�ݒ�����R�s�[����
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

	//��������
	CEnemyRobot *pEnemy = CEnemyRobot::Create(pFilePath);

	//�������s
	if (pEnemy == NULL) return false;

	//�z�u��񔽉f
	pEnemy->SetInfo(pos, rot, CheckPoint, nAttackPower);

	if (CheckPoint.pPoint != NULL)
	{
		delete[] CheckPoint.pPoint;
		CheckPoint.pPoint = NULL;
	}

	//��������
	return true;
}