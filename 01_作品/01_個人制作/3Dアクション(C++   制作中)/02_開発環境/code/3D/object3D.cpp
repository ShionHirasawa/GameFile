//================================================================================================
//
//�I�u�W�F�N�g3D����[object3D.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "object3D.h"

//������
#define ATTENUATE	(0.1f)

//=======================================
//�R���X�g���N�^
//=======================================
CObject3D::CObject3D(const int nPriority) : CObject(nPriority)
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
CObject3D::~CObject3D()
{

}

//=======================================
//����������
// MEMO�F�����ŕK�v�Ȃ���
// �e�N�X�`���̃p�X�A�ʒu�A���A�����A���_�J���[�A�e�N�X�`���̍���̍��W�A�e�N�X�`���̉E���̍��W
//=======================================
HRESULT CObject3D::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_���̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
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

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fWidth, -fHeight, +fDepth);
	pVtx[1].pos = D3DXVECTOR3(+fWidth, -fHeight, +fDepth);
	pVtx[2].pos = D3DXVECTOR3(-fWidth, +fHeight, -fDepth);
	pVtx[3].pos = D3DXVECTOR3(+fWidth, +fHeight, -fDepth);

	//rhw�E���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		pVtx[nCntVtx].nor = NOR_INIT;// nor������
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
void CObject3D::Uninit(void)
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
void CObject3D::Update(void)
{
	//=============================
	//�e��I�u�W�F�N�g�̍X�V����
	//=============================
	CObject::UpdateAll();
}

//=======================================
//�`�揈��
//=======================================
void CObject3D::Draw(void)
{
	/*  �f�o�C�X���擾  */	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	/*�v�Z�p�}�g���b�N�X*/	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=======================================
//�ړ��ʂ������i�����W���̓f�t�H���g�l
//=======================================
void CObject3D::MoveAttenuate(void)
{
	m_move += (VEC3_INIT - m_move) * ATTENUATE;
}

//=======================================
//�d�͂�p�����ړ���������
//=======================================
void CObject3D::MoveAttenuateGravity(const float fGravityPower, const float fGravityMag)
{
	/*�w�x�N�g������*/ m_move.x += (0.0f - m_move.x) * ATTENUATE;
	/*�y�x�N�g������*/ m_move.z += (0.0f - m_move.z) * ATTENUATE;
	/*�x�x�N�g���d�͂ɏ]�킹��*/ m_move.y += (fGravityPower - m_move.y) * fGravityMag;
}

//=======================================
//���_�J���[�ݒ�
//=======================================
HRESULT CObject3D::SetVertexInfo(const D3DXCOLOR col)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

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
//���_���W�ݒ�
//=======================================
HRESULT CObject3D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fSizeWidth, -fSizeHeight, +fSizeDepth);
	pVtx[1].pos = D3DXVECTOR3(+fSizeWidth, -fSizeHeight, +fSizeDepth);
	pVtx[2].pos = D3DXVECTOR3(-fSizeWidth, +fSizeHeight, -fSizeDepth);
	pVtx[3].pos = D3DXVECTOR3(+fSizeWidth, +fSizeHeight, -fSizeDepth);

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
HRESULT CObject3D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos.x = sinf(m_rot.y + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = cosf(m_rot.y + PI_LEFT_UP) * fSizeHeight;
	pVtx[0].pos.z = cosf(m_rot.y + PI_LEFT_UP) * fSizeDepth;
	pVtx[1].pos.x = sinf(m_rot.y + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = cosf(m_rot.y + PI_RIGHT_UP) * fSizeHeight;
	pVtx[1].pos.z = cosf(m_rot.y + PI_RIGHT_UP) * fSizeDepth;
	pVtx[2].pos.x = sinf(m_rot.y + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = cosf(m_rot.y + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[2].pos.z = cosf(m_rot.y + PI_LEFT_DOWN) * fSizeDepth;
	pVtx[3].pos.x = sinf(m_rot.y + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = cosf(m_rot.y + PI_RIGHT_DOWN) * fSizeHeight;
	pVtx[3].pos.z = cosf(m_rot.y + PI_RIGHT_DOWN) * fSizeDepth;

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
HRESULT CObject3D::SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

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
HRESULT CObject3D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�������v�Z
	float fLength = D3DXVec2Length(&D3DXVECTOR2(fSizeWidth, fSizeDepth));

	//�S���_�S�Ă̊p�x���v�Z
	float fVtxAngle[VTX_MAX] = {};

	fVtxAngle[0] = atan2f(-fSizeWidth, +fSizeDepth);
	fVtxAngle[1] = atan2f(+fSizeWidth, +fSizeDepth);
	fVtxAngle[2] = atan2f(-fSizeWidth, -fSizeDepth);
	fVtxAngle[3] = atan2f(+fSizeWidth, -fSizeDepth);

	//���_���W�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		pVtx[nCntVtx].pos.x = sinf(fVtxAngle[nCntVtx]) * fLength;
		pVtx[nCntVtx].pos.z = cosf(fVtxAngle[nCntVtx]) * fLength;
		pVtx[nCntVtx].pos.y = fSizeHeight * (1.0f - ((nCntVtx + 2) / 2));
	}

	/*���_���猩�ď�̒��_*/pVtx[0].pos.y = pVtx[1].pos.y = +fSizeHeight;
	/*���_���猩�ĉ��̒��_*/pVtx[2].pos.y = pVtx[3].pos.y = -fSizeHeight;

	D3DXVECTOR3 pos0 = pVtx[0].pos;
	D3DXVECTOR3 pos1 = pVtx[1].pos;
	D3DXVECTOR3 pos2 = pVtx[2].pos;
	D3DXVECTOR3 pos3 = pVtx[3].pos;

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
HRESULT CObject3D::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos.x = sinf(m_rot.y + PI_LEFT_UP) * fSizeWidth;
	pVtx[0].pos.y = cosf(m_rot.y + PI_LEFT_UP) * fSizeHeight;
	pVtx[0].pos.z = cosf(m_rot.y + PI_LEFT_UP) * fSizeDepth;
	pVtx[1].pos.x = sinf(m_rot.y + PI_RIGHT_UP) * fSizeWidth;
	pVtx[1].pos.y = cosf(m_rot.y + PI_RIGHT_UP) * fSizeHeight;
	pVtx[1].pos.z = cosf(m_rot.y + PI_RIGHT_UP) * fSizeDepth;
	pVtx[2].pos.x = sinf(m_rot.y + PI_LEFT_DOWN) * fSizeWidth;
	pVtx[2].pos.y = cosf(m_rot.y + PI_LEFT_DOWN) * fSizeHeight;
	pVtx[2].pos.z = cosf(m_rot.y + PI_LEFT_DOWN) * fSizeDepth;
	pVtx[3].pos.x = sinf(m_rot.y + PI_RIGHT_DOWN) * fSizeWidth;
	pVtx[3].pos.y = cosf(m_rot.y + PI_RIGHT_DOWN) * fSizeHeight;
	pVtx[3].pos.z = cosf(m_rot.y + PI_RIGHT_DOWN) * fSizeDepth;

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
//�ꕔ�̒��_���W��ݒ�
//=======================================
HRESULT CObject3D::SetVertexInfo(float fAddHeight)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_�P�E�Q�̂x���W���グ��������
	pVtx[1].pos.y = pVtx[2].pos.y = m_pos.y + fAddHeight;

	D3DXVECTOR3 Vec1To0 = pVtx[1].pos - pVtx[0].pos;	//���_�O -> ���_�P�܂ł̃x�N�g��
	D3DXVECTOR3 Vec2To0 = pVtx[2].pos - pVtx[0].pos;	//���_�O -> ���_�Q�܂ł̃x�N�g��

	D3DXVECTOR3 Vec1To3 = pVtx[1].pos - pVtx[3].pos;	//���_�R -> ���_�P�܂ł̃x�N�g��
	D3DXVECTOR3 Vec2To3 = pVtx[2].pos - pVtx[3].pos;	//���_�R -> ���_�Q�܂ł̃x�N�g��

	D3DXVECTOR3 Nor0, Nor3;

	//�O�ς����߂�
	D3DXVec3Cross(&Nor0, &Vec1To0, &Vec2To0);
	D3DXVec3Cross(&Nor3, &Vec2To3, &Vec1To3);

	//���K��
	D3DXVec3Normalize(&Nor0, &Nor0);
	D3DXVec3Normalize(&Nor3, &Nor3);

	//�@����ݒ�
	pVtx[0].nor = Nor0;
	pVtx[1].nor = (Nor0 + Nor3) * 0.5f;
	pVtx[2].nor = (Nor0 + Nor3) * 0.5f;
	pVtx[3].nor = Nor3;

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
HRESULT CObject3D::GetVertexPos(D3DXVECTOR3 &rPos, const int nVtxID)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

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

//=======================================
//���_���W�Ɩ@�����擾
//=======================================
HRESULT CObject3D::GetVertexNor(D3DXVECTOR3 &rNor, const int nVtxID)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�w�肳�ꂽ���_���W�Ɩ@������
	rNor = pVtx[nVtxID].nor;

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//�ݒ芮��
	return S_OK;
}