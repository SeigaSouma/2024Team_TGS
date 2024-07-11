//=============================================================================
// 
//  �G�̃}�l�[�W������ [peoplemanager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "debugproc.h"
#include "peoplemanager.h"
#include "people.h"
#include "calculation.h"
#include "manager.h"
#include "game.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\people\\peoplemanager.text";
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPeopleManager::CPeopleManager()
{
	// �l�̃N���A
	m_vecPattern.clear();			// �z�u�p�^�[��
	m_vecMotionFileName.clear();	// ���[�V�����t�@�C����
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
	// �������m��
	CPeopleManager* pModel = DEBUG_NEW CPeopleManager;

	if (pModel != nullptr)
	{
		// ����������
		HRESULT hr = pModel->ReadText(FILENAME);
		if (FAILED(hr))
		{
			return nullptr;
		}

		// ����������
		hr = pModel->Init();
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	return pModel;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPeopleManager::Init()
{

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPeopleManager::Uninit()
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CPeopleManager::Update()
{
	
}

//==========================================================================
// �G�z�u
//==========================================================================
void CPeopleManager::SetPeople(MyLib::Vector3 pos, MyLib::Vector3 rot, int nPattern)
{
	SPattern NowPattern = m_vecPattern[nPattern];
	int nNumSpawn = NowPattern.nNumEnemy;	// �X�|�[�����鐔
	CPeople* pEnemy = nullptr;

	for (const auto& data : NowPattern.enemydata)
	{
		// �X�|�[�����̌������|�����킹��
		MyLib::Vector3 spawnPos = pos;

		// ���_�̈ʒu�����Z
		spawnPos += data.pos;

		// �G�̐���
		pEnemy = CPeople::Create(
			m_vecMotionFileName[data.nType],	// �t�@�C����
			spawnPos,							// �ʒu
			(CPeople::TYPE)data.nType);			// ���

		if (pEnemy == nullptr)
		{
			delete pEnemy;
			break;
		}

		// �����ݒ�
		pEnemy->SetRotation(rot);
		pEnemy->SetRotDest(rot.y);
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


		if (line.find("MODEL_FILENAME") != std::string::npos)
		{// MODEL_FILENAME�Ń��f�����ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>		// ��
				hoge >>		// ��
				charaFile;	// ���f���t�@�C����
			continue;
		}

		if (line.find("COLLIDER_FILENAME") != std::string::npos)
		{// COLLIDER_FILENAME�ŃR���C�_�[�ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			//// ���n��
			//lineStream >>
			//	hoge >>
			//	hoge >>			// ��
			//	obstacleInfo.colliderFile;	// �R���C�_�[��


			
			continue;
		}

		//if (line.find("SETUP") != std::string::npos)
		//{// SETUP�ŏ��ǂݍ���

		//	while (line.find("END_SETUP") == std::string::npos)
		//	{
		//		std::getline(File, line);
		//		if (line.find("IS_AIR") != std::string::npos)
		//		{// IS_AIR�ŋ�C�ђʓǂݍ���

		//			// �X�g���[���쐬
		//			std::istringstream lineStream(line);

		//			// ���n��
		//			lineStream >>
		//				hoge >>
		//				hoge >>	// ��
		//				obstacleInfo.setup.isAir;	// ��C�ђ�
		//			continue;
		//		}

		//		if (line.find("IS_MOVE") != std::string::npos)
		//		{// IS_MOVE�œ����ǂݍ���

		//			// �X�g���[���쐬
		//			std::istringstream lineStream(line);

		//			// ���n��
		//			lineStream >>
		//				hoge >>
		//				hoge >>	// ��
		//				obstacleInfo.setup.isMove;	// ����
		//			continue;
		//		}
		//	}
		//}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}


	// �t�@�C�������
	File.close();
	return S_OK;
}
