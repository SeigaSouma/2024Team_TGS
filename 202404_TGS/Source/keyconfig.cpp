//=============================================================================
//
// �L�[�R���t�B�O���� [keyconfig.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig.h"

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