//================================================================================================
//
//�Ǐ���[wall.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "object3D.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "wall.h"

//�ǂ̏����D�揇��
#define PRIORITY_WALL	(2)
//�����蔻����s�����_�̐�
#define NUM_COLLI_VTX	(3)
//�e�N�X�`���̃p�X
#define TEX_PATH		"data\\TEXTURE\\wall_dark.png"
//�e�N�X�`���̕�����
#define TEX_NUM_SEPARATE	(1.0f)

//=======================================
//�R���X�g���N�^
//=======================================
CWall::CWall() : CObject3D(PRIORITY_WALL)
{
	//�����o�ϐ��N���A
	m_nTextureID = ERROR_ID;
}

//=======================================
//�f�X�g���N�^
//=======================================
CWall::~CWall()
{

}

//=======================================
//��������
//=======================================
CWall *CWall::Create(void)
{
	//�C���X�^���X����
	CWall *pWall = new CWall;

	//�C���X�^���X���������ꂽ
	if (pWall != NULL)
	{
		//�����������Ɏ��s
		if (FAILED(pWall->Init()))
		{
			delete pWall;	//�����������
			pWall = NULL;	//NULL�|�C���^��
			return NULL;	//NULL��Ԃ�
		}

		//�e�N�X�`���ǂݍ���
		pWall->Load();

		//�e�N�X�`�����蓖��
		pWall->BindTexture(CManager::GetTexture()->GetAddress(pWall->m_nTextureID));
	}

	//�C���X�^���X��Ԃ�
	return pWall;
}

//=======================================
//����������
//=======================================
HRESULT CWall::Init(void)
{
	//����������
	if (FAILED(CObject3D::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(TEX_NUM_SEPARATE, TEX_NUM_SEPARATE))))
	{
		//���������s
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_WALL);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CWall::Uninit(void)
{
	//�I������
	CObject3D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CWall::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CWall::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���ʃJ�����O���s��
	if (CManager::GetRenderer()->Culling())
	{
		//���ʃJ�����OON
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	//�`�揈��
	CObject3D::Draw();

	//�ʏ�J�����O�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CWall::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}

//=======================================
//�ݒ菈��
//=======================================
void CWall::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const D3DXVECTOR2 texSize)
{
	/* �ʒu�ݒ� */	SetTransrate(pos, VEC3_INIT, rot);
	/*�T�C�Y�ݒ�*/	SetSize(size);

	//���_���W�ݒ�
	SetVertexInfo(size.x, size.y, size.z, VEC2_INIT, texSize.x, texSize.y);
}

//=======================================
//�����蔻�菈��
//���R�����g�̒��́uPL�v�̓v���C���[�̎�
//=======================================
bool CWall::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, float fMargin, float fHeight)
{
	//�ړ��x�N�g�����O�Ȃ�A�����蔻����s���K�v���Ȃ��̂ň����Ԃ�
	if (fabsf(pMove->x) <= 0.00001f && fabsf(pMove->z) <= 0.00001f) return false;

	//�ǂɈ�ԋ߂��ʒu���i�[
	D3DXVECTOR3 NearestPos = VEC3_INIT;

	//�ǂ̗D�揇�ʑт̃I�u�W�F�N�g�̑������擾
	const int NUM_OBJECT = CObject::GetNumAll(PRIORITY_WALL);

	for (int nCntWall = 0; nCntWall < NUM_OBJECT; nCntWall++)
	{
		//�I�u�W�F�N�g���擾����
		CWall *pWall = (CWall *)CObject::GetObject(PRIORITY_WALL, nCntWall);

		//�ǃI�u�W�F�N�g���擾�ł��Ȃ�������A�������΂�
		if (pWall == NULL || pWall->GetType() != CObject::TYPE_WALL) continue;

		//�ǂ̍��W�E�T�C�Y�E��]�l�i�x���j���擾����
		const D3DXVECTOR3 pos = pWall->GetPosition();
		const D3DXVECTOR3 size = pWall->GetSize3D();
		const float rotY = pWall->GetRotation().y;

		//�ǂ̒�ӂ�蓪�̈ʒu���Ⴂ
		if (pos.y - size.y > pPos->y + fHeight) continue;

		//���E�̒��_�̃��[���h���W���v�Z
		const D3DXVECTOR3 LeftPos = GetWorldPosition(PI_LEFT + rotY, size, pos);
		const D3DXVECTOR3 RightPos = GetWorldPosition(PI_RIGHT + rotY, size, pos);

		//+++++++++++++++++++++++++++++
		//�ǂɈ�ԋ߂��ʒu���Z�o
		//+++++++++++++++++++++++++++++
		{
			//��ԋ߂��p�x���i�[
			const float NearRot = NearRotate(rotY, fMargin, *pPos, LeftPos, RightPos);

			//�ǂɈ�ԋ߂��ʒu���Z�o
			NearestPos.x = pPos->x - sinf(NearRot) * fMargin;
			NearestPos.z = pPos->z - cosf(NearRot) * fMargin;
		}

		//���E���x�N�g���i���E�̒��_�̃x�N�g��
		D3DXVECTOR3 VecLine = RightPos - LeftPos;

		//+++++++++++++++++++++++++++++
		//�ǂ̗����ɂ��邩
		//+++++++++++++++++++++++++++++
		{
			//���[�����ԋ߂����W�܂ł̃x�N�g��
			const D3DXVECTOR3 vecLeftPos = NearestPos - LeftPos;

			//���݈ʒu���A���E���x�N�g���̍����ɋ��Ȃ���Ώ������΂�
			if (VecLine.x * vecLeftPos.z - VecLine.z * vecLeftPos.x < 0.0f) continue;
		}

		//�O��E���݂̕ǂɈ�ԋ߂������̊Ԃ��ړ��x�N�g���Ƃ���
		D3DXVECTOR3 VecMove = NearestPos - *pPos;

		//+++++++++++++++++++++++++++++
		//��_���v�Z����
		//+++++++++++++++++++++++++++++
		D3DXVECTOR3 CrossPos = CrossPoint(NearestPos, VecMove, LeftPos, RightPos);

		//�O��ʒu����A��_�܂ł̒��������߂�
		const float PosOldLength = D3DXVec3Length(&(*pPos - CrossPos));

		//��_�܂ł̋������ړ��x�N�g�����傫���i�܂���_��ǂ��z���Ă��Ȃ�
		if (PosOldLength > D3DXVec3Length(&VecMove)) continue;

		//���E���x�N�g���̋���
		const float VecLineLength = D3DXVec3Length(&VecLine);

		//���_�����_�܂ł̒������A���E�̒��_�܂ł̒����̒��ɔ[�܂��ĂȂ���Έ����Ԃ�
		if (D3DXVec3Length(&(CrossPos - LeftPos)) > VecLineLength ||
			D3DXVec3Length(&(CrossPos - RightPos)) > VecLineLength) continue;

		//+++++++++++++++++++++++++++++
		//�ǂ��肷���_�����߂�
		//+++++++++++++++++++++++++++++
		{
			//���݈ʒu��ǂƒ��p�̕����ɖ߂������̍��W���i�[
			const D3DXVECTOR3 ReturnPos = D3DXVECTOR3(pPos->x - sinf(rotY) * VecLineLength, 0.0f,
													  pPos->z - cosf(rotY) * VecLineLength);

			//��_���W���Z�o����
			CrossPos = CrossPoint(ReturnPos, ReturnPos - *pPos, LeftPos, RightPos);
		}

		//+++++++++++++++++++++++++++++
		//��_�̈ʒu�ɖ߂��i�̌`���l������
		//+++++++++++++++++++++++++++++
		pPos->x += CrossPos.x - NearestPos.x;
		pPos->z += CrossPos.z - NearestPos.z;

		//��������
		return true;
	}

	return false;
}

//==========================================
//�Ώۂ܂ł̊Ԃɕǂ����邩�ǂ�������
//�����P�@�n�_�ʒu
//�����Q�@�I�_�ʒu
//�Ԃ�l�F�����ƖڕW�̊Ԃɕǂ����邩�ǂ����@true�F�ǂ�����@false�F�ǂ͂Ȃ�
//==========================================
bool CWall::IsAmongWall(D3DXVECTOR3 StartPos, D3DXVECTOR3 EndPos)
{
	return false;

	//�ǂƓ����D�揇�ʂ̃I�u�W�F�N�g�̑���
	const int NUM_OBJECT = CObject::GetNumAll(PRIORITY_WALL);

	//�������瓾��������i�[
	const D3DXVECTOR3 VecMove = EndPos - StartPos;			//�ړ��x�N�g��
	const float vecMoveLength = D3DXVec3Length(&VecMove);	//�ړ��x�N�g���̒���

	for (int nCntWall = 0; nCntWall < NUM_OBJECT; nCntWall++)
	{
		//�I�u�W�F�N�g�擾
		CWall *pWall = (CWall *)CObject::GetObjectA(PRIORITY_WALL, nCntWall++);

		//�ǂ���Ȃ��I�u�W�F�N�g���擾����
		if (pWall == NULL || pWall->GetType() != CObject::TYPE_WALL) continue;

		//�ǂ̍��W�E�T�C�Y�E��]�l�i�x���j���擾����
		const D3DXVECTOR3 WallPos = pWall->GetPosition();
		const D3DXVECTOR3 WallSize = pWall->GetSize3D();
		const float rotY = pWall->GetRotation().y;

		//���E�̒��_�̃��[���h���W���v�Z
		const D3DXVECTOR3 LeftPos = GetWorldPosition(PI_LEFT + rotY, WallSize, WallPos);
		const D3DXVECTOR3 RightPos = GetWorldPosition(PI_RIGHT + rotY, WallSize, WallPos);
		const D3DXVECTOR3 VecLine = RightPos - LeftPos;
		const float vecLineLength = D3DXVec3Length(&VecLine);

		//+++++++++++++++++++++++++++++
		//��_���v�Z����
		//+++++++++++++++++++++++++++++
		D3DXVECTOR3 CrossPos = CrossPoint(StartPos, VecMove, LeftPos, RightPos);

		{
			//�n�_�`��_�܂ł̃x�N�g��
			const D3DXVECTOR3 VecCross = StartPos - CrossPos;

			//��_�܂ł̋������ړ��x�N�g���������Ă�����_��
			if (vecMoveLength < D3DXVec3Length(&VecCross)) continue;
		}

		//���E�̒��_�����_�܂ł̃x�N�g�����擾
		D3DXVECTOR3 vecLeftCross = LeftPos - CrossPos;
		D3DXVECTOR3 vecRightCross = RightPos - CrossPos;

		//���E�̒��_�����_�܂ł̃x�N�g���̒������A�ǂ��炩�ł����_���m�̃x�N�g����蒷���ƃ_��
		if (D3DXVec3Length(&vecLeftCross)  <= vecLineLength &&
			D3DXVec3Length(&vecRightCross) <= vecLineLength)return true;
	}

	//�ǂ͂Ȃ�����
	return false;
}