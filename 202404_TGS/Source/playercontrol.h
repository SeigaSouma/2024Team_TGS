//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_	// ��d�C���N���[�h�h�~

#include "player.h"
#include "baggage.h"

#if _DEBUG
#include "debug_bressrange.h"
#endif

//==========================================================================
// �v���C���[�R���g���[���N���X��`
//==========================================================================
//=============================
// �ړ�
//=============================
class CPlayerControlMove
{
public:

	// �R���X�g���N�^
	CPlayerControlMove() {}

	virtual void Move(CPlayer* player);	// �ړ�

};

//=============================
// �ו�����
//=============================
class CPlayerControlBaggage
{
public:

	// �R���X�g���N�^
	CPlayerControlBaggage() 
	{
		m_BressHandle = nullptr;
	}

	virtual void Action(CPlayer* player, CBaggage* pBaggage);	// �A�N�V����

private:
	Effekseer::Handle* m_BressHandle = nullptr;
	CDebugBressRange* m_pBressRange = nullptr;
	CDebugBressRange* m_pBressHeight = nullptr;
	float m_fHeight = 0.0f;
	float m_fHeightVelocity = 10.0f;
};



#endif