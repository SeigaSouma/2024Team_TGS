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
	const int NUM_RANK = 10;
	const std::string FILE_BIN = "data\\TEXT\\ranking\\ranking.bin";
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
	m_pRankData = nullptr;
	
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

	// �����L���O�f�[�^�i�[�p�\���̂����
	m_pRankData = new SRankdata[NUM_RANK];

	// �t�@�C���ǂݍ���
	Load();

	// �t�@�C���ۑ�
	Save();

	//�����t�f�[�^

	for (int i = 0; i < NUM_RANK; i++)
	{
		int nDate[3] = { m_pRankData[i].year,m_pRankData[i].month,m_pRankData[i].day };
		int minutes = m_pRankData[i].minutes;
		int seconds = m_pRankData[i].seconds;
		int milliseconds = m_pRankData[i].milliSeconds;
		int allrank = m_pRankData[i].allrank;
		bool rankin = m_pRankData[i].rankin;

		if (i < 3)
		{
			//TOP3�̃A�C�e�������i���ʁA���ԁA���]�A���t�A��ʒu�A�g�嗦�A�����L���O�X�V�j
			m_pRanking[i] = CRankingItem_top3::Create(i + 1, minutes, seconds, milliseconds, allrank, nDate, MyLib::Vector3(250.0f, 250.0f + i * 140.0f, 0.0f), 0.9f, rankin);
		}
		else
		{
			int j = i - 3;
			//���ʂ̃A�C�e�������i���ʁA���ԁA���]�A��ʒu�A�g�嗦�A�����L���O�X�V�j
			m_pRanking[i] = CRankingItem::Create(i + 1, minutes, seconds, milliseconds, allrank, MyLib::Vector3(250.0f, 700.0f + j * 100.0f, 0.0f), 0.85f, rankin);
		}
	}
	
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RANKING);

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

	// �����L���O���̍폜
	if (m_pRankData != nullptr)
	{
		delete[] m_pRankData;
		m_pRankData = nullptr;
	}

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

	if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) || pPad->GetTrigger(OUTGAME::ACT_OK, 0))
	{
		//�X�N���[���J�n����
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			m_pRanking[nCnt]->SetMove(MyLib::Vector3(0.0f, SCROLL_SPEED, 0.0f));
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

//==========================================================================
// �t�@�C���ǂݍ���
//==========================================================================
void CRanking::Load()
{
	// �t�@�C�����J��
	std::ifstream File(FILE_BIN, std::ios::binary);
	if (!File.is_open()) {

		if (m_pRankData == nullptr)
		return;

		// �������擾
		time_t Time = time(NULL);

		// ���݂̎��������n���Ԃɕϊ�
		std::tm* now = std::localtime(&Time);

		// �N�A���A���A���A���A�b�����ꂼ��int�ɕϊ�
		int year = now->tm_year + 1900;  // �N��1900�N����̌o�ߔN��
		int month = now->tm_mon + 1;     // ����0����n�܂�̂�+1
		int day = now->tm_mday;          // ��

		// �����l������
		for (int i = 0; i < NUM_RANK; i++)
		{
			m_pRankData[i].year = year;
			m_pRankData[i].month = month;
			m_pRankData[i].day = day;
			m_pRankData[i].minutes = 8 + i * 1;
			m_pRankData[i].seconds = i % 6 * 10;
			m_pRankData[i].milliSeconds = 0;
			m_pRankData[i].allrank = 3;
			m_pRankData[i].rankin = false;
		}

		return;
	}


	// �f�[�^�ǂݍ���
	File.read(reinterpret_cast<char*>(m_pRankData), sizeof(SRankdata) * NUM_RANK);

	// �t�@�C�������
	File.close();
}

//==========================================================================
// �ۑ�
//==========================================================================
void CRanking::Save()
{
	// �t�@�C�����J��
	std::ofstream File(FILE_BIN, std::ios::binary);
	if (!File.is_open()) {

		return;
	}

	// �f�[�^�ۑ�
	File.write(reinterpret_cast<const char*>(m_pRankData), sizeof(SRankdata) * NUM_RANK);

	// �t�@�C�������
	File.close();
}