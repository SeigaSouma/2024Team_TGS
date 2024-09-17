//======================================================
//
//�L�����N�^�[�\���f�[�^[characterconfig.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "characterconfig.h"
#include "..\file(legacy)\file.h"

//�ÓI�����o�ϐ�
std::list<CCharacterConfig*> CCharacterConfig::m_charaConfigList;

//=================================
//�R���X�g���N�^
//=================================
CCharacterConfig::CCharacterConfig()
{
	m_filePath = "";
}

//=================================
//�f�X�g���N�^
//=================================
CCharacterConfig::~CCharacterConfig()
{
}

//=================================
//�ǂݍ���
//=================================
CCharacterConfig* CCharacterConfig::Load(std::string pPath)
{
	//���łɓǂݍ���ł��Ȃ����m�F
	for (std::list<CCharacterConfig*>::iterator itr = m_charaConfigList.begin(); itr != m_charaConfigList.end(); itr++)
	{
		if ((*itr)->m_filePath == pPath)
		{//���������炱��g����
			return *itr;
		}
	}

	//�Ȃ��̂Ő���
	CCharacterConfig* pCharaConfig = new CCharacterConfig;
	pCharaConfig->m_filePath = pPath;	//�t�@�C���p�X�L��
	TXTFileLoader::LoadMotionViewerFile(pPath, &pCharaConfig->m_charaConfig);
	m_charaConfigList.push_back(pCharaConfig);

	return pCharaConfig;
}

//=================================
//�j��
//=================================
void CCharacterConfig::Unload(void)
{
	//�\���f�[�^���X�g���珜�O
	m_charaConfigList.remove(this);

	//�j��
	delete this;
}

//=================================
//�S�j��
//=================================
void CCharacterConfig::UnloadAll(void)
{
	//�ǂݍ��񂾍\���f�[�^��j��
	for (std::list<CCharacterConfig*>::iterator itr = m_charaConfigList.begin(); itr != m_charaConfigList.end(); itr++)
	{
		(*itr)->Unload();
	}
}
