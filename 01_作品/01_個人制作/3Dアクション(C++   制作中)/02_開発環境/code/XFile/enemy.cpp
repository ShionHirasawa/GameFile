//================================================================================================
//
//�G����[enemy.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Number/score.h"
#include "objectX.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "../S_H_Lib/S_H_Control.h"
#include "../Player/player.h"
#include "../3D/object3D.h"
#include "../3D/wall.h"
#include "../3D/billboard.h"
#include "../3D/explosion.h"
#include "../3D/PolygonBill.h"
#include "../3D/shadow.h"
#include "../3D/mesh.h"
#include "../Main/sound.h"
#include "enemy.h"

/*�G�̏����D�揇��*/ #define PRIORITY_ENEMY	(3)
/*�|���S���̕�	*/	 #define ENEMY_WIDTH	(60.0f)
/*�|���S���̍���*/	 #define ENEMY_HEIGHT	(60.0f)

/*�e�N�X�`���̃p�X*/ #define TEX_PATH	"data\\TEXTURE\\enemy.png"

/*�ړ����x*/		 #define MOVE_SPEED		(0.5f)
/*�ő�ړ����x*/	 #define MAX_SPEED		(5.0f)
/*�ړ�������*/		 #define MOVE_ATTENUATE	(0.2f)
/*	 �̗�	*/		 #define ENEMY_LIFE		(5)

//�ÓI�����o�ϐ�
int	CEnemy::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CEnemy::CEnemy(int nPriority) : CObjectX(nPriority)
{
	//�����o�ϐ��N���A
}

//=======================================
//�f�X�g���N�^
//=======================================
CEnemy::~CEnemy()
{

}

//=======================================
//��������
//=======================================
CEnemy *CEnemy::Create(char *pFilePath)
{
	//�G�C���X�^���X�̐���
	CEnemy *pEnemy = new CEnemy;

	if (pEnemy == NULL) return NULL;

	//����������
	pEnemy->Init(pFilePath);

	//�G��Ԃ�
	return pEnemy;
}

//=======================================
//����������
//=======================================
HRESULT CEnemy::Init(char *pFilePath)
{
	//���_���W�̏�����
	if(FAILED(CObjectX::Init(pFilePath, XMODELTYPE_NONE, 100)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_ENEMY);
	
	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CEnemy::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObjectX::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CEnemy::Update(void)
{
	//�ړ�����
	MoveControl();
}

//---------------------
//�ړ����쏈��
//---------------------
void CEnemy::MoveControl(void)
{
	//�ړ�������
	Move();
}

//=======================================
//�`�揈��
//=======================================
void CEnemy::Draw(void) { CEnemy::Draw(NULL); }

//=======================================
//�`�揈��
//=======================================
void CEnemy::Draw(D3DXCOLOR *pDiffuse)
{
	//������`��
	CObjectX::Draw(pDiffuse);
}

//=======================================
//�ʒu�ݒ�
//=======================================
void CEnemy::SetPos(const D3DXVECTOR3 pos)
{
	CObjectX::SetPosition(pos);
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CEnemy::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}

//======================================================================================================
//												�h�ƃJ����
//======================================================================================================

//�T�[�`���C�g�F
const D3DXCOLOR CEnemySecurityCamera::SEARCH_LIGHT_COLOR = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.2f);

//���G�����̒���
const float CEnemySecurityCamera::SEARCH_LENGTH = 200.0f;

//���G���a
const float CEnemySecurityCamera::SEARCH_RADIUS = 20.0f;

//�~�����b�V���̕�����
const int CEnemySecurityCamera::NUM_DEVIDE = 16;

//�~�����b�V���̑S�̒��_�o�b�t�@��
const int CEnemySecurityCamera::ALL_VERTEX = 1 + CEnemySecurityCamera::NUM_DEVIDE;

//�~�����b�V���̑S�̃C���f�b�N�X�o�b�t�@��
const int CEnemySecurityCamera::ALL_INDEX = 1 + CEnemySecurityCamera::NUM_DEVIDE;

//��]���x
const float CEnemySecurityCamera::ROTAION_SPEED = 0.006f;

//��]���~�̃C���^�[�o��
const int CEnemySecurityCamera::ROTAION_INTERVAL = 50;

//=======================================
//�R���X�g���N�^
//=======================================
CEnemySecurityCamera::CEnemySecurityCamera(int nPriority) : CEnemy(nPriority)
{
	//�����o�ϐ��N���A
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_bFind = false;
	m_fPatrolStartRot = 0.0f;	//�p�g���[������J�n�p�x
	m_fPatrolEndRot = 0.0f;		//�p�g���[������I���p�x
	m_fRotationSpeed = 0.0f;	//��]���x
	m_bRotation = false;		//���n���ۂ̉�]�����@�@true�F���v���@�@false�F�����v���
	m_nRotationInterval = 0;	//�p�g���[���̌��E�p�x�ɒB�������́A�ꎞ�I�ɉ�]���~����C���^�[�o��
}

//=======================================
//�f�X�g���N�^
//=======================================
CEnemySecurityCamera::~CEnemySecurityCamera()
{

}

//=======================================
//��������
//=======================================
CEnemySecurityCamera *CEnemySecurityCamera::Create(char *pFilePath)
{
	//�C���X�^���X����
	CEnemySecurityCamera *pCamera = new CEnemySecurityCamera(2);

	//�C���X�^���X�������s
	if (!pCamera) return NULL;

	//������
	pCamera->Init(pFilePath);

	//�����������̂�Ԃ�
	return pCamera;
}

//=======================================
//����������
//=======================================
HRESULT CEnemySecurityCamera::Init(char *pFilePath)
{
	CEnemy::Init(pFilePath);

	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ALL_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
		return E_FAIL;

	//���_���ݒ�
	if (FAILED(SetVtxInfo()))
		return E_FAIL;

	//�C���f�b�N�X�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * ALL_INDEX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL)))	return E_FAIL;

	//�C���f�b�N�X���ݒ�
	if (FAILED(SetIdxInfo()))
		return E_FAIL;

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CEnemySecurityCamera::Uninit(void)
{
	//���_�o�b�t�@�j��
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�j��
	if (m_pIdxBuff != NULL) {
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	CEnemy::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CEnemySecurityCamera::Update(void)
{
	//�v���C���[�𔭌����Ă���
	if (m_bFind)
	{
		//�v���C���[��ǂ������鏈��
		Chaice();
	}
	else
	{
		//�v���C���[��T��
		Patrol();
	}

	//�v���C���[���T�[�`���C�g���ɋ��邩����
	//SearchLight();

	//���_���ݒ�
	SetVtxInfo();
}

//=======================================
//�`�揈��
//=======================================
void CEnemySecurityCamera::Draw(void)
{
	//������`��
	CEnemy::Draw(NULL);

	//���b�V����`��
	DrawMesh();
}

//=======================================
//���b�V���`�揈��
//=======================================
void CEnemySecurityCamera::DrawMesh(void)
{
	//�f�o�C�X���擾
	CRenderer *pRender = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//���ʃJ�����O
	pRender->SetCulling(D3DCULL_CW);

	//���e�X�gON
	pRender->SetAlphaTest(true, 0);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, ALL_INDEX, 0, ALL_INDEX);

	//���e�X�gOFF
	pRender->SetAlphaTest(false, 255);

	//�ʏ�J�����O
	pRender->SetCulling();
}

//=======================================
//���_���ݒ�
//=======================================
HRESULT CEnemySecurityCamera::SetVtxInfo(void)
{
	//���_���̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))	return E_FAIL;

	//���_���W�̐ݒ�
	pVtx[0].pos = VEC3_INIT;
	pVtx[0].pos.y = -13.0f;

	// nor������
	pVtx[0].nor = NOR_INIT;

	// �F�w��
	pVtx[0].col = SEARCH_LIGHT_COLOR;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = VEC2_INIT;

	//��������p�x���i�[
	float Rot = D3DX_PI;

	//�T�[�`���C�g�̒��S�ʒu
	m_CirclePos = D3DXVECTOR3(
		sinf(D3DX_PI) * SEARCH_LENGTH,
		-GetPosition().y,
		cosf(D3DX_PI) * SEARCH_LENGTH);

	for (int nCntVtx = 1; nCntVtx < ALL_VERTEX; nCntVtx++)
	{
		D3DXVECTOR3 pos = VEC3_INIT;
		//���_���W
		pos.x = pVtx[nCntVtx].pos.x = m_CirclePos.x + sinf(Rot) * SEARCH_RADIUS;
		pos.z = pVtx[nCntVtx].pos.z = m_CirclePos.z + cosf(Rot) * SEARCH_RADIUS;
		pos.y = pVtx[nCntVtx].pos.y = m_CirclePos.y;

		// nor������
		pVtx[nCntVtx].nor = NOR_INIT;

		// �F�w��
		pVtx[nCntVtx].col = SEARCH_LIGHT_COLOR;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCntVtx].tex = VEC2_INIT;

		//���ɐݒ肷�钸�_���W�̊p�x���v�Z
		Rot -= FIX_ROT / (float)NUM_DEVIDE;
	}

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))	return E_FAIL;

	//�ݒ芮��
	return S_OK;
}

//=======================================
//�C���f�b�N�X���ݒ�
//=======================================
HRESULT CEnemySecurityCamera::SetIdxInfo(void)
{
	//���_���̃|�C���^
	WORD *pIdx = NULL;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pIdxBuff->Lock(0, 0, (void* *)&pIdx, 0)))	return E_FAIL;

	//�J�E���g��
	const int NUM_COUNT = ALL_INDEX - 1;

	for (int nCntIdx = 0; nCntIdx <= NUM_COUNT; nCntIdx++)
	{
		pIdx[nCntIdx] = nCntIdx % NUM_COUNT + nCntIdx / NUM_COUNT;
	}

	//�C���f�b�N�X�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pIdxBuff->Unlock()))	return E_FAIL;

	//�ݒ芮��
	return S_OK;
}

//============================================
//�h�ƃJ�����̐ݒ菈��
//�����P�@pos	�F�ʒu
//�����Q�@rot	�F��������
//�����R�@fPatRot:���G���Ɍ���͈�
//============================================
void CEnemySecurityCamera::SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fPatRot)
{
	//�R��x�N�g�����ݒ�
	SetTransrate(pos, VEC3_INIT, rot);

	//�p�g���[�����ݒ�
	m_fPatrolStartRot = rot.y;	//�p�g���[���̏����p�x
	m_fPatrolEndRot = fPatRot;	//�p�g���[���̌��E�p�x
	m_bRotation = false;		//��]����������
	m_nRotationInterval = 0;	//�C���^�[�o��������

	//�I���p�x���A�����p�x���猩�Ď��v���
	if (rot.y > fPatRot)	m_fRotationSpeed = ROTAION_SPEED;

	//�����v���̏ꍇ
	else	m_fRotationSpeed = -ROTAION_SPEED;
}

//=======================================
//�v���C���[��T������
//=======================================
void CEnemySecurityCamera::Patrol(void)
{
	//��]�ꎞ��~��
	if (m_nRotationInterval > 0)
	{
		//��~���Ԍ���
		m_nRotationInterval--;
		return;
	}

	//�������擾
	D3DXVECTOR3 rot = GetRotation();

	//�ڕW�̊p�x���i�[
	float TargetRot = 0.0f;

	//�I���p�x�։�]
	if (m_bRotation)
	{
		//�I���p�x��ݒ�
		TargetRot = m_fPatrolEndRot;
	}
	else
	{
		//�J�n�p�x��ݒ�
		TargetRot = m_fPatrolStartRot;
	}

	//��]������
	rot.y += m_fRotationSpeed;

	//�p�x���ݒ肵���p�x�ɂقړ���
	if (fabsf(rot.y - TargetRot) <= ROTAION_SPEED)
	{
		rot.y = TargetRot;			//�p�x��ݒ�
		m_fRotationSpeed *= -1.0f;	//��]���x�𔽓]
		m_bRotation = !m_bRotation;	//��]�����𔽓]
		m_nRotationInterval = ROTAION_INTERVAL;//�ꎞ��~
	}

	//�����C��
	FixRotate(&rot.y);

	//�����ݒ�
	SetRotation(rot);
}

//=======================================
//�v���C���[���T�[�`���C�g���ɋ��邩���肷�鏈��
//=======================================
void CEnemySecurityCamera::SearchLight(void)
{
	return;
	//�p�x���擾
	D3DXVECTOR3 Rot = GetRotation();

	//�T�[�`���C�g�̏ꏊ���v�Z
	const D3DXVECTOR3 LightPos = D3DXVECTOR3(
		sinf(Rot.y) * SEARCH_LENGTH,
		0.0f,
		cosf(Rot.y) * SEARCH_LENGTH);

	const int PLAYER_PRIORITY = CPlayer::PRIORITY;
	const int NUM_OBJECT = CObject::GetNumAll(PLAYER_PRIORITY);

	for (int nCntPlayer = 0; nCntPlayer < NUM_OBJECT; nCntPlayer++)
	{
		//�v���C���[���擾
		CPlayer *pPlayer = (CPlayer *)CObject::GetObject(PLAYER_PRIORITY, nCntPlayer);

		//�v���C���[���擾�ł��Ȃ����� OR �v���C���[�͂�������ł���
		if (pPlayer == NULL || pPlayer->GetType() != CObject::TYPE_PLAYER || pPlayer->GetState() == CPlayer::STATE_DEATH) continue;

		//�v���C���[�̈ʒu���擾
		const D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
		const D3DXVECTOR3 PosDiff = PlayerPos - LightPos;

		//*************************
		//�v���C���[���͈͓��ɂ��邩�ǂ����@AND�@�v���C���[�Ƃ̊Ԃɕǂ����邩�ǂ���
		//				��
		//�͈͓��ɋ����ꍇ �l�Ԃ��ǂ����@�܂��́@�l�ԏ�Ԃ���Ȃ��Ƃ������Ă��邩�ǂ���
		//*************************
		if (D3DXVec3Length(&PosDiff) <= SEARCH_RADIUS &&
			pPlayer->GetTransformID() == ERROR_ID || D3DXVec3Length(&pPlayer->GetMoving()) > 0.1f)
			//�v���C���[���l�ԏ��  OR  �����Ă���  OR  �������Ă���Ԃ��Ȃ�
		{
			//�v���C���[�܂ł̊p�x���擾
			const float fPosDiffRot = atan2f(LightPos - PlayerPos);

			//�v���C���[�̕��֌����킹��
			OrientToDir(&Rot.y, fPosDiffRot, m_fRotationSpeed);
			SetRotation(Rot);
			m_bFind = true;
			break;

			//�x������炷
			//PlaySiren(true);

			//�v���C���[�ǔ���
			//return true;
		}

		//�����Ă��Ȃ�
		m_bFind = false;
		//�x�������~�߂�
		//PlaySiren(false);
	}
}

//=======================================
//�v���C���[��ǂ������鏈��
//=======================================
void CEnemySecurityCamera::Chaice(void)
{

}

//======================================================================================================
//												���{�b�g
//======================================================================================================
const float CEnemyRobot::SEARCH_RANGE = 1.0f;	//���G�͈͂̊p�x
const float CEnemyRobot::SEARCH_LENGTH = 150.0f;//���G�͈͂̒���
const float CEnemyRobot::CHACE_SPEED = 3.0f;	//�ǐՃX�s�[�h
const float CEnemyRobot::ROTATE_SPEED = 0.2f;	//��]��
const int CEnemyRobot::MISS_INTERVAL = 90;		//�������Ă�����G���鎞��
const int CEnemyRobot::RESTART_INTERVAL = 3000;	//�ċN���܂łɂ����鎞��
const int CEnemyRobot::PATROL_INTERVAL = 60;	//���񒆂ɗ����~�܂鎞��
const int CEnemyRobot::ATTACK_INTERVAL = 100;	//�U���d��

const float CEnemyRobot::SHADOW_SIZE = 25.0f;		//�e�|���S���̃T�C�Y

bool CEnemyRobot::s_bSiren = false;

//=======================================
//�R���X�g���N�^
//=======================================
CEnemyRobot::CEnemyRobot(int nPriority) : CEnemy(nPriority)
{
	//�����o�ϐ��N���A
	m_nPatrolInterval = 0;	//���񒆂̃`�F�b�N�|�C���g�ɓ��B�������̗����~�܂�C���^�[�o��
	m_nMissInterval = 0;	//�������Ă�����G����C���^�[�o��
	m_nReStartInterval = 0;	//�ċN���C���^�[�o��
	m_nAttackInterval = 0;	//�U���d���C���^�[�o��
	m_nAttackPower = 1;		//�U����
	m_nLife = ENEMY_LIFE;	//�̗�

	m_nNextFootPrint = 5;	//���̑��Ղ�\�����鎞��
	m_nMoveCounter = 0;		//���Ղ�\�����邽�߂̈ړ��J�E���^�[

	m_nShadowID = ERROR_ID; //�e�ԍ�
}

//=======================================
//�f�X�g���N�^
//=======================================
CEnemyRobot::~CEnemyRobot()
{

}

//=======================================
//��������
//=======================================
CEnemyRobot *CEnemyRobot::Create(char *pFilePath)
{
	//�C���X�^���X����
	CEnemyRobot *pRobot = new CEnemyRobot;

	//�C���X�^���X�������s
	if (!pRobot) return NULL;

	//������
	pRobot->Init(pFilePath);

	//�����������̂�Ԃ�
	return pRobot;
}

//=======================================
//����������
//=======================================
HRESULT CEnemyRobot::Init(char *pFilePath)
{
	//������
	CEnemy::Init(pFilePath);

	//��ސݒ�
	SetType(TYPE_ROBOT);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CEnemyRobot::Uninit(void)
{
	//�`�F�b�N�|�C���g���ݒ肳��Ă���
	if (m_CheckPoint.pPoint != NULL)
	{
		delete m_CheckPoint.pPoint;
		m_CheckPoint.pPoint = NULL;
	}

	//�����̉e���폜
	CShadow::Delete(m_nShadowID);

	//�I������
	CEnemy::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CEnemyRobot::Update(void)
{
	//�̗͂��Ȃ��̂Ȃ��p�X�V������
	if (m_nLife <= 0)
	{
		UpdateRestart();
		return;
	}

	//�O��ʒu�̍X�V
	UpdatePosOld();

	//�֐��ň����R��VECTOR3�^��錾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 move = VEC3_INIT;

	//�v���C���[��T��					->	//�������Ȃ�������p�g���[��
	if(!SearchPlayer(&pos, &move, &rot))	Patrol(&pos, &move, &rot);

	//�C���^�[�o�����o��
	CountInterval();

	//�����蔻��
	CollisionWhole(&pos, &move, &rot);

	//�R��VECTOR3�^�̑��
	SetTransrate(pos, move, rot);

	//���Ր���
	FootPrint(move, pos, rot.y);

	//�e����
	CShadow::SetPos(m_nShadowID, pos);
}

//=======================================
//�ċN�����̐�p�X�V����
//=======================================
void CEnemyRobot::UpdateRestart(void)
{
	//�C���^�[�o������
	m_nReStartInterval--;

	//���Ԋu�ŉ���\��
	if (m_nReStartInterval % 20 == 0)
	{
		//�����ʒu��G�̈ʒu��菭�����炷�p
		D3DXVECTOR3 Range = VEC3_INIT;

		//�p�x���擾
		const float fRot = RandomRotate();

		//���f��ID�i�[
		const int nID = GetMyXModelID();

		//�������a�擾
		const float fMargin = GetXModelMargin(nID);

		//�����ʒu�擾
		Range.x = sinf(fRot) * fMargin;
		Range.z = cosf(fRot) * fMargin;

		int nHeight = (int)(GetPosition(nID, TYPE_Y, CObjectX::TYPE_MAX) * 100.0f);
		Range.y = (float)(rand() % nHeight) * FIX_FLOAT;

		CExplosion *pExplosion = CExplosion::Create(GetPosition() + Range, D3DXVECTOR3(0.0f, (float)(rand() % 100) * FIX_FLOAT + 0.02f, 0.0f), false);
		pExplosion->SetVertexInfo(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));		
	}

	//�C���^�[�o���I��
	if (m_nReStartInterval <= 0)
	{
		//�S�C���^�[�o���N���A
		m_nAttackInterval = 0;
		m_nMissInterval = 0;
		m_nPatrolInterval = 0;
		m_nReStartInterval = 0;

		//�̗͂�߂�
		m_nLife = ENEMY_LIFE;

		//�U���͂��Q�{��
		m_nAttackPower *= 2;
	}
}

//=======================================
//�`�揈��
//=======================================
void CEnemyRobot::Draw(void)
{
	//���f���̐F�ݒ�
	D3DXCOLOR *pDiffuse = m_nLife <= 0 ? &D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f): NULL;

	//�`��
	CEnemy::Draw(pDiffuse);
}

//=======================================
//�e��C���^�[�o�������炷����
//=======================================
void CEnemyRobot::CountInterval(void)
{
	//�`�F�b�N�|�C���g�ɓ��B���ė����~�܂��Ă���C���^�[�o��
	if (m_nPatrolInterval > 0)	m_nPatrolInterval--;

	//�v���C���[�ɍU�������C���^�[�o��
	if (m_nAttackInterval > 0)	m_nAttackInterval--;

	//���������C���^�[�o��
	if (m_nMissInterval > 0)
	{
		m_nMissInterval--;

		//�`�F�b�N�|�C���g�����̏ꏊ�ɂ���
		m_CheckPoint.nIdx = (m_CheckPoint.nIdx + 1) % m_CheckPoint.nNumAll;
	}
}

//=======================================
//���񏈗�
//=======================================
void CEnemyRobot::Patrol(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot)
{
	//�`�F�b�N�|�C���g�������܂��͉�������C���^�[�o��������Ȃ�����Ԃ�
	if (m_CheckPoint.pPoint == NULL || IsAnyInterval()) return;

	//���݂Ǝ��̃`�F�b�N�|�C���g�̔ԍ���ʖ��ő��
	int NOW_INDEX = m_CheckPoint.nIdx;
	int NEXT_INDEX = (NOW_INDEX + 1) % m_CheckPoint.nNumAll;

	//���̃`�F�b�N�|�C���g�܂ł̊p�x���擾
	const float fDirRot = atan2f(m_CheckPoint.pPoint[NEXT_INDEX] - *pPos);

	//�ړ��ʂ�ݒ�
	pMove->x = sinf(fDirRot) * m_CheckPoint.fMove;
	pMove->z = cosf(fDirRot) * m_CheckPoint.fMove;

	//�`�F�b�N�|�C���g��ʉ߂���������
	if (IsPassCheckPoint(m_CheckPoint.pPoint[NEXT_INDEX], *pPos + *pMove))
	{
		*pMove = VEC3_INIT;						//�ړ��ʂ�����
		*pPos = m_CheckPoint.pPoint[NEXT_INDEX];//�`�F�b�N�|�C���g�Ɉړ�
		m_CheckPoint.nIdx = NEXT_INDEX;			//���̃`�F�b�N�|�C���g�ԍ���
		m_nPatrolInterval = PATROL_INTERVAL;	//�����~�܂鎞��
	}

	//�v���C���[�̕��֌����킹��
	OrientToDir(&pRot->y, atan2f(*pPos - m_CheckPoint.pPoint[NEXT_INDEX]), ROTATE_SPEED);
}

//=======================================
//�e��C���^�[�o�����c���Ă��邩����
//=======================================
bool CEnemyRobot::IsAnyInterval(void)
{
	//�`�F�b�N�|�C���g�ɓ��B���ė����~�܂��Ă���
	if (m_nPatrolInterval > 0 || m_nMissInterval > 0 || m_nAttackInterval > 0) return true;

	//�����Ԃ�
	return false;
}

//=======================================
//�`�F�b�N�|�C���g��ʉ߂������ǂ�������
//=======================================
bool CEnemyRobot::IsPassCheckPoint(D3DXVECTOR3 TargetPos, D3DXVECTOR3 Pos)
{
	//�ړI�n�܂ł̋������v�Z
	D3DXVECTOR3 posDiff = TargetPos - Pos;

	//�ړI�n�܂ł̋������߂��Ȃ�ʉ߂����Ƃ���
	if (D3DXVec3Length(&posDiff) <= 2.0f) return true;

	//�ړI�n�܂ł̋���������
	return false;
}

//=======================================
//�v���C���[��T������
//=======================================
bool CEnemyRobot::SearchPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot)
{
	//�|�C���^���ǂꂩ��NULL�Ȃ�����Ԃ�
	if (pPos == NULL || pMove == NULL || pRot == NULL) return false;

	const int PLAYER_PRIORITY = CPlayer::PRIORITY;
	const int NUM_OBJECT = CObject::GetNumAll(PLAYER_PRIORITY);

	for (int nCntPlayer = 0; nCntPlayer < NUM_OBJECT; nCntPlayer++)
	{
		//�v���C���[���擾
		CPlayer *pPlayer = (CPlayer *)CObject::GetObject(PLAYER_PRIORITY, nCntPlayer);

		//�v���C���[���擾�ł��Ȃ����� OR �v���C���[�͂�������ł���
		if (pPlayer == NULL || pPlayer->GetType() != CObject::TYPE_PLAYER || pPlayer->GetState() == CPlayer::STATE_DEATH) continue;

		//�v���C���[�̈ʒu���擾
		const D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		//*************************
		//�v���C���[���͈͓��ɂ��邩�ǂ����@AND�@�v���C���[�Ƃ̊Ԃɕǂ����邩�ǂ���
		//				��
		//�͈͓��ɋ����ꍇ �l�Ԃ��ǂ����@�܂��́@�l�ԏ�Ԃ���Ȃ��Ƃ������Ă��邩�ǂ���
		//*************************
		if (Search(*pPos, PlayerPos, pRot->y + SEARCH_RANGE, pRot->y - SEARCH_RANGE, SEARCH_LENGTH) &&
			!CWall::IsAmongWall(*pPos, PlayerPos))
		{
			//�v���C���[���l�ԏ��  OR  �����Ă���  OR  �������Ă���Ԃ��Ȃ�
			if (pPlayer->GetTransformID() == ERROR_ID || D3DXVec3Length(&pPlayer->GetMoving()) > 0.1f || m_nMissInterval > 0)
			{
				//�������Ă�����G���鎞��
				m_nMissInterval = MISS_INTERVAL;

				//�v���C���[�܂ł̊p�x���擾
				const float fRot = atan2f(*pPos - PlayerPos);

				//�ړ��ʂ�ݒ�
				pMove->x = -sinf(fRot) * CHACE_SPEED;
				pMove->z = -cosf(fRot) * CHACE_SPEED;

				//�v���C���[�̕��֌����킹��
				OrientToDir(&pRot->y, fRot, ROTATE_SPEED);

				//�v���C���[�Ƃ̓����蔻��
				if (CollisionPlayer(PlayerPos, pPlayer->COLLISION_MARGIN, pPos, CObjectX::GetXModelMargin(GetMyXModelID())) &&
					m_nAttackInterval == 0)
				{//��������
					m_nAttackInterval = ATTACK_INTERVAL;//�U���d��
					pPlayer->GiveDamege(m_nAttackPower);//�_���[�W��^����
				}

				//�x������炷
				PlaySiren(true);

				//�v���C���[�ǔ���
				return true;
			}
		}

		//�x�������~�߂�
		PlaySiren(false);
	}

	//�v���C���[���������Ȃ�����
	return false;
}

//=======================================
//�v���C���[�Ƃ̓����蔻�菈��
//=======================================
bool CEnemyRobot::CollisionPlayer(const D3DXVECTOR3 PlayerPos, const float fPlayerMargin, D3DXVECTOR3 *pPos, const float fMargin)
{
	//������͈͂𑫂�������
	const float fColliMargin = fPlayerMargin + fMargin;

	//�v���C���[�Ƃ̋����𑪒�
	D3DXVECTOR3 PosDiff = *pPos - PlayerPos;
	const float fLength = D3DXVec3Length(&PosDiff);

	//������͈͂ɂ߂荞��ł��Ȃ�
	if(fLength > fColliMargin)	return false;

	//*******************************************
	//�v���C���[�ɂ߂荞�܂Ȃ��悤�ɓG�̈ʒu����
	//*******************************************
	//�v���C���[����G�܂ł̊p�x
	const float fAngle = atan2f(PosDiff);

	//�ʒu�C��
	pPos->x = PlayerPos.x + sinf(fAngle) * fColliMargin;
	pPos->z = PlayerPos.z + cosf(fAngle) * fColliMargin;

	//��������
	return true;
}

//=======================================
//�_���[�W����
//=======================================
void CEnemyRobot::GiveDamege(int nDamage)
{
	//�_���[�W��^����
	m_nLife -= nDamage;

	//�̗͖����Ȃ�����ċN������
	if (m_nLife <= 0)
	{
		//�ċN���ɂ����鎞�Ԃ�ݒ�
		m_nReStartInterval = RESTART_INTERVAL;
	}
}

//=======================================
//�ݒ���
//=======================================
void CEnemyRobot::SetInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CheckPoint CheckPoint, int nAttackPower)
{
	//�e����
	m_nShadowID = CShadow::Create(pos, SHADOW_SIZE);

	//�U���͂̐ݒ�
	m_nAttackPower = nAttackPower;

	//�e�ݒ�
	CShadow::SetPos(m_nShadowID, pos);

	//�R��VECTOR3�^�̐ݒ�
	SetTransrate(pos, VEC3_INIT, rot);

	//�`�F�b�N�|�C���g�̏���n��
	m_CheckPoint.fMove = CheckPoint.fMove;
	m_CheckPoint.nIdx = CheckPoint.nIdx;
	m_CheckPoint.nNumAll = CheckPoint.nNumAll;

	//�������̃T�C�Y���v�Z
	const int size = sizeof D3DXVECTOR3 * m_CheckPoint.nNumAll;

	//�������𓮓I�m��
	m_CheckPoint.pPoint = new D3DXVECTOR3[size];
	memcpy(m_CheckPoint.pPoint, CheckPoint.pPoint, size * m_CheckPoint.nNumAll);
}

//=======================================
//�����蔻��܂Ƃ�
//=======================================
void CEnemyRobot::CollisionWhole(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot)
{
	const int nXModelID = GetMyXModelID();			//�����̇]���f���ԍ����擾
	const D3DXVECTOR3 PosOld = GetPosOld();			//�O��ʒu�擾
	const float fMargin = GetXModelMargin(nXModelID);//�����蔻��̃T�C�Y���擾
	const float fHeight = GetPosition(nXModelID, TYPE_Y, (CObjectX::TYPE)TYPE_MAX);

	for (int nCntType = 0; nCntType <= CObjectX::TYPE_Z; nCntType++)
	{	
		//���ԂɈړ�������
		switch (nCntType)
		{
			case CObjectX::TYPE_X:	pPos->x += pMove->x; break;
			case CObjectX::TYPE_Y:	pPos->y += pMove->y; break;
			case CObjectX::TYPE_Z:	pPos->z += pMove->z; break;
		}

		//�]���f���ɐG��Ă邩
		CObjectX::CollisionXModel(pPos, PosOld, pMove, (CObjectX::TYPE)nCntType, fMargin, nXModelID);
	}

	//�ǂɐG��Ă��邩
	CWall::Collision(pPos, PosOld, pMove, fMargin, fHeight);
}

const int	CEnemyRobot::FOOTPRINT_SPAWN_RANGE = 10;	//���Ղ𔭐�����Ԋu
const int	CEnemyRobot::FOOTPRINT_SPAWN_MIN = 3;		//���Ք����̍Œ�C���^�[�o��
const int	CEnemyRobot::FOOTPRINT_RANGE = 200;			//���Ղ��юU�点��͈�
const float	CEnemyRobot::FOOTPRINT_SIZE = 2.0f;			//���ՃT�C�Y
const float	CEnemyRobot::FOOTPRINT_GRAVITY = -2.0f;		//���Ղ𗎉�������d�͗�
const float	CEnemyRobot::FOOTPRINT_GRAVITY_CURRECT = 0.1f;//�d�͌W��
const float	CEnemyRobot::FOOTPRINT_JUMP = 2.0f;			//���������ۂ̕��V��
const float	CEnemyRobot::FOOTPRINT_SPEED = 1.0f;		//��юU�鑬�x
const int	CEnemyRobot::FOOTPRINT_LIFE = 20;			//����

//=======================================
//�������O�Ղ𐶐�
//=======================================
void CEnemyRobot::FootPrint(D3DXVECTOR3 move, D3DXVECTOR3 pos, float rotY)
{
	if (move == VEC3_INIT)
	{
		//�ړ��J�E���^�[�N���A
		m_nMoveCounter = 0;
		return;
	}

	//�ړ����ԑ���
	m_nMoveCounter = (m_nMoveCounter + 1) % m_nNextFootPrint;

	if (m_nMoveCounter == 0)
	{
		//���ɑ��Ղ���������^�C�~���O�����߂�
		m_nNextFootPrint = rand() % FOOTPRINT_SPAWN_RANGE + FOOTPRINT_SPAWN_MIN;

		//���Ր���
		CPolyBill::CreateFootPrint(pos, rotY, FOOTPRINT_RANGE,
								   FOOTPRINT_SIZE, GetColor(COL_BLACK), FOOTPRINT_GRAVITY, FOOTPRINT_GRAVITY_CURRECT,
								   FOOTPRINT_JUMP, FOOTPRINT_SPEED, FOOTPRINT_LIFE);
	}
}

//=======================================
//�x������炷����
//=======================================
void CEnemyRobot::PlaySiren(bool bPlay)
{
	//�ݒ肷���ԂƁA���݂̏�Ԃ������Ȃ珈�������Ȃ�
	if (s_bSiren == bPlay) return;

	//��ԍX�V
	s_bSiren = bPlay;

	//�T�E���h�Đ��؂�ւ�
	CManager::GetSound()->Coroutine(CSound::SE_SIREN_ROBOT, bPlay);
}