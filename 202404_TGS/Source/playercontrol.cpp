//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "playercontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "enemy.h"

namespace
{
	const float STAMINA_AVOID = 30.0f;	// ����̃X�^�~�i�����
	const float LENGTH_AUTOFACE = 200.0f;	// �����Ō�������
}

//==========================================================================
// �U���\�t���O�擾
//==========================================================================
bool CPlayerControlAttack::IsAttack(CPlayer* player)
{
	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// ���݂̎�ގ擾
	CMotion* pMotion = player->GetMotion();

	if ((pMotion->IsGetCombiable() || pMotion->IsGetCancelable()) &&
		!player->IsJump())
	{
		return true;
	}
	return false;
}

//==========================================================================
// �ʏ�U��
//==========================================================================
void CPlayerControlAttack::Attack(CPlayer* player)
{
	
}

//==========================================================================
// �ʏ�U��
//==========================================================================
void CPlayerControlAttack_Level1::Attack(CPlayer* player)
{

}



//==========================================================================
// �h��
//==========================================================================
// ���
void CPlayerControlDefence::Defence(CPlayer* player)
{
	
}

// ���x��1
void CPlayerControlDefence_Level1::Defence(CPlayer* player)
{
	
}

//==========================================================================
// ���
//==========================================================================
void CPlayerControlAvoid::Avoid(CPlayer* player)
{

}




//==========================================================================
// �K�[�h
//==========================================================================
// �󂯂����̏���
void CPlayerGuard::HitProcess(CPlayer* player, MyLib::Vector3 enemypos)
{
	
	// �ʒu�擾
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 rot = player->GetRotation();

	// �ڕW�̊p�x�ݒ�
	float fRotDest = atan2f((pos.x - enemypos.x), (pos.z - enemypos.z));
	player->SetRotation(MyLib::Vector3(rot.x, fRotDest, rot.z));
	player->SetRotDest(fRotDest);

	player->SetMove(MyLib::Vector3(
		sinf(fRotDest) * 40.0f,
		0.0f,
		cosf(fRotDest) * 40.0f));
}