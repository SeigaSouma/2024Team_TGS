//=============================================================================
// 
//  ���U���g�}�l�[�W���w�b�_�[ [resultmanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"
#include "judge.h"

// �O���錾
class CEnemy;
class CBoss;

//==========================================================================
// �N���X��`
//==========================================================================
// ���U���g�}�l�[�W���N���X��`
class CResultManager
{
public:

	CResultManager();
	~CResultManager();

	HRESULT Init();
	void Uninit();
	void Update();
	void Reset();	// ���Z�b�g

	void SetJudgeRank(CJudge::JUDGE rank) { m_JudgeRank = rank; }	// �ŏI�]���ݒ�
	CJudge::JUDGE GetJudgeRank() { return m_JudgeRank; }			// �ŏI�]���擾
	void SetClearTime(float time) { m_fClearTime = time; }			// �N���A���Ԑݒ�
	float GetClearTime() { return m_fClearTime; }					// �N���A���Ԏ擾


	static CResultManager *Create();	// ��������
	static CResultManager* GetInstance() { return m_pThisPtr; }
	
private:

	CJudge::JUDGE m_JudgeRank;	// �ŏI�]��
	float m_fClearTime;			// �N���A����
	static CResultManager* m_pThisPtr;	// ���g�̃|�C���^
};



#endif