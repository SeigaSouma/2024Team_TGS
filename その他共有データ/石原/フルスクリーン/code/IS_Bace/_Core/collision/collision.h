//======================================================
//
//�����蔻�菈���̃w�b�_[collision.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "..\interface.h"
#include <vector>

//�O���錾
class CObject;

//���̓����蔻��N���X
class CBoxCollider
{
public:
	//�����蔻����
	enum TYPE
	{
		TYPE_COLLISION = 0,	//�Ԃ���
		TYPE_TRIGGER,		//�d�Ȃ�
		TYPE_NONE,			//�����蔻�肪�Ȃ�
		TYPE_MAX
	};

	//�����蔻��pfloat3��
	struct ColFloat
	{
		float* pPosMain, pPosSubA, pPosSubB;
		float fSizeMain, fSizeSubA, fSizeSubB;
	};

	//�����蔻�茋��
	struct CollisionResult
	{
		std::vector<CObject*> collList;	//������������X�g
		bool bHit[3];					//�Փ˂�������3����
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CBoxCollider();
	~CBoxCollider();

	//�����蔻��`�F�b�N�i�������Ă���Γ��������I�u�W�F�N�g�A�Ȃ���΂ʂ�ہj
	void CollisionCheck(void);

	//�����蔻��ݒ�
	static CBoxCollider* Create(ICollisionReader* iCollisionReader);
	void SetType(TYPE type) { m_type = type; }

	//�j��
	void Release(void);

	//�擾
	CollisionResult GetResult(void) { return m_collisionResult; }

	//���X�g�擾
	static CBoxCollider* GetTop(void) { return m_pTop; }
	CBoxCollider* GetNext(void) { return m_pNext; }

private:
	//���X�g
	static CBoxCollider* m_pTop;	//�擪�I�u�W�F�N�g
	static CBoxCollider* m_pCur;	//�Ō���I�u�W�F�N�g
	CBoxCollider* m_pNext;			//���̃I�u�W�F�N�g
	CBoxCollider* m_pPrev;			//�O�̃I�u�W�F�N�g

	bool CollisionAxis(ColFloat source, const float fPosMainOld, ColFloat dest, const TYPE otherType);

	ICollisionReader* m_iCollisionReader;	//�����蔻��Ɏg�����̂̎擾�Ɏg���C���^�[�t�F�[�X
	TYPE m_type;							//�����蔻����
	CollisionResult m_collisionResult;		//�����蔻�茋��
};

#endif // !_COLLISION_H_
