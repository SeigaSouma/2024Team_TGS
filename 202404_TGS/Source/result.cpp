//=============================================================================
// 
//  ���U���g���� [result.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "result.h"
#include "resultManager.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "game.h"

#include "timer.h"
#include "clearrank.h"
#include "toatalrank.h"
#include "scroll.h"

//=============================================================================
// �萔��`
//=============================================================================
namespace
{
	const D3DXCOLOR WIN_COLOR = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	const D3DXCOLOR LOSE_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const float START_LENGTH = 300.0f; // ��������
	const float END_LENGTH = 2000.0f; // �ڕW����

	//���f���̔z�u�ʒu
	const MyLib::Vector3 CENTER = MyLib::Vector3(0.0f, 0.0f, -2000.0f);
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CResultScore *CResult::m_pResultScore = nullptr;	// ���U���g�X�N���[���̃I�u�W�F�N�g
bool CResult::m_bAllArrival = false;		// �S�ē�����������

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResult::CResult() : m_clear(false)
{
	// �l�̃N���A
	m_pTimer = nullptr;	// �^�C�}�[�̃I�u�W�F�N�g
	m_bAllArrival = false;	// �S�ē�����������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResult::~CResult()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CResult::Init()
{
	//�v���C���[�������Z�b�g
	CManager::GetInstance()->SetNumPlayer(0);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RESULT);

	// ���U���g���

	// ������
	CScroll::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), 1.0f, 350.0f, 1000.0f, true, 0);

	// ���U���g�}�l�[�W��
	CResultManager* pResultManager = CResultManager::GetInstance();

	// �^�C�}�[
	m_pTimer = CTimer::Create(CTimer::Type::TYPE_RESULT);
	m_pTimer->SetTime(pResultManager->GetClearTime());

	// �N���A�����N
	CClearRank::Create(pResultManager->GetJudgeRank());

	// �g�[�^�������N
	CToatalRank::Create(pResultManager->GetJudgeRank(), pResultManager->GetClearTime());


	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CResult::Uninit()
{
	m_pResultScore = nullptr;

	// �^�C�}�[�̔j��
	if (m_pTimer != nullptr)
	{
		// �I������
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CResult::Update()
{
	

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// ��ʑJ��
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		if (m_bAllArrival)
		{
			// ���[�h�ݒ�
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		}

		if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE)
		{
			// �S�Ă̓�������
			if (m_pResultScore != nullptr)
			{
			}
			m_bAllArrival = true;
		}
	}

	// �^�C�}�[�X�V
	m_pTimer->Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CResult::Draw()
{

}

//==========================================================================
// ���U���g�X�N���[���̏��擾
//==========================================================================
CResultScore *CResult::GetResultScore()
{
	return m_pResultScore;
}

//==========================================================================
// �����ݒ�ON
//==========================================================================
void CResult::SetEnableArrival()
{
	m_bAllArrival = true;
}
