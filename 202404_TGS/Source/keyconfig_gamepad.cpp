//=============================================================================
//
// ゲームパッドキーコンフィグ処理 [keyconfig_gamepad.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig_gamepad.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CkeyConfigPad::CkeyConfigPad()
{
	m_Info.clear();
}

//==========================================================================
// プレス取得
//==========================================================================
bool CkeyConfigPad::GetPress(const int type)
{
	if (m_Info.find(type) == m_Info.end()) { return false; }	// 存在しない

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetPress(m_Info[type], 0);
}

//==========================================================================
// トリガー取得
//==========================================================================
bool CkeyConfigPad::GetTrigger(const int type)
{
	// 確認
	if (m_Info.find(type) == m_Info.end()) { return false; }	// 存在しない

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetTrigger(m_Info[type], 0);
}

//==========================================================================
// リリース取得
//==========================================================================
bool CkeyConfigPad::GetRelease(const int type)
{
	// 確認
	if (m_Info.find(type) == m_Info.end()) { return false; }	// 存在しない

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetRelease(m_Info[type], 0);
}

//==========================================================================
// リピート処理
//==========================================================================
bool CkeyConfigPad::GetRepeat(const int type)
{
	// 確認
	if (m_Info.find(type) == m_Info.end()) { return false; }	// 存在しない

	CInputGamepad* pInputPad = CInputGamepad::GetInstance();
	return pInputPad->GetRepeat(m_Info[type], 0);
}

//==========================================================================
// 終了処理
//==========================================================================
void CkeyConfigPad::Uninit() {
	m_Info.clear();
}

//==========================================================================
// キー情報登録
//==========================================================================
void CkeyConfigPad::Join(const int action, const CInputGamepad::BUTTON key)
{
	m_Info[action] = key;
}