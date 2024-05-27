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
	CKeyConfig(Control type);

	//=============================
	// メンバ関数
	//=============================
	
	// 静的メンバ関数
	static CKeyConfig* GetKeyConfig(Control type);
	static void Release();
	virtual void Uninit() = 0;
	virtual bool GetPress(const int type) = 0;
	virtual bool GetTrigger(const int type) = 0;
	virtual bool GetRelease(const int type) = 0;
	virtual bool GetRepeat(const int type) = 0;

private:

	// 静的メンバ変数
	static CKeyConfig* m_apKeyConfig[CKeyConfig::Control::CONTROL_MAX];	// 情報格納場所
};

#endif