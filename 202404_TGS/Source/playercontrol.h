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

	// �����o�֐�
	bool CollisionObstacle(CPlayer* player, CBaggage* pBaggage);	// ��Q������

	// �����o�ϐ�
	Effekseer::Handle* m_BressHandle = nullptr;
	CDebugBressRange* m_pBressRange = nullptr;
	CDebugBressRange* m_pBressHeight = nullptr;
	float m_fHeight = 0.0f;
	float m_fHeightVelocity = 10.0f;
};

//=============================
// ���㑀��
//=============================
class CPlayerControlSurfacing
{
public:

	// �R���X�g���N�^
	CPlayerControlSurfacing() {}

	virtual float Surfacing(CPlayer* player);	// �ړ�

private:

	float m_fHeight = 0.0f;	// ���㍂��
};

#endif