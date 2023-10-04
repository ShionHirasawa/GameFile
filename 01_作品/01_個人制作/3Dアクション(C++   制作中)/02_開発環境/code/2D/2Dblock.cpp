//================================================================================================
//
//2D�u���b�N�I�u�W�F�N�g����[2Dblock.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../XFile/objectX.h"
#include "object2D.h"
#include "2Dblock.h"
#include "../Player/player.h"

/***  �e�N�X�`���̃p�X  ***/ 
#define TEX_PATH			"data\\TEXTURE\\block.jpg"
/***	�u���b�N�̏����D�揇��	***/
#define PRIORITY_2DBLOCK	(3)

//�ÓI�����o�ϐ��錾
/**/ int CBlock2D::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CBlock2D::CBlock2D() : CObject2D(PRIORITY_2DBLOCK)
{ 

}

//=======================================
//�f�X�g���N�^
//=======================================
CBlock2D::~CBlock2D()
{

}

//=======================================
//��������
//=======================================
CBlock2D *CBlock2D::Create(void)
{
	//�C���X�^���X����
	CBlock2D *pBlock2D = new CBlock2D;

	//�C���X�^���X���������ꂽ
	if (pBlock2D != NULL)
	{
		//�����������Ɏ��s
		if (FAILED(pBlock2D->Init()))
		{
			delete pBlock2D;//�����������
			pBlock2D = NULL;//NULL�|�C���^��
			return NULL;	//NULL��Ԃ�
		}

		//�e�N�X�`�����蓖��
		pBlock2D->BindTexture(CManager::GetTexture()->GetAddress(m_nTextureID));
	}

	//�C���X�^���X��Ԃ�
	return pBlock2D;
}

//=======================================
//����������
//=======================================
HRESULT CBlock2D::Init(void)
{
	//����������
	if (FAILED(CObject2D::Init(VEC3_INIT, 0.0f, 0.0f, GetColor(COL_WHITE), VEC2_INIT, D3DXVECTOR2(1.0f, 1.0f))))
	{
		//���������s
		return E_FAIL;
	}

	//��ސݒ�
	SetType(CObject::TYPE_BLOCK);

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CBlock2D::Uninit(void)
{
	//�I������
	CObject2D::Uninit();
}

//=======================================
//�X�V����
//=======================================
void CBlock2D::Update(void)
{
	//�v���C���[�Ƃ̓����蔻��
	CollisionPlayer(GetPosition(), GetSize2D());
}

//=======================================
//�`�揈��
//=======================================
void CBlock2D::Draw(void)
{ 
	//�`�揈��
	CObject2D::Draw();
}

//=======================================
//�e�N�X�`���ǂݍ��ݐݒ�
//=======================================
HRESULT CBlock2D::Load(void)
{
	//�e�N�X�`���ǂݍ���
	m_nTextureID = CManager::GetTexture()->Regist(TEX_PATH);

	//�e�N�X�`���ǂݍ��ݐ���
	return S_OK;
}

//=======================================
//�ݒ菈��
//=======================================
void CBlock2D::Set(const D3DXVECTOR3 pos, const float fWidth, const float fHeight)
{
	/* �ʒu�ݒ� */	SetPosition(pos);
	/*�T�C�Y�ݒ�*/	SetSize(D3DXVECTOR2(fWidth, fHeight));

	//���_���W�ݒ�
	SetVertexInfo(fWidth, fHeight);
}

//=======================================
//�v���C���[�Ƃ̓����蔻�菈��
//=======================================
//
//�����蔻��̃C���[�W
//�E��ʂ���ɓ������鄭
//�E�V���艺�ɑ������鄳����2�𖞂����Ȃ�A�����蔻������s����
//
//		��Player		���V��
//		 0  ���_  �^��������������
//	   �^l�_	><    ���u���b�N��
//	   �^ �_���^  �_��������������
//						�����
//
//=======================================
void CBlock2D::CollisionPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR2 size)
{
	/*�u���b�N�̍�����W���i�[*/D3DXVECTOR3 BlockLeftUp;
	/*�u���b�N�̉E�����W���i�[*/D3DXVECTOR3 BlockRightDown;
	
	/*�u���b�N�̍�����W���擾*/GetVertexInfo(BlockLeftUp, 0);
	/*�u���b�N�̉E�����W���擾*/GetVertexInfo(BlockRightDown, 3);

	//�I�u�W�F�N�g�S�̂𔻒�
	for (int nCntPlayer = 0; nCntPlayer < MAX_OBJ; nCntPlayer++)
	{
		//�I�u�W�F�N�g�̃|�C���^�����炤
		CObject *pPlayer = GetObject(CPlayer::PRIORITY, nCntPlayer);

		//�I�u�W�F�N�g�̃|�C���^�����炦��
		if (pPlayer != NULL)
		{
			//�I�u�W�F�N�g�̃^�C�v���v���C���[������
			if (pPlayer->GetType() == CObject::TYPE_PLAYER)
			{
				//�T�C�Y�E�ʒu���i�[
				/*�v���C���[�̃T�C�Y*/ D3DXVECTOR2 PlayerSize = pPlayer->GetSize2D();// ���̌W��  �̓|���S���̗]������������Ă���
				/*�v���C���[�̈ړ���*/ D3DXVECTOR3 PlayerMove = pPlayer->GetMoving();
				/*�v���C���[�̌��ݒn*/ D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
				/*�v���C���[�̑O��n*/ D3DXVECTOR3 PlayerPosOld = pPlayer->GetPosOld();

				/*�C������ʒu���i�[*/ D3DXVECTOR3 FixPos = PlayerPos;

				/*�|���S���̗]������*/PlayerSize = D3DXVECTOR2(PlayerSize.x * CPlayer::WIDTH_MARGIN, PlayerSize.y * CPlayer::HEIGHT_MARGIN);

				//===============================================================
				// STEP�P: �u���b�N�̓V��`��ʂ܂ł̊ԂɃv���C���[������
				//===============================================================
				if (BlockLeftUp.y <= PlayerPos.y + PlayerSize.y		&&	// �u���b�N�̓V�� <= �v���C���[�̑�
					PlayerPos.y - PlayerSize.y <= BlockRightDown.y	&&	// �v���C���[�̓� <= �u���b�N�̒��
					PlayerPosOld.y + PlayerSize.y != BlockLeftUp.y	&&	// �O��u���b�N�ɏ���Ă��Ȃ�
					PlayerPosOld.y - PlayerSize.y != BlockRightDown.y)	// �O��u���b�N�ɓ����������ĂȂ�
				{
					//===============================================================
					// STEP�Q : �u���b�N�̍��E�ǂ��炩�痈���̂��A�O��̈ʒu��p���Ĕ���
					//			����������A�u���b�N�̒[�ɕ����߂荞�܂Ȃ��ʒu���Z�o����
					//===============================================================
					//������߂荞�񂾂�
					if (BlockLeftUp.x >= PlayerPosOld.x + PlayerSize.x &&	//�O��̓u���b�N��荶�ɋ���
						BlockLeftUp.x <= PlayerPos.x + PlayerSize.x)		//���̓u���b�N�̍��[�ɂ߂荞��ł���
					{
						// �u���b�N�̍��[����A�v���C���[�̉����������������C���ʒu�Ƃ���
						FixPos.x = BlockLeftUp.x - PlayerSize.x;

						//�]�̈ړ��ʂ�0��
						PlayerMove.x = 0.0f;
					}

					//�E����߂荞�񂾂�
					if (BlockRightDown.x <= PlayerPosOld.x - PlayerSize.x &&//�O��̓u���b�N���E�ɋ���
						BlockRightDown.x >= PlayerPos.x - PlayerSize.x)		//���̓u���b�N�̉E�[�ɂ߂荞��ł���
					{
						// �u���b�N�̉E�[����A�v���C���[�̉����𑫂��������C���ʒu�Ƃ���
						FixPos.x = BlockRightDown.x + PlayerSize.x;

						//�]�̈ړ��ʂ�0��
						PlayerMove.x = 0.0f;
					}
				}

				//===============================================================
				// STEP�R : �u���b�N�̍��[����E�[�̒��Ƀv���C���[������
				//===============================================================
				if (BlockLeftUp.x <= PlayerPos.x + PlayerSize.x &&		// �u���b�N�̍��[ <= �v���C���[�̉E�[
					PlayerPos.x - PlayerSize.x <= BlockRightDown.x &&	// �v���C���[�̍��[ <= �u���b�N�̉E�[
					PlayerPosOld.x + PlayerSize.x != BlockLeftUp.x &&	// �O��u���b�N�̍��[�ɉ����Ă��Ȃ�
					PlayerPosOld.x - PlayerSize.x != BlockRightDown.x)	// �O��u���b�N�̉E�[�ɉ����Ă��Ȃ�
				{
					//===============================================================
					// STEP�S : �u���b�N�̏㉺�ǂ��炩�痈���̂��A�O��̈ʒu��p���Ĕ���
					//			����������A�u���b�N�̏㉺�ɍ������߂荞�܂Ȃ��ʒu���Z�o����
					//===============================================================
					//�ォ��߂荞�񂾂�
					if (BlockLeftUp.y >= PlayerPosOld.y + PlayerSize.y &&	//�O��̓u���b�N����ɋ���
						BlockLeftUp.y <= PlayerPos.y + PlayerSize.y)		//���̓u���b�N�̓V��ɂ߂荞��ł���
					{
						// �u���b�N�̓V�䂩��A�v���C���[�̍��������������ʒu���C���ʒu�Ƃ���
						FixPos.y = BlockLeftUp.y - PlayerSize.y;

						//�x�̈ړ��ʂ�0��
						PlayerMove.y = 0.0f;
					}

					//������߂荞�񂾂�
					if (BlockRightDown.y <= PlayerPosOld.y - PlayerSize.y &&//�O��̓u���b�N��艺�ɋ���
						BlockRightDown.y >= PlayerPos.y - PlayerSize.y)		//���̓u���b�N�̒�ʂɂ߂荞��ł���
					{
						// �u���b�N�̒�ʂ���A�v���C���[�̍����𑫂����ʒu���C���ʒu�Ƃ���
						FixPos.y = BlockRightDown.y + PlayerSize.y;

						//�v���C���[���㏸��
						if (PlayerMove.y < 0.0f)
						{
							//�x�̈ړ��ʂ�0��
							PlayerMove.y = 0.0f;
						}
					}
				}

				/* �ʒu���C������ */ pPlayer->SetPosition(FixPos);
				/*�ړ��ʂ��C������*/ pPlayer->SetMoving(PlayerMove);
			}
		}
	}
}