//=============================================================================
// 
//  �v���C���[���� [player.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "player.h"
#include "game.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "calculation.h"
#include "Xload.h"
#include "model.h"
#include "elevation.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"
#include "ballast.h"
#include "impactwave.h"
#include "sound.h"
#include "stage.h"
#include "objectX.h"
#include "instantfade.h"
#include "fade.h"
#include "listmanager.h"
#include "collisionobject.h"
#include "limitarea.h"
#include "MyEffekseer.h"
#include "map.h"
#include "rankingmanager.h"
#include "edit_map.h"
#include "damagepoint.h"
#include "retryui.h"
#include "timer.h"

#include "checkpoint.h"
#include "baggage.h"
#include "baggageManager.h"
#include "spline.h"
#include "course.h"
#include "meshbubble.h"

// �g�p�N���X
#include "playercontrol.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\player\\tyuuni\\setup_player.txt";	// �L�����N�^�[�t�@�C��
	const float JUMP = 20.0f * 1.5f;			// �W�����v�͏����l
	const float TIME_DMG = static_cast<float>(20) / static_cast<float>(mylib_const::DEFAULT_FPS);	// �_���[�W����
	const int INVINCIBLE_INT = 2;				// ���G�̊Ԋu
	const int INVINCIBLE_TIME = 0;				// ���G�̎���
	const int DEADTIME = 120;					// ���S���̎���
	const int FADEOUTTIME = 60;					// �t�F�[�h�A�E�g�̎���
	const float MULTIPLIY_DASH = 2.0f;			// �_�b�V���̔{��
	const float TIME_DASHATTACK = 0.3f;			// �_�b�V���U���ɕK�v�Ȏ���
	const int DEFAULT_STAMINA = 200;			// �X�^�~�i�̃f�t�H���g�l
	const float SUBVALUE_DASH = 0.1f;			// �_�b�V���̌��Z��
	const float SUBVALUE_AVOID = 25.0f;			// ����̌��Z��

	// �X�e�[�^�X
	const float DEFAULT_RESPAWNHEAL = 0.45f;			// ���X�|�[�����̉񕜊���
	const float DEFAULT_SUBVALUE_GUARD = 30.0f;			// �K�[�h�̃X�^�~�i���Z��
	const float DEFAULT_SUBVALUE_COUNTER = 5.0f;		// �J�E���^�[�̃X�^�~�i���Z��
	const float DEFAULT_COUNTERHEAL = 0.0f;				// �J�E���^�[�̃X�^�~�i�񕜗�
	const float DEFAULT_MULTIPLY_ATTACK = 1.0f;			// �U���{��
	const float DEFAULT_CHARGETIME = 0.9f;				// �`���[�W����
	const int DEFAULT_FRAME_EXTENSION_COUNTER = 14;		// �J�E���^�[�P�\�t���[��
	const float  DEFAULT_MULTIPLY_GUARD = 0.4f;			// �J�[�h�̌y��
	const float DEFAULT_TIME_ADDDOWN = 3.0f;			// �_�E�����ԕt�^
	const bool DEFAULT_IS_CHARGEFLINCH = true;			// �`���[�W�����݃t���O
	const int DEFAULT_RESPAWN_PERCENT = 20;				// �����m��
	const float MULTIPLY_CHARGEATK = 2.0f;				// �`���[�W�U���̔{��
	const float MAX_HEIGHT = 200.0f;					// �ő卂��
}

//==========================================================================
// �u���[�\��(�}���`�^�[�Q�b�g�����_�����O)�p�萔��`
//==========================================================================
namespace MULTITARGET
{
	// ON��
	const float ON_ALPHA = (0.6f);		// �ڕW�����x
	const float ON_MULTI = (1.02f);		// �ڕW�{��
	const float ON_TIMER = (120.0f);	// �J�ڃ^�C�}�[

	// ���S��
	const float DEATH_ALPHA = (0.0f);		// �ڕW�����x
	const float DEATH_MULTI = (1.0f);		// �ڕW�{��
	const float DEATH_TIMER = (240.0f);		// �J�ڃ^�C�}�[

	// ���Z�b�g��
	const float RESET_ALPHA = (0.0f);		// �ڕW�����x
	const float RESET_MULTI = (1.0f);		// �ڕW�{��
	const float RESET_TIMER = (0.01f);		// �J�ڃ^�C�}�[
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayer::STATE_FUNC CPlayer::m_StateFunc[] =
{
	&CPlayer::StateNone,		// �Ȃ�
	&CPlayer::StateInvincible,	// ���G
	&CPlayer::StateDamage,		// �_���[�W
	&CPlayer::StateDead,		// ���S
	&CPlayer::StateDeadWait,	// ���S�ҋ@
	&CPlayer::StateReturn,		// �`�F�b�N�|�C���g�ɖ߂�
	&CPlayer::StateRestart,		// �X�^�[�g�ɖ߂�
	&CPlayer::StateRespawn,		// ���X�|�[��
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CPlayer> CPlayer::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayer::CPlayer(int nPriority) : CObjectChara(nPriority)
{
	// �l�̃N���A
	// ���L�ϐ�
	m_bJump = false;				// �W�����v�����ǂ���
	m_bLandOld = false;				// �ߋ��̒��n���
	m_bHitStage = false;			// �X�e�[�W�̓����蔻��
	m_bLandField = false;			// �t�B�[���h�̒��n����
	m_bHitWall = false;				// �ǂ̓����蔻��
	m_nCntWalk = 0;					// ���s�J�E���^�[
	m_state = STATE_NONE;			// ���

	SMotionFrag initFrag = {};
	m_sMotionFrag = initFrag;		// ���[�V�����̃t���O

	// �v���C�x�[�g�ϐ�
	m_Oldstate = STATE_NONE;						// �O��̏��
	m_mMatcol = mylib_const::DEFAULT_COLOR;			// �}�e���A���̐F
	m_posKnokBack = mylib_const::DEFAULT_VECTOR3;	// �m�b�N�o�b�N�̈ʒu
	m_KnokBackMove = mylib_const::DEFAULT_VECTOR3;	// �m�b�N�o�b�N�̈ړ���
	m_nCntState = 0;								// ��ԑJ�ڃJ�E���^�[
	m_bDash = false;								// �_�b�V������
	m_fDashTime = 0.0f;								// �_�b�V������
	m_fMoveLength = 0.0f;							// �ړ�����
	m_bMotionAutoSet = false;						// ���[�V�����̎����ݒ�

	m_PlayerStatus = sPlayerStatus();				// �v���C���[�X�e�[�^�X
	m_sDamageInfo = sDamageInfo();					// �_���[�W���

	m_nMyPlayerIdx = 0;								// �v���C���[�C���f�b�N�X�ԍ�
	m_pShadow = nullptr;							// �e�̏��
	m_pHPGauge = nullptr;							// HP�Q�[�W�̃|�C���^
	m_pBaggage = nullptr;							// �ו��̃|�C���^
	m_pRetryUI = nullptr;							// ���g���CUI�̃|�C���^

	m_pControlMove = nullptr;						// �ړ�����
	m_pControlBaggage = nullptr;					// �ו�����
	m_pControlSurfacing = nullptr;					// ���㑀��
	m_pControlTrick = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayer::~CPlayer()
{

}

//==========================================================================
// ��������
//==========================================================================
CPlayer* CPlayer::Create(int nIdx)
{
	// �������m��
	CPlayer* pPlayer = DEBUG_NEW CPlayer;

	if (pPlayer != nullptr)
	{
		// �v���C���[�C���f�b�N�X�ԍ�
		pPlayer->m_nMyPlayerIdx = nIdx;

		// ����������
		pPlayer->Init();
	}

	return pPlayer;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayer::Init()
{
	// ��ނ̐ݒ�
	SetType(TYPE_PLAYER);

	// �_���[�W�󂯕t������
	m_sDamageInfo.bReceived = true;
	m_sDamageInfo.reciveTime = 0.0f;

	m_state = STATE_NONE;	// ���
	m_nCntState = 0;		// ��ԑJ�ڃJ�E���^�[
	m_bLandOld = true;		// �O��̒��n���
	m_bMotionAutoSet = true;						// ���[�V�����̎����ݒ�
	m_sMotionFrag.bMove = true;

	// �L�����쐬
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// ���蓖��
	m_List.Regist(this);

	// ����֘A
	ChangeMoveControl(DEBUG_NEW CPlayerControlMove());
	ChangeBaggageControl(DEBUG_NEW CPlayerControlBaggage);
	ChangeSurfacingControl(DEBUG_NEW CPlayerControlSurfacing);
	ChangeTrickControl(DEBUG_NEW CPlayerControlTrick);

	// �ו�����
	m_pBaggage = CBaggageManager::GetInstance()->CreateBaggage(CBaggage::TYPE::TYPE_CLOTH);

	// ����̈ʒu
	CMotion* pMotion = GetMotion();
	m_posCylinder = GetMotion()->GetAttackPosition(GetModel(), *pMotion->GetInfo(MOTION::MOTION_WALK).AttackInfo[0]);

	MyLib::Vector3 pos = GetPosition();
	m_pBaggage->SetPosition(m_posCylinder);
	m_pBaggage->SetOriginPosition(m_pBaggage->GetPosition());

	return S_OK;
}

//==========================================================================
// �ړ��̑���ύX
//==========================================================================
void CPlayer::ChangeMoveControl(CPlayerControlMove* control)
{
	delete m_pControlMove;
	m_pControlMove = control;
}

//==========================================================================
// �ו��̑���ύX
//==========================================================================
void CPlayer::ChangeBaggageControl(CPlayerControlBaggage* control)
{
	delete m_pControlBaggage;
	m_pControlBaggage = control;
}

//==========================================================================
// ����̑���ύX
//==========================================================================
void CPlayer::ChangeSurfacingControl(CPlayerControlSurfacing* control)
{
	delete m_pControlSurfacing;
	m_pControlSurfacing = control;
}

//==========================================================================
// ����̑���ύX
//==========================================================================
void CPlayer::ChangeTrickControl(CPlayerControlTrick* control)
{
	delete m_pControlTrick;
	m_pControlTrick = control;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayer::Uninit()
{
	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// HP�Q�[�W
	if (m_pHPGauge != nullptr)
	{
		m_pHPGauge = nullptr;
	}

	// ����n
	DeleteControl();

	// �I������
	CObjectChara::Uninit();

	// �폜
	m_List.Delete(this);
}

//==========================================================================
// �I������
//==========================================================================
void CPlayer::Kill()
{
	my_particle::Create(GetPosition(), my_particle::TYPE_ENEMY_FADE);

	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// �ו��폜
	if (m_pBaggage != nullptr)
	{
		m_pBaggage->Kill();
		m_pBaggage = nullptr;
	}

	// ����n
	DeleteControl();

	// ���b�N�I���ݒ�
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->SetRockOn(0.0f, false);
	}

}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayer::Update()
{
	if (IsDeath())
	{
		return;
	}

	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	if (pInputKeyboard->GetTrigger(DIK_F5))
	{// F5�Ń��Z�b�g
		SetPosition(MyLib::Vector3(0.0f, 0.0f, -1000.0f));
		SetMove(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	}

	// �G�f�B�b�g���͔�����
	if (CGame::GetInstance()->GetElevation()->IsEdit())
	{
		return;
	}

	// �G�f�B�b�g���͔�����
	if (CGame::GetInstance()->GetEditType() != CGame::GetInstance()->EDITTYPE_OFF)
	{
		return;
	}

	// �ߋ��̈ʒu�ۑ�
	SetOldPosition(GetPosition());

	// �t���O���Z�b�g
	ResetFrag();

	// �e�̍X�V����
	CObjectChara::Update();

	// ����
	Controll();

	// ���[�V�����̐ݒ菈��
	if (CGame::GetInstance()->GetGameManager()->IsControll()){
	MotionSet();
	}

	// ��ԍX�V
	UpdateState();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera->GetStateCameraR() == CCamera::POSR_STATE_ROCKON)
	{
		pCamera->SetTargetPosition(pos);
		pCamera->SetTargetRotation(rot);
	}
	else if(pCamera->GetStateCameraR() == CCamera::POSR_STATE_NORMAL)
	{
		MyLib::Vector3 camerapos = pos;
		camerapos.y = (m_pBaggage->GetPosition().y + pos.y) * 0.5f;
		camerapos.y = UtilFunc::Transformation::Clamp(camerapos.y, 200.0f, 600.0f);

		pCamera->SetTargetPosition(camerapos);
		pCamera->SetTargetRotation(rot);
	}

	// �e�̈ʒu�X�V
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

	// �ʒu�̐���
	LimitPos();


	// ����̈ʒu
	CMotion* pMotion = GetMotion();
	m_posCylinder = pMotion->GetAttackPosition(GetModel(), *pMotion->GetInfo(MOTION::MOTION_WALK).AttackInfo[0]);

#if 1

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	if (pMotion == nullptr)return;

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[�v���C���[�̑���]------------------\n"
		"�ʒu�F�yX�F%f, Y�F%f, Z�F%f�z �yW / A / S / D�z\n"
		"�����F�yX�F%f, Y�F%f, Z�F%f�z �yZ / C�z\n"
		"�ړ��ʁF�yX�F%f, Y�F%f, Z�F%f�z\n"
		"���[�V�����F�y%d�z\n"
		"�̗́F�y%d�z\n"
		"��ԁF�y%d�z\n"
		"�_�b�V�����ԁF�y%f�z\n"
		, pos.x, pos.y, pos.z, rot.x, rot.y, rot.y, move.x, move.y, move.z, pMotion->GetType(), GetLife(), m_state, m_fDashTime);

#endif

}

//==========================================================================
// ���쏈��
//==========================================================================
void CPlayer::Controll()
{

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// �o�ߎ��Ԏ擾
	float fCurrentTime = CManager::GetInstance()->GetDeltaTime();

	if (CGame::GetInstance()->GetGameManager()->IsControll())
	{// �s���ł���Ƃ�

		// ����֐�
		if (m_state != STATE_DEAD &&
			m_state != STATE_DEADWAIT &&
			m_state != STATE_RETURN &&
			m_state != STATE_RESTART &&
			m_state != STATE::STATE_RESPAWN)
		{
			Bobbing();

			// �ړ�����
			m_pControlMove->Move(this);

			 // ���㑀��
			float fHeight = m_pControlSurfacing->Surfacing(this);

			MyLib::Vector3 pos = m_pBaggage->GetPosition();
			m_pBaggage->SetOriginPosition(MyLib::Vector3(0.0f, m_posCylinder.y + fHeight, 0.0f));
			
			m_pControlBaggage->Action(this, m_pBaggage);		// �ו�����
			m_pBaggage->SetOriginPosition(MyLib::Vector3(0.0f, m_posCylinder.y + fHeight, 0.0f));

			{// �g���b�N����
				int idx = -1; bool value = false;
				m_pControlTrick->Trick(this, idx, value);

				// ���쐬��
				if (value)
				{
					SetMotion(idx);	// ���[�V�����ύX
				}
			}
		}
	}
	else
	{
		// ���[�V�����擾
		CMotion* pMotion = GetMotion();
		pMotion->Set(MOTION_WALK, false);
	}

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 newPosition = GetPosition();
	MyLib::Vector3 sakiPos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �ړ��ʉ��Z
	newPosition.x += move.x;
	newPosition.z += move.z;

	// �ړ��������Z
	m_fMoveLength += move.x;
	newPosition = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), m_fMoveLength);

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	sakiPos.x = newPosition.x + sinf(D3DX_PI + rot.y) * GetRadius();
	sakiPos.z = newPosition.z + cosf(D3DX_PI + rot.y) * GetRadius();

	// ���݂ƖڕW�̍��������߂�
	float fRotDiff = GetRotDest() - rot.y;

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(fRotDiff);

	// �p�x�̕␳������
	rot.y += fRotDiff * 0.25f;
	UtilFunc::Transformation::RotNormalize(rot.y);

	// �����ݒ�
	SetRotation(rot);


	// �d�͏���
	if (m_state != STATE_DEAD && m_state != STATE_RETURN && m_state != STATE_RESTART && m_state != STATE_DEADWAIT)
	{
		move.y -= mylib_const::GRAVITY;

		// �ʒu�X�V
		newPosition.y += move.y;
		sakiPos.y = newPosition.y;
	}

	//**********************************
	// �����蔻��
	//**********************************
	bool bLandStage = Collision(sakiPos, move);

	bool bMove = false;
	if (m_bLandOld == false && bLandStage)
	{// �O��͒��n���Ă��Ȃ��āA����͒��n���Ă���ꍇ

		bMove = false;
	}

	if (m_bLandOld && bLandStage)
	{// �O�����������n���Ă���ꍇ
		bMove = true;
	}

	if (m_bHitWall == false && 
		(bLandStage == false || bMove || m_bLandField || m_bJump || m_sMotionFrag.bKnockBack || m_sMotionFrag.bDead))
	{
		pos.x = newPosition.x;
		pos.z = newPosition.z;
		pos.y = sakiPos.y;
		Collision(pos, move);

		// �O��͏���Ă����Ƃɂ���
		m_bLandOld = true;
	}
	else
	{
		MyLib::Vector3 posOld = GetOldPosition();
		pos.x = posOld.x;
		pos.z = posOld.z;
		pos = posOld;
		pos.y -= mylib_const::GRAVITY * 7.0f;

		if (m_bJump == false)
		{
			m_bJump = true;
		}

		Collision(pos, move);
	}

	// �����␳
	if (m_state == STATE_DMG)
	{
		move.x += (0.0f - move.x) * 0.1f;
		move.z += (0.0f - move.z) * 0.1f;
	}
	else if (m_state != STATE_DEAD && m_state != STATE_RETURN && m_state != STATE_RESTART)
	{
		move.x += (0.0f - move.x) * 0.25f;
		move.z += (0.0f - move.z) * 0.25f;
	}


	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// ���b�N�I���Ώې؂�ւ�
	if (CManager::GetInstance()->GetCamera()->IsRockOn())
	{
		SwitchRockOnTarget();
	}
	

	// �f�o�b�O�p
#if _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_LEFT))
	{
		CCollisionObject::Create(GetPosition(), mylib_const::DEFAULT_VECTOR3, 100000.0f, 3, 10000, CCollisionObject::TAG_PLAYER);
	}

	if (pInputKeyboard->GetRepeat(DIK_RIGHT, 4))
	{
		CPlayer::Hit(10000);
	}

	static float fff = 1.0f;
	if (pInputKeyboard->GetTrigger(DIK_UP))
	{
		fff += 0.1f;
		CSound::GetInstance()->SetFrequency(CSound::LABEL_BGM_GAME, fff);


	}
	if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		fff -= 0.1f;
		CSound::GetInstance()->SetFrequency(CSound::LABEL_BGM_GAME, fff);
	}

	if (pInputKeyboard->GetPress(DIK_J))
	{
		CDamagePoint::Create(GetPosition(), UtilFunc::Transformation::Random(1, 99));
	}
#endif
}

//==========================================================================
// ����֘A�폜
//==========================================================================
void CPlayer::DeleteControl()
{
	if (m_pControlMove != nullptr) {
		delete m_pControlMove;
		m_pControlMove = nullptr;
	}

	if (m_pControlBaggage != nullptr) {
		delete m_pControlBaggage;
		m_pControlBaggage = nullptr;
	}

	if (m_pControlSurfacing != nullptr) {
		delete m_pControlSurfacing;
		m_pControlSurfacing = nullptr;
	}

	if (m_pControlTrick != nullptr) {
		m_pControlTrick->Uninit();
		delete m_pControlTrick;
		m_pControlTrick = nullptr;
	}
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CPlayer::SetMotion(int motionIdx)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx);
}

//==========================================================================
// ���[�V�����̐ݒ�
//==========================================================================
void CPlayer::MotionSet()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// ���[�V�����̎����ݒ�
	if (!m_bMotionAutoSet)
	{
		return;
	}

	if (m_state == STATE_DEAD ||
		m_state == STATE_DEADWAIT ||
		m_state == STATE_RETURN ||
		m_state == STATE_RESTART)
	{
		return;
	}

	if (pMotion->IsFinish())
	{// �I�����Ă�����

		// ���݂̎�ގ擾
		int nType = pMotion->GetType();
		int nOldType = pMotion->GetOldType();

		if (m_sMotionFrag.bMove && m_sMotionFrag.bKnockBack == false && m_bJump == false &&
			m_sMotionFrag.bATK == false && m_sMotionFrag.bATK == false)
		{// �ړ����Ă�����

			m_sMotionFrag.bMove = false;	// �ړ�����OFF

			// �ړ����[�V����
			if (m_bDash)
			{
				pMotion->Set(MOTION_DASH);
			}
			else
			{
				pMotion->Set(MOTION_WALK);
			}
		}
		else if (m_sMotionFrag.bJump && m_sMotionFrag.bATK == false && m_sMotionFrag.bKnockBack == false && m_sMotionFrag.bDead == false)
		{// �W�����v��

			// �W�����v�̃t���OOFF
			m_sMotionFrag.bJump = false;

			// �W�����v���[�V����
			pMotion->Set(MOTION_JUMP);
		}
		else if (m_bJump && m_sMotionFrag.bJump == false && m_sMotionFrag.bATK == false && m_sMotionFrag.bKnockBack == false && m_sMotionFrag.bDead == false)
		{// �W�����v��&&�W�����v���[�V�������I����Ă鎞

			// �������[�V����
			pMotion->Set(MOTION_FALL);
		}
		else
		{
			// �j���[�g�������[�V����
			pMotion->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// ���[�V�����ʂ̏�Ԑݒ�
//==========================================================================
void CPlayer::MotionBySetState()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int nType = pMotion->GetType();

	switch (nType)
	{
	case MOTION_DASH:

		// �_�b�V�����ԉ��Z
		m_fDashTime += CManager::GetInstance()->GetDeltaTime();
		break;

	default:
		m_fDashTime = 0.0f;
		break;
	}
	

	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

}

//==========================================================================
// �t���O���Z�b�g
//==========================================================================
void CPlayer::ResetFrag()
{

}

//==========================================================================
// ���b�N�I���Ώې؂�ւ�
//==========================================================================
void CPlayer::SwitchRockOnTarget()
{
	
}

//==========================================================================
// �U��������
//==========================================================================
void CPlayer::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	// ����̈ʒu
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	switch (nType)
	{
	case MOTION::MOTION_WALK:
		/*if (nCntATK == 0)
		{
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_WALK1);
		}
		else{

			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_WALK2);
		}*/
		break;

	case MOTION::MOTION_DASH:
		/*if (nCntATK == 0)
		{
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DASH1);
		}
		else {

			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DASH2);
		}*/
		break;

	default:
		break;
	}
}

//==========================================================================
// �U�����蒆����
//==========================================================================
void CPlayer::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();

	// ����̈ʒu
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	CEffect3D* pEffect = nullptr;

	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(
		weponpos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		pATKInfo->fRangeSize, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	if (pATKInfo->bEndAtk)
	{
		return;
	}

	
}

//==========================================================================
// �ʒu����
//==========================================================================
void CPlayer::LimitPos()
{
	return;

	MyLib::Vector3 pos = GetPosition();

	// �G���A�������擾
	CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
	CLimitArea* pLimitArea = nullptr;

	while (limitareaList.ListLoop(&pLimitArea))
	{
		CLimitArea::sLimitEreaInfo info = pLimitArea->GetLimitEreaInfo();

		// ��l�̕ǂ�K�p
		if (pos.x + GetRadius() >= info.fMaxX) { pos.x = info.fMaxX - GetRadius(); }
		if (pos.x - GetRadius() <= info.fMinX) { pos.x = info.fMinX + GetRadius(); }
		if (pos.z + GetRadius() >= info.fMaxZ) { pos.z = info.fMaxZ - GetRadius(); }
		if (pos.z - GetRadius() <= info.fMinZ) { pos.z = info.fMinZ + GetRadius(); }
	}

	// �~�̉����߂�
	if (pos.LengthXZ() > mylib_const::RADIUS_STAGE)
	{// �␳
		pos = pos.Normal() * mylib_const::RADIUS_STAGE;
	}
	SetPosition(pos);

	CollisionMapObject();
}

//==========================================================================
// �`�F�b�N�|�C���g����
//==========================================================================
void CPlayer::ReaspawnCheckPoint()
{
	// �`�F�b�N�|�C���g��ID�擾
	int saveID = CCheckpoint::GetSaveID();
	MyLib::Vector3 pos;
	float fLength = 0.0f;

	if (saveID > -1)	// �`�F�b�N�|�C���g�ʉߍς�
	{
		// �`�F�b�N�|�C���g�擾
		CListManager<CCheckpoint> checkpointList = CCheckpoint::GetListObj();
		CCheckpoint* pCheckPoint = checkpointList.GetData(saveID);

		// �ʒu�擾
		pos = pCheckPoint->GetPosition();
		fLength = pCheckPoint->GetLength();
	}
	else // �`�F�b�N�|�C���g���ʉ�
	{
		pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), 0);
	}

	SetPosition(pos);
	SetMoveLength(fLength);

	// �J�����u�Ԉړ�
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->WarpCamera(pos + MyLib::Vector3(0.0f, 150.0f, 0.0f));

	//�^�C�}�[�X�g�b�v
	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_WAIT_AIRPUSH);

	// ���X�|�[���ݒ�
	ReaspawnSetting();
}

//==========================================================================
// �X�^�[�g�����i�������񌈂ߑł��Ȃ̂ł������悤�ɂ��āj
//==========================================================================
void CPlayer::RespawnStart()
{
	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 10.0f, 0.0f);

	// �J�����u�Ԉړ�
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->WarpCamera(pos + MyLib::Vector3(0.0f, 150.0f, 0.0f));

	// �ʒu�ݒ�
	SetPosition(pos);

	// ���X�|�[���ݒ�
	ReaspawnSetting();
}

//==========================================================================
// ���X�|�[���ݒ�
//==========================================================================
void CPlayer::ReaspawnSetting()
{
	SetMove(0.0f);

	MyLib::Vector3 pos = GetPosition();
	m_pBaggage->SetPosition(MyLib::Vector3(pos.x, MAX_HEIGHT, pos.z));
}

//==========================================================================
// �����蔻��
//==========================================================================
bool CPlayer::Collision(MyLib::Vector3 &pos, MyLib::Vector3 &move)
{
	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// ���n�������ǂ���
	bool bLand = false;
	float fHeight = 0.0f;
	m_bLandField = false;
	m_bHitWall = false;			// �ǂ̓����蔻��

	// �����擾
	if (m_state != STATE_DEAD && m_state != STATE_RETURN && m_state != STATE_RESTART)
	{
		fHeight = CManager::GetInstance()->GetScene()->GetElevation()->GetHeight(pos, &bLand);
	}
	else
	{
		fHeight = pos.y;
	}

	if (fHeight > pos.y)
	{// �n�ʂ̕���������荂��������

		// �n�ʂ̍����ɕ␳
		pos.y = fHeight;
		m_bLandField = true;

		if (bLand)
		{// ���n���Ă���

			// �W�����v�g�p�\�ɂ���
			m_bJump = false;
			move.y = 0.0f;
			m_bLandOld = true;
		}
	}


	// X�t�@�C���Ƃ̔���
	CStage *pStage = CGame::GetInstance()->GetStage();
	if (pStage == nullptr)
	{// nullptr��������
		return false;
	}

	bool bNowLand = false;

	// �X�e�[�W�ɓ�����������
	m_bHitStage = false;
	for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
	{
		// �I�u�W�F�N�g�擾
		CObjectX *pObjX = pStage->GetObj(nCntStage);

		if (pObjX == nullptr)
		{// nullptr��������
			continue;
		}

		// �����擾
		bool bLand = false;
		fHeight = pObjX->GetHeight(pos, bLand);

		if (bLand && fHeight > pos.y)
		{// �n�ʂ̕���������荂��������

			// �n�ʂ̍����ɕ␳
			if (pos.y + 50.0f <= fHeight)
			{// �������ǂ���������
				m_bHitWall = true;
			}
			else
			{
				pos.y = fHeight;
			}

			m_bHitStage = true;
			m_bLandField = false;

			if (bLand)
			{// ���n���Ă���

				if ((m_sMotionFrag.bKnockBack || m_bJump) && GetPosition().y >= fHeight)
				{
					m_bLandOld = true;
				}

				if (m_bJump)
				{// �W�����v����������

					// ���[�V�����擾
					CMotion* pMotion = GetMotion();
					pMotion->ToggleFinish(true);
				}

				// �W�����v�g�p�\�ɂ���
				m_bJump = false;
				move.y = 0.0f;
				bNowLand = true;
				m_sMotionFrag.bJump = false;
			}
		}
	}

	if (pos.y <= 0.0f)
	{
		pos.y = 0.0f;

		// �W�����v�g�p�\�ɂ���
		m_bJump = false;
		move.y = 0.0f;
		bNowLand = true;
		m_sMotionFrag.bJump = false;
	}

	// �����ݒ�
	SetRotation(rot);

	return bNowLand;
}


//==========================================================================
// �}�b�v�I�u�W�F�N�g�Ƃ̓����蔻��
//==========================================================================
void CPlayer::CollisionMapObject()
{
	// �}�b�v�I�u�W�F�N�g�擾
	CListManager<CObjectX> mapList = CEdit_Map::GetListObj();
	CObjectX* pObj = nullptr;

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posOld = GetOldPosition();
	float radius = GetRadius();

	/*CEffect3D::Create(
		pos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		radius, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);*/
	MyLib::Vector3 crossPos = 0.0f;

	pObj = mapList.GetData(0);
	CObjectX* pObj1 = mapList.GetData(1);

	if (pObj == nullptr || pObj1 == nullptr)return;

	if (UtilFunc::Collision::IsAABBCollidingWithBox(pObj->GetAABB(), pObj->GetWorldMtx(), pObj1->GetAABB(), pObj1->GetWorldMtx()))
	{
		CEffect3D::Create(
			pos,
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			400.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}
}

//==========================================================================
// �q�b�g����
//==========================================================================
MyLib::HitResult_Character CPlayer::Hit(const int nValue)
{
	MyLib::HitResult_Character hitresult = {};

	if (m_state == STATE::STATE_DEAD ||
		m_state == STATE::STATE_DEADWAIT ||
		m_state == STATE::STATE_RETURN ||
		m_state == STATE::STATE_RESTART ||
		m_state == STATE::STATE_RESPAWN)
	{
		hitresult.isdeath = true;
		return hitresult;
	}

	// �̗͎擾
	int nLife = GetLife();

	// �U��
	int camlife = GetLifeOrigin() * 0.75f;
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	if (nLife <= camlife)
	{
		if (nLife == camlife)
		{
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_DROWN);
		}

		if (nLife % 4 == 0)
		{
			float ratioDest = 1.0f - static_cast<float>(nLife) / GetLifeOrigin();
			float ratio = ratioDest;
			UtilFunc::Transformation::Clamp(ratioDest, 0.0f, 0.7f);
			UtilFunc::Transformation::Clamp(ratio, 0.1f, 1.0f);
			pCamera->SetShake(3, 20.0f * ratio, 0.0f);	// �U��
		}

		
		for (int i = 0; i < 2; i++)
		{
			float randmoveX = UtilFunc::Transformation::Random(-50, 50) * 0.01f;
			float randmoveY = UtilFunc::Transformation::Random(-20, 20) * 0.01f;
			float randRadius = UtilFunc::Transformation::Random(-20, 20) * 0.01f;
			float randDestRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;
			float randCycle = UtilFunc::Transformation::Random(-20, 20) * 0.001f;

			// �ړ��������Z
			float len = m_fMoveLength + GetMove().x * 20.0f;
			MyLib::Vector3 setpos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), len);
			int x = UtilFunc::Transformation::Random(-80, 80);
			int z = UtilFunc::Transformation::Random(-80, 80);

			setpos.x += x;
			setpos.z += z;
			setpos.y -= 50.0f;

			CMeshBubble::Create(
				setpos,
				MyLib::Vector3(8.0f + randmoveX, 3.0f + randmoveY, 0.0f),
				1.0f + randRadius,
				10.5f + randDestRadius,
				0.08f + randCycle);
		}

		// �R���g���[���[�U��������
		if (nLife == camlife) 
		{ 
			pPad->SetVibMulti(0.0f); 
		}
		pPad->SetEnableVibration();
		pPad->SetVibMulti(pPad->GetVibMulti() + 0.02f);
		pPad->SetVibration(CInputGamepad::VIBRATION_STATE::VIBRATION_STATE_DMG, 0);
	}

	nLife -= nValue;
	UtilFunc::Transformation::Clamp(nLife, 0, GetLifeOrigin());

	// �̗͐ݒ�
	SetLife(nLife);

	if (nLife <= 0)
	{// �̗͂��Ȃ��Ȃ�����

		// ���S���̐ݒ�
		DeadSetting(&hitresult);
		pCamera->SetLenDest(pCamera->GetOriginDistance(), 1800, 0.0025f, 0.0025f);	// �������߂Â���
		pCamera->SetStateCameraV(new CStateCameraV_Distance);

		// �t�B�[�h�o�b�N�G�t�F�N�gOFF
		CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
			MULTITARGET::DEATH_ALPHA, 
			MULTITARGET::DEATH_MULTI,
			MULTITARGET::DEATH_TIMER);

		// �R���g���[���[�U����~
		pPad->SetEnableVibration();
		pPad->SetVibMulti(0.0f);

		// �^�C�}�[���~
		CTimer* pt = CGame::GetInstance()->GetTimer();
		if (pt != nullptr) 
		{ 
			pt->SetEnableAddTime(false); 
		}
	}
	else if (nLife <= camlife)
	{
		pCamera->SetLenDest(600.0f, 10, -20.0f, 0.07f);	// �������߂Â���
		CStateCameraV_Distance* pstate = new CStateCameraV_Distance;
		pstate->SetStartDistance(pCamera->GetDistance());
		pCamera->SetStateCameraV(pstate);

		// �t�B�[�h�o�b�N�G�t�F�N�gON
		CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
			MULTITARGET::ON_ALPHA,
			MULTITARGET::ON_MULTI,
			MULTITARGET::ON_TIMER);
	}

	// �ߋ��̏�ԕۑ�
	m_Oldstate = m_state;

	// �������������Ԃ�
	return hitresult;
}

//==========================================================================
// ���S���̐ݒ�
//==========================================================================
void CPlayer::DeadSetting(MyLib::HitResult_Character* result)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �����
	m_state = STATE_DEAD;

	// �J�ڃJ�E���^�[�ݒ�
	m_nCntState = DEADTIME;

	// ���ꃂ�[�V����
	GetMotion()->Set(MOTION_KNOCKBACK);

	// �m�b�N�o�b�N�̈ʒu�X�V
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	m_posKnokBack = pos;

	// ����
	result->isdeath = true;

	// ��C�G�t�F�N�g�����Z�b�g����
	if (m_pControlBaggage)
	{
		m_pControlBaggage->EffectStop();
	}
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CPlayer::UpdateState()
{
	// ���[�V�����ʂ̏�Ԑݒ�
	MotionBySetState();

	// �_���[�W��t���ԍX�V
	UpdateDamageReciveTimer();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �_���[�W��t���ԍX�V
//==========================================================================
void CPlayer::UpdateDamageReciveTimer()
{
	// �_���[�W�󂯕t�����Ԍ��Z
	m_sDamageInfo.reciveTime -= CManager::GetInstance()->GetDeltaTime();
	if (m_sDamageInfo.reciveTime <= 0.0f)
	{
		// �X�[�p�[�A�[�}�[���Ȃ����̓_���[�W���[�V�����I��
		if (!m_sDamageInfo.bActiveSuperArmor &&
			!m_sDamageInfo.bReceived)
		{
			// �Ȃɂ��Ȃ���Ԃɂ���
			m_state = STATE_NONE;

			// ���[�V�����擾
			CMotion* pMotion = GetMotion();
			if (pMotion == nullptr)
			{
				return;
			}
			pMotion->ToggleFinish(true);
		}

		if (!m_sDamageInfo.bReceived)
		{

		}

		// �_���[�W�󂯕t������
		m_sDamageInfo.bReceived = true;
		m_sDamageInfo.reciveTime = 0.0f;
	}
}

//==========================================================================
// �Ղ��Ղ�
//==========================================================================
void CPlayer::Bobbing()
{
	CModel* pModel = GetModel(0);
	MyLib::Vector3 pos = pModel->GetPosition();

	static float fff = 0.0f;
	static float cycle = 0.9f;
	static float power = 12.5f;
	float commandheight = 0;

	if (m_pControlTrick != nullptr)
	{
		commandheight = m_pControlTrick->GetHeight();
	}

	fff += CManager::GetInstance()->GetDeltaTime();

	ImGui::DragFloat("Bobbing Cycle", &cycle, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Bobbing", &power, 0.1f, 0.0f, 0.0f, "%.2f");

	pos.y += sinf(D3DX_PI * (fff / cycle)) * power;
	pos.y += commandheight;

	pModel->SetPosition(pos);
}

//==========================================================================
// �Ȃ�
//==========================================================================
void CPlayer::StateNone()
{
	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//==========================================================================
// ���G���
//==========================================================================
void CPlayer::StateInvincible()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	if (m_nCntState % INVINCIBLE_INT == 0)
	{// �K��Ԋu
		
		// �F�ݒ�
		if (m_mMatcol.a == 1.0f)
		{
			m_mMatcol.a = 0.4f;
		}
		else
		{
			m_mMatcol.a = 1.0f;
		}
	}

	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// �Ȃɂ��Ȃ���Ԃɂ���
		m_state = STATE_NONE;
		m_nCntState = 0;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

}

//==========================================================================
// �_���[�W���
//==========================================================================
void CPlayer::StateDamage()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState++;

	// �F�ݒ�
	m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	if (m_nCntState >= TIME_DMG)
	{
		m_state = STATE_INVINCIBLE;
		m_nCntState = INVINCIBLE_TIME;

		// ���[�V�����擾
		CMotion* pMotion = GetMotion();
		if (pMotion == nullptr)
		{
			return;
		}
		pMotion->ToggleFinish(true);
	}
}

//==========================================================================
// ���S���
//==========================================================================
void CPlayer::StateDead()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �ʒu�X�V
	if (m_nCntState > 0)
	{
		int nCnt = DEADTIME - m_nCntState;
		pos.y = (-0.1f * (float)(nCnt * nCnt) + m_KnokBackMove.y * (float)nCnt) + m_posKnokBack.y;
		pos.x += move.x;
	}

	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// �N���Ƃ̔���
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage) && m_nCntState >= 10)
	{// �n�ʂƓ������Ă�����

		//m_state = STATE::STATE_FADEOUT;	// ���S�ҋ@���
		m_nCntState = FADEOUTTIME;
		m_KnokBackMove.y = 0.0f;	// �ړ��ʃ[��
		m_bLandOld = true;
		move.x = 0.0f;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �m�b�N�o�b�N�������
		m_sMotionFrag.bKnockBack = false;
		m_sMotionFrag.bDead = true;
		//pMotion->ToggleFinish(true);

		// �Ԃ��|�ꃂ�[�V����
		GetMotion()->Set(MOTION_DEAD);

		// X�t�@�C���Ƃ̔���
		CStage *pStage = CGame::GetInstance()->GetStage();
		if (pStage == nullptr)
		{// nullptr��������
			return;
		}

		// �X�e�[�W�ɓ�����������
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// �I�u�W�F�N�g�擾
			CObjectX *pObjX = pStage->GetObj(nCntStage);

			if (pObjX == nullptr)
			{// nullptr��������
				continue;
			}

			// �����擾
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}

		// ���g���CUI�Ȃ���ΐ���
		CreateRetryUI();
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);
}

//==========================================================================
// ���S�ҋ@
//==========================================================================
void CPlayer::StateDeadWait()
{
	// �Ԃ��|�ꃂ�[�V����
	GetMotion()->Set(MOTION_DEADWAIT);
}

//==========================================================================
// �`�F�b�N�|�C���g�ɖ߂���
//==========================================================================
void CPlayer::StateReturn()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// �F�ݒ�
	//m_mMatcol.a = (float)m_nCntState / (float)FADEOUTTIME;

	// �Ԃ��|�ꃂ�[�V����
	//GetMotion()->Set(MOTION_DEAD);

	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	if (fadestate == CInstantFade::STATE::STATE_NONE)
	{
		pFade->SetFade(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 40);
	}
	else if (fadestate == CInstantFade::STATE::STATE_FADECOMPLETION)
	{
#if 0
		// ���S����
		Kill();

		// �I������
		Uninit();
#else
		// �`�F�b�N�|�C���g�ɕ���
		ReaspawnCheckPoint();
		m_state = STATE::STATE_RESPAWN;
		SetLife(GetLifeOrigin());

		// ��ʃ��Z�b�g
		ScreenReset();

		// ���g���CUI������
		if (m_pRetryUI != nullptr)
		{
			m_pRetryUI->Uninit();
			m_pRetryUI = nullptr;
		}
#endif
	}

	//if (m_nCntState <= 0)
	//{// �J�ڃJ�E���^�[��0�ɂȂ�����

	//	// ���S����
	//	Kill();

	//	// �I������
	//	Uninit();
	//	return;
	//}
}

//==========================================================================
//�X�^�[�g�ɖ߂���
//==========================================================================
void CPlayer::StateRestart()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// �F�ݒ�
	//m_mMatcol.a = (float)m_nCntState / (float)FADEOUTTIME;

	// �Ԃ��|�ꃂ�[�V����
	//GetMotion()->Set(MOTION_DEAD);

	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	if (fadestate == CInstantFade::STATE::STATE_NONE)
	{
		pFade->SetFade(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 40);
	}
	else if (fadestate == CInstantFade::STATE::STATE_FADECOMPLETION)
	{
#if 0
		// ���S����
		Kill();

		// �I������
		Uninit();
#else
		// �J�n�n�_�ɖ߂�
		m_fMoveLength = 0.0f;
		SetPosition(GetOriginPosition());
		m_state = STATE::STATE_RESPAWN;
		SetLife(GetLifeOrigin());

		// ��ʃ��Z�b�g
		ScreenReset();

		// �J�����u�Ԉړ�
		MyLib::Vector3 pos = GetPosition();
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->WarpCamera(pos + MyLib::Vector3(0.0f, 150.0f, 0.0f));

		// �`�F�b�N�|�C���g�ʉߏ�񃊃Z�b�g
		CCheckpoint::ResetSaveID();

		// ���g���CUI������
		if (m_pRetryUI != nullptr)
		{
			m_pRetryUI->Uninit();
			m_pRetryUI = nullptr;
		}
#endif
	}
}

//==========================================================================
// ���X�|�[��
//==========================================================================
void CPlayer::StateRespawn()
{
	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	if (fadestate == CInstantFade::STATE::STATE_NONE)
	{
		m_state = STATE::STATE_NONE;
	}

	//��C�҂���Ԃɂ���
	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_WAIT_AIRPUSH);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �Q�ƒl�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// �o�b�t�@�ւ̒l�ɑ΂��Ẵ}�X�N�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	// �e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z+�X�e���V������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z+�X�e���V�����s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�̂ݎ��s

	// �X�e���V���`��
	CObjectChara::Draw();

	// �X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ���ʂ̕`��
	if (m_state == STATE_DMG)
	{
		CObjectChara::Draw(m_mMatcol);
	}
	else if (m_state == STATE_INVINCIBLE)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else
	{
		CObjectChara::Draw();
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CPlayer::SetState(STATE state, int nCntState)
{
	m_state = state;
	m_nCntState = nCntState;
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CPlayer::STATE CPlayer::GetState()
{
	return m_state;
}

//==========================================================================
// ��ʃ��Z�b�g
//==========================================================================
void CPlayer::ScreenReset()
{
	// �t�B�[�h�o�b�N�G�t�F�N�g���Z�b�g
	CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
		MULTITARGET::RESET_ALPHA,
		MULTITARGET::RESET_MULTI,
		MULTITARGET::RESET_TIMER);

	// �ו��̃t���O�����Z�b�g����
	if (m_pBaggage != nullptr)
	{
		// ��������Z�b�g����
		m_pBaggage->Reset();
	}

	// �J�����̏�Ԃ����ɖ߂�
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetStateCameraV(new CStateCameraV);

	// ���[�V���������Z�b�g����
	GetMotion()->ToggleFinish(true);
	GetMotion()->Set(MOTION::MOTION_WALK, false);
}

//==========================================================================
// ���g���CUI����
//==========================================================================
void CPlayer::CreateRetryUI()
{
	if (m_pRetryUI == nullptr)
	{
		MyLib::HitResult_Character hitresult = {};
		m_pRetryUI = CRetry_Ui::Create();
		hitresult.isdeath = true;
		DeadSetting(&hitresult);
	}
}