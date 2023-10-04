//================================================================================================
//
//�A�C�e������[item.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../XFile/objectX.h"
#include "../3D/billboard.h"
#include "../Player/player.h"
#include "item.h"

//==============================
//�}�N��
//==============================
//�A�C�e���̏����D�揇��
#define PRIORITY_ITEM	(3)

//�A�C�e���|���S���̃T�C�Y
#define ITEM_SIZE	(30.0f)

//�e�N�X�`���̃p�X
#define TEX_PATH	"data\\TEXTURE\\Item.png"

//�ÓI�����o�ϐ�
int	CItem::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CItem::CItem() : CObjectBillboard(PRIORITY_ITEM)
{
	//�����o�ϐ��N���A
}

//=======================================
//�f�X�g���N�^
//=======================================
CItem::~CItem()
{

}

//=======================================
//��������
//=======================================
CItem *CItem::Create(const D3DXVECTOR3 pos)
{
	//�A�C�e���C���X�^���X�̐���
	CItem *pItem = new CItem;

	//����������
	pItem->Init();

	//�e�N�X�`�����蓖��
	pItem->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	pItem->Set(pos);

	//�A�C�e����Ԃ�
	return pItem;
}

//=======================================
//����������
//=======================================
HRESULT CItem::Init(void)
{
	//���_���W�̏�����
	if (FAILED(CObjectBillboard::Init(VEC3_INIT, ITEM_SIZE, ITEM_SIZE, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_ITEM);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CItem::Uninit(void)
{
	//���_�o�b�t�@�̔j���Ȃ�
	CObjectBillboard::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CItem::Update(void)
{
	//�ړ�����
	Move();

	//�v���C���[�ɓ�����Ȃ�����
	if (!CollisionPlayer(GetPosition()))
	{
	 //���_���ݒ�
		SetVertexInfo(ITEM_SIZE, ITEM_SIZE, 0.0f);
	}	
}

//=======================================
//�`�揈��
//=======================================
void CItem::Draw(void)
{
	//������`��
	CObjectBillboard::Draw();
}

//=======================================
//�ʒu�ݒ�
//=======================================
void CItem::Set(const D3DXVECTOR3 pos)
{
	//�ʒu�E�ړ��ʁE������ݒ�
	CObjectBillboard::SetPosition(pos);
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CItem::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}

//=======================================
//�v���C���[�Ƃ̓����蔻��
//=======================================
bool CItem::CollisionPlayer(const D3DXVECTOR3 pos)
{
	//�I�u�W�F�N�g�S�̂𔻒�
	for (int nCntObj = 0; nCntObj < MAX_OBJ; nCntObj++)
	{
		//�I�u�W�F�N�g�̃|�C���^�����炤
		CObject *pObj = GetObject(CPlayer::PRIORITY, nCntObj);

		//�I�u�W�F�N�g�̃|�C���^�����炦��
		if (pObj != NULL)
		{
			//�I�u�W�F�N�g�̃^�C�v���v���C���[������
			if (pObj->GetType() == CObject::TYPE_PLAYER)
			{
				/*�u���b�N�̍���ʒu���i�[*/ D3DXVECTOR3 PlayerLeftOver;	pObj->GetVertexPos(PlayerLeftOver, 0);
				/*�u���b�N�̉E���ʒu���i�[*/ D3DXVECTOR3 PlayerRightUnder;	pObj->GetVertexPos(PlayerRightUnder, 3);

				//�����蔻��
				if ((pos.x >= PlayerLeftOver.x  &&	// �v���C���[�̍��[����E�ɁA
					 pos.x <= PlayerRightUnder.x) &&// �v���C���[�̉E�[���獶�ɁA
					(pos.y >= PlayerLeftOver.y  &&	// �v���C���[�̏㕔���牺�ɁA
					 pos.y <= PlayerRightUnder.y))	// �v���C���[�̉��������ɁA�A�C�e��������
				{
					this->Release();//�Ȃ�����
					return true;	//��������
				}
			}
		}
	}

	//�N�ɂ�������Ȃ�����
	return false;
}