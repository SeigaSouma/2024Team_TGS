//=============================================================================
//
// �Q�[���p�b�h�L�[�R���t�B�O���� [keyconfig_gamepad.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig_gamepad.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CkeyConfigPad::CkeyConfigPad()
{
	m_Info.clear();
}

//==========================================================================
// �v���X�擾
//==========================================================================
bool CkeyConfigPad::GetPress(const int type)
{
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetPress(m_Info[type], 0);
}

//==========================================================================
// �g���K�[�擾
//==========================================================================
bool CkeyConfigPad::GetTrigger(const int type)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetTrigger(m_Info[type], 0);
}

//==========================================================================
// �����[�X�擾
//==========================================================================
bool CkeyConfigPad::GetRelease(const int type)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetRelease(m_Info[type], 0);
}

//==========================================================================
// ���s�[�g����
//==========================================================================
bool CkeyConfigPad::GetRepeat(const int type)
{
	// �m�F
	if (m_Info.find(type) == m_Info.end()) { return false; }	// ���݂��Ȃ�

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetRepeat(m_Info[type], 0);
}

//==========================================================================
// �I������
//==========================================================================
void CkeyConfigPad::Uninit() {
	m_Info.clear();
}

//==========================================================================
// �L�[���o�^
//==========================================================================
void CkeyConfigPad::Join(const int action, const CInputGamepad::BUTTON key)
{
	m_Info[action] = key;
}