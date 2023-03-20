/*==========================================================================================================================================================

														�r���{�[�h����[billboard.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "billboard.h"

#define NUM_BILLBOARD		(2)									//�r���{�[�h�̐�
#define BILLBOARD_POS		D3DXVECTOR3(-200.0f, 0.0f, 0.0f)	//�ʒu
#define BILLBOARD_WIDTH		(80.0f)								//���T�C�Y
#define BILLBOARD_HEIGHT	(180.0f)							//���s���T�C�Y
#define BILLBOARD_NOR		D3DXVECTOR3(0.0f, 0.0f, -1.0f)		//nor �̒l
#define MAX_ALPHA			(255)								//���e�X�g�̍ő�l

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;		//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureBillboard = NULL;		//�e�N�X�`��
D3DXMATRIX				g_mtxWorldBillboard;			//���[���h�}�g���b�N�X
Billboard				g_Billboard[NUM_BILLBOARD];		//�r���{�[�h�̏��
BillTest				g_BillTest;						//���e�X�g�Ȃǂ̏��

//------------------------------------------------
//			�r���{�[�h�̏���������
//------------------------------------------------
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tree000.png", &g_pTextureBillboard);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_BILLBOARD, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBillboard, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBill = START_CNT; nCntBill < NUM_BILLBOARD; nCntBill++, pVtx += VTX_MAX)
	{
		g_Billboard[nCntBill].pos = BILLBOARD_POS;	//�ʒu������
		g_Billboard[nCntBill].rot = ZERO_SET;		//����������
		g_Billboard[nCntBill].pos.x += nCntBill * 400.0f;

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-BILLBOARD_WIDTH,	BILLBOARD_HEIGHT,	g_Billboard[nCntBill].pos.z);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+BILLBOARD_WIDTH,	BILLBOARD_HEIGHT,	g_Billboard[nCntBill].pos.z);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-BILLBOARD_WIDTH,			F_ZERO,		g_Billboard[nCntBill].pos.z);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+BILLBOARD_WIDTH,			F_ZERO,		g_Billboard[nCntBill].pos.z);
		}

		//nor�̐ݒ�
		{
			pVtx[VTX_LE_UP].nor = SET_NOR;
			pVtx[VTX_RI_UP].nor = SET_NOR;
			pVtx[VTX_LE_DO].nor = SET_NOR;
			pVtx[VTX_RI_DO].nor = SET_NOR;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(F_TEN, F_TEN);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBillboard->Unlock();

	g_BillTest.AlphaTest = ZERO;	//���e�X�g�̒l������
	g_BillTest.bAlphaTest = true;	//���e�X�g���s���Ă��邩
	g_BillTest.bZTest = false;		//Z�e�X�g���s���Ă��邩
}	

//------------------------------------------------
//			�r���{�[�h�̏I������
//------------------------------------------------
void UninitBillboard(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//------------------------------------------------
//			�r���{�[�h�̍X�V����
//------------------------------------------------
void UpdateBillboard(void)
{

}

//------------------------------------------------
//			�r���{�[�h�̕`�揈��
//------------------------------------------------
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p

	//���e�X�g��L���ɂ���
	if (g_BillTest.bAlphaTest == true)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, g_BillTest.AlphaTest);
	}

	//Z�e�X�g�𖳌��ɂ���
	if (g_BillTest.bZTest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Z�o�b�t�@�̗L�������ݒ�
	}	

	//�r���{�[�h�̕`��ݒ�
	for (int nCntBill = START_CNT; nCntBill < NUM_BILLBOARD; nCntBill++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldBillboard);

		//�r���[�}�g���b�N�X��Get
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);	//�t�s������߂�
		g_mtxWorldBillboard._41 = F_ZERO;		//����
		g_mtxWorldBillboard._42 = F_ZERO;		//	���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
		g_mtxWorldBillboard._43 = F_ZERO;		//����

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCntBill].pos.x, g_Billboard[nCntBill].pos.y, g_Billboard[nCntBill].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(ZERO, g_pVtxBuffBillboard, ZERO, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureBillboard);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBill * VTX_MAX, SEC);
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL �� �u<=�v�̂���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Z�o�b�t�@�̗L�������ݒ�

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, g_BillTest.AlphaTest);
}

//�r���{�[�h�̏��擾
BillTest *GetAlphaTest(void)
{
	return &g_BillTest;
}