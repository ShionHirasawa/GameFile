//================================================================================================
//
//������[floor.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "object3D.h"
#include "floor.h"

//���̏����D�揇��
#define PRIORITY_FIELD	(2)
//�����蔻����s�����_�̐�
#define NUM_COLLI_VTX	(3)
//�e�N�X�`���̕�����
#define TEX_NUM_SEPARATE	(1.0f)

int	CFloor::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CFloor::CFloor() : CObject3D(PRIORITY_FIELD)
{
	//�����o�ϐ��N���A
	m_nTextureID = ERROR_ID;
	m_fAddHeight = m_fAddHeightOld = 0.0f;
	m_fLandHeight = m_fLandHeightOld = 0.0f;
}

//=======================================
//�f�X�g���N�^
//=======================================
CFloor::~CFloor()
{

}

//=======================================
//��������
//=======================================
CFloor *CFloor::Create(void)
{
	//�C���X�^���X����
	CFloor *pFloor = new CFloor;

	//�C���X�^���X���������ꂽ
	if (pFloor != NULL)
	{
		//�����������Ɏ��s
		if (FAILED(pFloor->Init()))
		{
			delete pFloor;//�����������
			pFloor = NULL;//NULL�|�C���^��
			return NULL;	//NULL��Ԃ�
		}
	}

	//�C���X�^���X��Ԃ�
	return pFloor;
}

//=======================================
//����������
//=======================================
HRESULT CFloor::Init(void)
{
	//����������
	if (FAILED(CObject3D::Init(VEC3_INIT, 0.0f, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(TEX_NUM_SEPARATE, TEX_NUM_SEPARATE))))
	{
		//���������s
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_FLOOR);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CFloor::Uninit(void)
{
	//�I������
	CObject3D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CFloor::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CFloor::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���ʃJ�����OON
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�`�揈��
	CObject3D::Draw();

	//�ʏ�J�����O�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CFloor::Load(char *pPath)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(pPath);

	CObject3D::BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}

//=======================================
//�ݒ菈��
//=======================================
void CFloor::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const D3DXVECTOR2 texSize)
{
	/* �ʒu�ݒ� */	SetTransrate(pos, VEC3_INIT, rot);
	/*�T�C�Y�ݒ�*/	SetSize(size);

	//���_���W�ݒ�
	SetVertexInfo(size.x, size.y, size.z, VEC2_INIT, texSize.x, texSize.y);
}

//=======================================
//�����蔻�菈��
//=======================================
bool CFloor::LandField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	//���̗D�揇�ʑт̃I�u�W�F�N�g�̑������擾
	const int NUM_OBJECT = CObject::GetNumAll(PRIORITY_FIELD);

	//��������ǂ����𔻒肷��
	bool bLand = false;

	for (int nCntObj = 0; nCntObj < NUM_OBJECT; nCntObj++)
	{
		//���Ɠ����D�揇�ʂ̃I�u�W�F�N�g���擾
		CObject *pFloor = GetObject(PRIORITY_FIELD, nCntObj);

		//�I�u�W�F�N�g�������擾�ł��A���̃I�u�W�F�N�g�̃^�C�v�����ł���
		if (pFloor != NULL && pFloor->GetType() == CObject::TYPE_FLOOR)
		{
			/* �ʒu���擾 */	D3DXVECTOR3 FieldPos = pFloor->GetPosition();
			/* �������擾 */	float fWidth = pFloor->GetWidth();
			/* ���s���擾 */	float fDepth = pFloor->GetDepth();

			//����^�ォ�猩���낵���Ƃ��A���̒��Ƀv���C���[������
			if (FieldPos.x - fWidth <= pPos->x && pPos->x <= FieldPos.x + fWidth &&
				FieldPos.z - fDepth <= pPos->z && pPos->z <= FieldPos.z + fDepth)
			{
				if (FieldPos.y <= pPosOld->y &&	//�O��͏��̏�ɋ���
					FieldPos.y >= pPos->y)		//���݂͏��ɂ߂荞��ł���
				{
					pPos->y = FieldPos.y;	//���̏�ɖ߂�
					pMove->y = 0.0f;		//�������x���O��
					bLand = true;			//�����
				}
			}
		}
	}

	//��������ǂ�����Ԃ�
	return bLand;
}