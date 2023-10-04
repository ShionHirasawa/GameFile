//================================================================================================
//
//X�t�@�C�����g�p�������f���Ǘ�����[objectX.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "../S_H_Lib/S_H_Calculation.h"
#include "objectX.h"
#include "../3D/Polygon3D.h"
#include "../3D/PolygonBill.h"
#include "../3D/shadow.h"

//*********************
//�}�N����`
//*********************
//������
#define ATTENUATE	(0.1f)

//�ϐg���̓����蔻��͈̔�
#define TRANSFORM_COLLISION_RANGE	(100.0f)

//�ÓI�����o�ϐ�
CObjectX::XModel	*CObjectX::m_apXModel[MAX_X_MODEL] = {};
char				*CObjectX::m_pFilePath[MAX_X_MODEL] = {};
int					CObjectX::m_nNumAll = 0;

//3D�^�[�Q�b�g�̐ÓI���
char		*CObjectX::TARGET_3D_TEX_PATH = "data\\TEXTURE\\Target_Circle.png";
const float  CObjectX::TARGET_3D_SIZE = 40.0f;	//���E���s��
CPoly3D		*CObjectX::m_pTarget3D = NULL;

//�r���{�[�h�^�[�Q�b�g�̐ÓI���
char		*CObjectX::TARGET_BILL_TEX_PATH = "data\\TEXTURE\\Target_Triangle.png";
const float  CObjectX::TARGET_BILL_SIZE = 30.0f;	//���E����
const float  CObjectX::TARGET_BILL_FLOAT = 50.0f;	//�\������ʒu���^�[�Q�b�g�̍ő�x���W���畂�������
CPolyBill	*CObjectX::m_pTargetBill = NULL;

const float CObjectX::SHADOW_SIZE_CORRECT = 1.8f;//�e�|���S���̕␳�l

//=======================================
//�R���X�g���N�^
//=======================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	//�����o�ϐ��N���A
	m_nShadowID = ERROR_ID;/*�e�ԍ�*/
	m_pos = VEC3_INIT;	// �ʒu
	m_move = VEC3_INIT;	// �ړ���
	m_rot = VEC3_INIT;	// ����
	m_nXmodelID = 0;	// �����̔ԍ�
	m_bDisp = true;		// �`�悷��

	m_pTarget3D = NULL;		//�ϐg�ł���I�u�W�F�N�g�̑����ɕ\��
	m_pTargetBill = NULL;	//�ϐg�ł���I�u�W�F�N�g�̓���ɕ\��
}

//=======================================
//�f�X�g���N�^
//=======================================
CObjectX::~CObjectX()
{

}

//=======================================
//����������
//=======================================
HRESULT CObjectX::Init(char *pXFilePath, XMODELTYPE type, int nLife)
{
	//�����̇]���f���ԍ����擾����
	m_nXmodelID = Regist(pXFilePath, type, nLife);

	//��ނ�ݒ肷��
	CObject::SetType(CObject::TYPE_XMODEL);

	//�^�[�Q�b�g�|���S���̏�����
	InitTargetPolygon();

	//�e�̈ʒu���v�Z
	D3DXVECTOR3 pos = GetPosition();
	pos.y = 0.0f;

	//�e����
	m_nShadowID = CShadow::Create(pos, GetXModelMargin(m_nXmodelID) * SHADOW_SIZE_CORRECT);
	CShadow::DispSwitch(m_nShadowID, false);

	//����������
	return S_OK;
}

//=======================================
//�^�[�Q�b�g�|���S���̏�����
//=======================================
void CObjectX::InitTargetPolygon(void)
{
	//3D�^�[�Q�b�g�̐���
	if (m_pTarget3D == NULL)	{
		m_pTarget3D = CPoly3D::Create(TARGET_3D_TEX_PATH);
		m_pTarget3D->SetInfo(VEC3_INIT, TARGET_3D_SIZE, 0.0f, TARGET_3D_SIZE);
		m_pTarget3D->DispSwitch(false);
	}

	//�r���{�[�h�^�[�Q�b�g�̐���
	if (m_pTargetBill == NULL)	{
		m_pTargetBill = CPolyBill ::Create(TARGET_BILL_TEX_PATH);
		m_pTargetBill->SetInfo(VEC3_INIT, TARGET_BILL_SIZE, TARGET_BILL_SIZE, 0.0f);
		m_pTargetBill->DispSwitch(false);
		m_pTargetBill->ZTestSwitch(true);
	}
}

//=======================================
//�I������
//=======================================
void CObjectX::Uninit(void)
{
	//�����̉e��j��
	CShadow::Delete(m_nShadowID);

	for (int nCntXModel = 0; nCntXModel < MAX_X_MODEL; nCntXModel++)
	{
		//�]���f�����g���Ă���
		if (m_apXModel[nCntXModel] != NULL)
		{
			//���b�V���̔j��
			if (m_apXModel[nCntXModel]->pMesh != NULL)
			{
				m_apXModel[nCntXModel]->pMesh->Release();
				m_apXModel[nCntXModel]->pMesh = NULL;
			}

			//�}�e���A���̔j��
			if (m_apXModel[nCntXModel]->pBuffMat != NULL)
			{
				m_apXModel[nCntXModel]->pBuffMat->Release();
				m_apXModel[nCntXModel]->pBuffMat = NULL;
			}

			//�e�N�X�`��ID���g���Ă���
			if (m_apXModel[nCntXModel]->pTextureID != NULL)
			{
				delete[] m_apXModel[nCntXModel]->pTextureID;
				m_apXModel[nCntXModel]->pTextureID = NULL;
			}

			//�]���f���̃��������J������
			delete m_apXModel[nCntXModel];
			m_apXModel[nCntXModel] = NULL;
		}

		//���������g���Ă�����A�j������
		if (m_pFilePath[nCntXModel] != NULL)
		{
			delete m_pFilePath[nCntXModel];
			m_pFilePath[nCntXModel] = NULL;
		}
	}

	//�������g��j��
	this->Release();

	m_nNumAll = 0;
}

//=======================================
//�X�V����
//=======================================
void CObjectX::Update(void)
{
	//�����̉e�ʒu�ݒ�
	CShadow::DispSwitch(m_nShadowID, m_bDisp);
	CShadow::SetPos(m_nShadowID, GetPosition());
}

//=======================================
//�`�揈��
//=======================================
void CObjectX::Draw(D3DXCOLOR *pDiffuse)
{
	//�`�悵�Ȃ�
	if (!m_bDisp) return;

	//�����̇]���f�����擾
	CObjectX::XModel *pXModel = GetXModel(m_nXmodelID);

	//�]���f�����擾�ł��Ȃ�������
	if (pXModel == NULL)
	{
		//�`�悹���Ɉ����Ԃ�
		return;
	}

	/* �����_���[�̃|�C���^ */ CRenderer *pRender = CManager::GetRenderer();
	/* �f�o�C�X�ւ̃|�C���^ */ LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	/*  �v�Z�p�}�g���b�N�X  */ D3DXMATRIX mtxRot, mtxTrans;
	/*���݂̃}�e���A���ۑ��p*/ D3DMATERIAL9 matDef;
	/* �}�e���A���̃|�C���^ */ D3DXMATERIAL *pMat;

	//���e�X�g��L���ɂ���
	pRender->SetAlphaTest(true, 0);

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

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL *)pXModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pXModel->dwNumMat; nCntMat++)
	{
		//�ꎞ�I�Ƀ}�e���A����ۊ�
		const D3DMATERIAL9 MatTmp = pMat[nCntMat].MatD3D;

		//�F�ύX�w��A��
		if (pDiffuse != nullptr)
		{
			//�F�ς�
			pMat[nCntMat].MatD3D.Diffuse = *pDiffuse;
		}

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, CManager::GetTexture()->GetAddress(pXModel->pTextureID[nCntMat]));

		//���f��(�p�[�c)�̕`��
		pXModel->pMesh->DrawSubset(nCntMat);

		//�ۊǂ����}�e���A����߂�
		pMat[nCntMat].MatD3D = MatTmp;
	}

	//���e�X�g�𖳌��ɂ���
	pRender->SetAlphaTest(false, 255);

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=======================================
//�ړ��ʂ������i�����W���̓f�t�H���g�l
//=======================================
void CObjectX::MoveAttenuate(void)
{
	m_move += (VEC3_INIT - m_move) * ATTENUATE;
}

//=======================================
//�d�͂�p�����ړ���������
//=======================================
void CObjectX::MoveAttenuateGravity(const float fGravityPower, const float fGravityMag)
{
	/*�w�x�N�g������*/ m_move.x += (0.0f - m_move.x) * ATTENUATE;
	/*�y�x�N�g������*/ m_move.z += (0.0f - m_move.z) * ATTENUATE;
	/*�x�x�N�g���d�͂ɏ]�킹��*/ m_move.y -= (fGravityPower + m_move.y) * fGravityMag;
}

//=======================================
//�����̇]���f����Ԃ����A�V�K�o�^������
//=======================================
int CObjectX::Regist(char *pFileName, XMODELTYPE type, int nLife)
{
	// �]���f���ԍ�
	int nXModelIdx = 0;

	//���łɓo�^���ꂽ�]���f���Ɠ������̂��Ȃ����m�F
	for (nXModelIdx = 0; nXModelIdx < m_nNumAll; nXModelIdx++)
	{
		//�w�肳�ꂽ�t�@�C�����Ɗ��ɓo�^����Ă���t�@�C��������v����
		if (strcmp(pFileName, m_pFilePath[nXModelIdx]) == 0)
		{
			//���ɓo�^����Ă���]���f����Ԃ�
			return nXModelIdx;
		}
	}

	//==============================================
	//�V�K�]���f���o�^
	//==============================================
	/*�������ő吔����*/
	if (m_nNumAll < MAX_X_MODEL)
	{
		//�f�o�C�X�ւ̃|�C���^
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		//���������m�ۂ���Ă��Ȃ�
		if (m_apXModel[nXModelIdx] == NULL)
		{
			//�C���X�^���X�𐶐�����
			m_apXModel[nXModelIdx] = new XModel;
		}

		//�C���X�^���X����������Ă���
		if (m_apXModel[nXModelIdx] != NULL &&
			D3DXLoadMeshFromX(pFileName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_apXModel[nXModelIdx]->pBuffMat, NULL,
			&m_apXModel[nXModelIdx]->dwNumMat, &m_apXModel[nXModelIdx]->pMesh) == D3D_OK)
		{
			/*�]���f���̃|�C���^�擾*/	 XModel *pXModel = m_apXModel[nXModelIdx];

			/*�}�e���A�����̃|�C���^�m��*/	pXModel->pTextureID = new int[(int)pXModel->dwNumMat];
			/*�}�e���A���f�[�^�̃|�C���^*/	D3DXMATERIAL *pMat = (D3DXMATERIAL *)pXModel->pBuffMat->GetBufferPointer();

			//+++++++++++++++++++++++++++++++++++++++++++++++
			//�e�N�X�`���̐ݒ�
			//+++++++++++++++++++++++++++++++++++++++++++++++
			for (int nCntMat = 0; nCntMat < (int)pXModel->dwNumMat; nCntMat++)
			{
				//�G���[�ԍ��������l�Ƃ��đ��
				pXModel->pTextureID[nCntMat] = ERROR_ID;

				//�e�N�X�`���t�@�C�������݂���
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//�e�N�X�`���ԍ��i�[ <- �e�N�X�`���̓ǂݍ���
					pXModel->pTextureID[nCntMat] = CManager::GetTexture()->Regist(pMat[nCntMat].pTextureFilename);
				}
			}
			//-----------------------------------------------
			//�e�N�X�`���ݒ�I��
			//-----------------------------------------------

			//+++++++++++++++++++++++++++++++++++++++++++++++
			//�ŏ��E�ō����W�̐ݒ�
			//+++++++++++++++++++++++++++++++++++++++++++++++
			{
				/*���_�o�b�t�@�ւ̃|�C���^*/ BYTE *pVtxBuff;
				/*���b�V���̃|�C���^�擾*/	 LPD3DXMESH pMesh = pXModel->pMesh;

				/*���_�����擾����*/	const int	nNumVtx = pMesh->GetNumVertices();
				/*���_�̃T�C�Y���擾*/	const DWORD dwSizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

				//���_�o�b�t�@�����b�N
				pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void* *)&pVtxBuff);

				//�ŏ��̒��_���W���ŏ��E�ő���W�ɐݒ�
				pXModel->MinPos = pXModel->MaxPos = *(D3DXVECTOR3 *)pVtxBuff;

				pVtxBuff += dwSizeFVF;						//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
				D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;	//�Q�Ԗڂ̒��_���W���擾����

				for (int nCntVtx = 1; nCntVtx < nNumVtx; nCntVtx++)
				{
					//---------------------------------------
					//�ݒ肳��Ă�����W��菬������������
					//---------------------------------------
					pXModel->MinPos.x = vtx.x < pXModel->MinPos.x ? vtx.x : pXModel->MinPos.x;
					pXModel->MinPos.y = vtx.y < pXModel->MinPos.y ? vtx.y : pXModel->MinPos.y;
					pXModel->MinPos.z = vtx.z < pXModel->MinPos.z ? vtx.z : pXModel->MinPos.z;

					//---------------------------------------
					//�ݒ肳��Ă�����W���傫����������
					//---------------------------------------
					pXModel->MaxPos.x = vtx.x > pXModel->MaxPos.x ? vtx.x : pXModel->MaxPos.x;
					pXModel->MaxPos.y = vtx.y > pXModel->MaxPos.y ? vtx.y : pXModel->MaxPos.y;
					pXModel->MaxPos.z = vtx.z > pXModel->MaxPos.z ? vtx.z : pXModel->MaxPos.z;

					pVtxBuff += dwSizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y���A�|�C���^��i�߂�
					vtx = *(D3DXVECTOR3 *)pVtxBuff;	//���_���W�̑��
				}

				//���_�o�b�t�@���A�����b�N
				pXModel->pMesh->UnlockVertexBuffer();
			}
			//-----------------------------------------------
			//�ŏ��E�ō����W�ݒ�I��
			//-----------------------------------------------

			//+++++++++++++++++++++++++++++++++++++++++++++++
			//�����蔻��͈̔͐ݒ�i�����͍l�����Ȃ�
			//+++++++++++++++++++++++++++++++++++++++++++++++
			{
				//�w�E�y�̍ő�͈͂��i�[
				D3DXVECTOR3 MaxMargin = VEC3_INIT;
				const D3DXVECTOR3 MaxPos = pXModel->MaxPos;
				const D3DXVECTOR3 MinPos = pXModel->MinPos;

				//���_�����ԉ����ق��̍��W���i�[����
				MaxMargin.x = fabsf(MinPos.x) <= fabsf(MaxPos.x) ? MaxPos.x : MinPos.x;
				MaxMargin.z = fabsf(MinPos.z) <= fabsf(MaxPos.z) ? MaxPos.z : MinPos.z;

				//�����蔻��͈̔͂�ݒ肷��
				pXModel->fMargin = D3DXVec3Length(&MaxMargin);
			}
			//-----------------------------------------------
			//�����蔻��͈̔͐ݒ�I��
			//-----------------------------------------------

			//+++++++++++++++++++++++++++++++++++++++++++++++
			//�t�@�C���o�X��ݒ�
			//+++++++++++++++++++++++++++++++++++++++++++++++
			{
				//���������������g���Ă�����A�j������
				if (m_pFilePath[nXModelIdx] != NULL)
				{
					delete m_pFilePath[nXModelIdx];
					m_pFilePath[nXModelIdx] = NULL;
				}

				/*�p�X�̒����{�I�[�������̃������m��*/	m_pFilePath[nXModelIdx] = new char[(int)strlen(pFileName) + 1];
				/*�]���f���̃t�@�C���p�X��ۑ�*/		strcpy(m_pFilePath[nXModelIdx], pFileName);
			}
			//-----------------------------------------------
			//�t�@�C���o�X�ݒ�I��
			//-----------------------------------------------

			/*���̐ݒ�*/	pXModel->Type = type;
			/*�̗͂̐ݒ�*/	pXModel->nLife = nLife;

			/*�]���f���̑����𑝂₷*/	m_nNumAll++;
			/*�]���f���̔ԍ���Ԃ�*/	return nXModelIdx;
		}
	}

	//�V�K�o�^���s�ŃG���[�ԍ���Ԃ�
	return ERROR_ID;
}

//=======================================
//�w��̔ԍ��̇]���f����Ԃ�
//=======================================
CObjectX::XModel *CObjectX::GetXModel(int nXModelID)
{
	//�z��͈̔͊O���A������葽���ԍ����w�肳�ꂽ
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll)
	{
		//����Ȃ��̂͂Ȃ��̂ŁANULL��Ԃ�
		return NULL;
	}

	//�������w�肳�ꂽ�]���f����Ԃ�
	return m_apXModel[nXModelID];
}

//=======================================
//�����蔻��͈͎̔擾�i�����͍l�����ĂȂ�
//=======================================
float CObjectX::GetXModelMargin(int nXModelID)
{
	//�z��͈̔͊O���w�肳�ꂽ�炻��Ȃ��̂͂Ȃ��̂ŁA������Ԃ�
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll) return 0.0f;

	//�����蔻��͈̔͂�Ԃ�
	return m_apXModel[nXModelID]->fMargin;
}

//=======================================
//�]���f���̌����擾
//=======================================
CObjectX::XMODELTYPE CObjectX::GetXModelType(int nXModelID)
{
	//�z��͈̔͊O���w�肳�ꂽ�炻��Ȃ��̂͂Ȃ��̂ŁA������Ԃ�
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll) return XMODELTYPE_NONE;

	//����Ԃ�
	return m_apXModel[nXModelID]->Type;
}

//=======================================
//�]���f���̗̑͂��擾
//=======================================
int CObjectX::GetXModelLife(int nXModelID)
{
	//�z��͈̔͊O���w�肳�ꂽ�炻��Ȃ��̂͂Ȃ��̂ŁA�O��Ԃ�
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll) return 0;

	//�̗͂�Ԃ�
	return m_apXModel[nXModelID]->nLife;
}

//=======================================
//�w�x�y�̂ǂꂩ�̍ŏ��E�ō����W��Ԃ�
//=======================================
float CObjectX::GetPosition(int nXModelID, TYPE posType, TYPE typeH_L)
{
	//�z��͈̔͂��w�肳�ꂽ�炻��Ȃ��̂͂Ȃ��̂ŁA0.0f��Ԃ�
	if (nXModelID <= ERROR_ID || nXModelID >= m_nNumAll) return 0.0f;

	//�ő���W��Ԃ��ꍇ
	if (typeH_L == TYPE_MAX)
	{
		//���W�̎�ޕʂŒl��Ԃ�
		switch (posType)
		{
		case TYPE_X:return m_apXModel[nXModelID]->MaxPos.x;	break;	//�w���W��Ԃ�
		case TYPE_Y:return m_apXModel[nXModelID]->MaxPos.y;	break;	//�x���W��Ԃ�
		case TYPE_Z:return m_apXModel[nXModelID]->MaxPos.z;	break;	//�y���W��Ԃ�
		default:	return 0.0f; break;
		}
	}
	//�ŏ����W��Ԃ��ꍇ
	else if (typeH_L == TYPE_MIN)
	{
		//���W�̎�ޕʂŒl��Ԃ�
		switch (posType)
		{
			case TYPE_X:return m_apXModel[nXModelID]->MinPos.x;	break;	//�w���W��Ԃ�
			case TYPE_Y:return m_apXModel[nXModelID]->MinPos.y;	break;	//�x���W��Ԃ�
			case TYPE_Z:return m_apXModel[nXModelID]->MinPos.z;	break;	//�y���W��Ԃ�
			default: return 0.0f; break;
		}
	}

	//�����ɕs�K�؂Ȓl���������ꍇ
	else return 0.0f;
}

//=======================================
//�R���_�̍ŏ��E�ō����W��Ԃ�
//=======================================
D3DXVECTOR3 CObjectX::GetPosition(int nXModelID, TYPE typeH_L)
{
	//���W��Ԃ�
	return D3DXVECTOR3(
		GetPosition(nXModelID, TYPE_X, typeH_L),	//�w���W���擾
		GetPosition(nXModelID, TYPE_Y, typeH_L),	//�x���W���擾
		GetPosition(nXModelID, TYPE_Z, typeH_L));	//�y���W���擾
}

//=======================================
//�R���_�̍ŏ��E�ō����W��Ԃ�
//=======================================
D3DXVECTOR3 CObjectX::GetPosition(int nXModelID, TYPE Xtype, TYPE Ytype, TYPE Ztype)
{
	//���W��Ԃ�
	return D3DXVECTOR3(
		GetPosition(nXModelID, TYPE_X, Xtype),	//�w���W���擾
		GetPosition(nXModelID, TYPE_Y, Ytype),	//�x���W���擾
		GetPosition(nXModelID, TYPE_Z, Ztype));	//�y���W���擾
}

//=======================================
//�]���f���Ƃ̓����蔻��i�l�ԏ�ԁj
//=======================================
void CObjectX::CollisionXModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, TYPE nType, float fRange, float fMarginSize)
{
	//�����ɕK�v�ȏ�񂪂�����Ă��Ȃ�����������Ԃ�
	if (pPos == NULL || pMove == NULL || nType != TYPE_X && nType != TYPE_Y && nType != TYPE_Z) return;

	//�I�u�W�F�N�g�̑���
	const int NUM_OBJECT = CObject::GetNumAll(CObject::OBJECT_PRIORITY_DEF);

	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		//�]���f�����擾
		CObjectX *pObjX = (CObjectX *)CObject::GetObject(CObject::OBJECT_PRIORITY_DEF, nCntModel);

		//�]���f�����ǂ�������
		if (pObjX == NULL || pObjX->GetType() != CObject::TYPE_XMODEL || !pObjX->m_bDisp) continue;

		/*�I�u�W�F�N�g�̌��݈ʒu*/ D3DXVECTOR3 ObjPos = pObjX->GetPosition();
		/*�I�u�W�F�N�g�܂ł̍���*/ D3DXVECTOR3 posDiff = *pPos - ObjPos;
		/*�I�u�W�F�N�g�܂ł̋���*/ float fXModelLength = D3DXVec3Length(&posDiff);

		//�I�u�W�F�N�g�̋����������蔻��͈͓̔�
		if (fXModelLength > fRange) continue;

		//�ŏ��ʒu�A�ő�ʒu���擾
		D3DXVECTOR3 ObjMinPos = ObjPos + pObjX->GetPosition(pObjX->m_nXmodelID, TYPE_MIN);
		D3DXVECTOR3 ObjMaxPos = ObjPos + pObjX->GetPosition(pObjX->m_nXmodelID, TYPE_MAX);

		//XZ�݂͈̂̔͂ŁA���ݒn�����ɓ����Ă��Ȃ�
		if (!IsBoxCollision(
			D3DXVECTOR3(pPos->x - fMarginSize, pPos->y - fMarginSize, pPos->z - fMarginSize),	//�����̍ŏ��ʒu
			D3DXVECTOR3(pPos->x + fMarginSize, pPos->y + fMarginSize, pPos->z + fMarginSize),	//�����̍ő�ʒu
			ObjMinPos, ObjMaxPos))
			continue;

		//X���W�̏C��
		if (nType == TYPE_X)
		{
			//�O��̈ʒu���A�I�u�W�F�N�g�̍ŏ��ʒu���}�C�i�X�̕��ɂ���
			if (PosOld.x <= ObjMinPos.x)		pPos->x = ObjMinPos.x - fMarginSize;

			//�O��̈ʒu���A�I�u�W�F�N�g�̍ő�ʒu���v���X�̕��ɂ���
			else if(PosOld.x >= ObjMaxPos.x)	pPos->x = ObjMaxPos.x + fMarginSize;

			//�ړ��ʂ�����
			pMove->x = 0.0f;
		}

		//Z���W�̏C��
		else if (nType == TYPE_Z)
		{
			//�O��̓I�u�W�F�̍ŏ��ʒu�ɂ߂荞��łȂ�����
			if (PosOld.z <= ObjMinPos.z)		pPos->z = ObjMinPos.z - fMarginSize;

			//�O��̓I�u�W�F�̍ő�ʒu�ɂ߂荞��łȂ�����
			else if (PosOld.z >= ObjMaxPos.z)	pPos->z = ObjMaxPos.z + fMarginSize;

			//�ړ��ʂ�����
			pMove->z = 0.0f;
		}
		break;
	}
}

//=======================================
//�]���f���Ƃ̓����蔻��i�ϐg��ԁj
//=======================================
void CObjectX::CollisionXModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 PosOld, D3DXVECTOR3 *pMove, TYPE nType, float fMargin, int nXModelID)
{
	//�����ɕK�v�ȏ�񂪂�����Ă��Ȃ�����������Ԃ�
	if (pPos == NULL || pMove == NULL || nType != TYPE_X && nType != TYPE_Y && nType != TYPE_Z) return;

	//�I�u�W�F�N�g�̑���
	const int NUM_OBJECT = CObject::GetNumAll(CObject::OBJECT_PRIORITY_DEF);

	//�ϐg�����I�u�W�F�N�g�̍ŏ��E�ő�ʒu�擾
	D3DXVECTOR3 MinPos = GetPosition(nXModelID, TYPE_MIN);
	D3DXVECTOR3 MaxPos = GetPosition(nXModelID, TYPE_MAX);
	D3DXVECTOR3 SelfMinPos	= *pPos + MinPos;
	D3DXVECTOR3 SelfMaxPos	= *pPos + MaxPos;
	D3DXVECTOR3 SelfMinPosOld = PosOld + MinPos;
	D3DXVECTOR3 SelfMaxPosOld = PosOld + MaxPos;

	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		//�]���f�����擾
		CObjectX *pObjX = (CObjectX *)CObject::GetObject(CObject::OBJECT_PRIORITY_DEF, nCntModel);

		//�]���f�����ǂ�������
		if (pObjX != NULL && pObjX->GetType() == CObject::TYPE_XMODEL && pObjX->m_bDisp)
		{
			/*�I�u�W�F�N�g�̌��݈ʒu*/ D3DXVECTOR3 ObjPos = pObjX->GetPosition();
			/*�I�u�W�F�N�g�܂ł̍���*/ D3DXVECTOR3 posDiff = *pPos - ObjPos;
			/*�I�u�W�F�N�g�܂ł̋���*/ float fXModelLength = D3DXVec3Length(&posDiff);

			//�ŏ��ʒu�A�ő�ʒu���擾
			const D3DXVECTOR3 ObjMinPos = ObjPos + pObjX->GetPosition(pObjX->m_nXmodelID, TYPE_MIN);
			const D3DXVECTOR3 ObjMaxPos = ObjPos + pObjX->GetPosition(pObjX->m_nXmodelID, TYPE_MAX);

			//�I�u�W�F�N�g�̋����������蔻��͈͓̔�
			if (fXModelLength <= fMargin + pObjX->GetXModelMargin(pObjX->m_nXmodelID))
			{
				//�ȍ~�̃R�����g�̐���
				if (!IsBoxCollision(SelfMinPos, SelfMaxPos, ObjMinPos, ObjMaxPos)) continue;

				//++++++++++++++++++++++++++++++++++++++
				//�y���W�̏C��
				//++++++++++++++++++++++++++++++++++++++
				if (nType == TYPE_Z)
				{
					//�O��̓I�u�W�F�̍ŏ��ʒu�ɂ߂荞��łȂ�����
					if (SelfMaxPosOld.z <= ObjMinPos.z)		 pPos->z = ObjMinPos.z - MaxPos.z;

					//�O��̓I�u�W�F�̍ŏ��ʒu�ɂ߂荞��łȂ�����
					else if (SelfMinPosOld.z >= ObjMaxPos.z) pPos->z = ObjMaxPos.z - MinPos.z;
				}

				//++++++++++++++++++++++++++++++++++++++
				//�w���W�̏C��
				//++++++++++++++++++++++++++++++++++++++
				else if (nType == TYPE_X)
				{
					//�O��̓I�u�W�F�̍ŏ��ʒu�ɂ߂荞��łȂ�����
					if (SelfMaxPosOld.x <= ObjMinPos.x)		 pPos->x = ObjMinPos.x - MaxPos.x;

					//�O��̓I�u�W�F�̍ŏ��ʒu�ɂ߂荞��łȂ�����
					else if (SelfMinPosOld.x >= ObjMaxPos.x) pPos->x = ObjMaxPos.x - MinPos.x;
				}
			}
		}
	}
}

//=======================================
//���[�ł���I�u�W�F�N�g���擾
//=======================================
CObjectX *CObjectX::TakeXModel(D3DXVECTOR3 pos, float rot, float fRangeAngle, float fLength)
{
	//�I�u�W�F�N�g�̑���
	const int NUM_OBJECT = CObject::GetNumAll(CObject::OBJECT_PRIORITY_DEF);

	//���E�̊p�x���v�Z����
	float fLeftAngle = rot + fRangeAngle; FixRotate(&fLeftAngle);
	float fRightAngle = rot - fRangeAngle; FixRotate(&fRightAngle);

	//�ϐg����I�u�W�F�N�g�܂ł̋�������ԏ��������̂��i�[
	float fMinLength = fLength;

	//���[����]�I�u�W�F�N�g�����[
	CObjectX *pTakeObjX = NULL;

	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		//�]���f�����擾
		CObjectX *pObjX = (CObjectX *)CObject::GetObject(CObject::OBJECT_PRIORITY_DEF, nCntModel);

		//�]���f�����ǂ�������
		if (pObjX == NULL || pObjX->GetType() != CObject::TYPE_XMODEL || !pObjX->m_bDisp) continue;

		//�]���f���̍��W
		const D3DXVECTOR3 XModelPos = pObjX->GetPosition();
		const float VecXModelLength = D3DXVec3Length(&(XModelPos - pos));

		//�͈͊O�ɂ���
		if (fMinLength < VecXModelLength || !Search(pos, XModelPos, fLeftAngle, fRightAngle, fLength)) continue;

		//�͈͓��ɋ���
		fMinLength = VecXModelLength;	//�ϐg�\�I�u�W�F�N�g�̍ŒZ�������X�V
		pTakeObjX = pObjX;				//�]�I�u�W�F�N�g�̃|�C���^��ۑ�
	}

	//�]�I�u�W�F�N�g��Ԃ�
	return pTakeObjX;
}

//=======================================
//�ڂ̑O�ɕϐg�ł���I�u�W�F�N�g�����邩�ǂ�������
//=======================================
int CObjectX::TransformXModel(D3DXVECTOR3 pos, float rot, float fRangeAngle, float fLength, int nTransformModelID)
{
	//�I�u�W�F�N�g�̑���
	const int NUM_OBJECT = CObject::GetNumAll(CObject::OBJECT_PRIORITY_DEF);

	//���E�̊p�x���v�Z����
	float fLeftAngle  = rot + fRangeAngle; FixRotate(&fLeftAngle);
	float fRightAngle = rot - fRangeAngle; FixRotate(&fRightAngle);

	float fMinLength = fLength;	//�ϐg����I�u�W�F�N�g�܂ł̋�������ԏ��������̂��i�[
	int nTransformID = ERROR_ID;//�ϐg����I�u�W�F�N�g�̔ԍ����擾

	for (int nCntModel = 0; nCntModel < NUM_OBJECT; nCntModel++)
	{
		//�]���f�����擾
		CObjectX *pObjX = (CObjectX *)CObject::GetObject(CObject::OBJECT_PRIORITY_DEF, nCntModel);

		//�]���f�����ǂ�������
		if (pObjX == NULL || pObjX->GetType() != CObject::TYPE_XMODEL || !pObjX->m_bDisp) continue;

		//�]���f���̍��W
		const D3DXVECTOR3 XModelPos = pObjX->GetPosition();
		const float VecXModelLength = D3DXVec3Length(&(XModelPos - pos));

		//�͈͊O�ɂ���
		if(fMinLength < VecXModelLength ||	!Search(pos, XModelPos, fLeftAngle, fRightAngle, fLength)) continue;

		//�͈͓��ɋ���
		fMinLength = VecXModelLength;		//�ϐg�\�I�u�W�F�N�g�̍ŒZ�������X�V
		nTransformID = pObjX->m_nXmodelID;	//�I�u�W�F�N�g�̔ԍ����i�[����

		//�^�[�Q�b�g�|���S����\��
		SetTargetPolygon(true, XModelPos, GetPosition(nTransformID, TYPE_Y, (TYPE)TYPE_MAX));
	}

	//�l�� or ���̕ϐg���Ă���ԍ��Ɠ����Ȃ�A�^�[�Q�b�g�|���S����\�����Ȃ�
	if (nTransformID == ERROR_ID || nTransformID == nTransformModelID) SetTargetPolygon(false);

	//�ϐg�ԍ���Ԃ�
	return nTransformID;
}

//=======================================
//�^�[�Q�b�g�|���S���̐ݒ�
//---------------------------------------
//�����P�@bDisp		�F�\�����邩�ۂ�
//�����Q�@TargetPos �F�^�[�Q�b�g�̈ʒu
//�����R�@MaxPosY	�F�^�[�Q�b�g�̍ō��x���W
//=======================================
void CObjectX::SetTargetPolygon(bool bDisp, D3DXVECTOR3 TargetPos, float MaxPosY)
{
	//�\�����邩�ǂ����ݒ�
	m_pTarget3D->DispSwitch(bDisp);
	m_pTargetBill->DispSwitch(bDisp);

	//3D�^�[�Q�b�g�̈ʒu���ݒ�
	m_pTarget3D->SetInfo(TargetPos, TARGET_3D_SIZE, 0.0f, TARGET_3D_SIZE);

	//�r���{�[�h�^�[�Q�b�g�͔����ʒu�𕂂����Đݒ�
	TargetPos.y = MaxPosY + TARGET_BILL_FLOAT;
	m_pTargetBill->SetInfo(TargetPos, TARGET_BILL_SIZE, TARGET_BILL_SIZE, 0.0f);
}

//=======================================
//�`��X�C�b�`ON/OFF
//=======================================
void CObjectX::DispSwitch(bool bDisp)
{
	//�`��ON/OFF�i�e�������ݒ�
	m_bDisp = bDisp;
	CShadow::DispSwitch(m_nShadowID, bDisp);
	CShadow::SetPos(m_nShadowID, GetPosition());
}