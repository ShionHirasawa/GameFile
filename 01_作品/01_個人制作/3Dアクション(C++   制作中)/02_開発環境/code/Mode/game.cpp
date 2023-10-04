//================================================================================================
//
//�Q�[����ʏ���[game.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Main/file.h"
#include "game.h"
#include "../Object/object.h"
#include "../2D/object2D.h"
#include "../3D/object3D.h"
#include "../3D/billboard.h"
#include "../XFile/objectX.h"
#include "../XFile/enemy.h"
#include "../2D/bg.h"
#include "../3D/bullet.h"
#include "../3D/effect.h"
#include "../3D/particle.h"
#include "../3D/explosion.h"
#include "../3D/item.h"
#include "../2D/2Dblock.h"
#include "../Number/number.h"
#include "../Number/score.h"
#include "../Number/timer.h"
#include "../3D/camera.h"
#include "../3D/light.h"
#include "../3D/floor.h"
#include "../3D/treeBillboard.h"
#include "../XFile/jewel.h"
#include "../Player/player.h"

//�u���b�N�̔z�u��
#define SET_NUM_BLOCK	(3)
//�u���b�N�̃T�C�Y
#define BLOCK_SIZE		 (60.0f)
//�����߂̃u���b�N�T�C�Y
#define BLOCK_SMALL_SIZE (20.0f)

//�������t�@�C����
const char	*CGame::FILENAME_INTERIOR = "data\\CSV\\interior.csv";

//�I�u�W�F�N�g�z�u���t�@�C����
const char	*CGame::FILENAME_OBJECT = "data\\CSV\\object.csv";

//�G�̔z�u���t�@�C����
const char	*CGame::FILENAME_ENEMY = "data\\CSV\\enemy.csv";

//�A�C�e���̔z�u��
const int	CGame::SET_NUM_ITEM = 3;

//=======================================
//�R���X�g���N�^
//=======================================
CGame::CGame()
{

}

//=======================================
//�f�X�g���N�^
//=======================================
CGame::~CGame()
{

}

//=======================================
//����������
//=======================================
HRESULT CGame::Init(void)
{
	//�I�u�W�F�N�g�̃e�N�X�`���ǂݍ���
	TextureLoad();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�Q�[�����I�u�W�F�N�g����
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�X�R�A�I�u�W�F�N�g����
	CScore *pScore = CScore::Create();
	pScore->Set();

	//�^�C�}�[�I�u�W�F�N�g����
	CTimer *pTimer = CTimer::Create();
	pTimer->Set(180);

	//�v���C���[�I�u�W�F�N�g����
	CPlayer *pPlayer = CPlayer::Create();
	pPlayer->SetPos(D3DXVECTOR3(600.0f, 0.0f, 600.0f));

	//�J�����Ƀv���C���[����n��
	CManager::GetCamera()->SetPlayer(pPlayer);

	//================================================================
	//�X�e�[�W�z�u���ǂݍ���
	//================================================================
	//�t�@�C���C���X�^���X�𐶐�
	CFile *pFile = new CFile;
	if (pFile == NULL) return E_FAIL;

	SetInterior(pFile);	// ���� �ǂݍ��݁i�ǁA���A�V��Ȃǁj
	SetObject(pFile);	//�z�u���ǂݍ���
	SetEnemy(pFile);	//�G�̏��ǂݍ���

	delete pFile;	//�������J��
	pFile = NULL;

	//�Q�[����BGM�Đ�
	CManager::GetSound()->Play(CSound::BGM_GAME);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CGame::Uninit(void)
{
	//�t�F�[�h�ȊO�̃I�u�W�F�N�g�S�j��
	CObject::ExceptReleaseAll(CObject::TYPE_FADE);
}

//=======================================
//�X�V����
//=======================================
void CGame::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CGame::Draw(void)
{

}

//=======================================
//�I�u�W�F�N�g�̃e�N�X�`���ǂݍ���
//=======================================
void CGame::TextureLoad(void)
{
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�I�u�W�F�N�g�̃e�N�X�`���ǂ݂��݁i�Q�c�E�A���t�@�x�b�g��
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/*    �e    */	CBullet::Load();
	/*�G�t�F�N�g*/	CEffect::Load();
	/*    �G    */	CEnemy::Load();
	/*   ����   */	CExplosion::Load();
	/*�����e�N�X�`��*/CNumber::Load();
}

//=======================================
//�A�C�e���̔z�u
//=======================================
void CGame::SetItem(void)
{
	for (int nCntItem = 0; nCntItem < SET_NUM_ITEM; nCntItem++)
	{
		CItem *pItem = CItem::Create(D3DXVECTOR3(360.0f + 40.0f * nCntItem, 330.0f, 0.0f));
	}
}

//=======================================
//�X�e�[�W�̏���ǂȂǂ̓�����z�u
//=======================================
void CGame::SetInterior(CFile *pFile)
{
	//���E�ǂ����ׂč폜����
	CObject::ReleaseAll(CObject::TYPE_FLOOR);
	CObject::ReleaseAll(CObject::TYPE_WALL);

	//��������ǂݍ���
	pFile->LoadStageInterior(FILENAME_INTERIOR);
}

//=======================================
//�I�u�W�F�N�g�̔z�u
//=======================================
void CGame::SetObject(CFile *pFile)
{
	//�]���f�������ׂč폜����
	CObject::ReleaseAll(CObject::TYPE_XMODEL);

	//�X�e�[�W�ɔz�u����I�u�W�F�N�g����ǂݍ���
	pFile->LoadStageObject(FILENAME_OBJECT);
}

//=======================================
//�G�̔z�u
//=======================================
void CGame::SetEnemy(CFile *pFile)
{
	//�G�����ׂč폜����
	CObject::ReleaseAll(CObject::TYPE_ENEMY);

	//�G�̔z�u����ǂݍ���
	pFile->LoadStageEnemy(FILENAME_ENEMY);
}