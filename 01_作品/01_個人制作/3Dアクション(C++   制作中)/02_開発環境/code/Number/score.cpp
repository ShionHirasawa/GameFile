//================================================================================================
//
//�X�R�A����[score.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../Main/manager.h"
#include "number.h"
#include "score.h"

/***		�}�N����`		***/
/*�X�R�A�̍ő吔*/		#define MAX_SCORE	((int)pow(10, SCORE_DEJIT + 1) - 1)
/*�X�R�A�̊�]���W*/	#define DEFAULT_POS_X	(1100.0f)
/*�����̉���*/			#define NUMBER_WIDTH	(20.0f)
/*�����̍���*/			#define NUMBER_HEIGHT	(40.0f)
/*�����̗]��*/			#define NUMBER_MARGIN	(10.0f)

//�ÓI�����o�ϐ�
/*�I�u�W�F�N�g2D�̃|�C���^*/CNumber	*CScore::m_apNumber[SCORE_DEJIT] = {};
/*�X�R�A*/					int		CScore::m_nScore = 0;

//=======================================
//�R���X�g���N�^
//=======================================
CScore::CScore()
{
	//�����I�u�W�F�N�g�����ׂ�NULL��
	for (int nCntScore = 0; nCntScore < SCORE_DEJIT; nCntScore++)
	{
		m_apNumber[nCntScore] = NULL;
	}

	/*�X�R�A������*/m_nScore = 0;
}

//=======================================
//�f�X�g���N�^
//=======================================
CScore::~CScore()
{

}

//=======================================
//��������
//=======================================
CScore *CScore::Create(void)
{
	//�����C���X�^���X����
	CScore *pScore = new CScore;

	//�C���X�^���X�𐶐��ł���
	if (pScore != NULL)
	{
		//����������
		pScore->Init();
	}

	//�C���X�^���X��Ԃ�
	return pScore;
}

//=======================================
//����������
//=======================================
HRESULT CScore::Init(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DEJIT; nCntScore++)
	{
		//�����C���X�^���X����
		CNumber *pNumber = CNumber::Create();

		//�C���X�^���X�������ł��Ȃ�����
		if (pNumber == NULL)
		{//���������s
			return E_FAIL;
		}

		SetType(CObject::TYPE_SCORE);	//��ސݒ�
		m_apNumber[nCntScore] = pNumber;//�C���X�^���X���蓖��
	}

	//����������
	return S_OK;
}

//=======================================
//�I������
//=======================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DEJIT; nCntScore++)
	{
		//�����I�u�W�F�N�g�폜
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();
			m_apNumber[nCntScore] = NULL;
		}
	}

	//�������g��j��
	this->Release();
}

//=======================================
//�X�V����
//=======================================
void CScore::Update(void)
{

}

//=======================================
//�`�揈��
//=======================================
void CScore::Draw(void)
{

}

//=======================================
//�X�R�A�ݒ菈��
//=======================================
void CScore::Set(int nScore)
{
	//�ݒ肷��X�R�A�l���ő�l�𒴂���
	if (nScore > MAX_SCORE)
	{
		//�ݒ肷��l���ő�l�ɖ߂�
		nScore = MAX_SCORE;
	}

	//�X�R�A�ݒ�
	m_nScore = nScore;

	//�S���̒��_���ݒ�
	for (int nCntScore = 0; nCntScore < SCORE_DEJIT; nCntScore++)
	{
		//�����ݒ�i��̈ʂ���ݒ�
		m_apNumber[nCntScore]->SetInfo
		(nScore % 10, D3DXVECTOR3(DEFAULT_POS_X - ((float)nCntScore * NUMBER_WIDTH + (float)nCntScore * NUMBER_MARGIN), NUMBER_HEIGHT, 0.0f), NUMBER_WIDTH, NUMBER_HEIGHT);

		//�ꌅ������
		nScore /= 10;
	}
}