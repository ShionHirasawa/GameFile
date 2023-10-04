//================================================================================================
//
//3D�r���{�[�h�|���S������[PolygonBill.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "PolygonBill.h"
#include "../S_H_Lib/S_H_Calculation.h"

//�D�揇��
const int CPolyBill::PRIORITY = 2;

//=======================================
//�R���X�g���N�^
//=======================================
CPolyBill::CPolyBill(const int nPriority) : CObjectBillboard(nPriority)
{
	//�����o�ϐ��N���A
	m_bDisp = true;

	//�����ɂ�郁���o�ϐ��N���A
	m_Property_bDead = false;
	m_Property_fGravity = 0.0f;
	m_Property_fGravityCorr = 0.0f;
	m_Property_move = VEC3_INIT;
	m_Property_nLife = 0;
}

//=======================================
//�f�X�g���N�^
//=======================================
CPolyBill::~CPolyBill()
{

}

//=======================================
//��������
//=======================================
CPolyBill *CPolyBill::Create(char *pPath)
{
	//�|���S���C���X�^���X����
	CPolyBill *pPoly3D = new CPolyBill;

	//�C���X�^���X�𐶐��ł���
	if (pPoly3D != NULL)
	{
		//����������
		pPoly3D->Init();

		//�e�N�X�`���ǂݍ���
		pPoly3D->m_nTextureID = CManager::GetTexture()->Regist(pPath);

		//�e�N�X�`�����蓖��
		pPoly3D->BindTexture(CManager::GetTexture()->GetAddress(pPoly3D->m_nTextureID));
	}

	//�C���X�^���X��Ԃ�
	return pPoly3D;
}

//=======================================
//����������
//=======================================
HRESULT CPolyBill::Init(void)
{
	//���_���W�̏�����
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_POLY_BILL);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CPolyBill::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObjectBillboard::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CPolyBill::Update(void)
{
	//�������Ƃ̍X�V����
	UpdatePropertyEntrance();

	//���S�t���O�������Ă�����j��
	if (m_Property_bDead)
	{
		CObjectBillboard::Uninit();
	}
}

//=======================================
//�`�揈��
//=======================================
void CPolyBill::Draw(void)
{
	if (!m_bDisp) return;

	//�`��
	CObjectBillboard::Draw();
}

//=======================================
//�������Ƃ̍X�V�����̑���
//=======================================
void CPolyBill::UpdatePropertyEntrance(void)
{
	for (int nCntProperty = 0; nCntProperty < PROPERTY_MAX; nCntProperty++)
	{
		//�Ώۂ̑������t���Ă��Ȃ��Ȃ珈�����΂�
		if (!m_bProperty[nCntProperty]) continue;

		switch (nCntProperty)
		{
			case PROPERTY_GRAVITY:	UpdateProperty_GRAVITY(); break;
			case PROPERTY_MOVE:		UpdateProperty_MOVE();	break;
			case PROPERTY_LIFE:		UpdateProperty_LIFE(); break;
		}

		//���ʂȂ�X�V���������Ȃ�
		if (m_Property_bDead) break;
	}
}

//=======================================
//�u�d�́v�����̍X�V����
//=======================================
void CPolyBill::UpdateProperty_GRAVITY(void)
{
	//�d�͏���
	m_Property_move.y += (m_Property_fGravity - m_Property_move.y) * m_Property_fGravityCorr;

	//�ړ��ʐݒ�
	SetMoving(m_Property_move);
}

//=======================================
//�u�ړ��v�����̍X�V����
//=======================================
void CPolyBill::UpdateProperty_MOVE(void)
{
	//�ړ�����
	Move();
}

//=======================================
//�u�̗́v�����̍X�V����
//=======================================
void CPolyBill::UpdateProperty_LIFE(void)
{
	//�̗͌���
	m_Property_nLife--;

	//�̗͂��s�����玀�S�t���O�𗧂Ă�
	if (m_Property_nLife <= 0)
	{
		m_Property_bDead = true;
	}
}

//=======================================
//�|���S�����̐ݒ菈��
//=======================================
void CPolyBill::SetInfo(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, D3DXVECTOR2 fTexPos, float TexWidth, float TexHeight)
{
	//�ʒu�ݒ�
	SetPosition(pos);

	//���_���W�E�e�N�X�`�����W�̐ݒ�
	SetVertexInfo(fWidth, fHeight, fDepth, fTexPos, TexWidth, TexHeight);
}

//=======================================
//�d�͑�����ݒ肷��
//�P�ڂ̈����ɂ͗������x���A�Q�ڂ̈����ɂ͌W����
//=======================================
void CPolyBill::SetProperty(float fGravity, float fCorrect)
{
	//�d�͑����L��
	m_bProperty[PROPERTY_GRAVITY] = true;
	m_Property_fGravity = fGravity;
	m_Property_fGravityCorr = fCorrect;
}

//=======================================
//�ړ�������ݒ肷��@�����̒l���ړ��ʂƂȂ�
//=======================================
void CPolyBill::SetProperty(D3DXVECTOR3 move)
{
	//�ړ������L��
	m_bProperty[PROPERTY_MOVE] = true;
	m_Property_move = move;
	SetMoving(move);
}

//=======================================
//�̗͑�����ݒ肷��
//=======================================
void CPolyBill::SetProperty(int nLife)
{
	m_bProperty[PROPERTY_LIFE] = true;
	m_Property_nLife = nLife;
}

//=========================================
//���Ղ𐶐�����
//�������鑫�Ղɂ́u�d�́E�ړ��E�̗́v������t�^����
//-----------------------------------------
//�����P�@pos	�F�����ʒu
//�����Q�@fRot	�F�p�x
//�����R�@nRange�F��юU��͈́iRange��100�̏ꍇ�A��юU��͈͂� +50 �` -50 �̊� * 0.01  �ɂȂ�
//�����S�@fSize	�F�|���S���T�C�Y
//------  �����ɕK�v�ȗv�f  ---------------
//�����T�@fGravity		�F�d��
//�����U�@fGravityCorr	�F�d�͌W��
//�����V�@fJump			�F�����オ���
//�����W�@fSpeed		�F�ړ��X�s�[�h
//�����X�@nLife			�F�̗�
//=========================================
void CPolyBill::CreateFootPrint(D3DXVECTOR3 pos, float fRot, int nRange, float fSize, D3DXCOLOR col, float fGravity, float fGravityCorr, float fJump, float fSpeed, int nLife)
{
	//��юU�����
	const float fRotRange = (rand() % nRange - (nRange * HALF)) * FIX_FLOAT;

	//���Ղ̔������������߂�
	float fSpreadRot = fRot + fRotRange;
	fSpreadRot = FixRotate(&fSpreadRot);

	//��юU��ړ���
	D3DXVECTOR3 move = VEC3_INIT;
	move.x = sinf(fSpreadRot) * fSpeed;
	move.y = fJump;
	move.z = cosf(fSpreadRot) * fSpeed;

	CPolyBill *pBill = CPolyBill::Create(NULL);
	pBill->SetInfo(pos, fSize, fSize);
	pBill->SetVertexInfo(col);
	pBill->SetProperty(fGravity, fGravityCorr);//�d�͐ݒ�
	pBill->SetProperty(nLife);	//�̗͐ݒ�
	pBill->SetProperty(move);	//�ړ��ʐݒ�
}