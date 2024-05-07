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
#include "enemy.h"
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

// �g�p�N���X
#include "playercontrol.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* CHARAFILE = "data\\TEXT\\character\\player\\tyuuni\\setup_player.txt";	// �L�����N�^�[�t�@�C��
	const float JUMP = 20.0f * 1.5f;			// �W�����v�͏����l
	const float TIME_DMG = static_cast<float>(20) / static_cast<float>(mylib_const::DEFAULT_FPS);	// �_���[�W����
	const int INVINCIBLE_INT = 2;				// ���G�̊Ԋu
	const int INVINCIBLE_TIME = 0;				// ���G�̎���
	const int DEADTIME = 120;					// ���S���̎���
	const int FADEOUTTIME = 60;					// �t�F�[�h�A�E�g�̎���
	const float MULTIPLIY_DASH = 2.0f;		// �_�b�V���̔{��
	const float TIME_DASHATTACK = 0.3f;			// �_�b�V���U���ɕK�v�Ȏ���
	const int DEFAULT_STAMINA = 200;			// �X�^�~�i�̃f�t�H���g�l
	const float SUBVALUE_DASH = 0.1f;			// �_�b�V���̌��Z��
	const float SUBVALUE_AVOID = 25.0f;			// ����̌��Z��

	const float VELOCITY_SIDESTEP = 12.0f;
	const float GOAL_Z = 0000.0f;
	const float TIME_MAXVELOCITY = 3.0f;	// �ō����ɂȂ�܂ł̎���
	const float TIME_START_VELOCITY = 0.2f;	// �ō����ɂȂ�܂ł̎���
	const float TIME_FLOWERING = 2.0f;		// �J�Ԃ܂ł̎���

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
	const int DEFAULT_RESPAWN_PERCENT = 20;			// �����m��
	const float MULTIPLY_CHARGEATK = 2.0f;				// �`���[�W�U���̔{��
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CPlayer::STATE_FUNC CPlayer::m_StateFunc[] =
{
	&CPlayer::StateNone,		// �Ȃ�
	&CPlayer::StateInvincible,	// ���G
	&CPlayer::StateDamage,		// �_���[�W
	&CPlayer::StateKnockBack,	// �m�b�N�o�b�N
	&CPlayer::StateDown,		// �_�E��
	&CPlayer::StateDead,		// ���S
	&CPlayer::StateDeadWait,	// ���S�ҋ@
	&CPlayer::StateFadeOut,		// �t�F�[�h�A�E�g
	&CPlayer::StateRespawn,		// ���X�|�[��
	&CPlayer::StateCounter,		// �J�E���^�[��
	&CPlayer::StateAvoid,		// ���
	&CPlayer::StatePrayer,		// �F��
	&CPlayer::StateCharge,		// �`���[�W
	&CPlayer::StateFlowering,	// �J��
	&CPlayer::StateAfterFlowering,	// �J��
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
	m_nCntPowerEmission = 0;						// �p���[�A�b�v�̔������J�E���^�[
	m_nComboStage = 0;								// �R���{�̒i�K
	m_nIdxRockOn = 0;								// ���b�N�I���Ώۂ̃C���f�b�N�X�ԍ�
	m_bLockOnAtStart = false;						// �J�E���^�[�J�n���Ƀ��b�N�I�����Ă�����
	m_bReadyDashAtk = false;						// �_�b�V���A�^�b�N�̃t���O
	m_bAttacking = false;							// �U����
	m_bCounterAccepting = false;					// �J�E���^�[��t��
	m_bDash = false;								// �_�b�V������
	m_fDashTime = 0.0f;								// �_�b�V������
	m_fChargeTime = 0.0f;							// �`���[�W����
	m_bChargeCompletion = false;					// �`���[�W�����t���O
	m_nRespawnPercent = 0;							// ���X�|�[���m��
	m_bTouchBeacon = false;							// �r�[�R���ɐG��Ă锻��
	m_bMotionAutoSet = false;						// ���[�V�����̎����ݒ�

	m_PlayerStatus = sPlayerStatus();				// �v���C���[�X�e�[�^�X
	m_sDamageInfo = sDamageInfo();					// �_���[�W���

	m_nMyPlayerIdx = 0;								// �v���C���[�C���f�b�N�X�ԍ�
	m_pShadow = nullptr;								// �e�̏��
	m_pHPGauge = nullptr;							// HP�Q�[�W�̃|�C���^

	m_pControlAtk = nullptr;						// �U������
	m_pControlDefence = nullptr;					// �h�䑀��
	m_pControlAvoid = nullptr;						// ��𑀍�
	m_pGuard = nullptr;								// �K�[�h
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
	m_nRespawnPercent = DEFAULT_RESPAWN_PERCENT;	// ���X�|�[���m��

	// �L�����쐬
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// ���蓖��
	m_List.Regist(this);

	// ����֘A
	ChangeAtkControl(DEBUG_NEW CPlayerControlAttack());
	ChangeDefenceControl(DEBUG_NEW CPlayerControlDefence());
	ChangeAvoidControl(DEBUG_NEW CPlayerControlAvoid());
	ChangeGuardGrade(DEBUG_NEW CPlayerGuard());

	//// �X�L���|�C���g����
	//m_pSkillPoint = CSkillPoint::Create();

	//// HP�Q�[�W����
	//m_pHPGauge = CHP_GaugePlayer::Create(MyLib::Vector3(640.0f, 640.0f, 0.0f), GetLifeOrigin());

	//// �X�^�~�i�Q�[�W����
	//m_pStaminaGauge = CStaminaGauge_Player::Create(MyLib::Vector3(640.0f, 680.0f, 0.0f), DEFAULT_STAMINA);

	return S_OK;
}

//==========================================================================
// �U���̑���ύX
//==========================================================================
void CPlayer::ChangeAtkControl(CPlayerControlAttack* control)
{ 
	delete m_pControlAtk;
	m_pControlAtk = control;
}

//==========================================================================
// �h��̑���ύX
//==========================================================================
void CPlayer::ChangeDefenceControl(CPlayerControlDefence* control)
{ 
	delete m_pControlDefence;
	m_pControlDefence = control;
}

//==========================================================================
// ����̑���ύX
//==========================================================================
void CPlayer::ChangeAvoidControl(CPlayerControlAvoid* control)
{ 
	delete m_pControlAvoid;
	m_pControlAvoid = control;
}

//==========================================================================
// �K�[�h���\�ύX
//==========================================================================
void CPlayer::ChangeGuardGrade(CPlayerGuard* guard)
{
	delete m_pGuard;
	m_pGuard = guard;
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
	if (m_pControlAtk != nullptr)
	{
		delete m_pControlAtk;
		m_pControlAtk = nullptr;
	}
	if (m_pControlDefence != nullptr)
	{
		delete m_pControlDefence;
		m_pControlDefence = nullptr;
	}
	if (m_pControlAvoid != nullptr)
	{
		delete m_pControlAvoid;
		m_pControlAvoid = nullptr;
	}
	if (m_pGuard != nullptr)
	{
		delete m_pGuard;
		m_pGuard = nullptr;
	}

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
	//// �J�ڂȂ��t�F�[�h�ǉ�
	//CManager::GetInstance()->GetInstantFade()->SetFade(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 40);

	//// �J�ڏ�ԂɕύX
	//CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SCENE_REASPAWN);

	CManager::GetInstance()->GetFade()->SetFade(CScene::MODE::MODE_RANKING);

	my_particle::Create(GetPosition(), my_particle::TYPE_ENEMY_FADE);

	// �e������
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// ����n
	if (m_pControlAtk != nullptr)
	{
		delete m_pControlAtk;
		m_pControlAtk = nullptr;
	}
	if (m_pControlDefence != nullptr)
	{
		delete m_pControlDefence;
		m_pControlDefence = nullptr;
	}
	if (m_pControlAvoid != nullptr)
	{
		delete m_pControlAvoid;
		m_pControlAvoid = nullptr;
	}
	if (m_pGuard != nullptr)
	{
		delete m_pGuard;
		m_pGuard = nullptr;
	}

	// ���b�N�I���ݒ�
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->SetRockOn(0.0f, false);
	}

	// ���X�g���[�v
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;
	while (enemyList.ListLoop(&pEnemy))
	{
		pEnemy->SetEnableRockOn(false);
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
	MotionSet();

	// ��ԍX�V
	UpdateState();

	if (CGame::GetInstance()->GetGameManager()->GetType() == CGameManager::SceneType::SCENE_MAIN)
	{
		//�ԕ���ӂ�ӏ���
		my_particle::Create(GetPosition() + MyLib::Vector3(0.0f, 70.0f, 0.0f), my_particle::TYPE_POLLENDROP);
	}

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

#if _DEBUG

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[�v���C���[�̑���]------------------\n"
		"�ʒu�F�yX�F%f, Y�F%f, Z�F%f�z �yW / A / S / D�z\n"
		"�����F�yX�F%f, Y�F%f, Z�F%f�z �yZ / C�z\n"
		"�ړ��ʁF�yX�F%f, Y�F%f, Z�F%f�z\n"
		"�̗́F�y%d�z\n"
		"��ԁF�y%d�z\n"
		"�R���{�X�e�[�W�F�y%d�z\n"
		"�_�b�V�����ԁF�y%f�z\n"
		, pos.x, pos.y, pos.z, rot.x, rot.y, rot.y, move.x, move.y, move.z, GetLife(), m_state, m_nComboStage, m_fDashTime);

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

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// �J�����̌����擾
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 newPosition = GetPosition();
	MyLib::Vector3 sakiPos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// ���݂̎�ގ擾
	CMotion* pMotion = GetMotion();
	int nMotionType = pMotion->GetType();

	// �ړ��ʎ擾
	float fMove = GetVelocity();

	// �o�ߎ��Ԏ擾
	float fCurrentTime = CManager::GetInstance()->GetDeltaTime();

	if (CGame::GetInstance()->GetGameManager()->IsControll())
	{// �s���ł���Ƃ�

		// ����֐�
		if (m_state != STATE_KNOCKBACK &&
			m_state != STATE_DEAD &&
			m_state != STATE_DEADWAIT &&
			m_state != STATE_FADEOUT)
		{
			m_pControlAtk->Attack(this);		// �U������
			m_pControlDefence->Defence(this);	// �h�䑀��
			m_pControlAvoid->Avoid(this);		// ��𑀍�
		}
		nMotionType = pMotion->GetType();
		fRotDest = GetRotDest();

		// �_�b�V������
		if (pInputGamepad->GetPress(CInputGamepad::BUTTON_LB, m_nMyPlayerIdx) &&
			pInputGamepad->IsTipStick())
		{// ���X�e�B�b�N���|��Ă�ꍇ
			m_bDash = true;
		}
		else
		{
			m_bDash = false;
		}

		if (m_bDash)
		{
			// �_�b�V���{���|����
			fMove *= MULTIPLIY_DASH;
		}

		if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
			!m_pControlAtk->IsReserve() &&
			//!m_sMotionFrag.bATK &&
			m_state != STATE_KNOCKBACK &&
			m_state != STATE_DEAD &&
			m_state != STATE_DEADWAIT &&
			m_state != STATE_FADEOUT)
		{// �ړ��\���[�V�����̎�

			if (pInputKeyboard->GetPress(DIK_A))
			{//���L�[�������ꂽ,���ړ�

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;

				if (pInputKeyboard->GetPress(DIK_W))
				{//A+W,����ړ�

					move.x += sinf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
					move.z += cosf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
					fRotDest = D3DX_PI * 0.75f + Camerarot.y;
				}
				else if (pInputKeyboard->GetPress(DIK_S))
				{//A+S,�����ړ�

					move.x += sinf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
					move.z += cosf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
					fRotDest = D3DX_PI * 0.25f + Camerarot.y;
				}
				else
				{//A,���ړ�

					move.x += sinf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
					move.z += cosf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
					fRotDest = D3DX_PI * 0.5f + Camerarot.y;
				}
			}
			else if (pInputKeyboard->GetPress(DIK_D))
			{//D�L�[�������ꂽ,�E�ړ�

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;

				if (pInputKeyboard->GetPress(DIK_W))
				{//D+W,�E��ړ�

					move.x += sinf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
					move.z += cosf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
					fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
				}
				else if (pInputKeyboard->GetPress(DIK_S))
				{//D+S,�E���ړ�

					move.x += sinf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
					move.z += cosf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
					fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
				}
				else
				{//D,�E�ړ�

					move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
					move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
					fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
				}
			}
			else if (pInputKeyboard->GetPress(DIK_W))
			{//W�������ꂽ�A��ړ�

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;
				move.x += sinf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 1.0f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//S�������ꂽ�A���ړ�

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;
				move.x += sinf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.0f + Camerarot.y;
			}
			else
			{
				// �ړ������ǂ���
				m_sMotionFrag.bMove = false;
			}

			if (pInputGamepad->IsTipStick())
			{// ���X�e�B�b�N���|��Ă�ꍇ

				// �ړ����ɂ���
				m_sMotionFrag.bMove = true;

				// �X�e�B�b�N�̌����擾
				float stickrot = pInputGamepad->GetStickRotL(m_nMyPlayerIdx);
				UtilFunc::Transformation::RotNormalize(stickrot);

				// �ړ��ʂƌ����ݒ�
				move.x += sinf(stickrot + Camerarot.y) * fMove;
				move.z += cosf(stickrot + Camerarot.y) * fMove;
				fRotDest = D3DX_PI + stickrot + Camerarot.y;
			}

			if (m_sMotionFrag.bMove &&
				!m_bJump)
			{// �L�����Z���\ && �ړ���

				pMotion->ToggleFinish(true);

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

			if (m_bJump == false &&
				(pInputKeyboard->GetTrigger(DIK_SPACE) ||
					pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, m_nMyPlayerIdx)) &&
				!m_bTouchBeacon)
			{// �W�����v

				m_bJump = true;
				m_sMotionFrag.bJump = true;
				move.y += 17.0f;

				pMotion->Set(MOTION_JUMP);

				// �T�E���h�Đ�
				CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_JUMP);
			}
		}
		else if (pMotion->IsGetMove(nMotionType) == 0 &&
			m_state != STATE_DEAD &&
			m_state != STATE_FADEOUT)
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

		// �p�x�̐��K��
		UtilFunc::Transformation::RotNormalize(fRotDest);
		SetRotDest(fRotDest);
	}

	// �ړ��ʉ��Z
	newPosition.x += move.x;
	newPosition.z += move.z;

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
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DEAD && m_state != STATE_FADEOUT && m_state != STATE_DEADWAIT)
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
	else if (m_state != STATE_KNOCKBACK && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
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

	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_RSTICKPUSH, m_nMyPlayerIdx))
	{
		if (CManager::GetInstance()->GetCamera()->IsRockOn())
		{// ���b�N�I������

			// ���b�N�I���ݒ�
			CManager::GetInstance()->GetCamera()->SetRockOn(0.0f, false);

			// ���X�g���[�v
			CListManager<CEnemy> enemyList = CEnemy::GetListObj();
			CEnemy* pEnemy = nullptr;
			while (enemyList.ListLoop(&pEnemy))
			{
				pEnemy->SetEnableRockOn(false);
			}
		}
		else
		{
			// ���b�N�I������
			RockOn();
		}
	}

	// ���X�g���[�v
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;
	while (enemyList.ListLoop(&pEnemy))
	{
		if (CGame::GetInstance()->GetRockOnDistance() <= UtilFunc::Calculation::GetPosLength3D(pos, pEnemy->GetPosition()))
		{
			if (pEnemy->IsRockOnAccept())
			{
				// ���b�N�I���ݒ�
				CManager::GetInstance()->GetCamera()->SetRockOn(0.0f, false);
			}

			pEnemy->SetEnableRockOn(false);
		}
	}

	////if (m_pWeaponHandle != nullptr)
	//{
	//	// ����̈ʒu
	//	MyLib::Vector3 weponpos = UtilFunc::Transformation::WorldMtxChangeToPosition(GetModel()[16]->GetWorldMtx());

	//	// ����̃}�g���b�N�X�擾
	//	D3DXMATRIX weaponWorldMatrix = GetModel()[16]->GetWorldMtx();

	//	// �O�Ղ̃}�g���b�N�X�ݒ�
	//	CMyEffekseer::GetInstance()->SetMatrix(m_WeaponHandle, weaponWorldMatrix);
	//	CMyEffekseer::GetInstance()->SetPosition(m_WeaponHandle, weponpos);
	//}

	// �f�o�b�O�p
#if _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_LEFT))
	{
		CCollisionObject::Create(GetPosition(), mylib_const::DEFAULT_VECTOR3, 100000.0f, 3, 10000, CCollisionObject::TAG_PLAYER);
	}

	if (pInputKeyboard->GetRepeat(DIK_RIGHT, 4)){
		//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_NORMALATK_HIT2);
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_COUNTER_TURN, false);

		CPlayer::Hit(10000, CGameManager::AttackType::ATTACK_NORMAL);
	}

	static float fff = 1.0f;
	if (pInputKeyboard->GetTrigger(DIK_UP))
	{
		fff += 0.1f;
		CManager::GetInstance()->GetSound()->SetFrequency(CSound::LABEL_BGM_GAME, fff);

		MyLib::Vector3 weponpos = 0.0f;
		weponpos.y += 150.0f;
		weponpos.z += 800.0f;
		CMyEffekseer::GetInstance()->SetEffect(
			CMyEffekseer::EFKLABEL_COUNTERLINE2,
			weponpos, 0.0f, 0.0f, 60.0f);

	}
	if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		fff -= 0.1f;
		CManager::GetInstance()->GetSound()->SetFrequency(CSound::LABEL_BGM_GAME, fff);
	}

	D3DXMATRIX adasf;
	D3DXMATRIX adaaaaasf(adasf);

	MyLib::Matrix mmmmmmm;
	MyLib::Matrix mmmmmmfafm(mmmmmmm);

	if (pInputKeyboard->GetPress(DIK_J))
	{
		CDamagePoint::Create(GetPosition(), UtilFunc::Transformation::Random(1, 99));
	}
#endif
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
		m_state == STATE_DOWN ||
		m_state == STATE_CHARGE ||
		m_state == STATE_FADEOUT ||
		m_state == STATE_KNOCKBACK ||
		m_state == STATE_PRAYER ||
		m_state == STATE_FLOWERING)
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

	// �_�b�V���A�^�b�N�̃t���O
	if (m_fDashTime >= TIME_DASHATTACK)
	{
		m_bReadyDashAtk = true;
	}
	else
	{
		m_bReadyDashAtk = false;
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
	m_bCounterAccepting = false;	// �J�E���^�[��t��
}

//==========================================================================
// ���b�N�I��
//==========================================================================
void CPlayer::RockOn()
{
	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �G�̃��X�g�擾
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;

	float fNearLen = CGame::GetInstance()->GetRockOnDistance();
	int nMaxIdx = 0;
	MyLib::Vector3 targetpos(0.0f);

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// ���X�g���[�v
	int i = 0;
	MyLib::Vector3 enemypos(0.0f);
	while (enemyList.ListLoop(&pEnemy))
	{
		// �G�̈ʒu�擾
		enemypos = pEnemy->GetPosition();

		if (pCamera->IsOnScreen(enemypos))
		{
			float len = UtilFunc::Calculation::GetPosLength3D(pos, enemypos);
			if (fNearLen > len)
			{
				fNearLen = len;
				nMaxIdx = i;
			}
		}

		// �C���f�b�N�X���Z
		i++;
	}

	if (fNearLen < CGame::GetInstance()->GetRockOnDistance())
	{// ���b�N�I���������Ȃ�

		// ���b�N�I���ݒ�
		//pCamera->SetRockOn(enemyList.GetData(nMaxIdx)->GetPosition(), true);
		enemyList.GetData(nMaxIdx)->SetEnableRockOn(true);

		// �C���f�b�N�X�ԍ��ݒ�
		m_nIdxRockOn = nMaxIdx;
	}
}

//==========================================================================
// ���b�N�I���Ώې؂�ւ�
//==========================================================================
void CPlayer::SwitchRockOnTarget()
{
	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �Q�[���p�b�h���擾
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// �G�̃��X�g�擾
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;

	bool bSwitch = true;
	if (pInputGamepad->GetRStickTrigger(CInputGamepad::STICK_X))
	{// ���E�ǂ��炩�ɐ؂�ւ�

		bool bSwitch = true;

		// ���X�g���[�v
		int i = 0, nMaxIdx = m_nIdxRockOn;
		float fNearLen = CGame::GetInstance()->GetRockOnDistance();
		pEnemy = nullptr;
		MyLib::Vector3 enemypos(0.0f);
		while (enemyList.ListLoop(&pEnemy))
		{
			// �G�̈ʒu�擾
			enemypos = pEnemy->GetPosition();

			if (pCamera->IsOnScreen(enemypos))
			{
				float len = UtilFunc::Calculation::GetPosLength3D(pos, enemypos);
				if (fNearLen > len &&
					m_nIdxRockOn != i)
				{
					fNearLen = len;
					nMaxIdx = i;
				}
			}

			// �C���f�b�N�X���Z
			i++;
		}

		if (enemyList.GetData(m_nIdxRockOn) != nullptr)
		{
			// ���܂Ń��b�N�I�����Ă��Ώۃ��Z�b�g
			enemyList.GetData(m_nIdxRockOn)->SetEnableRockOn(false);
		}

		// ���b�N�I���ݒ�
		CEnemy* pSetEnemy = enemyList.GetData(nMaxIdx);
		//CManager::GetInstance()->GetCamera()->SetRockOn(pSetEnemy->GetPosition(), true);
		pSetEnemy->SetEnableRockOn(true);

		// �C���f�b�N�X�ԍ��ݒ�
		m_nIdxRockOn = nMaxIdx;
	}
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
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_WALK1);
		}
		else{

			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_WALK2);
		}*/
		break;

	case MOTION::MOTION_DASH:
		/*if (nCntATK == 0)
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_DASH1);
		}
		else {

			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_DASH2);
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

	// �G�̃��X�g�擾
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;

	// ���X�g���[�v
	while (enemyList.ListLoop(&pEnemy))
	{
		// �R���C�_�[�̐��J��Ԃ�
		std::vector<SphereCollider> colliders = pEnemy->GetSphereColliders();
		for (const auto& collider : colliders)
		{
			MyLib::HitResult hitresult = UtilFunc::Collision::SphereRange(weponpos, collider.center, pATKInfo->fRangeSize, collider.radius);
			if (hitresult.ishit)
			{// ���̔���

				int damage = static_cast<int>(static_cast<float>(pATKInfo->nDamage) * m_PlayerStatus.attackMultiply);

				

				if (pEnemy->Hit(damage, GetPosition()))
				{// �������Ă���

					pATKInfo->bEndAtk = true;

					// �ʒu
					MyLib::Vector3 pos = GetPosition();
					MyLib::Vector3 enemypos = pEnemy->GetPosition();

					if (!pEnemy->IsActiveSuperArmor())
					{
						// �^�[�Q�b�g�ƓG�Ƃ̌���
						float fRot = enemypos.AngleXZ(pos);
						UtilFunc::Transformation::RotNormalize(fRot);

						pEnemy->SetMove(MyLib::Vector3(sinf(fRot) * 8.0f, 0.0f, cosf(fRot) * 8.0f));
					}

					CMyEffekseer::GetInstance()->SetEffect(
						CMyEffekseer::EFKLABEL_HITMARK_RED,
						hitresult.hitpos, 0.0f, 0.0f, 50.0f);

					// �_���[�W�\�L
					enemypos.y += pEnemy->GetHeight() * 0.5f;
					enemypos += UtilFunc::Transformation::GetRandomPositionSphere(enemypos, collider.radius * 0.5f);

					
					break;
				}
			}
		}
	}
}

//==========================================================================
// �ʒu����
//==========================================================================
void CPlayer::LimitPos()
{
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

	//CollisionMapObject();
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
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
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

	// ���X�g���[�v
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;
	MyLib::Vector3 enemypos;
	float radius = GetRadius();
	float enemyradius = 0.0f;

	while (enemyList.ListLoop(&pEnemy))
	{
		enemypos = pEnemy->GetCenterPosition();
		enemyradius = pEnemy->GetRadius();
		enemyradius *= 0.5f;

		if (UtilFunc::Collision::CircleRange3D(pos, enemypos, radius, enemyradius))
		{
			// �^�[�Q�b�g�ƓG�Ƃ̌���
			float fRot = atan2f((pos.x - enemypos.x), (pos.z - enemypos.z));
			float totalradius = radius + enemyradius;

			pos.x = enemypos.x + sinf(fRot) * totalradius;
			pos.z = enemypos.z + cosf(fRot) * totalradius;
		}
	}

	// �����ݒ�
	SetRotation(rot);

	return bNowLand;
}


// ����AABB�̓����蔻��
bool SphereAABBCollision(const D3DXVECTOR3& sphereCenter, float sphereRadius, const D3DXVECTOR3& boxMin, const D3DXVECTOR3& boxMax)
{
	// ���̒��S��AABB�̓����ɃN�����v����
	D3DXVECTOR3 closestPoint;
	closestPoint.x = max(boxMin.x, min(sphereCenter.x, boxMax.x));
	closestPoint.y = max(boxMin.y, min(sphereCenter.y, boxMax.y));
	closestPoint.z = max(boxMin.z, min(sphereCenter.z, boxMax.z));

	// ���̒��S��AABB�̍ł��߂��_�̋������v�Z����
	D3DXVECTOR3 aaaaaaaaaaaa = (sphereCenter - closestPoint);
	float distanceSquared = D3DXVec3LengthSq(&aaaaaaaaaaaa);

	// ���̔��a��2��Ɣ�r����
	return distanceSquared <= (sphereRadius * sphereRadius);
}

// ��]�s���K�p����֐�
void TransformBoundingBox(const D3DXVECTOR3& boxMin, const D3DXVECTOR3& boxMax, const D3DXMATRIX& rotationMatrix, D3DXVECTOR3& transformedMin, D3DXVECTOR3& transformedMax)
{
	D3DXVECTOR3 corners[8] = {
		D3DXVECTOR3(boxMin.x, boxMin.y, boxMin.z),
		D3DXVECTOR3(boxMax.x, boxMin.y, boxMin.z),
		D3DXVECTOR3(boxMin.x, boxMax.y, boxMin.z),
		D3DXVECTOR3(boxMax.x, boxMax.y, boxMin.z),
		D3DXVECTOR3(boxMin.x, boxMin.y, boxMax.z),
		D3DXVECTOR3(boxMax.x, boxMin.y, boxMax.z),
		D3DXVECTOR3(boxMin.x, boxMax.y, boxMax.z),
		D3DXVECTOR3(boxMax.x, boxMax.y, boxMax.z)
	};

	// �e���_����]�s��ŕϊ�����
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&corners[i], &corners[i], &rotationMatrix);
	}

	// �ϊ���̒��_����V����AABB�̍ŏ��l�ƍő�l���v�Z����
	transformedMin = transformedMax = corners[0];
	for (int i = 1; i < 8; ++i)
	{
		transformedMin = D3DXVECTOR3(min(transformedMin.x, corners[i].x),
			min(transformedMin.y, corners[i].y),
			min(transformedMin.z, corners[i].z));

		transformedMax = D3DXVECTOR3(max(transformedMax.x, corners[i].x),
			max(transformedMax.y, corners[i].y),
			max(transformedMax.z, corners[i].z));
	}
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
	float radius = GetRadius();

	CEffect3D::Create(
		pos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		radius, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	while (mapList.ListLoop(&pObj))
	{
		MyLib::Vector3 crossPos = 0.0f;
		if (UtilFunc::Collision::CollisionCircleToAABB(pos, radius, pObj->GetAABB(), pObj->GetWorldMtx()))
		{
			//pos = crossPos;

			MyLib::Vector3 move = GetMove();
			move.x = 0.0f, move.z = 0.0f;
			SetMove(move);
			int n = 0;
		}

		/*if (UtilFunc::Collision::CircleAABBIntersect(pos, radius, pObj->GetWorldMtx(), pObj->GetAABB(), crossPos)) {
			pos += crossPos;

		}*/
	}



	//CObjectX* pppp = mapList.GetData(0);

	//// ����AABB�̃p�����[�^
	//D3DXVECTOR3 boxMin = pppp->GetAABB().vtxMin;
	//D3DXVECTOR3 boxMax = pppp->GetAABB().vtxMax;

	//MyLib::Vector3 rot = pppp->GetRotation();

	//// AABB�̉�]�s��
	//D3DXMATRIX rotationMatrix;
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rot.y, rot.x, rot.z);

	//// ��]�s���K�p����AABB�̍ŏ��l�ƍő�l���v�Z����
	//D3DXVECTOR3 transformedMin, transformedMax;
	//TransformBoundingBox(boxMin, boxMax, rotationMatrix, transformedMin, transformedMax);

	//// ���Ɖ�]����AABB�̓����蔻��
	//bool collision = SphereAABBCollision(pos, radius, transformedMin, transformedMax);

	//if (collision)
	//{
	//	// �Փ˂����ꍇ�̏���

	//	// AABB�̊e�ӂɑ΂��鋅�̒��S����̋������v�Z
	//	float distX = max(transformedMin.x - pos.x, pos.x - transformedMax.x);
	//	float distY = max(transformedMin.y - pos.y, pos.y - transformedMax.y);
	//	float distZ = max(transformedMin.z - pos.z, pos.z - transformedMax.z);

	//	// �����߂��x�N�g���̏�����
	//	D3DXVECTOR3 pushBackVector(0.0f, 0.0f, 0.0f);

	//	// ����AABB�̊O���ɂ߂荞��ł���ӂ���肵�A�Ή���������ɉ����߂�
	//	if (distX < distY && distX < distZ) {
	//		// X�������ɉ����߂�
	//		pushBackVector.x = (distX + radius) * (pos.x < transformedMin.x ? 1 : -1);
	//	}
	//	else if (distY < distX && distY < distZ) {
	//		// Y�������ɉ����߂�
	//		pushBackVector.y = (distY + radius) * (pos.y < transformedMin.y ? 1 : -1);
	//	}
	//	else {
	//		// Z�������ɉ����߂�
	//		pushBackVector.z = (distZ + radius) * (pos.z < transformedMin.z ? 1 : -1);
	//	}

	//	// ���������߂�
	//	pos += pushBackVector;
	//}














	SetPosition(pos);
}

//==========================================================================
// �q�b�g����
//==========================================================================
MyLib::HitResult_Character CPlayer::Hit(const int nValue, CGameManager::AttackType atkType)
{
	MyLib::HitResult_Character hitresult = {};

	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	
	
	// ���ʂ̃q�b�g����
	hitresult = ProcessHit(nValue, 0.0f);

	// �������������Ԃ�
	return hitresult;
}

//==========================================================================
// �q�b�g����
//==========================================================================
MyLib::HitResult_Character CPlayer::Hit(const int nValue, CEnemy* pEnemy, CGameManager::AttackType atkType)
{

	MyLib::HitResult_Character hitresult = {};


	// ���ʂ̃q�b�g����
	hitresult = ProcessHit(nValue, pEnemy->GetPosition());

	return hitresult;
}

//==========================================================================
// ���ʂ̃q�b�g����
//==========================================================================
MyLib::HitResult_Character CPlayer::ProcessHit(const int nValue, const MyLib::Vector3& hitpos)
{

	MyLib::HitResult_Character hitresult = {};

	// �̗͎擾
	int nLife = GetLife();

	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (!m_sDamageInfo.bReceived)
	{// �_���[�W��t���̂�
		return hitresult;
	}

	if (m_state == STATE_COUNTER ||
		m_state == STATE_AVOID)
	{// �_���[�W�󂯂Ȃ����
		return hitresult;
	}

	if (m_state != STATE_DMG &&
		m_state != STATE_KNOCKBACK &&
		m_state != STATE_INVINCIBLE &&
		m_state != STATE_DEAD &&
		m_state != STATE_DEADWAIT &&
		m_state != STATE_RESPAWN &&
		m_state != STATE_FADEOUT)
	{// �_���[�W��t��Ԃ̎�

		// ��������
		hitresult.ishit = true;
		hitresult.isdamage = true;

		// �̗͌��炷
		nLife -= nValue;

		// �_�b�V������OFF
		m_bDash = false;

		//�ԕ��΂�������
		my_particle::Create(GetPosition() + MyLib::Vector3(0.0f, 100.0f, 0.0f), my_particle::TYPE_POLLENLOST);

		// �Q�[���p�b�h���擾
		CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
		pInputGamepad->SetVibration(CInputGamepad::VIBRATION_STATE_DMG, 0);

		// �m�b�N�o�b�N���鎞
		//m_KnokBackMove.y += 18.0f;
		m_bHitStage = false;

		// �̗͐ݒ�
		SetLife(nLife);

		if (nLife <= 0)
		{// �̗͂��Ȃ��Ȃ�����

			// �m�b�N�o�b�N����ɂ���
			m_sMotionFrag.bKnockBack = true;

			// �m�b�N�o�b�N��Ԃɂ���
			m_state = STATE_KNOCKBACK;

			DeadSetting(&hitresult);

			// �_���[�W��
			//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_PLAYERDMG_STRONG);
			return hitresult;
		}

		// �ߋ��̏�ԕۑ�
		m_Oldstate = m_state;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// �m�b�N�o�b�N�̈ʒu�X�V
		MyLib::Vector3 pos = GetPosition();
		m_posKnokBack = pos;
		
		float hitAngle = pos.AngleXZ(hitpos);

		// �Ռ��g����
		CImpactWave::Create
		(
			MyLib::Vector3(pos.x, pos.y + 80.0f, pos.z),	// �ʒu
			MyLib::Vector3(D3DX_PI * 0.5f, D3DX_PI + hitAngle, D3DX_PI),				// ����
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// �F
			80.0f,										// ��
			80.0f,										// ����
			0.0f,										// ���S����̊Ԋu
			20,											// ����
			10.0f,										// ���̈ړ���
			CImpactWave::TYPE_GIZAWHITE,				// �e�N�X�`���^�C�v
			false										// ���Z�������邩
		);


		{

			m_sDamageInfo.reciveTime = TIME_DMG;
			m_sDamageInfo.bReceived = false;

			MyLib::Vector3 move;
			move.x = sinf(D3DX_PI + hitAngle) * -60.0f;
			move.z = cosf(D3DX_PI + hitAngle) * -60.0f;
			SetMove(move);

			// ���ꃂ�[�V����
			if (!m_sDamageInfo.bActiveSuperArmor)
			{
				// �_���[�W��Ԃɂ���
				m_state = STATE_DMG;

				GetMotion()->Set(MOTION_DMG);

				// �_���[�W��
				CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_PLAYERDMG_NORMAL);
				CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_WINGS);
				CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_BOOST);
			}
		}

		// �U��
		pCamera->SetShake(8, 18.0f, 0.0f);
	}

	return hitresult;
}

//==========================================================================
// �_�E�����̐ݒ�
//==========================================================================
void CPlayer::DownSetting(const MyLib::Vector3& hitpos)
{
	// �m�b�N�o�b�N����ɂ���
	m_sMotionFrag.bKnockBack = true;

	// �m�b�N�o�b�N��Ԃɂ���
	m_state = STATE_KNOCKBACK;


	m_KnokBackMove.y = 10.0f;


	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʐݒ�
	float hitAngle = pos.AngleXZ(hitpos);
	MyLib::Vector3 move;
	move.x = sinf(D3DX_PI + hitAngle) * -7.0f;
	move.z = cosf(D3DX_PI + hitAngle) * -7.0f;
	SetMove(move);

	// �m�b�N�o�b�N���[�V����
	GetMotion()->Set(MOTION_KNOCKBACK);

	// �U��
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->SetShake(18, 25.0f, 0.0f);
	}
}

//==========================================================================
// ���S���̐ݒ�
//==========================================================================
void CPlayer::DeadSetting(MyLib::HitResult_Character* result)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �����
	m_state = STATE_DEAD;

	m_KnokBackMove.y = 8.0f;

	// �J�ڃJ�E���^�[�ݒ�
	m_nCntState = DEADTIME;

	// �̗͐ݒ�
	SetLife(0);

	// �m�b�N�o�b�N����ɂ���
	m_sMotionFrag.bKnockBack = true;

	// ���ꃂ�[�V����
	GetMotion()->Set(MOTION_KNOCKBACK);

	// �m�b�N�o�b�N�̈ʒu�X�V
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	m_posKnokBack = pos;

	// �Ռ��g����
	CImpactWave::Create
	(
		MyLib::Vector3(pos.x, pos.y + 80.0f, pos.z),	// �ʒu
		MyLib::Vector3(D3DX_PI * 0.5f, D3DX_PI + rot.y, D3DX_PI),				// ����
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// �F
		80.0f,										// ��
		80.0f,										// ����
		0.0f,										// ���S����̊Ԋu
		20,											// ����
		10.0f,										// ���̈ړ���
		CImpactWave::TYPE_GIZAWHITE,				// �e�N�X�`���^�C�v
		false										// ���Z�������邩
	);

	CManager::GetInstance()->SetEnableHitStop(18);

	// �U��
	pCamera->SetShake(21, 30.0f, 0.0f);

	// ����
	result->isdeath = true;
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
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_WINGS);
		}

		// �_���[�W�󂯕t������
		m_sDamageInfo.bReceived = true;
		m_sDamageInfo.reciveTime = 0.0f;
	}
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
	//// ��ԑJ�ڃJ�E���^�[���Z
	//m_nCntState++;

	//// �F�ݒ�
	//m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//if (m_nCntState >= TIME_DMG)
	//{
	//	m_state = STATE_INVINCIBLE;
	//	m_nCntState = INVINCIBLE_TIME;

	//	// ���[�V�����擾
	//	CMotion* pMotion = GetMotion();
	//	if (pMotion == nullptr)
	//	{
	//		return;
	//	}
	//	pMotion->ToggleFinish(true);
	//}
}

//==========================================================================
// �m�b�N�o�b�N
//==========================================================================
void CPlayer::StateKnockBack()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �ڕW�̌����擾
	float fRotDest = GetRotDest();

	// �ړ��ʎ擾
	float fMove = GetVelocity();

	// ���݂ƖڕW�̍���
	float fRotDiff = 0.0f;

	// �����̔���
	bool bLen = false;

	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int nType = pMotion->GetType();

	if (nType == MOTION_KNOCKBACK)
	{
		// ��ԑJ�ڃJ�E���^�[���Z
		m_nCntState++;

		// �ʒu�X�V
		pos.y = (-0.3f * (float)(m_nCntState * m_nCntState) + m_KnokBackMove.y * (float)m_nCntState) + m_posKnokBack.y;
		pos.x += move.x;
		pos.z += move.z;
	}

	// �N���Ƃ̔���
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage))
	{// �n�ʂƓ������Ă�����

		/*m_state = STATE_INVINCIBLE;
		m_nCntState = INVINCIBLE_TIME;*/
		m_KnokBackMove.y = 0.0f;	// �ړ��ʃ[��
		m_bLandOld = true;
		pos.y = 0.0f;

		// �F�ݒ�
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �m�b�N�o�b�N�������
		m_sMotionFrag.bKnockBack = false;

		pMotion->Set(MOTION_KNOCKBACK_PASSIVE);

		// X�t�@�C���Ƃ̔���
		CStage* pStage = CGame::GetInstance()->GetStage();
		if (pStage == nullptr)
		{// nullptr��������
			return;
		}

		// �X�e�[�W�ɓ�����������
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// �I�u�W�F�N�g�擾
			CObjectX* pObjX = pStage->GetObj(nCntStage);

			if (pObjX == nullptr)
			{// nullptr��������
				continue;
			}

			// �����擾
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}
	}

	// �ʒu�ݒ�
	SetPosition(pos);

	// �ړ��ʐݒ�
	SetMove(move);

	// �����ݒ�
	SetRotation(rot);

	// �ڕW�̌����ݒ�
	SetRotDest(fRotDest);


	if (nType == MOTION_KNOCKBACK_PASSIVE &&
		pMotion->IsFinish())
	{// �󂯐g���m�b�N�o�b�N���I��

		m_state = STATE_DOWN;
		return;
	}
}

//==========================================================================
// �_�E��
//==========================================================================
void CPlayer::StateDown()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(MOTION_DOWN);

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

		m_state = STATE_DEADWAIT;	// ���S�ҋ@���
		m_state = STATE::STATE_FADEOUT;	// ���S�ҋ@���
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
		//pMotion->Set(MOTION_DEAD);

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
// �t�F�[�h�A�E�g���
//==========================================================================
void CPlayer::StateFadeOut()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_nCntState--;

	// �F�ݒ�
	m_mMatcol.a = (float)m_nCntState / (float)FADEOUTTIME;

	// �Ԃ��|�ꃂ�[�V����
	GetMotion()->Set(MOTION_DEAD);

	if (m_nCntState <= 0)
	{// �J�ڃJ�E���^�[��0�ɂȂ�����

		// ���S����
		Kill();

		// �I������
		Uninit();
		return;
	}
}

//==========================================================================
// ���X�|�[��
//==========================================================================
void CPlayer::StateRespawn()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	
}

//==========================================================================
// �J�E���^�[���
//==========================================================================
void CPlayer::StateCounter()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	CEnemy* pEnemy = CEnemy::GetListObj().GetData(m_nIdxRockOn);
	if (pEnemy == nullptr)
	{
		return;
	}

	MyLib::Vector3 enemypos = pEnemy->GetPosition();
	SetRotDest(pos.AngleXZ(enemypos));

	
}

//==========================================================================
// ���
//==========================================================================
void CPlayer::StateAvoid()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	
}

//==========================================================================
// �F��
//==========================================================================
void CPlayer::StatePrayer()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	
}

//==========================================================================
// �`���[�W
//==========================================================================
void CPlayer::StateCharge()
{
	

	
}

//==========================================================================
// �J��
//==========================================================================
void CPlayer::StateFlowering()
{
	// ���[�V�����擾
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	SetMove(0.0f);

	if (pMotion->IsFinish())
	{
		m_state = STATE_AFTERFLOWERING;
	}

	//CManager::GetInstance()->GetCamera()->SetLenDest(4000.0f, 2, 1.0f, 0.015f);
}

//==========================================================================
// �J�Ԍ�
//==========================================================================
void CPlayer::StateAfterFlowering()
{
	MyLib::Vector3 pos = GetPosition();
	pos.y += (100.0f - pos.y) * 0.1f;
	SetPosition(pos);
	CManager::GetInstance()->GetCamera()->SetLenDest(4000.0f, 2, 1.0f, 0.03f);


	MyLib::Vector3 cameraRot = CManager::GetInstance()->GetCamera()->GetRotation();
	cameraRot.z += (-0.84f - cameraRot.z) * 0.03f;
	CManager::GetInstance()->GetCamera()->SetRotation(cameraRot);

}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayer::Draw()
{

	// �`�揈��
	if (m_state == STATE_DMG)
	{
		CObjectChara::Draw(m_mMatcol);
	}
	else if (m_state == STATE_INVINCIBLE || m_state == STATE_FADEOUT)
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
