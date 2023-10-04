//================================================================================================
//
//�Q�[����ʏ���[game.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

//�Q�[���N���X
class CGame : public CScene
{
public:
	CGame();
	virtual ~CGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
private:
	//====================
	//�ÓI�����o�萔
	//====================
	static const char	*FILENAME_INTERIOR;	//�������t�@�C����
	static const char	*FILENAME_OBJECT;	//�I�u�W�F�N�g���t�@�C����
	static const char	*FILENAME_ENEMY;	//�G�̔z�u���t�@�C����
	static const int	SET_NUM_ITEM;		//�A�C�e���̔z�u��

	void TextureLoad(void);		//�I�u�W�F�N�g�̃e�N�X�`���ǂݍ���
	void SetItem(void);			//�A�C�e���̔z�u
	void SetInterior(CFile *pFile);	//�X�e�[�W�̓����z�u
	void SetObject(CFile *pFile);	//�I�u�W�F�N�g�̔z�u
	void SetEnemy(CFile *pFile);	//�G�̔z�u
};

#endif