//=============================================================================
// 
//  �����L���O���� [ranking.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "rankingscore.h"
#include "keyconfig.h"

#include "rankingItem.h"
#include "rankingItem_top3.h"
//=============================================================================
// �萔��`
//=============================================================================
namespace
{
	//�����L���O�̃R���t�B�O
	const float SCROLL_SPEED = -10.0f;
	const float SCROLL_STOP_POS_Y = 600.0f;
}
//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CRankingScore *CRanking::m_pRankingScore = nullptr;	// �����L���O�X�R�A�̃I�u�W�F�N�g
bool CRanking::m_bAllArrival = false;		// �S�ē�����������

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRanking::CRanking()
{
	m_pRankingScore = nullptr;	// �����L���O�X�R�A�̃I�u�W�F�N�g
	m_bAllArrival = false;	// �S�ē�����������
	
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_pRanking[nCnt] = nullptr;
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRanking::~CRanking()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRanking::Init()
{

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// �����L���O�̃X�R�A����
	/*m_pRankingScore = CRankingScore::Create();*/

	//�����t�f�[�^
	int nDate[3] = { 2030,1,1 };

	//TOP3�̃A�C�e�������i���ʁA���ԁA���]�A���t�A��ʒu�A�g�嗦�A�����L���O�X�V�j
	m_pRanking[0] = CRankingItem_top3::Create(1, 0,1,99, 2, nDate, MyLib::Vector3(250.0f, 250.0f, 0.0f), 0.9f, true);
	m_pRanking[1] = CRankingItem_top3::Create(2, 1,12,33, 2, nDate, MyLib::Vector3(250.0f, 390.0f, 0.0f), 0.9f, false);
	m_pRanking[2] = CRankingItem_top3::Create(3, 2,22,22, 3, nDate, MyLib::Vector3(250.0f, 530.0f, 0.0f), 0.9f, false);

	//���ʂ̃A�C�e�������i���ʁA���ԁA���]�A��ʒu�A�g�嗦�A�����L���O�X�V�j
	m_pRanking[3] = CRankingItem::Create(4, 4,44,44, 3, MyLib::Vector3(250.0f, 700.0f, 0.0f), 0.85f, true);
	m_pRanking[4] = CRankingItem::Create(5, 55,55,55, 3, MyLib::Vector3(250.0f, 800.0f, 0.0f), 0.85f, false);
	m_pRanking[5] = CRankingItem::Create(6, 66,66,66, 3, MyLib::Vector3(250.0f, 900.0f, 0.0f), 0.85f, false);
	m_pRanking[6] = CRankingItem::Create(7, 77,77,77, 3, MyLib::Vector3(250.0f, 1000.0f, 0.0f), 0.85f, false);
	m_pRanking[7] = CRankingItem::Create(8, 88,88,88, 3, MyLib::Vector3(250.0f, 1100.0f, 0.0f), 0.85f, false);
	m_pRanking[8] = CRankingItem::Create(9, 99,99,99, 3, MyLib::Vector3(250.0f, 1200.0f, 0.0f), 0.85f, false);
	m_pRanking[9] = CRankingItem::Create(10, 10,10,10, 3, MyLib::Vector3(250.0f, 1300.0f, 0.0f), 0.85f, false);
	

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRanking::Uninit()
{
	// �����L���O�X�R�A�̃I�u�W�F�N�g
	m_pRankingScore = nullptr;

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CRanking::Update()
{
	CManager::GetInstance()->GetDebugProc()->Print(
		"���݂̃��[�h�F�y�����L���O�z\n"
		"�؂�ւ��F�y F �z\n\n");

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// �L�[�R���t�B�O
	CKeyConfigManager* pConfigMgr = CKeyConfigManager::GetInstance();
	CKeyConfig* pPad = pConfigMgr->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKey = pConfigMgr->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) || pPad->GetTrigger(OUTGAME::ACT_OK, 0))
	{
		//�X�N���[���J�n����
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			m_pRanking[nCnt]->SetMove(MyLib::Vector3(0.0f, SCROLL_SPEED, 0.0f));
		}
	}

	//�X�N���[����~����
	MyLib::Vector3 latestpos = m_pRanking[9]->GetPos();
	if (latestpos.y <= SCROLL_STOP_POS_Y)
	{
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			m_pRanking[nCnt]->SetMove(MyLib::Vector3(0.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) || pPad->GetTrigger(OUTGAME::ACT_OK, 0))
		{
			// ���[�h�ݒ�
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		}
	}

	//�X�N���[���X�V����
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_pRanking[nCnt]->Update();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRanking::Draw()
{

}

//==========================================================================
// �����ݒ�ON
//==========================================================================
void CRanking::SetEnableArrival()
{
	m_bAllArrival = true;
}
