//=============================================================================
// 
//  �t���σw�b�_�[ [leaf.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _LEAF_H_
#define _LEAF_H_	// ��d�C���N���[�h�h�~

#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �t���σN���X
class CLeaf : public CObject3D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_FALL = 0,		// ����
		STATE_FADEOUT = 0,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CLeaf(int nPriority = 7, const LAYER layer = LAYER::LAYER_LEAF);
	~CLeaf();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// �ÓI�֐�
	//=============================
	static CLeaf* Create(const MyLib::Vector3& pos);

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CLeaf::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateFall();		// ����
	void StateFadeOut();	// �t�F�[�h�A�E�g

	// ���̑��֐�
	void CalDestRotation();	// �ڕW�̌����v�Z

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTimer;	// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑��ϐ�
	MyLib::Vector3 m_rotDest;	// �ڕW�̌���
	float m_fRotateTimer;		// ��]�܂ł̎���
	float m_fRotateInterval;	// ��]�܂ł̊Ԋu
};


#endif