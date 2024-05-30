//=============================================================================
// 
//  �^�C�}�[�w�b�_�[ [timer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TIMER_H_
#define _TIMER_H_	// ��d�C���N���[�h�h�~

#include "main.h"

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�}�[�N���X
class CTimer
{
public:

	enum eState
	{
		STATE_WAIT = 0,		// �ҋ@
		STATE_GOAL,			// �S�[��
		STATE_MAX
	};

	CTimer(int nPriority = 6);
	~CTimer();

	static CTimer *Create();
	static CTimer *GetInstance() { return m_pTimer; }

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	float GetTime() { return m_fTime; }	// ���Ԏ擾
	void SetEnableAddTime(bool frag) { m_bAddTime = frag; }	// ���ԉ��Z�t���O�ݒ�

	CTimer::eState GetState() { return m_state; }
	void SetState(eState state) { m_state = state; }

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CTimer::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// ��Ԋ֐����X�g

	//=============================
	// �����o�֐�
	//=============================
	void StateWait();
	void StateGoal();

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_pos;			// �ʒu
	MyLib::Vector3 m_posOrigin;		// ���̈ʒu
	eState m_state;					// ���
	float m_fStateTime;				// ��Ԏ���
	float m_fTime;					// ����
	bool m_bAddTime;				// �^�C�}�[���Z�̃t���O
	CMultiNumber* m_pClearTime[3];		// ��ނ��Ƃ̐���
	static CTimer *m_pTimer;	// ���g�̃|�C���^
};



#endif