//=============================================================================
//
// キーコンフィグ処理 [keyconfig.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CKeyConfigManager* CKeyConfigManager::m_pInstance = nullptr;

//==========================================================================
// コンストラクタ
//==========================================================================
CKeyConfigManager::CKeyConfigManager()
{
	// 値のクリア
	for (int i = 0; i < Control::CONTROL_MAX; i++) {
		m_apKeyConfig[i] = nullptr;
	}
}

//==========================================================================
// インスタンス生成
//==========================================================================
CKeyConfigManager* CKeyConfigManager::Create()
{
	if (m_pInstance != nullptr) { return m_pInstance; }	// インスタンスが存在
	m_pInstance = new CKeyConfigManager;
	return m_pInstance;
}

//==========================================================================
// 終了処理
//==========================================================================
void CKeyConfigManager::Uninit()
{
	for (int i = 0; i < CONTROL_MAX; i++)
	{
		if (m_apKeyConfig[i] == nullptr) { continue; }	// 使用されていない

		// 廃棄
		m_apKeyConfig[i]->Uninit();
		delete m_apKeyConfig[i];
		m_apKeyConfig[i] = nullptr;
	}

	if (m_pInstance == nullptr) { return; }	// インスタンスが存在しない

	// 終了処理
	delete m_pInstance;
	m_pInstance = nullptr;
}

//==========================================================================
// 内部に登録
//==========================================================================
bool CKeyConfigManager::Bind(CKeyConfig* pConfig, const int type)
{
	// エラーチェック
	if (type >= Control::CONTROL_MAX || type < Control::CONTROL_INPAD) { return false; }	// 範囲外
	if (m_apKeyConfig[type] != nullptr) { return false; }	// すでに登録済み

	m_apKeyConfig[type] = pConfig;	// 登録

	return true;
}