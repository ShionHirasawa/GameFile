//================================================================================================
//
//�I�u�W�F�N�g2D����[object2D.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../XFile/objectX.h"
#include "../3D/billboard.h"
#include "object2D.h"
#include "../Player/player.h"
#include "bg.h"

//������
#define ATTENUATE	(0.1f)

//=======================================
//�R���X�g���N�^
//=======================================
CObject2D::CObject2D(const int nPriority) : CObject(nPriority)
{
	//�����o�ϐ��N���A
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fDepth = 0.0f;
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_pos = VEC3_INIT;
	m_rot = VEC3_INIT;
	m_move = VEC3_INIT;
	m_posOld = VEC3_INIT;
	m_col = GetColor(COL_CLEAR);
}

//=======================================
//�f�X�g���N�^
//=======================================
CObject2D::~CObject2D()
{

}

//=======================================
//����������
// MEMO�F�����ŕK�v�Ȃ���
// �e�N�X�`���̃p�X�A�ʒu�A���A�����A���_�J���[�A�e�N�X�`���̍���̍��W�A�e�N�X�`���̉E���̍��W
//=======================================
HRESULT CObject2D::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_���̃|�C���^
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//----------	�����o�ϐ�������	----------
	/*�ʒu�ݒ�*/	m_pos = pos;
	/*�J���[�ݒ�*/	m_col = col;
	/*�T�C�Y�ݒ�*/	SetSize(D3DXVECTOR2(fWidth, fHeight));

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y - fHeight, pos.z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y - fHeight, pos.z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y + fHeight, pos.z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHeight, pos.z);

	//rhw�E���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		pVtx[nCntVtx].rhw = RHW;	// 1.0�Œ�
		pVtx[nCntVtx].col = col;	// �F�w��
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(TexMin.x, TexMin.y);
	pVtx[1].tex = D3DXVECTOR2(TexMax.x, TexMin.y);
	pVtx[2].tex = D3DXVECTOR2(TexMin.x, TexMax.y);
	pVtx[3].tex = D3DXVECTOR2(TexMax.x, TexMax.y);

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))
	{
		//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CObject2D::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �������g�̏I������
	this->Release();
}

//=======================================
//�X�V����
//=======================================
void CObject2D::Update(void)
{
	//=============================
	//�e��I�u�W�F�N�g�̍X�V����
	//=============================
	CObject::UpdateAll();
}

//=======================================
//�`�揈��
//=======================================
void CObject2D::Draw(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=======================================
//�ړ��ʂ������i�����W���̓f�t�H���g�l
//=======================================
void CObject2D::MoveAttenuate(void)
{
	m_move += (VEC3_INIT - m_move) * ATTENUATE;
}

//=======================================
//�e�N�X�`�������蓖�Ă�
//=======================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//�e�N�X�`�������蓖�Ă�
	m_pTexture = pTexture;
}

//=======================================
//�d�͂�p�����ړ���������
//=======================================
void CObject2D::MoveAttenuateGravity(const float fGravityPower, const float fGravityMag)
{
	//�w�̈ړ��ʂ�����
	m_move.x += (0.0f - m_move.x) * ATTENUATE;

	//�x�̈ړ��ʂ��d�͂ɏ]�킹��
	m_move.y += (fGravityPower - m_move.y) * fGravityMag;
}

//=======================================
//��ʓ��ɂƂǂ߂鏈���i���_�ʒu���
//=======================================
bool CObject2D::KeepOnWindow(void)
{
	//�����������ǂ���
	bool bCollision = false;

	//��ʂ̍��[���A���ɍs���Ă��܂���
	if (m_pos.x <= 0.0f)
	{
		m_pos.x = 0.0f;		//���[�ɖ߂�
		m_move.x = 0.0f;	//���̈ړ��ʂ�����
		bCollision = true;	//��������
	}

	//��ʂ̉E�[���A�E�ɍs���Ă��܂���
	if (m_pos.x >= SCREEN_WIDTH_F)
	{
		m_pos.x = SCREEN_WIDTH_F;	//�E�[�ɖ߂�
		m_move.x = 0.0f;			//���̈ړ��ʂ�����
		bCollision = true;			//��������
	}

	//��ʂ̓V����A��ɍs���Ă��܂���
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;		//�V��ɖ߂�
		m_move.y = 0.0f;	//���̈ړ��ʂ�����
		bCollision = true;	//��������
	}

	//��ʂ̒���A���ɍs���Ă��܂���
	if (m_pos.y >= SCREEN_HEIGHT_F)
	{
		m_pos.y = SCREEN_HEIGHT_F;	//��ɖ߂�
		m_move.y = 0.0f;			//���̈ړ��ʂ�����
		bCollision = true;			//��������
	}

	//�����������ǂ��������Ԃ�
	return bCollision;
}

//=======================================
//��ʓ��ɂƂǂ߂鏈���i���S�ʒu����̕��A�������l������
//=======================================
bool CObject2D::KeepOnWindow(const float fWidth, const float fHeight)
{
	//�����������ǂ���
	bool bCollision = false;

	//��ʂ̍��[���A���ɍs���Ă��܂���
	if (m_pos.x - fWidth <= 0.0f)
	{
		m_pos.x = fWidth;	//���[���牡���̕� �߂�
		m_move.x = 0.0f;	//���̈ړ��ʂ�����
		bCollision = true;	//��������
	}

	//��ʂ̉E�[���A�E�ɍs���Ă��܂���
	if (m_pos.x + fWidth >= SCREEN_WIDTH_F)
	{
		m_pos.x = SCREEN_WIDTH_F - fWidth;	//�E�[���牡���̕� �߂�
		m_move.x = 0.0f;					//���̈ړ��ʂ�����
		bCollision = true;					//��������
	}

	//��ʂ̓V����A��ɍs���Ă��܂���
	if (m_pos.y - fHeight <= 0.0f)
	{
		m_pos.y = fHeight;	//�����̕� �߂�
		m_move.y = 0.0f;	//���̈ړ��ʂ�����
		bCollision = true;	//��������
	}

	//��ʂ̒���A���ɍs���Ă��܂���
	if (m_pos.y + fHeight >= SCREEN_HEIGHT_F)
	{
		m_pos.y = SCREEN_HEIGHT_F - fHeight;//��ɖ߂�
		m_move.y = 0.0f;					//���̈ړ��ʂ�����
		bCollision = true;					//��������
	}

	//�����������ǂ��������Ԃ�
	return bCollision;
}

//=======================================
//���_�J���[�ݒ�
//=======================================
HRESULT CObject2D::SetVertexInfo(const D3DXCOLOR col)
{
	//�|�C���^�ݒ�
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_�J���[�̐ݒ�
	m_col = pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = col;

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�ݒ芮��
	return S_OK;
}

//=======================================
//���_�J���[�ݒ�
//=======================================
HRESULT CObject2D::SetVertexInfo(const D3DXCOLOR col, const int nVtxID)
{
	//�|�C���^�ݒ�
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))return E_FAIL;

	//���_�J���[�̐ݒ�
	pVtx[nVtxID].col = col;

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))return E_FAIL;

	//�ݒ芮��
	return S_OK;
}

//=======================================
//���_���W�ݒ�
//=======================================
HRESULT CObject2D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight)
{
	//�|�C���^�ݒ�
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_UP) * fSizeHeight;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_UP) * fSizeHeight;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_DOWN) * fSizeHeight;

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�ݒ芮��
	return S_OK;
}

//=======================================
//���_���W�E�J���[��ς���
//=======================================
HRESULT CObject2D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const D3DXCOLOR col)
{
	//�|�C���^�ݒ�
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_UP) * fSizeHeight;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_UP) * fSizeHeight;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_DOWN) * fSizeHeight;

	//���_�J���[�̐ݒ�
	m_col = pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = col;

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�ݒ芮��
	return S_OK;
}

//=======================================
//�e�N�X�`�����W��ݒ�
//=======================================
HRESULT CObject2D::SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//�|�C���^�ݒ�
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex.x = pVtx[2].tex.x = Texpos.x;				//�� = ���̂܂ܑ��
	pVtx[1].tex.x = pVtx[3].tex.x = Texpos.x + fTexWidth;	//�E = ���̍��W + �e�N�X�`���̕�����

	pVtx[0].tex.y = pVtx[1].tex.y = Texpos.y;				//�� = ���̂܂ܑ��
	pVtx[2].tex.y = pVtx[3].tex.y = Texpos.y + fTexHeight;	//�� = ��̍��W + �e�N�X�`���̕����̌���

	if (FAILED(m_pVtxBuff->Unlock()))//���_�o�b�t�@���J���i�A�����b�N�j����
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�ݒ芮��
	return S_OK;
}

//=======================================
//���_���W�E�e�N�X�`�����W��ݒ�
//=======================================
HRESULT CObject2D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//�|�C���^�ݒ�
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_UP) * fSizeHeight;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_UP) * fSizeHeight;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_DOWN) * fSizeHeight;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex.x = pVtx[2].tex.x = Texpos.x;				//�� = ���̂܂ܑ��
	pVtx[1].tex.x = pVtx[3].tex.x = Texpos.x + fTexWidth;	//�E = ���̍��W + �e�N�X�`���̕�����

	pVtx[0].tex.y = pVtx[1].tex.y = Texpos.y;				//�� = ���̂܂ܑ��
	pVtx[2].tex.y = pVtx[3].tex.y = Texpos.y + fTexHeight;	//�� = ��̍��W + �e�N�X�`���̕����̌���

															//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�ݒ芮��
	return S_OK;
}

//=======================================
//���_���W�E���_�J���[�E�e�N�X�`�����W��ݒ�
//=======================================
HRESULT CObject2D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//�|�C���^�ݒ�
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_UP) * fSizeHeight;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_UP) * fSizeHeight;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + PI_RIGHT_DOWN) * fSizeHeight;

	//���_�J���[�̐ݒ�
	pVtx[0].col = pVtx[1].col = pVtx[2].col = pVtx[3].col = col;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex.x = pVtx[2].tex.x = Texpos.x;				//�� = ���̂܂ܑ��
	pVtx[1].tex.x = pVtx[3].tex.x = Texpos.x + fTexWidth;	//�E = ���̍��W + �e�N�X�`���̕�����

	pVtx[0].tex.y = pVtx[1].tex.y = Texpos.y;				//�� = ���̂܂ܑ��
	pVtx[2].tex.y = pVtx[3].tex.y = Texpos.y + fTexHeight;	//�� = ��̍��W + �e�N�X�`���̕����̌���

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�ݒ芮��
	return S_OK;
}

//=======================================
//���_���W���擾
//=======================================
HRESULT CObject2D::GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID)
{
	//�|�C���^�ݒ�
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�w�肳�ꂽ���_���W����
	rPos = pVtx[nVtxID].pos;

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�ݒ芮��
	return S_OK;
}