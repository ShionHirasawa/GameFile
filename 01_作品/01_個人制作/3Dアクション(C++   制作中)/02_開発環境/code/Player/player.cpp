//================================================================================================
//
//�v���C���[����[player.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../3D/object3D.h"
#include "../3D/floor.h"
#include "../3D/billboard.h"
#include "../3D/camera.h"
#include "../3D/bullet.h"
#include "../3D/wall.h"
#include "../Main/file.h"
#include "../Player/player.h"
#include "../XFile/jewel.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "../S_H_Lib/S_H_Control.h"
#include "../2D/object2D.h"
#include "../2D/fade.h"
#include "../2D/Polygon2D.h"
#include "../Mode/result.h"
#include "../3D/PolygonBill.h"
#include "../3D/shadow.h"

//==============================
//�v���C���[�}�N��
//==============================
/* �|���S���̕� */	#define PLAYER_WIDTH	(35.0f)
/*�|���S���̍���*/	#define PLAYER_HEIGHT	(40.0f)

//���f���p�[�c��񂪏����ꂽ�p�X
#define MODEL_PARTS_INFO	"data\\motion.txt"

//�X�j�[�N���x
#define SNEAK_SPEED		(0.4f)
//�ő�X�j�[�N���x
#define MAX_SNEAK_SPEED	(1.0f)

//�������x
#define WALK_SPEED		(1.0f)
//�ő�������x
#define MAX_WALK_SPEED	(3.0f)

//�ړ�������
#define MOVE_ATTENUATE	(0.2f)
//�W�����v��
#define JUMP_POWER		(20.0f)
//��]��
#define ROTATE_SPEED	(0.08f)

//�ړ����[�V�������~�߂�ړ��x�N�g��
#define STOP_MOTION_VECTOR	(0.1f)

//�d�͌W��
#define GRAVITY_MAG		(0.08f)
//�d�͉����x
#define GRAVITY_POWER	(9.0f)

//�e�̔��ˊp�x�̉�]��
#define BULLET_ANGLE_ROTATE	(0.03f)
//�e�̔��ˊԊu
#define BULLET_INTERVAL		(5)
//�e�̔��˕␳�ʒu
#define BULLET_FIRE_POS		(D3DXVECTOR3(0.0f, 70.0f, 0.0f))

//�ϐg�ł��鋗��
#define TRANSFORM_LENGTH	(100.0f)
//�ϐg�\�Ȋp�x
#define TRANSFORM_ANGLE		(0.3f)

//�ÓI�����o�ϐ�
const float	CPlayer::WIDTH_MARGIN = 0.7f;
const float	CPlayer::HEIGHT_MARGIN = 0.7f;
const float	CPlayer::COLLISION_MARGIN = 20.0f;
const float	CPlayer::COLLISION_LENGTH = 100.0f;
const int	CPlayer::PRIORITY = 4;
const int	CPlayer::MAX_LIFE = 100;
const int	CPlayer::DAMAGE_INTERVAL = 40;
const int	CPlayer::DEATH_INTERVAL = 100;
const int	CPlayer::SWITCH_DISP = 2;
const float CPlayer::PUT_OBJX_LENGTH = 50.0f;
const float CPlayer::NIMBLE_SPEED = 1.5f;
const float CPlayer::TOUGH_SPEED = 00.5f;

//�̗̓|���S���̔w�i
const	D3DXVECTOR3 CPlayer::LIFE_BG_POS = D3DXVECTOR3(1140.0f, 650.0f, 0.0f);
const	float		CPlayer::LIFE_BG_WIDTH = 130.0f;
const	float		CPlayer::LIFE_BG_HEIGHT = 60.0f;
const	D3DXCOLOR	 CPlayer::LIFE_BG_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);

//�̗͂̃��S
char	*CPlayer::LOGO_TEXTURE_PATH = "data\\TEXTURE\\Life.png";
const	D3DXVECTOR3 CPlayer::LIFE_LOGO_POS = D3DXVECTOR3(1080.0f, 630.0f, 0.0f);
const	float		CPlayer::LIFE_LOGO_WIDTH = 50.0f;
const	float		CPlayer::LIFE_LOGO_HEIGHT = 20.0f;

//�̗̓|���S��
char	*CPlayer::LIFE_TEXTURE_PATH = "data\\TEXTURE\\number001.png";
const	D3DXVECTOR3 CPlayer::LIFE_POS = D3DXVECTOR3(1200.0f, 660.0f, 0.0f);
const	float CPlayer::LIFE_WIDTH = 20.0f;
const	float CPlayer::LIFE_HEIGHT = 30.0f;
const	float CPlayer::LIFE_MARGIN = 20.0f;
const	float CPlayer::LIFE_TEX_WIDTH = 0.1f;
const	float CPlayer::LIFE_TEX_HEIGHT = 1.0f;

//�T�C�Y�g�k�̑J�ڃX�s�[�h
const float CPlayer::SCALING_SPEED = 0.05f;

//�e�|���S���̃T�C�Y
const float CPlayer::SHADOW_SIZE = 15.0f;

//=======================================
//�R���X�g���N�^
//=======================================
CPlayer::CPlayer() : CObject(PRIORITY)
{
	//�����o�ϐ��N���A
	/*�e�ԍ�*/		m_nShadowID = ERROR_ID;
	/*�X�j�[�N*/	m_bSneak = false;
	/*�`��ON/OFF*/	m_bDisp = true;
	/*����ON/OFF*/	m_bControll = true;
	/*�ϐgON/OFF*/	m_bTransform = true;
	/*�g�kON/OFF*/	m_bScaling = true;
	/*�g�k�T�C�Y*/	m_fScal = 1.0f;
	/*���f������*/	m_nNumModel = 0;
	/*�ϐg�ԍ�*/	m_nTransformModelID = m_nNextModelID = ERROR_ID;	
	/*�ʒu������*/	m_pos = VEC3_INIT;
	/*����������*/	m_rot = VEC3_INIT;
	/*�̗̓N���A*/	m_nLifeMax = m_nLife = MAX_LIFE;
	/*��ԃN���A*/	m_State = STATE_NORMAL;
	/*�q�b�g�d���N���A*/m_nDamageInterval = 0;
	/*���S�C���^�[�o��*/m_nDeathInterval = 0;
	/*���ɂ��ړ��ʑ�����*/m_fSpecialMaxSpeed = 1.0f;
	/*���Ղ�\�����邽�߂̈ړ��J�E���^�[*/m_nMoveCounter = 0;
	/*���̑��Ղ�\�����鎞��*/m_nNextFootPrint = 5;


	//���[����]�I�u�W�F�N�g���N���A
	m_nNumTakeObjX = 0;
	for (int nCntObjX = 0; nCntObjX < NUM_TAKE_OBJECTX; nCntObjX++)
	{
		m_pObjX[nCntObjX] = NULL;
	}

	//���f���̃|�C���^��NULL��
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		m_apModel[nCntModel] = NULL;
	}

	//�̗͌n�|���S���̃|�C���^��NULL��
	m_pLifeBG = NULL;
	m_pLifeLogo = NULL;
	for(int nCntLife = 0; nCntLife < NUM_LIFE_DEJIT; nCntLife++)
	{
		m_pLife[nCntLife] = NULL;
	}

	//���[�V�����̃|�C���^��NULL��
	m_pMotion = NULL;
}

//=======================================
//�f�X�g���N�^
//=======================================
CPlayer::~CPlayer()
{

}

//=======================================
//��������
//=======================================
CPlayer *CPlayer::Create(void)
{
	//�v���C���[�C���X�^���X�̐���
	CPlayer *pPlayer = new CPlayer;

	//����������
	pPlayer->Init();

	//�v���C���[��Ԃ�
	return pPlayer;
}

//=======================================
//����������
//=======================================
HRESULT CPlayer::Init(void)
{
	//���[�V�����C���X�^���X����
	m_pMotion = new CMotion;

	//���f���p�[�c�̏��ǂݎ��
	LoadPartsInfo();

	//���[�V�������u�ҋ@�v��Ԃɐݒ�
	m_pMotion->Set(CMotion::MOTIONTYPE_NEUTRAL);
	m_pMotion->SetPos();

	//�̗̓|���S��2D����
	SetLifePoly2D();

	//�e�|���S������
	m_nShadowID = CShadow::Create(m_pos, SHADOW_SIZE);

	//��ސݒ�
	SetType(CObject::TYPE_PLAYER);
	
	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CPlayer::Uninit(void)
{
	//�ۗL���Ă���S���f���I������
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();	//�I��������
		delete m_apModel[nCntModel];	//�|�C���^�j��
		m_apModel[nCntModel] = NULL;	//NULL�|�C���^��
	}

	//���[�V�����̃|�C���^�j��
	if (m_pMotion != NULL)
	{
		delete m_pMotion;	//�������폜
		m_pMotion = NULL;	//�|�C���^��NULL��
	}

	//�v���C���[�͏�����̂ŁA�o���Ă���Ă����|�C���^��NULL�ɂ���
	CManager::GetCamera()->SetPlayer(NULL);
	CManager::GetCamera()->Init();

	//�����̉e���폜
	CShadow::Delete(m_nShadowID);

	//���_�o�b�t�@�̔j���Ȃ�
	this->Release();
}

//=======================================
//�X�V����
//=======================================
void CPlayer::Update(void)
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_F1))
	{
		m_bDisp = !m_bDisp;
	}

	//�O��̈ʒu�X�V
	m_posOld = m_pos;

	//�ړ�����
	ActionControl();

	//�����蔻��܂Ƃ�
	CollisionWhole();

	/* �ړ��ʂ����� */ m_move.x += (0.0f - m_move.x) * MOVE_ATTENUATE;
	/* �ړ��ʂ����� */ m_move.z += (0.0f - m_move.z) * MOVE_ATTENUATE;
	///*�d�͂ɏ]�킹��*/ m_move.y -= (GRAVITY_POWER + m_move.y) * GRAVITY_MAG;

	//�I�u�W�F�N�g�̌��ʂ̍X�V����
	TransformUpdateEntrance();

	//��ԕʂ̍X�V����
	StateUpdateEntrance();

	//���[�V�����̍X�V����
	m_pMotion->Update();

	//�e�𔭎˂���
	SetBullet();

	//�ϐg
	Transform();

	//�S�[������
	Goal();

	//�̗̓|���S��2D����
	SetLifePoly2D();

	//�e�̈ʒu�ݒ�
	Shadow();

	//�s�����Ă��Ȃ� & ���[�V�������I������
	if (m_pMotion->IsFinish())
	{
		//�ҋ@���[�V�����ֈڍs
		m_pMotion->SetBlend(CMotion::MOTIONTYPE_NEUTRAL);
	}
}

//=======================================
//�A�N�V�������쏈��
//=======================================
void CPlayer::ActionControl(void)
{
	//����ł��� or �`���[�g���A�����̓����� or �ϐg���Ȃ�A�N�V�����s�\
	if (m_State == STATE_DEATH || !m_bControll || !m_bTransform) return;

	/*�J�����̊p�x*/	float CameraRot = CManager::GetCamera()->GetInverseRotate();
	/*�v���C���[����������p�x*/ float PlayerRotate = m_rot.y;

	//�ړ��ɂ܂��ϐ��̏����l�͒ʏ�ړ��̏ꍇ
	/*�ړ����x*/		float fMoveSpeed = WALK_SPEED;
	/*�ő�ړ���*/		float fMaxMoveSpeed = MAX_WALK_SPEED * m_fSpecialMaxSpeed;
	/*�ړ����[�V����*/	int nMoveMotionType = CMotion::MOTIONTYPE_WALK;

	//�X�j�[�N��Ԑ؂�ւ�
	if (CManager::IsInputPress(DIK_LCONTROL, CInputGamePad::L_THUMB))	m_bSneak ^= 1;

	//�X�j�[�N��Ԃ̏ꍇ
	if (m_bSneak)
	{
		/*�ړ����x���X�j�[�N��p��*/	fMoveSpeed = SNEAK_SPEED;
		/*�ő�ړ����x���X�j�[�N�p*/	fMaxMoveSpeed = MAX_SNEAK_SPEED * m_fSpecialMaxSpeed;
		/*���[�V�������y�X�j�[�N�z*/	nMoveMotionType = CMotion::MOTIONTYPE_SNEAK;
	}

	{
		//�T�u�L�[�̏��
		XZPlane_SubInfo Sub[2];

		//���Ɉړ�����ꍇ�̃T�u���ݒ�
		Sub[0].Key1 = DIK_S;		Sub[0].Key2 = DIK_W;
		Sub[0].MoveRot1 = PI_LEFT_UP;	Sub[0].MoveRot2 = PI_LEFT_DOWN;

		//�E�Ɉړ�����ꍇ�̃T�u���ݒ�
		Sub[1].Key1 = DIK_S;		Sub[1].Key2 = DIK_W;
		Sub[1].MoveRot1 = PI_RIGHT_UP;	Sub[1].MoveRot2 = PI_RIGHT_DOWN;

		if		(Move_XZPlane(CameraRot, fMoveSpeed, &m_move, &PlayerRotate, m_pMotion, nMoveMotionType));
		else if	(Move_XZPlane(DIK_A, CInputGamePad::LEFT,	PI_LEFT,	CameraRot, fMoveSpeed, &m_move, &Sub[0], &PlayerRotate, m_pMotion, nMoveMotionType));
		else if (Move_XZPlane(DIK_D, CInputGamePad::RIGHT,	PI_RIGHT,	CameraRot, fMoveSpeed, &m_move, &Sub[1], &PlayerRotate, m_pMotion, nMoveMotionType));
		else if (Move_XZPlane(DIK_S, CInputGamePad::DOWN,	PI_UP,		CameraRot, fMoveSpeed, &m_move, NULL,	 &PlayerRotate, m_pMotion, nMoveMotionType));
		else if (Move_XZPlane(DIK_W, CInputGamePad::UP,		PI_DOWN,	CameraRot, fMoveSpeed, &m_move, NULL,	 &PlayerRotate, m_pMotion, nMoveMotionType));
		else if (CManager::IsInputTrigger(DIK_RSHIFT, CInputGamePad::R_TRIGGER))	m_rot.y += 0.01f;//���v���ɉ�]
		FixRotate(&PlayerRotate);			//�v���C���[�̊p�x�C��
	}

	//�v���C���[��i�s�����Ɍ�������
	OrientToDir(&m_rot.y, PlayerRotate, ROTATE_SPEED);

	//B�L�[�������ꂽ��e���\����i�l�ԏ�Ԃ̂Ƃ�����
	if (CManager::IsInputTrigger(DIK_B, CInputGamePad::B) && m_nTransformModelID == ERROR_ID)	m_pMotion->Set(CMotion::MOTIONTYPE_ACTION);

	//���̈ړ��ʂ�͈͓��Ɏ��߂�
	Range(&m_move.x, fMaxMoveSpeed);
	Range(&m_move.z, fMaxMoveSpeed);

	//�������O�Ղ𐶐�
	FootPrint();

	//�ړ��x�N�g�����قږ���
	if (D3DXVec3Length(&m_move) <= STOP_MOTION_VECTOR)
	{
		//�ړ��J�E���^�[�N���A
		m_nMoveCounter = 0;

		//���݂̈ړ����[�V��������A�ҋ@���[�V�����ֈڍs������
		if (m_pMotion->GetType() == nMoveMotionType)
		{
			//�ҋ@���[�V�����ֈڍs
			m_pMotion->SetBlend(CMotion::MOTIONTYPE_NEUTRAL);
		}
	}
}

//=======================================
//�`�揈��
//=======================================
void CPlayer::Draw(void)
{
	//�e�\���ؑ�
	CShadow::DispSwitch(m_nShadowID, m_bDisp);

	//�`�悵�Ȃ�
	if (!m_bDisp) return;

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	/*�v�Z�p�}�g���b�N�X*/	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�T�C�Y�ݒ�
	D3DXMatrixScaling(&m_mtxWorld, m_fScal, m_fScal, m_fScal);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�l�ԏ�Ԃ̂Ƃ�
	if (m_nTransformModelID == ERROR_ID)
	{
		//�ێ����Ă��郂�f���̐��`�悷��
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->Draw();
		}
	}
	//�l�ԈȊO�ɕϐg��
	else
	{
		//���f���擾
		CObjectX::XModel *pModel = CObjectX::GetXModel(m_nTransformModelID);

		//�]���f�����擾�ł��Ȃ�������
		if (pModel == NULL)
		{
			//�`�悹���Ɉ����Ԃ�
			return;
		}

		/* �����_���[�̃|�C���^ */ CRenderer *pRender = CManager::GetRenderer();
		/* �f�o�C�X�ւ̃|�C���^ */ LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
		/*���݂̃}�e���A���ۑ��p*/ D3DMATERIAL9 matDef;
		/* �}�e���A���̃|�C���^ */ D3DXMATERIAL *pMat;

		//���e�X�g��L���ɂ���
		pRender->SetAlphaTest(true, 0);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL *)pModel->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(pModel->pTextureID[nCntMat]));

			//���f��(�p�[�c)�̕`��
			pModel->pMesh->DrawSubset(nCntMat);
		}

		//���e�X�g�𖳌��ɂ���
		pRender->SetAlphaTest(false, 0);

		//�}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=======================================
//�ϐg�����]���f���̌��ʂ̍X�V����
//=======================================
void CPlayer::TransformUpdateEntrance(void)
{
	//�l�ԏ�ԂȂ�X�V����
	if (m_nTransformModelID == ERROR_ID || !m_bControll) return;

	//�����擾
	switch (CObjectX::GetXModelType(m_nTransformModelID))
	{
		case CObjectX::XMODELTYPE_NIMBLE:	TransformUpdate_Nimble(); break;//���u�r�q�v
		case CObjectX::XMODELTYPE_TOUGH:	TransformUpdate_Tough(); break;//���u���x�v
		case CObjectX::XMODELTYPE_TAKE:		TransformUpdate_Take();	 break;//���u���[�v
		case CObjectX::XMODELTYPE_BIND:		TransformUpdate_Bind();	 break;//���u�����v
		default:break;	//�u�����v�͍X�V��������
	}
}

//=======================================
//���u�r�q�v�̍X�V����
//=======================================
void CPlayer::TransformUpdate_Nimble(void)
{
	//�ő�ړ��ʂɃo�t��������
	m_fSpecialMaxSpeed = NIMBLE_SPEED;
}

//=======================================
//���u���x�v�̍X�V����
//=======================================
void CPlayer::TransformUpdate_Tough(void)
{
	//�ő�ړ��ʂɃf�o�t��������
	m_fSpecialMaxSpeed = TOUGH_SPEED;
}

//=======================================
//���u���[�v�̍X�V����
//=======================================
void CPlayer::TransformUpdate_Take(void)
{
	//�������Q��
	int& rTmpNum = m_nNumTakeObjX;

	//�]�I�u�W�F�N�g�����[
	if (rTmpNum < NUM_TAKE_OBJECTX && CManager::IsInputTrigger(DIK_G, CInputGamePad::L_SHOULDER))
	{
		//���[����
		m_pObjX[rTmpNum] = CObjectX::TakeXModel(m_pos, m_rot.y, TRANSFORM_ANGLE, COLLISION_LENGTH);

		//���[�ł���
		if (m_pObjX[rTmpNum] != NULL)
		{
			m_pObjX[rTmpNum]->DispSwitch(false);//����
			rTmpNum++;							//���[����������
		}
	}

	//�]�I�u�W�F�N�g�����o��
	else if (CManager::IsInputTrigger(DIK_H, CInputGamePad::R_SHOULDER) && rTmpNum > 0 && m_pObjX[rTmpNum - 1] != NULL)
	{
		//���[���Ă��鐔����
		rTmpNum--;

		//�z�u����ꏊ�܂ł̒���
		const int XModelID = m_pObjX[rTmpNum]->GetMyXModelID();
		const float fMargin = m_pObjX[rTmpNum]->GetXModelMargin(XModelID) + PUT_OBJX_LENGTH;
		
		//�]�I�u�W�F�N�g�����o���ʒu
		const D3DXVECTOR3 PutPos = D3DXVECTOR3(
			m_pos.x - sinf(m_rot.y) * fMargin,
			m_pos.y - 0.0f,
			m_pos.z - cosf(m_rot.y) * fMargin);
		
		m_pObjX[rTmpNum]->SetTransrate(PutPos, VEC3_INIT, m_rot);//�ʒu�E������ݒ�
		m_pObjX[rTmpNum]->DispSwitch(true);	//�o��
		m_pObjX[rTmpNum] = NULL;			//�|�C���g�폜
	}
}

//=======================================
//���u�����v�̍X�V����
//=======================================
void CPlayer::TransformUpdate_Bind(void)
{

}

//=======================================
//�X�e�[�g�̍X�V��������
//=======================================
void CPlayer::StateUpdateEntrance(void)
{
	//��Ԃ��ƂɍX�V�����𕪊�
	switch (m_State)
	{
		case CPlayer::STATE_DAMAGE:	StateUpdate_Damage();	break;//�_���[�W�X�e�[�g�̍X�V����
		case CPlayer::STATE_DEATH:	StateUpdate_Death();	break;//���S�X�e�[�g�̍X�V����
	}
}

//=======================================
//�_���[�W�X�e�[�g�̍X�V����
//=======================================
void CPlayer::StateUpdate_Damage(void)
{
	//�C���^�[�o������
	m_nDamageInterval--;

	//�C���^�[�o���I��
	if (m_nDamageInterval <= 0)
	{
		m_nDamageInterval = 0;	//�C���^�[�o���N���A
		m_State = STATE_NORMAL;	//�ʏ��Ԃ�
		m_bDisp = true;			//�`�悷��
	}
	//�`��ON/OFF�؂�ւ�
	else if (m_nDamageInterval % SWITCH_DISP == 0)
	{
		m_bDisp ^= true;
	}
}

//=======================================
//���S�X�e�[�g�̍X�V����
//=======================================
void CPlayer::StateUpdate_Death(void)
{
	//�l�Ԃɖ߂�
	m_nTransformModelID = ERROR_ID;

	//�C���^�[�o������
	m_nDeathInterval--;

	//�C���^�[�o���I��			�t�F�[�h����
	if (m_nDeathInterval <= 0)	CFade::Set(CScene::RESULT, CFade::FADE_WIPE_OUT);

	//�`�悵�Ȃ�
	else if (m_nDeathInterval <= 30)	m_bDisp = false;

	//�`��ON/OFF�؂�ւ�
	else if (m_nDeathInterval % SWITCH_DISP == 0) m_bDisp ^= true;
}

//=======================================
//�̗̓|���S��2D����
//=======================================
void CPlayer::SetLifePoly2D(void)
{
	//�̗͕`��p�̔w�i���g���Ă��Ȃ�
	if (m_pLifeBG == NULL)
	{
		m_pLifeBG = CPoly2D::Create(NULL);

		if (m_pLifeBG == NULL) return;

		//���ݒ�
		m_pLifeBG->SetInfo(LIFE_BG_POS, LIFE_BG_WIDTH, LIFE_BG_HEIGHT);
		m_pLifeBG->SetVertexInfo(LIFE_BG_COLOR);
	}

	//�̗͂̃��S���g���Ă��Ȃ�
	if (m_pLifeLogo == NULL)
	{
		m_pLifeLogo = CPoly2D::Create(LOGO_TEXTURE_PATH);

		if (m_pLifeLogo == NULL) return;

		//���ݒ�
		m_pLifeLogo->SetInfo(LIFE_LOGO_POS, LIFE_LOGO_WIDTH, LIFE_LOGO_HEIGHT);
	}

	//�̗͕ۑ�
	int nLifeTmp = m_nLife;

	//����
	int nNumDejit = Dejit(m_nLife);

	for (int nCntLife = 0; nCntLife < NUM_LIFE_DEJIT; nCntLife++)
	{
		//�̗͗p�|���S������������Ă��Ȃ�
		if (m_pLife[nCntLife] == NULL)
		{//����
			m_pLife[nCntLife] = CPoly2D::Create(LIFE_TEXTURE_PATH);
		}

		//�̗͗p�|���S������������Ă���
		if (m_pLife[nCntLife] != NULL)
		{
			//������葽����Δ�\���ɂ���
			if (nCntLife >= nNumDejit)
			{
				m_pLife[nCntLife]->DispSwitch(false);
				continue;
			}

			//�P�̈ʂ𒊏o����
			const int nMod = nLifeTmp % 10;
			nLifeTmp /= 10;

			//���W�ݒ�i�]���W���������Ƃɍ��ւ��炷
			D3DXVECTOR3 pos = LIFE_POS;
			pos.x -= ((float)nCntLife * LIFE_WIDTH + (float)nCntLife * LIFE_MARGIN);

			//�e�N�X�`�����W��ݒ�
			D3DXVECTOR2 TexPos = VEC2_INIT;
			TexPos.x = nMod * LIFE_TEX_WIDTH;

			//�|���S�����ݒ�
			m_pLife[nCntLife]->SetInfo(pos, LIFE_WIDTH, LIFE_HEIGHT, TexPos, LIFE_TEX_WIDTH, LIFE_TEX_HEIGHT);
			m_pLife[nCntLife]->DispSwitch(true);
			m_pLife[nCntLife]->SetVertexInfo(D3DXCOLOR(1.0f, (float)m_nLife / (float)MAX_LIFE, 0.0f, 1.0f));
		}

		m_pLife[nCntLife]->DispSwitch(false);
	}

	m_pLifeBG->DispSwitch(false);
	m_pLifeLogo->DispSwitch(false);
}

//=======================================
//�_���[�W����
//=======================================
void CPlayer::GiveDamege(int nDamage)
{
	//���̏R��������Ȃ�
	if (m_State == STATE_DEATH) return;

	//�_���[�W��^����
	m_nLife -= nDamage;

	//����
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		m_State = STATE_DEATH;
		m_nDeathInterval = DEATH_INTERVAL;
		CResult::SetState(CResult::STATE_OVER);
	}
	//�܂������Ă�
	else
	{
		m_State = STATE_DAMAGE;
		m_nDamageInterval = DAMAGE_INTERVAL;
	}
}

//=======================================
//�����蔻��܂Ƃ�
//=======================================
void CPlayer::CollisionWhole(void)
{
	//�`���[�g���A���̓����ԂȂ瓖���蔻�薳��
	if (CManager::GetMode() == CScene::TUTORIAL && !m_bControll) return;

	for (int nCntType = 0; nCntType <= CObjectX::TYPE_Z; nCntType++)
	{
		//���ԂɈړ�������
		switch (nCntType)
		{
			case CObjectX::TYPE_X:	m_pos.x += m_move.x; break;
			case CObjectX::TYPE_Y:	m_pos.y += m_move.y; break;
			case CObjectX::TYPE_Z:	m_pos.z += m_move.z; break;
		}

		if (m_nTransformModelID == -1)	HumanCollisionWhole(nCntType);		//�l�ԏ�Ԃ̂Ƃ�
		else							TransformCollisionWhole(nCntType);	//�ϐg���Ă��鎞
	}

	//��΂ɐG��Ă��邩
	CJewel::Collision(m_pos);
}

//=======================================
//�l�ԏ�Ԃ̂Ƃ��̓����蔻��
//=======================================
void CPlayer::HumanCollisionWhole(int nType)
{
	/*�]���f���ɐG��Ă邩*/ CObjectX::CollisionXModel(&m_pos, m_posOld, &m_move, (CObjectX::TYPE)nType, COLLISION_LENGTH, COLLISION_MARGIN);
	/*  �ǂɐG��Ă��邩  */ CWall::Collision(&m_pos, m_posOld, &m_move, COLLISION_MARGIN, 90.0f);
}

//=======================================
//�ϐg��Ԃ̂Ƃ��̓����蔻��
//=======================================
void CPlayer::TransformCollisionWhole(int nType)
{
	//�����蔻��͈̔͂��擾
	const float fMargin = CObjectX::GetXModelMargin(m_nTransformModelID);
	const float fHeight = CObjectX::GetPosition(m_nTransformModelID, CObjectX::TYPE_Y, (CObjectX::TYPE)CObjectX::TYPE_MAX);

	/*X���f���ɐG��Ă邩*/	CObjectX::CollisionXModel(&m_pos, m_posOld, &m_move, (CObjectX::TYPE)nType, fMargin, m_nTransformModelID);
	/*	�ǂɐG��Ă��邩 */	CWall::Collision(&m_pos, m_posOld, &m_move, fMargin, fHeight);
}

//=======================================
//�e�̔��ˈʒu
//=======================================
void CPlayer::SetBullet(void)
{
	//�U��������������i�l�ԏ�Ԃ̂Ƃ�����
	if (CManager::IsInputPress(DIK_B, CInputGamePad::B) && m_nTransformModelID == ERROR_ID && m_bControll)
	{
		m_rot.y = -CManager::GetCamera()->GetInverseRotate();

		if (m_pMotion->IsAction())
		{
			//�e�̔��ːݒ�
			CBullet *pBullet = CBullet::Create(m_pos + BULLET_FIRE_POS, m_rot);
		}
	}
}

//=======================================
//�ϐg����
//=======================================
void CPlayer::Transform(void)
{
	//�ϐg�ԍ����i�[
	const int nTempID = CObjectX::TransformXModel(m_pos, m_rot.y, TRANSFORM_ANGLE, COLLISION_LENGTH, m_nNextModelID);

	//�ϐg
	if (CManager::IsInputTrigger(DIK_T, CInputGamePad::Y) && m_bControll && m_bTransform)
	{
		//���ɕϐg����X���f���̔ԍ����i�[
		m_nNextModelID = nTempID;

		//���f���ԍ����ς���Ă��Ȃ�������ȍ~�̏����������I��
		if (m_nTransformModelID == m_nNextModelID) return;

		//�ϐg�s�ɂ��A�k�����Ă���
		m_bTransform = false;
		m_bScaling = false;
	}

	//�ϐg��
	if (!m_bTransform)
	{
		//�g�咆
		if (m_bScaling)
		{
			m_fScal += SCALING_SPEED;

			//�g��I��
			if (m_fScal >= 1.0f)
			{
				//�ʏ�T�C�Y�ɖ߂��A�ϐg�\��
				m_fScal = 1.0f;
				m_bScaling = false;
				m_bTransform = true;
			}
		}

		//�k����
		else
		{
			m_fScal -= SCALING_SPEED;

			//�k���I��
			if (m_fScal <= 0.0f)
			{
				//�ϐg�ɂ��ړ��ʂ̕ω����N���A
				m_fSpecialMaxSpeed = 1.0f;

				//�T�C�Y�𖳂ɋA���A�g�咆�ɑJ��
				m_fScal = 0.0f;
				m_bScaling = true;

				//�ϐg���郂�f���ԍ���ݒ肵�đ̗͕ω�
				m_nTransformModelID = m_nNextModelID;
				TransformSetLife();
			}
		}
	}
}

//=======================================
//�ϐg�������̗͕̑ω�
//=======================================
void CPlayer::TransformSetLife(void)
{
	//�ݒ肷��̗͏�����
	int nLife = MAX_LIFE;

	//�I�u�W�F�N�g�ɕϐg���Ȃ炻�̃I�u�W�F�N�g�̗̑͂��擾
	if(m_nTransformModelID != ERROR_ID) nLife = CObjectX::GetXModelLife(m_nTransformModelID);

	//���̗̑͂��ő�̗͂Ɠ��� or �ݒ肷��̗͂�荡�̗̑͂�����
	if (m_nLife == m_nLifeMax || m_nLife > nLife)
	{
		//�ϐg�������f���̗̑͂ɂ���
		m_nLife = nLife;
	}

	//�ő�̗͐ݒ�
	m_nLifeMax = nLife;
}

//=======================================
//�p�[�c���ǂݎ��
//=======================================
void CPlayer::LoadPartsInfo(void)
{
	//�e�ԍ��̔z����i�[
	int aParentNumber[MAX_PARTS];

	//�t�@�C���C���X�^���X����
	CFile *pFile = new CFile;

	//�p�[�c�E���[�V�����̏���ǂݍ���
	pFile->LoadMotion(MODEL_PARTS_INFO, &m_apModel[0], m_pMotion, &m_nNumModel, &aParentNumber[0]);

	//���f���p�[�c�̐e���f����ݒ肷��
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		//�e�ԍ����Q�Ƃ���
		int &ParentID = aParentNumber[nCntModel];

		//�e�����Ȃ��ꍇ
		if (ParentID == -1)
		{
			//NULL��������
			m_apModel[nCntModel]->SetParent(NULL);
		}
		else
		{
			//�e�̃��f����������
			m_apModel[nCntModel]->SetParent(m_apModel[ParentID]);
		}
	}

	//���[�V�����Ƀ��f���������L����
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);

	//�������J��
	delete pFile;
	pFile = NULL;
}

//=======================================
//�ʒu�ݒ�
//=======================================
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	//�����ʒu�ݒ�
	m_pos = m_posOld = pos;
}

//=======================================
//�S�[������
//=======================================
void CPlayer::Goal(void)
{
	//�S�[������
	CJewel::Goal(m_pos);
}

//=======================================
//�`���[�g���A������̑���ON/OFF
//=======================================
void CPlayer::IsTutorialControll(bool bControll)
{
	//�`���[�g���A�����Ȃ�A����ON/OFF��؂�ւ���
	if (CManager::GetMode() == CScene::TUTORIAL) m_bControll = bControll;

	//�`���[�g���A���ȊO�Ȃ�A����͏��ON
	else m_bControll = true;
}

const int	CPlayer::FOOTPRINT_SPAWN_RANGE = 10;	//���Ղ𔭐�����Ԋu
const int	CPlayer::FOOTPRINT_SPAWN_MIN = 3;		//���Ք����̍Œ�C���^�[�o��
const int	CPlayer::FOOTPRINT_RANGE = 200;			//���Ղ��юU�点��͈�
const float	CPlayer::FOOTPRINT_SIZE = 2.0f;			//���ՃT�C�Y
const float	CPlayer::FOOTPRINT_GRAVITY = -2.0f;		//���Ղ𗎉�������d�͗�
const float	CPlayer::FOOTPRINT_GRAVITY_CURRECT = 0.1f;//�d�͌W��
const float	CPlayer::FOOTPRINT_JUMP = 2.0f;			//���������ۂ̕��V��
const float	CPlayer::FOOTPRINT_SPEED = 1.0f;		//��юU�鑬�x
const int	CPlayer::FOOTPRINT_LIFE = 20;			//����

//=======================================
//�������O�Ղ𐶐�
//=======================================
void CPlayer::FootPrint(void)
{
	//�ړ����ԑ���
	m_nMoveCounter = (m_nMoveCounter + 1) % m_nNextFootPrint;

	if (m_nMoveCounter == 0)
	{
		//���ɑ��Ղ���������^�C�~���O�����߂�
		m_nNextFootPrint = rand() % FOOTPRINT_SPAWN_RANGE + FOOTPRINT_SPAWN_MIN;

		//���Ր���
		CPolyBill::CreateFootPrint(m_pos, m_rot.y, FOOTPRINT_RANGE,
								   FOOTPRINT_SIZE, GetColor(COL_BLACK), FOOTPRINT_GRAVITY, FOOTPRINT_GRAVITY_CURRECT,
								   FOOTPRINT_JUMP, FOOTPRINT_SPEED, FOOTPRINT_LIFE);
	}
}

//=======================================
//�e�ݒ�
//=======================================
void CPlayer::Shadow(void)
{
	//�ʒu�ݒ�
	CShadow::SetPos(m_nShadowID, m_pos);

	//�e�̃T�C�Y�ݒ�
	float fSize = SHADOW_SIZE;

	//�]���f���ɕϐg��
	if (m_nTransformModelID != ERROR_ID)
	{
		//�]���f���̓����蔻��͈͂��擾
		fSize = CObjectX::GetXModelMargin(m_nTransformModelID);
	}

	//�e�̃T�C�Y��ݒ�i�ϐg���̏ꍇ�A�g�k�T�C�Y���ݒ肷��
	CShadow::SetSize(m_nShadowID, fSize * m_fScal);
}