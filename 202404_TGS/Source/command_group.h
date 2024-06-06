//=============================================================================
//
// �R�}���h�O���[�v�w�b�_�[ [command_group.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _COMMAND_GROUP_H_
#define _COMMAND_GROUP_H_	// ��d�C���N���[�h�h�~

#include "command.h"

//==========================================================================
// �N���X��`
//==========================================================================
class CCommandGroup
{
public:

	// �R���X�g���N�^
	CCommandGroup();
	~CCommandGroup() {}

public:

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void ListIn(CCommand* command);
	static CCommandGroup* Create(const std::string& string);
	int GetSuccess();

	// �ÓI�����o�֐�

private:

	void Load(const std::string& string);

	std::vector<CCommand*> m_CommandList;	// �R�}���h���X�g
};


#endif
