//======================================================
//
//デバイスロストなどで破棄が必要なものマネージャー[lostrssmanager.cpp]
//Author:石原颯馬
//
//======================================================
#include "lostrssmanager.h"

CLostResourceManager* CLostResourceManager::m_pInstance = nullptr;
//=================================
//コンストラクタ
//=================================
CLostResourceManager::CLostResourceManager()
{
	m_lostResources.clear();
}

//=================================
//デストラクタ
//=================================
CLostResourceManager::~CLostResourceManager()
{
	
}

//=================================
//インスタンス取得（なければ生成）
//=================================
CLostResourceManager* CLostResourceManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CLostResourceManager;
	}
	return m_pInstance;
}

//=================================
//インスタンス破棄
//=================================
void CLostResourceManager::Release()
{
	if (m_pInstance != nullptr)
	{
		m_pInstance->m_lostResources.clear();	//リスト空にする
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

//=================================
//ロストするもの登録
//ヘッダで済んじゃった。
//=================================

//=================================
//ロストするもの削除
//=================================
void CLostResourceManager::Remove(ILostResource* lostrss)
{
	m_lostResources.erase(std::remove(m_lostResources.begin(), m_lostResources.end(), lostrss));
}

//=================================
//ロストするものいったんバックアップ
//=================================
void CLostResourceManager::Backup()
{
	for (auto itr = m_lostResources.begin(); itr != m_lostResources.end(); itr++)
	{
		(*itr)->Backup();
	}
}

//=================================
//ロストするもの復元
//=================================
void CLostResourceManager::Restore()
{
	for (auto itr = m_lostResources.begin(); itr != m_lostResources.end(); itr++)
	{
		(*itr)->Restore();
	}
}
