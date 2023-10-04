//================================================================================================
//
//�r���{�[�h����[billboard.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Object/object.h"
#include "billboard.h"

//�e�N�X�`���̃p�X
#define TEX_PATH		"data\\TEXTURE\\tree000.png"

//�ÓI�����o�ϐ��錾
int CObjectBillboard::m_nTextureID = ERROR_ID;

//=======================================
//�R���X�g���N�^
//=======================================
CObjectBillboard::CObjectBillboard(const int nPriority) : CObject(nPriority)
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
	m_bZTest = true;
}

//=======================================
//�f�X�g���N�^
//=======================================
CObjectBillboard::~CObjectBillboard()
{

}

//=======================================
//����������
//=======================================
HRESULT CObjectBillboard::Init(const D3DXVECTOR3 pos, const float fWidth, const float fHeight, const float fDepth, const D3DXCOLOR col, const D3DXVECTOR2 TexMin, const D3DXVECTOR2 TexMax)
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
	pVtx[0].pos = D3DXVECTOR3(-fWidth, +fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+fWidth, +fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+fWidth, -fHeight, 0.0f);

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

	return S_OK;
}

//=======================================
//�I������
//=======================================
void CObjectBillboard::Uninit(void)
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
void CObjectBillboard::Update(void)
{
	//=============================
	//�e��I�u�W�F�N�g�̍X�V����
	//=============================
	CObject::UpdateAll();
}

//=======================================
//�`�揈��
//=======================================
void CObjectBillboard::Draw(void)
{
	/*		�����_���[�擾		*/	CRenderer *pRender = CManager::GetRenderer();
	/*		�f�o�C�X���擾		*/	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	/*	  �v�Z�p�}�g���b�N�X	*/	D3DXMATRIX mtxTrans;
	/* �r���[�}�g���b�N�X�擾�p */	D3DXMATRIX mtxView;

	//���e�X�g��L���ɂ���
	pRender->SetAlphaTest(true, 0);

	//Z�e�X�g�𖳌��ɂ���
	if (m_bZTest)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X��Get
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//=========================================
	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	//=========================================
	//�t�s������߂�
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	//���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
	m_mtxWorld._41 = m_mtxWorld._42 = m_mtxWorld._43 = 0.0f;

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

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL �� �u<=�v�̂���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Z�o�b�t�@�̗L�������ݒ�

	//���e�X�g�𖳌��ɂ���
	pRender->SetAlphaTest(false, 255);
}

//=======================================
//���_�J���[�ݒ�
//=======================================
HRESULT CObjectBillboard::SetVertexInfo(const D3DXCOLOR col)
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
HRESULT CObjectBillboard::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fSizeWidth, -fSizeHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+fSizeWidth, -fSizeHeight, 0.0f);

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
HRESULT CObjectBillboard::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fSizeWidth, -fSizeHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+fSizeWidth, -fSizeHeight, 0.0f);

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
HRESULT CObjectBillboard::SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
HRESULT CObjectBillboard::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))
	{//���s������G���[��Ԃ�
		return E_FAIL;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+fSizeWidth, +fSizeHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fSizeWidth, -fSizeHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+fSizeWidth, -fSizeHeight, 0.0f);

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
HRESULT CObjectBillboard::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
//���_���W���擾
//=======================================
HRESULT CObjectBillboard::GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID)
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