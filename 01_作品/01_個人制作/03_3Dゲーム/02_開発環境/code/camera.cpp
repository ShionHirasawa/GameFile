/*==========================================================================================================================================================

														�J��������[camera.cpp]
															Author:���V����

============================================================================================================================================================*/
#include "camera.h"
#include "input.h"
#include "player.h"

#define POS_POSV			D3DXVECTOR3(-500.0f, 100.0f, -500.0f)	//���_�̏����ʒu
#define POS_POSR			D3DXVECTOR3(0.0f, 0.0f, 0.0f)		//�����_�̏����ʒu
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)		//������x�N�g���̏����l
#define VIEW_ANGLE			(45.0f)								//����p
#define MOVE_CAMERA_SPEED	(2.0f)								//�J�����E�����_���W�̈ړ���
#define MIN_DRAW_Z			(10.0f)								//�`�揈���̍Œቜ�s��
#define MAX_DRAW_Z			(2000.0f)							//�`�揈���̍ō����s��
#define MIN_LENGTH			(100.0f)							//���_�ƒ����_�̍Œ዗��
#define MAX_LENGTH			(300.0f)							//���_�ƒ����_�̍ő勗��
#define CHANGE_LENGHT		(0.5f)								//�����̈ړ���
#define SPIN_ROT			(0.05f)								//��]��
#define COR_ROT_POSV		(3.14f)								//�p�x�v�Z�ɂ�鎋�_����̒����l
#define COR_ROT_POSR		(0.0f)								//�p�x�v�Z�ɂ�钍���_����̒����l
#define DRAW_LENGTH			(500.0f)							//�I�u�W�F�N�g��`�悳���Ă�����͈�

//�O���[�o���ϐ�
Camera						g_Camera;							//�J�����̏��

//------------------------------------------------
//				�J�����̏���������
//------------------------------------------------
void InitCamera(void)
{
	ResetCameraPos();

	D3DXVECTOR3 fDiffPos = g_Camera.posR - g_Camera.posV;	//���_�ƒ����_�̈ʒu�̍�
	fDiffPos.x = powf(fDiffPos.x, 2.0f);					//���_���m�̈ʒu�̍����Q��
	fDiffPos.z = powf(fDiffPos.z, 2.0f);					//���_���m�̈ʒu�̍����Q��

	g_Camera.rot.y = -atan2f(fDiffPos.x, fDiffPos.z);		// Y�����̊p�x���v�Z

	float PosDiffX, PosDiffY, PosDiffZ;

	PosDiffX = powf(g_Camera.posR.x - g_Camera.posV.x, 2.0f);	//2��
	PosDiffY = powf(g_Camera.posR.y - g_Camera.posV.y, 2.0f);	//2��
	PosDiffZ = powf(g_Camera.posR.z - g_Camera.posV.z, 2.0f);	//2��

	g_Camera.fLength = sqrtf(PosDiffX + PosDiffY + PosDiffZ);	//�������Z�o

	MoveCamera(&g_Camera.posR, &g_Camera.posV, COR_ROT_POSV);
}

//------------------------------------------------
//				�J�����̏I������
//------------------------------------------------
void UninitCamera(void)
{

}

//------------------------------------------------
//				�J�����̍X�V����
//------------------------------------------------
void UpdateCamera(void)
{	
	Player *pPlayer = GetPlayer();	//�v���C���[�̏��擾

	g_Camera.posR = pPlayer->pos;						//�����_�̈ʒu���v���C���[�̈ʒu�ɐݒ�
	g_Camera.posV += pPlayer->pos - pPlayer->posOld;	//�v���C���[���ړ������������_�ɂ������悤�ɉ��Z
	
	MovePosV();		//���_�ړ�
	SpinPosV();		//���_��]

	LengthCamera();

	//�J�����̊p�x�C��
	FixCameraRot();

#if _DEBUG
	//�J�����̈ʒu������
	if (GetKeyboardTrigger(DIK_N) == true)
	{
		ResetCameraPos();
	}
#endif
}

//���_�ړ�
void MovePosV(void)
{
#if 0
	//���_�A�����_�Ƃ��ɍ��Ɉړ�
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Camera.posV.x += sinf(LEFT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(LEFT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
									 
		g_Camera.posV.z += sinf(DOWN - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(DOWN - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//���_�A�����_�Ƃ��ɉE�Ɉړ�
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Camera.posV.x += sinf(RIGHT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(RIGHT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(UP - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(UP - g_Camera.rot.y) * MOVE_CAMERA_SPEED;		
	}

	//���_�A�����_�Ƃ��ɉ��Ɉړ�
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_Camera.posV.x += sinf(UP + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(UP + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(RIGHT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(RIGHT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//���_�A�����_�Ƃ��Ɏ�O�Ɉړ�
	if (GetKeyboardPress(DIK_S) == true)
	{
		g_Camera.posV.x += sinf(DOWN + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(DOWN + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(LEFT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(LEFT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

#endif

	//���_�㏸
	if ((GetKeyboardPress(DIK_UP) || GetGamePadPress(XINPUT_GAMEPAD_Y)) == true)
	{
		g_Camera.posV.y -= cosf(UP) * MOVE_CAMERA_SPEED;
	}

	//���_���~
	if ((GetKeyboardPress(DIK_DOWN) || GetGamePadPress(XINPUT_GAMEPAD_X)) == true)
	{
		g_Camera.posV.y -= cosf(DOWN) * MOVE_CAMERA_SPEED;
	}
}

//���_��]
void SpinPosV(void)
{
	//������
	if (GetKeyboardPress(DIK_LEFT) == true || GetGamePadPress(XINPUT_GAMEPAD_LEFT_SHOULDER) == true)
	{
		g_Camera.rot.y -= SPIN_ROT;
	}

	//�E����
	if (GetKeyboardPress(DIK_RIGHT) == true || GetGamePadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER) == true)
	{
		g_Camera.rot.y += SPIN_ROT;
	}

	MoveCamera(&g_Camera.posR, &g_Camera.posV, COR_ROT_POSV);
}

//�p�x�C��
void FixCameraRot(void)
{
	//X�����̉E���C��(3.14�ȏ�)
	if (g_Camera.rot.x >= D3DX_PI)
	{
		g_Camera.rot.x -= FIX_ROT;
	}

	//X�����̍����C��(-3.14�ȉ�)
	if (g_Camera.rot.x <= -D3DX_PI)
	{
		g_Camera.rot.x += FIX_ROT;
	}

	//Y�����̉E���C��(3.14�ȏ�)
	if (g_Camera.rot.y >= D3DX_PI)
	{
		g_Camera.rot.y -= FIX_ROT;
	}

	//Y�����̍����C��(-3.14�ȉ�)
	if (g_Camera.rot.y <= -D3DX_PI)
	{
		g_Camera.rot.y += FIX_ROT;
	}

	//Z�����̉E���C��(3.14�ȏ�)
	if (g_Camera.rot.z >= D3DX_PI)
	{
		g_Camera.rot.z -= FIX_ROT;
	}

	//Z�����̍����C��(-3.14�ȉ�)
	if (g_Camera.rot.z <= -D3DX_PI)
	{
		g_Camera.rot.z += FIX_ROT;
	}
}

//�J�����̈ړ�����
void MoveCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = pParentPos->x + sinf((CorRot - g_Camera.rot.y)) * g_Camera.fLength;
	pChildPos->z = pParentPos->z + cosf((CorRot - g_Camera.rot.y)) * g_Camera.fLength;
}

//�J�����̈ʒu���Z�b�g
void ResetCameraPos(void)
{
	g_Camera.posR = GetPlayer()->pos;			//�����_�̈ʒu���v���C���[�̈ʒu�ɐݒ�
	g_Camera.posV = g_Camera.posR + POS_POSV;	//���_������
	g_Camera.rot = ZERO_SET;					//����������
	g_Camera.vecU = POS_VECU;					//������x�N�g��������
	LengthCamera();								//�J�����̋���������
}

//���_�Ԃ̒�������
void LengthCamera(void)
{
	//���������ꂷ����
	if (g_Camera.fLength > MAX_LENGTH)
	{
		g_Camera.fLength -= 1.5f;
	}

	//�������߂�����
	if (g_Camera.fLength < MIN_LENGTH)
	{
		g_Camera.fLength = MIN_LENGTH;
	}
}

//------------------------------------------------
//				�J�����̐ݒ菈��
//------------------------------------------------
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//�v���W�F�N�V�����}�g���b�N�X������
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, D3DXToRadian(VIEW_ANGLE), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_Z, MAX_DRAW_Z);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	//�r���[�}�g���b�N�X
	D3DXMatrixIdentity(&g_Camera.mtxview);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_Camera.mtxview,
						&g_Camera.posV,		//���_
						&g_Camera.posR,		//�����_
						&g_Camera.vecU);	//������x�N�g��

	//�r���[�}�g���b�N�X
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxview);
}

//�J�����̏��擾
Camera *GetCamera(void)
{
	return &g_Camera;
}