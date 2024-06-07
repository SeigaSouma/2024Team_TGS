//=============================================================================
//
// �R�}���h�O���[�v���� [command_group.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "command_group.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCommandGroup::CCommandGroup()
{
	m_CommandList.clear();
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCommandGroup::Init()
{
	m_CommandList.clear();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCommandGroup::Uninit()
{
	for (auto it = m_CommandList.begin(); it != m_CommandList.end(); it++) {
		if ((*it) == nullptr) continue;
		(*it)->Uninit();
		delete (*it);
		(*it) = nullptr;
	}

	m_CommandList.clear();
}

//==========================================================================
// �����ɓo�^
//==========================================================================
void CCommandGroup::ListIn(CCommand* command)
{
	m_CommandList.push_back(command);
}

//==========================================================================
// �R���t�B�O����
//==========================================================================
CCommandGroup* CCommandGroup::Create(const std::string& string)
{
	CCommandGroup* pGroup = DEBUG_NEW CCommandGroup;

	if (pGroup != nullptr) {
		pGroup->Load(string);
	}

	return pGroup;
}

//==========================================================================
// �R�}���h�����ԍ����擾
//==========================================================================
int CCommandGroup::GetSuccess()
{
	bool value = false;
	for (auto it = m_CommandList.begin(); it != m_CommandList.end(); it++) {

		if ((*it)->GetCommand()) return (*it)->GetId();
	}

	return -1;
}

//==========================================================================
// �ǂݍ���
//==========================================================================
void CCommandGroup::Load(const std::string& string)
{
	// �t�@�C�����J��
	std::ifstream File(string);
	if (!File.is_open()) {
		return;
	}

	int id = 0;

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

		if (line.find("COMMAND_FILENAME") != std::string::npos)
		{// COMMAND_FILENAME�ŃR�}���h���ǂݍ���
			std::string commandfile;
			commandfile.clear();

			// ���n��
			lineStream >>
				hoge >>
				hoge >>
				commandfile;	// �z�u���̎��

			CCommandPad* pPad = DEBUG_NEW CCommandPad;
			pPad->Load(commandfile);
			pPad->SetId(id);
			ListIn(pPad);
			id++;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}
