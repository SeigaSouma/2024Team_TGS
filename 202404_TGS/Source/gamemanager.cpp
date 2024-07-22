//=============================================================================
// 
// �Q�[���}�l�[�W������ [gamemanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "stage.h"
#include "map.h"
#include "stagecleartext.h"
#include "limitarea.h"
#include "calculation.h"
#include "fog.h"
#include "fade.h"
#include "beforebattle.h"
#include "stagename.h"
#include "timer.h"
#include "input.h"
#include "baggage.h"
#include "course.h"
#include "judgezoneManager.h"
#include "peoplemanager.h"
#include "spline.h"
#include "request_people.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int POINT_WAVECLEAR = 5;		// �E�F�[�u�N���A�̃|�C���g
	const float POSR_Y_APPROACH_COEF = 0.3f;	//�J�������߂Â��ۂ̊���
	const float POSR_Y_PULL_SCREEN_POS = 210.0f;	// �J�����������n�߂�X�N���[�����W
	const float POSR_Y_APPROACH_SCREEN_POS = SCREEN_HEIGHT * 0.5f;	// �J�������߂Â��n�߂�X�N���[�����W
	const float POSR_YDEST_BAGGTOPLAYER_RATIO = 0.4f;	// �ו��ƃv���C���[�����̊����iposRYDest�j

	const int CHANGE_BASEPOINT[] =	// �|�C���g�ύX����
	{
		40,	// AAA
		20,	// BBB
		10,	// CCC
		1,	// DDD
	};
}

namespace SceneTime
{
	const float RequestStart = 3.5f;	// �˗��J�n
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGameManager::CGameManager()
{
	// �l�̃N���A
	m_SceneType = SCENE_MAIN;	// �V�[���̎��
	m_OldSceneType = SCENE_MAIN;	// �V�[���̎��
	m_bEndRush = false;			// ���b�V�����I��������
	m_bControll = false;		// ����ł��邩
	m_bEndNormalStage = false;	// �ʏ�X�e�[�W���I��������
	m_bGameStart = false;		// �Q�[���J�n���̃t���O
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_nNumStage = 0;			// �X�e�[�W�̑���
	m_nEvaluationPoint = 0;		// �]���|�C���g
	m_fCameraLengthOld = 0;		// �O�̃J�����̋���
	m_fPosRY = 0.0f;
	m_fSceneTimer = 0.0f;		// �V�[���^�C�}�[
	m_pRequestPeople = nullptr;	// �˗��l�̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CGameManager* CGameManager::Create(CScene::MODE mode)
{
	// �������m��
	CGameManager* pManager = nullptr;

	// �C���X�^���X����
	switch (mode)
	{
	case CScene::MODE_GAME:
		pManager = DEBUG_NEW CGameManager;
		break;

	}

	if (pManager != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ����������
		HRESULT hr = pManager->Init();

		if (FAILED(hr))
		{// ���s���Ă�����
			return nullptr;
		}
	}

	return pManager;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGameManager::Init()
{
	m_bControll = true;			// ����ł��邩
	m_bEndNormalStage = false;	// �ʏ�X�e�[�W���I��������
	m_bGameStart = false;		// �Q�[���J�n���̃t���O

#if _DEBUG
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_SceneType = SceneType::SCENE_START;	// �V�[���̎�� 
#else
	m_nNowStage = 0;			// ���݂̃X�e�[�W
	m_SceneType = SceneType::SCENE_START;	// �V�[���̎�� 
#endif

	m_OldSceneType = m_SceneType;

	// �˗��l����
	m_pRequestPeople = CRequestPeople::Create(MyLib::Vector3(500.0f, 0.0f, 900.0f));
	m_pRequestPeople->SetState(CRequestPeople::STATE::STATE_WAIT);

	// ���t���[���C��
	CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_INCOMPLETION);

	// �J�������[�V�����Đ�
	CManager::GetInstance()->GetCamera()->GetCameraMotion()->SetMotion(CCameraMotion::MOTION::MOTION_PASS, CCameraMotion::EASING::Linear);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGameManager::Uninit()
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CGameManager::Update()
{
	
	// ������
	m_fSceneTimer += CManager::GetInstance()->GetDeltaTime();		// �V�[���^�C�}�[
	switch (m_SceneType)
	{
	case CGameManager::SceneType::SCENE_MAIN:
		m_bControll = true;
		CheckJudgeZone();
		ContainPlayerBaggage();
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_START:
		m_bControll = false;
		SceneStart();
		break;

	case CGameManager::SceneType::SCENE_MAINCLEAR:
		m_bControll = false;
		SceneGoal();
		break;

	case SceneType::SCENE_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_DURING_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_BEFOREBATTLE:
		m_bControll = false;
		break;

	case SceneType::SCENE_BATTLESTART:
		m_bControll = false;
		break;

	case SceneType::SCENE_SKILLTREE:	// �X�L���c���[
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_BOSS:
		m_bControll = true;
		break;

	case CGameManager::SceneType::SCENE_TRANSITIONWAIT:
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_TRANSITION:
		m_bControll = false;
		SceneTransition();
		break;

	case SceneType::SCENE_RESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_WAIT_AIRPUSH:
		m_bControll = false;
		SceneWaitAirPush();
		break;

	case SceneType::SCENE_DEBUG:
		m_bControll = true;
		break;

	default:
		break;
	}


	for (int i = 0; i < CJudge::JUDGE::JUDGE_MAX; i++)
	{
		if (m_nEvaluationPoint >= CHANGE_BASEPOINT[i])
		{
			CPeopleManager::GetInstance()->SetRank(static_cast<CJudge::JUDGE>(i));
			break;
		}

		CPeopleManager::GetInstance()->SetRank(CJudge::JUDGE::JUDGE_MAX);
	}


	// �e�L�X�g�̕`��
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- �Q�[���}�l�[�W����� ----------------\n"
		"�y���[�h�z[%d]\n"
		"�y�X�e�[�W�z[%d]\n"
		"�y�]���|�C���g�z[%d]\n"
		, m_SceneType, m_nNowStage, m_nEvaluationPoint);
}

//==========================================================================
// �Q�[���N���A���̐ݒ�
//==========================================================================
void CGameManager::GameClearSettings()
{
	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = playerList.GetData(0);


}

//==========================================================================
// �J�n���o
//==========================================================================
void CGameManager::SceneStart()
{
	if (m_fSceneTimer >= SceneTime::RequestStart &&
		m_pRequestPeople->GetState() == CRequestPeople::STATE::STATE_WAIT)
	{
		m_pRequestPeople->SetState(CRequestPeople::STATE::STATE_PASS);
	}
}

//==========================================================================
// �Q�[�����U���g�̐ݒ�
//==========================================================================
void CGameManager::GameResultSettings()
{

	// �N���A�e�L�X�g����
	CStageClearText::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f));


}

//==========================================================================
// ���C���J�ڒ�
//==========================================================================
void CGameManager::SceneTransition()
{
	// �J�ڂȂ��t�F�[�h�̏�Ԏ擾
	CInstantFade::STATE fadestate = CManager::GetInstance()->GetInstantFade()->GetState();

	if (fadestate == CInstantFade::STATE_FADECOMPLETION ||
		!m_bGameStart)
	{// ���������u��

		// BGM�X�g�b�v
		CSound::GetInstance()->StopSound();
		CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_GAME);

		// �Q�[���J�n���̃t���O
		m_bGameStart = true;

		// �J�����擾
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera != nullptr)
		{
			pCamera->SetEnableFollow(true);
		}

		// �G���A�������擾
		CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
		CLimitArea* pLimitArea = nullptr;
		while (limitareaList.ListLoop(&pLimitArea))
		{
			pLimitArea->Kill();
		}

		// �G�t�F�N�g�S�Ē�~
		CMyEffekseer::GetInstance()->StopAll();
		CMyEffekseer::GetInstance()->StopAll();

		// �t�H�O���Z�b�g
		MyFog::ToggleFogFrag(false);

		// �v���C���[�擾
		CListManager<CPlayer> playerList = CPlayer::GetListObj();
		CPlayer* pPlayer = playerList.GetData(0);

		// �G�z�u
		SetEnemy();

		// �퓬�����ɑJ��
		CBeforeBattle::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f));
		SetType(SceneType::SCENE_BEFOREBATTLE);
	}
}

//==========================================================================
// ��C����҂�
//==========================================================================
void CGameManager::SceneWaitAirPush()
{
	CTimer* pTimer = CGame::GetInstance()->GetTimer();
	CKeyConfig* pConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	if (pTimer != nullptr &&
		pConfigPad->GetTrigger(INGAME::ACT_AIR) ||
		CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN))
	{
		pTimer->SetEnableAddTime(true);
		SetType(SceneType::SCENE_MAIN);
	}
	else if (pTimer != nullptr)
	{
		pTimer->SetEnableAddTime(false);
	}
}

//==========================================================================
// �S�[�����
//==========================================================================
void CGameManager::SceneGoal()
{
	CGame::GetInstance()->GetTimer()->SetState(CTimer::eState::STATE_GOAL);
}

//==========================================================================
// �v���C���[�Ɖו�����ʓ��Ɏ��߂�
//==========================================================================
void CGameManager::ContainPlayerBaggage()
{
	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	// �ו��擾
	CListManager<CBaggage> baggageList = CBaggage::GetListObj();
	CBaggage* pBaggage = nullptr;
	baggageList.ListLoop(&pBaggage);

	// ���߂���̂̍��W
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	
	// posR���݂�Y���W�擾
	m_fPosRY = pCamera->GetPositionR().y;

	// �͈͓��Ɏ��߂�
	MyLib::Vector3 screenPos = pCamera->GetScreenPos(posBaggage);

	if (screenPos.y <= POSR_Y_PULL_SCREEN_POS)
	{// ���ʊO�i�����j
		float posRYDest = (posBaggage.y - posPlayer.y) * POSR_YDEST_BAGGTOPLAYER_RATIO;
		if (m_fPosRY <= posRYDest)
		{
			m_fPosRY = posRYDest;
		}
	}
	else if (screenPos.y >= POSR_Y_APPROACH_SCREEN_POS)
	{// ����ʊO�i�����j
		float posRYDest = posBaggage.y;
		m_fPosRY += (posRYDest - m_fPosRY) * POSR_Y_APPROACH_COEF;
		m_fPosRY = UtilFunc::Transformation::Clamp(m_fPosRY, 200.0f, FLT_MAX);
	}

	pCamera->SetAutoMovingPosR(MyLib::Vector3(posBaggage.x, m_fPosRY, posBaggage.z));
}

//==========================================================================
// �J��������ɉ��������悤�ɂ���
//==========================================================================
void CGameManager::TurnAway()
{
	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	// �ړ���������p�x�Z�o
	float moveLength = pPlayer->GetMoveLength();
	MyLib::Vector3 posDest = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), moveLength + 1.0f);
	
	float angleXZ = pPlayer->GetPosition().AngleXZ(posDest);
	angleXZ += (D3DX_PI * 0.5f);
	UtilFunc::Transformation::RotNormalize(angleXZ);

	// �p�x�ݒ�
	MyLib::Vector3 rot = pCamera->GetRotation();
	pCamera->SetRotation(MyLib::Vector3(rot.x, angleXZ + pCamera->GetOriginRotation().y, rot.z));
}

//==========================================================================
// ����]�[���m�F
//==========================================================================
void CGameManager::CheckJudgeZone()
{
	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	// �R�[�X�擾
	CCourse* pCource = CGame::GetInstance()->GetCourse();

	// �i�s�x�v�Z���Ċm�F
	float progress = pPlayer->GetMoveLength() / pCource->GetCourceLength();
	CManager::GetInstance()->GetDebugProc()->Print("�i�s�x:%f\n", progress);
	CJudgeZoneManager::GetInstance()->Check(progress);
}

//==========================================================================
// �{�X�ݒ�
//==========================================================================
void CGameManager::SetBoss()
{

}

//==========================================================================
// �G�ݒ�
//==========================================================================
void CGameManager::SetEnemy()
{

	// �X�e�[�W�؂�ւ�
	CGame::GetInstance()->GetStage()->ChangeStage("data\\TEXT\\stage\\info.txt");

	// �}�b�v�؂�ւ�
	MyMap::ChangeMap("data\\TEXT\\map\\info.txt");

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// ���X�g���[�v
	while (playerList.ListLoop(&pPlayer))
	{
		// �ʒu�ݒ�
		pPlayer->SetPosition(MyLib::Vector3(0.0f, 0.0f, -1000.0f));
		pPlayer->SetRotation(MyLib::Vector3(0.0f, D3DX_PI, 0.0f));
		pPlayer->SetRotDest(D3DX_PI);
	}

	// �J�����̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->Reset(CScene::MODE_GAME);
	}

	// ��ސݒ�
	m_SceneType = SCENE_MAIN;
}

//==========================================================================
// �X�e�[�W�̑����ݒ�
//==========================================================================
void CGameManager::SetNumStage(int nStage)
{ 
	m_nNumStage = nStage;
}

//==========================================================================
// �X�e�[�W�̉��Z
//==========================================================================
void CGameManager::AddNowStage()
{
	// ���Z
	m_nNowStage++;

	if (m_nNumStage <= m_nNowStage)
	{// ���X�e�[�W���𒴂�����

		// �ʏ�X�e�[�W���I������
		m_bEndNormalStage = true;
	}

}

//==========================================================================
// ���݂̃X�e�[�W�擾
//==========================================================================
int CGameManager::GetNowStage()
{
	return m_nNowStage;
}

//==========================================================================
// �V�[���̎�ސݒ�
//==========================================================================
void CGameManager::SetType(SceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
}

//==========================================================================
// �V�[���̎�ގ擾
//==========================================================================
CGameManager::SceneType CGameManager::GetType()
{
	return m_SceneType;
}

