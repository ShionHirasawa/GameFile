/*==========================================================================================================================================================

															�e�̏���[shadow.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "input.h"
#include "color.h"
#include "shadow.h"
#include "meshfield.h"
#include "model.h"
#include "slope.h"
#include "player.h"

#define SHADOW_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define SHADOW_DISP_MIN_Y	(-200.0f)						//�e��\���ł���Œ�Y���W
#define SHADOW_ALPTH		(200)							//�e�̃��l
#define SHADOW_ALPHATEST	(0)								//�e�̃��e�X�g�̊�l
#define MAX_SHADOW			(512)							//�e�̍ő吔

//�e�̍\����
typedef struct
{
	D3DXVECTOR3		pos;		// �ʒu
	D3DXVECTOR3		rot;		// ����
	int				aColor;		// �e�̃��l
	float			fWidht;		// ��
	float			fHeight;	// �⓹�̎������Ɏg�p
	float			fDepth;		// ���s��
	bool			bSet;		// �Ώۂ̉e�����܂��Ă��邩�ǂ���
	bool			bUse;		// �g���Ă��邩�ǂ���
}Shadow;

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	//�e�N�X�`��
D3DXMATRIX				g_mtxWorldShadow;			//���[���h�}�g���b�N�X
Shadow					g_aShadow[MAX_SHADOW];		//�e�̏��
int g_nAlphaShadow = 0;

//------------------------------------------------
//				�e�̏���������
//------------------------------------------------
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * MAX_SHADOW, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx+= VTX_MAX)
	{
		g_aShadow[nCntShadow].pos = SHADOW_POS;			//�ʒu������
		g_aShadow[nCntShadow].rot = ZERO_SET;			//����������
		g_aShadow[nCntShadow].fWidht = NIL_F;			//��������
		g_aShadow[nCntShadow].fHeight = NIL_F;			//��������
		g_aShadow[nCntShadow].fDepth = NIL_F;			//���s��������
		g_aShadow[nCntShadow].aColor = SHADOW_ALPTH;	//�e�̃��l������
		g_aShadow[nCntShadow].bSet = false;				//�g�p����Ă��Ȃ�
		g_aShadow[nCntShadow].bUse = false;				//�g�p����Ă��Ȃ�

		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidht, +g_aShadow[nCntShadow].fHeight, +g_aShadow[nCntShadow].fDepth);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidht, +g_aShadow[nCntShadow].fHeight, +g_aShadow[nCntShadow].fDepth);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidht, -g_aShadow[nCntShadow].fHeight, -g_aShadow[nCntShadow].fDepth);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidht, -g_aShadow[nCntShadow].fHeight, -g_aShadow[nCntShadow].fDepth);
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
			pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
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
	g_pVtxBuffShadow->Unlock();

	g_nAlphaShadow = 0;
}

//------------------------------------------------
//				�e�̏I������
//------------------------------------------------
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//------------------------------------------------
//				�e�̍X�V����
//------------------------------------------------
void UpdateShadow(void)
{
	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++ ,pVtx += VTX_MAX)
	{
		//�Ώۂ̉e���g���Ă��āA���Ɏg���e�����܂��Ă���
		if (g_aShadow[nCntShadow].bUse == true && g_aShadow[nCntShadow].bSet == true)
		{
			//���_���W�̐ݒ�
			{
				pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidht, +g_aShadow[nCntShadow].fHeight, +g_aShadow[nCntShadow].fDepth);
				pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidht, +g_aShadow[nCntShadow].fHeight, +g_aShadow[nCntShadow].fDepth);
				pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidht, -g_aShadow[nCntShadow].fHeight, -g_aShadow[nCntShadow].fDepth);
				pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aShadow[nCntShadow].fWidht, -g_aShadow[nCntShadow].fHeight, -g_aShadow[nCntShadow].fDepth);
			}

			//���_�J���[�̐ݒ�
			{
				pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
				pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
				pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
				pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//------------------------------------------------
//				�e�̕`�揈��
//------------------------------------------------
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, g_nAlphaShadow);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldShadow);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);

			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(ZERO, g_pVtxBuffShadow, ZERO, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(ZERO, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * VTX_MAX, SEC);
		}
	}

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, g_nAlphaShadow);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//�e�̐ݒ菈��
int SetShadow(void)
{
	int nCntShadow = MINUS_ONE;

	for (int mCntShadow = START_CNT; mCntShadow < MAX_SHADOW; mCntShadow++)
	{
		nCntShadow++;

		if (g_aShadow[mCntShadow].bUse == false && g_aShadow[mCntShadow].bSet == false)
		{
			g_aShadow[mCntShadow].pos = SHADOW_POS;	//�ʒu������
			g_aShadow[mCntShadow].rot = ZERO_SET;	//����������
			g_aShadow[mCntShadow].bSet = true;		//�Ώۂ����܂���
			break;
		}
	}

	return nCntShadow;
}

//�e�̈ʒu�ݒ菈��
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fWIdth, float fDepth) 
{
	g_aShadow[nIdxShadow].pos = pos;

	//�e��`�悷��Y���W�̐ݒ�
	DrawPos_YShadow(nIdxShadow);

	//�e�Ɖe���g���Ă�����̂�Y���W�̋���
	float fLength = (pos.y - g_aShadow[nIdxShadow].pos.y) * 1.3f;

	//�e�Ƃ��̉e�̎�����̋��������ꂷ���Ă���
	if (fLength >= SHADOW_ALPTH)
	{
		fLength = SHADOW_ALPTH;
	}

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	pVtx += VTX_MAX * nIdxShadow;		//���_���W���C���f�b�N�X���i�߂�

	g_aShadow[nIdxShadow].fWidht = fWIdth;									//������
	g_aShadow[nIdxShadow].fDepth = fDepth;									//���s������
	g_aShadow[nIdxShadow].aColor = SHADOW_ALPTH - (int)fLength;				//���l����
	g_aShadow[nIdxShadow].bUse = true;										//�g�p����

	//���_���W�̐ݒ�
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidht, +g_aShadow[nIdxShadow].fHeight, +g_aShadow[nIdxShadow].fDepth);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_aShadow[nIdxShadow].fWidht, +g_aShadow[nIdxShadow].fHeight, +g_aShadow[nIdxShadow].fDepth);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidht, -g_aShadow[nIdxShadow].fHeight, -g_aShadow[nIdxShadow].fDepth);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_aShadow[nIdxShadow].fWidht, -g_aShadow[nIdxShadow].fHeight, -g_aShadow[nIdxShadow].fDepth);
	}

	//���_�J���[�̐ݒ�
	{
		pVtx[VTX_LE_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[VTX_RI_UP].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[VTX_LE_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[VTX_RI_DO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//�e��`�悷��Y���W�̈ʒu�ݒ�
void DrawPos_YShadow(int nIdxShadow)
{	
	float FixPosY = SHADOW_DISP_MIN_Y;		//�C������ʒu
	g_aShadow[nIdxShadow].fHeight = F_ZERO;	//����������
	g_aShadow[nIdxShadow].rot = ZERO_SET;	//����������

	//���b�V���t�B�[���h�ɉe��`�悷�邩�̔���
	ColliFieldShadow(g_aShadow[nIdxShadow].pos, &FixPosY);

	//���f���̏㕔�ɉe��`�悷�邩�̔���
	ColliModelShadow(nIdxShadow, g_aShadow[nIdxShadow].pos, &FixPosY);

	//�⓹�ɉe��`�悷��
	ColliSlopeShadow(g_aShadow[nIdxShadow].pos, &g_aShadow[nIdxShadow].rot, &FixPosY, &g_aShadow[nIdxShadow].fHeight);

	//�`�悷��ʒu�C��
	g_aShadow[nIdxShadow].pos.y = FixPosY;
}

//�e�̎g�p�𖳂���
void BreakShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}