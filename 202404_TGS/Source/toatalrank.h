//=============================================================================
// 
//  �g�[�^�������N�w�b�_�[ [toatalrank.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TOATALRANK_H_
#define _TOATALRANK_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"
#include "judge.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �g�[�^�������N�N���X
class CToatalRank : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	CToatalRank(int nPriority = 3);
	~CToatalRank();

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
	static CToatalRank* Create(CJudge::JUDGE rank, float time);

private:

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn

	// ���̑��֐�
	CJudge::JUDGE CalRank(CJudge::JUDGE rank, float time);	// �����N�v�Z

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑�
	CJudge::JUDGE m_Rank;	// �����N

};


#endif