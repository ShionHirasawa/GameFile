//================================================================================================
//
//�`���[�g���A����ʏ���[tutorial.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Main/file.h"
#include "tutorial.h"
#include "../Object/object.h"
#include "../2D/object2D.h"
#include "../2D/Polygon2D.h"
#include "../2D/fade.h"
#include "../3D/object3D.h"
#include "../3D/floor.h"
#include "../3D/wall.h"
#include "../3D/camera.h"
#include "../3D/billboard.h"
#include "../3D/PolygonBill.h"
#include "../3D/bullet.h"
#include "../3D/effect.h"
#include "../3D/explosion.h"
#include "../3D/Polygon3D.h"
#include "../Number/number.h"
#include "../XFile/objectX.h"
#include "../XFile/enemy.h"
#include "../Player/player.h"

//��������e�N�X�`�����
const D3DXVECTOR3	CTutorial::CONTROLL_TEX_CENTER_POS = D3DXVECTOR3(SCREEN_CENTER_POS);
const float			CTutorial::CONTROLL_TEX_CENTER_WIDTH = 600.0f;
const float			CTutorial::CONTROLL_TEX_CENTER_HEIGHT = 300.0f;
const D3DXVECTOR3	CTutorial::CONTROLL_TEX_WIPE_POS = D3DXVECTOR3(210.0f, 120.0f, 0.0f);
const float			CTutorial::CONTROLL_TEX_WIPE_WIDTH = 280.0f;
const float			CTutorial::CONTROLL_TEX_WIPE_HEIGHT = 140.0f;

//�I�u�W�F�N�g�̌������e�N�X�`�����
//�߂����̃T�C�Y���Ȃ�
const float CTutorial::OBJ_IDENTITY_TEX_NEAR_WIDTH = 80.0f;
const float CTutorial::OBJ_IDENTITY_TEX_NEAR_HEIGHT = 60.0f;
const float CTutorial::OBJ_IDENTITY_NEAR_SET_POS_Y = 120.0f;

//�������̃T�C�Y���Ȃ�
const float CTutorial::OBJ_IDENTITY_TEX_FAR_WIDTH = 60.0f;
const float CTutorial::OBJ_IDENTITY_TEX_FAR_HEIGHT = 15.0f;
const float CTutorial::OBJ_IDENTITY_FAR_SET_POS_Y = 80.0f;
const float CTutorial::OBJ_IDENTITY_FAR_TEX_Y = 0.2f;

//���̑����ʏ��
const float CTutorial::OBJ_IDENTITY_LENGTH = 90.0f;
const D3DXVECTOR3 CTutorial::OBJ_IDENTITY_SET_POS[NUM_OBJ_IDENTITY_TEXTURE] = {
	D3DXVECTOR3(+200.0f, OBJ_IDENTITY_FAR_SET_POS_Y, -100.0f),	//�v�����^�[
	D3DXVECTOR3(+200.0f, OBJ_IDENTITY_FAR_SET_POS_Y, -300.0f),	//�i�{�[��
	D3DXVECTOR3(-200.0f, OBJ_IDENTITY_FAR_SET_POS_Y, -100.0f),	//�ؔ�
	D3DXVECTOR3(-200.0f, OBJ_IDENTITY_FAR_SET_POS_Y, -300.0f),	//��
};

char *CTutorial::s_pControllTexturePath[NUM_CONTROLL_TEXTURE] = {
	"data\\TUTORIAL\\tutorial000.png",
	"data\\TUTORIAL\\tutorial001.png",
};

char *CTutorial::s_pObjectTexturePath[NUM_OBJ_IDENTITY_TEXTURE] = {
	"data\\TUTORIAL\\Identity000.png",
	"data\\TUTORIAL\\Identity001.png",
	"data\\TUTORIAL\\Identity002.png",
	"data\\TUTORIAL\\Identity003.png",
};

//=======================================
//�R���X�g���N�^
//=======================================
CTutorial::CTutorial()
{
	//�����o�ϐ��N���A
	m_pPlayer = NULL;

	//��������e�N�X�`���̕ϐ��N���A
	m_Controll.pPoly2D = NULL;
	m_Controll.nTexID = 0;
	m_Controll.bDispChange = true;
	m_Controll.pos =		VEC3_INIT;
	m_Controll.fWidth = 0.0f;
	m_Controll.fHeight = 0.0f;

	//�I�u�W�F�N�g�̌������e�N�X�`���̕ϐ��N���A
	for (int nCntObj = 0; nCntObj < NUM_CONTROLL_TEXTURE; nCntObj++)
	{
		m_ObjIdentity[nCntObj].pPolyBill = NULL;
		m_ObjIdentity[nCntObj].pos = VEC3_INIT;
		m_ObjIdentity[nCntObj].fWidth = 0.0f;
		m_ObjIdentity[nCntObj].fHeight = 0.0f;
		m_ObjIdentity[nCntObj].fTexV = 0.0f;
	}
}

//=======================================
//�f�X�g���N�^
//=======================================
CTutorial::~CTutorial()
{

}

//=======================================
//����������
//=======================================
HRESULT CTutorial::Init(void)
{
	//�e�N�X�`����ǂݍ���
	TextureLoad();

	//�X�e�[�W�z�u
	SetStage();

	//BGM�Đ�
	CManager::GetSound()->Play(CSound::BGM_TUTORIAL);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CTutorial::SetStage(void)
{
	//�`���[�g���A���Ŏg���I�u�W�F�N�g�𑍔j��
	CObject::ReleaseAll(CObject::TYPE_WALL);
	CObject::ReleaseAll(CObject::TYPE_FLOOR);
	CObject::ReleaseAll(CObject::TYPE_PLAYER);
	CObject::ReleaseAll(CObject::TYPE_ENEMY);
	CObject::ReleaseAll(CObject::TYPE_ROBOT);
	CObject::ReleaseAll(CObject::TYPE_XMODEL);
	CObject::ReleaseAll(CObject::TYPE_JEWEL);
	CObject::ReleaseAll(CObject::TYPE_POLY_2D);
	CObject::ReleaseAll(CObject::TYPE_POLY_3D);
	CObject::ReleaseAll(CObject::TYPE_POLY_BILL);
	CObject::ReleaseAll(CObject::TYPE_MESH);

	//�t���A
	{
		//�e�N�X�`���ǂݍ���
		CFloor *pFloor = CFloor::Create();
		pFloor->Load("data\\TUTORIAL\\carpet001.jpg");
		pFloor->Set(D3DXVECTOR3(0, 0, 200), VEC3_INIT, D3DXVECTOR3(100.0f, 0.0f, 200.0f), D3DXVECTOR2(3.0f, 6.0f));
	}

	//�v���C���[����
	{
		m_pPlayer = CPlayer::Create();
		m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, -900.0f));

		//�J�����Ƀv���C���[����n��
		CManager::GetCamera()->SetPlayer(m_pPlayer);
	}

	//�X�e�[�W�z�u���ǂݍ���
	{
		//�t�@�C���C���X�^���X�𐶐�
		CFile *pFile = new CFile;
		if (pFile == NULL) return;

		//�Ǎ�����
		pFile->LoadStageInterior("data\\TUTORIAL\\interior.csv");
		pFile->LoadStageObject("data\\TUTORIAL\\object.csv");
		pFile->LoadStageEnemy("data\\TUTORIAL\\enemy.csv");

		//�������J��
		delete pFile;
		pFile = NULL;
	}

	//�G�����|���S��
	{
		CPoly3D *pEnemy = CPoly3D::Create("data\\TUTORIAL\\enemyTuto000.png");
		pEnemy->SetRotation(VEC3_INIT);
		pEnemy->SetInfo(D3DXVECTOR3(0.0f, 60.0f, 519.0f), 50.0f, 40.0f, 0.0f);
		pEnemy->DispSwitch(true);
	}

	//��������|���S�����ݒ�
	m_Controll.pos = CONTROLL_TEX_CENTER_POS;
	m_Controll.fWidth = CONTROLL_TEX_CENTER_WIDTH;
	m_Controll.fHeight = CONTROLL_TEX_CENTER_HEIGHT;
	m_Controll.pPoly2D = CPoly2D::Create(s_pControllTexturePath[0]);
	SwapControllTexture();

	//�������|���S�����ݒ�
	for (int nCntObj = 0; nCntObj < NUM_OBJ_IDENTITY_TEXTURE; nCntObj++)
	{
		//�ϐ��������̂ŏk�߂�
		ObjIdentity *pObj = &m_ObjIdentity[nCntObj];

		//�ʒu���ݒ�
		pObj->pos = OBJ_IDENTITY_SET_POS[nCntObj];

		//�T�C�Y�����������p�ɐݒ�
		pObj->fWidth = OBJ_IDENTITY_TEX_FAR_WIDTH;
		pObj->fHeight = OBJ_IDENTITY_TEX_FAR_HEIGHT;
		pObj->fTexV = OBJ_IDENTITY_FAR_TEX_Y;

		//�T�C�Y��ς��Ă���
		pObj->pPolyBill = CPolyBill::Create(s_pObjectTexturePath[nCntObj]);
		pObj->pPolyBill->ZTestSwitch(false);
		pObj->pPolyBill->SetInfo(pObj->pos, pObj->fWidth, pObj->fHeight, 0.0f, VEC2_INIT, 1.0f, pObj->fTexV);
	}
}

//=======================================
//�I�u�W�F�N�g�̃e�N�X�`���ǂݍ���
//=======================================
void CTutorial::TextureLoad(void)
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
//�I������
//=======================================
void CTutorial::Uninit(void)
{
	//�t�F�[�h�ȊO�̃I�u�W�F�N�g�S�j��
	CObject::ExceptReleaseAll(CObject::TYPE_FADE);
}

//=======================================
//�X�V����
//=======================================
void CTutorial::Update(void)
{
	//��������e�N�X�`���\��ON/OFF�؂�ւ�
	SwitchDispPoly2D();

	//�v���C���[�̑���؂�ւ��̓e�N�X�`���̕\�����@�ɂ���ď�ɍ��E�����
	m_pPlayer->IsTutorialControll(!m_Controll.bDispChange);

	//�������e�N�X�`���̃T�C�Y�X�V
	UpdateObjIdentity();

	//�e�N�X�`����؂�ւ���
	SwapControllTexture();

	//�`���[�g���A���I��
	End();

#ifdef _DEBUG
	if (CManager::GetKeyboard()->GetTrigger(DIK_TAB)) SetStage();
#endif
}

//=======================================
//�`�揈��
//=======================================
void CTutorial::Draw(void)
{

}

//=======================================
//�������̍X�V����
//=======================================
void CTutorial::UpdateObjIdentity(void)
{
	//�v���C���[�̈ʒu���i�[
	D3DXVECTOR3 *pPlayerPos = NULL;

	const int PLAYER_PRIORITY = CPlayer::PRIORITY;
	const int NUM_ALL_OBJ = CObject::GetNumAll(PLAYER_PRIORITY);

	for (int nCntPlayer = 0; nCntPlayer < NUM_ALL_OBJ; nCntPlayer++)
	{
		//�v���C���[�擾
		CPlayer *pPlayer = (CPlayer *)CObject::GetObjectA(PLAYER_PRIORITY, nCntPlayer++);

		//�擾�����̂��v���C���[�ł͂Ȃ�����
		if (pPlayer == NULL || pPlayer->GetType() != CObject::TYPE_PLAYER) continue;

		//�v���C���[�̈ʒu���i�[
		pPlayerPos = &pPlayer->GetPosition();
		break;
	}

	//�v���C���[�̈ʒu���擾�ł��Ȃ�����
	if (pPlayerPos == NULL) return;

	//�e�������e�N�X�`����傫�������邩�ǂ�������
	for (int nCntObj = 0; nCntObj < NUM_OBJ_IDENTITY_TEXTURE; nCntObj++)
	{
		//�ϐ��������̂ŏk�߂�
		ObjIdentity *pObj = &m_ObjIdentity[nCntObj];

		//�ʒu�̍������i�[
		D3DXVECTOR3 PosDiff = pObj->pos - *pPlayerPos;
		PosDiff.y = 0.0f;

		//�߂������ɂ���
		if (D3DXVec3Length(&PosDiff) <= OBJ_IDENTITY_LENGTH)
		{
			//�T�C�Y���߂������p�ɕω�������
			pObj->fWidth +=	(OBJ_IDENTITY_TEX_NEAR_WIDTH - pObj->fWidth) * 0.1f;
			pObj->fHeight +=(OBJ_IDENTITY_TEX_NEAR_HEIGHT - pObj->fHeight) * 0.1f;
			pObj->pos.y += (OBJ_IDENTITY_NEAR_SET_POS_Y - pObj->pos.y) * 0.1f;
			pObj->fTexV += (1.0f - pObj->fTexV) * 0.1f;
			pObj->pPolyBill->ZTestSwitch(true);
		}
		else
		{
			//�T�C�Y�����������p�ɕω�������
			pObj->fWidth +=	(OBJ_IDENTITY_TEX_FAR_WIDTH - pObj->fWidth) * 0.1f;
			pObj->fHeight +=(OBJ_IDENTITY_TEX_FAR_HEIGHT - pObj->fHeight) * 0.1f;
			pObj->pos.y += (OBJ_IDENTITY_FAR_SET_POS_Y - pObj->pos.y) * 0.1f;
			pObj->fTexV += (OBJ_IDENTITY_FAR_TEX_Y - pObj->fTexV) * 0.1f;
			pObj->pPolyBill->ZTestSwitch(false);
		}

		//�T�C�Y��ς��Ă���
		pObj->pPolyBill->SetInfo(pObj->pos, pObj->fWidth, pObj->fHeight, 0.0f, VEC2_INIT, 1.0f, pObj->fTexV);
	}
}

//=======================================
//��������|���S���̕\���؂�ւ�
//=======================================
void CTutorial::SwitchDispPoly2D(void)
{
	//�\�����@�؂�ւ�
	if (CManager::IsInputTrigger(DIK_Z, CInputGamePad::X)) m_Controll.bDispChange ^= TRUE;

	//�\���e�N�X�`���؂�ւ�
	if (CManager::IsInputTrigger(DIK_Q, CInputGamePad::L_TRIGGER))
	{
		m_Controll.nTexID = (m_Controll.nTexID + 1) % NUM_CONTROLL_TEXTURE;
		m_Controll.pPoly2D->SetTexture(s_pControllTexturePath[m_Controll.nTexID]);
	}
}

//=======================================
//�`���[�g���A���I��
//=======================================
void CTutorial::End(void)
{
	//��ʑJ��
	if (CManager::IsInputTrigger(DIK_RETURN, CInputGamePad::A))
	{
		CFade::Set(CScene::GAME, CFade::FADE_WIPE_OUT);
	}
}

//=======================================
//��������e�N�X�`���̐؂�ւ�
//=======================================
void CTutorial::SwapControllTexture(void)
{
	//���݂̕\�����@�ɂ������T�C�Y���i�[
	D3DXVECTOR3 TargetPos = VEC3_INIT;
	float TargetWidth = 0.0f;
	float TargetHeight = 0.0f;

	//�����\���̏ꍇ
	if (m_Controll.bDispChange)
	{
		TargetPos = CONTROLL_TEX_CENTER_POS;
		TargetWidth = CONTROLL_TEX_CENTER_WIDTH;
		TargetHeight = CONTROLL_TEX_CENTER_HEIGHT;
	}
	//���ハ�C�v�\���̏ꍇ
	else
	{
		TargetPos = CONTROLL_TEX_WIPE_POS;
		TargetWidth = CONTROLL_TEX_WIPE_WIDTH;
		TargetHeight = CONTROLL_TEX_WIPE_HEIGHT;
	}

	//���X�ɕ\���T�C�Y�ɕω������Ă���
	m_Controll.pos.x += (TargetPos.x - m_Controll.pos.x) * 0.1f;
	m_Controll.pos.y += (TargetPos.y - m_Controll.pos.y) * 0.1f;
	m_Controll.fWidth += (TargetWidth - m_Controll.fWidth) * 0.1f;
	m_Controll.fHeight += (TargetHeight - m_Controll.fHeight) * 0.1f;

	//�|���S���T�C�Y���w�肷��
	m_Controll.pPoly2D->SetInfo(m_Controll.pos, m_Controll.fWidth, m_Controll.fHeight);
	m_Controll.pPoly2D->DispSwitch(false);
}