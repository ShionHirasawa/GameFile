#ifndef _BLOCK_H_
#define _BLOCK_H_

//�u���b�N�̐��\�\����
typedef enum
{
	BLOCK_CAPACITY_STAY = ZERO,		//�����Ȃ�
	BLOCK_CAPACITY_MOVE,			//����
	BLOCK_CAPACITY_CARRY,			//�����^�ׂ�
	BLOCK_CAPACITY_MAX
}BLOCK_CAPACITY;

//�u���b�N�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;			//�ʒu
	float fWidth;				//��
	float fHeight;				//����
	int nType;					//�u���b�N�̎��
	D3DXVECTOR2 Tex;			//�e�N�X�`���̍��W
	bool bUse;					//�g�p����Ă��邩�ǂ���
}Block;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(void);
bool X_CollisionBlockToPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove,float fWidth, float fHeight, Block **pBlock);
bool Y_CollisionBlockToPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove,float fWidth, float fHeight, Block **pBlock);
bool CollisionBlockToHako(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fWidth, float fHeight, Block **pBlock);
void FixHakoPos(D3DXVECTOR3 FixMove);
#endif