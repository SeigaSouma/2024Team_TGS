//=============================================================================
// 
//  �G�f�B�^�\���� [edit.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit.h"
#include "manager.h"
#include "calculation.h"

// �h���N���X
#include "edit_map.h"
#include "edit_obstacle.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit::CEdit()
{
	// �l�̃N���A
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit::~CEdit()
{

}

//==========================================================================
// ��������
//==========================================================================
CEdit* CEdit::Create(CGame::EditType type)
{
	// �����p�C���X�^���X
	CEdit* pMarker = nullptr;

	switch (type)
	{
	case CGame::EDITTYPE_OFF:
		break;

	case CGame::EDITTYPE_ENEMYBASE:
		break;

	case CGame::EDITTYPE_MAP:
		pMarker = DEBUG_NEW CEdit_Map;
		break;

	case CGame::EditType::EDITTYPE_OBSTACLE:
		pMarker = DEBUG_NEW CEdit_Obstacle;
		break;

	default:
		break;
	}

	if (pMarker != nullptr)
	{
		// ����������
		pMarker->Init();
	}

	return pMarker;
}


//==========================================================================
// �I������
//==========================================================================
void CEdit::Uninit()
{
	delete this;
}