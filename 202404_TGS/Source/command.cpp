//=============================================================================
//
// �R�}���h���� [command.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "command.h"
#include "input_gamepad.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCommand::CCommand()
{
	// �l�̃N���A
	m_nNowFlame = 0;
	m_nMaxFlame = 0;
	m_Info.clear();
}

//=============================================================================
// �I������
//=============================================================================
void CCommand::Uninit()
{
	m_Info.clear();
}

//=============================================================================
// �R�}���h�ݒ�
//=============================================================================
void CCommand::SetCommand(int nKey)
{
	m_Info[nKey] = false;	// �o�^
}

//=============================================================================
// ���݂̃t���[������ݒ�
//=============================================================================
void CCommand::SetNowFlame(int nFlame)
{
	m_nNowFlame = nFlame;

	if (m_nNowFlame < 0 || m_nNowFlame >= m_nMaxFlame) Reset();	// �ő�t���[���𒴂���
}

//=============================================================================
// ���͏󋵏�����
//=============================================================================
void CCommand::Reset()
{
	// �t���O�����Z�b�g
	for (auto it = m_Info.begin(); it != m_Info.end(); it++) {
		it->second = false;
	}

	m_nNowFlame = 0;
}

//=============================================================================
// 
// �p�b�h�ݒ�
// 
//=============================================================================
// �R�}���h���͊m�F
//=============================================================================
bool CCommandPad::GetCommand()
{
	bool bValue = false;
	DIRECTION direction = GetDirection();
	auto it = m_Info.begin();

	if (it->second) SetNowFlame(GetNowFlame() + 1);	// ��ڂ����͂���Ă�����J�E���g����

	for (it; it != m_Info.end(); it++) {
		if (it->second) continue;	// ���ɓ��͍ς�
		else if (it->first == direction) it->second = true;	// ���͕�������v
		else	Reset();	// ���̓L�[����v���Ȃ�
	}

	if (it == m_Info.end() && it->second) bValue = true;	// �Ō�̃R�}���h�܂œ��͂ł���

	return bValue;	// �R�}���h�������s��Ԃ�
}

//=============================================================================
// ���͕����擾
//=============================================================================
CCommandPad::DIRECTION CCommandPad::GetDirection()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	float rot = pPad->GetStickRotR(0);
	float size = D3DX_PI * 0.125f;

	// �������犄��o��
	if		(rot <= 0.0f + size				&& rot >= 0.0f - size)				return DIRECTION::STICK_UP;
	else if (rot <= -D3DX_PI + size			&& rot >= D3DX_PI - size)			return DIRECTION::STICK_DOWN;
	else if (rot <= -D3DX_PI * 0.5f + size	&& rot >= -D3DX_PI * 0.5f - size)	return DIRECTION::STICK_LEFT;
	else if (rot <= D3DX_PI * 0.5f + size	&& rot >= D3DX_PI * 0.5f - size)	return DIRECTION::STICK_RIGHT;
	else if (rot <= -D3DX_PI * 0.25f + size && rot >= -D3DX_PI * 0.25f - size)	return DIRECTION::STICK_LEFTUP;
	else if (rot <= D3DX_PI * 0.25f + size	&& rot >= D3DX_PI * 0.25f - size)	return DIRECTION::STICK_RIGHTUP;
	else if (rot <= -D3DX_PI * 0.75f + size && rot >= -D3DX_PI * 0.75f - size)	return DIRECTION::STICK_LEFTDOWN;
	else if (rot <= D3DX_PI * 0.75f + size	&& rot >= D3DX_PI * 0.75f - size)	return DIRECTION::STICK_RIGHTDOWN;

	return DIRECTION::STICK_NONE;	// ���͂���Ă��Ȃ��ꍇ
}