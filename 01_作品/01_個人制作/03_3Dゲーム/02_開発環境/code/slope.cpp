/*==========================================================================================================================================================

															�⓹�̏���[slope.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "polygon.h"
#include "slope.h"

//�}�N����`
#define NUM_SLOPE				(2)								// �⓹�̐�
#define SLOPE_NOR				(D3DXVECTOR3(0.0f, 1.0f, 0.0f))	// �⓹��nor
#define SLOPE_FILE				"data/CSV/slope.csv"			// �⓹�̏�񂪓������t�@�C����
#define SLOPE_CORRECT_SIZE		(1.0f)							// �⓹�̃T�C�Y�̒����p
#define GO_UP_a_SLOPE_MAG		(0.9f)							// �⓹�����Ƃ��̈ړ����x�̔{��
#define GO_DOWN_a_SLOPE_MAG		(0.4f)							// �⓹������Ƃ��̈ړ����x�̔{��
#define SLOPE_FRONT_BACK_ROT	(0.25f)							// �O��̍⓹��ʂ�Ƃ��̃v���C���[�̊p�x
#define SLOPE_RIGHT_LEFT_ROT	(0.50f)							// ���E�⓹��ʂ�Ƃ��̃v���C���[�̊p�x

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSlope = NULL;		//���_�o�b�t�@
LPDIRECT3DTEXTURE9		g_pTextureSlope = NULL;		//�e�N�X�`��
D3DXMATRIX				g_mtxWorldSlope;			//���[���h�}�g���b�N�X
Slope					g_Slope[NUM_SLOPE];			//�⓹�̏��


//------------------------------------------------
//				�⓹�̏���������
//------------------------------------------------
void InitSlope(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�ւ̃|�C���^	

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * VTX_MAX * NUM_SLOPE, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffSlope, NULL);

	//�|�C���^��ݒ�
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSlope->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	/*-------------------------------
			�⓹�̏��ǂݍ���
	---------------------------------*/
	LoadSlope();

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field.png", &g_pTextureSlope);	//�e�N�X�`���̓ǂݍ���

	//�e����E���_���W�̏�����
	for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++, pVtx += VTX_MAX)
	{
		//�����ɂ�钸�_���W�̕��򏈗�
		switch (g_Slope[nCntSlope].sloperot)
		{
		case SLOPEROT_FRONT_BACK:
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(-g_Slope[nCntSlope].fWidth, g_Slope[nCntSlope].fHeight, +g_Slope[nCntSlope].fDepth);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Slope[nCntSlope].fWidth, g_Slope[nCntSlope].fHeight, +g_Slope[nCntSlope].fDepth);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(-g_Slope[nCntSlope].fWidth, NIL_F, NIL_F);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(+g_Slope[nCntSlope].fWidth, NIL_F, NIL_F);
			break;
			
		case SLOPEROT_RIGHT_LEFT:
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(+g_Slope[nCntSlope].fWidth, +g_Slope[nCntSlope].fHeight, +g_Slope[nCntSlope].fDepth);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(+g_Slope[nCntSlope].fWidth, +g_Slope[nCntSlope].fHeight, -g_Slope[nCntSlope].fDepth);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(NIL_F, NIL_F, +g_Slope[nCntSlope].fDepth);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(NIL_F, NIL_F, -g_Slope[nCntSlope].fDepth);
			break;
		}

		//nor�̐ݒ�
		{
			pVtx[VTX_LE_UP].nor = SLOPE_NOR;
			pVtx[VTX_RI_UP].nor = SLOPE_NOR;
			pVtx[VTX_LE_DO].nor = SLOPE_NOR;
			pVtx[VTX_RI_DO].nor = SLOPE_NOR;
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
			pVtx[VTX_LE_UP].tex = ORIGIN_TEX;
			pVtx[VTX_RI_UP].tex = MA_ZE_TEX;
			pVtx[VTX_LE_DO].tex = ZE_MA_TEX;
			pVtx[VTX_RI_DO].tex = LIMIT_TEX;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffSlope->Unlock();
}

//------------------------------------------------
//				�⓹�̏I������
//------------------------------------------------
void UninitSlope(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureSlope != NULL)
	{
		g_pTextureSlope->Release();
		g_pTextureSlope = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSlope != NULL)
	{
		g_pVtxBuffSlope->Release();
		g_pVtxBuffSlope = NULL;
	}
}

//------------------------------------------------
//				�⓹�̍X�V����
//------------------------------------------------
void UpdateSlope(void)
{

}

//------------------------------------------------
//				�⓹�̕`�揈��
//------------------------------------------------
void DrawSlope(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

	for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldSlope);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Slope[nCntSlope].rot.y, g_Slope[nCntSlope].rot.x, g_Slope[nCntSlope].rot.z);

		D3DXMatrixMultiply(&g_mtxWorldSlope, &g_mtxWorldSlope, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Slope[nCntSlope].pos.x, g_Slope[nCntSlope].pos.y, g_Slope[nCntSlope].pos.z);

		D3DXMatrixMultiply(&g_mtxWorldSlope, &g_mtxWorldSlope, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSlope);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(ZERO, g_pVtxBuffSlope, ZERO, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(ZERO, g_pTextureSlope);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSlope * VTX_MAX, SEC);
	}
}

//�⓹�̓����蔻�菈��
bool CollisionSlope(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, COLLITYPE nType)
{
	bool bLand = false;				//��������ǂ�������
	bool bCollison = false;			//�⓹�Ɠ����������ǂ����i�e����j
	float fOldPercent = F_ZERO;		//�ӂ��Ƃ���O��̋����̊���
	float fPercent = F_ZERO;		//�ӂ��Ƃ��獡��̋����̊���
	float FixHeight = F_ZERO;		//����C�����鍂�����i�[

	float fSlopeRot = F_ZERO;		//�⓹�̊p�x
	float FixStandardRot = F_ZERO;	//�C������p�x�̊
	float MinDownRot = F_ZERO;		//�����Ă锻����Ƃ�ŏ��p�x
	float MaxDownRot = F_ZERO;		//�����Ă锻����Ƃ�ő�p�x

	D3DXVECTOR3 BasePos;			//�ӂ��Ƃ̍��W
	D3DXVECTOR3 PeakPos;			//����̍��W

	for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++)
	{
		fOldPercent = F_ZERO;				//�ӂ��Ƃ���O��̋����̊���
		fPercent = F_ZERO;					//�ӂ��Ƃ��獡��̋����̊���
		FixHeight = F_ZERO;					//����C�����鍂�����i�[

		BasePos = g_Slope[nCntSlope].pos;	//�ӂ��Ƃ̍��W
		PeakPos = g_Slope[nCntSlope].pos;	//����̍��W

		//��������
		switch (g_Slope[nCntSlope].sloperot)
		{
		case SLOPEROT_FRONT_BACK:	//�O��ɐL�т��
		{
			//�ӂ��Ƃ̈ʒu��ݒ�
			BasePos.x += g_Slope[nCntSlope].fWidth;		//�ӂ��Ƃ�X���W�̈ʒu��ݒ�

			//����̈ʒu��ݒ�
			PeakPos.x -= g_Slope[nCntSlope].fWidth;		//�����X���W�̈ʒu��ݒ�
			PeakPos.y += g_Slope[nCntSlope].fHeight;	//�����Y���W�̈ʒu��ݒ�
			PeakPos.z += g_Slope[nCntSlope].fDepth;		//�����Z���W�̈ʒu��ݒ�

			//���_�Ƃӂ��Ƃ̊Ԃɂ���
			if (PeakPos.x <= pPosOld->x && BasePos.x >= pPos->x &&
				PeakPos.z >= pPosOld->z && BasePos.z <= pPos->z &&
				PeakPos.y >= pPosOld->y && BasePos.y <= pPos->y)
			{
				//�O��̈ʒu�����Z�o
				fOldPercent = fabsf(pPosOld->z - BasePos.z) / g_Slope[nCntSlope].fDepth;

				//����̈ʒu�����Z�o
				fPercent = fabsf(pPos->z - BasePos.z) / g_Slope[nCntSlope].fDepth;

				//�⓹�̊p�x����
				fSlopeRot = SLOPE_FRONT_BACK_ROT;

				//�C������p�x�̊�l��ݒ�
				FixStandardRot = FIX_ROT;

				MinDownRot = LEFT;		//�����Ă��锻����Ƃ�ŏ��l��ݒ�
				MaxDownRot = RIGHT;		//�����Ă��锻����Ƃ�ő�l��ݒ�
			}

			//���Ȃ���΃��[�v������񕪔�΂�
			else
			{
				continue;
			}
		}
		break;

		case SLOPEROT_RIGHT_LEFT:	//���E�ɍL�����
		{
			//�ӂ��Ƃ̈ʒu��ݒ�
			BasePos.z += g_Slope[nCntSlope].fDepth;		//�ӂ��Ƃ�Z���W�̈ʒu��ݒ�

			//����̈ʒu��ݒ�
			PeakPos.x += g_Slope[nCntSlope].fWidth;		//�����X���W�̈ʒu��ݒ�
			PeakPos.y += g_Slope[nCntSlope].fHeight;	//�����Y���W�̈ʒu��ݒ�
			PeakPos.z -= g_Slope[nCntSlope].fDepth;		//�����Z���W�̈ʒu��ݒ�

			//���_�Ƃӂ��Ƃ̊Ԃɂ���
			if (PeakPos.x >= pPosOld->x && BasePos.x <= pPos->x &&
				PeakPos.z <= pPosOld->z && BasePos.z >= pPos->z &&
				PeakPos.y >= pPosOld->y && BasePos.y <= pPos->y)
			{
				//�O��̈ʒu�����Z�o
				fOldPercent = fabsf(pPosOld->x - BasePos.x) / g_Slope[nCntSlope].fWidth;

				//����̈ʒu�����Z�o
				fPercent = fabsf(pPos->x - BasePos.x) / g_Slope[nCntSlope].fWidth;

				//�⓹�̊p�x����
				fSlopeRot = SLOPE_RIGHT_LEFT_ROT;

				//�C������p�x�̊�l��ݒ�
				FixStandardRot = D3DX_PI;

				MinDownRot = -D3DX_PI;		//�����Ă��锻����Ƃ�ŏ��l��ݒ�
				MaxDownRot = F_ZERO;		//�����Ă��锻����Ƃ�ő�l��ݒ�
			}

			//���Ȃ���΃��[�v������񕪔�΂�
			else
			{
				continue;
			}
		}
		break;
		}

		//��������ɍ������Z�o
		FixHeight = BasePos.y + (g_Slope[nCntSlope].fHeight * fPercent);

		//�����������̂ɂ���Ĕ����ς���
		switch (nType)
		{
		case COLLITYPE_BULLET:		/*�e�̏ꍇ*/
		{
			//�Ώۂ̈ʒu���⓹�̍����ȉ�
			if (pPos->y <= FixHeight)
			{
				return bCollison = true;
			}
		}
			break;

		case COLLITYPE_PLAYER:		/*�v���C���[�̏ꍇ*/
		{
			//�O��ƍ���̂ӂ��Ƃ���̋����̊������Ⴄ �i�ړ����Ă���j
			if (fOldPercent != fPercent)
			{
				//����Ă���
				if (fOldPercent < fPercent)
				{
					//����Ă���̂ňړ��ʂ�����
					*pMove *= GO_UP_a_SLOPE_MAG;
				}

				//�����Ă��� == �ړ��L�[��������Ă���
				else if ((GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_D)) == true)
				{
					//�����Ă���̂ňړ��ʂ̈ꕔ�������Ĉʒu���X�V
					pPos->x += pMove->x * GO_DOWN_a_SLOPE_MAG;
					pPos->z += pMove->z * GO_DOWN_a_SLOPE_MAG;
					pPos->y = FixHeight;						//Y���W�͊������狁�߂��ʒu�ɑ��
				}
			}

			//�Ώۂ̈ʒu���⓹�̍����ȉ�
			if (pPos->y <= FixHeight)
			{
				//�ʒu���C������
				pPos->y = FixHeight;

				//Y���̈ړ��ʂ��[���ɂ���
				pMove->y = F_ZERO;

				//������ς���{��
				float fAngle = 0.3f;

				//��������
				switch (g_Slope[nCntSlope].sloperot)
				{
				case SLOPEROT_FRONT_BACK:	//�O��ɐL�т��
				{
					//�p�x�I�ɉ����Ă���
					if (MinDownRot <= pRot->y && pRot->y <= MaxDownRot)
					{
						//������ς���
						pRot->x += (-(fSlopeRot - fabsf(pRot->y) * 0.25f) - pRot->x) * fAngle;
					}

					//����Ă���
					else
					{
						//Y�̊p�x�̊����Ō�����ς���
						pRot->x += ((fSlopeRot * (fabsf(pRot->y) / D3DX_PI)) - pRot->x) * fAngle;
					}

					//X�̕������v�Z
					pRot->x = (fabsf(pRot->y) / FixStandardRot) - fSlopeRot;

					//Y�̊p�x���}�C�i�X�����̏ꍇ
					if (pRot->y <= ZERO)
					{
						pRot->z = fSlopeRot - fabsf(pRot->x);
					}

					//Y�̊p�x���v���X�����̏ꍇ
					else
					{
						pRot->z = -fSlopeRot + fabsf(pRot->x);
					}
				}
				break;

				case SLOPEROT_RIGHT_LEFT:	//���E�ɍL�����
				{
					D3DXVECTOR3 Rot = *pRot;	//�p�x���i�[

					//X�̕������v�Z
					//pRot->z = (fabsf(Rot.y) / FixStandardRot) + fSlopeRot;

					/*----------------------------------------------------------------------------------
												�p�x�����ꂢ�ȋ��ڂƓ���
					------------------------------------------------------------------------------------*/
					//�p�x�̐�Βl��3.14
					if (fabsf(Rot.y) == UP)
					{
						pRot->x = NIL_F;
						pRot->z = fSlopeRot;
					}

					//�p�x�̐�Βl��1.57
					else if (fabsf(Rot.y) == RIGHT)
					{
						pRot->x = fSlopeRot * (RIGHT / Rot.y);
						pRot->z = NIL_F;
					}

					//�p�x��0.0
					else if (Rot.y == NIL_F)
					{
						pRot->x = NIL_F;
						pRot->z = -fSlopeRot;
					}

					/*----------------------------------------------------------------------------------
												�p�x�����ꂢ�ȋ��ڂƂ͈Ⴄ
					------------------------------------------------------------------------------------*/
					else
					{
						//�p�x�����_�������傫��
						if (Rot.y < NIL_F)
						{
							pRot->z = sinf(fabsf(RIGHT - fabsf(Rot.y)) / D3DX_PI);
						}
						else
						{
							pRot->z = sinf(fabsf(RIGHT - Rot.y) / -D3DX_PI);
						}

						//��Βl��1.57f�ȏ�
						if (fabsf(Rot.y) > RIGHT)
						{
							//pRot->z = (RIGHT - fabsf(Rot.y)) + fSlopeRot;
						}
						else
						{
							//pRot->x = (RIGHT - fabsf(pRot->y)) + fSlopeRot;
						}
					}
				}
				break;
				}

				//�����Ԃ�
				return bLand = true;
			}

			//�C���ʒu����ɋ�����p�x�����X�ɖ߂�
			else
			{
				pRot->x += (F_ZERO - pRot->x) * F_ONE;
				pRot->z += (F_ZERO - pRot->z) * F_ONE;
			}
		}

		case COLLITYPE_ENEMY:		/*�G�̏ꍇ*/
		{
			//�O��ƍ���̂ӂ��Ƃ���̋����̊������Ⴄ �i�ړ����Ă���j
			//����Ă���
			if (fOldPercent < fPercent)
			{
				//����Ă���̂ňړ��ʂ�����
				*pMove *= GO_UP_a_SLOPE_MAG;
			}

			//�����Ă���
			else
			{
				//�����Ă���̂ňړ��ʂ̈ꕔ�������Ĉʒu���X�V�i�ʏ�̈ʒu�X�V�ɉ����Ĉʒu���X�V����̂ŁA�����ړ�����
				pPos->x += pMove->x * GO_DOWN_a_SLOPE_MAG;
				pPos->z += pMove->z * GO_DOWN_a_SLOPE_MAG;
				pPos->y = FixHeight;						//Y���W�͊������狁�߂��ʒu�ɑ��
			}
		}
			break;
		}

		//���[�v���𔲂���
		break;
	}

	//�v���C���[�����̊֐����Ăяo���āA�⓹�̓����蔻�肪�ʂ�Ȃ������ꍇ
	if (nType == COLLITYPE_PLAYER)
	{
		//�p�x�����X�ɖ߂�
		pRot->x += (F_ZERO - pRot->x) * F_ONE;
		pRot->z += (F_ZERO - pRot->z) * F_ONE;
	}

	//�����Ԃ�
	return bLand;
}

//�⓹�ɉe��`�悷��
void ColliSlopeShadow(D3DXVECTOR3 pos, D3DXVECTOR3 *pRot, float *pDrawPosY, float *pHeight)
{
	for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++)
	{
		float FixHeight = F_ZERO;	//�C������ʒu
		float fPercent = F_ZERO;	//����
		float fHeight = F_ZERO;		//�����ɍ�����t����
		float fRot = F_ZERO;		//�C���������

		D3DXVECTOR3 BasePos = g_Slope[nCntSlope].pos;			//�ӂ��Ƃ̍��W

		//�⓹�̕����ɂ�鏈������
		switch (g_Slope[nCntSlope].sloperot)
		{
		case SLOPEROT_FRONT_BACK:	//�O�����
			if (g_Slope[nCntSlope].pos.x - g_Slope[nCntSlope].fWidth <= pos.x &&	//���[���E�ɂ���
				g_Slope[nCntSlope].pos.x + g_Slope[nCntSlope].fWidth >= pos.x &&	//�E�[��荶�ɂ���
				g_Slope[nCntSlope].pos.z + g_Slope[nCntSlope].fDepth >= pos.z &&	//��O��艜�ɂ���
				g_Slope[nCntSlope].pos.z <= pos.z)
			{
				fRot = F_ZERO;		//������ݒ�

				//����̈ʒu�����Z�o
				fPercent = fabsf(pos.z - BasePos.z) / g_Slope[nCntSlope].fDepth;

				//�e�̌X���̍����́A�����Ɖ��s���̊����@�~10 �Ŋ���o��
				fHeight = (g_Slope[nCntSlope].fHeight / g_Slope[nCntSlope].fDepth) * 10.0f;
			}

			//���Ȃ���΃��[�v������񕪔�΂�
			else
			{
				continue;
			}

			break;

		case SLOPEROT_RIGHT_LEFT:	//���E����
			if (g_Slope[nCntSlope].pos.x <= pos.x &&	//���[���E�ɂ���
				g_Slope[nCntSlope].pos.x + g_Slope[nCntSlope].fWidth >= pos.x &&	//�E�[��荶�ɂ���
				g_Slope[nCntSlope].pos.z + g_Slope[nCntSlope].fDepth >= pos.z &&	//��O��艜�ɂ���
				g_Slope[nCntSlope].pos.z - g_Slope[nCntSlope].fDepth <= pos.z)		//������O�ɂ���
			{
				fRot = RIGHT;		//������ݒ�
				
				//����̈ʒu�����Z�o
				fPercent = fabsf(pos.x - BasePos.x) / g_Slope[nCntSlope].fWidth;

				//�e�̌X���̍����́A�����ƕ��̊����@�~10 �Ŋ���o��
				fHeight = (g_Slope[nCntSlope].fHeight / g_Slope[nCntSlope].fWidth) * 10.0f;
			}

			//���Ȃ���΃��[�v������񕪔�΂�
			else
			{
				continue;
			}

			break;
		}

		//��������ɍ������Z�o
		FixHeight = BasePos.y + (g_Slope[nCntSlope].fHeight * fPercent);

		//�C������ʒu���e�̈ʒu���Ⴂ�@���@�������s��
		if (*pDrawPosY <= FixHeight)
		{
			pRot->y = fRot;			//�p�x��t����
			*pHeight = fHeight;		//������t���ČX����
			*pDrawPosY = FixHeight;	//���̂܂܉e�̈ʒu�ɕ`�悷��
		}

		//�������I����
		break;
	}
}

//�⓹�̏��
Slope *GetSlope(void)
{
	//�⓹�̏��̐擪�A�h���X��Ԃ�
	return &g_Slope[0];
}

//�⓹�̏��ǂݍ��ݏ���
void LoadSlope(void)
{
	FILE *pFile;
	char aData[MAX_MOJI];		//�ǂݍ��񂾕������i�[

	pFile = fopen(SLOPE_FILE, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &aData[ZERO]);//�ŏ��̃Z���ɏ��������ږ��ǂݍ���

		for (int nCntSlope = START_CNT; nCntSlope < NUM_SLOPE; nCntSlope++)
		{
			fscanf(pFile, "%s", &aData[ZERO]);	//��s�ǂݍ���

			//�z�u�ꏊ�̏�����
			g_Slope[nCntSlope].pos.x = atof(strtok(&aData[ZERO], ","));			//�ʒu������
			g_Slope[nCntSlope].pos.y = atof(strtok(NULL, ","));					//�ʒu������
			g_Slope[nCntSlope].pos.z = atof(strtok(NULL, ","));					//�ʒu������

			//�z�u�����̏�����
			g_Slope[nCntSlope].rot.x = atof(strtok(NULL, ",")) / FIX_FLOAT;		//����������
			g_Slope[nCntSlope].rot.y = atof(strtok(NULL, ",")) / FIX_FLOAT;		//����������
			g_Slope[nCntSlope].rot.z = atof(strtok(NULL, ",")) / FIX_FLOAT;		//����������

			g_Slope[nCntSlope].fWidth = atof(strtok(NULL, ","));				//���̏�����
			g_Slope[nCntSlope].fHeight = atof(strtok(NULL, ","));				//�����̏�����
			g_Slope[nCntSlope].fDepth = atof(strtok(NULL, ","));				//���s���̏�����

			g_Slope[nCntSlope].sloperot = (SLOPEROT)(atoi(strtok(NULL, ",")));	//�⓹�̕���������
		}
	
		//�t�@�C�������
		fclose(pFile);
	}
}