//================================================================================================
//
//���b�V������[billboard.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../Object/object.h"
#include "mesh.h"
#include "../S_H_Lib/S_H_Calculation.h"

//�e�N�X�`���̃p�X
#define TEX_PATH		"data\\TEXTURE\\tree000.png"

//=======================================
//�R���X�g���N�^
//=======================================
CMesh::CMesh(const int nPriority) : CObject(nPriority)
{
	//�����o�ϐ��N���A
	m_nTextureID = ERROR_ID;
	m_fTexUSize = 0.0f;
	m_fTexVSize = 0.0f;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_fDepth = 0.0f;
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_pos = VEC3_INIT;
	m_rot = VEC3_INIT;
	m_move = VEC3_INIT;
	m_posOld = VEC3_INIT;
	m_col = GetColor(COL_CLEAR);
	m_Buffer.nNumIdx = 0;
	m_Buffer.nNumPoly = 0;
	m_Buffer.nNumVtx = 0;
	m_bFieldType = true;
	m_bDisp = true;
}

//=======================================
//�f�X�g���N�^
//=======================================
CMesh::~CMesh()
{

}

//=======================================
//�����������i�V�����_�[�p
//=======================================
HRESULT CMesh::Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, int nDevide, float fRadius, float fHeight, float fTexUSize, float fTexVSize)
{
	//----------	�����o�ϐ�������	----------
	/*  �ʒu�ݒ�  */	m_pos = pos;
	/* �J���[�ݒ� */	m_col = col;
	/* �o�b�t�@�[ */	m_Buffer = GetMeshBuffer(nDevide);
	/*	 ������	  */	m_nDevide = nDevide;
	/*	  ����	  */	m_fRadius = fRadius;
	/*	  ���a	  */	m_fHeight = fHeight;
	/*���b�V���^�C�v*/	m_bFieldType = false;

	m_nTextureID = CManager::GetTexture()->Regist("data\\TEXTURE\\ChargeCylinder.png");

	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_Buffer.nNumIdx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))	return E_FAIL;

	//�C���f�b�N�X�o�b�t�@�̐���
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * m_Buffer.nNumIdx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL)))	return E_FAIL;

	//���_��񏉊���
	if (FAILED(InitVtxInfo_Cylinder()))	return E_FAIL;

	//�C���f�b�N�X��񏉊���
	if (FAILED(InitIdxInfo_Cylinder()))	return E_FAIL;

	return S_OK;
}

//=======================================
//���_��񏉊����ݒ�i�V�����_�[�p
//=======================================
HRESULT CMesh::InitVtxInfo_Cylinder(void)
{
	//���_���̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))	return E_FAIL;

	//���_�̊p�x
	const float fRot = (D3DX_PI * 2.0f) / m_nDevide;
	float fRound = D3DX_PI;
	float fTexWidth = 1.0f / m_nDevide;

	//���_�����㉺�y�A�Őݒ�
	for (int nCntVtx = 0; nCntVtx < m_nDevide; nCntVtx++)
	{
		//��ʂ̒��_�ԍ�
		int nLowID = nCntVtx + m_nDevide + 1;

		//�ݒ肷��ʒu�i�x���W�����ς���
		D3DXVECTOR3 pos = D3DXVECTOR3(
			sinf(fRound) * m_fRadius,
			0.0f,
			cosf(fRound) * m_fRadius);

		// ���_���W
		pVtx[nCntVtx].pos = pVtx[nLowID].pos = pos;
		pVtx[nCntVtx].pos.y = m_fHeight;//�V�ʂ̍����ݒ�

		// �e�N�X�`�����W
		pVtx[nCntVtx].tex.x = pVtx[nLowID].tex.x = fTexWidth * nCntVtx;
		pVtx[nCntVtx].tex.y = 0.0f;
		pVtx[nLowID].tex.y = 1.0f;

		pVtx[nCntVtx].nor = pVtx[nLowID].nor = NOR_INIT;// nor������
		pVtx[nCntVtx].col = pVtx[nLowID].col = m_col;	// �F�w��

		fRound -= fRot;//���̒��_�̊p�x�֐ݒ�
	}

	//�Ō�̒��_��ݒ肷��
	pVtx[m_nDevide] = pVtx[0];
	pVtx[m_nDevide * 2 + 1] = pVtx[m_nDevide + 1];

	pVtx[m_nDevide].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[m_nDevide * 2 + 1].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))	return E_FAIL;

	//����������
	return S_OK;
}

//=======================================
//�C���f�b�N�X��񏉊����ݒ�i�V�����_�[�p
//=======================================
HRESULT CMesh::InitIdxInfo_Cylinder(void)
{
	WORD *pIdx;
	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void* *)&pIdx, 0);

	//��ʂ̒��_�ԍ�
	int nBottomNumber = ODDPARITY + m_nDevide;

	//�C���f�b�N�X�ԍ��f�[�^�̐ݒ�
	for (int nCntIdx = 0; nCntIdx < m_Buffer.nNumIdx; nCntIdx++)
	{
		//�J�E���^�[�������̏ꍇ�A�㔼�ȍ~�̌v�Z�� �� �̓������[���ɂȂ�	��̏ꍇ�́A�V�����_�[�����̊���_�ԍ������Z�����
		pIdx[nCntIdx] = (nCntIdx / EVENPARITY) + (nBottomNumber * (nCntIdx % EVENPARITY));
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();
	//����������
	return S_OK;
}

//=======================================
//�����������i�t�B�[���h�p
//=======================================
HRESULT CMesh::Init(const D3DXVECTOR3 pos, const D3DXCOLOR col, int nColumn, int nRow, float fTexUSize, float fTexVSize)
{
	//----------	�����o�ϐ�������	----------
	/*  �ʒu�ݒ�  */	m_pos = pos;
	/* �J���[�ݒ� */	m_col = col;
	/* �o�b�t�@�[ */	m_Buffer = GetMeshBuffer(nColumn, nRow);
	/*���b�V���^�C�v*/	m_bFieldType = true;

	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_Buffer.nNumIdx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))	return E_FAIL;

	//���_��񏉊���
	if (FAILED(InitVtxInfo_Field()))	return E_FAIL;

	//�C���f�b�N�X��񏉊���
	if (FAILED(InitIdxInfo_Field()))	return E_FAIL;

	SetType(CObject::TYPE_MESH);

	return S_OK;
}

//=======================================
//���_��񏉊����ݒ�i�t�B�[���h�p
//=======================================
HRESULT CMesh::InitVtxInfo_Field(void)
{
	//���_���̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))	return E_FAIL;

	//rhw�E���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < m_Buffer.nNumIdx; nCntVtx++)
	{
		pVtx[nCntVtx].pos = VEC3_INIT;	// ���_���W
		pVtx[nCntVtx].nor = NOR_INIT;	// nor������
		pVtx[nCntVtx].col = m_col;		// �F�w��
		pVtx[nCntVtx].tex = VEC2_INIT;	// �e�N�X�`�����W
	}

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))	return E_FAIL;

	//����������
	return S_OK;
}

//=======================================
//�C���f�b�N�X��񏉊����ݒ�i�t�B�[���h�p
//=======================================
HRESULT CMesh::InitIdxInfo_Field(void)
{
	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CMesh::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �������g�̏I������
	this->Release();
}

//=======================================
//�X�V����
//=======================================
void CMesh::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CMesh::Draw(void)
{
	//�`�悵�Ȃ�
	if (!m_bDisp) return;

	//�f�o�C�X���擾
	CRenderer *pRender = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

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

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(m_nTextureID));

	//���ʃJ�����O
	pRender->SetCulling(D3DCULL_NONE);

	//���e�X�gON
	pRender->SetAlphaTest(true, 0);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_Buffer.nNumIdx, 0, m_Buffer.nNumIdx);

	//���e�X�gOFF
	pRender->SetAlphaTest(false, 255);

	//�ʏ�J�����O
	pRender->SetCulling();
}

//=======================================
//���_�J���[�ݒ�
//=======================================
HRESULT CMesh::SetVertexInfo(const D3DXCOLOR col)
{
	//�|�C���^�ݒ�
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	if (FAILED(m_pVtxBuff->Lock(0, 0, (void* *)&pVtx, 0)))return E_FAIL;

	//���_�J���[�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < m_Buffer.nNumVtx; nCntVtx++)
	{
		pVtx[nCntVtx].col = col;
	}
	m_col = col;

	//���_�o�b�t�@���J���i�A�����b�N�j����
	if (FAILED(m_pVtxBuff->Unlock()))	return E_FAIL;

	//�ݒ芮��
	return S_OK;
}

//=======================================
//���_���W�ݒ�
//=======================================
HRESULT CMesh::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth)
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
HRESULT CMesh::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXCOLOR col)
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
HRESULT CMesh::SetVertexInfo(const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
HRESULT CMesh::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
HRESULT CMesh::SetVertexInfo(const float fSizeWidth, const float fSizeHeight, const float fSizeDepth, D3DXCOLOR col, const D3DXVECTOR2 Texpos, const float fTexWidth, const float fTexHeight)
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
HRESULT CMesh::GetVertexInfo(D3DXVECTOR3 &rPos, const int nVtxID)
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