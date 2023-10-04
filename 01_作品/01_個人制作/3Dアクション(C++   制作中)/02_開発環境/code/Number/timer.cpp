//================================================================================================
//
//�^�C�}�[����[timer.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "number.h"
#include "timer.h"
#include "../2D/fade.h"

/***		�}�N����`		***/
/*�^�C�}�[�̍ő吔*/	#define MAX_TIMER	((int)pow(10, TIMER_DEJIT + 1) - 1)
/*�^�C�}�[�̊�]���W*/#define DEFAULT_POS_X	(660.0f)
/*�����̉���*/			#define NUMBER_WIDTH	(20.0f)
/*�����̍���*/			#define NUMBER_HEIGHT	(40.0f)
/*�����̗]��*/			#define NUMBER_MARGIN	(10.0f)

//�ÓI�����o�ϐ�
/*�I�u�W�F�N�g2D�̃|�C���^*/CNumber	*CTimer::m_apNumber[TIMER_DEJIT] = {};
/*�^�C�}�[�̐F*/			D3DXCOLOR CTimer::s_TimerColor = GetColor(COL_WHITE);

//=======================================
//�R���X�g���N�^
//=======================================
CTimer::CTimer()
{
	//�����I�u�W�F�N�g�����ׂ�NULL��
	for (int nCntTimer = 0; nCntTimer < TIMER_DEJIT; nCntTimer++)
	{
		m_apNumber[nCntTimer] = NULL;
	}

	/*�^�C�}�[������*/m_nTimer = 0;
	/*�^�C�}�[�̐F*/s_TimerColor = GetColor(COL_WHITE);
}

//=======================================
//�f�X�g���N�^
//=======================================
CTimer::~CTimer()
{

}

//=======================================
//��������
//=======================================
CTimer *CTimer::Create(void)
{
	//�C���X�^���X����
	CTimer *pTimer = new CTimer;

	//�C���X�^���X�𐶐��ł���
	if (pTimer != NULL)
	{
		//����������
		pTimer->Init();
	}

	//�C���X�^���X��Ԃ�
	return pTimer;
}

//=======================================
//����������
//=======================================
HRESULT CTimer::Init(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DEJIT; nCntTimer++)
	{
		//�����C���X�^���X����
		CNumber *pNumber = CNumber::Create();

		//�C���X�^���X�������ł��Ȃ�����
		if (pNumber == NULL)
		{//���������s
			return E_FAIL;
		}

		SetType(CObject::TYPE_SCORE);	//��ސݒ�
		m_apNumber[nCntTimer] = pNumber;//�C���X�^���X���蓖��
	}

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CTimer::Uninit(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DEJIT; nCntTimer++)
	{
		//�����I�u�W�F�N�g�폜
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->Uninit();
			m_apNumber[nCntTimer] = NULL;
		}
	}

	//�������g��j��
	this->Release();
}

//=======================================
//�X�V����
//=======================================
void CTimer::Update(void)
{
	//�t���[���J�E���^�[����
	m_nCountFrame++;

	//�t���[����1�b�o�߂���
	if (m_nCountFrame % MAX_FPS == 0)
	{
		/*	�J�E���^�[������ */	m_nCountFrame = 0;
		/*	  �^�C�}�[����	 */	m_nTimer--;

		//�^�C�����~�b�g�\��
		if (m_nTimer == 40)
		{
			CManager::GetSound()->Play(CSound::SE_TIME_LIMIT);
			s_TimerColor = GetColor(COL_RED);
		}

		//�^�C�}�[���O�ɂȂ���
		if (m_nTimer < 0)
		{
			//�t�F�[�h����
			CFade::Set(CScene::RESULT, CFade::FADE_WIPE_OUT);
			m_nTimer = 0;
		}

		//�^�C�}�[�Z�b�g
		Set(m_nTimer);
	}
}

//=======================================
//�`�揈��
//=======================================
void CTimer::Draw(void)
{

}

//=======================================
//�^�C�}�[�ݒ菈��
//=======================================
void CTimer::Set(int nTimer)
{
	//�ݒ肷��^�C�}�[�l���ő�l�𒴂���
	if (nTimer > MAX_TIMER)
	{
		//�ݒ肷��l���ő�l�ɖ߂�
		nTimer = MAX_TIMER;
	}

	//�^�C�}�[�ݒ�
	m_nTimer = nTimer;

	//�S���̒��_���ݒ�
	for (int nCntTimer = 0; nCntTimer < TIMER_DEJIT; nCntTimer++)
	{
		//�����ݒ�i��̈ʂ���ݒ�
		m_apNumber[nCntTimer]->SetInfo
		(nTimer % 10, D3DXVECTOR3(DEFAULT_POS_X - ((float)nCntTimer * NUMBER_WIDTH + (float)nCntTimer * NUMBER_MARGIN), NUMBER_HEIGHT, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT);

		//�^�C�}�[�F�ݒ�
		m_apNumber[nCntTimer]->SetVertexInfo(s_TimerColor);

		//�ꌅ������
		nTimer /= 10;
	}
}