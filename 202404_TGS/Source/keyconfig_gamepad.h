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
protected:

	// 情報構造体
	struct KEY
	{

	};

public:
	// コンストラクタ
	CkeyConfigPad() {}

private:

	
};

#endif