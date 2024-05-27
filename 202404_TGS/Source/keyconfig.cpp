//=============================================================================
//
// キーコンフィグ処理 [keyconfig.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig.h"

//==========================================================================
// 静的メンバ変数
//==========================================================================
CKeyConfig* CKeyConfig::m_apKeyConfig[CKeyConfig::Control::CONTROL_MAX] = {};

//==========================================================================
// コンストラクタ
//==========================================================================
CKeyConfig::CKeyConfig(Control type)
{
	if (type < CONTROL_MAX)	// 列挙範囲内
	{
		m_apKeyConfig[type] = this;
	}
}

//==========================================================================
// 開放
//==========================================================================
void CKeyConfig::Release()
{
	for (int i = 0; i < CONTROL_MAX; i++)
	{
		if (m_apKeyConfig[i] == nullptr) { continue; }	// 使用されていない

		// 廃棄
		m_apKeyConfig[i]->Uninit();
		delete m_apKeyConfig[i];
		m_apKeyConfig[i] = nullptr;
	}
}