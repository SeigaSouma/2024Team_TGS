//======================================================
//
//�w�i�i2D�j�����̃w�b�_[bg.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _BG_H_	//���d�C���N���[�h�΍�
#define _BG_H_
#include "..\..\_Core\_object\object_2d\object2D.h"

class CBG : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CBG(int nPriority = PRIORITY_BG);
	virtual ~CBG();

	//����
	static CBG* Create(const int nPriority = PRIORITY_BG);

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	
};

#endif	//���d