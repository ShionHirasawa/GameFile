//================================================================================================
//
//�J��������[camera.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//�O���錾
class CPlayer;

//�J�����N���X
class CCamera
{
public:
	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void Set(void);//�J�����̊p�x�Ȃǂ�ݒ�	
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }	//�v���C���[�����擾

	D3DXVECTOR3 GetPosV(void) { return m_posV;  }	//���_�̈ʒu���擾
	 D3DXVECTOR3 GetRotate(void) { return m_rot; }	//�J�����̊p�x���擾����
	 float GetInverseRotate(void);					//�J�����̋t�p�x���擾����
	 void CalculateRotate(void);					//�J�����̊p�x���v�Z����

private:
	static const float MOUSE_ROT_SPEED;
	static const float PAD_ROT_SPEED;

	void Move(void);
	void MovePosV(CInputMouse *pMouse);
	void SpinPosV(CInputMouse *pMouse);
	void MovePosR(CInputMouse *pMouse);
	void ResetPos(void);
	void Length(void);

	CPlayer *m_pPlayer;	//�v���C���[�����i�[

	D3DXMATRIX	m_mtxProjection;//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxview;		//�r���[�}�g���b�N�X
	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posR;			//�����_
	D3DXVECTOR3 m_posROffset;	//�����_�̃I�t�Z�b�g
	D3DXVECTOR3 m_vecU;			//������x�N�g��
	D3DXVECTOR3 m_rot;			//����
	float		m_fLength;		//���_�ƒ����_�̋���
};

#endif 