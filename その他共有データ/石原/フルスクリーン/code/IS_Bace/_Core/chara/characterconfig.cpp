//======================================================
//
//キャラクター構成データ[characterconfig.cpp]
//Author:石原颯馬
//
//======================================================
#include "characterconfig.h"
#include "..\file(legacy)\file.h"

//静的メンバ変数
std::list<CCharacterConfig*> CCharacterConfig::m_charaConfigList;

//=================================
//コンストラクタ
//=================================
CCharacterConfig::CCharacterConfig()
{
	m_filePath = "";
}

//=================================
//デストラクタ
//=================================
CCharacterConfig::~CCharacterConfig()
{
}

//=================================
//読み込み
//=================================
CCharacterConfig* CCharacterConfig::Load(std::string pPath)
{
	//すでに読み込んでいないか確認
	for (std::list<CCharacterConfig*>::iterator itr = m_charaConfigList.begin(); itr != m_charaConfigList.end(); itr++)
	{
		if ((*itr)->m_filePath == pPath)
		{//あったからこれ使って
			return *itr;
		}
	}

	//ないので生成
	CCharacterConfig* pCharaConfig = new CCharacterConfig;
	pCharaConfig->m_filePath = pPath;	//ファイルパス記憶
	TXTFileLoader::LoadMotionViewerFile(pPath, &pCharaConfig->m_charaConfig);
	m_charaConfigList.push_back(pCharaConfig);

	return pCharaConfig;
}

//=================================
//破棄
//=================================
void CCharacterConfig::Unload(void)
{
	//構成データリストから除外
	m_charaConfigList.remove(this);

	//破棄
	delete this;
}

//=================================
//全破棄
//=================================
void CCharacterConfig::UnloadAll(void)
{
	//読み込んだ構成データを破棄
	for (std::list<CCharacterConfig*>::iterator itr = m_charaConfigList.begin(); itr != m_charaConfigList.end(); itr++)
	{
		(*itr)->Unload();
	}
}
