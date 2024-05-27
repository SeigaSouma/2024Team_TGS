//=============================================================================
//
// ゲームパッドキーコンフィグヘッダー [keyconfig_gamepad.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_GAMEPAD_H_
#define _KEYCONFIG_GAMEPAD_H_	// 二重インクルード防止

#include "keyconfig.h"
#include "input_gamepad.h"

//==========================================================================
// クラス定義
//==========================================================================
// キーコンフィグ
class CkeyConfigPad: public CKeyConfig
{
public:

	// コンストラクタ
	CkeyConfigPad(Control type);

	// メンバ関数
	bool GetPress(const int type);
	bool GetTrigger(const int type);
	bool GetRelease(const int type);
	bool GetRepeat(const int type);
	void Uninit() override;

public:
	void Join(const int action, const CInputGamepad::BUTTON key);

private:

	// メンバ変数
	std::map<int, CInputGamepad::BUTTON> m_Info;
};

#endif