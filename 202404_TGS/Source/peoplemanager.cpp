//=============================================================================
// 
//  �l�̃}�l�[�W������ [peoplemanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "peoplemanager.h"
#include "people.h"
#include "player.h"
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "debugproc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\people\\manager.txt";
	const float SPAWN_DISTANCE = 600.0f;		// �N�������Ԋu
	const float SPAWN_ALL_LENGTH = 80000.0f;	// �o������S�Ă̒���
	const float SPAWN_MIN_LENGTH = -6000.0f;	// �o������Œ዗���i�v���C���[����̋����j
	const float SPAWN_MAX_LENGTH = 6000.0f;		// �o������ō������i�v���C���[����̋����A�������͌㐶�����X�g�s���j
}
CPeopleManager* CPeopleManager::m_ThisPtr = nullptr;				// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPeopleManager::CPeopleManager()
{
	// �l�̃N���A
	m_Rank = CJudge::JUDGE::JUDGE_MAX;	// ���݂̃����N
	m_OldRank = CJudge::JUDGE::JUDGE_MAX;		// �O��̃����N
	m_vecPattern.clear();			// �z�u�p�^�[��
	m_vecMotionFileName.clear();	// ���[�V�����t�@�C����
	m_PatternByRank;			// �z�u�p�^�[��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPeopleManager::~CPeopleManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CPeopleManager* CPeopleManager::Create()
{
	if (m_ThisPtr != nullptr) return m_ThisPtr;

	// �������m��
	m_ThisPtr = DEBUG_NEW CPeopleManager;

	if (m_ThisPtr != nullptr)
	{
		// ����������
		HRESULT hr = m_ThisPtr->Init();
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPeopleManager::Init()
{
	// �t�@�C���ǂݍ���
	HRESULT hr = ReadText(FILENAME);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPeopleManager::Uninit()
{
	ResetLateSpawn();

	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CPeopleManager::Update()
{
	// �͈͊O�̐l������
	DespawnPeople();

	// �]�����Ƃɐl�����₷
	if (m_Rank != m_OldRank)
	{
		SetByRank();
	}

	// �㐶���̐l���o��
	LateSpawn();

	// �O��̃����N
	m_OldRank = m_Rank;
}

//==========================================================================
// �����N���Ƃ̃Z�b�g����
//==========================================================================
void CPeopleManager::SetByRank()
{
	// �l�̃��X�g�擾
	CListManager<CPeople> listPeople = CPeople::GetListObj();

	// �v���C���[���X�g�擾
	CListManager<CPlayer> listPlayer = CPlayer::GetListObj();

	// �擪��ۑ�
	std::list<CPeople*>::iterator itr = listPeople.GetEnd();
	CPeople* pObj = nullptr;
	CPlayer* pPlayer = (*listPlayer.GetBegin());

	// �S���t�F�[�h�A�E�g
	while (listPeople.ListLoop(itr))
	{
		(*itr)->SetState(CPeople::STATE::STATE_FADEOUT);
	}

	if (m_Rank == CJudge::JUDGE::JUDGE_MAX)
	{// �Œ჉���N
		return;
	}

	// ��o�����X�g���Z�b�g
	m_lateSpawnPeople.clear();

	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 300.0f, 1500.0f);
	MyLib::Vector3 spawnpos = pos;
	MyLib::Vector3 rot = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f);
	int type = 0, patternNum = static_cast<int>(m_PatternByRank[m_Rank].size());
	float playerLen = pPlayer->GetMoveLength();
	float playerPosX = pPlayer->GetPosition().x;

	for (float len = playerLen + SPAWN_MIN_LENGTH; len <= SPAWN_ALL_LENGTH; len += SPAWN_DISTANCE)
	{
		type = rand() % patternNum;

		// �ʒu���Z�b�g
		spawnpos = pos;
		spawnpos.x += len;
		spawnpos.z += UtilFunc::Transformation::Random(-50, 50) * 10.0f;
		spawnpos.z += UtilFunc::Transformation::Random(-50, 50);

		if (len < playerLen + SPAWN_MAX_LENGTH)
		{// �͈͓��Ȃ̂ł����o��
			SetPeople(spawnpos, rot, type);
		}
		else
		{// �͈͊O�Ȃ̂Ō�ŏo��
			m_lateSpawnPeople.push_back(CPeopleManager::SPeopleData(type, spawnpos));
		}
	}
}

//==========================================================================
// �l�P��
//==========================================================================
void CPeopleManager::DespawnPeople()
{
	std::list<CPeople*> despawnList;
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �l�̃��X�g�擾
	CListManager<CPeople> listObjPeople = CPeople::GetListObj();

	// �v���C���[���X�g�擾
	CListManager<CPlayer> listPlayer = CPlayer::GetListObj();
	
	// �擪��ۑ�
	std::list<CPeople*>::iterator itr = listObjPeople.GetEnd();
	CPlayer* pPlayer = (*listPlayer.GetBegin());

	// �����擾�E��苗������O�ɏo����������X�g�ɒǉ�
	float playerPosX = pPlayer->GetPosition().x;
	while (listObjPeople.ListLoop(itr))
	{
		MyLib::Vector3 pos = (*itr)->GetPosition();
		MyLib::Vector3 screenPos = pCamera->GetScreenPos(pos);

		if (pos.x < playerPosX + SPAWN_MIN_LENGTH && screenPos.x < -100.0f)
		{
			despawnList.push_back((*itr));
		}
	}

	// �l������
	for (const auto& peaple : despawnList)
	{
		peaple->Uninit();
	}
}

//==========================================================================
// �㐶�����̐���
//==========================================================================
void CPeopleManager::LateSpawn()
{
	std::vector<SPeopleData> spawnList;

	// �v���C���[���X�g�擾�E�擪��ۑ�
	CListManager<CPlayer> listPlayer = CPlayer::GetListObj();
	CPlayer* pPlayer = (*listPlayer.GetBegin());

	// �o���͈͂ɂ���l�𒊏o
	float playerPosX = pPlayer->GetPosition().x;
	for (const auto& latePeaple : m_lateSpawnPeople)
	{
		float peoplePosX = latePeaple.pos.x;

		if (peoplePosX <= playerPosX + SPAWN_MAX_LENGTH)
		{
			spawnList.push_back(latePeaple);
		}
	}

	// ���o�����l���o�����Č㐶�����X�g����폜
	MyLib::Vector3 rot = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f);
	for (const auto& spawnPeaple : spawnList)
	{
		SetPeople(spawnPeaple.pos, rot, spawnPeaple.nType);
		m_lateSpawnPeople.remove(spawnPeaple);
	}
}

//==========================================================================
// �l�z�u
//==========================================================================
void CPeopleManager::SetPeople(const MyLib::Vector3& pos, const MyLib::Vector3& rot, int nPattern)
{
	const SPattern& NowPattern = m_PatternByRank[m_Rank][nPattern];
	int nNumSpawn = NowPattern.nNum;	// �X�|�[�����鐔
	CPeople* pPeople = nullptr;

	MyLib::Vector3 spawnPos;

	// �L�����̐�
	int maxType = static_cast<int>(m_vecMotionFileName.size()) - 1;
	for (const auto& data : NowPattern.data)
	{
		// �X�|�[�����̌������|�����킹��
		spawnPos = pos;

		// �X�|�[���ʒu�����Z
		spawnPos += data.pos;

		// ����
		pPeople = CPeople::Create(
			m_vecMotionFileName[UtilFunc::Transformation::Random(0, maxType)],	// �t�@�C����
			spawnPos);							// �ʒu

		// �����ݒ�
		if (pPeople != nullptr)
		{
			pPeople->SetRotation(rot);
			pPeople->SetRotDest(rot.y);
		}
	}
	
}

//==========================================================================
// �O���t�@�C���ǂݍ��ݏ���
//==========================================================================
HRESULT CPeopleManager::ReadText(const std::string& filename)
{
	// �t�@�C�����J��
	std::ifstream File(filename);
	if (!File.is_open()) {
		return E_FAIL;
	}

	// �R�����g�p
	std::string hoge;
	std::string charaFile;
	m_vecMotionFileName.clear();


	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// �X�g���[���쐬
		std::istringstream lineStream(line);


		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// MOTION_FILENAME�Ń��[�V�����t�@�C�����ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>		// ��
				hoge >>		// ��
				charaFile;	// ���f���t�@�C����

			// ���[�V�����t�@�C���ǉ�
			m_vecMotionFileName.push_back(charaFile);
			continue;
		}

		if (line.find("PATTERNSET") != std::string::npos)
		{// PATTERNSET�Ńp�^�[���ǂݍ���

			// �p�^�[���ǉ�
			m_vecPattern.emplace_back();

			int rank = 0;

			while (line.find("END_PATTERNSET") == std::string::npos)
			{
				std::getline(File, line);


				if (line.find("RANK") != std::string::npos)
				{// �L�����̎��

					// �X�g���[���쐬
					std::istringstream lineStream(line);

					// ���n��
					lineStream >>
						hoge >>
						hoge >>	// ��
						rank;	// �����N

					m_vecPattern.back().nRank = rank;
					continue;
				}


				if (line.find("PEOPLESET") != std::string::npos)
				{// PEOPLESET�Ńp�^�[���ǂݍ���

					// �ǂݍ��݃f�[�^
					SPeopleData data;
					data.nType = 0;

					while (line.find("END_PEOPLESET") == std::string::npos)
					{
						std::getline(File, line);

						if (line.find("TYPE") != std::string::npos)
						{// �L�����̎��

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>
								hoge >>	// ��
								data.nType;	// �L�����̎��
							continue;
						}

						if (line.find("POS") != std::string::npos)
						{// POS�ňʒu

							// �X�g���[���쐬
							std::istringstream lineStream(line);

							// ���n��
							lineStream >>
								hoge >>
								hoge >>						// ��
								data.pos.x >> data.pos.y >> data.pos.z;	// �ʒu
							continue;
						}
					}

					// �f�[�^�ǉ�
					m_vecPattern.back().data.push_back(data);

				}// END_PEOPLESET
			}

			// �l�̐�
			m_vecPattern.back().nNum = m_vecPattern.back().data.size();

		}// END_PATTERNSET

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();

	// �����N�ʃp�^�[��
	for (const auto& pattern : m_vecPattern)
	{
		m_PatternByRank[pattern.nRank].push_back(pattern);
	}


	return S_OK;
}
