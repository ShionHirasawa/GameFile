//================================================================================================
//
//�X�R�A����[score.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "number.h"

//�X�R�A�̌���
#define SCORE_DEJIT	(8)

//�O���錾
class CNumber;

//�X�R�A�N���X
class CScore : public CNumber
{
public:
	CScore();
	~CScore();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore *Create(void);
	/*	�X�R�A�ݒ�	*/static void Set(int nScore = 0);
	/*	�X�R�A���Z	*/static void Add(const int nValue) { Set(m_nScore + nValue); }

private:
	static CNumber	*m_apNumber[SCORE_DEJIT];	// �����I�u�W�F�N�g�̃|�C���^
	static int		m_nScore;	// �X�R�A
};

#endif