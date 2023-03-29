/*==========================================================================================================================================================

														�G�t�F�N�g����[effect.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "effect.h"
#include "shadow.h"
#include "color.h"
#include "billboard.h"
#include "wall.h"

//�}�N����`
#define MAX_EFFECT		(20000)		//�G�t�F�N�g�̍ő吔
#define EFFECT_SIZE		(10.0f)		//�G�t�F�N�g�̑傫��
#define EFFECT_LIFE		(50)		//�G�t�F�N�g�̎���

//�G�t�F�N�g�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;			//�F
	float fRadius;			//���a(�傫��)
	int nLife;				//����(�\������)
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Effect;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX				g_mtxWorldEffect;			//���[���h�}�g���b�N�X
Effect					g_aEffect[MAX_EFFECT];		//�G�t�F�N�g�̏��
LPD3DXMESH				g_pMeshEffect = NULL;		//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER			g_pBuffMatEffect = NULL;	//�}�e���A���ւ̃|�C���^
DWORD					g_dwNumMatEffect = ZERO;	//�}�e���A���̐�

//------------------------------------------------
//			�G�t�F�N�g�̏���������
//------------------------------------------------
void InitEffect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�G�t�F�N�g�̏��̏�����
	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos		= ZERO_SET;			//�ʒu�̏�����
		g_aEffect[nCntEffect].col		= XCOL_WHITE;		//�F�̏�����
		g_aEffect[nCntEffect].fRadius	= F_ZERO;			//���a(�傫��)�̏�����
		g_aEffect[nCntEffect].nLife		= EFFECT_LIFE;		//�����̏�����
		g_aEffect[nCntEffect].bUse		= false;			//�g�p���Ă��Ȃ���Ԃɂ���

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, ORIGIN_POS);
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
			pVtx[VTX_LE_UP].col = XCOL_YELLOW;
			pVtx[VTX_RI_UP].col = XCOL_YELLOW;
			pVtx[VTX_LE_DO].col = XCOL_YELLOW;
			pVtx[VTX_RI_DO].col = XCOL_YELLOW;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(MAX_POS, ORIGIN_POS);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(ORIGIN_POS, MAX_POS);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2( MAX_POS,    MAX_POS);
		}

		pVtx += VTX_MAX;			//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEffect->Unlock();

	/*//X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/Effect.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatEffect, NULL, &g_dwNumMatEffect, &g_pMeshEffect);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatEffect->GetBufferPointer();

	if (pMat->pTextureFilename != NULL)
	{//�e�N�X�`���t�@�C�������݂���
	 //�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &g_pTextureEffect);
	}*/
}

//------------------------------------------------
//				�G�t�F�N�g�̏I������
//------------------------------------------------
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	/*//���b�V���̔j��
	if (g_pMeshEffect != NULL)
	{
		g_pMeshEffect->Release();
		g_pMeshEffect = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatEffect != NULL)
	{
		g_pBuffMatEffect->Release();
		g_pBuffMatEffect = NULL;
	}*/

}

//------------------------------------------------
//				�G�t�F�N�g�̍X�V����
//------------------------------------------------
void UpdateEffect(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă���
			 //�����Ƒ傫�������炵�Ă���
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].fRadius >= F_ZERO)
			{
				g_aEffect[nCntEffect].fRadius -= 0.2f;
			}

			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].fRadius / 200.0f;

			if (g_aEffect[nCntEffect].nLife <= ZERO || g_aEffect[nCntEffect].fRadius < F_ZERO)
			{
				g_aEffect[nCntEffect].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}

			//���_���W�̐ݒ�
			{
				pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, +g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[VTX_LE_UP].col = g_aEffect[nCntEffect].col;
				pVtx[VTX_RI_UP].col = g_aEffect[nCntEffect].col;
				pVtx[VTX_LE_DO].col = g_aEffect[nCntEffect].col;
				pVtx[VTX_RI_DO].col = g_aEffect[nCntEffect].col;
			}

		}

		//���_�f�[�^�̃|�C���^��4���i�߂�
		pVtx += VTX_MAX;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//------------------------------------------------
//				�G�t�F�N�g�̕`�揈��
//------------------------------------------------
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;						//�v�Z�p�}�g���b�N�X
	BillTest *pBill = GetAlphaTest();			//�r���{�[�h�̃��EZ�e�X�g�����擾
	D3DXMATRIX mtxView;							//�r���[�}�g���b�N�X�擾�p

	/*LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^*/

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	
	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Z�o�b�t�@�̗L�������ݒ�
	
	//�r���{�[�h�̕`��ݒ�
	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true &&				//�G�t�F�N�g���g�p����Ă���
			WallDrawObject(g_aEffect[nCntEffect].pos) == true)	//�G�t�F�N�g�ƃJ�����̊Ԃɕǂ��Ȃ�
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldEffect);

			//�r���[�}�g���b�N�X��Get
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_mtxWorldEffect, NULL, &mtxView);	//�t�s������߂�
			g_mtxWorldEffect._41 = F_ZERO;		//����
			g_mtxWorldEffect._42 = F_ZERO;		//	���]�����ċ��߂��t�s��̕s�v�ȕ������[���ɂ���
			g_mtxWorldEffect._43 = F_ZERO;		//����

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffEffect, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureEffect);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * VTX_MAX, SEC);

			/*//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_pBuffMatEffect->GetBufferPointer();

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat->MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureEffect);

			//���f��(�p�[�c)�̕`��
			g_pMeshEffect->DrawSubset(0);

			//�}�e���A����߂�
			pDevice->SetMaterial(&matDef);*/
		}
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL �� �u<=�v�̂���
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Z�o�b�t�@�̗L�������ݒ�

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//�G�t�F�N�g�̐ݒ菈��
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntEffect = START_CNT; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�

			//�G�t�F�N�g���̐ݒ�
			g_aEffect[nCntEffect].pos = pos;			//�ʒu���
			g_aEffect[nCntEffect].col = col;			//�F�ʑ��
			g_aEffect[nCntEffect].fRadius = fRadius;	//���a���
			g_aEffect[nCntEffect].nLife = EFFECT_LIFE;	//�������
			g_aEffect[nCntEffect].bUse = true;			//�g�p���Ă����Ԃɂ���

			//���_���̐ݒ�
			//���_���W�̐ݒ�
			{
				pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[VTX_RI_UP].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
				pVtx[VTX_RI_DO].pos = D3DXVECTOR3(  g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, F_ZERO);
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[VTX_LE_UP].col = g_aEffect[nCntEffect].col;
				pVtx[VTX_RI_UP].col = g_aEffect[nCntEffect].col;
				pVtx[VTX_LE_DO].col = g_aEffect[nCntEffect].col;
				pVtx[VTX_RI_DO].col  = g_aEffect[nCntEffect].col;
			}

			break;
		}
		pVtx += VTX_MAX;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffEffect->Unlock();
}