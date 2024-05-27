//=============================================================================
//
// キーコンフィグヘッダー [keyconfig.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_H_
#define _KEYCONFIG_H_	// 二重インクルード防止

//==========================================================================
// クラス定義
//==========================================================================
// キーコンフィグ
class CKeyConfig
{
public:

	// 操作種類列挙
	enum Control
	{
		CONTROL_INPAD = 0,	// インゲームパッド
		CONTROL_OUTPAD,		// アウトゲームパッド
		CONTROL_INKEY,		// インゲームキーボード
		CONTROL_OUTKEY,		// アウトゲームキーボード
		CONTROL_MAX
	};

public:
	// コンストラクタ
	CKeyConfig() {}

	//=============================
	// メンバ関数
	//=============================
	
	// 静的メンバ関数
	static CKeyConfig* GetKeyConfig(Control type) { return m_apKeyConfig[type]; }


private:

	static CKeyConfig* m_apKeyConfig[CKeyConfig::Control::CONTROL_MAX];	// 情報格納場所
};

#endif