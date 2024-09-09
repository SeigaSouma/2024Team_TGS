//=============================================================================
//
// キーコンフィグ設定ヘッダー [keyconfig_setting.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_SETTING_H_
#define _KEYCONFIG_SETTING_H_	// 二重インクルード防止

#include "keyconfig.h"

// 前方宣言
class CControlKeyDisp;

//==========================================================================
// クラス定義
//==========================================================================
// キーコンフィグ
class CKeyConfigSetting
{
private:

	// キー情報
	struct KEYINFO
	{
		CObject2D* s_p2D;				// 2Dポリゴン
		CObject2D* s_p2DFront;			// 表面
		CControlKeyDisp* s_pKeyDisp;	// キーコンフィグ表示

		// コンストラクタ
		KEYINFO() { s_p2D = nullptr; s_pKeyDisp = nullptr; s_p2DFront = nullptr; }
	};

public:

	// コンストラクタ
	CKeyConfigSetting();
	~CKeyConfigSetting() {}

	//=============================
	// メンバ関数
	//============================= 
	static CKeyConfigSetting* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool IsDeath(void) { return m_bDeath; }
	bool IsChange(void) { return m_bNowChange; }

private:

	// メンバ関数
	void Chenge();

	// メンバ変数
	float m_AlphaSin;
	int m_SelectKey;	// 選択中
	bool m_bNowChange;	// 変更中か否か
	bool m_bDeath;		// 死亡フラグ
	KEYINFO m_apKeyConfig[INGAME::ACT_MAX];
};

#endif