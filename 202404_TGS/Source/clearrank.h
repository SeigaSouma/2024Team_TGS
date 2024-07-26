//=============================================================================
// 
//  �N���A�����N�w�b�_�[ [clearrank.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GLASSCLUSH_H_
#define _GLASSCLUSH_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"
#include "judge.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �N���A�����N�N���X
class CClearRank : public CObject2D
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

	CClearRank(int nPriority = 3);
	~CClearRank();

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
	static CClearRank* Create(CJudge::JUDGE rank);

private:

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn

	// ���̑��֐�
	void CreateText();	// ��������

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑�
	CJudge::JUDGE m_Rank;	// �����N
	CObject2D* m_pText;		// ����

};


#endif