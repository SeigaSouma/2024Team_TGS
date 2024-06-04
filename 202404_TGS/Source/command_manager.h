//=============================================================================
//
// �R�}���h�}�l�[�W���[�w�b�_�[ [command_manager.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _COMMAND_MANAGER_H_
#define _COMMAND_MANAGER_H_	// ��d�C���N���[�h�h�~

#include "command.h"

//==========================================================================
// �N���X��`
//==========================================================================
class CCommandManager
{
public:
	

private:

	// �R���X�g���N�^
	CCommandManager();
	virtual ~CCommandManager() {}

public:

	// �����o�֐�
	void Uninit();

	// �ÓI�����o�֐�
	static CCommandManager* GetInstance() { return m_pInstance; }
	static CCommandManager* Create();

private:

	std::vector<CCommand> m_CommandList;	// �R�}���h���X�g
	static CCommandManager* m_pInstance;	// �C���X�^���X
};


#endif
