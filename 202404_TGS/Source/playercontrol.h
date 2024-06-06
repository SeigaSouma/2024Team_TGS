//=============================================================================
// 
//  �v���C���[�R���g���[���w�b�_�[ [playercontrol.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_	// ��d�C���N���[�h�h�~

#define BRESSRANGE (1)

#include "player.h"
#include "baggage.h"

#include "command_group.h"

#if BRESSRANGE
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

#if BRESSRANGE
	CDebugBressRange* m_pBressRange = nullptr;		// ���͈�
	CDebugBressRange* m_pBressHeight = nullptr;		// ���̓͂������͈�
	CCollisionLine_Box* m_pBox = nullptr;	// �f�o�b�O�p�����蔻��
#endif

	float m_fHeight = 0.0f;				// ���݂̑�����
	float m_fMaxHeight = 1000.0f;		// ���̓͂��ő�̍���
	float m_fTimeDownHeight = 0.0f;		// �����̍~������
	float m_fHeightVelocity = 10.0f;	// ���̉��Z��

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

//=============================
// �g���b�N����
//=============================
class CPlayerControlTrick
{
public:

	// �R���X�g���N�^
	CPlayerControlTrick() {
		m_pCommandPad = nullptr;
		m_pCommandPad = CCommandGroup::Create("data\\TEXT\\command\\gamepad.txt");
	}

	virtual void Uninit();
	virtual void Trick(CPlayer* player, int& nIdx, bool& bValue);	// �g���b�N

private:

	CCommandGroup* m_pCommandPad;
};

#endif