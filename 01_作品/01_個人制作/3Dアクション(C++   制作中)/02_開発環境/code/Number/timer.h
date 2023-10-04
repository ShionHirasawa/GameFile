//================================================================================================
//
//�^�C�}�[����[timer.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

//�^�C�}�[�̌���
#define TIMER_DEJIT	(3)

//�^�C�}�[�N���X
class CTimer : public CNumber
{
public:
	CTimer();
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer *Create(void);
	/*	�^�C�}�[�ݒ�	*/void Set(int nTimer = 0);

private:
	static D3DXCOLOR s_TimerColor;				//�^�C�}�[�̐F
	static CNumber	*m_apNumber[TIMER_DEJIT];	// �����I�u�W�F�N�g�̃|�C���^
	int		m_nTimer;		// �^�C�}�[�i�c�莞��
	int		m_nCountFrame;	// �t���[���J�E���^�[
};

#endif