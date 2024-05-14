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
	const float MULTIPLIY_DASH = 2.0f;		// �_�b�V���̔{��
	const float STAMINA_AVOID = 30.0f;		// ����̃X�^�~�i�����
	const float LENGTH_AUTOFACE = 200.0f;	// �����Ō�������
}

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player)
{
	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();


	// �_�b�V������
	bool bDash = false;
	if (pInputGamepad->GetPress(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()) &&
		pInputGamepad->IsTipStick())
	{// ���X�e�B�b�N���|��Ă�ꍇ
		bDash = true;
	}
	player->SetEnableDash(bDash);


	// �ړ��ʎ擾
	float fMove = player->GetVelocity();
	if (bDash){
		// �_�b�V���{���|����
		fMove *= MULTIPLIY_DASH;
	}


	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();
	Camerarot.y -= pCamera->GetOriginRotation().y;

	// �ڕW�̌����擾
	float fRotDest = player->GetRotDest();

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();

	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// ��Ԏ擾
	CPlayer::STATE state = player->GetState();

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		//!m_pControlAtk->IsReserve() &&
		//!motionFrag.bATK &&
		state != CPlayer::STATE::STATE_KNOCKBACK &&
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_DEADWAIT &&
		state != CPlayer::STATE::STATE_FADEOUT)
	{// �ړ��\���[�V�����̎�

		/*move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
		move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;*/

		if (pInputKeyboard->GetPress(DIK_A))
		{// ���ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// ����ړ�

				move.x += sinf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// �����ړ�

				move.x += sinf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// ���ړ�

				move.x += sinf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{// �E�ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// �E��ړ�

				move.x += sinf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// �E���ړ�

				move.x += sinf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// �E�ړ�

				move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{// ��ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{// ���ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
		else
		{
			motionFrag.bMove = false;
		}

		if (pInputGamepad->IsTipStick())
		{// ���X�e�B�b�N���|��Ă�ꍇ

			// �ړ����ɂ���
			motionFrag.bMove = true;

			// �X�e�B�b�N�̌����擾
			float stickrot = pInputGamepad->GetStickRotL(player->GetMyPlayerIdx());
			UtilFunc::Transformation::RotNormalize(stickrot);

			// �ړ��ʂƌ����ݒ�
			move.x += sinf(stickrot + Camerarot.y) * fMove;
			move.z += cosf(stickrot + Camerarot.y) * fMove;
			fRotDest = D3DX_PI + stickrot + Camerarot.y;
		}

		// �W�����v�󋵎擾
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// �L�����Z���\ && �ړ���

			// ���[�V�����I������
			pMotion->ToggleFinish(true);

			// �ړ����[�V����
			if (bDash)
			{
				pMotion->Set(CPlayer::MOTION::MOTION_DASH);
			}
			else
			{
				pMotion->Set(CPlayer::MOTION::MOTION_WALK);
			}
		}

		if (!bJump &&
			(pInputKeyboard->GetTrigger(DIK_SPACE) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx())))
		{// �W�����v

			bJump = true;
			motionFrag.bJump = true;
			move.y += 17.0f;

			pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

			// �T�E���h�Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_JUMP);
		}

		// �W�����v����ݒ�
		player->SetEnableJump(bJump);

	}
	else if (
		pMotion->IsGetMove(nMotionType) == 0 &&	// �ړ��\�ȃ��[�V�������擾
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_FADEOUT)
	{
		if (pInputKeyboard->GetPress(DIK_A))
		{//���L�[�������ꂽ,���ړ�

			if (pInputKeyboard->GetPress(DIK_W))
			{//A+W,����ړ�
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//A+S,�����ړ�
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,���ړ�
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{//D�L�[�������ꂽ,�E�ړ�

			if (pInputKeyboard->GetPress(DIK_W))
			{//D+W,�E��ړ�
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//D+S,�E���ړ�
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,�E�ړ�
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{//W�������ꂽ�A��ړ�
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{//S�������ꂽ�A���ړ�
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
	}

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);

	// �ړ��ʐݒ�
	player->SetMove(move);

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(fRotDest);
	player->SetRotDest(fRotDest);
}

//==========================================================================
// �A�N�V����
//==========================================================================
void CPlayerControlBaggage::Action(CPlayer* player, CBaggage* pBaggage)
{
	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();
	//move *= 0.5f;

	static bool fall = false;

	static float up = 0.7f, power = 0.5f;
	ImGui::DragFloat("up", &up, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("power", &power, 0.01f, 0.0f, 0.0f, "%.2f");
	if (CInputKeyboard::GetInstance()->GetPress(DIK_RETURN))
	{
		if (fall) {
			fall = false;
			pBaggage->SetForce(0.0f);
		}

		pBaggage->SetMove(MyLib::Vector3(move.x, pBaggage->GetMove().y, move.z));
		pBaggage->AddForce(MyLib::Vector3(power, up, 0.0f), player->GetPosition() + move);
	}

	if (CInputKeyboard::GetInstance()->GetRelease(DIK_RETURN))
	{
		// �~�����
		fall = true;
	}
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