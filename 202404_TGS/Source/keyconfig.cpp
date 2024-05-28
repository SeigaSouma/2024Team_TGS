//=============================================================================
//
// キーコンフィグ処理 [keyconfig.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig.h"
#include "keyconfig_gamepad.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CONFIGFILE[CKeyConfigManager::Control::CONTROL_MAX] =	// ファイル名
	{
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
		"data\\TEXT\\keyconfig\\gamepad_in.txt",
	};
}

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
	m_pInstance->ConfigCreate();	// コンフィグ生成
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

//==========================================================================
// コンフィグ生成
//==========================================================================
void CKeyConfigManager::ConfigCreate()
{
	if (m_pInstance == nullptr) { return; }	// インスタンス無し

	for (int i = 0; i < Control::CONTROL_MAX; i++) {

		CKeyConfig* p = nullptr;
		switch (i) 
		{
		case Control::CONTROL_INPAD:
			p = new CkeyConfigPad;
			break;

		/*case Control::CONTROL_OUTPAD:

			break;

		case Control::CONTROL_INKEY:

			break;

		case Control::CONTROL_OUTKEY:

			break;*/

		default:

			break;
		}

		if (p == nullptr) { continue; }	// 生成失敗
		p->Load(CONFIGFILE[i]);
		if (!m_pInstance->Bind(p, i))	// 登録失敗
		{
			p->Uninit();
			delete p;
		}
	}
}