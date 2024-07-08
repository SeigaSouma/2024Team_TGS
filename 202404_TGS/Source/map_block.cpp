//=============================================================================
// 
//  �}�b�v�u���b�N�Ǘ����� [map_block.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "map_block.h"
#include "checkpoint.h"
#include "map_obstacle.h"
#include "map_obstacleManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\map\\block.txt";
	const int NUM_CHUNK = 5;	// �`�����N�̐�

}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CMapBlock> CMapBlock::m_List = {};
CListManager<CMapBlockInfo> CMapBlock::m_InfoList = {};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMapBlock::CMapBlock()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMapBlock::~CMapBlock()
{

}

//==========================================================================
// ��������
//==========================================================================
CListManager<CMapBlockInfo>* CMapBlock::Load()
{
	if (m_InfoList.GetNumAll() > 0) {
		return &m_InfoList;
	}

	// �t�@�C�����J��
	std::ifstream File(FILENAME);
	if (!File.is_open()) {
		return &m_InfoList;
	}

	// �R�����g�p
	std::string hoge;

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

		if (line.find("BLOCKSET") != std::string::npos)
		{// MODELSET�Ŕz�u���ǂݍ���

			// �������ă��X�g�̊Ǘ�����
			CMapBlockInfo* pBlock = new CMapBlockInfo;
			pBlock->Init();
			pBlock->Load(&File);
			m_InfoList.Regist(pBlock);
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{	// �I������
			break;
		}
	}

	// �t�@�C�������
	File.close();

	return &m_InfoList;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMapBlock::Init()
{
	m_InfoList.Init();
	m_CheckpointList.Init();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMapBlock::Uninit()
{
	// �`�F�b�N�|�C���g�̏I��
	for (int i = 0; i < m_CheckpointList.GetNumAll(); i++)
	{
		m_CheckpointList.GetData(i)->Uninit();
	}

	m_CheckpointList.Uninit();


	// ��Q���̏I��
	for (int i = 0; i < m_ObstacleList.GetNumAll(); i++)
	{
		m_ObstacleList.GetData(i)->Uninit();
	}

	m_ObstacleList.Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CMapBlock::Kill()
{
	// �}�b�v�u���b�N�̏I��
	for (int i = 0; i < m_List.GetNumAll(); i++)
	{
		m_List.GetData(i)->Uninit();
	}

	m_List.Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMapBlock::Update()
{


}

//==========================================================================
// �Z�[�u
//==========================================================================
void CMapBlock::Save()
{
	
}

//==========================================================================
// �z�u�ʒu�ݒ�
//==========================================================================
void CMapBlock::Set(const int Idx, const MyLib::Vector3& startpos, float startlength)
{
	CMapBlockInfo* pInfo = m_InfoList.GetData(Idx);

	// �I�u�W�F�N�g�̔z�u
	{
		CMap_ObstacleManager* pManager = CMap_ObstacleManager::GetInstance();

		for (const auto& it : pInfo->GetObstacleInfo())
		{
			CMap_Obstacle* pObj = CMap_Obstacle::Create(pManager->GetObstacleInfo(it.nType));
			pObj->SetPosition(it.pos + startpos);
			pObj->SetRotation(it.rot);
			pObj->SetScale(it.scale);
		}
	}

	// �`�F�b�N�|�C���g�̔z�u
	{
		for (const auto& it : pInfo->GetCheckpointInfo())
		{
			CCheckpoint* p = CCheckpoint::Create(it + startlength);
			m_CheckpointList.Regist(p);
		}
	}

	m_List.Regist(this);
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMapBlockInfo::CMapBlockInfo()
{
	m_nLevel = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMapBlockInfo::~CMapBlockInfo()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMapBlockInfo::Init()
{
	m_CheckpointList.clear();
	m_ObstacleList.clear();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMapBlockInfo::Uninit()
{
	m_CheckpointList.clear();
	m_ObstacleList.clear();
}

//==========================================================================
// ���[�h
//==========================================================================
void CMapBlockInfo::Load(std::ifstream* pFile)
{
	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;

	while (line.find("END_BLOCKSET") == std::string::npos)
	{
		std::getline(*pFile, line);

		if (line.find("CHECKPOINTSET") != std::string::npos)
		{// �`�F�b�N�|�C���g�z�u

			float length;

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>	// ��
				length;	// �z�u���̎��

			// �������ĊǗ�
			m_CheckpointList.push_back(length);

			continue;
		}

		if (line.find("LEVELSET") != std::string::npos)
		{// ��Փx�ݒ�

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>	// ��
				m_nLevel;	// ��Փx�̐ݒ�

			continue;
		}

		if (line.find("MODELSET") != std::string::npos)
		{// POS�ňʒu

			// ��Q�����ǂݍ���
			ObstacleLoad(pFile);

			continue;
		}
	}
}

//==========================================================================
// ��Q���ǂݍ���
//==========================================================================
void CMapBlockInfo::ObstacleLoad(std::ifstream* pFile)
{
	// �ǂݍ��ݏ��
	int type;
	MyLib::Vector3 pos, rot, scale(1.0f);
	CMap_ObstacleManager* pManager = CMap_ObstacleManager::GetInstance();

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;

	while (line.find("END_MODELSET") == std::string::npos)
	{
		std::getline(*pFile, line);

		if (line.find("TYPE") != std::string::npos)
		{// TYPE�Ŕz�u���̎��

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>	// ��
				type;	// �z�u���̎��
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
				pos.x >> pos.y >> pos.z;	// �ʒu
			continue;
		}

		if (line.find("ROT") != std::string::npos)
		{// ROT�Ō���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>						// ��
				rot.x >> rot.y >> rot.z;	// ����
			continue;
		}

		if (line.find("SCALE") != std::string::npos)
		{// SCALE�Ŋg�嗦

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>						// ��
				scale.x >> scale.y >> scale.z;	// �g�嗦
			continue;
		}
	}

	// ����
	if (type < static_cast<int>(pManager->GetObstacleInfo().size()))
	{
		SObsacleInfo info;
		info.pos = pos;
		info.rot = rot;
		info.scale = scale;
		info.nType = type;
		m_ObstacleList.push_back(info);
	}
}