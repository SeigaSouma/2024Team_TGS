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
	const float SCROLL_SPEED = -10.0f;	// �X�N���[�����x
	const float SCROLL_STOP_POS_Y = 600.0f;	// ��~���W
	const int NUM_RANK = 10;	// �����L���O��
	const int NUM_ALLRANK = 4;	// ���]�����N��
	const std::string FILE_BIN = "data\\TEXT\\ranking\\ranking.bin";	// �����L���O�f�[�^�ۑ��t�@�C��
}
//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CRankingScore *CRanking::m_pRankingScore = nullptr;	// �����L���O�X�R�A�̃I�u�W�F�N�g
bool CRanking::m_bAllArrival = false;		// �S�ē�����������
CRanking::SRankdata CRanking::m_NowData = CRanking::SRankdata(0, 0, 0, 0, 0, 0, NUM_ALLRANK);

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

	// �\�[�g
	Sort();

	// �����N�C���m�F
	RankIn();

	// �t�@�C���ۑ�
	Save();

	// �����L���O��������
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

	// ���Z�b�g
	m_NowData = SRankdata();
	m_NowData.allrank = NUM_ALLRANK;

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

		if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) || pPad->GetTrigger(OUTGAME::ACT_OK, 0))
		{
			// ���[�h�ݒ�
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		}
	}
	else
	{
		if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) || pPad->GetTrigger(OUTGAME::ACT_OK, 0))
		{
			//�X�N���[���J�n����
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				m_pRanking[nCnt]->SetMove(MyLib::Vector3(0.0f, SCROLL_SPEED, 0.0f));
			}
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
			m_pRankData[i].minutes = 8 + (NUM_RANK - i) * 1;
			m_pRankData[i].seconds = i % 6 * 10;
			m_pRankData[i].milliSeconds = 0;
			m_pRankData[i].allrank = i % 3 + 1;
			m_pRankData[i].rankin = false;
		}

		return;
	}


	// �f�[�^�ǂݍ���
	File.read(reinterpret_cast<char*>(m_pRankData), sizeof(SRankdata) * NUM_RANK);

	// �t�@�C�������
	File.close();

	// �t���O�I�t
	for (int i = 0; i < NUM_RANK; i++)
	{
		m_pRankData[i].rankin = false;
	}
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

//==========================================================================
// �\�[�g
//==========================================================================
void CRanking::Sort()
{
	// �����\�[�g(���])
	for (int fst = 0; fst < NUM_RANK - 1; fst++)
	{
		int tempNum = fst;	// ���̈�ԑ傫���ԍ�

		for (int sec = fst + 1; sec < NUM_RANK; sec++)
		{
			SRankdata* pRank = &m_pRankData[sec];

			if (pRank->allrank < m_pRankData[tempNum].allrank)
			{// �l���������ꍇ
				tempNum = sec;	// �������ԍ���ύX
			}
		}

		if (tempNum != fst)
		{// �ύX����ꍇ
			SRankdata temp = m_pRankData[fst];
			m_pRankData[fst] = m_pRankData[tempNum];
			m_pRankData[tempNum] = temp;
		}
	}

	// �����\�[�g(�^�C��)
	for (int i = 0; i < NUM_ALLRANK; i++)
	{
		for (int fst = 0; fst < NUM_RANK - 1; fst++)
		{
			// �������N���ł̂݊m�F����
			if (m_pRankData[fst].allrank != i) { continue; }

			int tempNum = fst;	// ���̈�ԑ傫���ԍ�
			int temptime = (m_pRankData[fst].minutes * 10000.0f) + 
				(m_pRankData[fst].seconds * 100.0f) + 
				m_pRankData[fst].milliSeconds;

			for (int sec = fst + 1; sec < NUM_RANK; sec++)
			{
				// �������N���ł̂ݔ�r
				if (m_pRankData[sec].allrank != i) { continue; }

				SRankdata* pRank = &m_pRankData[sec];
				int time = (pRank->minutes * 10000.0f) +
					(pRank->seconds * 100.0f) +
					pRank->milliSeconds;

				if (time < temptime)
				{// �l���������ꍇ
					tempNum = sec;	// �������ԍ���ύX
					temptime = time;
				}
			}

			if (tempNum != fst)
			{// �ύX����ꍇ
				SRankdata temp = m_pRankData[fst];
				m_pRankData[fst] = m_pRankData[tempNum];
				m_pRankData[tempNum] = temp;
			}
		}
	}
}

//==========================================================================
// �����N�C��
//==========================================================================
void CRanking::RankIn()
{
	// ����̃^�C�����i�[
	int nowallrank = m_NowData.allrank;
	int nowtime = (m_NowData.minutes * 10000.0f) +
		(m_NowData.seconds * 100.0f) +
		m_NowData.milliSeconds;

	// �ŉ��ʂ̃^�C�����i�[
	SRankdata* pRank = &m_pRankData[NUM_RANK - 1];
	int lowestallrank = pRank->allrank;
	int lowesttime = (pRank->minutes * 10000.0f) +
		(pRank->seconds * 100.0f) +
		pRank->milliSeconds;

	// �ŉ��ʂ�胉���N����������͓������N�ȏォ�^�C��������
	if (nowallrank < lowestallrank ||
		(nowallrank <= lowestallrank && nowtime <= lowesttime))
	{
		// �������擾
		time_t Time = time(NULL);

		// ���݂̎��������n���Ԃɕϊ�
		std::tm* now = std::localtime(&Time);

		// �N�A���A���A���A���A�b�����ꂼ��int�ɕϊ�
		int year = now->tm_year + 1900;  // �N��1900�N����̌o�ߔN��
		int month = now->tm_mon + 1;     // ����0����n�܂�̂�+1
		int day = now->tm_mday;          // ��
		m_NowData.year = year;
		m_NowData.month = month;
		m_NowData.day = day;

		m_pRankData[NUM_RANK - 1] = m_NowData;
		m_pRankData[NUM_RANK - 1].rankin = true;

		// �T�C�h�\�[�g
		Sort();
	}
}