//=============================================================================
//
// �L�[�R���t�B�O���� [keyconfig.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig.h"
#include "keyconfig_gamepad.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string CONFIGFILE[CKeyConfigManager::Control::CONTROL_MAX] =	// �t�@�C����
	{
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
	};
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CKeyConfigManager* CKeyConfigManager::m_pInstance = nullptr;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CKeyConfigManager::CKeyConfigManager()
{
	// �l�̃N���A
	for (int i = 0; i < Control::CONTROL_MAX; i++) {
		m_apKeyConfig[i] = nullptr;
	}
}

//==========================================================================
// �C���X�^���X����
//==========================================================================
CKeyConfigManager* CKeyConfigManager::Create()
{
	if (m_pInstance != nullptr) { return m_pInstance; }	// �C���X�^���X������
	m_pInstance = new CKeyConfigManager;
	m_pInstance->ConfigCreate();	// �R���t�B�O����
	return m_pInstance;
}

//==========================================================================
// �I������
//==========================================================================
void CKeyConfigManager::Uninit()
{
	for (int i = 0; i < CONTROL_MAX; i++)
	{
		if (m_apKeyConfig[i] == nullptr) { continue; }	// �g�p����Ă��Ȃ�

		// �p��
		m_apKeyConfig[i]->Uninit();
		delete m_apKeyConfig[i];
		m_apKeyConfig[i] = nullptr;
	}

	if (m_pInstance == nullptr) { return; }	// �C���X�^���X�����݂��Ȃ�

	// �I������
	delete m_pInstance;
	m_pInstance = nullptr;
}

//==========================================================================
// �����ɓo�^
//==========================================================================
bool CKeyConfigManager::Bind(CKeyConfig* pConfig, const int type)
{
	// �G���[�`�F�b�N
	if (type >= Control::CONTROL_MAX || type < Control::CONTROL_INPAD) { return false; }	// �͈͊O
	if (m_apKeyConfig[type] != nullptr) { return false; }	// ���łɓo�^�ς�

	m_apKeyConfig[type] = pConfig;	// �o�^

	return true;
}

//==========================================================================
// �R���t�B�O����
//==========================================================================
void CKeyConfigManager::ConfigCreate()
{
	if (m_pInstance == nullptr) { return; }	// �C���X�^���X����

	for (int i = 0; i < Control::CONTROL_MAX; i++) {

		CKeyConfig* p = nullptr;
		switch (i) 
		{
		case Control::CONTROL_INPAD:
			p = new CkeyConfigPad;
			break;

		/*case Control::CONTROL_OUTPAD:

			break;

		case Control::CONTROL_INKEY:

			break;

		case Control::CONTROL_OUTKEY:

			break;*/

		default:

			break;
		}

		if (p == nullptr) { continue; }	// �������s
		p->Load(CONFIGFILE[i]);
		if (!m_pInstance->Bind(p, i))	// �o�^���s
		{
			p->Uninit();
			delete p;
		}
	}
}