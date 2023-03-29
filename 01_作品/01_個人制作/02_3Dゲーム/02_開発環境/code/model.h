/*==========================================================================================================================================================

															���f������[model.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _MODEL_H_
#define _MODEL_H_

#include "game.h"

//���f���̎��
typedef enum
{
	MODELTYPE_WOODEN_BLOCK = ZERO,	// �ؔ�
	MODELTYPE_WOODEN_BLOCKs,		// �ؔ��Q
	MODELTYPE_HANACHAN,				// �n�i�����
	MODELTYPE_MAX
}MODELTYPE;

//���f���̏�ԍ\����
typedef enum
{
	MODELSTATE_NORMAL = ZERO,	// �ʏ�
	MODELSTATE_APPEAR,			// �o��
	MODELSTATE_DAMAGE,			// �_���[�W���
	MODELSTATE_NONE,			// �����Ă���(���Ȃ�)
	MODELSTATE_MAX
}MODELSTATE;

//���f���̍\����
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		PosOld;			// �O��̈ʒu
	D3DXVECTOR3		move;			// �ړ���
	D3DXVECTOR3		MinPos;			// �ŏ��̈ʒu
	D3DXVECTOR3		MaxPos;			// �ő�̈ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXCOLOR		col;			// �F
	int				nIdxShadow;		// �Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
	int				nLife;			// ����
	int				nPoint;			// �ۗL����|�C���g
	int				nStateCounter;	// ��Ԃ̃J�E���^
	MODELSTATE		state;			// ���
	MODELTYPE		type;			// ���
	bool			bDisp;			// �\�����邩�ǂ���
	bool			bUse;			// �g�p���Ă��邩�ǂ���
}Model;

//�����������f���̍\����
typedef struct
{
	int			nDeadModelCount;	// �������Ă��郂�f���̐�
	int			nCount;				// �Q�[���I���܂ł̃J�E���g
}VanishModel;

void InitModel(void);
void LoadModel(void);
void SearchModel_MinAndMax_Pos(void);
void SetModel_Max_Min_Pos(int nCntModel);
void UninitModel(void);
void UpdateModel(void);
void FallModel(int nCntModel);
void DrawModel(void);

void SwitchStateModel(int nCntModel);
void DeadCountModel(void);

void ColliModel_to_Model(int nCntTarget);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, COLLITYPE nType);
bool ColliModel_Max(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType);
bool ColliModel_Min(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, int nCntModel, COLLITYPE nType);
void ColliModelShadow(int nIdxShadow, D3DXVECTOR3 pos, float *pDrawPosY);
Model *GetModel(void);
VanishModel *GetEndModel(void);

#endif