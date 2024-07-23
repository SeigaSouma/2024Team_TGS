//=============================================================================
// 
// �L���b�`���ʏ��� [catchresult.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CATCHRESULT_H_
#define _CATCHRESULT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �L���b�`����
class CCatchResult : public CObject2D
{
public:

	CCatchResult(int nPriority = 8);
	~CCatchResult();

	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	static CCatchResult* Create(const MyLib::Vector3& pos, bool bClear);

private:

	// ��ԗ�
	enum State
	{
		STATE_EXPANSION = 0,
		STATE_EXPNONE,
		STATE_FADEOUT,
		STATE_MAX
	};

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CCatchResult::* STATE_FUNC)();	// ��ԏ����̃��X�g
	static STATE_FUNC m_StateFuncList[];

	//=============================
	// �����o�֐�
	//=============================
	void StateExpansion();	// �g��
	void StateExpNone();	// �g��㉽�����Ȃ�
	void StateFadeOut();	// �t�F�[�h�A�E�g���

	//=============================
	// �����o�ϐ�
	//=============================
	State m_state;			// ���
	float m_fStateTimer;	// ��ԃ^�C�}�[
};

#endif